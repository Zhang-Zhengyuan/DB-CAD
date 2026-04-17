from fastapi.testclient import TestClient
from uuid import uuid4

from app.main import app

client = TestClient(app)


def test_health() -> None:
    response = client.get("/health")
    assert response.status_code == 200
    assert response.json()["status"] == "ok"


def test_create_project_and_save_model() -> None:
    create_project_response = client.post("/projects", json={"name": f"demo-project-{uuid4()}"})
    assert create_project_response.status_code == 201
    project_id = create_project_response.json()["id"]

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
