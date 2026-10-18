"""/api/v1/projects：项目目录（V3_PLAN.md §2）。"""

from datetime import datetime

from fastapi import APIRouter, Depends, HTTPException, status
from pydantic import BaseModel, Field

from ...dependencies import verify_api_password
from ...v1_store import get_v1_store

router = APIRouter(dependencies=[Depends(verify_api_password)])


class ProjectCreate(BaseModel):
    name: str = Field(min_length=1, max_length=200)


class ProjectRename(BaseModel):
    name: str = Field(min_length=1, max_length=200)


class ProjectRead(BaseModel):
    id: str
    name: str
    created_at: datetime
    updated_at: datetime
    head_version: int | None = None


def _project_json(record) -> ProjectRead:
    store = get_v1_store()
    head = store.latest(record.id)
    return ProjectRead(
        id=record.id,
        name=record.name,
        created_at=record.created_at,
        updated_at=record.updated_at,
        head_version=head.number if head else None,
    )


@router.get("/projects", response_model=list[ProjectRead])
def list_projects() -> list[ProjectRead]:
    return [_project_json(record) for record in get_v1_store().list_projects()]


@router.post("/projects", response_model=ProjectRead, status_code=status.HTTP_201_CREATED)
def create_project(payload: ProjectCreate) -> ProjectRead:
    store = get_v1_store()
    record = store.create_project(payload.name)
    return _project_json(record)


def _project_or_404(project_id: str):
    record = get_v1_store().get_project_or_none(project_id)
    if record is None:
        raise HTTPException(status.HTTP_404_NOT_FOUND, "Project not found")
    return record


@router.get("/projects/{project_id}", response_model=ProjectRead)
def get_project(project_id: str) -> ProjectRead:
    return _project_json(_project_or_404(project_id))


@router.patch("/projects/{project_id}", response_model=ProjectRead)
def rename_project(project_id: str, payload: ProjectRename) -> ProjectRead:
    _project_or_404(project_id)
    record = get_v1_store().rename_project(project_id, payload.name)
    return _project_json(record)


@router.delete("/projects/{project_id}", status_code=status.HTTP_204_NO_CONTENT)
def delete_project(project_id: str) -> None:
    _project_or_404(project_id)
    get_v1_store().delete_project(project_id)
