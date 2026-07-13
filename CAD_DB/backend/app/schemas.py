from datetime import datetime
from typing import Any

from pydantic import BaseModel, ConfigDict, Field


class ProjectCreate(BaseModel):
    name: str = Field(min_length=1, max_length=200)


class ProjectRead(BaseModel):
    id: str
    name: str
    created_at: datetime
    updated_at: datetime

    model_config = ConfigDict(from_attributes=True)


class ModelVersionCreate(BaseModel):
    author: str = Field(min_length=1, max_length=120)
    content: dict[str, Any]
    base_version: int | None = Field(default=None, ge=1)


class ModelVersionRead(BaseModel):
    id: int
    project_id: str
    version: int
    author: str
    content: dict[str, Any]
    created_at: datetime

    model_config = ConfigDict(from_attributes=True)


class SaveResult(BaseModel):
    version: int
    created_at: datetime


# ---------------------------------------------------------------------------
# Entity Graph schemas — for neo4j entity graph storage (Phase 2+)
# ---------------------------------------------------------------------------

class EntityNodeSchema(BaseModel):
    id: str
    labels: list[str] = Field(default_factory=list)
    props: dict[str, Any] = Field(default_factory=dict)


class EntityRelSchema(BaseModel):
    type: str
    start: str
    end: str
    props: dict[str, Any] = Field(default_factory=dict)


class EntityGraphSchema(BaseModel):
    nodes: list[EntityNodeSchema] = Field(default_factory=list)
    rels: list[EntityRelSchema] = Field(default_factory=list)


class EntityVersionCreate(BaseModel):
    author: str = Field(min_length=1, max_length=120)
    entity_graph: EntityGraphSchema
    base_version: int | None = Field(default=None, ge=1)
