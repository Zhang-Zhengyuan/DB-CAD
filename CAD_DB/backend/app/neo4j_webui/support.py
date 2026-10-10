"""
DB-CAD Neo4j 管理 WebUI — 直接连接 Neo4j，提供项目/版本/图数据查询界面。

访问地址: http://localhost:8000/  或 http://localhost:8000/webui
（与 FastAPI 主服务同端口，无需额外启动）

【数据模型】（统一命名，兼容旧 label）
  BridgeProject / DBCADProject  — 项目根节点
  BridgeVersion / DBCADVersion   — Mode0 全量版本（每次 Save 写一份完整 SAT）
  JsonGraphVersion / JsonGraphEntity — 当前 Mode1 纯 JSON 快照与可查询实体
  BridgeDeltaVersion / DBCADDelta — 历史 SAT 增量数据
  BridgeEntityGraphVersion / entity_graph_version — Entity Graph 版本
  part / DBCADPart                — ACIS N 叉拓扑树根

【实际关系】（以 Neo4j 实测为准，本版按真实存在的字段与关系重写）
  (BridgeProject)-[:HAS_VERSION]->(BridgeVersion)
  (BridgeProject)-[:HAS_EG_VERSION]->(BridgeEntityGraphVersion)
  JsonGraphVersion 通过 HAS_JSON_GRAPH_VERSION 关联项目
  BridgeDeltaVersion 没有 BELONGS_TO 关系，仅通过 project_id 属性挂载
  part 拓扑树是孤立节点，通过 BridgeVersion/BridgeEntityGraphVersion.part_name 引用
"""

import json
from typing import Any

from fastapi import Header, HTTPException

from ..config import settings


def _verify_admin_password(x_api_password: str | None = Header(default=None)) -> None:
    if not settings.api_password:
        raise HTTPException(
            status_code=503,
            detail="Configure CAD_DB_API_PASSWORD for the admin dashboard",
        )
    if x_api_password != settings.api_password:
        raise HTTPException(status_code=401, detail="Invalid API password")


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


def _run_cypher(
    query: str, params: dict[str, Any] | None = None
) -> list[dict[str, Any]]:
    driver = _get_driver()
    with driver.session(database=settings.neo4j_database) as session:
        result = session.run(query, params or {})
        return [dict(record) for record in result]


def _run_single_cypher(
    query: str, params: dict[str, Any] | None = None
) -> dict[str, Any] | None:
    rows = _run_cypher(query, params)
    return rows[0] if rows else None


def _iso(value: Any) -> str:
    """把 Neo4j datetime/字符串规整到 ISO。"""
    if value is None:
        return ""
    if hasattr(value, "isoformat"):
        return value.isoformat()
    return str(value)


_TOPOLOGY_LABEL_ZH = {
    "body": "Body 实体",
    "lump": "Lump 体块",
    "shell": "Shell 壳体",
    "face": "Face 面",
    "loop": "Loop 环",
    "coedge": "Coedge 半边",
    "edge": "Edge 边",
    "vertex": "Vertex 顶点",
    "transform": "Transform 变换",
}


def _semantic_label(
    labels: list[str], props: dict[str, Any] | None = None, body_name: str = ""
) -> str:
    """Return a readable CAD meaning while retaining the raw ACIS label."""
    lowered = {str(label).lower() for label in labels}
    if "body" in lowered and body_name:
        return f"{body_name} · Body"
    for label in labels:
        key = str(label).lower()
        if key in _TOPOLOGY_LABEL_ZH:
            return _TOPOLOGY_LABEL_ZH[key]
    entity_type = str((props or {}).get("entity_type") or "").strip()
    return entity_type or (str(labels[0]) if labels else "实体节点")


def _decode_changes(raw: Any) -> list[dict[str, str]]:
    """Decode the compact per-version change manifest for WebUI display."""
    if not raw:
        return []
    try:
        parsed = json.loads(raw) if isinstance(raw, str) else raw
    except (TypeError, ValueError):
        return []
    result: list[dict[str, str]] = []
    for item in parsed.get("changes", []) if isinstance(parsed, dict) else []:
        if not isinstance(item, dict):
            continue
        result.append(
            {
                "change_type": str(
                    item.get("changeType") or item.get("change_type") or ""
                ).upper(),
                "uuid": str(item.get("uuid") or item.get("id") or ""),
                "entity_type": str(
                    item.get("entityType") or item.get("entity_type") or ""
                ),
                "name": str(item.get("name") or ""),
            }
        )
    known_removed = {
        item["uuid"]
        for item in result
        if item["change_type"] == "REMOVE" and item["uuid"]
    }
    for removed_id in parsed.get("removed_ids", []) if isinstance(parsed, dict) else []:
        removed_uuid = str(removed_id)
        if removed_uuid not in known_removed:
            result.append(
                {
                    "change_type": "REMOVE",
                    "uuid": removed_uuid,
                    "entity_type": "",
                    "name": "",
                }
            )
    return [
        item for item in result if item["uuid"] or item["entity_type"] or item["name"]
    ]


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
