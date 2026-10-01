"""Protocol boundaries that would corrupt a freshly joined JSON client."""

from datetime import UTC, datetime

from app import main
from app.json_graph_store import JsonGraphVersion


def _version(number: int, graph: dict, changes: list, removed: list) -> JsonGraphVersion:
    return JsonGraphVersion(
        project_id="p", version=number, author="tester", created_at=datetime.now(UTC),
        content={"entity_graph": graph, "changes": changes, "removed_ids": removed,
                 "schema": "dbcad.entity_graph.v1", "graph_hash": f"hash-{number}"},
    )


def test_initial_pull_is_complete_and_readded_body_is_not_removed(monkeypatch):
    graph = {
        "nodes": [
            {"id": "b1", "labels": ["body"], "props": {}},
            {"id": "f1", "labels": ["face"], "props": {}},
            {"id": "b2", "labels": ["body"], "props": {}},
        ],
        "rels": [{"type": "body_face", "start": "b1", "end": "f1", "props": {}}],
        "serializer_build": "test-v1",
    }
    commits = [
        _version(1, graph, [{"uuid": "b1", "changeType": "ADD"}], []),
        _version(2, graph, [{"uuid": "b1", "changeType": "REMOVE"}], ["b1"]),
        _version(3, graph, [{"uuid": "b1", "changeType": "ADD"}], []),
    ]

    class Store:
        def latest(self, project_id):
            return commits[-1]

        def get(self, project_id, version):
            return commits[version - 1]

        def list_after(self, project_id, version):
            return commits[version:]

    monkeypatch.setattr(main.settings, "direct_json_mode", True)
    monkeypatch.setattr(main, "get_json_graph_store", Store)

    initial = main.get_json_delta("p", base_version=0, _=None)
    assert not initial["delta"]
    assert {node["id"] for node in initial["entity_graph"]["nodes"]} == {"b1", "f1", "b2"}
    assert initial["removed_ids"] == []
    assert initial["entity_graph"]["serializer_build"] == "test-v1"

    incremental = main.get_json_delta("p", base_version=1, _=None)
    assert incremental["delta"]
    assert {node["id"] for node in incremental["entity_graph"]["nodes"]} == {"b1", "f1"}
    assert incremental["removed_ids"] == []
