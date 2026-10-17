"""Read historical snapshots from the same version chain used by Push/Pull."""

from typing import Any
from fastapi import APIRouter, Depends, HTTPException, Query
from ..dependencies import verify_api_password
from ..json_graph_store import get_json_graph_store

router = APIRouter()


def _read(project_id: str, version) -> dict[str, Any]:
    if version is None:
        raise HTTPException(status_code=404, detail="Model version not found")
    store = get_json_graph_store()
    return {
        "project_id": project_id,
        "version": version.version,
        "author": version.author,
        "created_at": version.created_at.isoformat(),
        "content": {
            **version.content,
            "entity_graph": store.graph(project_id, version.version),
        },
    }


@router.get("/projects/{project_id}/models/latest")
def get_latest_model(project_id: str, _: None = Depends(verify_api_password)):
    return _read(project_id, get_json_graph_store().latest(project_id))


@router.get("/projects/{project_id}/models/versions")
def get_versions(
    project_id: str,
    limit: int = Query(50, ge=1, le=200),
    offset: int = Query(0, ge=0),
    _: None = Depends(verify_api_password),
):
    store = get_json_graph_store()
    rows = list(reversed(store.list_after(project_id, 0)))[offset : offset + limit]
    return [
        {
            "project_id": project_id,
            "version": v.version,
            "author": v.author,
            "created_at": v.created_at.isoformat(),
        }
        for v in rows
    ]


@router.get("/projects/{project_id}/models/{version}")
def get_model_by_version(
    project_id: str, version: int, _: None = Depends(verify_api_password)
):
    return _read(project_id, get_json_graph_store().get(project_id, version))
