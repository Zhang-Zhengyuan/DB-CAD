"""/api/v1/events：WS 事件流（V3_PLAN.md §2：版本提交/定版/分支/在场/编辑意图）。

双向通道（P4）：
  客户端 → {"type":"edit_intent","component":"<id>"}     声明编辑意图（null=清除）
  客户端 → {"type":"ping"}                               保活
  服务端 → presence 事件（join/leave，附全量在场名单）
  服务端 → edit_intent 事件（谁在编辑哪个组件，附全量意图表）
断线重连后以 GET /versions 对账补齐丢失事件。
"""

import asyncio

from fastapi import APIRouter, WebSocket, WebSocketDisconnect

from ...events_hub import hub

router = APIRouter()


@router.websocket("/events")
async def events(websocket: WebSocket) -> None:
    await websocket.accept()
    project_ids = [
        value
        for value in (websocket.query_params.get("projects") or "").split(",")
        if value
    ]
    if not project_ids:
        await websocket.send_json({"type": "error", "detail": "projects query param required"})
        await websocket.close()
        return
    queue: asyncio.Queue = asyncio.Queue(maxsize=256)
    user = websocket.query_params.get("user") or "anonymous"
    marker = f"ws-{id(websocket):x}"
    for project_id in project_ids:
        hub.subscribe(project_id, queue)
    try:
        await websocket.send_json({"type": "subscribed", "projects": project_ids})
        for project_id in project_ids:
            users = hub.presence_join(project_id, user, marker)
            hub.publish(project_id, "presence", {"action": "join", "user": user, "users": users})

        async def pump() -> None:
            while True:
                event = await queue.get()
                await websocket.send_json(event)

        pump_task = asyncio.create_task(pump())
        try:
            while True:
                message = await websocket.receive_json()
                kind = message.get("type")
                if kind == "edit_intent":
                    for project_id in project_ids:
                        intents = hub.set_intent(project_id, user, message.get("component"))
                        hub.publish(
                            project_id,
                            "edit_intent",
                            {
                                "user": user,
                                "component": message.get("component"),
                                "intents": intents,
                            },
                        )
        finally:
            pump_task.cancel()
    except WebSocketDisconnect:
        pass
    finally:
        for project_id in project_ids:
            hub.unsubscribe(project_id, queue)
            users = hub.presence_leave(project_id, marker)
            hub.publish(project_id, "presence", {"action": "leave", "user": user, "users": users})
