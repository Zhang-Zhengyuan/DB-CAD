import asyncio
import json
from collections import defaultdict
from typing import Any

from fastapi import WebSocket


class ProjectSyncManager:
    def __init__(self) -> None:
        self._connections: dict[str, set[WebSocket]] = defaultdict(set)
        self._lock = asyncio.Lock()

    async def connect(self, project_id: str, websocket: WebSocket) -> None:
        await websocket.accept()
        async with self._lock:
            self._connections[project_id].add(websocket)

    async def disconnect(self, project_id: str, websocket: WebSocket) -> None:
        async with self._lock:
            sockets = self._connections.get(project_id)
            if not sockets:
                return
            sockets.discard(websocket)
            if not sockets:
                self._connections.pop(project_id, None)

    async def broadcast(self, project_id: str, event: dict[str, Any]) -> None:
        message = json.dumps(event, ensure_ascii=False)
        async with self._lock:
            sockets = list(self._connections.get(project_id, set()))

        if not sockets:
            return

        disconnected: list[WebSocket] = []
        for socket in sockets:
            try:
                await socket.send_text(message)
            except Exception:
                disconnected.append(socket)

        for socket in disconnected:
            await self.disconnect(project_id, socket)


sync_manager = ProjectSyncManager()
