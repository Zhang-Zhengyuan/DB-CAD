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


class JsonTagRequest(BaseModel):
    """M4.3 显式定版：命名标签指向某一版本。"""

    name: str = Field(min_length=1, max_length=120)
    version: int = Field(ge=1)


class JsonDeltaRequest(BaseModel):
    """Client serialized graph delta for the bridge-free Mode1 path."""

    author: str = Field(min_length=1, max_length=120)
    entity_graph: dict[str, Any]
    changes: list[dict[str, Any] | str] = Field(default_factory=list)
    removed_ids: list[str] = Field(default_factory=list)
    base_version: int | None = Field(default=None, ge=0)
    source_client_id: str | None = Field(default=None, max_length=200)
