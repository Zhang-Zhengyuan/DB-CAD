"""Presence/heartbeat and version refresh; commits use the one HTTP JSON route."""

import json
from fastapi import WebSocket
from .snapshots import _send_latest_model_saved_event


async def _handle_project_ws_message(
    websocket: WebSocket,
    project_id: str,
    client_id: str,
    author: str,
    message: str,
    stream: str = "mode1",
) -> None:
    message_type = message.strip().lower()
    if message_type not in {"ping", "sync_now"}:
        try:
            payload = json.loads(message)
            message_type = payload.get("type") if isinstance(payload, dict) else None
        except (TypeError, ValueError):
            message_type = None
    if message_type == "ping":
        await websocket.send_json({"type": "pong", "project_id": project_id})
    elif message_type == "sync_now":
        await _send_latest_model_saved_event(project_id, websocket, "sync_now")
    else:
        await websocket.send_json(
            {
                "type": "error",
                "project_id": project_id,
                "detail": "Use POST /projects/{project_id}/json-delta for commits",
            }
        )
