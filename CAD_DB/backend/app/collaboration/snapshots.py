"""WebSocket publishes version metadata; HTTP Pull owns reconstruction."""

from fastapi import WebSocket
from ..json_graph_store import get_json_graph_store


async def _send_latest_model_saved_event(
    project_id: str, websocket: WebSocket, trigger: str, stream: str = "mode1"
) -> None:
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
