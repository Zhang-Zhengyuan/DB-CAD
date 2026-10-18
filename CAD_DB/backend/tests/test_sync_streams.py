import asyncio

from app.sync import ProjectSyncManager


class Socket:
    def __init__(self):
        self.messages = []

    async def accept(self):
        pass

    async def send_text(self, message):
        self.messages.append(message)

    async def send_json(self, payload):
        import json as _json

        self.messages.append(_json.dumps(payload))


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


def test_edit_intent_is_relayed_to_peers_but_not_the_sender():
    import json

    import app.collaboration.protocol as protocol
    from app.collaboration.protocol import _handle_project_ws_message

    async def scenario():
        manager = ProjectSyncManager()
        original = protocol.sync_manager
        protocol.sync_manager = manager  # 处理器用模块级单例；换成局部实例隔离
        try:
            await _scenario(manager)
        finally:
            protocol.sync_manager = original

    async def _scenario(manager):
        alice, bob = Socket(), Socket()
        await manager.connect("p", alice, "alice", "Alice")
        await manager.connect("p", bob, "bob", "Bob")

        # alice 通过协议处理器广播编辑意图
        await _handle_project_ws_message(
            alice, "p", "alice", "Alice",
            json.dumps({"type": "edit_intent", "component_uuid": "u-1", "action": "begin"}),
        )
        assert alice.messages == [], "发送者不收到自己的意图"
        assert len(bob.messages) == 1
        event = json.loads(bob.messages[0])
        assert event["type"] == "edit_intent"
        assert event["component_uuid"] == "u-1"
        assert event["action"] == "begin"
        assert event["author"] == "Alice"

        # 非法意图被拒绝且不广播（发送者收到 error 应答）
        bob.messages.clear()
        await _handle_project_ws_message(
            alice, "p", "alice", "Alice",
            json.dumps({"type": "edit_intent", "component_uuid": "", "action": "begin"}),
        )
        assert bob.messages == []
        assert any('"error"' in item for item in alice.messages)

    asyncio.run(scenario())
