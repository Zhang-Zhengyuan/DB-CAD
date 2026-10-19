"""实体级图投影（V3_PLAN.md P6 / ARCHITECTURE.md ADR-3）。

SAT + 版本清单是唯一权威；实体图是 worker 在组件发射管线产出的可查询
投影。本模块做投影规范化：worker 侧 root 节点 id 是临时 sid（n0/comp-0），
网关以 SAT 内容哈希重编（同 P2 组件 sid 重编语义），使投影幂等——
同一组件（同 hash_sat）的投影恒相同，重复写入零成本跳过。

细档物理表示（原生标签 + 一级扁平字段 + ``:Unknown`` 兜底）复用
json_graph_native 的纯映射函数，字段语义与 v2 Phase B 完全一致。
"""

from __future__ import annotations

import logging
from typing import Any

logger = logging.getLogger(__name__)

GRAPH_SCHEMA = "dbcad.entity_graph.v2"


class ProjectionError(ValueError):
    """投影载荷不符合 worker 契约（错误显式，不静默兜底）。"""


def normalize_entity_projection(component_hash: str, graph: dict[str, Any]) -> dict[str, Any]:
    """规范化投影：校验 schema、把临时 root id 重编为组件内容哈希。

    返回 {"nodes": [...], "rels": [...], "serializer_build": str}——nodes/rels
    结构与 worker codec 输出一致（root 的 id/props.uuid 已重编）。
    """
    if not isinstance(graph, dict) or graph.get("schema") != GRAPH_SCHEMA:
        raise ProjectionError(f"entity graph schema mismatch: {graph.get('schema')!r}")
    nodes = graph.get("nodes")
    rels = graph.get("rels")
    if not isinstance(nodes, list) or not nodes:
        raise ProjectionError("entity graph has no nodes")
    if not isinstance(rels, list):
        rels = []

    roots = [n for n in nodes if isinstance(n, dict) and n.get("props", {}).get("top_level")]
    if len(roots) != 1:
        raise ProjectionError(f"entity graph must have exactly one root, got {len(roots)}")
    root = roots[0]
    temp_id = str(root.get("id", ""))
    if not temp_id:
        raise ProjectionError("entity graph root has no id")

    renamed = dict(root)
    renamed["id"] = component_hash
    props = dict(root.get("props", {}))
    props["uuid"] = component_hash
    renamed["props"] = props

    out_nodes = [renamed if n is root else n for n in nodes]
    out_rels = []
    for rel in rels:
        if not isinstance(rel, dict):
            continue
        fixed = dict(rel)
        if fixed.get("start") == temp_id:
            fixed["start"] = component_hash
        if fixed.get("end") == temp_id:
            fixed["end"] = component_hash
        out_rels.append(fixed)
    return {
        "schema": GRAPH_SCHEMA,
        "nodes": out_nodes,
        "rels": out_rels,
        "serializer_build": str(graph.get("serializer_build", "")),
    }


def projection_stats(projection: dict[str, Any]) -> dict[str, int]:
    return {"entities": len(projection["nodes"]), "rels": len(projection["rels"])}


def write_projections(store: Any, pairs: list[tuple[str | None, dict | None]]) -> dict[str, int]:
    """批量写入组件投影（commit 之后调用）。投影是派生数据：写入被拒只告警
    不阻断版本（版本是权威，投影可由 graph 资产重建），统计如实上报。"""
    written = skipped = entities = 0
    for component_hash, graph in pairs:
        if not component_hash or not isinstance(graph, dict):
            continue
        try:
            outcome = store.write_entity_projection(str(component_hash), graph)
        except ProjectionError as exc:
            logger.warning("entity projection rejected for %s: %s", str(component_hash)[:12], exc)
            continue
        entities += int(outcome.get("entities", 0))
        if outcome.get("written"):
            written += 1
        else:
            skipped += 1
    return {"written": written, "skipped": skipped, "entities": entities}
