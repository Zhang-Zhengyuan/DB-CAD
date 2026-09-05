"""
DB-CAD Neo4j 管理 WebUI — 直接连接 Neo4j，提供项目/版本/图数据查询界面。

访问地址: http://localhost:8000/  或 http://localhost:8000/webui
（与 FastAPI 主服务同端口，无需额外启动）

【数据模型】（统一命名，兼容旧 label）
  BridgeProject / DBCADProject  — 项目根节点
  BridgeVersion / DBCADVersion   — Mode0 全量版本（每次 Save 写一份完整 SAT）
  BridgeDeltaVersion / DBCADDelta — Mode1 增量 delta（每次 push 只写增量）
  BridgeEntityGraphVersion / entity_graph_version — Entity Graph 版本
  part / DBCADPart                — ACIS N 叉拓扑树根

【实际关系】（以 Neo4j 实测为准，本版按真实存在的字段与关系重写）
  (BridgeProject)-[:HAS_VERSION]->(BridgeVersion)
  (BridgeProject)-[:HAS_EG_VERSION]->(BridgeEntityGraphVersion)
  BridgeDeltaVersion 没有 BELONGS_TO 关系，仅通过 project_id 属性挂载
  part 拓扑树是孤立节点，通过 BridgeVersion/BridgeEntityGraphVersion.part_name 引用
"""
from __future__ import annotations

import json
import logging
from typing import Any

from fastapi import APIRouter, HTTPException, Query
from fastapi.responses import HTMLResponse, JSONResponse
from pydantic import BaseModel

from .config import settings

logger = logging.getLogger(__name__)

# ---------------------------------------------------------------------------
# Neo4j Driver
# ---------------------------------------------------------------------------

_neo4j_driver = None


def _get_driver():
    global _neo4j_driver
    if _neo4j_driver is None:
        from neo4j import GraphDatabase
        _neo4j_driver = GraphDatabase.driver(
            settings.neo4j_uri or "bolt://localhost:7687",
            auth=(settings.neo4j_user or "neo4j", settings.neo4j_password or "neo4j"),
        )
    return _neo4j_driver


def _close_driver():
    global _neo4j_driver
    if _neo4j_driver is not None:
        _neo4j_driver.close()
        _neo4j_driver = None


# ---------------------------------------------------------------------------
# Helper
# ---------------------------------------------------------------------------

def _run_cypher(query: str, params: dict[str, Any] | None = None) -> list[dict[str, Any]]:
    driver = _get_driver()
    with driver.session(database=settings.neo4j_database) as session:
        result = session.run(query, params or {})
        return [dict(record) for record in result]


def _run_single_cypher(query: str, params: dict[str, Any] | None = None) -> dict[str, Any] | None:
    rows = _run_cypher(query, params)
    return rows[0] if rows else None


def _iso(value: Any) -> str:
    """把 Neo4j datetime/字符串规整到 ISO。"""
    if value is None:
        return ""
    if hasattr(value, "isoformat"):
        return value.isoformat()
    return str(value)


# ---------------------------------------------------------------------------
# 动态 label 探测：避免 OPTIONAL MATCH 不存在的 label 触发 warning
# ---------------------------------------------------------------------------

_EXISTING_LABELS_CACHE: set[str] | None = None


def _existing_labels() -> set[str]:
    """从 Neo4j schema 探测真实存在的 label（首次调用后缓存）。"""
    global _EXISTING_LABELS_CACHE
    if _EXISTING_LABELS_CACHE is not None:
        return _EXISTING_LABELS_CACHE
    labels: set[str] = set()
    # 优先使用 SHOW LABELS（不会对不存在的 label 触发 warning）
    try:
        rows = _run_cypher("SHOW LABELS")
        for r in rows:
            lab = r.get("label")
            if lab:
                labels.add(str(lab))
    except Exception:
        # 后备：使用 COUNT DISTINCT keys(labels(n))
        try:
            rows = _run_cypher(
                """
                MATCH (n)
                UNWIND labels(n) AS l
                WITH l, count(*) AS c
                RETURN l, c
                """
            )
            for r in rows:
                if r.get("l"):
                    labels.add(str(r["l"]))
        except Exception:
            pass
    _EXISTING_LABELS_CACHE = labels
    return labels


def _label_in(lab: str, *alternatives: str) -> str:
    """选取第一个真实存在的 label 用于 MATCH。"""
    existing = _existing_labels()
    for opt in (lab, *alternatives):
        if opt in existing:
            return f"`{opt}`"
    return f"`{lab}`"  # 兜底（仍可能触发 warning，但保证语义）


def _count_with(label_cypher: str) -> int:
    """使用 0-匹配型 count 避免在缺 label 时产生 warning。"""
    try:
        rows = _run_cypher(label_cypher)
        return int(rows[0]["c"]) if rows else 0
    except Exception:
        return 0


def _invalidate_label_cache():
    global _EXISTING_LABELS_CACHE
    _EXISTING_LABELS_CACHE = None


# ---------------------------------------------------------------------------
# 路由
# ---------------------------------------------------------------------------

router = APIRouter(prefix="/api/neo4j", tags=["Neo4j 管理"])


# ---------------------------------------------------------------------------
# 概览统计（含三种存储模式、ACIS 拓扑统计）
# ---------------------------------------------------------------------------

@router.get("/overview")
def get_overview() -> dict[str, Any]:
    """数据库全局概览：节点、分 label 统计、关系、三模式累计版本数。"""
    _invalidate_label_cache()  # 实时刷新，避免新增 label 时不一致
    label_stats = _run_cypher(
        """
        MATCH (n)
        WITH labels(n)[0] AS label, count(*) AS n
        RETURN label, n
        ORDER BY n DESC
        """
    )
    rel_stats = _run_cypher(
        """
        MATCH ()-[r]->()
        WITH type(r) AS rel_type, count(*) AS n
        RETURN rel_type, n
        ORDER BY n DESC
        """
    )

    # 三种存储模式累计版本数（仅查询真实存在的 label，避免 warning）
    proj_label = _label_in("BridgeProject", "DBCADProject")
    mv0_label = _label_in("BridgeVersion", "DBCADVersion")
    mv1_label = _label_in("BridgeDeltaVersion", "DBCADDelta")
    eg_label  = _label_in("BridgeEntityGraphVersion", "entity_graph_version")

    total_projects = _count_with(f"MATCH (n:{proj_label}) RETURN count(n) AS c")
    total_mode0    = _count_with(f"MATCH (n:{mv0_label}) RETURN count(n) AS c")
    total_mode1    = _count_with(f"MATCH (n:{mv1_label}) RETURN count(n) AS c")
    total_eg       = _count_with(f"MATCH (n:{eg_label}) RETURN count(n) AS c")

    total_nodes = sum(int(r["n"] or 0) for r in label_stats)
    total_rels = sum(int(r["n"] or 0) for r in rel_stats)

    # 拓扑节点分类汇总（ACIS 拓扑树 + 几何 + 装配关系）
    topology_buckets = _run_cypher(
        """
        MATCH (n)
        WITH labels(n)[0] AS l, count(*) AS n
        RETURN
            CASE
              WHEN l IN ['part', 'DBCADPart'] THEN '拓扑根'
              WHEN l IN ['body', 'lump', 'shell', 'face', 'loop', 'coedge', 'edge', 'vertex', 'transform', 'point'] THEN '拓扑节点'
              WHEN l IN ['straight-curve','ellipse-curve','int_cur','plane-surface','sphere-surface','cone-surface','spl_sur','curve','surface'] THEN '几何元素'
              WHEN l IN ['BridgeProject','DBCADProject'] THEN '项目'
              WHEN l IN ['BridgeVersion','DBCADVersion'] THEN 'Mode0版本'
              WHEN l IN ['BridgeDeltaVersion','DBCADDelta'] THEN 'Mode1增量'
              WHEN l IN ['BridgeEntityGraphVersion','entity_graph_version'] THEN 'Entity Graph版本'
              WHEN l IN ['entity_node'] THEN 'Entity节点'
              ELSE '其他'
            END AS bucket,
            sum(n) AS total
        ORDER BY total DESC
        """
    )

    return {
        "total_nodes": int(total_nodes),
        "total_relationships": int(total_rels),
        "total_projects": int(total_projects),
        "total_mode0_versions": int(total_mode0),
        "total_mode1_deltas": int(total_mode1),
        "total_entity_graph_versions": int(total_eg),
        "label_counts": [{"label": r["label"], "count": int(r["n"] or 0)} for r in label_stats],
        "relationship_counts": [{"type": r["rel_type"], "count": int(r["n"] or 0)} for r in rel_stats],
        "topology_buckets": [{"bucket": r["bucket"], "count": int(r["total"] or 0)} for r in topology_buckets],
    }


# ---------------------------------------------------------------------------
# 项目列表（每项目带三种模式版本数 + 最新版本号）
# ---------------------------------------------------------------------------

class ProjectSummary(BaseModel):
    name: str
    project_id: str
    created_at: str
    updated_at: str
    mode0_version_count: int
    mode0_latest: int | None
    mode1_delta_count: int
    mode1_latest: int | None
    entity_graph_version_count: int
    entity_graph_latest: int | None


@router.get("/projects", response_model=list[ProjectSummary])
def list_projects() -> list[ProjectSummary]:
    """
    列出所有项目，并附带三种模式版本计数。

    数据真实性修正（与 Neo4j 实测一致）：
      - BridgeVersion 通过 :HAS_VERSION 关系挂到项目
      - BridgeEntityGraphVersion 通过 :HAS_EG_VERSION 关系挂到项目
      - BridgeDeltaVersion 没有 BELONGS_TO 关系，按 project_id 属性匹配
    """
    rows = _run_cypher(
        """
        MATCH (p)
        WHERE labels(p)[0] IN ['BridgeProject', 'DBCADProject']
        WITH p, labels(p)[0] AS label
        // Mode0：通过关系
        OPTIONAL MATCH (p)-[:HAS_VERSION]->(v)
        WHERE labels(v)[0] IN ['BridgeVersion', 'DBCADVersion']
        WITH p, label,
             count(DISTINCT v) AS m0_count,
             max(v.version) AS m0_latest
        // Mode1：按 project_id 属性匹配（实测无 BELONGS_TO 关系）
        OPTIONAL MATCH (d)
        WHERE labels(d)[0] IN ['BridgeDeltaVersion', 'DBCADDelta']
          AND (d.project_id = p.id OR d.project_id = p.project_id)
        WITH p, label, m0_count, m0_latest,
             count(DISTINCT d) AS m1_count,
             max(d.version) AS m1_latest
        // Entity Graph：通过关系
        OPTIONAL MATCH (p)-[:HAS_EG_VERSION]->(egv)
        WHERE labels(egv)[0] IN ['BridgeEntityGraphVersion', 'entity_graph_version']
        WITH p, label, m0_count, m0_latest, m1_count, m1_latest,
             count(DISTINCT egv) AS eg_count,
             max(egv.version) AS eg_latest
        RETURN p.name AS name,
               p.id AS project_id,
               p.created_at AS created_at,
               COALESCE(p.updated_at, p.created_at) AS updated_at,
               m0_count, m0_latest, m1_count, m1_latest, eg_count, eg_latest
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
                mode0_latest=int(r["m0_latest"]) if r.get("m0_latest") is not None else None,
                mode1_delta_count=int(r.get("m1_count") or 0),
                mode1_latest=int(r["m1_latest"]) if r.get("m1_latest") is not None else None,
                entity_graph_version_count=int(r.get("eg_count") or 0),
                entity_graph_latest=int(r["eg_latest"]) if r.get("eg_latest") is not None else None,
            )
        )
    return out


# ---------------------------------------------------------------------------
# 项目完整详情：元数据 + 三模式版本列表
# ---------------------------------------------------------------------------

@router.get("/projects/{project_name}")
def get_project_detail(project_name: str) -> dict[str, Any]:
    """查询某项目的完整信息：元数据 + mode0 版本列表 + mode1 delta 列表 + eg 版本列表。"""
    project_row = _run_single_cypher(
        """
        MATCH (p)
        WHERE labels(p)[0] IN ['BridgeProject', 'DBCADProject']
          AND p.name = $name
        RETURN p
        """,
        {"name": project_name},
    )
    if not project_row:
        raise HTTPException(status_code=404, detail=f"Project '{project_name}' not found")
    p = project_row["p"]
    pid = str(p.get("id") or p.get("project_id") or "")

    # Mode0 版本：通过关系
    mode0_rows = _run_cypher(
        """
        MATCH (p)-[:HAS_VERSION]->(v)
        WHERE labels(p)[0] IN ['BridgeProject', 'DBCADProject']
          AND p.name = $name
          AND labels(v)[0] IN ['BridgeVersion', 'DBCADVersion']
        RETURN v.version AS version, v.author AS author,
               v.created_at AS created_at,
               v.part_name AS part_name,
               v.project_id AS vp_id
        ORDER BY v.version ASC
        """,
        {"name": project_name},
    )

    # Mode1 delta：按 project_id 属性匹配
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

    # Entity Graph 版本：通过关系
    eg_rows = _run_cypher(
        """
        MATCH (p)-[:HAS_EG_VERSION]->(egv)
        WHERE labels(p)[0] IN ['BridgeProject', 'DBCADProject']
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
                info["delta_sat_segment_count"] = len(parsed.get("delta_sat_segments", []))
                info["removed_uuid_count"] = len(parsed.get("removed_uuids", []))
                info["preview"] = content_text[:200]
            except Exception:
                info["delta_uuid_count"] = -1
                info["preview"] = content_text[:200]
        else:
            info["preview"] = content_text[:200]
        parsed_deltas.append(info)

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
        "mode1_deltas": parsed_deltas,
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


# ---------------------------------------------------------------------------
# Mode0 单版本详情
# ---------------------------------------------------------------------------

@router.get("/projects/{project_name}/mode0/{version}")
def get_mode0_version_detail(project_name: str, version: int) -> dict[str, Any]:
    """查询某 Mode0 版本的元数据与 part 名引用。Mode0 没有 content 文本，留作引用追溯。"""
    row = _run_single_cypher(
        """
        MATCH (p)-[:HAS_VERSION]->(v)
        WHERE labels(p)[0] IN ['BridgeProject', 'DBCADProject']
          AND p.name = $name
          AND labels(v)[0] IN ['BridgeVersion', 'DBCADVersion']
          AND v.version = $version
        RETURN v, p.name AS pname
        """,
        {"name": project_name, "version": version},
    )
    if not row:
        raise HTTPException(status_code=404, detail=f"Mode0 v{version} not found for '{project_name}'")
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
    return {
        "version": int(v.get("version") or 0),
        "author": str(v.get("author") or ""),
        "created_at": _iso(v.get("created_at")),
        "project_id": str(v.get("project_id") or ""),
        "part_name": part_name,
        "topology_by_label": [{"label": t["l"], "count": int(t["n"] or 0)} for t in topology],
    }


# ---------------------------------------------------------------------------
# Mode1 delta 单版本详情
# ---------------------------------------------------------------------------

@router.get("/projects/{project_name}/mode1/{version}")
def get_mode1_detail(project_name: str, version: int) -> dict[str, Any]:
    """查询某 delta 的完整内容（JSON 反序列化预览）。"""
    project_row = _run_single_cypher(
        """
        MATCH (p)
        WHERE labels(p)[0] IN ['BridgeProject', 'DBCADProject']
          AND p.name = $name
        RETURN p.id AS pid
        """,
        {"name": project_name},
    )
    if not project_row:
        raise HTTPException(status_code=404, detail=f"Project '{project_name}' not found")
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
        raise HTTPException(status_code=404, detail=f"Mode1 v{version} not found for '{project_name}'")
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


# ---------------------------------------------------------------------------
# Entity Graph 单版本详情
# ---------------------------------------------------------------------------

@router.get("/projects/{project_name}/eg/{version}")
def get_eg_version_detail(project_name: str, version: int) -> dict[str, Any]:
    """查询某 Entity Graph 版本的元数据与图结构概览（节点/关系分类计数）。"""
    row = _run_single_cypher(
        """
        MATCH (p)-[:HAS_EG_VERSION]->(egv)
        WHERE labels(p)[0] IN ['BridgeProject', 'DBCADProject']
          AND p.name = $name
          AND labels(egv)[0] IN ['BridgeEntityGraphVersion', 'entity_graph_version']
          AND egv.version = $version
        RETURN egv, p.id AS pid
        """,
        {"name": project_name, "version": version},
    )
    if not row:
        raise HTTPException(status_code=404, detail=f"EntityGraph v{version} not found for '{project_name}'")
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
        "label_breakdown": [{"label": k, "count": v} for k, v in sorted(label_count.items(), key=lambda x: -x[1])],
        "rel_type_breakdown": [{"type": k, "count": v} for k, v in sorted(rel_type_count.items(), key=lambda x: -x[1])],
    }


# ---------------------------------------------------------------------------
# Part 拓扑树
# ---------------------------------------------------------------------------

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
            WHERE labels(p)[0] IN ['BridgeProject', 'DBCADProject']
              AND p.name = $name
              AND labels(v)[0] IN ['BridgeVersion', 'DBCADVersion']
            RETURN v.part_name AS pn
            ORDER BY v.version DESC LIMIT 1
            """,
            {"name": project_name},
        )
        target_part = (latest or {}).get("pn") or ""

    if not target_part:
        raise HTTPException(status_code=404, detail=f"No part topology for '{project_name}'")

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
        "total_nodes_by_label": [{"label": d["label"], "count": int(d["n"] or 0)} for d in depth_stats],
        "body_count": len(bodies),
        "bodies": [{"uuid": str(b["uuid"] or "")} for b in bodies],
        "top_chains": [{"uuid": c["body_uuid"], "chain_size": int(c["chain_size"] or 0)} for c in chains],
    }


# ---------------------------------------------------------------------------
# 原始 Cypher（只读）
# ---------------------------------------------------------------------------

class CypherRequest(BaseModel):
    query: str
    params: dict[str, Any] = {}


@router.post("/query")
def execute_cypher(req: CypherRequest) -> dict[str, Any]:
    """执行只读 Cypher（拒绝 CREATE/DELETE/SET/MERGE/DROP/DETACH）。"""
    q = req.query.strip().upper()
    for kw in ["CREATE", "DELETE", "REMOVE", "SET", "MERGE", "DROP", "DETACH"]:
        if q.startswith(kw + " ") or q.startswith(kw + "\n") or q == kw:
            raise HTTPException(
                status_code=400,
                detail=f"Write keyword '{kw}' is not allowed. This endpoint is read-only.",
            )
    try:
        rows = _run_cypher(req.query, req.params)
        return {"rows": rows, "count": len(rows)}
    except Exception as ex:
        return JSONResponse(
            status_code=400,
            content={"error": str(ex), "detail": "Cypher execution failed."},
        )


# ---------------------------------------------------------------------------
# 清空 & 删除
# ---------------------------------------------------------------------------

@router.post("/clear")
def clear_database(confirm: bool = Query(default=False)) -> dict[str, str]:
    """清空所有节点和关系。dangerous！"""
    if not confirm:
        raise HTTPException(
            status_code=400,
            detail="Must set confirm=true to actually clear the database.",
        )
    try:
        _run_cypher("MATCH (n) DETACH DELETE n")
        return {"status": "ok", "message": "All nodes and relationships have been deleted."}
    except Exception as ex:
        raise HTTPException(status_code=500, detail=str(ex)) from ex


@router.delete("/projects/{project_name}")
def delete_project(project_name: str, confirm: bool = Query(default=False)) -> dict[str, str]:
    """删除某项目及其所有 BridgeVersion/BridgeEntityGraphVersion，并级联删除该项目的 BridgeDeltaVersion。"""
    if not confirm:
        raise HTTPException(
            status_code=400,
            detail="Must set confirm=true to actually delete the project.",
        )
    try:
        info = _run_cypher(
            """
            MATCH (p)
            WHERE labels(p)[0] IN ['BridgeProject', 'DBCADProject']
              AND p.name = $name
            WITH p, p.id AS pid
            OPTIONAL MATCH (p)-[:HAS_VERSION]->(v)
            OPTIONAL MATCH (p)-[:HAS_EG_VERSION]->(egv)
            OPTIONAL MATCH (d)
              WHERE (d.project_id = pid)
                AND labels(d)[0] IN ['BridgeDeltaVersion', 'DBCADDelta']
            WITH p, collect(DISTINCT v) AS vs, collect(DISTINCT egv) AS egvs, collect(DISTINCT d) AS ds
            FOREACH (x IN vs | DETACH DELETE x)
            FOREACH (x IN egvs | DETACH DELETE x)
            FOREACH (x IN ds | DETACH DELETE x)
            DETACH DELETE p
            RETURN p.name AS deleted
            """,
            {"name": project_name},
        )
        if not info:
            raise HTTPException(status_code=404, detail=f"Project '{project_name}' not found")
        return {"status": "ok", "message": f"Project '{project_name}' and related data deleted."}
    except HTTPException:
        raise
    except Exception as ex:
        raise HTTPException(status_code=500, detail=str(ex)) from ex


# ---------------------------------------------------------------------------
# HTML WebUI  ─ 科研配色：深蓝底、安静排版、丰富信息密度
# ---------------------------------------------------------------------------

_HTML_TEMPLATE = r"""<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>DB-CAD · Neo4j Management Console</title>
<style>
  /* ============== 设计 token ============== */
  :root {
    /* 基础色板（科研配色：沉静的深蓝 + 节制的强调色） */
    --bg: #0a1020;
    --bg-soft: #0e1730;
    --surface: #131e38;
    --surface-2: #1a2745;
    --surface-3: #213057;
    --border: #2a385c;
    --border-soft: #1f2b48;
    --text: #d3dcf0;
    --text-dim: #8a96b3;
    --text-bright: #f1f6ff;

    /* 语义色 */
    --primary: #6cb6ff;
    --primary-soft: rgba(108, 182, 255, 0.14);
    --cyan: #6fd9d2;
    --cyan-soft: rgba(111, 217, 210, 0.14);
    --amber: #f4a261;
    --amber-soft: rgba(244, 162, 97, 0.14);
    --violet: #b48ad6;
    --violet-soft: rgba(180, 138, 214, 0.14);
    --green: #7fcf9a;
    --red: #e76f51;
    --yellow: #f0c674;

    /* 模式色（前端使用） */
    --mode0: #7286d3;     /* Mode0 SAT 蓝紫 */
    --mode0-soft: rgba(114, 134, 211, 0.15);
    --mode1: #6cb6ff;     /* Mode1 Delta 蓝 */
    --mode1-soft: rgba(108, 182, 255, 0.15);
    --eg: #f4a261;        /* Entity Graph 琥珀 */
    --eg-soft: rgba(244, 162, 97, 0.15);

    /* 字体 */
    --ui: -apple-system, BlinkMacSystemFont, 'Segoe UI', 'PingFang SC', 'Microsoft YaHei', sans-serif;
    --mono: 'JetBrains Mono', 'Fira Code', 'Cascadia Code', 'Consolas', 'SF Mono', monospace;

    /* 字号（新设计提高基础字号） */
    --fz-xs: 11px;
    --fz-sm: 12.5px;
    --fz-base: 14px;
    --fz-md: 15px;
    --fz-lg: 17px;
    --fz-xl: 20px;
    --fz-2xl: 26px;
    --fz-3xl: 34px;

    --radius: 6px;
    --radius-lg: 10px;
  }

  /* ============== Reset & Base ============== */
  * { box-sizing: border-box; }
  html, body {
    margin: 0;
    background: var(--bg);
    color: var(--text);
    font-family: var(--ui);
    font-size: var(--fz-base);
    line-height: 1.55;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
  }
  body {
    min-height: 100vh;
    background:
      radial-gradient(1200px 600px at 8% -10%, rgba(108,182,255,0.06), transparent 70%),
      radial-gradient(800px 400px at 100% 0%, rgba(180,138,214,0.05), transparent 70%),
      var(--bg);
  }
  ::selection { background: var(--primary-soft); color: var(--text-bright); }
  ::-webkit-scrollbar { width: 10px; height: 10px; }
  ::-webkit-scrollbar-track { background: var(--bg-soft); }
  ::-webkit-scrollbar-thumb { background: var(--surface-3); border-radius: 5px; }
  ::-webkit-scrollbar-thumb:hover { background: var(--border); }

  button, input, select, textarea {
    font-family: inherit;
    font-size: inherit;
    color: inherit;
  }
  code, pre, .mono { font-family: var(--mono); }

  /* ============== Layout ============== */
  .app {
    display: grid;
    grid-template-rows: auto 1fr;
    min-height: 100vh;
  }

  /* 顶栏 */
  .topbar {
    display: flex;
    align-items: center;
    gap: 22px;
    padding: 14px 28px;
    background: linear-gradient(180deg, rgba(19,30,56,0.96) 0%, rgba(14,23,48,0.92) 100%);
    border-bottom: 1px solid var(--border-soft);
    backdrop-filter: blur(8px);
    position: sticky; top: 0; z-index: 10;
  }
  .brand {
    display: flex; align-items: center; gap: 12px;
    font-family: var(--mono);
    font-size: var(--fz-md);
    font-weight: 600;
    letter-spacing: 0.4px;
  }
  .brand-mark {
    width: 28px; height: 28px;
    border-radius: 6px;
    background: linear-gradient(135deg, var(--primary) 0%, var(--cyan) 100%);
    color: var(--bg);
    display: flex; align-items: center; justify-content: center;
    font-weight: 800; font-size: 13px;
    box-shadow: 0 0 18px rgba(108,182,255,0.35);
  }
  .brand small {
    color: var(--text-dim); font-weight: 400; margin-left: 6px;
    font-family: var(--ui); font-size: var(--fz-sm);
  }

  .nav-tabs {
    display: flex; gap: 4px;
    margin-left: 12px;
  }
  .nav-tab {
    padding: 7px 14px;
    font-size: var(--fz-sm);
    color: var(--text-dim);
    border-radius: var(--radius);
    cursor: pointer;
    border: 1px solid transparent;
    transition: all .15s;
    user-select: none;
  }
  .nav-tab:hover { color: var(--text); background: var(--surface); }
  .nav-tab.active {
    color: var(--text-bright);
    background: var(--surface-2);
    border-color: var(--border);
  }

  .topbar-spacer { flex: 1; }

  .topbar-stats {
    display: flex; gap: 18px;
    font-family: var(--mono); font-size: var(--fz-sm);
    color: var(--text-dim);
  }
  .topbar-stat { display: flex; flex-direction: column; align-items: flex-end; }
  .topbar-stat b {
    color: var(--text-bright); font-size: var(--fz-lg); font-weight: 600; line-height: 1.1;
  }

  .conn-status {
    display: inline-flex; align-items: center; gap: 7px;
    font-size: var(--fz-sm);
    color: var(--green);
    padding: 5px 10px;
    background: rgba(127, 207, 154, 0.08);
    border: 1px solid rgba(127, 207, 154, 0.25);
    border-radius: 999px;
  }
  .conn-status::before {
    content: ''; width: 6px; height: 6px; border-radius: 50%;
    background: var(--green);
    box-shadow: 0 0 8px var(--green);
  }

  /* 主体 */
  main {
    padding: 26px 28px 60px;
    max-width: 1480px;
    margin: 0 auto;
    width: 100%;
  }
  .tab-panel { display: none; }
  .tab-panel.active { display: block; }

  /* Section heading */
  .section-head {
    display: flex; justify-content: space-between; align-items: baseline;
    margin: 8px 0 18px;
  }
  .section-title {
    font-size: var(--fz-lg); font-weight: 600;
    color: var(--text-bright);
    margin: 0;
    display: flex; align-items: center; gap: 10px;
  }
  .section-title .bar {
    width: 4px; height: 16px; border-radius: 2px;
    background: linear-gradient(180deg, var(--primary), var(--cyan));
  }
  .section-sub { color: var(--text-dim); font-size: var(--fz-sm); }

  /* ============== Cards ============== */
  .card {
    background: var(--surface);
    border: 1px solid var(--border-soft);
    border-radius: var(--radius-lg);
    padding: 18px 20px;
  }
  .card-head {
    display: flex; justify-content: space-between; align-items: baseline;
    margin-bottom: 14px;
    padding-bottom: 12px;
    border-bottom: 1px solid var(--border-soft);
  }
  .card-title {
    font-size: var(--fz-md); font-weight: 600;
    color: var(--text-bright); margin: 0;
    display: flex; align-items: center; gap: 8px;
  }
  .card-title small { color: var(--text-dim); font-weight: 400; font-size: var(--fz-sm); }

  /* ============== Stat grid ============== */
  .stat-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(190px, 1fr));
    gap: 14px;
    margin-bottom: 22px;
  }
  .stat-card {
    position: relative;
    background: var(--surface);
    border: 1px solid var(--border-soft);
    border-radius: var(--radius-lg);
    padding: 16px 18px;
    overflow: hidden;
  }
  .stat-card::before {
    content: '';
    position: absolute; left: 0; top: 0; bottom: 0;
    width: 3px;
    background: var(--primary);
  }
  .stat-card.mode0::before { background: var(--mode0); }
  .stat-card.mode1::before { background: var(--mode1); }
  .stat-card.eg::before { background: var(--eg); }
  .stat-card.accent::before { background: var(--cyan); }
  .stat-card.project::before { background: var(--violet); }
  .stat-card .label {
    font-size: var(--fz-sm);
    color: var(--text-dim);
    text-transform: uppercase;
    letter-spacing: 0.5px;
    margin-bottom: 8px;
    font-weight: 500;
  }
  .stat-card .value {
    font-size: var(--fz-3xl);
    font-weight: 700;
    color: var(--text-bright);
    font-family: var(--mono);
    letter-spacing: -0.5px;
    line-height: 1;
  }
  .stat-card .sub {
    margin-top: 6px;
    font-size: var(--fz-xs);
    color: var(--text-dim);
  }

  /* ============== Tables ============== */
  .table-wrap {
    background: var(--surface);
    border: 1px solid var(--border-soft);
    border-radius: var(--radius-lg);
    overflow: hidden;
  }
  .data-table {
    width: 100%;
    border-collapse: collapse;
    font-size: var(--fz-base);
  }
  .data-table thead {
    background: var(--surface-2);
  }
  .data-table th {
    text-align: left;
    padding: 12px 14px;
    font-weight: 500;
    color: var(--text-dim);
    border-bottom: 1px solid var(--border-soft);
    font-size: var(--fz-sm);
    text-transform: uppercase;
    letter-spacing: 0.4px;
  }
  .data-table td {
    padding: 12px 14px;
    border-bottom: 1px solid var(--border-soft);
    color: var(--text);
    vertical-align: middle;
  }
  .data-table tr:last-child td { border-bottom: none; }
  .data-table tbody tr { transition: background .12s; }
  .data-table tbody tr:hover {
    background: rgba(108,182,255,0.04);
  }
  .data-table tbody tr.expanded {
    background: rgba(108,182,255,0.06);
  }
  .data-table tbody tr.expanded:hover {
    background: rgba(108,182,255,0.08);
  }
  .cell-name {
    font-weight: 500;
    color: var(--text-bright);
    cursor: pointer;
    display: inline-flex; align-items: center; gap: 8px;
  }
  .cell-name:hover { color: var(--primary); }
  .expand-caret {
    display: inline-block;
    width: 16px; height: 16px;
    text-align: center;
    color: var(--text-dim);
    font-size: 10px;
    transition: transform .15s;
  }
  tr.expanded .expand-caret { transform: rotate(90deg); color: var(--primary); }

  /* 模式徽章 */
  .badge {
    display: inline-flex; align-items: center; gap: 5px;
    padding: 3px 9px;
    font-size: var(--fz-xs);
    font-weight: 600;
    font-family: var(--mono);
    border-radius: 999px;
    letter-spacing: 0.3px;
  }
  .badge.mode0 { background: var(--mode0-soft); color: var(--mode0); }
  .badge.mode1 { background: var(--mode1-soft); color: var(--mode1); }
  .badge.eg    { background: var(--eg-soft); color: var(--eg); }
  .badge.zero  { background: var(--surface-3); color: var(--text-dim); }
  .badge dot { width: 6px; height: 6px; border-radius: 50%; background: currentColor; }

  /* 数字列 */
  .num {
    font-family: var(--mono);
    color: var(--text);
    text-align: right;
  }
  .num.zero { color: var(--text-dim); }
  .num.dim { color: var(--text-dim); }

  /* ============== Project detail panel ============== */
  .detail-row td {
    padding: 0 !important;
    background: var(--bg-soft) !important;
    border-bottom: 1px solid var(--border) !important;
  }
  .detail-panel {
    padding: 22px 26px;
    color: var(--text);
  }
  .detail-grid {
    display: grid;
    grid-template-columns: repeat(3, minmax(0, 1fr));
    gap: 18px;
  }
  .mode-col {
    background: var(--surface);
    border: 1px solid var(--border-soft);
    border-radius: var(--radius-lg);
    overflow: hidden;
  }
  .mode-col.mode0 { border-top: 3px solid var(--mode0); }
  .mode-col.mode1 { border-top: 3px solid var(--mode1); }
  .mode-col.eg    { border-top: 3px solid var(--eg); }
  .mode-col-head {
    padding: 12px 16px;
    display: flex; align-items: center; justify-content: space-between;
    border-bottom: 1px solid var(--border-soft);
  }
  .mode-col-title {
    font-size: var(--fz-md); font-weight: 600;
    display: flex; align-items: center; gap: 8px;
  }
  .mode-col-title small {
    color: var(--text-dim); font-weight: 400; font-size: var(--fz-sm);
  }
  .mode-col-body {
    max-height: 360px; overflow-y: auto;
  }
  .version-item {
    padding: 10px 16px;
    border-bottom: 1px solid var(--border-soft);
    display: grid; grid-template-columns: auto 1fr auto; gap: 10px;
    align-items: center;
    cursor: pointer;
    transition: background .12s;
  }
  .version-item:last-child { border-bottom: none; }
  .version-item:hover { background: rgba(108,182,255,0.05); }
  .version-num {
    font-family: var(--mono); font-weight: 600;
    color: var(--text-bright); min-width: 36px;
  }
  .version-meta { color: var(--text-dim); font-size: var(--fz-sm); }
  .version-time {
    font-family: var(--mono); font-size: var(--fz-xs);
    color: var(--text-dim);
  }
  .empty-state {
    padding: 28px 16px;
    text-align: center;
    color: var(--text-dim);
    font-size: var(--fz-sm);
  }

  /* ============== Modal ============== */
  .modal-mask {
    position: fixed; inset: 0;
    background: rgba(8, 14, 28, 0.78);
    backdrop-filter: blur(4px);
    display: none;
    align-items: center; justify-content: center;
    z-index: 100;
    padding: 40px;
  }
  .modal-mask.show { display: flex; }
  .modal {
    background: var(--surface);
    border: 1px solid var(--border);
    border-radius: var(--radius-lg);
    width: min(960px, 100%);
    max-height: calc(100vh - 80px);
    display: flex; flex-direction: column;
    box-shadow: 0 20px 60px rgba(0,0,0,0.6);
  }
  .modal-head {
    display: flex; align-items: center; justify-content: space-between;
    padding: 16px 20px;
    border-bottom: 1px solid var(--border-soft);
  }
  .modal-title {
    font-size: var(--fz-lg); font-weight: 600;
    color: var(--text-bright);
    display: flex; align-items: center; gap: 10px;
  }
  .modal-close {
    width: 28px; height: 28px;
    border-radius: 4px;
    border: 1px solid var(--border);
    background: transparent;
    color: var(--text-dim);
    cursor: pointer;
    display: flex; align-items: center; justify-content: center;
    font-size: 16px;
  }
  .modal-close:hover { color: var(--text-bright); background: var(--surface-2); }
  .modal-body {
    padding: 18px 20px;
    overflow-y: auto;
  }
  .modal-footer {
    padding: 14px 20px;
    border-top: 1px solid var(--border-soft);
    display: flex; justify-content: flex-end; gap: 8px;
  }

  /* KV 表 */
  .kv-table {
    width: 100%; border-collapse: collapse;
    font-size: var(--fz-base);
  }
  .kv-table th, .kv-table td {
    padding: 9px 12px;
    border-bottom: 1px solid var(--border-soft);
    text-align: left;
  }
  .kv-table th {
    color: var(--text-dim); font-weight: 500;
    width: 32%; font-size: var(--fz-sm);
    text-transform: uppercase; letter-spacing: 0.3px;
  }
  .kv-table td {
    color: var(--text); font-family: var(--mono); font-size: var(--fz-sm);
  }

  /* code block (JSON) */
  pre.code {
    background: var(--bg);
    border: 1px solid var(--border-soft);
    border-radius: var(--radius);
    padding: 14px 16px;
    margin: 0;
    font-family: var(--mono);
    font-size: var(--fz-sm);
    color: var(--text);
    overflow: auto;
    max-height: 480px;
    line-height: 1.55;
  }

  /* 工具栏 */
  .toolbar {
    display: flex; align-items: center; gap: 10px;
    margin-bottom: 14px;
  }
  .input {
    background: var(--bg);
    border: 1px solid var(--border);
    border-radius: var(--radius);
    padding: 8px 12px;
    color: var(--text);
    font-size: var(--fz-base);
    outline: none;
    min-width: 220px;
    transition: border-color .15s;
  }
  .input:focus { border-color: var(--primary); box-shadow: 0 0 0 2px var(--primary-soft); }
  .input::placeholder { color: var(--text-dim); }

  .btn {
    padding: 7px 14px;
    border: 1px solid var(--border);
    background: var(--surface-2);
    color: var(--text);
    border-radius: var(--radius);
    cursor: pointer;
    font-size: var(--fz-sm);
    transition: all .15s;
  }
  .btn:hover { background: var(--surface-3); border-color: var(--primary); color: var(--text-bright); }
  .btn.primary { background: var(--primary-soft); border-color: var(--primary); color: var(--primary); }
  .btn.primary:hover { background: var(--primary); color: var(--bg); }
  .btn.danger { background: rgba(231, 111, 81, 0.12); border-color: var(--red); color: var(--red); }
  .btn.danger:hover { background: var(--red); color: var(--bg); }
  .btn.ghost { background: transparent; border-color: var(--border); }
  .btn.tiny { padding: 4px 10px; font-size: var(--fz-xs); }

  /* Bar chart */
  .bar-chart { display: flex; flex-direction: column; gap: 8px; }
  .bar-row {
    display: grid; grid-template-columns: 200px 1fr 80px;
    align-items: center; gap: 12px;
    font-size: var(--fz-sm);
  }
  .bar-label {
    color: var(--text); font-family: var(--mono);
    white-space: nowrap; overflow: hidden; text-overflow: ellipsis;
  }
  .bar-track {
    background: var(--bg);
    height: 12px; border-radius: 6px;
    overflow: hidden;
    border: 1px solid var(--border-soft);
  }
  .bar-fill {
    height: 100%;
    background: linear-gradient(90deg, var(--primary), var(--cyan));
    transition: width .4s;
  }
  .bar-val { text-align: right; font-family: var(--mono); color: var(--text-bright); }

  /* 通用：工具型徽章/标签 */
  .chip {
    display: inline-block;
    padding: 2px 8px;
    border-radius: 4px;
    font-family: var(--mono);
    font-size: var(--fz-xs);
    background: var(--surface-3);
    color: var(--text-dim);
    border: 1px solid var(--border-soft);
  }

  /* Toast */
  .toast {
    position: fixed;
    top: 24px; right: 24px;
    z-index: 1000;
    display: flex; flex-direction: column; gap: 10px;
  }
  .toast-msg {
    background: var(--surface);
    border: 1px solid var(--border);
    border-left: 4px solid var(--primary);
    color: var(--text);
    padding: 10px 16px;
    border-radius: var(--radius);
    box-shadow: 0 10px 30px rgba(0,0,0,0.4);
    max-width: 380px;
    font-size: var(--fz-base);
  }
  .toast-msg.error { border-left-color: var(--red); }
  .toast-msg.success { border-left-color: var(--green); }

  /* Topology indicator */
  .topo-strip {
    display: flex; flex-wrap: wrap; gap: 6px; margin-top: 6px;
  }
  .topo-pill {
    padding: 3px 9px;
    background: var(--surface-3);
    border: 1px solid var(--border-soft);
    border-radius: 4px;
    font-family: var(--mono);
    font-size: var(--fz-xs);
    color: var(--text);
    display: inline-flex; align-items: center; gap: 6px;
  }
  .topo-pill .count {
    color: var(--text-dim);
  }

  /* project-id 样 */
  .pid {
    font-family: var(--mono); font-size: var(--fz-xs);
    color: var(--text-dim);
    letter-spacing: 0.3px;
  }
  .pid::before { content: '· '; color: var(--border); }

  /* responsive */
  @media (max-width: 1100px) {
    .detail-grid { grid-template-columns: 1fr; }
    .topbar-stats { display: none; }
  }

  /* Browser tab */
  .cypher-editor {
    width: 100%;
    min-height: 140px;
    background: var(--bg);
    border: 1px solid var(--border);
    border-radius: var(--radius);
    padding: 12px 14px;
    color: var(--text);
    font-family: var(--mono);
    font-size: var(--fz-sm);
    line-height: 1.6;
    resize: vertical;
  }
  .cypher-editor:focus { outline: none; border-color: var(--primary); box-shadow: 0 0 0 2px var(--primary-soft); }
  .row-result {
    font-family: var(--mono); font-size: var(--fz-sm);
    background: var(--bg);
    padding: 10px 14px;
    border-radius: var(--radius);
    border: 1px solid var(--border-soft);
    margin-bottom: 6px;
    color: var(--text);
    overflow-x: auto;
    white-space: pre-wrap;
    word-break: break-all;
  }

  /* small helpers */
  .row { display: flex; gap: 8px; align-items: center; flex-wrap: wrap; }
  .right { text-align: right; }
  .center { text-align: center; }
  .muted { color: var(--text-dim); }
  .mt-12 { margin-top: 12px; } .mt-16 { margin-top: 16px; } .mt-24 { margin-top: 24px; }
</style>
</head>
<body>
<div class="app">

  <!-- 顶栏 -->
  <header class="topbar">
    <div class="brand">
      <div class="brand-mark">DB</div>
      <span>CAD-Neo4j Console<small>v2 · research build</small></span>
    </div>
    <nav class="nav-tabs">
      <div class="nav-tab active" data-tab="overview">总览</div>
      <div class="nav-tab" data-tab="projects">项目</div>
      <div class="nav-tab" data-tab="browser">数据浏览器</div>
    </nav>
    <div class="topbar-spacer"></div>
    <div class="topbar-stats">
      <div class="topbar-stat"><span id="topbar-projects">—</span><b>projects</b></div>
      <div class="topbar-stat"><span id="topbar-mode0">—</span><b>mode0</b></div>
      <div class="topbar-stat"><span id="topbar-mode1">—</span><b>mode1</b></div>
      <div class="topbar-stat"><span id="topbar-eg">—</span><b>eg-ver</b></div>
    </div>
    <div class="conn-status">Neo4j 在线</div>
  </header>

  <main>

    <!-- ============== 总览 ============== -->
    <section class="tab-panel active" id="tab-overview">
      <div class="section-head">
        <h2 class="section-title"><span class="bar"></span>数据库概览</h2>
        <div class="section-sub" id="overview-sub">实时统计</div>
      </div>

      <div class="stat-grid" id="top-stats"></div>

      <div class="row" style="gap: 18px;">
        <div class="card" style="flex: 1.4;">
          <div class="card-head">
            <h3 class="card-title">Label 分布 <small>· 节点类型占比</small></h3>
            <span class="chip" id="label-total">—</span>
          </div>
          <div class="bar-chart" id="label-chart"></div>
        </div>

        <div class="card" style="flex: 1;">
          <div class="card-head">
            <h3 class="card-title">拓扑分类 <small>· 按功能归并</small></h3>
          </div>
          <div class="bar-chart" id="bucket-chart"></div>
        </div>
      </div>

      <div class="card mt-24">
        <div class="card-head">
          <h3 class="card-title">关系类型 Top <small>· 数据流骨架</small></h3>
          <span class="chip" id="rel-total">—</span>
        </div>
        <div class="bar-chart" id="rel-chart"></div>
      </div>
    </section>

    <!-- ============== 项目 ============== -->
    <section class="tab-panel" id="tab-projects">
      <div class="section-head">
        <h2 class="section-title"><span class="bar"></span>项目列表</h2>
        <div class="section-sub">点击项目名展开三模式版本历史</div>
      </div>

      <div class="stat-grid" id="project-stats"></div>

      <div class="toolbar">
        <input class="input" id="project-search" placeholder="过滤项目名（实时搜索）"/>
        <button class="btn" id="btn-refresh">刷新</button>
        <span class="muted" id="project-count" style="margin-left: auto;"></span>
      </div>

      <div class="table-wrap">
        <table class="data-table" id="project-table">
          <thead>
            <tr>
              <th style="width: 30px;"></th>
              <th>项目名</th>
              <th class="center" style="width: 100px;">Mode0</th>
              <th class="center" style="width: 100px;">Mode1</th>
              <th class="center" style="width: 100px;">Entity Graph</th>
              <th class="center" style="width: 90px;">最新版</th>
              <th class="right" style="width: 170px;">更新时间</th>
            </tr>
          </thead>
          <tbody id="project-tbody">
            <tr><td colspan="7" class="empty-state">正在加载项目列表…</td></tr>
          </tbody>
        </table>
      </div>
    </section>

    <!-- ============== 数据浏览器 ============== -->
    <section class="tab-panel" id="tab-browser">
      <div class="section-head">
        <h2 class="section-title"><span class="bar"></span>只读 Cypher</h2>
        <div class="section-sub">仅允许 MATCH/RETURN，禁止写操作</div>
      </div>
      <div class="card">
        <textarea id="cypher-input" class="cypher-editor" placeholder="MATCH (n) RETURN labels(n)[0] AS l, count(*) AS n ORDER BY n DESC LIMIT 20"></textarea>
        <div class="row mt-12" style="justify-content: space-between;">
          <div class="muted" style="font-size: var(--fz-sm);">示例：
            <code class="chip" onclick="setCypher(this)">MATCH (n) RETURN labels(n)[0] AS l, count(*) AS n ORDER BY n DESC</code>
            <code class="chip" onclick="setCypher(this)">MATCH (p:BridgeProject) RETURN p.name, p.id LIMIT 20</code>
            <code class="chip" onclick="setCypher(this)">MATCH ()-[r]->() RETURN type(r) AS t, count(*) AS n ORDER BY n DESC</code>
          </div>
          <div class="row">
            <button class="btn ghost" id="cypher-clear">清空</button>
            <button class="btn primary" id="cypher-run">执行</button>
          </div>
        </div>
        <div id="cypher-result" class="mt-16"></div>
      </div>
    </section>

  </main>
</div>

<!-- 详情/版本 modal -->
<div class="modal-mask" id="modal">
  <div class="modal">
    <div class="modal-head">
      <div class="modal-title" id="modal-title">—</div>
      <button class="modal-close" onclick="closeModal()">×</button>
    </div>
    <div class="modal-body" id="modal-body"></div>
    <div class="modal-footer">
      <button class="btn ghost" onclick="closeModal()">关闭</button>
    </div>
  </div>
</div>

<div class="toast" id="toast"></div>

<script>
const API = '/api/neo4j';

const $ = (s, p=document) => p.querySelector(s);
const $$ = (s, p=document) => [...p.querySelectorAll(s)];

function toast(msg, kind='') {
  const box = $('#toast');
  const el = document.createElement('div');
  el.className = 'toast-msg ' + kind;
  el.textContent = msg;
  box.appendChild(el);
  setTimeout(() => el.remove(), 3500);
}

async function getJSON(url) {
  const r = await fetch(url);
  if (!r.ok) {
    let detail = r.statusText;
    try { const j = await r.json(); detail = j.detail || JSON.stringify(j); } catch(_) {}
    throw new Error(`${r.status} ${detail}`);
  }
  return r.json();
}

function setCypher(el) {
  $('#cypher-input').value = el.textContent;
}

// ============== Tab 切换 ==============
$$('.nav-tab').forEach(t => {
  t.addEventListener('click', () => {
    $$('.nav-tab').forEach(x => x.classList.remove('active'));
    $$('.tab-panel').forEach(x => x.classList.remove('active'));
    t.classList.add('active');
    $('#tab-' + t.dataset.tab).classList.add('active');
    if (t.dataset.tab === 'projects') loadProjects();
    if (t.dataset.tab === 'overview') loadOverview();
  });
});

// ============== 总览 ==============
async function loadOverview() {
  try {
    const data = await getJSON(`${API}/overview`);
    $('#topbar-projects').textContent = data.total_projects.toLocaleString();
    $('#topbar-mode0').textContent = data.total_mode0_versions.toLocaleString();
    $('#topbar-mode1').textContent = data.total_mode1_deltas.toLocaleString();
    $('#topbar-eg').textContent = data.total_entity_graph_versions.toLocaleString();

    $('#overview-sub').textContent = `共 ${data.total_nodes.toLocaleString()} 节点 · ${data.total_relationships.toLocaleString()} 关系`;

    const topStats = $('#top-stats');
    topStats.innerHTML = `
      <div class="stat-card project">
        <div class="label">项目总数</div>
        <div class="value">${data.total_projects.toLocaleString()}</div>
        <div class="sub">BridgeProject + DBCADProject</div>
      </div>
      <div class="stat-card mode0">
        <div class="label">Mode0 全量版本</div>
        <div class="value">${data.total_mode0_versions.toLocaleString()}</div>
        <div class="sub">BridgeVersion（完整 SAT）</div>
      </div>
      <div class="stat-card mode1">
        <div class="label">Mode1 增量 Delta</div>
        <div class="value">${data.total_mode1_deltas.toLocaleString()}</div>
        <div class="sub">BridgeDeltaVersion（增量 SAT）</div>
      </div>
      <div class="stat-card eg">
        <div class="label">Entity Graph 版本</div>
        <div class="value">${data.total_entity_graph_versions.toLocaleString()}</div>
        <div class="sub">BridgeEntityGraphVersion</div>
      </div>
      <div class="stat-card accent">
        <div class="label">总节点</div>
        <div class="value">${data.total_nodes.toLocaleString()}</div>
        <div class="sub">${data.label_counts.length} 种 label</div>
      </div>
      <div class="stat-card">
        <div class="label">总关系</div>
        <div class="value">${data.total_relationships.toLocaleString()}</div>
        <div class="sub">${data.relationship_counts.length} 种关系类型</div>
      </div>
    `;

    // Label chart
    const labels = data.label_counts.slice(0, 22);
    const labelMax = labels[0]?.count || 1;
    $('#label-total').textContent = `${data.label_counts.length} 种 label · 累计 ${data.total_nodes.toLocaleString()}`;
    $('#label-chart').innerHTML = labels.map(r => `
      <div class="bar-row">
        <div class="bar-label" title="${r.label}">${r.label}</div>
        <div class="bar-track"><div class="bar-fill" style="width: ${(r.count/labelMax*100).toFixed(1)}%"></div></div>
        <div class="bar-val">${r.count.toLocaleString()}</div>
      </div>
    `).join('');

    // Topology buckets
    const buckets = data.topology_buckets;
    const bucketMax = buckets[0]?.count || 1;
    $('#bucket-chart').innerHTML = buckets.map(r => `
      <div class="bar-row">
        <div class="bar-label">${r.bucket}</div>
        <div class="bar-track"><div class="bar-fill" style="width: ${(r.count/bucketMax*100).toFixed(1)}%"></div></div>
        <div class="bar-val">${r.count.toLocaleString()}</div>
      </div>
    `).join('');

    // Rel chart
    const rels = data.relationship_counts.slice(0, 18);
    const relMax = rels[0]?.count || 1;
    $('#rel-total').textContent = `${data.relationship_counts.length} 种关系`;
    $('#rel-chart').innerHTML = rels.map(r => `
      <div class="bar-row">
        <div class="bar-label">${r.rel_type}</div>
        <div class="bar-track"><div class="bar-fill" style="width: ${(r.count/relMax*100).toFixed(1)}%"></div></div>
        <div class="bar-val">${r.count.toLocaleString()}</div>
      </div>
    `).join('');
  } catch (e) {
    toast('加载概览失败：' + e.message, 'error');
  }
}

// ============== 项目 ==============
let ALL_PROJECTS = [];

async function loadProjects() {
  const tb = $('#project-tbody');
  tb.innerHTML = '<tr><td colspan="7" class="empty-state">正在加载项目列表…</td></tr>';
  try {
    const projects = await getJSON(`${API}/projects`);
    ALL_PROJECTS = projects;
    renderProjects();

    const totalMode0 = projects.reduce((s,p) => s + p.mode0_version_count, 0);
    const totalMode1 = projects.reduce((s,p) => s + p.mode1_delta_count, 0);
    const totalEG    = projects.reduce((s,p) => s + p.entity_graph_version_count, 0);

    $('#project-stats').innerHTML = `
      <div class="stat-card project">
        <div class="label">项目总数</div>
        <div class="value">${projects.length.toLocaleString()}</div>
        <div class="sub">BridgeProject / DBCADProject</div>
      </div>
      <div class="stat-card mode0">
        <div class="label">Mode0 总版本</div>
        <div class="value">${totalMode0.toLocaleString()}</div>
        <div class="sub">累计完整 SAT 版本</div>
      </div>
      <div class="stat-card mode1">
        <div class="label">Mode1 总 Delta</div>
        <div class="value">${totalMode1.toLocaleString()}</div>
        <div class="sub">累计增量 Delta</div>
      </div>
      <div class="stat-card eg">
        <div class="label">Entity Graph 总版本</div>
        <div class="value">${totalEG.toLocaleString()}</div>
        <div class="sub">累计 Entity Graph 版本</div>
      </div>
    `;

    $('#project-count').textContent = `共 ${projects.length} 个项目`;
  } catch (e) {
    tb.innerHTML = `<tr><td colspan="7" class="empty-state">加载失败：${e.message}</td></tr>`;
    toast('加载项目失败：' + e.message, 'error');
  }
}

function renderProjects() {
  const kw = $('#project-search').value.trim().toLowerCase();
  const list = kw ? ALL_PROJECTS.filter(p => (p.name||'').toLowerCase().includes(kw)) : ALL_PROJECTS;
  const tb = $('#project-tbody');
  if (list.length === 0) {
    tb.innerHTML = '<tr><td colspan="7" class="empty-state">无匹配项目</td></tr>';
    return;
  }
  tb.innerHTML = list.map(p => projectRowHTML(p)).join('');
  // 绑定 row 事件
  $$('#project-tbody tr.project-row').forEach(tr => {
    const name = tr.dataset.name;
    tr.querySelector('.cell-name').addEventListener('click', () => toggleProject(name, tr));
    tr.querySelector('.expand-caret').addEventListener('click', () => toggleProject(name, tr));
  });
}

function badge(mode, n) {
  if (!n) return `<span class="badge zero">0</span>`;
  const cls = (mode === 'mode0') ? 'mode0' : (mode === 'mode1' ? 'mode1' : 'eg');
  const label = (mode === 'mode0') ? 'M0' : (mode === 'mode1' ? 'M1' : 'EG');
  return `<span class="badge ${cls}">${label} × ${n}</span>`;
}

function fmtTime(s) {
  if (!s) return '<span class="muted">—</span>';
  try {
    const d = new Date(s);
    if (isNaN(d.getTime())) return s;
    const pad = (n) => String(n).padStart(2,'0');
    return `${d.getFullYear()}-${pad(d.getMonth()+1)}-${pad(d.getDate())} ${pad(d.getHours())}:${pad(d.getMinutes())}`;
  } catch (_) { return s; }
}

function projectRowHTML(p) {
  const latest = []
    .concat(p.mode0_latest ? ['M0#'+p.mode0_latest] : [])
    .concat(p.mode1_latest ? ['M1#'+p.mode1_latest] : [])
    .concat(p.entity_graph_latest ? ['EG#'+p.entity_graph_latest] : []);
  return `
    <tr class="project-row" data-name="${escapeAttr(p.name)}">
      <td class="center"><span class="expand-caret">▶</span></td>
      <td>
        <span class="cell-name">
          <span>${escapeHTML(p.name)}</span>
        </span>
        <span class="pid">${(p.project_id||'').slice(0,8)}</span>
      </td>
      <td class="center">${badge('mode0', p.mode0_version_count)}</td>
      <td class="center">${badge('mode1', p.mode1_delta_count)}</td>
      <td class="center">${badge('eg', p.entity_graph_version_count)}</td>
      <td class="center"><span class="mono muted" style="font-size: var(--fz-sm);">${latest.length ? latest.join(' / ') : '—'}</span></td>
      <td class="right muted" style="font-size: var(--fz-sm);">${fmtTime(p.updated_at)}</td>
    </tr>
    <tr class="detail-row" data-name="${escapeAttr(p.name)}" style="display: none;">
      <td colspan="7">
        <div class="detail-panel" id="detail-${escapeAttr(p.name)}">
          <div class="muted center" style="padding: 16px;">加载中…</div>
        </div>
      </td>
    </tr>
  `;
}

const expanded = new Set();

async function toggleProject(name, tr) {
  const detailTr = document.querySelector(`tr.detail-row[data-name="${cssEscape(name)}"]`);
  if (!detailTr) return;
  if (expanded.has(name)) {
    expanded.delete(name);
    detailTr.style.display = 'none';
    tr.classList.remove('expanded');
    return;
  }
  expanded.add(name);
  detailTr.style.display = '';
  tr.classList.add('expanded');

  const detailEl = detailTr.querySelector('.detail-panel');
  try {
    const d = await getJSON(`${API}/projects/${encodeURIComponent(name)}`);
    renderDetail(d, detailEl);
  } catch (e) {
    detailEl.innerHTML = `<div class="empty-state" style="color: var(--red);">加载失败：${escapeHTML(e.message)}</div>`;
  }
}

function renderDetail(d, el) {
  const totalM0 = d.mode0_versions.length;
  const totalM1 = d.mode1_deltas.length;
  const totalEG = d.entity_graph_versions.length;

  el.innerHTML = `
    <div class="row" style="gap: 14px; margin-bottom: 14px;">
      <div style="flex: 1;">
        <div style="font-size: var(--fz-lg); font-weight: 600; color: var(--text-bright);">${escapeHTML(d.project.name)}</div>
        <div class="muted" style="font-size: var(--fz-sm); margin-top: 4px;">
          ID <span class="mono">${escapeHTML(d.project.id)}</span>
          · 创建 ${fmtTime(d.project.created_at)}
          · 更新 ${fmtTime(d.project.updated_at)}
        </div>
        ${d.current_part ? `
          <div class="muted" style="font-size: var(--fz-sm); margin-top: 4px;">
            当前 part 引用：<span class="mono">${escapeHTML(d.current_part.part_name)}</span>
          </div>
        ` : ''}
      </div>
    </div>
    <div class="detail-grid">
      ${renderModeCol('mode0', 'Mode 0', 'SAT 全量版本', d.mode0_versions, totalM0, 'mode0')}
      ${renderModeCol('mode1', 'Mode 1', 'Delta 增量', d.mode1_deltas, totalM1, 'mode1')}
      ${renderModeCol('eg', 'Entity Graph', '图版本', d.entity_graph_versions, totalEG, 'eg')}
    </div>
  `;

  // 绑定版本点击
  el.querySelectorAll('.version-item').forEach(v => {
    v.addEventListener('click', () => {
      const project = d.project.name;
      const mode = v.dataset.mode;
      const version = v.dataset.version;
      openVersionDetail(project, mode, version);
    });
  });
}

function renderModeCol(cls, title, sub, items, total, mode) {
  const body = items.length === 0
    ? `<div class="empty-state">该模式还没有版本</div>`
    : items.slice().reverse().map(v => {
        const vnum = v.version;
        return `
          <div class="version-item" data-mode="${mode}" data-version="${vnum}">
            <div class="version-num">#${vnum}</div>
            <div class="version-meta">
              <div style="color: var(--text); font-size: var(--fz-sm);">
                ${escapeHTML(v.author || 'anon')}
                ${v.part_name ? `<span class="mono muted" style="margin-left: 6px;">${escapeHTML(v.part_name)}</span>` : ''}
              </div>
              <div class="muted" style="font-size: var(--fz-xs); margin-top: 2px;">
                ${v.graph_bytes ? `${(v.graph_bytes/1024).toFixed(1)} KB graph` : ''}
                ${v.delta_uuid_count !== undefined && v.delta_uuid_count >= 0
                  ? ` · +${v.delta_uuid_count} bodies · -${v.removed_uuid_count || 0}`
                  : ''}
              </div>
            </div>
            <div class="version-time">${fmtTime(v.created_at)}</div>
          </div>
        `;
      }).join('');
  return `
    <div class="mode-col ${cls}">
      <div class="mode-col-head">
        <div class="mode-col-title">
          ${title} <small>${sub} · ${total}</small>
        </div>
      </div>
      <div class="mode-col-body">${body}</div>
    </div>
  `;
}

async function openVersionDetail(project, mode, version) {
  const url =
    mode === 'mode0' ? `${API}/projects/${encodeURIComponent(project)}/mode0/${version}`
    : mode === 'mode1' ? `${API}/projects/${encodeURIComponent(project)}/mode1/${version}`
    : `${API}/projects/${encodeURIComponent(project)}/eg/${version}`;

  openModal(`版本详情 · ${mode.toUpperCase()} v${version}`);
  const body = $('#modal-body');
  body.innerHTML = '<div class="muted center" style="padding: 20px;">加载中…</div>';
  try {
    const d = await getJSON(url);
    if (mode === 'mode0') renderMode0Modal(d, body, project);
    else if (mode === 'mode1') renderMode1Modal(d, body, project);
    else renderEGModal(d, body, project);
  } catch (e) {
    body.innerHTML = `<div class="empty-state" style="color: var(--red);">失败：${escapeHTML(e.message)}</div>`;
  }
}

function renderMode0Modal(d, body, project) {
  const rows = d.topology_by_label.map(t =>
    `<tr><th>${escapeHTML(t.label)}</th><td>${t.count.toLocaleString()}</td></tr>`
  ).join('');
  body.innerHTML = `
    <table class="kv-table">
      <tr><th>版本号</th><td>#${d.version}</td></tr>
      <tr><th>作者</th><td>${escapeHTML(d.author)}</td></tr>
      <tr><th>创建时间</th><td>${fmtTime(d.created_at)}</td></tr>
      <tr><th>所属项目</th><td>${escapeHTML(project)}</td></tr>
      <tr><th>Part 名</th><td>${escapeHTML(d.part_name)}</td></tr>
    </table>
    <h4 style="margin-top: 18px; color: var(--text-bright); font-size: var(--fz-md);">Mode 0 拓扑分布（基于 part 实体）</h4>
    <table class="kv-table">
      <tr><th>label</th><th>count</th></tr>
      ${rows || '<tr><th colspan="2" class="muted">未找到 part 拓扑节点</th></tr>'}
    </table>
  `;
}

function renderMode1Modal(d, body, project) {
  const c = d.parsed_content;
  body.innerHTML = `
    <table class="kv-table">
      <tr><th>版本号</th><td>#${d.version}</td></tr>
      <tr><th>作者</th><td>${escapeHTML(d.author)}</td></tr>
      <tr><th>创建时间</th><td>${fmtTime(d.created_at)}</td></tr>
      <tr><th>所属项目</th><td>${escapeHTML(project)}</td></tr>
      <tr><th>原始长度</th><td>${d.raw_length.toLocaleString()} 字符</td></tr>
    </table>
    <h4 style="margin-top: 18px; color: var(--text-bright); font-size: var(--fz-md);">Delta JSON 内容</h4>
    <pre class="code">${escapeHTML(JSON.stringify(c, null, 2))}</pre>
  `;
}

function renderEGModal(d, body, project) {
  const lbls = d.label_breakdown.map(t =>
    `<tr><th>${escapeHTML(t.label)}</th><td>${t.count.toLocaleString()}</td></tr>`
  ).join('');
  const rels = d.rel_type_breakdown.map(t =>
    `<tr><th>${escapeHTML(t.type)}</th><td>${t.count.toLocaleString()}</td></tr>`
  ).join('');
  body.innerHTML = `
    <table class="kv-table">
      <tr><th>版本号</th><td>#${d.version}</td></tr>
      <tr><th>作者</th><td>${escapeHTML(d.author)}</td></tr>
      <tr><th>创建时间</th><td>${fmtTime(d.created_at)}</td></tr>
      <tr><th>所属项目</th><td>${escapeHTML(project)}</td></tr>
      <tr><th>图 JSON 长度</th><td>${(d.graph_bytes/1024).toFixed(2)} KB (${d.graph_bytes.toLocaleString()} B)</td></tr>
      <tr><th>节点 / 关系</th><td>${d.node_count.toLocaleString()} nodes · ${d.rel_count.toLocaleString()} rels</td></tr>
      <tr><th>Part 名</th><td>${escapeHTML(d.part_name)}</td></tr>
    </table>
    <div class="row mt-16" style="gap: 18px; align-items: flex-start;">
      <div style="flex: 1;">
        <h4 style="color: var(--text-bright); font-size: var(--fz-md); margin: 0 0 8px;">节点 Label 分布</h4>
        <table class="kv-table">${lbls || '<tr><th colspan="2" class="muted">空</th></tr>'}</table>
      </div>
      <div style="flex: 1;">
        <h4 style="color: var(--text-bright); font-size: var(--fz-md); margin: 0 0 8px;">关系类型分布</h4>
        <table class="kv-table">${rels || '<tr><th colspan="2" class="muted">空</th></tr>'}</table>
      </div>
    </div>
  `;
}

function openModal(title) {
  $('#modal-title').innerHTML = title;
  $('#modal').classList.add('show');
}
function closeModal() { $('#modal').classList.remove('show'); }

// 搜索
$('#project-search').addEventListener('input', () => renderProjects());
$('#btn-refresh').addEventListener('click', loadProjects);

// ============== 数据浏览器 ==============
$('#cypher-run').addEventListener('click', runCypher);
$('#cypher-clear').addEventListener('click', () => {
  $('#cypher-input').value = '';
  $('#cypher-result').innerHTML = '';
});
$('#cypher-input').addEventListener('keydown', e => {
  if (e.key === 'Enter' && (e.ctrlKey || e.metaKey)) runCypher();
});
async function runCypher() {
  const q = $('#cypher-input').value.trim();
  if (!q) { toast('请输入 Cypher 查询', 'error'); return; }
  const out = $('#cypher-result');
  out.innerHTML = '<div class="muted">执行中…</div>';
  try {
    const r = await fetch(`${API}/query`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ query: q, params: {} }),
    });
    const data = await r.json();
    if (!r.ok || data.error) {
      out.innerHTML = `<div class="row-result" style="color: var(--red); border-color: var(--red);">${escapeHTML(data.error || data.detail || '查询失败')}</div>`;
      return;
    }
    out.innerHTML = `<div class="muted" style="margin-bottom: 10px;">${data.count} 条结果</div>` +
      data.rows.slice(0, 200).map(r => `<div class="row-result">${escapeHTML(JSON.stringify(r, null, 2))}</div>`).join('');
  } catch (e) {
    out.innerHTML = `<div class="row-result" style="color: var(--red);">${escapeHTML(e.message)}</div>`;
  }
}

// Modal 关闭
$('#modal').addEventListener('click', e => {
  if (e.target.id === 'modal') closeModal();
});

// ============== 工具 ==============
function escapeHTML(s) {
  if (s == null) return '';
  return String(s)
    .replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;')
    .replace(/"/g,'&quot;').replace(/'/g,'&#39;');
}
function escapeAttr(s) { return escapeHTML(s); }
function cssEscape(s) { return String(s).replace(/"/g, '\\"'); }

// 启动
loadOverview();
</script>
</body>
</html>
"""


@router.get("/webui", include_in_schema=False)
def serve_webui(request) -> HTMLResponse:
    """返回 DB-CAD Neo4j 管理 WebUI 页面。

    注意：函数签名接受 request 参数，原因是 main.py 里用 app.add_route("/webui", serve_webui, ...)
    直接把这个函数注册为路由，Starlette 会以 serve_webui(request) 形式调用；
    @router.get() 路径通过 FastAPI 自动识别 Request 类型注解，二者兼容。
    """
    return HTMLResponse(content=_HTML_TEMPLATE, status_code=200)


# ---------------------------------------------------------------------------
# 在 FastAPI 关闭时同步关闭 Neo4j Driver
# ---------------------------------------------------------------------------

import atexit as _atexit
_atexit.register(_close_driver)
