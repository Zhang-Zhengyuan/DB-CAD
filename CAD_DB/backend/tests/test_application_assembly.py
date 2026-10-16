"""Regression checks for deployable assets, routing and persistence DTOs."""

from fastapi.testclient import TestClient

from app.main import app


def test_literal_routes_are_not_shadowed_by_version_parameters():
    for path, endpoint in [
        ("/projects/p/models/versions", "get_versions"),
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


def test_only_one_collaboration_write_route_remains():
    posts = {
        r.path for r in app.routes if "POST" in (getattr(r, "methods", None) or set())
    }
    assert "/projects/{project_id}/json-delta" in posts
    assert "/projects/{project_id}/models" not in posts
    assert "/projects/{project_id}/entities" not in posts
    health = TestClient(app).get("/health").json()
    assert health["collaboration_mode"] == "hybrid_json"
    assert health["bridge_required"] is False
