from fastapi import HTTPException, status

from . import schemas
from .config import settings
from .storage_bridge import StorageBridgeClient, VersionRecord


storage_bridge = (
    StorageBridgeClient(settings.storage_bridge_url, settings.storage_bridge_timeout_seconds)
    if settings.storage_bridge_url.strip() and not settings.direct_json_mode
    else None
)
direct_store = None


def initialize_backend() -> None:
    if storage_bridge is not None:
        storage_bridge.healthcheck()
    else:
        from .neo4j_store import Neo4jStore
        global direct_store
        direct_store = Neo4jStore()
        direct_store.initialize()


def shutdown_backend() -> None:
    if storage_bridge is not None:
        storage_bridge.close()
    elif direct_store is not None:
        direct_store.close()


def create_project(payload: schemas.ProjectCreate):
    backend = storage_bridge or direct_store
    project = backend.get_project_by_name_or_none(payload.name)
    if project is not None:
        raise HTTPException(status_code=409, detail="Project name already exists")
    return backend.create_project(payload.name)


def get_project_or_404(project_id: str):
    backend = storage_bridge or direct_store
    project = backend.get_project_or_none(project_id)
    if project is None:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Project not found")
    return project


def get_project_by_name_or_404(project_name: str):
    backend = storage_bridge or direct_store
    project = backend.get_project_by_name_or_none(project_name)
    if project is None:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Project not found")
    return project


def create_model_version(project_id: str, payload: schemas.ModelVersionCreate):
    # A caller supplied base_version is a compare-and-swap token.  Never
    # rewrite it after a conflict: doing so turns a genuine collaboration
    # conflict into an invisible last-write-wins overwrite.  The bridge owns
    # the atomic version check, so a single request is sufficient here.
    backend = storage_bridge or direct_store
    return backend.create_model_version(
        project_id, payload.author, payload.content, payload.base_version
    )
    

def get_latest_version_or_404(project_id: str):
    get_project_or_404(project_id)
    backend = storage_bridge or direct_store
    version = backend.get_latest_version_or_none(project_id)
    if version is None:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="No model version found")
    return version


def get_latest_version_or_none(project_id: str):
    """Return the latest version without turning an empty project into 404."""
    get_project_or_404(project_id)
    backend = storage_bridge or direct_store
    return backend.get_latest_version_or_none(project_id)


def get_version_or_404(project_id: str, version_number: int):
    get_project_or_404(project_id)
    backend = storage_bridge or direct_store
    version = backend.get_version_or_none(project_id, version_number)
    if version is None:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Model version not found")
    return version


def list_versions(project_id: str, limit: int = 50, offset: int = 0):
    get_project_or_404(project_id)
    backend = storage_bridge or direct_store
    return backend.list_versions(project_id, limit=limit, offset=offset)


def deserialize_version(entity) -> schemas.ModelVersionRead:
    if not isinstance(entity, VersionRecord):
        raise HTTPException(status_code=500, detail="Invalid version entity type")
    return schemas.ModelVersionRead(id=entity.id, project_id=entity.project_id, version=entity.version, author=entity.author, content=entity.content, created_at=entity.created_at)
