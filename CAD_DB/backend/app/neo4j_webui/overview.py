"""Overview endpoints for the administrator dashboard."""

from typing import Any
from fastapi import APIRouter
from .support import _run_cypher, _label_in, _count_with, _invalidate_label_cache

router = APIRouter()


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
    eg_label = _label_in("BridgeEntityGraphVersion", "entity_graph_version")

    total_projects = _count_with(
        "MATCH (n) WHERE any(l IN labels(n) WHERE l IN ['BridgeProject','DBCADProject','Project']) RETURN count(n) AS c"
    )
    total_mode0 = _count_with(
        "MATCH (n) WHERE any(l IN labels(n) WHERE l IN ['BridgeVersion','DBCADVersion','ModelVersion']) RETURN count(n) AS c"
    )
    total_mode1 = _count_with("MATCH (n:JsonGraphVersion) RETURN count(n) AS c")
    total_eg = _count_with(f"MATCH (n:{eg_label}) RETURN count(n) AS c")

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
              WHEN l IN ['BridgeProject','DBCADProject','Project'] THEN '项目'
              WHEN l IN ['BridgeVersion','DBCADVersion','ModelVersion'] THEN 'Mode0版本'
              WHEN l IN ['JsonGraphVersion'] THEN 'Mode1 JSON版本'
              WHEN l IN ['JsonGraphEntity'] THEN 'Mode1实体'
              WHEN l IN ['BridgeDeltaVersion','DBCADDelta'] THEN '历史SAT增量'
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
        "total_mode1_json_versions": int(total_mode1),
        "total_entity_graph_versions": int(total_eg),
        "label_counts": [
            {"label": r["label"], "count": int(r["n"] or 0)} for r in label_stats
        ],
        "relationship_counts": [
            {"type": r["rel_type"], "count": int(r["n"] or 0)} for r in rel_stats
        ],
        "topology_buckets": [
            {"bucket": r["bucket"], "count": int(r["total"] or 0)}
            for r in topology_buckets
        ],
    }
