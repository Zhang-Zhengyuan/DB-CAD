"""Admin endpoints for the administrator dashboard."""

import re
from typing import Any
from fastapi import APIRouter, HTTPException, Query
from fastapi.responses import JSONResponse
from pydantic import BaseModel, Field
from ..config import settings
from .support import _get_driver, _run_cypher

router = APIRouter()


class CypherRequest(BaseModel):
    query: str
    params: dict[str, Any] = Field(default_factory=dict)


@router.post("/query")
def execute_cypher(req: CypherRequest) -> dict[str, Any]:
    """Run a bounded, read-only Cypher query for administrators."""
    q = req.query.strip()
    if not re.match(r"^(MATCH|OPTIONAL\s+MATCH)\b", q, re.IGNORECASE):
        raise HTTPException(status_code=400, detail="Only MATCH queries are allowed")
    if re.search(
        r"\b(CREATE|DELETE|REMOVE|SET|MERGE|DROP|DETACH|CALL|LOAD|FOREACH|USE|ALTER|GRANT|DENY|REVOKE|START|STOP)\b",
        q,
        re.IGNORECASE,
    ):
        raise HTTPException(
            status_code=400, detail="Write or procedure clauses are not allowed"
        )
    if (
        ";" in q
        or "//" in q
        or "/*" in q
        or not re.search(r"\bRETURN\b", q, re.IGNORECASE)
    ):
        raise HTTPException(
            status_code=400, detail="One MATCH/RETURN query is required"
        )
    try:
        from neo4j import Query

        driver = _get_driver()
        with driver.session(
            database=settings.neo4j_database, default_access_mode="READ"
        ) as session:
            rows = [
                dict(record)
                for record in session.run(
                    Query(q, timeout=5.0), req.params or {}
                ).fetch(200)
            ]
        return {"rows": rows, "count": len(rows)}
    except Exception as ex:
        return JSONResponse(
            status_code=400,
            content={"error": str(ex), "detail": "Cypher execution failed."},
        )


@router.post("/clear")
def clear_database(confirm: bool = Query(default=False)) -> dict[str, str]:
    """清空所有节点和关系。dangerous！"""
    if not confirm:
        raise HTTPException(
            status_code=400,
            detail="Must set confirm=true to actually clear the database.",
        )
    try:
        _run_cypher("MATCH (n) DETACH DELETE n")
        return {
            "status": "ok",
            "message": "All nodes and relationships have been deleted.",
        }
    except Exception as ex:
        raise HTTPException(status_code=500, detail=str(ex)) from ex


@router.delete("/projects/{project_name}")
def delete_project(
    project_name: str, confirm: bool = Query(default=False)
) -> dict[str, str]:
    """Delete one project and its version-scoped JSON and historical records."""
    if not confirm:
        raise HTTPException(
            status_code=400,
            detail="Must set confirm=true to actually delete the project.",
        )
    try:
        info = _run_cypher(
            """
            MATCH (p)
            WHERE labels(p)[0] IN ['BridgeProject', 'DBCADProject', 'Project']
              AND p.name = $name
            WITH p, p.id AS pid
            OPTIONAL MATCH (p)-[:HAS_VERSION]->(v)
            OPTIONAL MATCH (p)-[:HAS_EG_VERSION]->(egv)
            OPTIONAL MATCH (p)-[:HAS_JSON_GRAPH_VERSION]->(jv:JsonGraphVersion)
            OPTIONAL MATCH (jv)-[:HAS_COMPONENT]->(jc:JsonGraphComponent)
            OPTIONAL MATCH (jc)-[:HAS_JSON_ENTITY]->(je:JsonGraphEntity)
            OPTIONAL MATCH (jv)-[:HAS_RELATION]->(jr:JsonGraphRelation)
            OPTIONAL MATCH (d)
              WHERE (d.project_id = pid)
                AND labels(d)[0] IN ['BridgeDeltaVersion', 'DBCADDelta']
            WITH p, collect(DISTINCT v) AS vs, collect(DISTINCT egv) AS egvs,
                 collect(DISTINCT jv) AS jvs, collect(DISTINCT jc) AS jcs,
                 collect(DISTINCT je) AS jes, collect(DISTINCT jr) AS jrs,
                 collect(DISTINCT d) AS ds
            FOREACH (x IN jes | DETACH DELETE x)
            FOREACH (x IN jrs | DETACH DELETE x)
            FOREACH (x IN jcs | DETACH DELETE x)
            FOREACH (x IN jvs | DETACH DELETE x)
            FOREACH (x IN vs | DETACH DELETE x)
            FOREACH (x IN egvs | DETACH DELETE x)
            FOREACH (x IN ds | DETACH DELETE x)
            WITH p, p.name AS deleted
            DETACH DELETE p
            RETURN deleted
            """,
            {"name": project_name},
        )
        if not info:
            raise HTTPException(
                status_code=404, detail=f"Project '{project_name}' not found"
            )
        return {
            "status": "ok",
            "message": f"Project '{project_name}' and related data deleted.",
        }
    except HTTPException:
        raise
    except Exception as ex:
        raise HTTPException(status_code=500, detail=str(ex)) from ex
