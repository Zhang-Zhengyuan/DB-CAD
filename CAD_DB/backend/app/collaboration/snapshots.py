"""Initial and requested snapshots, separated by collaboration stream."""

from fastapi import HTTPException, WebSocket, status

from .. import crud
from ..json_graph_store import get_json_graph_store


from ..services.events import _model_saved_event, _entity_graph_saved_event


async def _send_latest_model_saved_event(
    project_id: str, websocket: WebSocket, trigger: str, stream: str = "mode0"
) -> None:
    if stream == "mode1":
        json_latest = get_json_graph_store().latest(project_id)
        if json_latest is not None:
            await websocket.send_json(
                {
                    "type": "json_delta_saved",
                    "project_id": project_id,
                    "version": json_latest.version,
                    "author": json_latest.author,
                    "changed_ids": [
                        str(item.get("uuid", item.get("id"))).strip()
                        for item in json_latest.content.get("changes", [])
                        if isinstance(item, dict)
                        and str(item.get("uuid", item.get("id", ""))).strip()
                    ],
                    "removed_ids": json_latest.content.get("removed_ids", []),
                    "graph_hash": json_latest.content.get("graph_hash"),
                    "trigger": trigger,
                }
            )
        return
    try:
        latest = crud.get_latest_version_or_404(project_id)
    except HTTPException as ex:
        if ex.status_code == status.HTTP_404_NOT_FOUND:
            return
        raise

    content = dict(latest.content or {})
    has_eg = isinstance(content, dict) and "entity_graph" in content
    has_delta = isinstance(content, dict) and (
        "delta_bodies" in content or "deleted_uuids" in content
    )
    print(
        f"[_send_latest_model_saved_event] project_id={project_id} latest_v={latest.version} "
        f"content_keys={list(content.keys())} has_entity_graph={has_eg} has_delta={has_delta} "
        f"delta_bodies_size={len(content.get('delta_bodies', [])) if isinstance(content.get('delta_bodies'), list) else 'N/A'} "
        f"deleted_uuids_size={len(content.get('deleted_uuids', [])) if isinstance(content.get('deleted_uuids'), list) else 'N/A'}",
        flush=True,
    )
    print(
        f"[_send_latest_model_saved_event]   routing to: {'entity_graph_saved' if has_eg else 'model_saved'}",
        flush=True,
    )
    # 如果最新版本是用 entity_graph 路径写入的，必须按 entity_graph_saved 事件广播，
    # 否则接收端会按 model_saved 走"清空+restore"路径，丢掉已有的 entity_graph 节点对齐信息。
    if has_eg:
        await websocket.send_json(
            _entity_graph_saved_event(
                project_id,
                latest,
                trigger=trigger,
            )
        )
        return

    await websocket.send_json(
        _model_saved_event(project_id, latest, trigger=trigger, include_content=True)
    )
