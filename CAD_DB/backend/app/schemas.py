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


# ---------------------------------------------------------------------------
# Mode1 Delta Push / Pull schemas
# ---------------------------------------------------------------------------

class SaveDeltaRequest(BaseModel):
    """Mode1 Delta Push 请求体"""
    author: str = Field(min_length=1, max_length=120)
    base_version: int | None = Field(default=None, ge=0)
    delta_uuids: list[str] = Field(default_factory=list)
    delta_sat_segments: list[str] = Field(default_factory=list)
    removed_uuids: list[str] = Field(default_factory=list)
    # 【Phase B】提交方 client_id，用于 broadcast 时排除自身。
    # 客户端从 WebSocket 连接时拿到 server 分配的 client_id，保存到本地后再在 push 时带回。
    # 允许为空（向后兼容旧客户端）。
    source_client_id: str | None = Field(default=None)


class DeltaBodyItem(BaseModel):
    """Delta 中的单个 body 条目"""
    uuid: str
    sat: str


class GetDeltaResponse(BaseModel):
    """Mode1 Delta Pull 响应体"""
    version: int
    delta_bodies: list[DeltaBodyItem] = Field(default_factory=list)
    deleted_uuids: list[str] = Field(default_factory=list)
