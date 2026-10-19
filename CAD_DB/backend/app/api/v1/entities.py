"""/api/v1 实体级投影查询（V3_PLAN.md P6 / ADR-3）。

按版本定位组件清单 → 查各组件的实体级投影节点（原生标签 + 一级扁平字段，
细档同 v2 Phase B 语义）。label 大小写不敏感（face / Face 皆可）。
WebUI / AI 场景摘要 / 演示共用的只读端点。
"""

from fastapi import APIRouter, Depends, HTTPException, Query, status

from ...dependencies import verify_api_password
from ...v1_store import get_v1_store
from .projects import _project_or_404

router = APIRouter(dependencies=[Depends(verify_api_password)])


@router.get("/projects/{project_id}/entities")
def list_entities(
    project_id: str,
    v: int | None = None,
    label: str | None = None,
    limit: int = Query(default=1000, ge=1, le=5000),
) -> dict:
    _project_or_404(project_id)
    store = get_v1_store()
    if v is not None:
        version = store.get_version(project_id, v)
        if version is None:
            raise HTTPException(status.HTTP_404_NOT_FOUND, f"Version {v} not found")
    else:
        version = store.latest(project_id)
        if version is None:
            raise HTTPException(status.HTTP_404_NOT_FOUND, "Project has no versions")
    hashes = [c.hash_sat for c in version.components]
    entities = store.query_entities(hashes, label=label, limit=limit)
    derived = store.count_derived_chains(hashes)
    return {
        "project_id": project_id,
        "version": version.number,
        "label": label,
        "components": len(hashes),
        "count": len(entities),
        "derived_chains": derived,
        "entities": entities,
    }


@router.get("/projects/{project_id}/diff")
def diff_versions(project_id: str, from_v: int, to_v: int) -> dict:
    """实体级版本对比（P8/Phase D，语义 diff 的数据基础）：

    组件级（集合对称差）+ 实体内容级（entity_key 差集）+ generation 链数。
    前端叠加高亮 / 论文实验 / 协同 review 共用。
    """
    _project_or_404(project_id)
    store = get_v1_store()
    base = store.get_version(project_id, from_v)
    target = store.get_version(project_id, to_v)
    if base is None or target is None:
        raise HTTPException(
            status.HTTP_404_NOT_FOUND, f"Versions {from_v} or {to_v} not found"
        )
    base_components = {c.hash_sat for c in base.components}
    target_components = {c.hash_sat for c in target.components}
    base_entities = store.query_entities(sorted(base_components), limit=5000)
    target_entities = store.query_entities(sorted(target_components), limit=5000)
    base_keys = {e["entity_key"]: e for e in base_entities if e.get("entity_key")}
    target_keys = {e["entity_key"]: e for e in target_entities if e.get("entity_key")}
    added = sorted(target_keys.keys() - base_keys.keys())
    removed = sorted(base_keys.keys() - target_keys.keys())
    shared = base_keys.keys() & target_keys.keys()
    derived_in_range = store.count_derived_chains(sorted(target_components))
    return {
        "project_id": project_id,
        "from_version": from_v,
        "to_version": to_v,
        "components": {
            "added": sorted(target_components - base_components),
            "removed": sorted(base_components - target_components),
            "shared": len(base_components & target_components),
        },
        "entities": {
            "added": len(added),
            "removed": len(removed),
            "shared": len(shared),
            # 明细截断到 200，防大 diff 响应膨胀
            "added_keys": added[:200],
            "removed_keys": removed[:200],
        },
        "derived_chains": derived_in_range,
    }


@router.post("/admin/compact-entities")
def compact_orphan_entities() -> dict:
    """压实（P8.5）：清理不再被任何组件投影引用的孤立实体节点（幂等）。
    版本链淘汰旧组件后其独有内容的实体成为孤魂；定期调用控节点膨胀。
    链短路（DERIVED_FROM 传递闭包合并）为后续优化，当前只清孤立节点。"""
    return {"removed_entities": get_v1_store().compact_orphan_entities()}
