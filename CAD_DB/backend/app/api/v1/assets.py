"""/api/v1/assets/{hash}.{sat|glb}：不可变组件资产，内容寻址强缓存（V3_PLAN.md §2）。"""

from fastapi import APIRouter, Depends, HTTPException, Response, status

from ...component_store import KIND_MEDIA_TYPES, get_component_store
from ...dependencies import verify_api_password

router = APIRouter(dependencies=[Depends(verify_api_password)])


@router.get("/assets/{asset}")
def get_asset(asset: str) -> Response:
    digest, _, kind = asset.rpartition(".")
    payload = get_component_store().read(kind, digest) if digest and kind else None
    if payload is None:
        raise HTTPException(status.HTTP_404_NOT_FOUND, "Asset not found")
    return Response(
        content=payload,
        media_type=KIND_MEDIA_TYPES.get(kind, "application/octet-stream"),
        headers={
            # 内容寻址 ⇒ 永不复用 URL 指向不同字节：可给最长强缓存。
            "Cache-Control": "public, max-age=31536000, immutable",
            "ETag": f'"{digest}"',
        },
    )
