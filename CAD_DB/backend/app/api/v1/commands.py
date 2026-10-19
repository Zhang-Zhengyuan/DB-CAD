"""/api/v1 命令端点（V3_PLAN.md §2/P2：服务端权威命令执行引擎）。

命令全序即版本链（ADR-2）：POST /commands 在 head 几何上执行 → 产出新版本
（变更组件内容寻址入库）→ WS 广播。rebase 语义：base_version 过期但命令
引用的组件未被 (base, head] 区间变更 → 自动前移执行；引用组件已变更 →
409（几何级冲突诚实拒绝）。undo = 提交父版本清单的新版本（树状撤销，
Onshape 同款）；redo 弹出撤销栈。
"""

from fastapi import APIRouter, Body, Depends, HTTPException, status

from ...dependencies import verify_api_password
from ...entity_projection import write_projections
from ...events_hub import hub
from ...v1_store import SceneComponent, get_v1_store
from ...worker_client import WorkerError as _BatchWorkerError
from ...worker_pool import WorkerError, get_worker_pool
from .projects import _project_or_404

router = APIRouter(dependencies=[Depends(verify_api_password)])

_KINDS = [
    "cube", "sphere", "cylinder", "torus", "cone", "prism", "pyramid", "wiggle",
    "plane", "pplane", "cplane", "spheres", "pspheres", "conic_side", "cylinder_side",
    "pconic", "toruss", "ptoruss", "laws",
    "line", "arc", "ellipse", "conic", "helix", "spiral", "spring", "lawc",
    "bspline_ctrlpts", "bspline_fit", "bspline_intrp", "bezier", "bspline",
]

COMMAND_SCHEMAS: dict[str, dict] = {
    "component.create": {
        "description": "参数化创建图元（体/面/线 33 种）",
        "params": {
            "kind": {"type": "string", "enum": _KINDS, "required": True},
            "position": {"type": "array<number>[3]"},
            "width": {"type": "number"}, "height": {"type": "number"},
            "depth": {"type": "number"}, "radius": {"type": "number"},
            "major_radius": {"type": "number"}, "minor_radius": {"type": "number"},
            "sides": {"type": "integer"},
        },
    },
    "component.translate": {
        "description": "平移组件（烘焙几何）",
        "params": {
            "component": {"type": "string", "required": True},
            "offset": {"type": "array<number>[3]", "required": True},
        },
    },
    "component.rotate": {
        "description": "绕轴旋转组件",
        "params": {
            "component": {"type": "string", "required": True},
            "axis": {"type": "array<number>[3]", "required": True},
            "angle_degrees": {"type": "number", "required": True},
        },
    },
    "component.scale": {
        "description": "均匀缩放组件",
        "params": {
            "component": {"type": "string", "required": True},
            "factor": {"type": "number", "required": True},
        },
    },
    "component.delete": {
        "description": "删除组件",
        "params": {"component": {"type": "string", "required": True}},
    },
    "boolean.unite": {
        "description": "布尔并（单目标+工具，输出挂目标槽位）",
        "params": {
            "targets": {"type": "array<string>[1]", "required": True},
            "tool": {"type": "string", "required": True},
        },
    },
    "boolean.subtract": {
        "description": "布尔差",
        "params": {
            "targets": {"type": "array<string>[1]", "required": True},
            "tool": {"type": "string", "required": True},
        },
    },
    "boolean.intersect": {
        "description": "布尔交",
        "params": {
            "targets": {"type": "array<string>[1]", "required": True},
            "tool": {"type": "string", "required": True},
        },
    },
    "intersector.intersect": {
        "description": "求交器（面/线组合，产出新组件，输入保留）",
        "params": {
            "a": {"type": "string", "required": True},
            "b": {"type": "string", "required": True},
        },
    },
    "defeature.unrepeat": {
        "description": "去重（成对减法空体检测）",
        "params": {"targets": {"type": "array<string>", "required": True}},
    },
}


def _referenced_components(action: str, params: dict) -> set[str]:
    """命令几何引用的组件 sid（rebase 冲突检测用）。"""
    referenced: set[str] = set()
    if action.startswith("component.") and action != "component.create":
        if isinstance(params.get("component"), str):
            referenced.add(params["component"])
    elif action.startswith("boolean."):
        referenced.update(sid for sid in params.get("targets", []) if isinstance(sid, str))
        if isinstance(params.get("tool"), str):
            referenced.add(params["tool"])
    elif action == "intersector.intersect":
        for key in ("a", "b"):
            if isinstance(params.get(key), str):
                referenced.add(params[key])
    elif action == "defeature.unrepeat":
        referenced.update(sid for sid in params.get("targets", []) if isinstance(sid, str))
    return referenced


def _validate(action: str, params: dict) -> None:
    schema = COMMAND_SCHEMAS.get(action)
    if schema is None:
        raise HTTPException(
            status.HTTP_422_UNPROCESSABLE_ENTITY,
            "unknown action; see /api/v1/schema/commands",
        )
    missing = [
        name
        for name, spec in schema["params"].items()
        if spec.get("required") and name not in params
    ]
    if missing:
        raise HTTPException(
            status.HTTP_422_UNPROCESSABLE_ENTITY, f"missing required params: {missing}"
        )


@router.get("/schema/commands")
def command_schemas() -> dict:
    return {"schema": "dbcad.commands/1", "commands": COMMAND_SCHEMAS}


@router.post("/projects/{project_id}/commands/dry-run")
def dry_run_command(project_id: str, payload: dict = Body(...)) -> dict:
    _project_or_404(project_id)
    action = str(payload.get("action", ""))
    params = payload.get("params", {})
    if not isinstance(params, dict):
        raise HTTPException(status.HTTP_422_UNPROCESSABLE_ENTITY, "params must be an object")
    _validate(action, params)
    referenced = _referenced_components(action, params)
    known = {c.hash_sat for c in (get_v1_store().latest(project_id).components if get_v1_store().latest(project_id) else [])}
    unknown = referenced - known
    if unknown:
        raise HTTPException(
            status.HTTP_422_UNPROCESSABLE_ENTITY,
            f"unknown component references: {sorted(unknown)}",
        )
    return {"valid": True, "action": action, "affects": sorted(referenced)}


@router.post("/projects/{project_id}/commands")
async def execute_command(project_id: str, payload: dict = Body(...)) -> dict:
    _project_or_404(project_id)
    action = str(payload.get("action", ""))
    params = payload.get("params", {})
    if not isinstance(params, dict):
        raise HTTPException(status.HTTP_422_UNPROCESSABLE_ENTITY, "params must be an object")
    _validate(action, params)
    author = str(payload.get("author", "command"))
    base_version = payload.get("base_version")

    store = get_v1_store()
    head = store.latest(project_id)
    head_number = head.number if head else None
    head_components = head.components if head else []

    referenced = _referenced_components(action, params)
    head_set = {c.hash_sat for c in head_components}

    # rebase 判定先于存在性检查：组件 sid 是内容哈希——被 (base, head] 区间
    # 修改/删除的组件 sid 会从 head 消失，不能误报 422。
    rebased = False
    if base_version is not None and base_version != head_number:
        base_row = store.get_version(project_id, base_version)
        if base_row is None:
            raise HTTPException(
                status.HTTP_422_UNPROCESSABLE_ENTITY, f"unknown base_version {base_version}"
            )
        base_set = {c.hash_sat for c in base_row.components}
        gone = referenced & (base_set - head_set)  # base 时存在、head 已变更/删除
        if gone:
            # P8/Phase D：冲突细化到实体级——列出被冲突组件在 head 投影中的
            # 实体键（截断），前端可显示"alice 改了这些 face"。
            conflicting_entities = [
                e["entity_key"]
                for e in store.query_entities(sorted(gone), label=None, limit=20)
                if e.get("entity_key")
            ]
            raise HTTPException(
                status.HTTP_409_CONFLICT,
                {
                    "reason": "stale_base_version",
                    "current_head": head_number,
                    "conflicting_components": sorted(gone),
                    "conflicting_entities": conflicting_entities,
                },
            )
        rebased = True  # 引用未被变更：自动前移到 head 执行

    if referenced:
        unknown = referenced - head_set
        if unknown:
            raise HTTPException(
                status.HTTP_422_UNPROCESSABLE_ENTITY,
                f"unknown component references: {sorted(unknown)}",
            )

    try:
        result = await get_worker_pool().execute(
            project_id, head_number or 0, head_components, {"action": action, "params": params}
        )
    except (WorkerError, _BatchWorkerError) as exc:
        raise HTTPException(
            status.HTTP_502_BAD_GATEWAY, f"geometry worker failed: {exc}"
        ) from exc

    removed = {sid for sid in result.get("removed", []) if isinstance(sid, str)}
    manifest: list[SceneComponent] = [
        c for c in head_components if c.hash_sat not in removed
    ]
    previous = {c.hash_sat: c for c in head_components}
    added: list[SceneComponent] = []
    for entry in result.get("modified", []):
        target = previous.get(str(entry.get("sid", "")))
        if target is None or "sat_bytes" not in entry:
            raise HTTPException(
                status.HTTP_502_BAD_GATEWAY, "worker reported an unknown modified component"
            )
        replacement = SceneComponent(
            hash_sat=entry["sat_bytes"],
            hash_glb=entry.get("glb_bytes", target.hash_glb),
            name=target.name,
            color=target.color,
            bbox_min=[float(v) for v in entry.get("bbox", {}).get("min", target.bbox_min)],
            bbox_max=[float(v) for v in entry.get("bbox", {}).get("max", target.bbox_max)],
            triangles=int(entry.get("triangles", target.triangles)),
            vertices=int(entry.get("vertices", target.vertices)),
            graph=entry.get("graph_bytes", target.graph),
        )
        manifest[manifest.index(target)] = replacement
    for entry in result.get("added", []):
        if "sat_bytes" not in entry:
            raise HTTPException(status.HTTP_502_BAD_GATEWAY, "worker emitted no component bytes")
        bbox = entry.get("bbox", {})
        component = SceneComponent(
            hash_sat=entry["sat_bytes"],
            hash_glb=entry.get("glb_bytes", ""),
            name=str(entry.get("kind") or "component"),
            color=[float(v) for v in entry.get("color", [0.8, 0.8, 0.8])],
            bbox_min=[float(v) for v in bbox.get("min", [0.0, 0.0, 0.0])],
            bbox_max=[float(v) for v in bbox.get("max", [0.0, 0.0, 0.0])],
            triangles=int(entry.get("triangles", 0)),
            vertices=int(entry.get("vertices", 0)),
            graph=entry.get("graph_bytes", ""),
        )
        manifest.append(component)
        added.append(component)

    version = store.commit_version(
        project_id,
        message=action,
        author=author,
        components=manifest,
        base_version=head_number,
    )
    # 实体级投影（P6/ADR-3 + P8/Phase D）：变更组件入库后写投影（幂等，
    # 未变更组件零写入）；modified 组件带命令前哈希作 generation 父
    # （worker 会话内组件 sid 即其 SAT 哈希），实体内容寻址共享 +
    # DERIVED_FROM 链；added 组件无父。
    previous_by_sid = {c.hash_sat: c.hash_sat for c in head_components}
    projection_pairs = [
        (
            entry.get("sat_bytes"),
            entry.get("graph_json"),
            previous_by_sid.get(str(entry.get("sid", ""))),
        )
        for entry in result.get("modified", [])
    ]
    projection_pairs += [
        (entry.get("sat_bytes"), entry.get("graph_json"), None)
        for entry in result.get("added", [])
    ]
    projections = write_projections(store, projection_pairs)
    hub.publish(
        project_id,
        "version.committed",
        {
            "version": version.number,
            "message": action,
            "author": author,
            "rebased": rebased,
            "added": len(added),
            "modified": len(result.get("modified", [])),
            "removed": len(removed),
        },
    )
    return {
        "project_id": project_id,
        "version": version.number,
        "base_version": head_number,
        "rebased": rebased,
        "added": [c.to_scene_json() for c in added],
        "removed": sorted(removed),
        "components": [c.to_scene_json() for c in version.components],
        "entity_projection": projections,
    }


def _manifest_only_commit(project_id: str, message: str, author: str, components: list[SceneComponent]):
    store = get_v1_store()
    head = store.latest(project_id)
    version = store.commit_version(
        project_id,
        message=message,
        author=author,
        components=components,
        base_version=head.number if head else None,
    )
    hub.publish(
        project_id,
        "version.committed",
        {"version": version.number, "message": message, "author": author},
    )
    return version


@router.post("/projects/{project_id}/undo")
def undo(project_id: str, author: str = "undo") -> dict:
    """树状撤销：提交父版本清单为新版本（几何不变，清单回退）。"""
    _project_or_404(project_id)
    store = get_v1_store()
    head = store.latest(project_id)
    if head is None or head.base_version is None:
        raise HTTPException(status.HTTP_409_CONFLICT, "nothing to undo")
    parent = store.get_version(project_id, head.base_version)
    if parent is None:
        raise HTTPException(status.HTTP_409_CONFLICT, "undo target version missing")
    store.push_redo(project_id, head.components)
    version = _manifest_only_commit(project_id, f"undo -> v{parent.number}", author, parent.components)
    return {"project_id": project_id, "version": version.number, "undone": head.number}


@router.post("/projects/{project_id}/redo")
def redo(project_id: str, author: str = "redo") -> dict:
    _project_or_404(project_id)
    store = get_v1_store()
    components = store.pop_redo(project_id)
    if components is None:
        raise HTTPException(status.HTTP_409_CONFLICT, "nothing to redo")
    version = _manifest_only_commit(project_id, "redo", author, components)
    return {"project_id": project_id, "version": version.number}

