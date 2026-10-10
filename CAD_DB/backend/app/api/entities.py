"""Entity graph storage endpoints retained for existing clients."""

from typing import Any

from fastapi import APIRouter, Depends, HTTPException, Query, status

from .. import schemas


from ..dependencies import verify_api_password

router = APIRouter()


@router.post("/projects/{project_id}/entities", status_code=201)
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
    from .. import neo4j_entity_store as _nes

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


@router.get("/projects/{project_id}/entities/versions")
async def list_entity_versions(
    project_id: str,
    limit: int = Query(default=50, ge=1, le=200),
    offset: int = Query(default=0, ge=0),
    _: None = Depends(verify_api_password),
) -> list[dict[str, Any]]:
    """List entity graph versions (metadata only, no full graph)."""
    from .. import neo4j_entity_store as _nes

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


@router.get("/projects/{project_id}/entities/diff")
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
    from .. import neo4j_entity_store as _nes

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
                "new": {
                    "id": new_node.local_id,
                    "labels": new_node.labels,
                    "props": new_node.props,
                },
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


@router.get("/projects/{project_id}/entities/{version}")
async def get_entity_version(
    project_id: str,
    version: int,
    _: None = Depends(verify_api_password),
) -> dict[str, Any]:
    """
    Get a specific entity graph version.
    Returns the full graph as JSON for rendering/diffing on the client.
    """
    from .. import neo4j_entity_store as _nes

    entity_store = _nes.get_entity_store()
    result = entity_store.get_entity_version_as_json(project_id, version)
    if result is None:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND, detail="Entity version not found"
        )
    return result
