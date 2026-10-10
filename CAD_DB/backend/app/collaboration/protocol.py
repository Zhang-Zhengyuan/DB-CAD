"""Dispatch messages; transport lifetime and persistence live in other modules."""

import json

from fastapi import WebSocket


from .snapshots import _send_latest_model_saved_event
from .submissions import (
    _handle_submit_model_message,
    _handle_submit_entity_graph_message,
)


async def _handle_project_ws_message(
    websocket: WebSocket,
    project_id: str,
    client_id: str,
    author: str,
    message: str,
    stream: str = "mode0",
) -> None:
    normalized = message.strip()
    lowered = normalized.lower()
    if lowered == "sync_now":
        await _send_latest_model_saved_event(
            project_id, websocket, trigger="sync_now", stream=stream
        )
        return
    if lowered == "ping":
        await websocket.send_json({"type": "pong", "project_id": project_id})
        return

    try:
        data = json.loads(normalized)
    except json.JSONDecodeError:
        await websocket.send_json(
            {
                "type": "error",
                "project_id": project_id,
                "detail": "Unsupported WebSocket message",
            }
        )
        return

    if not isinstance(data, dict):
        await websocket.send_json(
            {
                "type": "error",
                "project_id": project_id,
                "detail": "WebSocket JSON message must be an object",
            }
        )
        return

    message_type = str(data.get("type") or "").strip()
    if message_type == "submit_model":
        if stream != "mode0":
            await websocket.send_json(
                {
                    "type": "error",
                    "project_id": project_id,
                    "detail": "Wrong version stream",
                }
            )
            return
        data["_raw_request_json"] = message
        await _handle_submit_model_message(
            websocket, project_id, client_id, author, data
        )
    elif message_type == "submit_entity_graph":
        if stream != "mode0":
            await websocket.send_json(
                {
                    "type": "error",
                    "project_id": project_id,
                    "detail": "Wrong version stream",
                }
            )
            return
        await _handle_submit_entity_graph_message(
            websocket, project_id, client_id, author, data
        )
    elif message_type == "sync_now":
        await _send_latest_model_saved_event(
            project_id, websocket, trigger="sync_now", stream=stream
        )
    elif message_type == "ping":
        await websocket.send_json({"type": "pong", "project_id": project_id})
    else:
        await websocket.send_json(
            {
                "type": "error",
                "project_id": project_id,
                "detail": f"Unsupported WebSocket message type: {message_type}",
            }
        )
