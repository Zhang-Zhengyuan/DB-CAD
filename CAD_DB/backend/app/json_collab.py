"""Deterministic, storage independent CAD collaboration primitives.

The CAD client owns ACIS serialization.  The server only validates and merges
JSON graphs, so this module deliberately has no ACIS or SAT dependency.
"""

from __future__ import annotations

from copy import deepcopy
import hashlib
import json
from typing import Any, Iterable


class GraphValidationError(ValueError):
    """The collaboration payload is not a valid entity graph."""


def _as_dict(value: Any, name: str) -> dict[str, Any]:
    if hasattr(value, "model_dump"):
        value = value.model_dump(mode="json")
    if not isinstance(value, dict):
        raise GraphValidationError(f"{name} must be an object")
    return value


def validate_graph(graph: Any) -> dict[str, Any]:
    """Validate and return a deep copied canonical graph.

    IDs are intentionally opaque strings.  The server must not interpret ACIS
    geometry, but it must guarantee referential integrity before committing a
    version.
    """
    graph = _as_dict(graph, "entity_graph")
    raw_nodes = graph.get("nodes", [])
    raw_rels = graph.get("rels", graph.get("relationships", []))
    if not isinstance(raw_nodes, list) or not isinstance(raw_rels, list):
        raise GraphValidationError("nodes and rels must be arrays")

    nodes: list[dict[str, Any]] = []
    ids: set[str] = set()
    for index, raw in enumerate(raw_nodes):
        node = _as_dict(raw, f"nodes[{index}]")
        node_id = str(node.get("id", "")).strip()
        if not node_id:
            raise GraphValidationError(f"nodes[{index}].id is required")
        if node_id in ids:
            raise GraphValidationError(f"duplicate node id: {node_id}")
        labels = node.get("labels", [])
        props = node.get("props", {})
        if not isinstance(labels, list) or not all(isinstance(v, str) for v in labels):
            raise GraphValidationError(
                f"nodes[{index}].labels must be an array of strings"
            )
        if not isinstance(props, dict):
            raise GraphValidationError(f"nodes[{index}].props must be an object")
        if "sat_fragment" in props and (
            not isinstance(props["sat_fragment"], str)
            or not props["sat_fragment"].strip()
        ):
            raise GraphValidationError(
                f"nodes[{index}].props.sat_fragment must be nonempty text"
            )
        if props.get("fallback") in ("sat_fragment", "on_failure") and not props.get(
            "sat_fragment"
        ):
            raise GraphValidationError(f"nodes[{index}] requires a SAT backup")
        if (
            graph.get("schema") == "dbcad.entity_graph.v2"
            and ("body" in labels or props.get("top_level"))
            and not props.get("sat_fragment")
        ):
            raise GraphValidationError(
                f"nodes[{index}] requires a SAT backup for hybrid v2"
            )
        ids.add(node_id)
        nodes.append({"id": node_id, "labels": list(labels), "props": deepcopy(props)})

    rels: list[dict[str, Any]] = []
    for index, raw in enumerate(raw_rels):
        rel = _as_dict(raw, f"rels[{index}]")
        start = str(rel.get("start", "")).strip()
        end = str(rel.get("end", "")).strip()
        rel_type = str(rel.get("type", "")).strip()
        if not start or not end or not rel_type:
            raise GraphValidationError(f"rels[{index}] requires type, start and end")
        if start not in ids or end not in ids:
            raise GraphValidationError(f"rels[{index}] references an unknown node")
        props = rel.get("props", {})
        if not isinstance(props, dict):
            raise GraphValidationError(f"rels[{index}].props must be an object")
        rels.append(
            {"type": rel_type, "start": start, "end": end, "props": deepcopy(props)}
        )

    # Preserve arbitrary metadata, but keep the graph payload itself stable.
    result = {"nodes": nodes, "rels": rels}
    for key, value in graph.items():
        if key not in {"nodes", "rels", "relationships"}:
            result[key] = deepcopy(value)
    return result


def canonical_json(value: Any) -> str:
    return json.dumps(value, ensure_ascii=False, sort_keys=True, separators=(",", ":"))


def graph_hash(graph: Any) -> str:
    return hashlib.sha256(
        canonical_json(validate_graph(graph)).encode("utf-8")
    ).hexdigest()


def _changed_ids(changes: Iterable[Any] | None, incoming: dict[str, Any]) -> set[str]:
    ids: set[str] = set()
    for change in changes or []:
        if isinstance(change, str):
            ids.add(change)
        elif isinstance(change, dict):
            value = change.get("uuid", change.get("id"))
            if value is not None and str(value).strip():
                ids.add(str(value).strip())
    # A graph with no explicit change list is a complete snapshot.
    if not ids:
        ids = {str(node["id"]) for node in incoming["nodes"]}
    return ids


def _component_ids(graph: dict[str, Any], roots: set[str]) -> set[str]:
    """Return the topology component containing each changed root.

    ACIS child node IDs are generated locally and therefore are not stable
    across clients.  A body UUID is stable, so a body change must replace its
    complete connected topology component rather than one node by exact ID.
    """
    if not roots:
        return set()
    # Topology edges form a component.  Cross-body design dependencies do
    # not: replacing one body must never remove its neighbour.
    # These are the relation names emitted by entity_graph_serializer.cpp.
    # Keep the complete ACIS topology chain here. Omitting e.g. loop_start or
    # face_next truncates a cube during a delta merge and leaves an
    # apparently valid, but unrenderable, BODY.
    topology_types = {
        "body_lump",
        "body_wire",
        "body_transform",
        "lump_shell",
        "lump_next",
        "lump_body",
        "shell_next",
        "shell_face",
        "shell_wire",
        "shell_lump",
        "shell_subshell",
        "subshell_shell",
        "subshell_parent",
        "subshell_sibling",
        "subshell_child",
        "subshell_face",
        "subshell_wire",
        "face_subshell",
        "wire_subshell",
        "wire_coedge",
        "wire_next",
        "wire_owner",
        "face_loop",
        "face_next",
        "face_shell",
        "loop_face",
        "loop_start",
        "loop_next",
        "coedge_next",
        "coedge_previous",
        "coedge_partner",
        "coedge_edge",
        "coedge_owner",
        "coedge_geometry",
        "edge_start",
        "edge_end",
        "edge_coedge",
        "vertex_edge",
        "edge_vertex",
        "face_geometry",
        "edge_geometry",
        "vertex_geometry",
    }
    adjacency: dict[str, set[str]] = {}
    for rel in graph["rels"]:
        rel_type = rel["type"].lower()
        if (
            rel_type not in topology_types
            and not rel_type.endswith("_ptr")
            and rel_type not in {"body_face", "body_edge", "body_vertex"}
        ):
            continue
        adjacency.setdefault(rel["start"], set()).add(rel["end"])
        adjacency.setdefault(rel["end"], set()).add(rel["start"])
    node_ids = {node["id"] for node in graph["nodes"]}
    seen = roots & node_ids
    pending = list(seen)
    while pending:
        current = pending.pop()
        for neighbour in adjacency.get(current, ()):
            if neighbour not in seen:
                seen.add(neighbour)
                pending.append(neighbour)
    return seen


def graph_component(graph: Any, roots: Iterable[str]) -> dict[str, Any]:
    """Return complete topology components rooted at stable body UUIDs."""
    canonical = validate_graph(graph or {"nodes": [], "rels": []})
    root_ids = {str(value).strip() for value in roots if str(value).strip()}
    scope = _component_ids(canonical, root_ids)
    return validate_graph(
        {
            "nodes": [node for node in canonical["nodes"] if node["id"] in scope],
            "rels": [
                rel
                for rel in canonical["rels"]
                if rel["start"] in scope and rel["end"] in scope
            ],
            **{
                key: deepcopy(value)
                for key, value in canonical.items()
                if key not in {"nodes", "rels"}
            },
        }
    )


def merge_graph(
    base_graph: Any,
    incoming_graph: Any,
    changes: Iterable[Any] | None = None,
    removed_ids: Iterable[str] | None = None,
) -> dict[str, Any]:
    """Apply a client graph delta to a base graph.

    A change is scoped to a node ID.  Replacing a changed node also replaces
    every relationship incident to it.  This keeps the operation deterministic
    and avoids silently retaining stale dependency edges.
    """
    base = validate_graph(base_graph or {"nodes": [], "rels": []})
    incoming = validate_graph(incoming_graph or {"nodes": [], "rels": []})
    changed_roots = _changed_ids(changes, incoming)
    removed_roots = {
        str(value).strip() for value in (removed_ids or []) if str(value).strip()
    }
    changed_roots -= removed_roots

    # Replace complete topology components.  This is the crucial difference
    # from a UUID dictionary merge: ACIS child IDs are not cross-client keys.
    base_scope = _component_ids(base, changed_roots | removed_roots)
    incoming_scope = _component_ids(incoming, changed_roots)
    nodes_by_id = {
        node["id"]: node for node in base["nodes"] if node["id"] not in base_scope
    }
    for node in incoming["nodes"]:
        if node["id"] in incoming_scope:
            nodes_by_id[node["id"]] = deepcopy(node)

    old_rels = [
        rel
        for rel in base["rels"]
        if rel["start"] in nodes_by_id
        and rel["end"] in nodes_by_id
        and rel["start"] not in base_scope
        and rel["end"] not in base_scope
    ]
    new_rels = [
        deepcopy(rel)
        for rel in incoming["rels"]
        if rel["start"] in incoming_scope
        and rel["end"] in incoming_scope
        and rel["start"] in nodes_by_id
        and rel["end"] in nodes_by_id
    ]
    merged = {
        "nodes": list(nodes_by_id.values()),
        "rels": old_rels + new_rels,
    }
    # Keep metadata from the base version unless the client explicitly sends
    # a replacement value (for example a document schema version).
    for key, value in base.items():
        if key not in {"nodes", "rels"}:
            merged[key] = deepcopy(value)
    for key, value in incoming.items():
        if key not in {"nodes", "rels"}:
            merged[key] = deepcopy(value)
    # Legacy components cannot acquire a SAT backup on the server (no ACIS).
    # Validate incoming v2 strictly above, and keep the materialized graph v1
    # until every old root has been replaced with a hybrid component.
    if merged.get("schema") == "dbcad.entity_graph.v2" and any(
        ("body" in node["labels"] or node["props"].get("top_level"))
        and not node["props"].get("sat_fragment")
        for node in merged["nodes"]
    ):
        merged["schema"] = "dbcad.entity_graph.v1"
    return validate_graph(merged)
