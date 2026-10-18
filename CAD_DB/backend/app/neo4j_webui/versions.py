"""Versions endpoints for the administrator dashboard."""

import json
from typing import Any
from fastapi import APIRouter, HTTPException, Query
from .support import _run_cypher, _run_single_cypher, _iso

router = APIRouter()


@router.get("/projects/{project_name}/mode0/{version}")
def get_mode0_version_detail(
    project_name: str, version: int, include_raw: bool = Query(False)
) -> dict[str, Any]:
    """查询某 Mode0 版本的元数据与 part 名引用。Mode0 没有 content 文本，留作引用追溯。"""
    row = _run_single_cypher(
        """
        MATCH (p)-[:HAS_VERSION]->(v)
        WHERE labels(p)[0] IN ['BridgeProject', 'DBCADProject', 'Project']
          AND p.name = $name
          AND labels(v)[0] IN ['BridgeVersion', 'DBCADVersion', 'ModelVersion']
          AND v.version = $version
        RETURN v, p.name AS pname
        """,
        {"name": project_name, "version": version},
    )
    if not row:
        raise HTTPException(
            status_code=404, detail=f"Mode0 v{version} not found for '{project_name}'"
        )
    v = dict(row["v"])
    part_name = str(v.get("part_name") or "")
    topology = _run_cypher(
        """
        MATCH (root)-[:part_entity_ptr*1..6]->(desc)
        WHERE labels(root)[0] IN ['part', 'DBCADPart'] AND root.b = $pn
        WITH labels(desc)[0] AS l, count(*) AS n
        RETURN l, n ORDER BY n DESC
        """,
        {"pn": part_name},
    )
    stored: dict[str, Any] = {}
    try:
        parsed = json.loads(str(v.get("content_json") or "{}"))
        if isinstance(parsed, dict):
            stored = parsed
    except (TypeError, ValueError):
        pass
    try:
        audit = json.loads(str(v.get("audit_json") or "{}"))
    except (TypeError, ValueError):
        audit = {}
    result = {
        "version": int(v.get("version") or 0),
        "author": str(v.get("author") or ""),
        "created_at": _iso(v.get("created_at")),
        "project_id": str(v.get("project_id") or ""),
        "part_name": part_name,
        "topology_by_label": [
            {"label": t["l"], "count": int(t["n"] or 0)} for t in topology
        ],
        "payload_format": str(audit.get("payload_format") or "legacy"),
        "contains_sat": bool(audit.get("contains_sat", bool(stored.get("sat")))),
        "request_bytes": int(audit.get("request_bytes") or 0),
        "sat_bytes": len(str(stored.get("sat") or "").encode("utf-8")),
        "db_write_ms": audit.get("db_write_ms"),
    }
    if include_raw:
        result["raw_push_json"] = audit.get("request_json")
        result["raw_sat"] = stored.get("sat")
    return result


@router.get("/projects/{project_name}/mode1/{version}")
def get_mode1_detail(project_name: str, version: int) -> dict[str, Any]:
    """查询某 delta 的完整内容（JSON 反序列化预览）。"""
    project_row = _run_single_cypher(
        """
        MATCH (p)
        WHERE labels(p)[0] IN ['BridgeProject', 'DBCADProject', 'Project']
          AND p.name = $name
        RETURN p.id AS pid
        """,
        {"name": project_name},
    )
    if not project_row:
        raise HTTPException(
            status_code=404, detail=f"Project '{project_name}' not found"
        )
    pid = project_row.get("pid") or ""

    row = _run_single_cypher(
        """
        MATCH (d)
        WHERE labels(d)[0] IN ['BridgeDeltaVersion', 'DBCADDelta']
          AND d.project_id = $pid AND d.version = $version
        RETURN d
        """,
        {"pid": pid, "version": version},
    )
    if not row:
        raise HTTPException(
            status_code=404, detail=f"Mode1 v{version} not found for '{project_name}'"
        )
    d = dict(row["d"])
    content = str(d.get("content_text") or "{}")
    parsed: Any
    try:
        parsed = json.loads(content)
    except Exception:
        parsed = {"raw": content}
    return {
        "version": int(d.get("version") or 0),
        "author": str(d.get("author") or ""),
        "created_at": _iso(d.get("created_at")),
        "project_id": str(d.get("project_id") or ""),
        "raw_length": len(content),
        "parsed_content": parsed,
    }


@router.get("/projects/{project_name}/eg/{version}")
def get_eg_version_detail(project_name: str, version: int) -> dict[str, Any]:
    """查询某 Entity Graph 版本的元数据与图结构概览（节点/关系分类计数）。"""
    row = _run_single_cypher(
        """
        MATCH (p)-[:HAS_EG_VERSION]->(egv)
        WHERE labels(p)[0] IN ['BridgeProject', 'DBCADProject', 'Project']
          AND p.name = $name
          AND labels(egv)[0] IN ['BridgeEntityGraphVersion', 'entity_graph_version']
          AND egv.version = $version
        RETURN egv, p.id AS pid
        """,
        {"name": project_name, "version": version},
    )
    if not row:
        raise HTTPException(
            status_code=404,
            detail=f"EntityGraph v{version} not found for '{project_name}'",
        )
    egv = dict(row["egv"])
    graph_text = str(egv.get("entity_graph_text") or "{}")
    parsed: dict[str, Any]
    try:
        parsed = json.loads(graph_text)
        node_count = len(parsed.get("nodes", []))
        rel_count = len(parsed.get("rels", []))
        # 按 label 分类
        label_count: dict[str, int] = {}
        for n in parsed.get("nodes", []):
            for lab in n.get("labels", []):
                label_count[lab] = label_count.get(lab, 0) + 1
        rel_type_count: dict[str, int] = {}
        for r in parsed.get("rels", []):
            t = r.get("type", "?")
            rel_type_count[t] = rel_type_count.get(t, 0) + 1
    except Exception:
        parsed = {"raw": graph_text}
        node_count = 0
        rel_count = 0
        label_count = {}
        rel_type_count = {}

    return {
        "version": int(egv.get("version") or 0),
        "author": str(egv.get("author") or ""),
        "created_at": _iso(egv.get("created_at")),
        "project_id": str(egv.get("project_id") or ""),
        "part_name": str(egv.get("part_name") or ""),
        "graph_bytes": len(graph_text),
        "node_count": node_count,
        "rel_count": rel_count,
        "label_breakdown": [
            {"label": k, "count": v}
            for k, v in sorted(label_count.items(), key=lambda x: -x[1])
        ],
        "rel_type_breakdown": [
            {"type": k, "count": v}
            for k, v in sorted(rel_type_count.items(), key=lambda x: -x[1])
        ],
    }


@router.get("/projects/{project_name}/json/{version}")
def get_json_version_detail(
    project_name: str, version: int, include_raw: bool = Query(False)
) -> dict[str, Any]:
    """A bounded summary of one queryable Mode1 JSON version."""
    row = _run_single_cypher(
        "MATCH (p {name: $name})-[:HAS_JSON_GRAPH_VERSION]->"
        "(v:JsonGraphVersion {version: $version}) "
        "RETURN v.project_id AS project_id, v.version AS version, "
        "v.author AS author, v.created_at AS created_at, "
        "size(coalesce(v.delta_json, v.content_json)) AS manifest_chars, v.graph_hash AS graph_hash",
        {"name": project_name, "version": version},
    )
    if row is None:
        raise HTTPException(status_code=404, detail="JSON version not found")
    # Phase B v2：优先按原生 Neo4j 标签聚合（可在库端按标签查询）；
    # legacy 行（迁移前）退回 labels_json。
    counts = _run_cypher(
        "MATCH (v:JsonGraphVersion {project_id: $pid, version: $version})"
        "-[:HAS_COMPONENT]->(c:JsonGraphComponent)-[:HAS_JSON_ENTITY]->(e:JsonGraphEntity) "
        "RETURN [l IN labels(e) WHERE l <> 'JsonGraphEntity'] AS native_labels, "
        "e.labels_json AS labels_json, count(e) AS count "
        "ORDER BY count DESC",
        {"pid": row["project_id"], "version": version},
    )
    components = _run_cypher(
        "MATCH (v:JsonGraphVersion {project_id: $pid, version: $version})"
        "-[hc:HAS_COMPONENT]->(c:JsonGraphComponent) "
        "RETURN c.component_hash AS component_hash, hc.root_uuid AS root_uuid, "
        "c.node_count AS node_count, c.rel_count AS rel_count, c.created_at AS created_at, "
        "c.native_field_count AS native_field_count, c.native_skipped_count AS native_skipped_count "
        "ORDER BY root_uuid",
        {"pid": row["project_id"], "version": version},
    )
    relation_count = _run_single_cypher(
        "MATCH (v:JsonGraphVersion {project_id: $pid, version: $version})"
        "-[:HAS_RELATION]->(r:JsonGraphRelation) RETURN count(r) AS count",
        {"pid": row["project_id"], "version": version},
    )
    version_meta = (
        _run_single_cypher(
            "MATCH (v:JsonGraphVersion {project_id: $pid, version: $version}) "
            "RETURN v.storage_mode AS storage_mode, v.delta_json AS delta_json, "
            "v.db_write_ms AS db_write_ms",
            {"pid": row["project_id"], "version": version},
        )
        or {}
    )
    delta: Any = {}
    try:
        delta = json.loads(version_meta.get("delta_json") or "{}")
    except (TypeError, ValueError):
        delta = {"raw": str(version_meta.get("delta_json") or "")}
    safe_delta = dict(delta) if isinstance(delta, dict) else {}
    safe_audit = dict(safe_delta.get("audit") or {})
    raw_request_json = safe_audit.pop("request_json", None)
    if "audit" in safe_delta:
        safe_delta["audit"] = safe_audit
    result = {
        **{
            key: (_iso(value) if key == "created_at" else value)
            for key, value in row.items()
        },
        "storage_mode": version_meta.get("storage_mode") or "legacy_snapshot",
        "delta": safe_delta,
        "component_count": len(components),
        "components": [
            {
                "component_hash": str(item.get("component_hash") or ""),
                "root_uuid": str(item.get("root_uuid") or ""),
                "node_count": int(item.get("node_count") or 0),
                "rel_count": int(item.get("rel_count") or 0),
                "created_at": _iso(item.get("created_at")),
            }
            for item in components
        ],
        "cross_component_relation_count": int((relation_count or {}).get("count") or 0),
        "entity_counts": [
            {
                "labels": (
                    [str(label) for label in item["native_labels"]]
                    if item.get("native_labels")
                    else json.loads(item["labels_json"])
                ),
                "count": int(item["count"]),
                "native": bool(item.get("native_labels")),
            }
            for item in counts
        ],
        "native_coverage": {
            "fields": sum(int(item.get("native_field_count") or 0) for item in components),
            "skipped_fields": sum(int(item.get("native_skipped_count") or 0) for item in components),
        },
        "payload_format": str(safe_audit.get("payload_format") or "json"),
        "contains_sat": bool(safe_audit.get("contains_sat", False)),
        "request_bytes": int(safe_audit.get("request_bytes") or 0),
        "db_write_ms": version_meta.get("db_write_ms"),
    }
    if include_raw:
        raw_row = (
            _run_single_cypher(
                "MATCH (v:JsonGraphVersion {project_id: $pid, version: $version}) "
                "RETURN v.request_json AS request_json",
                {"pid": row["project_id"], "version": version},
            )
            or {}
        )
        result["raw_push_json"] = raw_row.get("request_json") or raw_request_json
    return result
