import asyncio
import json
from collections import defaultdict
from dataclasses import dataclass
from datetime import datetime, timezone
from typing import Any

from fastapi import WebSocket


@dataclass(slots=True)
class ClientConnection:
    websocket: WebSocket
    client_id: str
    author: str
    connected_at: datetime


class ProjectSyncManager:
    def __init__(self) -> None:
        self._connections: dict[str, dict[str, ClientConnection]] = defaultdict(dict)
        self._lock = asyncio.Lock()

    async def connect(self, project_id: str, websocket: WebSocket, client_id: str, author: str) -> None:
        await websocket.accept()
        async with self._lock:
            self._connections[project_id][client_id] = ClientConnection(
                websocket=websocket,
                client_id=client_id,
                author=author,
                connected_at=datetime.now(timezone.utc),
            )

    async def disconnect(self, project_id: str, websocket: WebSocket) -> dict[str, Any] | None:
        async with self._lock:
            clients = self._connections.get(project_id)
            if not clients:
                return None

            disconnected_client_id: str | None = None
            disconnected_author: str | None = None
            for client_id, connection in clients.items():
                if connection.websocket is websocket:
                    disconnected_client_id = client_id
                    disconnected_author = connection.author
                    break

            if disconnected_client_id is None:
                return None

            clients.pop(disconnected_client_id, None)
            if not clients:
                self._connections.pop(project_id, None)

            return {
                "client_id": disconnected_client_id,
                "author": disconnected_author or "unknown",
            }

    async def list_members(self, project_id: str) -> list[dict[str, Any]]:
        async with self._lock:
            clients = list(self._connections.get(project_id, {}).values())

        return [
            {
                "client_id": client.client_id,
                "author": client.author,
                "connected_at": client.connected_at.isoformat(),
            }
            for client in clients
        ]

    async def broadcast(self, project_id: str, event: dict[str, Any], exclude_client_id: str | None = None) -> None:
        message = json.dumps(event, ensure_ascii=False)
        async with self._lock:
            clients = list(self._connections.get(project_id, {}).values())

        if not clients:
            return

        disconnected: list[WebSocket] = []
        for client in clients:
            if exclude_client_id and client.client_id == exclude_client_id:
                continue
            try:
                await client.websocket.send_text(message)
            except Exception:
                disconnected.append(client.websocket)

        for socket in disconnected:
            await self.disconnect(project_id, socket)


sync_manager = ProjectSyncManager()
