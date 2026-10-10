"""Serialize version writes before broadcasting; preserve optimistic locking."""

import json
import logging
import time


from .. import crud, schemas
from ..sync import sync_manager


from .events import _model_saved_event, _entity_graph_saved_event

logger = logging.getLogger(__name__)


async def _create_model_version_serialized(
    project_id: str,
    payload: schemas.ModelVersionCreate,
    *,
    trigger: str,
    request_id: str | None = None,
    source_client_id: str | None = None,
    raw_request_json: str | None = None,
):
    request_json = (
        raw_request_json
        if raw_request_json is not None
        else json.dumps(
            payload.model_dump(mode="json"), ensure_ascii=False, separators=(",", ":")
        )
    )
    content = dict(payload.content or {})
    audit = {
        "payload_format": "sat" if content.get("sat") else "json",
        "contains_sat": bool(content.get("sat")),
        "request_bytes": len(request_json.encode("utf-8")),
        "request_json": request_json,
        "sat_bytes": len(str(content.get("sat") or "").encode("utf-8")),
    }
    db_started = time.perf_counter()
    async with sync_manager.write_lock(project_id):
        version = crud.create_model_version(project_id, payload)
        audit["db_write_ms"] = round((time.perf_counter() - db_started) * 1000, 3)
        backend = crud.storage_bridge or crud.direct_store
        save_audit = getattr(backend, "record_version_audit", None)
        if callable(save_audit):
            try:
                save_audit(project_id, version.version, audit)
            except Exception:
                # The model version is already committed. An optional WebUI
                # audit failure must not make the client retry that Push.
                logger.exception(
                    "Failed to record Mode0 Push audit for %s v%s",
                    project_id,
                    version.version,
                )
        # 智能分发：如果这次提交的内容里包含 entity_graph 增量信息，必须按 entity_graph_saved
        # 事件广播给所有接收端，让它们走"ADD/REMOVE/MODIFY 合并"路径；否则一律按 model_saved
        # 广播会污染版本序列，接收端只能"清空+restore"，丢失对齐信息。
        content = dict(version.content or {})
        has_entity_graph = isinstance(content, dict) and "entity_graph" in content
        has_delta = isinstance(content, dict) and (
            "delta_bodies" in content or "deleted_uuids" in content
        )
        print(
            f"[_create_model_version_serialized] project_id={project_id} version={version.version} "
            f"trigger={trigger} request_id={request_id} "
            f"content_keys={list(content.keys())} "
            f"has_entity_graph={has_entity_graph} has_delta={has_delta} "
            f"delta_bodies_size={len(content.get('delta_bodies', [])) if isinstance(content.get('delta_bodies'), list) else 'N/A'} "
            f"deleted_uuids_size={len(content.get('deleted_uuids', [])) if isinstance(content.get('deleted_uuids'), list) else 'N/A'}",
            flush=True,
        )
        broadcast_event = (
            _entity_graph_saved_event(
                project_id,
                version,
                trigger=trigger,
                request_id=request_id,
                source_client_id=source_client_id,
            )
            if has_entity_graph
            else _model_saved_event(
                project_id,
                version,
                trigger=trigger,
                request_id=request_id,
                source_client_id=source_client_id,
                include_content=True,
            )
        )
        print(
            f"[_create_model_version_serialized]   routing to: {'entity_graph_saved' if has_entity_graph else 'model_saved'}",
            flush=True,
        )
        await sync_manager.broadcast(
            project_id,
            broadcast_event,
            exclude_client_id=source_client_id,
            stream="mode0",
        )
    return version
