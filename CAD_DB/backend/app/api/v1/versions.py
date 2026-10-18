"""/api/v1 版本链、定版标签、分支与场景清单（V3_PLAN.md §2）。

命令全序即版本链（ADR-2）：每个版本 = 一次提交（P1 为导入；P2 起为命令）。
分支为 M4.4 预留：P1 建分支只记录 head 指针，不改变主链提交语义。
"""


from fastapi import APIRouter, Depends, HTTPException, status
from pydantic import BaseModel, Field

from ...dependencies import verify_api_password
from ...events_hub import hub
from ...v1_store import V1Version, get_v1_store
from .projects import _project_or_404

router = APIRouter(dependencies=[Depends(verify_api_password)])


class VersionTagCreate(BaseModel):
    name: str = Field(min_length=1, max_length=100)
    version: int = Field(ge=1)
    author: str = "tagger"


class BranchCreate(BaseModel):
    name: str = Field(min_length=1, max_length=100)
    from_version: int = Field(ge=1)


def _version_or_404(project_id: str, version: int) -> V1Version:
    record = get_v1_store().get_version(project_id, version)
    if record is None:
        raise HTTPException(status.HTTP_404_NOT_FOUND, f"Version {version} not found")
    return record


def _version_summary(version: V1Version) -> dict:
    return {
        "version": version.number,
        "message": version.message,
        "author": version.author,
        "created_at": version.created_at,
        "base_version": version.base_version,
        "component_count": len(version.components),
    }


@router.get("/projects/{project_id}/versions")
def list_versions(project_id: str) -> dict:
    _project_or_404(project_id)
    store = get_v1_store()
    return {
        "versions": [_version_summary(v) for v in store.list_versions(project_id)],
        "tags": store.list_tags(project_id),
    }


@router.post("/projects/{project_id}/versions", status_code=status.HTTP_201_CREATED)
def tag_version(project_id: str, payload: VersionTagCreate) -> dict:
    """定版：给既有微版本打标签。"""
    _project_or_404(project_id)
    tag = get_v1_store().tag_version(project_id, payload.name, payload.version)
    hub.publish(project_id, "version.tagged", {"tag": tag, "author": payload.author})
    return tag


@router.get("/projects/{project_id}/versions/{version}")
def get_version(project_id: str, version: int) -> dict:
    _project_or_404(project_id)
    record = _version_or_404(project_id, version)
    return {
        **_version_summary(record),
        "components": [c.to_scene_json() for c in record.components],
    }


@router.get("/projects/{project_id}/scene")
def get_scene(project_id: str, v: int | None = None) -> dict:
    """场景清单：组件引用（hash/名称/颜色/包围盒），前端据此拉资产。"""
    _project_or_404(project_id)
    store = get_v1_store()
    version = store.get_version(project_id, v) if v is not None else store.latest(project_id)
    if version is None:
        raise HTTPException(
            status.HTTP_404_NOT_FOUND, "Project has no versions" if v is None else f"Version {v} not found"
        )
    return {
        "project_id": project_id,
        "version": version.number,
        "components": [c.to_scene_json() for c in version.components],
    }


@router.get("/projects/{project_id}/branches")
def list_branches(project_id: str) -> dict:
    _project_or_404(project_id)
    return {"branches": get_v1_store().list_branches(project_id)}


@router.post("/projects/{project_id}/branches", status_code=status.HTTP_201_CREATED)
def create_branch(project_id: str, payload: BranchCreate) -> dict:
    _project_or_404(project_id)
    branch = get_v1_store().create_branch(project_id, payload.name, payload.from_version)
    hub.publish(project_id, "branch.created", branch)
    return branch
