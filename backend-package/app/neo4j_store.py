from __future__ import annotations

from dataclasses import dataclass
from datetime import UTC, datetime
import json
from typing import Any
from uuid import uuid4

from fastapi import HTTPException, status
from neo4j import Driver, GraphDatabase

from .config import settings


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


class Neo4jStore:
    def __init__(self) -> None:
        if not settings.neo4j_uri:
            raise RuntimeError("CAD_DB_NEO4J_URI is required when storage_backend=neo4j")

        self._driver: Driver = GraphDatabase.driver(
            settings.neo4j_uri,
            auth=(settings.neo4j_user, settings.neo4j_password),
        )
        self._database = settings.neo4j_database

    def close(self) -> None:
        self._driver.close()

    def initialize(self) -> None:
        with self._driver.session(database=self._database) as session:
            session.run("CREATE CONSTRAINT project_id_unique IF NOT EXISTS FOR (p:Project) REQUIRE p.id IS UNIQUE")
            session.run("CREATE CONSTRAINT project_name_unique IF NOT EXISTS FOR (p:Project) REQUIRE p.name IS UNIQUE")
            session.run(
                "CREATE CONSTRAINT model_version_unique IF NOT EXISTS FOR (m:ModelVersion) REQUIRE (m.project_id, m.version) IS UNIQUE"
            )

    @staticmethod
    def _now_iso() -> str:
        return datetime.now(UTC).isoformat()

    @staticmethod
    def _parse_time(value: str) -> datetime:
        return datetime.fromisoformat(value.replace("Z", "+00:00"))

    @staticmethod
    def _parse_time_or_now(value: Any) -> datetime:
        if isinstance(value, str) and value:
            return Neo4jStore._parse_time(value)
        return datetime.now(UTC)

    @staticmethod
    def _parse_content(raw_value: Any) -> dict[str, Any]:
        if isinstance(raw_value, dict):
            return raw_value
        if isinstance(raw_value, str) and raw_value:
            decoded = json.loads(raw_value)
            if isinstance(decoded, dict):
                return decoded
        return {}

    def _project_from_node(self, node: Any) -> ProjectRecord:
        props = dict(node)
        return ProjectRecord(
            id=str(props.get("id", "")),
            name=str(props.get("name", "")),
            created_at=self._parse_time_or_now(props.get("created_at")),
            updated_at=self._parse_time_or_now(props.get("updated_at")),
        )

    def _version_from_node(self, node: Any) -> VersionRecord:
        props = dict(node)
        return VersionRecord(
            id=int(node.id),
            project_id=str(props.get("project_id", "")),
            version=int(props.get("version", 0)),
            author=str(props.get("author", "")),
            content=self._parse_content(props.get("content_json", props.get("content"))),
            created_at=self._parse_time_or_now(props.get("created_at")),
        )

    def create_project(self, name: str) -> ProjectRecord:
        now = self._now_iso()
        project_id = str(uuid4())

        with self._driver.session(database=self._database) as session:
            record = session.run(
                """
                CREATE (p:Project {
                    id: $id,
                    name: $name,
                    created_at: $now,
                    updated_at: $now
                })
                RETURN p AS project
                """,
                id=project_id,
                name=name,
                now=now,
            ).single()

        if record is None:
            raise HTTPException(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR, detail="Failed to create project")

        return self._project_from_node(record["project"])

    def get_project_or_none(self, project_id: str) -> ProjectRecord | None:
        with self._driver.session(database=self._database) as session:
            record = session.run(
                """
                MATCH (p:Project {id: $id})
                RETURN p AS project
                """,
                id=project_id,
            ).single()

        if record is None:
            return None

        return self._project_from_node(record["project"])

    def get_project_by_name_or_none(self, project_name: str) -> ProjectRecord | None:
        with self._driver.session(database=self._database) as session:
            record = session.run(
                """
                MATCH (p:Project {name: $name})
                RETURN p AS project
                """,
                name=project_name,
            ).single()

        if record is None:
            return None

        return self._project_from_node(record["project"])

    def create_model_version(
        self,
        project_id: str,
        author: str,
        content: dict[str, Any],
        base_version: int | None,
    ) -> VersionRecord:
        content_json = json.dumps(content, ensure_ascii=False)
        created_at = self._now_iso()

        def write(tx: Any) -> VersionRecord:
            exists_record = tx.run(
                "MATCH (p:Project {id: $project_id}) RETURN p.id AS id",
                project_id=project_id,
            ).single()
            if exists_record is None:
                raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Project not found")

            latest_record = tx.run(
                """
                OPTIONAL MATCH (p:Project {id: $project_id})-->(m:ModelVersion)
                RETURN max(m.version) AS latest
                """,
                project_id=project_id,
            ).single()

            latest_version = latest_record["latest"] if latest_record else None
            if base_version is not None and base_version != latest_version:
                raise HTTPException(
                    status_code=status.HTTP_409_CONFLICT,
                    detail=f"Version conflict: latest version is {latest_version or 0}",
                )

            next_version = 1 if latest_version is None else int(latest_version) + 1

            record = tx.run(
                """
                MATCH (p:Project {id: $project_id})
                SET p.updated_at = $created_at
                CREATE (m:ModelVersion {
                    project_id: $project_id,
                    version: $version,
                    author: $author,
                    content_json: $content,
                    created_at: $created_at
                })
                CREATE (p)-[:HAS_VERSION]->(m)
                RETURN m AS version
                """,
                project_id=project_id,
                version=next_version,
                author=author,
                content=content_json,
                created_at=created_at,
            ).single()

            return self._version_from_node(record["version"])

        with self._driver.session(database=self._database) as session:
            return session.execute_write(write)

    def get_latest_version_or_none(self, project_id: str) -> VersionRecord | None:
        with self._driver.session(database=self._database) as session:
            record = session.run(
                """
                MATCH (p:Project {id: $project_id})-->(m:ModelVersion)
                RETURN m AS version
                ORDER BY m.version DESC
                LIMIT 1
                """,
                project_id=project_id,
            ).single()

        if record is None:
            return None

        return self._version_from_node(record["version"])

    def get_version_or_none(self, project_id: str, version: int) -> VersionRecord | None:
        with self._driver.session(database=self._database) as session:
            record = session.run(
                """
                MATCH (p:Project {id: $project_id})-->(m:ModelVersion {version: $version})
                RETURN m AS version
                LIMIT 1
                """,
                project_id=project_id,
                version=version,
            ).single()

        if record is None:
            return None

        return self._version_from_node(record["version"])

    def list_versions(self, project_id: str, limit: int, offset: int) -> list[VersionRecord]:
        with self._driver.session(database=self._database) as session:
            result = session.run(
                """
                MATCH (p:Project {id: $project_id})-->(m:ModelVersion)
                RETURN m AS version
                ORDER BY m.version DESC
                SKIP $offset
                LIMIT $limit
                """,
                project_id=project_id,
                offset=offset,
                limit=limit,
            )

            rows = list(result)

        return [self._version_from_node(row["version"]) for row in rows]
