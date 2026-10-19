"""Overview endpoints for the administrator dashboard (V1 model)."""

from typing import Any
from fastapi import APIRouter
from .support import _run_cypher, _invalidate_label_cache

router = APIRouter()


@router.get("/overview")
def get_overview() -> dict[str, Any]:
    """数据库全局概览：节点、分 label 统计、关系、V1 模型累计数。

    v2 历史 label（JsonGraph*/Bridge* 等）归入"历史(v2)"桶只读保留——
    P7 起不再写入，但存量数据仍在库中可审计。
    """
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

    total_projects = _run_cypher("MATCH (p:V1Project) RETURN count(p) AS c")
    total_versions = _run_cypher("MATCH (v:V1Version) RETURN count(v) AS c")
    total_components = _run_cypher("MATCH (c:V1Component) RETURN count(c) AS c")
    total_entities = _run_cypher("MATCH (e:V1Entity) RETURN count(e) AS c")

    total_nodes = sum(int(r["n"] or 0) for r in label_stats)
    total_rels = sum(int(r["n"] or 0) for r in rel_stats)

    # 节点分类汇总：V1 模型 + V1Entity 原生拓扑标签 + 历史(v2) 只读归档
    topology_buckets = _run_cypher(
        """
        MATCH (n)
        WITH labels(n) AS ls, count(*) AS n
        RETURN
            CASE
              WHEN 'V1Entity' IN ls THEN '实体投影(V1)'
              WHEN 'V1Project' IN ls THEN '项目(V1)'
              WHEN 'V1Version' IN ls THEN '版本(V1)'
              WHEN 'V1Component' IN ls THEN '组件(V1)'
              WHEN any(l IN ls WHERE l STARTS WITH 'JsonGraph'
                       OR l STARTS WITH 'Bridge' OR l IN ['Project','ModelVersion','part','body','lump','shell','face','loop','coedge','edge','vertex','transform','point','entity_node','DBCADPart','DBCADVersion','DBCADDelta'])
                THEN '历史(v2 只读)'
              ELSE '其他'
            END AS bucket,
            sum(n) AS total
        ORDER BY total DESC
        """
    )

    def _count(rows: list[dict[str, Any]]) -> int:
        return int(rows[0]["c"]) if rows else 0

    return {
        "total_nodes": int(total_nodes),
        "total_relationships": int(total_rels),
        "total_projects": _count(total_projects),
        "total_versions": _count(total_versions),
        "total_components": _count(total_components),
        "total_entities": _count(total_entities),
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
