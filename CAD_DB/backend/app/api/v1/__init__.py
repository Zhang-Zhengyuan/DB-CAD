"""API v1（V3_PLAN.md §2 契约）：全新服务端权威协议，取代 json-delta 族端点。

路由分模块聚合；契约冻结由 tests/test_api_v1_contract.py 的路径断言守门。
"""

from fastapi import APIRouter

from . import assets, commands, events, files, projects, versions

router = APIRouter(prefix="/api/v1", tags=["v1"])
router.include_router(projects.router)
router.include_router(versions.router)
router.include_router(assets.router)
router.include_router(files.router)
router.include_router(commands.router)
router.include_router(events.router)
