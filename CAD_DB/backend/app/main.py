import logging
import json
from datetime import datetime, timezone
from logging.handlers import RotatingFileHandler
from pathlib import Path
from typing import Any
from uuid import uuid4

from fastapi import Depends, FastAPI, Header, HTTPException, Query, Request, WebSocket, WebSocketDisconnect, status
from fastapi.responses import JSONResponse
from pydantic import ValidationError

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
    from . import neo4j_entity_store
    if settings.storage_backend == "neo4j_entities":
        neo4j_entity_store.initialize_entity_store()
    logger.info("Backend startup completed, log file: %s", LOG_FILE_PATH)
    print(f"[BACKEND] error log file: {LOG_FILE_PATH}")


@app.on_event("shutdown")
def on_shutdown() -> None:
    crud.shutdown_backend()
    from . import neo4j_entity_store
    neo4j_entity_store.shutdown_entity_store()


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
        "build": datetime.now(timezone.utc).strftime("%Y%m%d-%H%M%S"),
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


def _model_saved_event(
    project_id: str,
    version: Any,
    *,
    trigger: str,
    request_id: str | None = None,
    source_client_id: str | None = None,
    include_content: bool = False,
) -> dict[str, Any]:
    event = {
        "type": "model_saved",
        "project_id": project_id,
        "version": version.version,
        "author": version.author,
        "created_at": version.created_at.isoformat(),
        "trigger": trigger,
        "include_content": include_content,
    }
    if include_content:
        event["content"] = version.content
    if request_id:
        event["request_id"] = request_id
    if source_client_id:
        event["source_client_id"] = source_client_id
    return event


def _entity_graph_saved_event(
    project_id: str,
    version: Any,
    *,
    trigger: str,
    request_id: str | None = None,
    source_client_id: str | None = None,
) -> dict[str, Any]:
    """增量 entity_graph 提交事件。

    content 里包含 {entity_graph, changes, sat, sat_format}：
    - entity_graph: 提交方的完整图（含全部 uuid 节点，便于接收端重建对齐）
    - changes: 本次相对 base_version 的增量变更（ADD/REMOVE/MODIFY）
    - sat: 兜底的 SAT 文本（便于接收端在 ACIS 层面做几何重建）

    接收端应基于 changes 做增量合并（ADD/REMOVE/MODIFY），
    而不是用 sat 整体替换本地 ACIS 模型——这正是"以服务端最新一次
    提交的完整图"为基准的协作语义。
    """
    content = dict(version.content or {})
    event = {
        "type": "entity_graph_saved",
        "project_id": project_id,
        "version": version.version,
        "author": version.author,
        "created_at": version.created_at.isoformat(),
        "trigger": trigger,
        "content": content,
    }
    if request_id:
        event["request_id"] = request_id
    if source_client_id:
        event["source_client_id"] = source_client_id
    return event


async def _create_model_version_serialized(
    project_id: str,
    payload: schemas.ModelVersionCreate,
    *,
    trigger: str,
    request_id: str | None = None,
    source_client_id: str | None = None,
):
    async with sync_manager.write_lock(project_id):
        version = crud.create_model_version(project_id, payload)
        # 智能分发：如果这次提交的内容里包含 entity_graph 增量信息，必须按 entity_graph_saved
        # 事件广播给所有接收端，让它们走"ADD/REMOVE/MODIFY 合并"路径；否则一律按 model_saved
        # 广播会污染版本序列，接收端只能"清空+restore"，丢失对齐信息。
        content = dict(version.content or {})
        has_entity_graph = isinstance(content, dict) and "entity_graph" in content
        broadcast_event = _entity_graph_saved_event(
            project_id,
            version,
            trigger=trigger,
            request_id=request_id,
            source_client_id=source_client_id,
        ) if has_entity_graph else _model_saved_event(
            project_id,
            version,
            trigger=trigger,
            request_id=request_id,
            source_client_id=source_client_id,
            include_content=False,
        )
        await sync_manager.broadcast(
            project_id,
            broadcast_event,
            exclude_client_id=source_client_id,
        )
    return version


@app.post("/projects/{project_id}/models", response_model=schemas.SaveResult, status_code=201)
async def save_model(
    project_id: str,
    payload: schemas.ModelVersionCreate,
    _: None = Depends(verify_api_password),
) -> schemas.SaveResult:
    version = await _create_model_version_serialized(project_id, payload, trigger="http_save")
    return schemas.SaveResult(version=version.version, created_at=version.created_at)


async def _send_latest_model_saved_event(project_id: str, websocket: WebSocket, trigger: str) -> None:
    try:
        latest = crud.get_latest_version_or_404(project_id)
    except HTTPException as ex:
        if ex.status_code == status.HTTP_404_NOT_FOUND:
            return
        raise

    content = dict(latest.content or {})
    # 如果最新版本是用 entity_graph 路径写入的，必须按 entity_graph_saved 事件广播，
    # 否则接收端会按 model_saved 走"清空+restore"路径，丢掉已有的 entity_graph 节点对齐逻辑。
    if isinstance(content, dict) and "entity_graph" in content:
        await websocket.send_json(
            _entity_graph_saved_event(
                project_id,
                latest,
                trigger=trigger,
            )
        )
        return

    await websocket.send_json(_model_saved_event(project_id, latest, trigger=trigger, include_content=True))


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


@app.get("/projects/{project_id}/meta")
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


async def _send_submit_rejected(
    websocket: WebSocket,
    project_id: str,
    *,
    request_id: str | None,
    reason: str,
    detail: str,
) -> None:
    event: dict[str, Any] = {
        "type": "submit_rejected",
        "project_id": project_id,
        "reason": reason,
        "detail": detail,
    }
    if request_id:
        event["request_id"] = request_id

    try:
        latest = crud.get_latest_version_or_404(project_id)
    except HTTPException:
        latest = None

    if latest is not None:
        event.update(
            {
                "latest_version": latest.version,
                "author": latest.author,
                "created_at": latest.created_at.isoformat(),
                "content": latest.content,
            }
        )

    await websocket.send_json(event)


async def _handle_submit_model_message(
    websocket: WebSocket,
    project_id: str,
    client_id: str,
    author: str,
    data: dict[str, Any],
) -> None:
    request_id = str(data.get("request_id") or uuid4().hex)
    message_project_id = str(data.get("project_id") or project_id)
    if message_project_id != project_id:
        await _send_submit_rejected(
            websocket,
            project_id,
            request_id=request_id,
            reason="invalid_project",
            detail="submit_model project_id does not match the WebSocket project",
        )
        return

    payload_author = str(data.get("author") or author).strip() or author
    payload = {
        "author": payload_author,
        "content": data.get("content"),
        "base_version": data.get("base_version"),
    }
    try:
        model_payload = schemas.ModelVersionCreate.model_validate(payload)
    except ValidationError as ex:
        await _send_submit_rejected(
            websocket,
            project_id,
            request_id=request_id,
            reason="invalid_payload",
            detail=str(ex),
        )
        return

    trigger = str(data.get("reason") or "submit_model").strip() or "submit_model"
    try:
        await _create_model_version_serialized(
            project_id,
            model_payload,
            trigger=trigger,
            request_id=request_id,
            source_client_id=client_id,
        )
    except HTTPException as ex:
        await _send_submit_rejected(
            websocket,
            project_id,
            request_id=request_id,
            reason="conflict" if ex.status_code == status.HTTP_409_CONFLICT else "save_failed",
            detail=str(ex.detail),
        )
        return

    await websocket.send_json(
        {
            "type": "submit_accepted",
            "project_id": project_id,
            "request_id": request_id,
            "version_model": "model",
        }
    )


async def _handle_submit_entity_graph_message(
    websocket: WebSocket,
    project_id: str,
    client_id: str,
    author: str,
    data: dict[str, Any],
) -> None:
    """处理 client 通过 WebSocket 发来的增量 entity_graph 提交。

    与 submit_model 走的是同一条持久化路径（create_model_version），
    只是后续广播走 entity_graph_saved 事件，让其他 client 用增量
    changes 合并而不是 clear+restore 整体替换。
    """
    request_id = str(data.get("request_id") or uuid4().hex)
    message_project_id = str(data.get("project_id") or project_id)
    if message_project_id != project_id:
        await _send_submit_rejected(
            websocket,
            project_id,
            request_id=request_id,
            reason="invalid_project",
            detail="submit_entity_graph project_id does not match the WebSocket project",
        )
        return

    payload_author = str(data.get("author") or author).strip() or author
    payload = {
        "author": payload_author,
        "content": data.get("content"),
        "base_version": data.get("base_version"),
    }
    try:
        model_payload = schemas.ModelVersionCreate.model_validate(payload)
    except ValidationError as ex:
        await _send_submit_rejected(
            websocket,
            project_id,
            request_id=request_id,
            reason="invalid_payload",
            detail=str(ex),
        )
        return

    trigger = str(data.get("reason") or "submit_entity_graph").strip() or "submit_entity_graph"
    print(f"[fastapi _handle_submit_entity_graph] ENTER project_id={project_id} request_id={request_id} base_version_in={model_payload.base_version}", flush=True)
    try:
        async with sync_manager.write_lock(project_id):
            version = crud.create_model_version(project_id, model_payload)
            print(f"[fastapi _handle_submit_entity_graph] CREATED v={version.version} project_id={project_id} request_id={request_id}", flush=True)
            await sync_manager.broadcast(
                project_id,
                _entity_graph_saved_event(
                    project_id,
                    version,
                    trigger=trigger,
                    request_id=request_id,
                    source_client_id=client_id,
                ),
                exclude_client_id=client_id,
            )
            print(f"[fastapi _handle_submit_entity_graph] BROADCAST entity_graph_saved v={version.version} project_id={project_id} (exclude {client_id})", flush=True)
    except HTTPException as ex:
        print(f"[fastapi _handle_submit_entity_graph] HTTPException status={ex.status_code} detail={ex.detail} project_id={project_id} request_id={request_id}", flush=True)
        await _send_submit_rejected(
            websocket,
            project_id,
            request_id=request_id,
            reason="conflict" if ex.status_code == status.HTTP_409_CONFLICT else "save_failed",
            detail=str(ex.detail),
        )
        return

    print(f"[fastapi _handle_submit_entity_graph] SENDING submit_accepted v={version.version} to client_id={client_id}", flush=True)
    await websocket.send_json(
        {
            "type": "submit_accepted",
            "project_id": project_id,
            "request_id": request_id,
            "version_model": "entity_graph",
            "version": version.version,
        }
    )


async def _handle_project_ws_message(
    websocket: WebSocket,
    project_id: str,
    client_id: str,
    author: str,
    message: str,
) -> None:
    normalized = message.strip()
    lowered = normalized.lower()
    if lowered == "sync_now":
        await _send_latest_model_saved_event(project_id, websocket, trigger="sync_now")
        return
    if lowered == "ping":
        await websocket.send_json({"type": "pong", "project_id": project_id})
        return

    try:
        data = json.loads(normalized)
    except json.JSONDecodeError:
        await websocket.send_json({"type": "error", "project_id": project_id, "detail": "Unsupported WebSocket message"})
        return

    if not isinstance(data, dict):
        await websocket.send_json({"type": "error", "project_id": project_id, "detail": "WebSocket JSON message must be an object"})
        return

    message_type = str(data.get("type") or "").strip()
    if message_type == "submit_model":
        await _handle_submit_model_message(websocket, project_id, client_id, author, data)
    elif message_type == "submit_entity_graph":
        await _handle_submit_entity_graph_message(websocket, project_id, client_id, author, data)
    elif message_type == "sync_now":
        await _send_latest_model_saved_event(project_id, websocket, trigger="sync_now")
    elif message_type == "ping":
        await websocket.send_json({"type": "pong", "project_id": project_id})
    else:
        await websocket.send_json({"type": "error", "project_id": project_id, "detail": f"Unsupported WebSocket message type: {message_type}"})


@app.websocket("/ws/projects/{project_id}")
async def ws_project_sync(websocket: WebSocket, project_id: str) -> None:
    if settings.api_password and websocket.query_params.get("password") != settings.api_password:
        await websocket.close(code=1008)
        return

    client_id = websocket.query_params.get("client_id") or uuid4().hex
    author = (websocket.query_params.get("author") or "anonymous").strip() or "anonymous"

    await sync_manager.connect(project_id, websocket, client_id=client_id, author=author)
    try:
        members = await sync_manager.list_members(project_id)
        await websocket.send_json(
            {
                "type": "presence_snapshot",
                "project_id": project_id,
                "self": {"client_id": client_id, "author": author},
                "members": members,
            }
        )
        await sync_manager.broadcast(
            project_id,
            {
                "type": "collaborator_joined",
                "project_id": project_id,
                "client_id": client_id,
                "author": author,
            },
            exclude_client_id=client_id,
        )

        await _send_latest_model_saved_event(project_id, websocket, trigger="snapshot")
        while True:
            message = await websocket.receive_text()
            await _handle_project_ws_message(websocket, project_id, client_id, author, message)
    except WebSocketDisconnect:
        disconnected = await sync_manager.disconnect(project_id, websocket)
        if disconnected is not None:
            await sync_manager.broadcast(
                project_id,
                {
                    "type": "collaborator_left",
                    "project_id": project_id,
                    "client_id": disconnected["client_id"],
                    "author": disconnected["author"],
                },
            )


# ---------------------------------------------------------------------------
# Entity Graph API — stores ACIS entity graphs instead of SAT text
# ---------------------------------------------------------------------------

@app.post("/projects/{project_id}/entities", status_code=201)
async def save_entity_version(
    project_id: str,
    payload: schemas.EntityVersionCreate,
    _: None = Depends(verify_api_password),
) -> dict[str, Any]:
    """
    Save an entity graph version for a project.

    Request body:
    {
        "author": "<name>",
        "entity_graph": {"nodes": [...], "rels": [...]},
        "base_version": <int|null>
    }
    """
    from . import neo4j_entity_store as _nes
    entity_store = _nes.get_entity_store()
    record = entity_store.create_entity_version(
        project_id=project_id,
        author=payload.author,
        entity_graph=payload.entity_graph,
        base_version=payload.base_version,
    )
    return {
        "version": record.version,
        "created_at": record.created_at.isoformat(),
        "node_count": len(record.entity_graph.nodes),
        "rel_count": len(record.entity_graph.rels),
    }


@app.get("/projects/{project_id}/entities/{version}")
async def get_entity_version(
    project_id: str,
    version: int,
    _: None = Depends(verify_api_password),
) -> dict[str, Any]:
    """
    Get a specific entity graph version.
    Returns the full graph as JSON for rendering/diffing on the client.
    """
    from . import neo4j_entity_store as _nes
    entity_store = _nes.get_entity_store()
    result = entity_store.get_entity_version_as_json(project_id, version)
    if result is None:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Entity version not found")
    return result


@app.get("/projects/{project_id}/entities/versions")
async def list_entity_versions(
    project_id: str,
    limit: int = Query(default=50, ge=1, le=200),
    offset: int = Query(default=0, ge=0),
    _: None = Depends(verify_api_password),
) -> list[dict[str, Any]]:
    """List entity graph versions (metadata only, no full graph)."""
    from . import neo4j_entity_store as _nes
    entity_store = _nes.get_entity_store()
    records = entity_store.list_entity_versions(project_id, limit=limit, offset=offset)
    return [
        {
            "project_id": r.project_id,
            "version": r.version,
            "author": r.author,
            "created_at": r.created_at.isoformat(),
        }
        for r in records
    ]


@app.get("/projects/{project_id}/entities/diff")
async def diff_entity_versions(
    project_id: str,
    version_a: int = Query(..., ge=1, alias="a"),
    version_b: int = Query(..., ge=1, alias="b"),
    _: None = Depends(verify_api_password),
) -> dict[str, Any]:
    """
    Compute the graph diff between two entity graph versions.

    Returns:
    {
        "added_nodes": [...],
        "removed_nodes": [...],
        "modified_nodes": [{"old": {...}, "new": {...}}],
        "added_rels": [...],
        "removed_rels": [...]
    }
    """
    from . import neo4j_entity_store as _nes
    entity_store = _nes.get_entity_store()
    diff = entity_store.diff_entity_versions(project_id, version_a, version_b)
    return {
        "version_a": version_a,
        "version_b": version_b,
        "added_nodes": [
            {"id": n.local_id, "labels": n.labels, "props": n.props}
            for n in diff.added_nodes
        ],
        "removed_nodes": [
            {"id": n.local_id, "labels": n.labels, "props": n.props}
            for n in diff.removed_nodes
        ],
        "modified_nodes": [
            {
                "old": {"id": old.local_id, "labels": old.labels, "props": old.props},
                "new": {"id": new_node.local_id, "labels": new_node.labels, "props": new_node.props},
            }
            for old, new_node in diff.modified_nodes
        ],
        "added_rels": [
            {"type": r.rel_type, "start": r.start_id, "end": r.end_id, "props": r.props}
            for r in diff.added_rels
        ],
        "removed_rels": [
            {"type": r.rel_type, "start": r.start_id, "end": r.end_id, "props": r.props}
            for r in diff.removed_rels
        ],
    }
