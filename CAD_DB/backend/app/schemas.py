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
