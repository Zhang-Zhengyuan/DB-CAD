from app.json_collab import conflicting_ids, graph_component, graph_diff, graph_hash, merge_graph, validate_graph


def graph(body="b", child="f", value=1):
    return {
        "nodes": [
            {"id": body, "labels": ["body"], "props": {"value": value}},
            {"id": child, "labels": ["face"], "props": {"value": value}},
        ],
        "rels": [{"type": "body_face", "start": body, "end": child, "props": {}}],
    }


def test_validate_rejects_unknown_relationship_endpoint():
    try:
        validate_graph({"nodes": [{"id": "a"}], "rels": [{"type": "x", "start": "a", "end": "missing"}]})
    except ValueError as exc:
        assert "unknown node" in str(exc)
    else:
        raise AssertionError("invalid relationship was accepted")


def test_merge_replaces_complete_topology_component():
    base = graph(child="old-face", value=1)
    incoming = graph(child="new-face", value=2)
    merged = merge_graph(base, incoming, changes=[{"uuid": "b", "changeType": "MODIFY"}])
    assert {node["id"] for node in merged["nodes"]} == {"b", "new-face"}
    assert merged["rels"][0]["end"] == "new-face"


def test_merge_remove_body_removes_topology():
    merged = merge_graph(graph(), {"nodes": [], "rels": []}, changes=[], removed_ids=["b"])
    assert merged["nodes"] == []
    assert merged["rels"] == []


def test_conflicts_are_detected_by_stable_body_id():
    base = graph(value=1)
    local = graph(value=2)
    remote = graph(value=3)
    assert conflicting_ids(base, local, remote) == ["b"]
    assert conflicting_ids(base, local, base) == []


def test_diff_and_hash_are_deterministic():
    changed = graph(value=2)
    diff = graph_diff(graph(value=1), changed)
    assert diff["modified_nodes"]
    assert graph_hash(changed) == graph_hash(changed)


def test_component_contains_complete_topology_chain():
    source = {
        "nodes": [
            {"id": "b", "labels": ["body"], "props": {}},
            {"id": "l", "labels": ["lump"], "props": {}},
            {"id": "s", "labels": ["shell"], "props": {}},
            {"id": "f", "labels": ["face"], "props": {}},
        ],
        "rels": [
            {"type": "body_lump", "start": "b", "end": "l", "props": {}},
            {"type": "lump_shell", "start": "l", "end": "s", "props": {}},
            {"type": "shell_face", "start": "s", "end": "f", "props": {}},
        ],
    }
    result = graph_component(source, ["b"])
    assert {node["id"] for node in result["nodes"]} == {"b", "l", "s", "f"}
    assert len(result["rels"]) == 3
