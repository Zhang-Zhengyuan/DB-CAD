"""Versions endpoints for the administrator dashboard (V1 model)."""

import json
from typing import Any
from fastapi import APIRouter, HTTPException
from .support import _run_cypher, _run_single_cypher, _iso

router = APIRouter()


@router.get("/projects/{project_name}/versions/{version}")
def get_version_detail(project_name: str, version: int) -> dict[str, Any]:
    """V1 版本详情：组件清单（含投影统计与 fallback 覆盖标记）+ 实体标签分布。"""
    project_row = _run_single_cypher(
        "MATCH (p:V1Project {name: $name}) RETURN p.id AS pid",
        {"name": project_name},
    )
    if not project_row:
        raise HTTPException(status_code=404, detail=f"Project '{project_name}' not found")
    pid = str(project_row.get("pid") or "")

    meta = _run_single_cypher(
        """
        MATCH (v:V1Version {project_id: $pid, number: $version})
        RETURN v.message AS message, v.author AS author, v.created_at AS created_at,
               v.base_version AS base_version
        """,
        {"pid": pid, "version": version},
    )
    if meta is None:
        raise HTTPException(
            status_code=404, detail=f"Version {version} not found for '{project_name}'"
        )

    components = _run_cypher(
        """
        MATCH (v:V1Version {project_id: $pid, number: $version})-[hc:CONTAINS]->(c:V1Component)
        OPTIONAL MATCH (c)-[:HAS_ENTITY]->(e:V1Entity)
        RETURN hc.order AS order, c.hash_sat AS hash_sat, c.hash_glb AS hash_glb,
               c.hash_graph AS hash_graph, c.name AS name, c.triangles AS triangles,
               c.vertices AS vertices, count(e) AS entity_count,
               head(collect(e.props_json)) AS root_props
        ORDER BY order
        """,
        {"pid": pid, "version": version},
    )
    component_items: list[dict[str, Any]] = []
    for item in components:
        fallback = ""
        root_props = item.get("root_props")
        if root_props:
            try:
                props = json.loads(str(root_props))
                fallback = str(props.get("fallback") or "")
            except (TypeError, ValueError):
                fallback = ""
        component_items.append(
            {
                "hash_sat": str(item.get("hash_sat") or ""),
                "hash_glb": str(item.get("hash_glb") or ""),
                "hash_graph": str(item.get("hash_graph") or ""),
                "name": str(item.get("name") or ""),
                "triangles": int(item.get("triangles") or 0),
                "vertices": int(item.get("vertices") or 0),
                "entity_count": int(item.get("entity_count") or 0),
                "fallback": fallback,
            }
        )

    entity_counts = _run_cypher(
        """
        MATCH (:V1Version {project_id: $pid, number: $version})-[:CONTAINS]->(:V1Component)
              -[:HAS_ENTITY]->(e:V1Entity)
        UNWIND [l IN labels(e) WHERE l <> 'V1Entity'] AS l
        RETURN l AS label, count(*) AS count
        ORDER BY count DESC
        """,
        {"pid": pid, "version": version},
    )

    return {
        "project_id": pid,
        "version": version,
        "message": str(meta.get("message") or ""),
        "author": str(meta.get("author") or ""),
        "created_at": _iso(meta.get("created_at")),
        "base_version": meta.get("base_version"),
        "component_count": len(component_items),
        "components": component_items,
        "entity_counts": [
            {"label": row["label"], "count": int(row["count"] or 0)} for row in entity_counts
        ],
    }
