"""Application assembly. Run with uvicorn app.main:app from backend/."""

import logging
from datetime import datetime, timezone
from fastapi import FastAPI, HTTPException, Request
from fastapi.responses import JSONResponse, RedirectResponse
from fastapi.staticfiles import StaticFiles

from .config import settings
from .lifecycle import LOG_FILE_PATH, lifespan
from . import neo4j_webui
from .api import v1

logger = logging.getLogger(__name__)
app = FastAPI(title=settings.app_name, version="0.1.0", lifespan=lifespan)
app.mount(
    "/webui-static", StaticFiles(directory=neo4j_webui.STATIC_ROOT), name="webui-static"
)
# v3 Web 客户端（web/ 构建产物；开发期用 vite dev server + 代理）。
from pathlib import Path as _Path  # noqa: E402

_WEB_DIST = _Path(__file__).resolve().parents[3] / "web" / "dist"
if _WEB_DIST.exists():
    app.mount("/web", StaticFiles(directory=str(_WEB_DIST), html=True), name="web")


@app.middleware("http")
async def web_entry_no_cache(request: Request, call_next):
    # 入口 HTML 禁缓存：构建产物是内容哈希命名（可长缓存），但 index.html
    # 换构建后旧引用的哈希资源已被清空——缓存的旧 index 会白屏。
    response = await call_next(request)
    if request.url.path in ("/web", "/web/") or request.url.path.endswith("/index.html"):
        response.headers["Cache-Control"] = "no-cache"
    return response
app.include_router(neo4j_webui.router)
app.add_route("/webui", neo4j_webui.serve_webui, ["GET"])
# v3 起 / 直达 Web 客户端；旧管理页仍在 /webui（P7 后 V1 模型浏览）。
if _WEB_DIST.exists():
    app.add_route("/", lambda: RedirectResponse("/web/"), ["GET"])
else:  # 无前端构建时保留管理页入口，避免裸 404。
    app.add_route("/", neo4j_webui.serve_webui, ["GET"])


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
            logger.error(
                "HTTP %s %s -> %s",
                request.method,
                request.url.path,
                response.status_code,
            )
        return response
    except Exception as ex:
        logger.exception(
            "Unhandled exception on %s %s: %s", request.method, request.url.path, ex
        )
        raise


@app.get("/health")
def health() -> dict[str, str | bool]:
    return {
        "status": "ok",
        "build": datetime.now(timezone.utc).strftime("%Y%m%d-%H%M%S"),
        "log_file": str(LOG_FILE_PATH),
        "collaboration_mode": "v3_server_authoritative",
        "bridge_required": False,
    }


# v1（V3_PLAN.md §2）：服务端权威协议，唯一线协议（P7 起旧端点已物理删除）。
app.include_router(v1.router)
