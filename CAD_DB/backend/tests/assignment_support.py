from __future__ import annotations

from dataclasses import dataclass
from datetime import datetime, timezone
from uuid import uuid4

from fastapi import HTTPException, status
from pytest import MonkeyPatch

from app import crud
from app.storage_bridge import VersionRecord


@dataclass
class ProjectRecord:
    id: str
    name: str
    created_at: datetime
    updated_at: datetime


class MemoryStorage:
    def __init__(self) -> None:
        self.projects: dict[str, ProjectRecord] = {}
        self.project_names: dict[str, str] = {}
        self.versions: dict[str, list[VersionRecord]] = {}
        self.next_version_id = 1

    def healthcheck(self) -> None:
        return None

    def close(self) -> None:
        return None

    def create_project(self, name: str) -> ProjectRecord:
        now = datetime.now(timezone.utc)
        project = ProjectRecord(id=uuid4().hex, name=name, created_at=now, updated_at=now)
        self.projects[project.id] = project
        self.project_names[name] = project.id
        self.versions[project.id] = []
        return project

    def get_project_or_none(self, project_id: str) -> ProjectRecord | None:
        return self.projects.get(project_id)

    def get_project_by_name_or_none(self, project_name: str) -> ProjectRecord | None:
        project_id = self.project_names.get(project_name)
        return self.projects.get(project_id) if project_id is not None else None

    def create_model_version(
        self,
        project_id: str,
        author: str,
        content: dict,
        base_version: int | None,
    ) -> VersionRecord:
        rows = self.versions.setdefault(project_id, [])
        latest = rows[-1].version if rows else None
        if latest is not None and base_version is None:
            raise HTTPException(
                status_code=status.HTTP_409_CONFLICT,
                detail=f"base_version is required: latest version is {latest}",
            )
        if base_version is not None and base_version != latest:
            raise HTTPException(
                status_code=status.HTTP_409_CONFLICT,
                detail=f"Version conflict: latest version is {latest or 0}",
            )

        now = datetime.now(timezone.utc)
        version = VersionRecord(
            id=self.next_version_id,
            project_id=project_id,
            version=1 if latest is None else latest + 1,
            author=author,
            content=dict(content),
            created_at=now,
        )
        self.next_version_id += 1
        rows.append(version)
        return version

    def get_latest_version_or_none(self, project_id: str) -> VersionRecord | None:
        rows = self.versions.get(project_id, [])
        return rows[-1] if rows else None

    def get_version_or_none(self, project_id: str, version: int) -> VersionRecord | None:
        for row in self.versions.get(project_id, []):
            if row.version == version:
                return row
        return None

    def list_versions(self, project_id: str, limit: int, offset: int) -> list[VersionRecord]:
        rows = list(reversed(self.versions.get(project_id, [])))
        return rows[offset : offset + limit]


def install_memory_storage(monkeypatch: MonkeyPatch) -> MemoryStorage:
    storage = MemoryStorage()
    monkeypatch.setattr(crud, "storage_bridge", storage)
    return storage
