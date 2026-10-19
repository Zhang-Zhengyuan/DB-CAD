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
    return {
        "project_id": project_id,
        "version": version.number,
        "label": label,
        "components": len(hashes),
        "count": len(entities),
        "entities": entities,
    }
