"""
DB-CAD Neo4j 管理 WebUI — 直接连接 Neo4j，提供项目/版本/投影浏览界面。

访问地址: http://localhost:8000/webui（与 FastAPI 主服务同端口）

【V1 数据模型】（v3 服务端权威，P7 起唯一写入模型）
  V1Project                      — 项目
  V1Version                      — 版本链节点（project_id 属性挂项目）
    -[:CONTAINS {order}]-> V1Component（不可变，内容寻址 hash_sat）
  V1Component -[:HAS_ENTITY]-> V1Entity（实体级投影，P6/ADR-3：
    原生标签 Body/Face/.../Unknown + 一级扁平字段；权威载荷 labels_json/
    props_json）  V1Entity -[:TOPOLOGY {rel_type}]-> V1Entity（组件内拓扑）

  v2 历史 label（JsonGraph*/Bridge* 等）不再写入，存量数据只读保留。
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
