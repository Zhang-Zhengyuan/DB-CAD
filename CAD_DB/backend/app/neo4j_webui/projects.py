"""Projects endpoints for the administrator dashboard (V1 model)."""

from typing import Any
from fastapi import APIRouter, HTTPException
from pydantic import BaseModel
from .support import _run_cypher, _run_single_cypher, _iso

router = APIRouter()


class ProjectSummary(BaseModel):
    name: str
    project_id: str
    created_at: str
    updated_at: str
    version_count: int
    latest_version: int | None
    head_component_count: int


@router.get("/projects", response_model=list[ProjectSummary])
def list_projects() -> list[ProjectSummary]:
    """V1 项目列表：版本计数 + head 版本组件数。"""
    rows = _run_cypher(
        """
        MATCH (p:V1Project)
        OPTIONAL MATCH (v:V1Version {project_id: p.id})
        WITH p, count(v) AS version_count, max(v.number) AS latest
        OPTIONAL MATCH (h:V1Version {project_id: p.id, number: latest})
        OPTIONAL MATCH (h)-[:CONTAINS]->(c:V1Component)
        RETURN p.name AS name, p.id AS project_id, p.created_at AS created_at,
               COALESCE(p.updated_at, p.created_at) AS updated_at,
               version_count, latest, count(DISTINCT c) AS head_components
        ORDER BY updated_at DESC
        LIMIT 500
        """
    )
    return [
        ProjectSummary(
            name=str(r.get("name") or ""),
            project_id=str(r.get("project_id") or ""),
            created_at=_iso(r.get("created_at")),
            updated_at=_iso(r.get("updated_at")),
            version_count=int(r.get("version_count") or 0),
            latest_version=int(r["latest"]) if r.get("latest") is not None else None,
            head_component_count=int(r.get("head_components") or 0),
        )
        for r in rows
    ]


@router.get("/projects/{project_name}")
def get_project_detail(project_name: str) -> dict[str, Any]:
    """项目详情：V1 版本链（组件数、实体投影数、相对父版本的变更/复用）。"""
    project_row = _run_single_cypher(
        "MATCH (p:V1Project {name: $name}) RETURN p",
        {"name": project_name},
    )
    if not project_row:
        raise HTTPException(status_code=404, detail=f"Project '{project_name}' not found")
    p = dict(project_row["p"])
    pid = str(p.get("id") or "")

    version_rows = _run_cypher(
        """
        MATCH (v:V1Version {project_id: $pid})
        OPTIONAL MATCH (v)-[:CONTAINS]->(c:V1Component)
        OPTIONAL MATCH (c)-[:HAS_ENTITY]->(e:V1Entity)
        RETURN v.number AS number, v.message AS message, v.author AS author,
               v.created_at AS created_at, v.base_version AS base_version,
               collect(DISTINCT c.hash_sat) AS component_hashes,
               count(DISTINCT e) AS entity_count
        ORDER BY number ASC
        """,
        {"pid": pid},
    )
    manifests = {
        int(row.get("number") or 0): {str(h) for h in (row.get("component_hashes") or []) if h}
        for row in version_rows
    }
    versions: list[dict[str, Any]] = []
    for row in version_rows:
        number = int(row.get("number") or 0)
        hashes = manifests.get(number, set())
        base = row.get("base_version")
        parent_hashes = manifests.get(int(base), set()) if base is not None else set()
        versions.append(
            {
                "version": number,
                "message": str(row.get("message") or ""),
                "author": str(row.get("author") or ""),
                "created_at": _iso(row.get("created_at")),
                "base_version": base,
                "component_count": len(hashes),
                "changed_component_count": len(hashes - parent_hashes),
                "reused_component_count": len(hashes & parent_hashes),
                "entity_count": int(row.get("entity_count") or 0),
            }
        )

    return {
        "project": {
            "name": str(p.get("name") or ""),
            "id": pid,
            "created_at": _iso(p.get("created_at")),
            "updated_at": _iso(p.get("updated_at") or p.get("created_at")),
        },
        "versions": versions,
        "head_version": versions[-1]["version"] if versions else None,
    }
