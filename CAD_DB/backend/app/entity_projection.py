"""实体级图投影（V3_PLAN.md P6 / ARCHITECTURE.md ADR-3）。

SAT + 版本清单是唯一权威；实体图是 worker 在组件发射管线产出的可查询
投影。本模块做投影规范化：worker 侧 root 节点 id 是临时 sid（n0/comp-0），
网关以 SAT 内容哈希重编（同 P2 组件 sid 重编语义），使投影幂等——
同一组件（同 hash_sat）的投影恒相同，重复写入零成本跳过。

细档物理表示（原生标签 + 一级扁平字段 + ``:Unknown`` 兜底）复用
json_graph_native 的纯映射函数，字段语义与 v2 Phase B 完全一致。
"""

from __future__ import annotations

import hashlib
import json
import logging
from typing import Any

logger = logging.getLogger(__name__)

GRAPH_SCHEMA = "dbcad.entity_graph.v2"

# 内容寻址剥离字段：组件级统计/风险标记/身份链不参与实体内容键——
# 同结构几何（如两个同参 cube 的 body/face）必须收敛到同一节点。
_KEY_STRIPPED = {"uuid", "prev_id", "topology_node_count", "topology_rel_count",
                 "fallback", "top_level"}


class ProjectionError(ValueError):
    """投影载荷不符合 worker 契约（错误显式，不静默兜底）。"""


def entity_key(node: dict[str, Any]) -> str:
    """实体内容键（P8/Phase D）：labels + 内容 props 的规范化哈希。

    不可变实体节点按此键 MERGE——同内容实体跨组件/跨版本共享同一节点
    （D 栈"不可变实体+内容寻址共享"原语）。
    """
    labels = [str(l) for l in node.get("labels", [])]
    props = node.get("props", {})
    payload = {k: v for k, v in props.items() if k not in _KEY_STRIPPED}
    canonical = json.dumps(
        {"labels": labels, "props": payload},
        ensure_ascii=False, sort_keys=True, separators=(",", ":"),
    )
    return hashlib.sha256(canonical.encode("utf-8")).hexdigest()


def _fingerprint(node: dict[str, Any]) -> tuple:
    """结构指纹（第三层匹配）：内容变化后识别"同一结构角色"的实体。"""
    props = node.get("props", {})
    geometry = props.get("geometry") if isinstance(props.get("geometry"), dict) else {}
    return (
        tuple(str(l) for l in node.get("labels", [])),
        str(geometry.get("type", "")),
        str(props.get("entity_type", "")),
    )


def match_entities(parent: dict[str, Any], child: dict[str, Any]) -> dict[str, str]:
    """父子投影的实体身份匹配（child entity_id → parent entity_id）。

    三层策略（成本递增）：① id 相等（同会话指针稳定，实证常态）；
    ② 显式 prev_id；③ 结构指纹（label+几何类型+entity_type 一对一贪心，
    按出现序对齐）。漏配安全：匹配不上即视为新实体。
    """
    parent_by_id = {str(n["id"]): n for n in parent["nodes"]}
    matched: dict[str, str] = {}
    used_parent: set[str] = set()
    # ①② id 相等与 prev_id
    for node in child["nodes"]:
        cid = str(node["id"])
        prev = node.get("props", {}).get("prev_id")
        target = None
        if prev is not None and str(prev) in parent_by_id and str(prev) not in used_parent:
            target = str(prev)
        elif cid in parent_by_id and cid not in used_parent:
            target = cid
        if target is not None:
            matched[cid] = target
            used_parent.add(target)
    # ③ 结构指纹（仅未匹配节点，按序一对一）
    child_pending = [n for n in child["nodes"] if str(n["id"]) not in matched]
    parent_free: dict[tuple, list[dict[str, Any]]] = {}
    for node in parent["nodes"]:
        if str(node["id"]) in used_parent:
            continue
        parent_free.setdefault(_fingerprint(node), []).append(node)
    for node in child_pending:
        queue = parent_free.get(_fingerprint(node))
        if queue:
            mate = queue.pop(0)
            matched[str(node["id"])] = str(mate["id"])
            used_parent.add(str(mate["id"]))
    return matched


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


def write_projections(
    store: Any, pairs: list[tuple[str | None, dict | None, str | None]]
) -> dict[str, int]:
    """批量写入组件投影（commit 之后调用）。pairs = (hash, graph, parent_hash)，
    parent_hash 用于 generation 匹配（命令修改的组件传其命令前哈希）。
    投影是派生数据：写入被拒只告警不阻断版本（版本是权威，投影可由 graph
    资产重建），统计如实上报。"""
    written = skipped = entities = entity_added = entity_shared = derived = 0
    for component_hash, graph, parent_hash in pairs:
        if not component_hash or not isinstance(graph, dict):
            continue
        try:
            outcome = store.write_entity_projection(
                str(component_hash), graph, parent_hash or None
            )
        except ProjectionError as exc:
            logger.warning("entity projection rejected for %s: %s", str(component_hash)[:12], exc)
            continue
        entities += int(outcome.get("entities", 0))
        entity_added += int(outcome.get("entity_added", 0))
        entity_shared += int(outcome.get("entity_shared", 0))
        derived += int(outcome.get("derived", 0))
        if outcome.get("written"):
            written += 1
        else:
            skipped += 1
    return {
        "written": written,
        "skipped": skipped,
        "entities": entities,
        "entity_added": entity_added,
        "entity_shared": entity_shared,
        "derived": derived,
    }
