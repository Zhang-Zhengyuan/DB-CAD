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
