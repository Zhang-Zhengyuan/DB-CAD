from fastapi.testclient import TestClient
from uuid import uuid4

from app.main import app

client = TestClient(app)


def test_health() -> None:
    response = client.get("/health")
    assert response.status_code == 200
    assert response.json()["status"] == "ok"


def test_create_project_and_save_model() -> None:
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


def test_websocket_multi_client_sync() -> None:
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

            ws1.send_text("sync_now")
            sync_now = ws1.receive_json()
            assert sync_now["type"] == "model_saved"
            assert sync_now["version"] == 2
            assert sync_now["trigger"] == "sync_now"

        left = ws1.receive_json()
        assert left["type"] == "collaborator_left"
