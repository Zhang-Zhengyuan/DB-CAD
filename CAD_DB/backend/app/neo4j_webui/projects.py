"""Projects endpoints for the administrator dashboard."""

import json
from typing import Any
from fastapi import APIRouter, HTTPException
from pydantic import BaseModel
from .support import _run_cypher, _run_single_cypher, _iso, _decode_changes

router = APIRouter()


class ProjectSummary(BaseModel):
    name: str
    project_id: str
    created_at: str
    updated_at: str
    mode0_version_count: int
    mode0_latest: int | None
    mode1_json_count: int
    mode1_latest: int | None
    legacy_delta_count: int
    entity_graph_version_count: int
    entity_graph_latest: int | None


@router.get("/projects", response_model=list[ProjectSummary])
def list_projects() -> list[ProjectSummary]:
    """
    列出所有项目，并附带三种模式版本计数。

    数据真实性修正（与 Neo4j 实测一致）：
      - ModelVersion / BridgeVersion 通过 :HAS_VERSION 关系挂到项目
      - JsonGraphVersion 通过 :HAS_JSON_GRAPH_VERSION 关系挂到项目
      - BridgeEntityGraphVersion 通过 :HAS_EG_VERSION 关系挂到项目
      - BridgeDeltaVersion 没有 BELONGS_TO 关系，按 project_id 属性匹配
    """
    rows = _run_cypher(
        """
        MATCH (p)
        WHERE labels(p)[0] IN ['BridgeProject', 'DBCADProject', 'Project']
        WITH p, labels(p)[0] AS label
        // Mode0：通过关系
        OPTIONAL MATCH (p)-[:HAS_VERSION]->(v)
        WHERE labels(v)[0] IN ['BridgeVersion', 'DBCADVersion', 'ModelVersion']
        WITH p, label,
             count(DISTINCT v) AS m0_count,
             max(v.version) AS m0_latest
        // Historical SAT delta, retained for migration audit.
        OPTIONAL MATCH (d)
        WHERE labels(d)[0] IN ['BridgeDeltaVersion', 'DBCADDelta']
          AND (d.project_id = p.id OR d.project_id = p.project_id)
        WITH p, label, m0_count, m0_latest,
             count(DISTINCT d) AS legacy_delta_count
        OPTIONAL MATCH (p)-[:HAS_JSON_GRAPH_VERSION]->(j:JsonGraphVersion)
        WITH p, label, m0_count, m0_latest, legacy_delta_count,
             count(DISTINCT j) AS m1_count,
             max(j.version) AS m1_latest
        // Entity Graph：通过关系
        OPTIONAL MATCH (p)-[:HAS_EG_VERSION]->(egv)
        WHERE labels(egv)[0] IN ['BridgeEntityGraphVersion', 'entity_graph_version']
        WITH p, label, m0_count, m0_latest, legacy_delta_count, m1_count, m1_latest,
             count(DISTINCT egv) AS eg_count,
             max(egv.version) AS eg_latest
        RETURN p.name AS name,
               p.id AS project_id,
               p.created_at AS created_at,
               COALESCE(p.updated_at, p.created_at) AS updated_at,
               m0_count, m0_latest, legacy_delta_count, m1_count, m1_latest, eg_count, eg_latest
        ORDER BY updated_at DESC
        LIMIT 500
        """
    )
    out: list[ProjectSummary] = []
    for r in rows:
        out.append(
            ProjectSummary(
                name=str(r.get("name") or ""),
                project_id=str(r.get("project_id") or ""),
                created_at=_iso(r.get("created_at")),
                updated_at=_iso(r.get("updated_at")),
                mode0_version_count=int(r.get("m0_count") or 0),
                mode0_latest=int(r["m0_latest"])
                if r.get("m0_latest") is not None
                else None,
                mode1_json_count=int(r.get("m1_count") or 0),
                mode1_latest=int(r["m1_latest"])
                if r.get("m1_latest") is not None
                else None,
                legacy_delta_count=int(r.get("legacy_delta_count") or 0),
                entity_graph_version_count=int(r.get("eg_count") or 0),
                entity_graph_latest=int(r["eg_latest"])
                if r.get("eg_latest") is not None
                else None,
            )
        )
    return out


@router.get("/projects/{project_name}")
def get_project_detail(project_name: str) -> dict[str, Any]:
    """Return project metadata, current JSON versions, and legacy history."""
    project_row = _run_single_cypher(
        """
        MATCH (p)
        WHERE labels(p)[0] IN ['BridgeProject', 'DBCADProject', 'Project']
          AND p.name = $name
        RETURN p
        """,
        {"name": project_name},
    )
    if not project_row:
        raise HTTPException(
            status_code=404, detail=f"Project '{project_name}' not found"
        )
    p = project_row["p"]
    pid = str(p.get("id") or p.get("project_id") or "")

    # Mode0 版本：通过关系
    mode0_rows = _run_cypher(
        """
        MATCH (p)-[:HAS_VERSION]->(v)
        WHERE labels(p)[0] IN ['BridgeProject', 'DBCADProject', 'Project']
          AND p.name = $name
          AND labels(v)[0] IN ['BridgeVersion', 'DBCADVersion', 'ModelVersion']
        RETURN v.version AS version, v.author AS author,
               v.created_at AS created_at,
               v.part_name AS part_name,
               v.project_id AS vp_id
        ORDER BY v.version ASC
        """,
        {"name": project_name},
    )

    # Historical SAT delta: retained for audit; the UI uses JSON versions.
    delta_rows = _run_cypher(
        """
        MATCH (d)
        WHERE labels(d)[0] IN ['BridgeDeltaVersion', 'DBCADDelta']
          AND d.project_id = $pid
        RETURN d.version AS version, d.author AS author,
               d.created_at AS created_at,
               d.content_text AS content_text,
               d.project_id AS dp_id
        ORDER BY d.version ASC
        """,
        {"pid": pid},
    )
    json_rows = _run_cypher(
        "MATCH (v:JsonGraphVersion {project_id: $pid}) "
        "RETURN v.version AS version, v.author AS author, "
        "v.created_at AS created_at, size(coalesce(v.delta_json, v.content_json)) AS manifest_chars, "
        "v.storage_mode AS storage_mode "
        "ORDER BY v.version ASC",
        {"pid": pid},
    )
    json_history = _run_cypher(
        "MATCH (v:JsonGraphVersion {project_id: $pid}) "
        "OPTIONAL MATCH (v)-[:HAS_COMPONENT]->(c:JsonGraphComponent) "
        "OPTIONAL MATCH (v)-[:HAS_RELATION]->(r:JsonGraphRelation) "
        "RETURN v.version AS version, v.author AS author, v.created_at AS created_at, "
        "v.storage_mode AS storage_mode, v.graph_hash AS graph_hash, "
        "count(DISTINCT c) AS component_count, count(DISTINCT r) AS relation_count, "
        "collect(DISTINCT c.component_hash) AS component_hashes, v.delta_json AS delta_json "
        "ORDER BY v.version ASC",
        {"pid": pid},
    )

    # Entity Graph 版本：通过关系
    eg_rows = _run_cypher(
        """
        MATCH (p)-[:HAS_EG_VERSION]->(egv)
        WHERE labels(p)[0] IN ['BridgeProject', 'DBCADProject', 'Project']
          AND p.name = $name
          AND labels(egv)[0] IN ['BridgeEntityGraphVersion', 'entity_graph_version']
        RETURN egv.version AS version, egv.author AS author,
               egv.created_at AS created_at,
               egv.part_name AS part_name,
               size(egv.entity_graph_text) AS graph_bytes,
               egv.project_id AS eg_id
        ORDER BY egv.version ASC
        """,
        {"name": project_name},
    )

    # 当前 part（最新 mode0 版本对应的 part 节点）
    current_part = None
    if mode0_rows:
        latest_part_name = mode0_rows[-1].get("part_name") or ""
        if latest_part_name:
            pr = _run_single_cypher(
                """
                MATCH (part)
                WHERE labels(part)[0] IN ['part', 'DBCADPart']
                  AND part.b = $pn
                RETURN part.b AS part_name, part.a AS a
                LIMIT 1
                """,
                {"pn": latest_part_name},
            )
            if pr:
                current_part = {
                    "part_name": str(pr.get("part_name") or ""),
                    "a": str(pr.get("a") or ""),
                }

    # 解析 delta
    parsed_deltas = []
    for d in delta_rows:
        content_text = str(d.get("content_text") or "{}")
        info = {
            "version": int(d.get("version") or 0),
            "author": str(d.get("author") or ""),
            "created_at": _iso(d.get("created_at")),
            "content_bytes": len(content_text),
        }
        if content_text.startswith("{"):
            try:
                parsed = json.loads(content_text)
                info["delta_uuid_count"] = len(parsed.get("delta_uuids", []))
                info["delta_sat_segment_count"] = len(
                    parsed.get("delta_sat_segments", [])
                )
                info["removed_uuid_count"] = len(parsed.get("removed_uuids", []))
                info["preview"] = content_text[:200]
            except Exception:
                info["delta_uuid_count"] = -1
                info["preview"] = content_text[:200]
        else:
            info["preview"] = content_text[:200]
        parsed_deltas.append(info)

    previous_component_hashes: set[str] = set()
    history_chain: list[dict[str, Any]] = []
    for value in json_history:
        hashes = {str(item) for item in (value.get("component_hashes") or []) if item}
        changed_hashes = hashes - previous_component_hashes
        change_items = _decode_changes(value.get("delta_json"))
        history_chain.append(
            {
                "version": int(value.get("version") or 0),
                "author": str(value.get("author") or ""),
                "created_at": _iso(value.get("created_at")),
                "storage_mode": str(value.get("storage_mode") or "legacy_snapshot"),
                "graph_hash": str(value.get("graph_hash") or ""),
                "component_count": int(value.get("component_count") or 0),
                "relation_count": int(value.get("relation_count") or 0),
                "changed_component_count": len(changed_hashes),
                "reused_component_count": max(0, len(hashes) - len(changed_hashes)),
                "changes": change_items,
                "new_entity_types": sorted(
                    {
                        item["entity_type"]
                        for item in change_items
                        if item["change_type"] == "ADD" and item["entity_type"]
                    }
                ),
            }
        )
        previous_component_hashes = hashes

    return {
        "project": {
            "name": str(p.get("name") or ""),
            "id": str(p.get("id") or ""),
            "project_id": str(p.get("project_id") or ""),
            "created_at": _iso(p.get("created_at")),
            "updated_at": _iso(p.get("updated_at") or p.get("created_at")),
        },
        "current_part": current_part,
        "mode0_versions": [
            {
                "version": int(v.get("version") or 0),
                "author": str(v.get("author") or ""),
                "created_at": _iso(v.get("created_at")),
                "part_name": str(v.get("part_name") or ""),
            }
            for v in mode0_rows
        ],
        "legacy_sat_deltas": parsed_deltas,
        "mode1_json_versions": [
            {
                "version": int(v.get("version") or 0),
                "author": str(v.get("author") or ""),
                "created_at": _iso(v.get("created_at")),
                "manifest_chars": int(v.get("manifest_chars") or 0),
                "storage_mode": str(v.get("storage_mode") or "legacy_snapshot"),
            }
            for v in json_rows
        ],
        "mode1_history_chain": history_chain,
        "entity_graph_versions": [
            {
                "version": int(eg.get("version") or 0),
                "author": str(eg.get("author") or ""),
                "created_at": _iso(eg.get("created_at")),
                "part_name": str(eg.get("part_name") or ""),
                "graph_bytes": int(eg.get("graph_bytes") or 0),
            }
            for eg in eg_rows
        ],
    }
