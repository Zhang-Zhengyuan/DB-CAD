"""/api/v1/files：SAT 导入建项目 / 版本级导出（V3_PLAN.md §2）。

导入流水线（P1 核心 + P6 投影）：上传 SAT → C++ worker（MeshExtractor +
GlbWriter + 实体图 codec）→ 组件（SAT 源 + GLB + graph）内容寻址入库 →
版本 1 提交 → 实体投影写入 → WS 广播。组件不可变，同几何跨项目/跨版本
自动去重。
"""

import json

from fastapi import APIRouter, Depends, File, Form, HTTPException, Response, UploadFile, status

from ...component_store import get_component_store
from ...dependencies import verify_api_password
from ...entity_projection import write_projections
from ...events_hub import hub
from ...v1_store import SceneComponent, get_v1_store
from ...worker_client import WorkerError, get_worker_gateway
from .projects import _project_or_404
from .versions import _version_or_404

router = APIRouter(dependencies=[Depends(verify_api_password)])


def _store_components(components) -> tuple[list[SceneComponent], list[tuple[str, dict | None, None]]]:
    """入库 SAT/GLB/graph 三资产；返回 (场景组件, (hash, graph, parent) 投影三元组)。
    导入组件无 generation 父（parent=None）。"""
    store = get_component_store()
    scene: list[SceneComponent] = []
    projection_pairs: list[tuple[str, dict | None, None]] = []
    for index, part in enumerate(components):
        hash_sat = store.put("sat", part.sat)
        graph_hash = ""
        if part.graph is not None:
            from ...entity_projection import normalize_entity_projection

            normalized = normalize_entity_projection(hash_sat, part.graph)
            graph_hash = store.put(
                "graph", json.dumps(normalized, ensure_ascii=False).encode("utf-8")
            )
        scene.append(
            SceneComponent(
                hash_sat=hash_sat,
                hash_glb=store.put("glb", part.glb),
                name=f"component-{index}",
                color=part.color,
                bbox_min=part.bbox_min,
                bbox_max=part.bbox_max,
                triangles=part.triangles,
                vertices=part.vertices,
                graph=graph_hash,
            )
        )
        projection_pairs.append((hash_sat, part.graph, None))
    return scene, projection_pairs


@router.post("/files/import", status_code=status.HTTP_201_CREATED)
async def import_sat(
    file: UploadFile = File(...),
    project_name: str | None = Form(default=None),
    project_id: str | None = Form(default=None),
    author: str = Form(default="importer"),
) -> dict:
    if bool(project_name) == bool(project_id):
        raise HTTPException(
            status.HTTP_422_UNPROCESSABLE_ENTITY,
            "Provide exactly one of project_name (new project) or project_id (append version)",
        )
    payload = await file.read()
    if not payload:
        raise HTTPException(status.HTTP_422_UNPROCESSABLE_ENTITY, "Empty SAT upload")

    store = get_v1_store()
    if project_id is not None:
        _project_or_404(project_id)
    else:
        project_id = store.create_project(project_name).id  # type: ignore[arg-type]

    try:
        imported = get_worker_gateway().import_sat(payload)
    except WorkerError as exc:
        raise HTTPException(status.HTTP_502_BAD_GATEWAY, f"geometry worker failed: {exc}") from exc

    components, projection_pairs = _store_components(imported.components)
    version = store.commit_version(
        project_id,
        message=f"import {file.filename or 'upload.sat'}",
        author=author,
        components=components,
    )
    projections = write_projections(store, projection_pairs)
    hub.publish(
        project_id,
        "version.committed",
        {"version": version.number, "message": version.message, "author": author},
    )
    return {
        "project_id": project_id,
        "version": version.number,
        "components": [c.to_scene_json() for c in version.components],
        "entity_projection": projections,
    }


@router.get("/projects/{project_id}/files/export")
def export_project(project_id: str, v: int | None = None, fmt: str = "sat") -> Response:
    """导出：sat=组件合并回整体（全保真）；glb=整体单网格；stl 留 P5。"""
    _project_or_404(project_id)
    version = (
        _version_or_404(project_id, v)
        if v is not None
        else get_v1_store().latest(project_id)
    )
    if version is None:
        raise HTTPException(status.HTTP_404_NOT_FOUND, "Project has no versions")
    if fmt not in ("sat", "glb", "stl"):
        raise HTTPException(status.HTTP_422_UNPROCESSABLE_ENTITY, "fmt must be sat|glb|stl")

    assets = get_component_store()
    parts = [assets.read("sat", c.hash_sat) for c in version.components]
    if any(part is None for part in parts):
        raise HTTPException(status.HTTP_500_INTERNAL_SERVER_ERROR, "Component asset missing")
    try:
        gateway = get_worker_gateway()
        if fmt == "sat":
            payload = gateway.merge_sat(parts)  # type: ignore[arg-type]
        elif fmt == "stl":
            payload = gateway.export_stl(parts)  # type: ignore[arg-type]
        else:
            payload = gateway.export_glb(parts)  # type: ignore[arg-type]
    except WorkerError as exc:
        raise HTTPException(status.HTTP_502_BAD_GATEWAY, f"geometry worker failed: {exc}") from exc

    media = {"sat": "application/octet-stream", "glb": "model/gltf-binary", "stl": "model/stl"}[fmt]
    name = f"{project_id}-v{version.number}.{fmt}"
    return Response(
        content=payload,
        media_type=media,
        headers={"Content-Disposition": f'attachment; filename="{name}"'},
    )
