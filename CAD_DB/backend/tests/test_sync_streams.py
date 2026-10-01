import asyncio

from app.sync import ProjectSyncManager


class Socket:
    def __init__(self):
        self.messages = []

    async def accept(self):
        pass

    async def send_text(self, message):
        self.messages.append(message)


def test_collaboration_versions_are_broadcast_only_to_their_stream():
    async def scenario():
        manager = ProjectSyncManager()
        sat_client, json_client = Socket(), Socket()
        await manager.connect("p", sat_client, "sat", "Alice", stream="mode0")
        await manager.connect("p", json_client, "json", "Bob", stream="mode1")
        await manager.broadcast("p", {"type": "json_delta_saved", "version": 4}, stream="mode1")
        assert sat_client.messages == []
        assert len(json_client.messages) == 1
        await manager.broadcast("p", {"type": "model_saved", "version": 2}, stream="mode0")
        assert len(sat_client.messages) == 1
        assert len(json_client.messages) == 1

    asyncio.run(scenario())
