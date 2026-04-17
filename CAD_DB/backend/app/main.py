import logging
from logging.handlers import RotatingFileHandler
from pathlib import Path

from fastapi import Depends, FastAPI, Header, HTTPException, Query, Request, WebSocket, WebSocketDisconnect, status
from fastapi.responses import JSONResponse

from . import crud, schemas
from .config import settings
from .sync import sync_manager

app = FastAPI(title=settings.app_name, version="0.1.0")
logger = logging.getLogger(__name__)
LOG_FILE_PATH = (Path(__file__).resolve().parent.parent / "logs" / "backend-error.log")


def _setup_file_logging() -> None:
    log_dir = LOG_FILE_PATH.parent
    log_dir.mkdir(parents=True, exist_ok=True)
    log_file = LOG_FILE_PATH

    formatter = logging.Formatter("%(asctime)s %(levelname)s [%(name)s] %(message)s")
    file_handler = RotatingFileHandler(log_file, maxBytes=2 * 1024 * 1024, backupCount=3, encoding="utf-8")
    file_handler.setFormatter(formatter)
    file_handler.setLevel(logging.INFO)

    root_logger = logging.getLogger()
    root_logger.setLevel(logging.INFO)
    has_same_handler = any(
        isinstance(h, RotatingFileHandler) and getattr(h, "baseFilename", "") == str(log_file.resolve())
        for h in root_logger.handlers
    )
    if not has_same_handler:
        root_logger.addHandler(file_handler)


def verify_api_password(x_api_password: str | None = Header(default=None)) -> None:
    if settings.api_password and x_api_password != settings.api_password:
        raise HTTPException(status_code=status.HTTP_401_UNAUTHORIZED, detail="Invalid API password")


@app.on_event("startup")
def on_startup() -> None:
    _setup_file_logging()
    crud.initialize_backend()
    logger.info("Backend startup completed, log file: %s", LOG_FILE_PATH)
    print(f"[BACKEND] error log file: {LOG_FILE_PATH}")


@app.on_event("shutdown")
def on_shutdown() -> None:
    crud.shutdown_backend()


@app.exception_handler(HTTPException)
async def http_exception_handler(request: Request, exc: HTTPException) -> JSONResponse:
    if exc.status_code >= 500:
        logger.error(
            "HTTPException: method=%s path=%s status=%s detail=%s",
            request.method,
            request.url.path,
            exc.status_code,
            exc.detail,
        )
    return JSONResponse(status_code=exc.status_code, content={"detail": exc.detail})


@app.middleware("http")
async def request_logging_middleware(request: Request, call_next):
    try:
        response = await call_next(request)
        if response.status_code >= 500:
            logger.error("HTTP %s %s -> %s", request.method, request.url.path, response.status_code)
        return response
    except Exception as ex:
        logger.exception("Unhandled exception on %s %s: %s", request.method, request.url.path, ex)
        raise


@app.get("/health")
def health() -> dict[str, str]:
    return {
        "status": "ok",
        "build": "diag-20260417-3",
        "log_file": str(LOG_FILE_PATH),
    }


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
