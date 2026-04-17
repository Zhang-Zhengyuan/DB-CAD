import json

from fastapi import HTTPException, status
from sqlalchemy import Select, desc, func, select
from sqlalchemy.orm import Session

from . import models, schemas


def create_project(db: Session, payload: schemas.ProjectCreate) -> models.Project:
    exists_stmt: Select[tuple[models.Project]] = select(models.Project).where(models.Project.name == payload.name)
    if db.execute(exists_stmt).scalar_one_or_none() is not None:
        raise HTTPException(status_code=status.HTTP_409_CONFLICT, detail="Project name already exists")

    project = models.Project(name=payload.name)
    db.add(project)
    db.commit()
    db.refresh(project)
    return project


def get_project_or_404(db: Session, project_id: str) -> models.Project:
    stmt: Select[tuple[models.Project]] = select(models.Project).where(models.Project.id == project_id)
    project = db.execute(stmt).scalar_one_or_none()
    if project is None:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Project not found")
    return project


def get_project_by_name_or_404(db: Session, project_name: str) -> models.Project:
    stmt: Select[tuple[models.Project]] = select(models.Project).where(models.Project.name == project_name)
    project = db.execute(stmt).scalar_one_or_none()
    if project is None:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Project not found")
    return project


def create_model_version(db: Session, project_id: str, payload: schemas.ModelVersionCreate) -> models.ModelVersion:
    get_project_or_404(db, project_id)

    latest_version_stmt = select(func.max(models.ModelVersion.version)).where(models.ModelVersion.project_id == project_id)
    latest_version = db.execute(latest_version_stmt).scalar_one()
    next_version = 1 if latest_version is None else latest_version + 1

    if payload.base_version is not None and payload.base_version != latest_version:
        raise HTTPException(
            status_code=status.HTTP_409_CONFLICT,
            detail=f"Version conflict: latest version is {latest_version or 0}",
        )

    entity = models.ModelVersion(
        project_id=project_id,
        version=next_version,
        author=payload.author,
        content=json.dumps(payload.content, ensure_ascii=False),
    )
    db.add(entity)
    db.commit()
    db.refresh(entity)
    return entity


def get_latest_version_or_404(db: Session, project_id: str) -> models.ModelVersion:
    get_project_or_404(db, project_id)

    stmt = (
        select(models.ModelVersion)
        .where(models.ModelVersion.project_id == project_id)
        .order_by(desc(models.ModelVersion.version))
        .limit(1)
    )
    version = db.execute(stmt).scalar_one_or_none()
    if version is None:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="No model version found")
    return version


def get_version_or_404(db: Session, project_id: str, version_number: int) -> models.ModelVersion:
    get_project_or_404(db, project_id)

    stmt = select(models.ModelVersion).where(
        models.ModelVersion.project_id == project_id,
        models.ModelVersion.version == version_number,
    )
    version = db.execute(stmt).scalar_one_or_none()
    if version is None:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Model version not found")
    return version


def list_versions(db: Session, project_id: str, limit: int = 50, offset: int = 0) -> list[models.ModelVersion]:
    get_project_or_404(db, project_id)

    stmt = (
        select(models.ModelVersion)
        .where(models.ModelVersion.project_id == project_id)
        .order_by(desc(models.ModelVersion.version))
        .offset(offset)
        .limit(limit)
    )
    return list(db.execute(stmt).scalars().all())


def deserialize_version(entity: models.ModelVersion) -> schemas.ModelVersionRead:
    return schemas.ModelVersionRead(
        id=entity.id,
        project_id=entity.project_id,
        version=entity.version,
        author=entity.author,
        content=json.loads(entity.content),
        created_at=entity.created_at,
    )
