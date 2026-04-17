from fastapi import Depends, FastAPI, Header, HTTPException, Query, WebSocket, WebSocketDisconnect, status

from . import crud, schemas
from .config import settings
from .sync import sync_manager

app = FastAPI(title=settings.app_name, version="0.1.0")


def verify_api_password(x_api_password: str | None = Header(default=None)) -> None:
    if settings.api_password and x_api_password != settings.api_password:
        raise HTTPException(status_code=status.HTTP_401_UNAUTHORIZED, detail="Invalid API password")


@app.on_event("startup")
def on_startup() -> None:
    crud.initialize_backend()


@app.on_event("shutdown")
def on_shutdown() -> None:
    crud.shutdown_backend()


@app.get("/health")
def health() -> dict[str, str]:
    return {"status": "ok"}


@app.post("/projects", response_model=schemas.ProjectRead, status_code=201)
def create_project(
    payload: schemas.ProjectCreate,
    _: None = Depends(verify_api_password),
) -> schemas.ProjectRead:
    entity = crud.create_project(payload)
    return schemas.ProjectRead.model_validate(entity)


@app.get("/projects/{project_id}", response_model=schemas.ProjectRead)
def get_project(
    project_id: str,
    _: None = Depends(verify_api_password),
) -> schemas.ProjectRead:
    entity = crud.get_project_or_404(project_id)
    return schemas.ProjectRead.model_validate(entity)


@app.get("/projects/by-name/{project_name}", response_model=schemas.ProjectRead)
def get_project_by_name(
    project_name: str,
    _: None = Depends(verify_api_password),
) -> schemas.ProjectRead:
    entity = crud.get_project_by_name_or_404(project_name)
    return schemas.ProjectRead.model_validate(entity)


@app.post("/projects/{project_id}/models", response_model=schemas.SaveResult, status_code=201)
async def save_model(
    project_id: str,
    payload: schemas.ModelVersionCreate,
    _: None = Depends(verify_api_password),
) -> schemas.SaveResult:
    version = crud.create_model_version(project_id, payload)
    await sync_manager.broadcast(
        project_id,
        {
            "type": "model_saved",
            "project_id": project_id,
            "version": version.version,
            "author": version.author,
            "created_at": version.created_at.isoformat(),
        },
    )
    return schemas.SaveResult(version=version.version, created_at=version.created_at)


@app.get("/projects/{project_id}/models/latest", response_model=schemas.ModelVersionRead)
def get_latest_model(
    project_id: str,
    _: None = Depends(verify_api_password),
) -> schemas.ModelVersionRead:
    version = crud.get_latest_version_or_404(project_id)
    return crud.deserialize_version(version)


@app.get("/projects/{project_id}/models/{version}", response_model=schemas.ModelVersionRead)
def get_model_by_version(
    project_id: str,
    version: int,
    _: None = Depends(verify_api_password),
) -> schemas.ModelVersionRead:
    entity = crud.get_version_or_404(project_id, version)
    return crud.deserialize_version(entity)


@app.get("/projects/{project_id}/models/versions", response_model=list[schemas.ModelVersionRead])
def get_versions(
    project_id: str,
    limit: int = Query(default=50, ge=1, le=200),
    offset: int = Query(default=0, ge=0),
    _: None = Depends(verify_api_password),
) -> list[schemas.ModelVersionRead]:
    versions = crud.list_versions(project_id, limit=limit, offset=offset)
    return [crud.deserialize_version(item) for item in versions]


@app.websocket("/ws/projects/{project_id}")
async def ws_project_sync(websocket: WebSocket, project_id: str) -> None:
    if settings.api_password and websocket.query_params.get("password") != settings.api_password:
        await websocket.close(code=1008)
        return

    await sync_manager.connect(project_id, websocket)
    try:
        while True:
            await websocket.receive_text()
    except WebSocketDisconnect:
        await sync_manager.disconnect(project_id, websocket)
