"""/api/v1/events：WS 事件流（V3_PLAN.md §2：版本提交/定版/分支广播）。"""

from fastapi import APIRouter, WebSocket, WebSocketDisconnect

from ...events_hub import hub

router = APIRouter()


@router.websocket("/events")
async def events(websocket: WebSocket) -> None:
    """订阅若干项目的事件；断线重连后以 GET /versions 对账补齐。"""
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
    queue = hub.subscribe(project_ids)
    try:
        await websocket.send_json({"type": "subscribed", "projects": project_ids})
        while True:
            event = await queue.get()
            await websocket.send_json(event)
    except WebSocketDisconnect:
        pass
    finally:
        hub.unsubscribe(queue, project_ids)
