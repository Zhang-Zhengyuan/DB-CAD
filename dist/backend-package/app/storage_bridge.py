from __future__ import annotations

from dataclasses import dataclass
from datetime import datetime
from typing import Any
from urllib.parse import quote

import httpx
from fastapi import HTTPException, status


@dataclass
class ProjectRecord:
    id: str
    name: str
    created_at: datetime
    updated_at: datetime


@dataclass
class VersionRecord:
    id: int
    project_id: str
    version: int
    author: str
    content: dict[str, Any]
    created_at: datetime


class StorageBridgeClient:
    def __init__(self, base_url: str, timeout_seconds: float) -> None:
        trimmed = base_url.strip().rstrip("/")
        if not trimmed:
            raise RuntimeError("CAD_DB_STORAGE_BRIDGE_URL is required when storage_backend=neo4j")

        self._client = httpx.Client(base_url=trimmed, timeout=timeout_seconds)

    def close(self) -> None:
        self._client.close()

    @staticmethod
    def _parse_datetime(value: str) -> datetime:
        return datetime.fromisoformat(value.replace("Z", "+00:00"))

    @staticmethod
    def _parse_project(data: dict[str, Any]) -> ProjectRecord:
        return ProjectRecord(
            id=str(data["id"]),
            name=str(data["name"]),
            created_at=StorageBridgeClient._parse_datetime(str(data["created_at"])),
            updated_at=StorageBridgeClient._parse_datetime(str(data["updated_at"])),
        )

    @staticmethod
    def _parse_version(data: dict[str, Any]) -> VersionRecord:
        return VersionRecord(
            id=int(data["id"]),
            project_id=str(data["project_id"]),
            version=int(data["version"]),
            author=str(data["author"]),
            content=dict(data["content"]),
            created_at=StorageBridgeClient._parse_datetime(str(data["created_at"])),
        )

    @staticmethod
    def _extract_detail(response: httpx.Response) -> str:
        try:
            body = response.json()
            if isinstance(body, dict) and "detail" in body:
                return str(body["detail"])
        except Exception:
            pass
        return response.text or response.reason_phrase

    def _request(self, method: str, path: str, **kwargs: Any) -> httpx.Response:
        try:
            response = self._client.request(method, path, **kwargs)
        except httpx.RequestError as ex:
            raise HTTPException(
                status_code=status.HTTP_503_SERVICE_UNAVAILABLE,
                detail=f"Storage bridge unavailable: {ex}",
            ) from ex

        if response.status_code >= 400:
            raise HTTPException(status_code=response.status_code, detail=self._extract_detail(response))

        return response

    def healthcheck(self) -> None:
        self._request("GET", "/health")

    def create_project(self, name: str) -> ProjectRecord:
        response = self._request("POST", "/projects", json={"name": name})
        return self._parse_project(response.json())

    def get_project_or_none(self, project_id: str) -> ProjectRecord | None:
        encoded = quote(project_id, safe="")
        try:
            response = self._request("GET", f"/projects/{encoded}")
        except HTTPException as ex:
            if ex.status_code == status.HTTP_404_NOT_FOUND:
                return None
            raise
        return self._parse_project(response.json())

    def get_project_by_name_or_none(self, project_name: str) -> ProjectRecord | None:
        encoded = quote(project_name, safe="")
        try:
            response = self._request("GET", f"/projects/by-name/{encoded}")
        except HTTPException as ex:
            if ex.status_code == status.HTTP_404_NOT_FOUND:
                return None
            raise
        return self._parse_project(response.json())

    def create_model_version(
        self,
        project_id: str,
        author: str,
        content: dict[str, Any],
        base_version: int | None,
    ) -> VersionRecord:
        encoded = quote(project_id, safe="")
        response = self._request(
            "POST",
            f"/projects/{encoded}/models",
            json={
                "author": author,
                "content": content,
                "base_version": base_version,
            },
        )
        data = response.json()

        if "content" not in data:
            latest = self.get_version_or_none(project_id, int(data["version"]))
            if latest is None:
                raise HTTPException(
                    status_code=status.HTTP_502_BAD_GATEWAY,
                    detail="Storage bridge returned incomplete save response",
                )
            return latest

        return self._parse_version(data)

    def get_latest_version_or_none(self, project_id: str) -> VersionRecord | None:
        encoded = quote(project_id, safe="")
        try:
            response = self._request("GET", f"/projects/{encoded}/models/latest")
        except HTTPException as ex:
            if ex.status_code == status.HTTP_404_NOT_FOUND:
                return None
            raise
        return self._parse_version(response.json())

    def get_version_or_none(self, project_id: str, version: int) -> VersionRecord | None:
        encoded = quote(project_id, safe="")
        try:
            response = self._request("GET", f"/projects/{encoded}/models/{version}")
        except HTTPException as ex:
            if ex.status_code == status.HTTP_404_NOT_FOUND:
                return None
            raise
        return self._parse_version(response.json())

    def list_versions(self, project_id: str, limit: int, offset: int) -> list[VersionRecord]:
        encoded = quote(project_id, safe="")
        response = self._request(
            "GET",
            f"/projects/{encoded}/models/versions",
            params={"limit": limit, "offset": offset},
        )
        rows = response.json()
        if not isinstance(rows, list):
            raise HTTPException(
                status_code=status.HTTP_502_BAD_GATEWAY,
                detail="Storage bridge returned invalid versions list",
            )
        return [self._parse_version(item) for item in rows]
