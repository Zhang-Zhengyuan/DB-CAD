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
        alice, bob = Socket(), Socket()
        await manager.connect("p", alice, "alice", "Alice")
        await manager.connect("p", bob, "bob", "Bob")
        await manager.broadcast(
            "p",
            {"type": "json_delta_saved", "version": 4},
            exclude_client_id="alice",
            stream="mode1",
        )
        assert alice.messages == []
        assert len(bob.messages) == 1

    asyncio.run(scenario())
