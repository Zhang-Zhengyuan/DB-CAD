"""Graph endpoints for the administrator dashboard (V1 entity projection)."""

from typing import Any
from fastapi import APIRouter, HTTPException, Query
from .support import _run_cypher, _run_single_cypher, _semantic_label

router = APIRouter()


@router.get("/components/{hash_sat}/entity-graph")
def get_component_entity_graph(
    hash_sat: str,
    label: str | None = Query(default=None),
    limit: int = Query(default=400, ge=1, le=2000),
) -> dict[str, Any]:
    """组件实体级投影图（P6/ADR-3）：实体节点 + TOPOLOGY 拓扑边。

    label 过滤（face/Face 皆可）只筛节点，边保留全图拓扑中两端都可见的边；
    大组件截断到 limit 并如实上报 truncated。
    """
    component = _run_single_cypher(
        "MATCH (c:V1Component {hash_sat: $h}) RETURN c.hash_sat AS hash_sat, c.name AS name",
        {"h": hash_sat},
    )
    if component is None:
        raise HTTPException(status_code=404, detail=f"Component '{hash_sat[:12]}' not found")

    filter_clause = ""
    params: dict[str, Any] = {"h": hash_sat, "limit": limit}
    if label:
        filter_clause = "WHERE $needle IN [l IN labels(e) | toUpper(l)]"
        params["needle"] = label.upper()

    nodes = _run_cypher(
        f"""
        MATCH (c:V1Component {{hash_sat: $h}})-[:HAS_ENTITY]->(e:V1Entity)
        {filter_clause}
        RETURN e.entity_id AS entity_id, labels(e) AS labels, e.props_json AS props_json,
               e.native_skipped AS native_skipped
        ORDER BY entity_id
        LIMIT $limit
        """,
        params,
    )
    visible = {str(n["entity_id"]) for n in nodes}
    rels = _run_cypher(
        """
        MATCH (c:V1Component {hash_sat: $h})-[:HAS_ENTITY]->(s:V1Entity)-[t:TOPOLOGY]->(e:V1Entity)
        RETURN s.entity_id AS start, e.entity_id AS end, t.rel_type AS rel_type
        """,
        {"h": hash_sat},
    )
    total_entities = _run_cypher(
        "MATCH (:V1Component {hash_sat: $h})-[:HAS_ENTITY]->(e) RETURN count(e) AS c",
        {"h": hash_sat},
    )

    import json as _json

    node_items: list[dict[str, Any]] = []
    for node in nodes:
        labels = [str(l) for l in (node.get("labels") or []) if l != "V1Entity"]
        try:
            props = _json.loads(str(node.get("props_json") or "{}"))
        except (TypeError, ValueError):
            props = {}
        geometry = props.get("geometry") if isinstance(props.get("geometry"), dict) else {}
        node_items.append(
            {
                "id": str(node["entity_id"]),
                "labels": labels,
                "semantic": _semantic_label(labels, props),
                "entity_type": str(props.get("entity_type") or ""),
                "geometry_type": str(geometry.get("type") or ""),
                "top_level": bool(props.get("top_level")),
            }
        )

    return {
        "hash_sat": str(component.get("hash_sat") or ""),
        "name": str(component.get("name") or ""),
        "total_entities": int(total_entities[0]["c"]) if total_entities else 0,
        "returned": len(node_items),
        "truncated": len(nodes) >= limit,
        "label_filter": label,
        "nodes": node_items,
        "rels": [
            {"start": str(r["start"]), "end": str(r["end"]), "type": str(r["rel_type"])}
            for r in rels
            if str(r["start"]) in visible and str(r["end"]) in visible
        ],
    }
