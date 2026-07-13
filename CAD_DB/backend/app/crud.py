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
    # 重试循环：storage_bridge 端会在 base_version != latest 时返回 409。
    # 真实的并发场景下两个请求可能都"看到"同一个 latest，都传同一个 base_version，
    # 第二个请求到达 storage_bridge 时 latest 已经被第一个请求改了。
    # 这里最多重试 3 次，每次重新拉 latest 来设置 base_version。
    print(f"[crud create_model_version] ENTER project_id={project_id} payload.base_version={payload.base_version}", flush=True)
    last_error: Exception | None = None
    for attempt in range(3):
        latest_version = storage_bridge.get_latest_version_or_none(project_id)
        print(f"[crud create_model_version] attempt={attempt} latest={latest_version.version if latest_version else None} project_id={project_id}", flush=True)
        if latest_version and payload.base_version is None:
            payload.base_version = latest_version.version
            print(f"[crud create_model_version] attempt={attempt} set payload.base_version={payload.base_version} from latest", flush=True)
        try:
            result = storage_bridge.create_model_version(
                project_id, payload.author, payload.content, payload.base_version
            )
            print(f"[crud create_model_version] attempt={attempt} SUCCESS v={result.version} project_id={project_id}", flush=True)
            return result
        except HTTPException as ex:
            print(f"[crud create_model_version] attempt={attempt} HTTPException status={ex.status_code} detail={ex.detail} project_id={project_id}", flush=True)
            if ex.status_code != status.HTTP_409_CONFLICT:
                raise
            # 冲突：清掉 base_version，让下一次循环重新读 latest
            last_error = ex
            payload.base_version = None
            continue
    # 3 次都冲突，抛出最后一次的错误
    assert last_error is not None
    raise last_error
    

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
