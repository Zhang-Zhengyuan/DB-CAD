"""Validate WebSocket submissions and report conflicts without overwrites."""

from typing import Any
from uuid import uuid4

from fastapi import HTTPException, WebSocket, status
from pydantic import ValidationError

from .. import crud, schemas
from ..sync import sync_manager


from ..services.events import _entity_graph_saved_event
from ..services.model_versions import _create_model_version_serialized


async def _send_submit_rejected(
    websocket: WebSocket,
    project_id: str,
    *,
    request_id: str | None,
    reason: str,
    detail: str,
) -> None:
    event: dict[str, Any] = {
        "type": "submit_rejected",
        "project_id": project_id,
        "reason": reason,
        "detail": detail,
    }
    if request_id:
        event["request_id"] = request_id

    try:
        latest = crud.get_latest_version_or_404(project_id)
    except HTTPException:
        latest = None

    if latest is not None:
        event.update(
            {
                "latest_version": latest.version,
                "author": latest.author,
                "created_at": latest.created_at.isoformat(),
                "content": latest.content,
            }
        )

    await websocket.send_json(event)


async def _handle_submit_model_message(
    websocket: WebSocket,
    project_id: str,
    client_id: str,
    author: str,
    data: dict[str, Any],
) -> None:
    request_id = str(data.get("request_id") or uuid4().hex)
    message_project_id = str(data.get("project_id") or project_id)
    if message_project_id != project_id:
        await _send_submit_rejected(
            websocket,
            project_id,
            request_id=request_id,
            reason="invalid_project",
            detail="submit_model project_id does not match the WebSocket project",
        )
        return

    payload_author = str(data.get("author") or author).strip() or author
    payload = {
        "author": payload_author,
        "content": data.get("content"),
        "base_version": data.get("base_version"),
    }
    try:
        model_payload = schemas.ModelVersionCreate.model_validate(payload)
    except ValidationError as ex:
        await _send_submit_rejected(
            websocket,
            project_id,
            request_id=request_id,
            reason="invalid_payload",
            detail=str(ex),
        )
        return

    trigger = str(data.get("reason") or "submit_model").strip() or "submit_model"
    try:
        version = await _create_model_version_serialized(
            project_id,
            model_payload,
            trigger=trigger,
            request_id=request_id,
            source_client_id=client_id,
            raw_request_json=data.get("_raw_request_json"),
        )
    except HTTPException as ex:
        await _send_submit_rejected(
            websocket,
            project_id,
            request_id=request_id,
            reason="conflict"
            if ex.status_code == status.HTTP_409_CONFLICT
            else "save_failed",
            detail=str(ex.detail),
        )
        return

    # 返回真实版本号，让客户端在 submit_accepted 里就能更新版本，
    # 不必等待后续的 model_saved 广播（可能导致版本号延迟更新）。
    await websocket.send_json(
        {
            "type": "submit_accepted",
            "project_id": project_id,
            "request_id": request_id,
            "version_model": "model",
            "version": version.version,
        }
    )


async def _handle_submit_entity_graph_message(
    websocket: WebSocket,
    project_id: str,
    client_id: str,
    author: str,
    data: dict[str, Any],
) -> None:
    """处理 client 通过 WebSocket 发来的增量 entity_graph 提交。

    与 submit_model 走的是同一条持久化路径（create_model_version），
    只是后续广播走 entity_graph_saved 事件，让其他 client 用增量
    changes 合并而不是 clear+restore 整体替换。
    """
    request_id = str(data.get("request_id") or uuid4().hex)
    message_project_id = str(data.get("project_id") or project_id)
    if message_project_id != project_id:
        await _send_submit_rejected(
            websocket,
            project_id,
            request_id=request_id,
            reason="invalid_project",
            detail="submit_entity_graph project_id does not match the WebSocket project",
        )
        return

    payload_author = str(data.get("author") or author).strip() or author
    payload = {
        "author": payload_author,
        "content": data.get("content"),
        "base_version": data.get("base_version"),
    }
    try:
        model_payload = schemas.ModelVersionCreate.model_validate(payload)
    except ValidationError as ex:
        await _send_submit_rejected(
            websocket,
            project_id,
            request_id=request_id,
            reason="invalid_payload",
            detail=str(ex),
        )
        return

    trigger = (
        str(data.get("reason") or "submit_entity_graph").strip()
        or "submit_entity_graph"
    )
    content_for_log = dict(data.get("content") or {})
    print(
        f"[fastapi _handle_submit_entity_graph] ENTER project_id={project_id} request_id={request_id} "
        f"base_version_in={model_payload.base_version} "
        f"content_has_entity_graph={'entity_graph' in content_for_log} "
        f"content_keys={list(content_for_log.keys())}",
        flush=True,
    )
    try:
        async with sync_manager.write_lock(project_id):
            version = crud.create_model_version(project_id, model_payload)
            print(
                f"[fastapi _handle_submit_entity_graph] CREATED v={version.version} project_id={project_id} request_id={request_id}",
                flush=True,
            )
            await sync_manager.broadcast(
                project_id,
                _entity_graph_saved_event(
                    project_id,
                    version,
                    trigger=trigger,
                    request_id=request_id,
                    source_client_id=client_id,
                ),
                exclude_client_id=client_id,
                stream="mode0",
            )
            print(
                f"[fastapi _handle_submit_entity_graph] BROADCAST entity_graph_saved v={version.version} project_id={project_id} (exclude {client_id})",
                flush=True,
            )
    except HTTPException as ex:
        print(
            f"[fastapi _handle_submit_entity_graph] HTTPException status={ex.status_code} detail={ex.detail} project_id={project_id} request_id={request_id}",
            flush=True,
        )
        await _send_submit_rejected(
            websocket,
            project_id,
            request_id=request_id,
            reason="conflict"
            if ex.status_code == status.HTTP_409_CONFLICT
            else "save_failed",
            detail=str(ex.detail),
        )
        return

    print(
        f"[fastapi _handle_submit_entity_graph] SENDING submit_accepted v={version.version} to client_id={client_id}",
        flush=True,
    )
    await websocket.send_json(
        {
            "type": "submit_accepted",
            "project_id": project_id,
            "request_id": request_id,
            "version_model": "entity_graph",
            "version": version.version,
        }
    )
