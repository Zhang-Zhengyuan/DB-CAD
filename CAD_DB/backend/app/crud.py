"""Project catalog. Version persistence is owned exclusively by JsonGraphStore."""

from typing import Any
from fastapi import HTTPException, status
from . import schemas

direct_store: Any | None = None


def _backend():
    if direct_store is None:
        raise HTTPException(
            status_code=503, detail="Storage backend is not initialized"
        )
    return direct_store


def initialize_backend():
    global direct_store
    from .neo4j_store import Neo4jStore

    direct_store = Neo4jStore()
    direct_store.initialize()


def shutdown_backend():
    global direct_store
    if direct_store is not None:
        direct_store.close()
        direct_store = None


def create_project(payload: schemas.ProjectCreate):
    backend = _backend()
    project = backend.get_project_by_name_or_none(payload.name)
    if project is not None:
        raise HTTPException(status_code=409, detail="Project name already exists")
    return backend.create_project(payload.name)


def get_project_or_404(project_id: str):
    backend = _backend()
    project = backend.get_project_or_none(project_id)
    if project is None:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND, detail="Project not found"
        )
    return project


def get_project_by_name_or_404(project_name: str):
    backend = _backend()
    project = backend.get_project_by_name_or_none(project_name)
    if project is None:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND, detail="Project not found"
        )
    return project
