from collections.abc import Iterator
from dataclasses import dataclass
from datetime import datetime, timezone
from fastapi.testclient import TestClient
import pytest
from uuid import uuid4

from app import crud
from app.main import app
from app.storage_bridge import VersionRecord


@dataclass
class _Project:
    id: str
    name: str
    created_at: datetime
    updated_at: datetime


class _MemoryStorage:
    def __init__(self) -> None:
        self.projects: dict[str, _Project] = {}
        self.project_names: dict[str, str] = {}
        self.versions: dict[str, list[VersionRecord]] = {}
        self.next_version_id = 1

    def healthcheck(self) -> None:
        return None

    def close(self) -> None:
        return None

    def create_project(self, name: str) -> _Project:
        now = datetime.now(timezone.utc)
        project = _Project(id=uuid4().hex, name=name, created_at=now, updated_at=now)
        self.projects[project.id] = project
        self.project_names[name] = project.id
        self.versions[project.id] = []
        return project

    def get_project_or_none(self, project_id: str) -> _Project | None:
        return self.projects.get(project_id)

    def get_project_by_name_or_none(self, project_name: str) -> _Project | None:
        project_id = self.project_names.get(project_name)
        return self.projects.get(project_id) if project_id is not None else None

    def create_model_version(
        self,
        project_id: str,
        author: str,
        content: dict,
        base_version: int | None,
    ) -> VersionRecord:
        rows = self.versions.setdefault(project_id, [])
        latest = rows[-1].version if rows else None
        if latest is not None and base_version is None:
            from fastapi import HTTPException, status

            raise HTTPException(
                status_code=status.HTTP_409_CONFLICT,
                detail=f"base_version is required: latest version is {latest}",
            )
        if base_version is not None and base_version != latest:
            from fastapi import HTTPException, status

            raise HTTPException(
                status_code=status.HTTP_409_CONFLICT,
                detail=f"Version conflict: latest version is {latest or 0}",
            )

        now = datetime.now(timezone.utc)
        version = VersionRecord(
            id=self.next_version_id,
            project_id=project_id,
            version=1 if latest is None else latest + 1,
            author=author,
            content=dict(content),
            created_at=now,
        )
        self.next_version_id += 1
        rows.append(version)
        return version

    def get_latest_version_or_none(self, project_id: str) -> VersionRecord | None:
        rows = self.versions.get(project_id, [])
        return rows[-1] if rows else None

    def get_version_or_none(self, project_id: str, version: int) -> VersionRecord | None:
        for row in self.versions.get(project_id, []):
            if row.version == version:
                return row
        return None

    def list_versions(self, project_id: str, limit: int, offset: int) -> list[VersionRecord]:
        rows = list(reversed(self.versions.get(project_id, [])))
        return rows[offset : offset + limit]


@pytest.fixture()
def client(monkeypatch: pytest.MonkeyPatch) -> Iterator[TestClient]:
    storage = _MemoryStorage()
    monkeypatch.setattr(crud, "storage_bridge", storage)

    with TestClient(app) as test_client:
        yield test_client


def test_health(client: TestClient) -> None:
    response = client.get("/health")
    assert response.status_code == 200
    assert response.json()["status"] == "ok"


def test_create_project_and_save_model(client: TestClient) -> None:
    project_name = f"demo-project-{uuid4()}"
    create_project_response = client.post("/projects", json={"name": project_name})
    assert create_project_response.status_code == 201
    project_id = create_project_response.json()["id"]

    by_name_response = client.get(f"/projects/by-name/{project_name}")
    assert by_name_response.status_code == 200
    assert by_name_response.json()["id"] == project_id

    save_response = client.post(
        f"/projects/{project_id}/models",
        json={"author": "tester", "content": {"entities": []}, "base_version": None},
    )
    assert save_response.status_code == 201
    assert save_response.json()["version"] == 1

    latest_response = client.get(f"/projects/{project_id}/models/latest")
    assert latest_response.status_code == 200
    assert latest_response.json()["version"] == 1
    assert latest_response.json()["content"] == {"entities": []}

    version_response = client.get(f"/projects/{project_id}/models/1")
    assert version_response.status_code == 200
    assert version_response.json()["version"] == 1


def test_save_requires_base_version_after_first_version(client: TestClient) -> None:
    project_name = f"demo-conflict-project-{uuid4()}"
    create_project_response = client.post("/projects", json={"name": project_name})
    assert create_project_response.status_code == 201
    project_id = create_project_response.json()["id"]

    first = client.post(
        f"/projects/{project_id}/models",
        json={"author": "tester", "content": {"sat": "v1"}, "base_version": None},
    )
    assert first.status_code == 201

    missing_base = client.post(
        f"/projects/{project_id}/models",
        json={"author": "tester", "content": {"sat": "v2"}, "base_version": None},
    )
    assert missing_base.status_code == 409
    assert "base_version is required" in missing_base.json()["detail"]


def test_websocket_multi_client_sync(client: TestClient) -> None:
    project_name = f"demo-ws-project-{uuid4()}"
    create_project_response = client.post("/projects", json={"name": project_name})
    assert create_project_response.status_code == 201
    project_id = create_project_response.json()["id"]

    seed_response = client.post(
        f"/projects/{project_id}/models",
        json={"author": "seed", "content": {"entities": ["v1"]}, "base_version": None},
    )
    assert seed_response.status_code == 201
    assert seed_response.json()["version"] == 1

    with client.websocket_connect(f"/ws/projects/{project_id}") as ws1:
        presence1 = ws1.receive_json()
        assert presence1["type"] == "presence_snapshot"
        assert len(presence1["members"]) == 1

        snapshot1 = ws1.receive_json()
        assert snapshot1["type"] == "model_saved"
        assert snapshot1["version"] == 1
        assert snapshot1["trigger"] == "snapshot"
        assert snapshot1["content"] == {"entities": ["v1"]}

        with client.websocket_connect(f"/ws/projects/{project_id}") as ws2:
            joined = ws1.receive_json()
            assert joined["type"] == "collaborator_joined"

            snapshot2 = ws2.receive_json()
            assert snapshot2["type"] == "presence_snapshot"
            assert len(snapshot2["members"]) == 2

            snapshot2_model = ws2.receive_json()
            assert snapshot2_model["type"] == "model_saved"
            assert snapshot2_model["version"] == 1
            assert snapshot2_model["trigger"] == "snapshot"
            assert snapshot2_model["content"] == {"entities": ["v1"]}

            save_response = client.post(
                f"/projects/{project_id}/models",
                json={"author": "collaborator", "content": {"entities": ["v2"]}, "base_version": 1},
            )
            assert save_response.status_code == 201
            assert save_response.json()["version"] == 2

            update1 = ws1.receive_json()
            update2 = ws2.receive_json()

            assert update1["type"] == "model_saved"
            assert update2["type"] == "model_saved"
            assert update1["version"] == 2
            assert update2["version"] == 2
            assert update1["trigger"] == "http_save"
            assert update2["trigger"] == "http_save"
            assert update1["content"] == {"entities": ["v2"]}
            assert update2["content"] == {"entities": ["v2"]}

            ws1.send_text("sync_now")
            sync_now = ws1.receive_json()
            assert sync_now["type"] == "model_saved"
            assert sync_now["version"] == 2
            assert sync_now["trigger"] == "sync_now"
            assert sync_now["content"] == {"entities": ["v2"]}

        left = ws1.receive_json()
        assert left["type"] == "collaborator_left"


def test_websocket_join_empty_project_before_first_save(client: TestClient) -> None:
    project_name = f"demo-empty-ws-project-{uuid4()}"
    create_project_response = client.post("/projects", json={"name": project_name})
    assert create_project_response.status_code == 201
    project_id = create_project_response.json()["id"]

    with client.websocket_connect(f"/ws/projects/{project_id}?client_id=a&author=A") as ws1:
        presence1 = ws1.receive_json()
        assert presence1["type"] == "presence_snapshot"
        assert presence1["self"]["client_id"] == "a"
        assert len(presence1["members"]) == 1

        with client.websocket_connect(f"/ws/projects/{project_id}?client_id=b&author=B") as ws2:
            joined = ws1.receive_json()
            assert joined["type"] == "collaborator_joined"
            assert joined["client_id"] == "b"

            presence2 = ws2.receive_json()
            assert presence2["type"] == "presence_snapshot"
            assert len(presence2["members"]) == 2

            save_response = client.post(
                f"/projects/{project_id}/models",
                json={"author": "A", "content": {"sat": "first-version"}, "base_version": None},
            )
            assert save_response.status_code == 201
            assert save_response.json()["version"] == 1

            update1 = ws1.receive_json()
            update2 = ws2.receive_json()
            assert update1["type"] == "model_saved"
            assert update2["type"] == "model_saved"
            assert update1["version"] == 1
            assert update2["version"] == 1
            assert update1["content"] == {"sat": "first-version"}
            assert update2["content"] == {"sat": "first-version"}


def test_websocket_submit_model_accepts_and_rejects_stale_base(client: TestClient) -> None:
    project_name = f"demo-submit-ws-project-{uuid4()}"
    create_project_response = client.post("/projects", json={"name": project_name})
    assert create_project_response.status_code == 201
    project_id = create_project_response.json()["id"]

    with client.websocket_connect(f"/ws/projects/{project_id}?client_id=a&author=A") as ws1:
        assert ws1.receive_json()["type"] == "presence_snapshot"

        with client.websocket_connect(f"/ws/projects/{project_id}?client_id=b&author=B") as ws2:
            assert ws1.receive_json()["type"] == "collaborator_joined"
            assert ws2.receive_json()["type"] == "presence_snapshot"

            ws1.send_json(
                {
                    "type": "submit_model",
                    "request_id": "r1",
                    "author": "A",
                    "content": {"sat": "v1"},
                    "base_version": None,
                    "reason": "test-submit",
                }
            )
            accepted1 = ws1.receive_json()
            broadcast1 = ws2.receive_json()
            assert accepted1["type"] == "model_saved"
            assert broadcast1["type"] == "model_saved"
            assert accepted1["request_id"] == "r1"
            assert accepted1["source_client_id"] == "a"
            assert accepted1["version"] == 1
            assert broadcast1["version"] == 1

            ws2.send_json(
                {
                    "type": "submit_model",
                    "request_id": "r2",
                    "author": "B",
                    "content": {"sat": "stale"},
                    "base_version": None,
                }
            )
            rejected = ws2.receive_json()
            assert rejected["type"] == "submit_rejected"
            assert rejected["request_id"] == "r2"
            assert rejected["reason"] == "conflict"
            assert rejected["latest_version"] == 1
            assert rejected["content"] == {"sat": "v1"}

            ws2.send_json(
                {
                    "type": "submit_model",
                    "request_id": "r3",
                    "author": "B",
                    "content": {"sat": "v2"},
                    "base_version": 1,
                }
            )
            accepted2_on_ws1 = ws1.receive_json()
            accepted2_on_ws2 = ws2.receive_json()
            assert accepted2_on_ws1["type"] == "model_saved"
            assert accepted2_on_ws2["type"] == "model_saved"
            assert accepted2_on_ws2["request_id"] == "r3"
            assert accepted2_on_ws2["version"] == 2
            assert accepted2_on_ws1["content"] == {"sat": "v2"}
