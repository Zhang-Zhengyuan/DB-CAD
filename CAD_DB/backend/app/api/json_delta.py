"""Mode1 JSON Push/Pull. Version and hash semantics are protocol contracts."""

from typing import Any

from fastapi import APIRouter, Depends, HTTPException, Query, Request, status

from .. import schemas
from ..config import settings
from ..sync import sync_manager
from ..json_collab import (
    GraphValidationError,
    graph_component,
    graph_hash,
    merge_graph,
    validate_graph,
)
from ..json_graph_store import get_json_graph_store


from ..dependencies import verify_api_password

router = APIRouter()


@router.post("/projects/{project_id}/json-delta", status_code=201)
async def save_json_delta(
    project_id: str,
    payload: schemas.JsonDeltaRequest,
    request: Request,
    _: None = Depends(verify_api_password),
) -> dict[str, Any]:
    """Mode1 pure JSON path.

    The client serializes ACIS locally.  FastAPI validates the graph, loads the
    base snapshot, applies an entity/component delta, and persists one complete
    canonical snapshot.  No C++ bridge or SAT is involved.
    """
    if not settings.direct_json_mode:
        raise HTTPException(
            status_code=503, detail="Direct JSON collaboration is disabled"
        )
    request_json = (await request.body()).decode("utf-8")
    try:
        incoming = validate_graph(payload.entity_graph)
    except GraphValidationError as ex:
        raise HTTPException(status_code=422, detail=str(ex)) from ex
    incoming_ids = {
        str(node.get("id", "")).strip() for node in incoming.get("nodes", [])
    }
    changed_ids = {
        str(item.get("uuid", item.get("id", ""))).strip()
        for item in payload.changes
        if isinstance(item, dict) and str(item.get("uuid", item.get("id", ""))).strip()
    }
    changed_ids.update(
        str(item).strip()
        for item in payload.changes
        if isinstance(item, str) and str(item).strip()
    )
    removed_ids = {
        str(value).strip() for value in payload.removed_ids if str(value).strip()
    }
    missing_changed_ids = sorted(changed_ids - removed_ids - incoming_ids)
    if missing_changed_ids:
        # A changed body must carry its complete topology component. Without
        # this guard, a stale UUID/position mapping could be accepted as an
        # empty component and a peer would incorrectly delete its canvas.
        raise HTTPException(
            status_code=422,
            detail={"reason": "changed_component_missing", "ids": missing_changed_ids},
        )

    async with sync_manager.write_lock(project_id):
        store = get_json_graph_store()
        # JSON graph versions have their own namespace.  A SAT/legacy version
        # must not silently become the base of a pure JSON graph stream.
        latest = store.latest(project_id)
        latest_version = latest.version if latest else None
        base_version = payload.base_version
        # The JSON stream has its own version namespace. A client joining an
        # existing SAT project must Pull or start from an empty JSON version.
        if latest_version is None:
            if base_version not in (None, 0):
                raise HTTPException(
                    status_code=409,
                    detail={"reason": "stale_base", "latest_version": 0},
                )
            base_version = None
        elif base_version is None:
            raise HTTPException(
                status_code=status.HTTP_409_CONFLICT,
                detail={
                    "reason": "base_version_required",
                    "latest_version": latest_version,
                },
            )
        if base_version != latest_version:
            raise HTTPException(
                status_code=status.HTTP_409_CONFLICT,
                detail={
                    "reason": "stale_base",
                    "latest_version": latest_version or 0,
                    "base_version": base_version,
                },
            )
        base_graph = (
            store.graph(project_id, latest.version)
            if latest
            else {"nodes": [], "rels": []}
        )
        try:
            merged = merge_graph(
                base_graph, incoming, payload.changes, payload.removed_ids
            )
        except GraphValidationError as ex:
            raise HTTPException(status_code=422, detail=str(ex)) from ex
        content = {
            "entity_graph": merged,
            "changes": payload.changes,
            "removed_ids": payload.removed_ids,
            "schema": "dbcad.entity_graph.v1",
            "graph_hash": graph_hash(merged),
            "_audit": {
                "payload_format": "json",
                "contains_sat": False,
                "request_bytes": len(request_json.encode("utf-8")),
                "request_json": request_json,
            },
        }
        version = store.create(project_id, payload.author, content, base_version)
        await sync_manager.broadcast(
            project_id,
            {
                "type": "json_delta_saved",
                "project_id": project_id,
                "version": version.version,
                "author": version.author,
                "changed_ids": [
                    str(item.get("uuid", item.get("id"))).strip()
                    for item in payload.changes
                    if isinstance(item, dict)
                    and str(item.get("uuid", item.get("id", ""))).strip()
                ],
                "removed_ids": payload.removed_ids,
                "graph_hash": content["graph_hash"],
                "trigger": "json_delta",
            },
            exclude_client_id=payload.source_client_id,
            stream="mode1",
        )
        return {
            "project_id": project_id,
            "version": version.version,
            "created_at": version.created_at.isoformat(),
            "graph_hash": content["graph_hash"],
        }


@router.get("/projects/{project_id}/json-delta")
def get_json_delta(
    project_id: str,
    base_version: int = Query(..., ge=0),
    _: None = Depends(verify_api_password),
) -> dict[str, Any]:
    """Return changed body components since the requested JSON version."""
    if not settings.direct_json_mode:
        raise HTTPException(
            status_code=503, detail="Direct JSON collaboration is disabled"
        )
    store = get_json_graph_store()
    latest = store.latest(project_id)
    if latest is None:
        return {
            "version": 0,
            "entity_graph": {"nodes": [], "rels": []},
            "changes": None,
        }
    if base_version > latest.version:
        raise HTTPException(
            status_code=409, detail="base_version is ahead of latest version"
        )
    base = store.get(project_id, base_version) if base_version > 0 else None
    if base_version > 0 and base is None:
        raise HTTPException(status_code=404, detail="Base JSON graph version not found")
    graph = (
        store.graph(project_id, latest.version)
        if hasattr(store, "graph")
        else latest.content.get("entity_graph", {"nodes": [], "rels": []})
    )
    commits = store.list_after(project_id, base_version)
    latest_changes = [
        item
        for commit in commits
        for item in commit.content.get("changes", [])
        if isinstance(item, (dict, str))
    ]
    changed_roots = {
        str(item.get("uuid", item.get("id"))).strip()
        for item in latest_changes
        if isinstance(item, dict) and str(item.get("uuid", item.get("id", ""))).strip()
    }
    changed_roots.update(
        item.strip()
        for item in latest_changes
        if isinstance(item, str) and item.strip()
    )
    removed_ids = [
        str(value).strip()
        for commit in commits
        for value in commit.content.get("removed_ids", [])
        if str(value).strip()
    ]
    # A body deleted and subsequently re-added in the requested interval is
    # present in the latest graph. Do not delete it again on the client.
    current_node_ids = {node["id"] for node in graph["nodes"]}
    removed_ids = sorted(
        {value for value in removed_ids if value not in current_node_ids}
    )
    # A Pull carries only complete topology components for bodies changed by
    # this commit. The client can merge these components into a dirty canvas.
    # It must never interpret the absence of an unchanged body as a deletion.
    if base_version == 0:
        delta_graph = graph
    elif changed_roots:
        delta_graph = graph_component(graph, changed_roots)
    else:
        delta_graph = {
            "nodes": [],
            "rels": [],
            **{
                key: value
                for key, value in graph.items()
                if key not in {"nodes", "rels"}
            },
        }
    return {
        "version": latest.version,
        "entity_graph": delta_graph,
        "changes": latest_changes,
        "removed_ids": removed_ids,
        "delta": base_version != 0,
        "schema": latest.content.get("schema", "dbcad.entity_graph.v1"),
        "graph_hash": latest.content.get("graph_hash"),
    }
