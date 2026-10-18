"""/api/v1 命令端点：P2 命令引擎占位（V3_PLAN.md §2）。

P1 冻结契约占位：路径/语义已在计划表定稿，实现随 P2（CommandBus 服务端化
+ rebase 语义）交付。501 而非 404，让前端可区分"协议已知、引擎未上"。
"""

from fastapi import APIRouter, Depends, HTTPException, status

from ...dependencies import verify_api_password

router = APIRouter(dependencies=[Depends(verify_api_password)])

_NOT_IMPLEMENTED = "command engine lands in P2 (V3_PLAN.md); import/tag/scene are available now"


@router.get("/schema/commands")
def command_schemas() -> dict:
    return {"schema": "dbcad.commands/1", "commands": []}


@router.post("/projects/{project_id}/commands", status_code=status.HTTP_501_NOT_IMPLEMENTED)
def execute_command(project_id: str) -> dict:
    raise HTTPException(status.HTTP_501_NOT_IMPLEMENTED, _NOT_IMPLEMENTED)


@router.post(
    "/projects/{project_id}/commands/dry-run", status_code=status.HTTP_501_NOT_IMPLEMENTED
)
def dry_run_command(project_id: str) -> dict:
    raise HTTPException(status.HTTP_501_NOT_IMPLEMENTED, _NOT_IMPLEMENTED)


@router.post("/projects/{project_id}/undo", status_code=status.HTTP_501_NOT_IMPLEMENTED)
def undo(project_id: str) -> dict:
    raise HTTPException(status.HTTP_501_NOT_IMPLEMENTED, _NOT_IMPLEMENTED)


@router.post("/projects/{project_id}/redo", status_code=status.HTTP_501_NOT_IMPLEMENTED)
def redo(project_id: str) -> dict:
    raise HTTPException(status.HTTP_501_NOT_IMPLEMENTED, _NOT_IMPLEMENTED)
