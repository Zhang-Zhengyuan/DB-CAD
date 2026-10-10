"""Graph endpoints for the administrator dashboard."""

import json
from typing import Any
from fastapi import APIRouter, HTTPException, Query
from .support import _run_cypher, _run_single_cypher, _semantic_label

router = APIRouter()


@router.get("/projects/{project_name}/part-topology")
def get_part_topology(
    project_name: str,
    part_name: str | None = Query(default=None),
    max_depth: int = Query(default=8, ge=1, le=20),
) -> dict[str, Any]:
    """
    查询某项目某版本的 part 拓扑树状分布。
    若 part_name 未指定，则采用该项目最新 Mode0 版本的 part_name。
    """
    target_part = part_name
    if not target_part:
        latest = _run_single_cypher(
            """
            MATCH (p)-[:HAS_VERSION]->(v)
            WHERE labels(p)[0] IN ['BridgeProject', 'DBCADProject', 'Project']
              AND p.name = $name
              AND labels(v)[0] IN ['BridgeVersion', 'DBCADVersion']
            RETURN v.part_name AS pn
            ORDER BY v.version DESC LIMIT 1
            """,
            {"name": project_name},
        )
        target_part = (latest or {}).get("pn") or ""

    if not target_part:
        raise HTTPException(
            status_code=404, detail=f"No part topology for '{project_name}'"
        )

    depth_stats = _run_cypher(
        f"""
        MATCH (root)-[:part_entity_ptr*1..{int(max_depth)}]->(desc)
        WHERE labels(root)[0] IN ['part', 'DBCADPart'] AND root.b = $pn
        WITH labels(desc)[0] AS label, count(*) AS n
        RETURN label, n ORDER BY n DESC
        """,
        {"pn": target_part},
    )

    bodies = _run_cypher(
        """
        MATCH (root)-[:part_entity_ptr]->(b:body)
        WHERE labels(root)[0] IN ['part', 'DBCADPart'] AND root.b = $pn
        RETURN b.uuid AS uuid
        ORDER BY b.uuid
        """,
        {"pn": target_part},
    )

    # 链路分布（每条 body 链上的节点数）
    chains = _run_cypher(
        f"""
        MATCH (root)-[:part_entity_ptr]->(b:body)
        WHERE labels(root)[0] IN ['part', 'DBCADPart'] AND root.b = $pn
        OPTIONAL MATCH (b)-[:part_entity_ptr*1..{int(max_depth)}]->(desc)
        WITH b.uuid AS body_uuid, count(desc) AS chain_size
        RETURN body_uuid, chain_size ORDER BY chain_size DESC LIMIT 20
        """,
        {"pn": target_part},
    )

    return {
        "part_name": target_part,
        "max_depth": int(max_depth),
        "total_nodes_by_label": [
            {"label": d["label"], "count": int(d["n"] or 0)} for d in depth_stats
        ],
        "body_count": len(bodies),
        "bodies": [{"uuid": str(b["uuid"] or "")} for b in bodies],
        "top_chains": [
            {"uuid": c["body_uuid"], "chain_size": int(c["chain_size"] or 0)}
            for c in chains
        ],
    }


@router.get("/projects/{project_name}/neo4j-state")
def get_neo4j_graph_state(project_name: str) -> dict[str, Any]:
    """Return the latest JSON entity graph, or historical part graph state."""
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

    json_latest = _run_single_cypher(
        "MATCH (v:JsonGraphVersion {project_id: $pid}) "
        "RETURN v.version AS version, v.created_at AS created_at "
        "ORDER BY v.version DESC LIMIT 1",
        {"pid": pid},
    )
    if json_latest is not None:
        buckets = _run_cypher(
            "MATCH (v:JsonGraphVersion {project_id: $pid, version: $version})"
            "-[:HAS_COMPONENT]->(c:JsonGraphComponent)-[:HAS_JSON_ENTITY]->(e:JsonGraphEntity) "
            "RETURN e.labels_json AS label, count(e) AS n ORDER BY n DESC",
            {"pid": pid, "version": int(json_latest["version"])},
        )
        body_rows = _run_cypher(
            "MATCH (v:JsonGraphVersion {project_id: $pid, version: $version})"
            "-[:HAS_COMPONENT]->(c:JsonGraphComponent)-[:HAS_JSON_ENTITY]->(e:JsonGraphEntity) "
            "WHERE e.labels_json CONTAINS 'body' "
            "RETURN e.entity_id AS uuid LIMIT 1000",
            {"pid": pid, "version": int(json_latest["version"])},
        )
        history = _run_cypher(
            "MATCH (v:JsonGraphVersion {project_id: $pid}) "
            "OPTIONAL MATCH (v)-[:HAS_COMPONENT]->(c:JsonGraphComponent) "
            "OPTIONAL MATCH (v)-[:HAS_RELATION]->(r:JsonGraphRelation) "
            "RETURN v.version AS version, v.storage_mode AS storage_mode, "
            "count(DISTINCT c) AS component_count, count(DISTINCT r) AS relation_count "
            "ORDER BY v.version ASC",
            {"pid": pid},
        )
        graph_nodes = _run_cypher(
            "MATCH (v:JsonGraphVersion {project_id: $pid, version: $version})"
            "-[:HAS_COMPONENT]->(:JsonGraphComponent)-[:HAS_JSON_ENTITY]->(e:JsonGraphEntity) "
            "RETURN e.entity_id AS id, e.labels_json AS labels_json, e.props_json AS props_json "
            "ORDER BY e.entity_id LIMIT 180",
            {"pid": pid, "version": int(json_latest["version"])},
        )
        graph_rels = _run_cypher(
            "MATCH (v:JsonGraphVersion {project_id: $pid, version: $version})"
            "-[:HAS_COMPONENT]->(:JsonGraphComponent)-[:HAS_JSON_ENTITY]->(s:JsonGraphEntity)"
            "-[r:JSON_TO]->(e:JsonGraphEntity) "
            "RETURN s.entity_id AS start, e.entity_id AS end, r.type AS type LIMIT 360",
            {"pid": pid, "version": int(json_latest["version"])},
        )
        return {
            "project_id": pid,
            "storage_label": "Mode1 JSON graph",
            "has_part": True,
            "part_name": "JSON graph",
            "part_version": int(json_latest["version"]),
            "total_nodes": sum(int(item["n"]) for item in buckets),
            "buckets": [
                {
                    "label": ",".join(json.loads(item["label"])),
                    "label_zh": _semantic_label(json.loads(item["label"])),
                    "count": int(item["n"]),
                }
                for item in buckets
            ],
            "bodies": [{"uuid": str(item["uuid"])} for item in body_rows],
            "body_count": len(body_rows),
            "graph": {
                "nodes": [
                    {
                        "id": str(item["id"]),
                        "labels": json.loads(item["labels_json"]),
                        "props": json.loads(item["props_json"]),
                    }
                    for item in graph_nodes
                ],
                "rels": [
                    {
                        "start": str(item["start"]),
                        "end": str(item["end"]),
                        "type": str(item["type"]),
                    }
                    for item in graph_rels
                ],
            },
            "history_chain": [
                {
                    "version": int(item.get("version") or 0),
                    "storage_mode": str(item.get("storage_mode") or "legacy_snapshot"),
                    "component_count": int(item.get("component_count") or 0),
                    "relation_count": int(item.get("relation_count") or 0),
                }
                for item in history
            ],
        }

    # 优先用最新的 Mode0 版本的 part_name（如果是 Mode1 项目则用 delta 路径）
    # 三种选择：Mode0 / Mode1 / EG，按"最近一次更新"取
    storage_label = ""
    part_name = ""
    part_version = 0

    # 1. Mode0 最新
    m0 = _run_single_cypher(
        """
        MATCH (p)-[:HAS_VERSION]->(v)
        WHERE labels(p)[0] IN ['BridgeProject','DBCADProject']
          AND p.name = $name
          AND labels(v)[0] IN ['BridgeVersion','DBCADVersion']
        RETURN v.part_name AS pn, v.version AS v
        ORDER BY v.version DESC LIMIT 1
        """,
        {"name": project_name},
    )
    # 2. EG 最新
    eg = _run_single_cypher(
        """
        MATCH (p)-[:HAS_EG_VERSION]->(egv)
        WHERE labels(p)[0] IN ['BridgeProject','DBCADProject']
          AND p.name = $name
          AND labels(egv)[0] IN ['BridgeEntityGraphVersion','entity_graph_version']
        RETURN egv.part_name AS pn, egv.version AS v, egv.created_at AS ca
        ORDER BY egv.version DESC LIMIT 1
        """,
        {"name": project_name},
    )
    # 3. Mode1（按 project_id 属性）
    m1 = _run_single_cypher(
        """
        MATCH (d)
        WHERE labels(d)[0] IN ['BridgeDeltaVersion','DBCADDelta']
          AND d.project_id = $pid
        RETURN d.version AS v, d.created_at AS ca
        ORDER BY d.version DESC LIMIT 1
        """,
        {"pid": pid},
    )

    # 按 created_at 选最新
    candidates = []
    if m0:
        candidates.append(
            ("Mode0 (SAT 全量)", m0.get("pn") or "", int(m0.get("v") or 0), None)
        )
    if eg:
        candidates.append(
            ("Entity Graph", eg.get("pn") or "", int(eg.get("v") or 0), eg.get("ca"))
        )
    if m1:
        # Mode1 的 part_name 模式: "delta__{project_id}"，但 part 节点可能没创建
        m1_part_name = f"delta__{pid}"
        candidates.append(
            ("Mode1 (Delta 增量)", m1_part_name, int(m1.get("v") or 0), m1.get("ca"))
        )

    if not candidates:
        return {
            "project_id": pid,
            "storage_label": "",
            "has_part": False,
            "part_name": "",
            "part_version": 0,
            "total_nodes": 0,
            "buckets": [],
            "bodies": [],
            "body_count": 0,
        }

    # 简单按 version 选最大的（实际可按 created_at 更准）
    storage_label, part_name, part_version, _ = max(candidates, key=lambda x: x[2])

    # 查询 part 子图节点分布
    buckets_rows = _run_cypher(
        """
        MATCH (root)-[:part_entity_ptr*1..8]->(desc)
        WHERE labels(root)[0] IN ['part','DBCADPart'] AND root.b = $pn
        WITH labels(desc)[0] AS l, count(*) AS n
        RETURN l, n ORDER BY n DESC
        """,
        {"pn": part_name},
    )
    total = sum(int(b["n"] or 0) for b in buckets_rows)
    buckets = [{"label": b["l"], "count": int(b["n"] or 0)} for b in buckets_rows]

    bodies = _run_cypher(
        """
        MATCH (root)-[:part_entity_ptr]->(b:body)
        WHERE labels(root)[0] IN ['part','DBCADPart'] AND root.b = $pn
        RETURN b.uuid AS uuid
        ORDER BY b.uuid
        LIMIT 100
        """,
        {"pn": part_name},
    )

    return {
        "project_id": pid,
        "storage_label": storage_label,
        "has_part": total > 0,
        "part_name": part_name,
        "part_version": part_version,
        "total_nodes": total,
        "buckets": buckets,
        "bodies": [{"uuid": str(b["uuid"] or "")} for b in bodies],
        "body_count": len(bodies),
    }
