"""Phase B（存储物理表示 v2）：JsonGraphEntity 的原生标签 + 一级扁平字段。

设计（history/STORAGE_CONSOLIDATION_PLAN §3"细"档 + history/REFACTOR_PLAN M3，v2 归档）：
- 原生标签：codec 语义标签 → Neo4j 标签（Body/Face/…），标签集合封闭、
  经 FOREACH 静态子句写入，不依赖 apoc；无法识别的标签落 ``:Unknown``
  并保留原始标签值（绝不静默丢弃）。
- 一级扁平字段：``props.sense → sense``；嵌套字典加前缀
  （``geometry.type → geometry_type``）。标量与同质原始类型列表直存；
  复杂值（嵌套结构、异质列表、NaN/Inf 浮点、None）不物化，只计入
  skipped 统计（序列化覆盖率），权威载荷仍在 ``props_json``。
- 读投影不变：``graph()`` 仍从 labels_json/props_json 重组，迁移前后
  逐字节等价由构造保证——原生字段是可查询的去规范化投影，不是第二真相源。

协议与客户端零改动（硬约束）。
"""

from __future__ import annotations

import json
import logging
import math
import re
from typing import Any

logger = logging.getLogger(__name__)

# codec 语义标签 → Neo4j 原生标签（封闭集合，与 entity_graph_serializer 一致）。
NATIVE_LABEL_BY_CODEC: dict[str, str] = {
    label: label.capitalize()
    for label in (
        "body",
        "lump",
        "shell",
        "subshell",
        "face",
        "loop",
        "coedge",
        "edge",
        "vertex",
        "wire",
        "transform",
        "unknown",
    )
}
SUPPORTED_NATIVE_LABELS: tuple[str, ...] = tuple(sorted(NATIVE_LABEL_BY_CODEC.values()))

# 需要建 entity_id 索引的热点标签。
INDEXED_NATIVE_LABELS: tuple[str, ...] = ("Body", "Face", "Edge", "Vertex")

_PROP_NAME = re.compile(r"[^0-9A-Za-z_]")
_PROP_MAX_LEN = 120


def native_labels(labels: list[str] | None) -> list[str]:
    """codec 标签列表 → 去重排序的原生标签；一个都不认识时落 Unknown。"""
    matched = {
        NATIVE_LABEL_BY_CODEC[str(label).lower()]
        for label in (labels or [])
        if str(label).lower() in NATIVE_LABEL_BY_CODEC
    }
    if not matched:
        return ["Unknown"]
    return sorted(matched)


def _storable_scalar(value: Any) -> bool:
    if value is None:
        return False  # Neo4j 不存 null 属性
    if isinstance(value, bool):
        return True
    if isinstance(value, int):
        return True
    if isinstance(value, float):
        return math.isfinite(value)  # NaN/Inf 不可存
    if isinstance(value, str):
        return True
    return False


def _storable_value(value: Any) -> bool:
    if isinstance(value, list):
        if not value:
            return True  # 空列表可存
        kinds = {type(item) for item in value}
        if kinds <= {int, float}:
            return all(_storable_scalar(item) for item in value)
        if kinds == {bool} or kinds == {str}:
            return True
        return False  # 异质 / 嵌套列表不物化
    return _storable_scalar(value)


def _prop_name(key: str) -> str:
    name = _PROP_NAME.sub("_", str(key))
    if not name or name[0].isdigit():
        name = f"_{name}"
    return name[:_PROP_MAX_LEN]


def native_fields(props: dict[str, Any] | None) -> tuple[dict[str, Any], int]:
    """一级扁平化；返回 (可物化字段, 跳过计数)。"""
    flat: dict[str, Any] = {}
    skipped = 0

    def put(key: str, value: Any) -> None:
        nonlocal skipped
        if _storable_value(value):
            flat[_prop_name(key)] = value
        else:
            skipped += 1

    for key, value in (props or {}).items():
        if isinstance(value, dict):
            for sub_key, sub_value in value.items():
                put(f"{key}_{sub_key}", sub_value)
        else:
            put(key, value)
    return flat, skipped


def entity_write_projection(node: dict[str, Any]) -> dict[str, Any]:
    """单实体节点的完整写入投影（权威 JSON + 原生标签/字段）。"""
    fields, skipped = native_fields(node.get("props", {}))
    labels = node.get("labels", []) or []
    return {
        "id": str(node["id"]),
        "labels_json": json.dumps(labels, ensure_ascii=False, separators=(",", ":")),
        "props_json": json.dumps(node.get("props", {}), ensure_ascii=False, separators=(",", ":")),
        "native_labels": native_labels(labels),
        "native_fields": fields,
        "native_skipped": skipped,
        "raw_labels": json.dumps(labels, ensure_ascii=False),
    }


def native_label_set_clauses(labels_expr: str) -> str:
    """FOREACH 静态子句：按封闭标签集合打原生标签（无 apoc 依赖，幂等）。

    labels_expr 是求值为标签列表的表达式：UNWIND 变量写 ``item.native_labels``，
    参数映射写 ``$projection.native_labels``。
    """
    return "\n                        ".join(
        f"FOREACH (ignore IN CASE WHEN '{label}' IN {labels_expr} THEN [1] ELSE [] END | SET e:{label})"
        for label in SUPPORTED_NATIVE_LABELS
    )


def migrate_native_v2(session: Any, batch_size: int = 500) -> dict[str, int]:
    """存量 JsonGraphEntity 补齐原生标签/字段（幂等：只处理 native_v2 IS NULL）。

    迁移只新增字段与标签，不触碰 labels_json/props_json——graph() 读投影
    在迁移前后逐字节等价。可在任意时刻安全重跑。
    """
    migrated = 0
    skipped_fields_total = 0
    while True:
        rows = session.run(
            "MATCH (e:JsonGraphEntity) WHERE e.native_v2 IS NULL "
            "RETURN id(e) AS internal_id, e.labels_json AS labels_json, e.props_json AS props_json "
            f"LIMIT {int(batch_size)}"
        ).data()
        if not rows:
            break
        for row in rows:
            try:
                labels = json.loads(row["labels_json"] or "[]")
                props = json.loads(row["props_json"] or "{}")
            except (TypeError, ValueError):
                labels, props = [], {}
            fields, skipped = native_fields(props)
            skipped_fields_total += skipped
            session.run(
                "MATCH (e) WHERE id(e) = $internal_id "
                "SET e += $fields, e.native_v2 = true, e.native_skipped = $skipped, e.raw_labels = $raw_labels "
                + native_label_set_clauses("$projection.native_labels"),
                internal_id=row["internal_id"],
                fields=fields,
                skipped=skipped,
                raw_labels=json.dumps(labels, ensure_ascii=False),
                projection={"native_labels": native_labels(labels)},
            ).consume()
            migrated += 1
    report = {"migrated": migrated, "skipped_fields": skipped_fields_total}
    if migrated:
        logger.info("native v2 migration: %s", report)
    return report
