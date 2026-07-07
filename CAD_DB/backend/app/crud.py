from fastapi import HTTPException, status

from . import schemas
from .config import settings
from .storage_bridge import StorageBridgeClient, VersionRecord


storage_bridge = StorageBridgeClient(settings.storage_bridge_url, settings.storage_bridge_timeout_seconds)


def initialize_backend() -> None:
    storage_bridge.healthcheck()


def shutdown_backend() -> None:
    storage_bridge.close()


def create_project(payload: schemas.ProjectCreate):
    project = storage_bridge.get_project_by_name_or_none(payload.name)
    if project is not None:
        raise HTTPException(status_code=409, detail="Project name already exists")
    return storage_bridge.create_project(payload.name)


def get_project_or_404(project_id: str):
    project = storage_bridge.get_project_or_none(project_id)
    if project is None:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Project not found")
    return project


def get_project_by_name_or_404(project_name: str):
    project = storage_bridge.get_project_by_name_or_none(project_name)
    if project is None:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Project not found")
    return project


def create_model_version(project_id: str, payload: schemas.ModelVersionCreate):
    latest_version = storage_bridge.get_latest_version_or_none(project_id)
    if latest_version and payload.base_version == None:
        raise HTTPException(status_code=409, detail=f"base_version is required: latest version is {latest_version.version}")
    return storage_bridge.create_model_version(project_id, payload.author, payload.content, payload.base_version)
    

def get_latest_version_or_404(project_id: str):
    get_project_or_404(project_id)
    version = storage_bridge.get_latest_version_or_none(project_id)
    if version is None:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="No model version found")
    return version


def get_version_or_404(project_id: str, version_number: int):
    get_project_or_404(project_id)
    version = storage_bridge.get_version_or_none(project_id, version_number)
    if version is None:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Model version not found")
    return version


def list_versions(project_id: str, limit: int = 50, offset: int = 0):
    get_project_or_404(project_id)
    return storage_bridge.list_versions(project_id, limit=limit, offset=offset)


def deserialize_version(entity) -> schemas.ModelVersionRead:
    if not isinstance(entity, VersionRecord):
        raise HTTPException(status_code=500, detail="Invalid version entity type")
    return schemas.ModelVersionRead(id=entity.id, project_id=entity.project_id, version=entity.version, author=entity.author, content=entity.content, created_at=entity.created_at)
