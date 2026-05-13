import asyncio
import json

from app.sync import ProjectSyncManager


class FakeWebSocket:
    def __init__(self, *, fail_on_send: bool = False) -> None:
        self.accepted = False
        self.fail_on_send = fail_on_send
        self.text_messages: list[str] = []
        self.json_messages: list[dict] = []

    async def accept(self) -> None:
        self.accepted = True

    async def send_text(self, message: str) -> None:
        if self.fail_on_send:
            raise RuntimeError("simulated send failure")
        self.text_messages.append(message)
        self.json_messages.append(json.loads(message))


def test_disconnect_returns_metadata_and_removes_connection() -> None:
    async def scenario() -> None:
        manager = ProjectSyncManager()
        socket = FakeWebSocket()

        await manager.connect("project-a", socket, client_id="c1", author="alice")
        disconnected = await manager.disconnect("project-a", socket)

        assert socket.accepted is True
        assert disconnected == {"client_id": "c1", "author": "alice"}
        assert await manager.list_members("project-a") == []

    asyncio.run(scenario())


def test_list_members_returns_snapshot() -> None:
    async def scenario() -> None:
        manager = ProjectSyncManager()
        socket_a = FakeWebSocket()
        socket_b = FakeWebSocket()

        await manager.connect("project-a", socket_a, client_id="c1", author="alice")
        await manager.connect("project-a", socket_b, client_id="c2", author="bob")

        members = await manager.list_members("project-a")

        assert {member["client_id"] for member in members} == {"c1", "c2"}
        assert {member["author"] for member in members} == {"alice", "bob"}
        assert all("connected_at" in member for member in members)

    asyncio.run(scenario())


def test_broadcast_skips_excluded_client_and_cleans_dead_connections() -> None:
    async def scenario() -> None:
        manager = ProjectSyncManager()
        socket_a = FakeWebSocket()
        socket_b = FakeWebSocket()
        socket_dead = FakeWebSocket(fail_on_send=True)

        await manager.connect("project-a", socket_a, client_id="c1", author="alice")
        await manager.connect("project-a", socket_b, client_id="c2", author="bob")
        await manager.connect("project-a", socket_dead, client_id="c3", author="charlie")

        event = {"type": "model_saved", "version": 3}
        await manager.broadcast("project-a", event, exclude_client_id="c2")

        assert socket_a.json_messages == [event]
        assert socket_b.json_messages == []
        members = await manager.list_members("project-a")
        assert {member["client_id"] for member in members} == {"c1", "c2"}

    asyncio.run(scenario())
