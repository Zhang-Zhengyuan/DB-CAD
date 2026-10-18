"""Presence/heartbeat, edit intent and version refresh; commits use the one HTTP JSON route."""

import json
from fastapi import WebSocket
from ..sync import sync_manager
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
    elif message_type == "edit_intent":
        # M4.2 在场感知：组件级编辑意图广播（软锁提示）。纯转发，服务端不落库——
        # 提交仍走 json-delta 的 base_version 乐观锁，意图只用于 UI 提示。
        component_uuid = str(payload.get("component_uuid") or "").strip()
        action = str(payload.get("action") or "begin").strip().lower()
        if component_uuid and action in {"begin", "end"}:
            await sync_manager.broadcast(
                project_id,
                {
                    "type": "edit_intent",
                    "project_id": project_id,
                    "client_id": client_id,
                    "author": author,
                    "component_uuid": component_uuid,
                    "action": action,
                },
                exclude_client_id=client_id,
                stream="mode1",
            )
        else:
            await websocket.send_json(
                {
                    "type": "error",
                    "project_id": project_id,
                    "detail": "edit_intent requires component_uuid and action begin|end",
                }
            )
    else:
        await websocket.send_json(
            {
                "type": "error",
                "project_id": project_id,
                "detail": "Use POST /projects/{project_id}/json-delta for commits",
            }
        )
