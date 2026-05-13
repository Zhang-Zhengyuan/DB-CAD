import asyncio
from datetime import datetime, timezone

import pytest
from fastapi import HTTPException, status

from app import crud
from app.main import _handle_project_ws_message, _model_saved_event, _send_submit_rejected
from app.storage_bridge import VersionRecord
from tests.assignment_support import install_memory_storage


class FakeJsonWebSocket:
    def __init__(self) -> None:
        self.messages: list[dict] = []

    async def send_json(self, payload: dict) -> None:
        self.messages.append(payload)


def test_model_saved_event_includes_optional_fields() -> None:
    version = VersionRecord(
        id=1,
        project_id="project-a",
        version=2,
        author="alice",
        content={"sat": "v2"},
        created_at=datetime(2024, 1, 1, tzinfo=timezone.utc),
    )

    event = _model_saved_event(
        "project-a",
        version,
        trigger="http_save",
        request_id="req-1",
        source_client_id="client-7",
    )

    assert event == {
        "type": "model_saved",
        "project_id": "project-a",
        "version": 2,
        "author": "alice",
        "created_at": "2024-01-01T00:00:00+00:00",
        "trigger": "http_save",
        "content": {"sat": "v2"},
        "request_id": "req-1",
        "source_client_id": "client-7",
    }


def test_send_submit_rejected_includes_latest_metadata(monkeypatch: pytest.MonkeyPatch) -> None:
    install_memory_storage(monkeypatch)
    latest = VersionRecord(
        id=3,
        project_id="project-a",
        version=5,
        author="bob",
        content={"sat": "latest"},
        created_at=datetime(2024, 2, 2, tzinfo=timezone.utc),
    )
    monkeypatch.setattr(crud, "get_latest_version_or_404", lambda project_id: latest)
    socket = FakeJsonWebSocket()

    asyncio.run(
        _send_submit_rejected(
            socket,
            "project-a",
            request_id="req-9",
            reason="conflict",
            detail="Version conflict",
        )
    )

    assert socket.messages == [
        {
            "type": "submit_rejected",
            "project_id": "project-a",
            "reason": "conflict",
            "detail": "Version conflict",
            "request_id": "req-9",
            "latest_version": 5,
            "author": "bob",
            "created_at": "2024-02-02T00:00:00+00:00",
            "content": {"sat": "latest"},
        }
    ]


def test_handle_project_ws_message_routes_protocol_messages(monkeypatch: pytest.MonkeyPatch) -> None:
    async def scenario() -> None:
        calls: list[tuple[str, str]] = []
        socket = FakeJsonWebSocket()

        async def fake_send_latest(project_id: str, websocket: FakeJsonWebSocket, trigger: str) -> None:
            calls.append((project_id, trigger))

        async def fake_handle_submit(
            websocket: FakeJsonWebSocket,
            project_id: str,
            client_id: str,
            author: str,
            data: dict,
        ) -> None:
            calls.append((project_id, data["type"]))

        monkeypatch.setattr("app.main._send_latest_model_saved_event", fake_send_latest)
        monkeypatch.setattr("app.main._handle_submit_model_message", fake_handle_submit)

        await _handle_project_ws_message(socket, "project-a", "c1", "alice", "sync_now")
        await _handle_project_ws_message(socket, "project-a", "c1", "alice", "ping")
        await _handle_project_ws_message(socket, "project-a", "c1", "alice", '{"type":"submit_model","content":{"sat":"v1"}}')
        await _handle_project_ws_message(socket, "project-a", "c1", "alice", "not-json")
        await _handle_project_ws_message(socket, "project-a", "c1", "alice", '{"type":"unknown"}')

        assert ("project-a", "sync_now") in calls
        assert ("project-a", "submit_model") in calls
        assert socket.messages[0] == {"type": "pong", "project_id": "project-a"}
        assert socket.messages[1]["type"] == "error"
        assert "Unsupported WebSocket message" in socket.messages[1]["detail"]
        assert socket.messages[2]["type"] == "error"
        assert "Unsupported WebSocket message type: unknown" == socket.messages[2]["detail"]

    asyncio.run(scenario())


def test_send_submit_rejected_omits_latest_when_project_has_no_versions(monkeypatch: pytest.MonkeyPatch) -> None:
    install_memory_storage(monkeypatch)

    def raise_not_found(project_id: str):
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="No model version found")

    monkeypatch.setattr(crud, "get_latest_version_or_404", raise_not_found)
    socket = FakeJsonWebSocket()

    asyncio.run(
        _send_submit_rejected(
            socket,
            "project-empty",
            request_id=None,
            reason="save_failed",
            detail="No latest version",
        )
    )

    assert socket.messages == [
        {
            "type": "submit_rejected",
            "project_id": "project-empty",
            "reason": "save_failed",
            "detail": "No latest version",
        }
    ]
