"""Dashboard router assembly and HTML entry point."""

from pathlib import Path
from fastapi import APIRouter, Depends
from fastapi.responses import HTMLResponse
from . import overview, projects, versions, graph, admin
from .support import _verify_admin_password, _close_driver as _close_driver

STATIC_ROOT = Path(__file__).resolve().parent.parent / "static" / "webui"
router = APIRouter(
    prefix="/api/neo4j",
    tags=["Neo4j 管理"],
    dependencies=[Depends(_verify_admin_password)],
)
for module in (overview, projects, versions, graph, admin):
    router.include_router(module.router)


def serve_webui(request) -> HTMLResponse:
    """Keep / and /webui available; assets are served by the application."""
    return HTMLResponse(STATIC_ROOT.joinpath("index.html").read_text(encoding="utf-8"))
