"""Regression checks for deployable assets, routing and persistence DTOs."""

from datetime import datetime, timezone

from fastapi.testclient import TestClient

from app import crud
from app.main import app
from app.neo4j_store import Neo4jStore


def test_literal_routes_are_not_shadowed_by_version_parameters():
    for path, endpoint in [
        ("/projects/p/models/versions", "get_versions"),
        ("/projects/p/entities/versions", "list_entity_versions"),
        ("/projects/p/entities/diff", "diff_entity_versions"),
    ]:
        scope = {"type": "http", "path": path, "method": "GET"}
        from starlette.routing import Match

        match = next(r for r in app.routes if r.matches(scope)[0] == Match.FULL)
        assert match.endpoint.__name__ == endpoint


def test_dashboard_assets_work_without_database_startup():
    # No lifespan context: serving assets must not require a live database.
    client = TestClient(app)
    assert client.get("/webui").status_code == 200
    for asset in ["dashboard.css", "core.js", "projects.js", "graph.js", "versions.js"]:
        response = client.get("/webui-static/" + asset)
        assert response.status_code == 200
        assert response.content


def test_direct_neo4j_version_can_be_read_by_business_layer():
    # Exercise the actual adapter conversion without opening a driver.
    store = object.__new__(Neo4jStore)

    class Node(dict):
        id = 7

    record = store._version_from_node(
        Node(
            {
                "project_id": "p",
                "version": 1,
                "author": "a",
                "content_json": '{"sat":"stored"}',
                "created_at": datetime.now(timezone.utc).isoformat(),
            }
        )
    )
    model = crud.deserialize_version(record)
    assert model.version == 1
    assert model.content == {"sat": "stored"}
