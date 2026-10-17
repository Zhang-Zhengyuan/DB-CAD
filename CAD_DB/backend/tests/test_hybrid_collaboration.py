"""Unified HTTP/WS contracts, SAT preservation, conflicts and historical replay."""

import pytest
from fastapi.testclient import TestClient
from starlette.websockets import WebSocketDisconnect

from app import crud
from app.api import json_delta, models, projects
from app.collaboration import snapshots
from app.json_graph_store import split_graph_components
from app.json_collab import validate_graph
from app.main import app
from memory_store import MemoryStore


@pytest.fixture()
def setup(monkeypatch):
    store = MemoryStore()
    monkeypatch.setattr(crud, "direct_store", store)
    for module in (json_delta, models, snapshots):
        monkeypatch.setattr(module, "get_json_graph_store", lambda: store)
    import app.json_graph_store as persistence

    monkeypatch.setattr(persistence, "get_json_graph_store", lambda: store)
    client = TestClient(app)
    pid = client.post("/projects", json={"name": "hybrid"}).json()["id"]
    return client, store, pid


def graph(uuid="a", sat="SAT original text\n", label="body"):
    return {
        "schema": "dbcad.entity_graph.v2",
        "nodes": [
            {
                "id": uuid,
                "labels": [label],
                "props": {
                    "uuid": uuid,
                    "top_level": True,
                    "sat_fragment": sat,
                    "fallback": "on_failure",
                    "geometry": {"type": "plane"},
                },
            }
        ],
        "rels": [],
    }


def push(client, pid, g, base=None, removed=None):
    return client.post(
        f"/projects/{pid}/json-delta",
        json={
            "author": "Alice",
            "base_version": base,
            "entity_graph": g,
            "removed_ids": removed or [],
        },
    )


def test_sat_is_versioned_with_component_and_replaced_on_modify(setup):
    client, store, pid = setup
    assert push(client, pid, graph()).status_code == 201
    assert push(client, pid, graph("b", "B original"), 1).status_code == 201
    assert push(client, pid, graph("a", "A modified"), 2).status_code == 201
    delta = client.get(f"/projects/{pid}/json-delta?base_version=2").json()
    assert [n["id"] for n in delta["entity_graph"]["nodes"]] == ["a"]
    assert delta["entity_graph"]["nodes"][0]["props"]["sat_fragment"] == "A modified"
    full = client.get(f"/projects/{pid}/json-delta?base_version=0").json()
    assert {n["props"]["sat_fragment"] for n in full["entity_graph"]["nodes"]} == {
        "A modified",
        "B original",
    }
    history = client.get(f"/projects/{pid}/models/1").json()
    assert (
        history["content"]["entity_graph"]["nodes"][0]["props"]["sat_fragment"]
        == "SAT original text\n"
    )
    assert store.rows[-1].content["_audit"]["contains_sat"] is True
    assert client.get(f"/projects/{pid}/meta").json()["latest_version"] == 3
    assert [
        v["version"] for v in client.get(f"/projects/{pid}/models/versions").json()
    ] == [3, 2, 1]


def test_conflict_and_missing_component_do_not_advance_head(setup):
    client, store, pid = setup
    assert push(client, pid, graph()).status_code == 201
    assert push(client, pid, graph(), None).status_code == 409
    assert push(client, pid, graph(), 0).status_code == 409
    response = client.post(
        f"/projects/{pid}/json-delta",
        json={
            "author": "a",
            "base_version": 1,
            "entity_graph": {"nodes": [], "rels": []},
            "changes": [{"uuid": "missing"}],
        },
    )
    assert response.status_code == 422
    assert store.latest(pid).version == 1
    assert client.get(f"/projects/{pid}/json-delta?base_version=8").status_code == 409


def test_deletion_and_readd_across_multiple_versions(setup):
    client, store, pid = setup
    assert push(client, pid, graph()).status_code == 201
    assert push(client, pid, {"nodes": [], "rels": []}, 1, ["a"]).status_code == 201
    delta = client.get(f"/projects/{pid}/json-delta?base_version=1").json()
    assert delta["removed_ids"] == ["a"]
    assert delta["entity_graph"]["nodes"] == []
    assert push(client, pid, graph(sat="readded"), 2).status_code == 201
    delta = client.get(f"/projects/{pid}/json-delta?base_version=1").json()
    assert delta["removed_ids"] == []
    assert delta["entity_graph"]["nodes"][0]["props"]["sat_fragment"] == "readded"


def test_v2_rejects_missing_backups_and_standalone_faces_are_components(setup):
    client, store, pid = setup
    g = graph(label="face")
    assert split_graph_components(g)[0][0]["root_uuid"] == "a"
    assert push(client, pid, g).status_code == 201
    invalid = graph()
    del invalid["nodes"][0]["props"]["sat_fragment"]
    assert push(client, pid, invalid, 1).status_code == 422
    assert store.latest(pid).version == 1


def test_websocket_only_announces_same_chain_and_rejects_old_mode(setup):
    client, store, pid = setup
    with client.websocket_connect(f"/ws/projects/{pid}?client_id=bob") as socket:
        assert socket.receive_json()["type"] == "presence_snapshot"
        assert push(client, pid, graph()).status_code == 201
        event = socket.receive_json()
        assert event["type"] == "json_delta_saved" and event["version"] == 1
        assert "entity_graph" not in event
        socket.send_text("ping")
        assert socket.receive_json()["type"] == "pong"
        socket.send_json({"type": "submit_model", "content": {"sat": "obsolete"}})
        assert socket.receive_json()["type"] == "error"
        socket.send_text("sync_now")
        assert socket.receive_json()["version"] == 1
    with pytest.raises(WebSocketDisconnect):
        with client.websocket_connect(f"/ws/projects/{pid}?stream=mode0"):
            pass
    assert len(store.rows) == 1
    assert client.post(f"/projects/{pid}/models", json={}).status_code == 404
    assert client.post(f"/projects/{pid}/entities", json={}).status_code == 404


def test_upgrade_legacy_components_without_rewriting_unchanged_bodies(setup):
    client, store, pid = setup
    legacy = {
        "schema": "dbcad.entity_graph.v1",
        "nodes": [{"id": "legacy", "labels": ["body"], "props": {}}],
        "rels": [],
    }
    assert push(client, pid, legacy).status_code == 201
    assert push(client, pid, graph(), 1).status_code == 201
    mixed = store.graph(pid, 2)
    assert mixed["schema"] == "dbcad.entity_graph.v1"
    assert {n["id"] for n in mixed["nodes"]} == {"legacy", "a"}
    assert push(client, pid, graph("legacy"), 2).status_code == 201
    assert store.graph(pid, 3)["schema"] == "dbcad.entity_graph.v2"


def test_distinct_dirty_component_can_pull_then_push_on_new_base(setup):
    client, store, pid = setup
    assert push(client, pid, graph("a"), None).status_code == 201
    assert push(client, pid, graph("b"), 1).status_code == 201
    remote = client.get(f"/projects/{pid}/json-delta?base_version=1").json()
    assert [n["id"] for n in remote["entity_graph"]["nodes"]] == ["b"]
    assert (
        push(client, pid, graph("a", "local edit"), remote["version"]).status_code
        == 201
    )
    full = store.graph(pid, 3)
    assert {n["id"]: n["props"]["sat_fragment"] for n in full["nodes"]} == {
        "a": "local edit",
        "b": "SAT original text\n",
    }
