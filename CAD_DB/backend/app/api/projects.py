"""Project lookup, creation and metadata endpoints."""

from typing import Any

from fastapi import APIRouter, Depends

from .. import crud, schemas


from ..dependencies import verify_api_password

router = APIRouter()


@router.post("/projects", response_model=schemas.ProjectRead, status_code=201)
def create_project(
    payload: schemas.ProjectCreate,
    _: None = Depends(verify_api_password),
) -> schemas.ProjectRead:
    entity = crud.create_project(payload)
    return schemas.ProjectRead.model_validate(entity)


@router.get("/projects/{project_id}", response_model=schemas.ProjectRead)
def get_project(
    project_id: str,
    _: None = Depends(verify_api_password),
) -> schemas.ProjectRead:
    entity = crud.get_project_or_404(project_id)
    return schemas.ProjectRead.model_validate(entity)


@router.get("/projects/by-name/{project_name}", response_model=schemas.ProjectRead)
def get_project_by_name(
    project_name: str,
    _: None = Depends(verify_api_password),
) -> schemas.ProjectRead:
    entity = crud.get_project_by_name_or_404(project_name)
    return schemas.ProjectRead.model_validate(entity)


@router.get("/projects/{project_id}/meta")
def get_project_meta(
    project_id: str,
    _: None = Depends(verify_api_password),
) -> dict[str, Any]:
    crud.get_project_or_404(project_id)
    latest = crud.get_latest_version_or_none(project_id)
    return {
        "project_id": project_id,
        "latest_version": latest.version if latest else None,
        "latest_author": latest.author if latest else None,
        "latest_created_at": latest.created_at.isoformat() if latest else None,
    }
