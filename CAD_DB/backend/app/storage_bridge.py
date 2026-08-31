from __future__ import annotations

from dataclasses import dataclass
from datetime import datetime
import logging
from typing import Any
from urllib.parse import quote

import httpx
from fastapi import HTTPException, status


logger = logging.getLogger(__name__)


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

        self._client = httpx.Client(base_url=trimmed, timeout=timeout_seconds, trust_env=False)

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
    def _parse_saved_version_fallback(
        data: dict[str, Any],
        *,
        project_id: str,
        author: str,
        content: dict[str, Any],
    ) -> VersionRecord | None:
        version_value = data.get("version")
        created_at_value = data.get("created_at")
        if version_value is None or created_at_value is None:
            return None

        try:
            version = int(version_value)
            created_at = StorageBridgeClient._parse_datetime(str(created_at_value))
        except Exception:
            return None

        id_value = data.get("id", 0)
        try:
            version_id = int(id_value)
        except Exception:
            version_id = 0

        project_value = str(data.get("project_id") or project_id)
        author_value = str(data.get("author") or author)
        content_value_raw = data.get("content", content)
        content_value = dict(content_value_raw) if isinstance(content_value_raw, dict) else dict(content)

        return VersionRecord(
            id=version_id,
            project_id=project_value,
            version=version,
            author=author_value,
            content=content_value,
            created_at=created_at,
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
            detail = self._extract_detail(response)
            logger.error(
                "Storage bridge request failed: method=%s path=%s status=%s detail=%s",
                method,
                path,
                response.status_code,
                detail,
            )
            raise HTTPException(status_code=response.status_code, detail=detail)

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
        latest = self.get_latest_version_or_none(project_id)
        print(f"[storage_bridge.create_model_version] project_id={project_id} base_version_in={base_version} latest_seen={latest.version if latest else None}", flush=True)
        if base_version is None and latest is not None:
            base_version = latest.version
            print(f"[storage_bridge.create_model_version] project_id={project_id} auto-set base_version={base_version} from latest", flush=True)
        print(f"[storage_bridge.create_model_version] POST /projects/{encoded}/models base_version={base_version} author={author}", flush=True)
        response = self._request(
            "POST",
            f"/projects/{encoded}/models",
            json={
                "author": author,
                "content": content,
                "base_version": base_version,
            },
        )
        print(f"[storage_bridge.create_model_version] POST /projects/{encoded}/models status={response.status_code}", flush=True)
        try:
            data = response.json()
        except Exception as ex:
            body_preview = (response.text or "").strip()
            if len(body_preview) > 300:
                body_preview = body_preview[:300] + "..."
            raise HTTPException(
                status_code=status.HTTP_502_BAD_GATEWAY,
                detail=f"Storage bridge returned non-JSON save response: {ex}; body={body_preview!r}",
            ) from ex

        if not isinstance(data, dict):
            raise HTTPException(
                status_code=status.HTTP_502_BAD_GATEWAY,
                detail="Storage bridge returned invalid save response",
            )

        full = self._parse_saved_version_fallback(data, project_id=project_id, author=author, content=content)
        if full is not None:
            return full

        version_value = data.get("version")
        if isinstance(version_value, int):
            latest = self.get_version_or_none(project_id, version_value)
            if latest is not None:
                return latest

        latest = self.get_latest_version_or_none(project_id)
        if latest is not None:
            return latest

        raise HTTPException(
            status_code=status.HTTP_502_BAD_GATEWAY,
            detail="Storage bridge returned incomplete save response",
        )

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

    # ================================================================================================
    # Mode1 Delta Push / Pull
    # ================================================================================================

    def save_delta(
        self,
        project_id: str,
        author: str,
        base_version: int | None,
        delta_uuids: list[str],
        delta_sat_segments: list[str],
        removed_uuids: list[str],
        source_client_id: str | None = None,
    ) -> dict[str, Any]:
        """
        Mode1 Delta Push: POST /projects/{projectId}/delta
        Delegates to the C++ storage_bridge handleSaveDelta.
        Returns {"version": int, "project_id": str, "author": str, "created_at": str}
        """
        encoded = quote(project_id, safe="")
        print(f"[storage_bridge save_delta] POST /projects/{encoded}/delta "
              f"base_version={base_version} delta_uuids={len(delta_uuids)} "
              f"delta_sat_segments={len(delta_sat_segments)} removed_uuids={len(removed_uuids)}", flush=True)
        # 【Phase B】通过 HTTP header 把提交方 client_id 透传到 bridge。
        # bridge 内部写入 BridgeVersionDelta.source_client_id 节点属性，
        # FastAPI 在 broadcast 时再用 exclude_client_id 排除提交者。
        headers = {}
        if source_client_id:
            headers["X-Source-Client-Id"] = source_client_id
        response = self._request(
            "POST",
            f"/projects/{encoded}/delta",
            json={
                "author": author,
                "base_version": base_version,
                "delta_uuids": delta_uuids,
                "delta_sat_segments": delta_sat_segments,
                "removed_uuids": removed_uuids,
            },
            headers=headers or None,
        )
        print(f"[storage_bridge save_delta] POST /projects/{encoded}/delta status={response.status_code}", flush=True)
        try:
            data = response.json()
        except Exception as ex:
            raise HTTPException(
                status_code=status.HTTP_502_BAD_GATEWAY,
                detail=f"Storage bridge returned non-JSON delta response: {ex}",
            ) from ex
        return dict(data)

    def get_delta(self, project_id: str, base_version: int) -> dict[str, Any]:
        """
        Mode1 Delta Pull: GET /projects/{projectId}/delta?base_version=X
        Returns {"version": int, "delta_bodies": [{"uuid": str, "sat": str}, ...], "deleted_uuids": []}
        """
        encoded = quote(project_id, safe="")
        print(f"[storage_bridge get_delta] GET /projects/{encoded}/delta base_version={base_version}", flush=True)
        response = self._request(
            "GET",
            f"/projects/{encoded}/delta",
            params={"base_version": base_version},
        )
        print(f"[storage_bridge get_delta] GET /projects/{encoded}/delta status={response.status_code}", flush=True)
        try:
            data = response.json()
        except Exception as ex:
            raise HTTPException(
                status_code=status.HTTP_502_BAD_GATEWAY,
                detail=f"Storage bridge returned non-JSON delta response: {ex}",
            ) from ex
        return dict(data)
