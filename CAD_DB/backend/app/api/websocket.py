"""WebSocket connection lifetime, presence and stream selection."""

from datetime import datetime, timezone
from uuid import uuid4

from fastapi import APIRouter, WebSocket, WebSocketDisconnect

from ..config import settings
from ..sync import sync_manager


router = APIRouter()

from ..collaboration.snapshots import _send_latest_model_saved_event
from ..collaboration.protocol import _handle_project_ws_message


@router.websocket("/ws/projects/{project_id}")
async def ws_project_sync(websocket: WebSocket, project_id: str) -> None:
    incoming_ts = datetime.now(timezone.utc).isoformat()
    incoming_query = dict(websocket.query_params)
    stream = str(websocket.query_params.get("stream") or "mode0").lower()
    if stream not in {"mode0", "mode1"} or (
        stream == "mode1" and not settings.direct_json_mode
    ):
        await websocket.close(code=1008)
        return
    has_pw = bool(incoming_query.get("password"))
    print(
        f"[WS] incoming ts={incoming_ts} project_id={project_id} "
        f"client_id={incoming_query.get('client_id')} "
        f"author={incoming_query.get('author')} has_password={has_pw}",
        flush=True,
    )

    if (
        settings.api_password
        and websocket.query_params.get("password") != settings.api_password
    ):
        print(f"[WS] REJECT password mismatch project_id={project_id}", flush=True)
        await websocket.close(code=1008)
        return

    client_id = websocket.query_params.get("client_id") or uuid4().hex
    author = (
        websocket.query_params.get("author") or "anonymous"
    ).strip() or "anonymous"

    await sync_manager.connect(
        project_id, websocket, client_id=client_id, author=author, stream=stream
    )
    print(f"[WS] ACCEPTED project_id={project_id}", flush=True)
    try:
        members = await sync_manager.list_members(project_id)
        await websocket.send_json(
            {
                "type": "presence_snapshot",
                "project_id": project_id,
                "self": {"client_id": client_id, "author": author},
                "members": members,
            }
        )
        await sync_manager.broadcast(
            project_id,
            {
                "type": "collaborator_joined",
                "project_id": project_id,
                "client_id": client_id,
                "author": author,
            },
            exclude_client_id=client_id,
        )

        await _send_latest_model_saved_event(
            project_id, websocket, trigger="snapshot", stream=stream
        )
        while True:
            message = await websocket.receive_text()
            await _handle_project_ws_message(
                websocket, project_id, client_id, author, message, stream
            )
    except WebSocketDisconnect:
        disconnected = await sync_manager.disconnect(project_id, websocket)
        if disconnected is not None:
            await sync_manager.broadcast(
                project_id,
                {
                    "type": "collaborator_left",
                    "project_id": project_id,
                    "client_id": disconnected["client_id"],
                    "author": disconnected["author"],
                },
            )
    print(f"[WS] CLOSED project_id={project_id} client_id={client_id}", flush=True)
