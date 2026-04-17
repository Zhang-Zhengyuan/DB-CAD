from __future__ import annotations

from dataclasses import dataclass
from datetime import UTC, datetime
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
                RETURN p.id AS id, p.name AS name, p.created_at AS created_at, p.updated_at AS updated_at
                """,
                id=project_id,
                name=name,
                now=now,
            ).single()

        if record is None:
            raise HTTPException(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR, detail="Failed to create project")

        return ProjectRecord(
            id=record["id"],
            name=record["name"],
            created_at=self._parse_time(record["created_at"]),
            updated_at=self._parse_time(record["updated_at"]),
        )

    def get_project_or_none(self, project_id: str) -> ProjectRecord | None:
        with self._driver.session(database=self._database) as session:
            record = session.run(
                """
                MATCH (p:Project {id: $id})
                RETURN p.id AS id, p.name AS name, p.created_at AS created_at, p.updated_at AS updated_at
                """,
                id=project_id,
            ).single()

        if record is None:
            return None

        return ProjectRecord(
            id=record["id"],
            name=record["name"],
            created_at=self._parse_time(record["created_at"]),
            updated_at=self._parse_time(record["updated_at"]),
        )

    def get_project_by_name_or_none(self, project_name: str) -> ProjectRecord | None:
        with self._driver.session(database=self._database) as session:
            record = session.run(
                """
                MATCH (p:Project {name: $name})
                RETURN p.id AS id, p.name AS name, p.created_at AS created_at, p.updated_at AS updated_at
                """,
                name=project_name,
            ).single()

        if record is None:
            return None

        return ProjectRecord(
            id=record["id"],
            name=record["name"],
            created_at=self._parse_time(record["created_at"]),
            updated_at=self._parse_time(record["updated_at"]),
        )

    def create_model_version(
        self,
        project_id: str,
        author: str,
        content: dict[str, Any],
        base_version: int | None,
    ) -> VersionRecord:
        content_json = content
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
                MATCH (p:Project {id: $project_id})-[:HAS_VERSION]->(m:ModelVersion)
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
                    content: $content,
                    created_at: $created_at
                })
                CREATE (p)-[:HAS_VERSION]->(m)
                RETURN id(m) AS id,
                       m.project_id AS project_id,
                       m.version AS version,
                       m.author AS author,
                       m.content AS content,
                       m.created_at AS created_at
                """,
                project_id=project_id,
                version=next_version,
                author=author,
                content=content_json,
                created_at=created_at,
            ).single()

            return VersionRecord(
                id=int(record["id"]),
                project_id=record["project_id"],
                version=int(record["version"]),
                author=record["author"],
                content=record["content"],
                created_at=self._parse_time(record["created_at"]),
            )

        with self._driver.session(database=self._database) as session:
            return session.execute_write(write)

    def get_latest_version_or_none(self, project_id: str) -> VersionRecord | None:
        with self._driver.session(database=self._database) as session:
            record = session.run(
                """
                MATCH (p:Project {id: $project_id})-[:HAS_VERSION]->(m:ModelVersion)
                RETURN id(m) AS id,
                       m.project_id AS project_id,
                       m.version AS version,
                       m.author AS author,
                       m.content AS content,
                       m.created_at AS created_at
                ORDER BY m.version DESC
                LIMIT 1
                """,
                project_id=project_id,
            ).single()

        if record is None:
            return None

        return VersionRecord(
            id=int(record["id"]),
            project_id=record["project_id"],
            version=int(record["version"]),
            author=record["author"],
            content=record["content"],
            created_at=self._parse_time(record["created_at"]),
        )

    def get_version_or_none(self, project_id: str, version: int) -> VersionRecord | None:
        with self._driver.session(database=self._database) as session:
            record = session.run(
                """
                MATCH (p:Project {id: $project_id})-[:HAS_VERSION]->(m:ModelVersion {version: $version})
                RETURN id(m) AS id,
                       m.project_id AS project_id,
                       m.version AS version,
                       m.author AS author,
                       m.content AS content,
                       m.created_at AS created_at
                LIMIT 1
                """,
                project_id=project_id,
                version=version,
            ).single()

        if record is None:
            return None

        return VersionRecord(
            id=int(record["id"]),
            project_id=record["project_id"],
            version=int(record["version"]),
            author=record["author"],
            content=record["content"],
            created_at=self._parse_time(record["created_at"]),
        )

    def list_versions(self, project_id: str, limit: int, offset: int) -> list[VersionRecord]:
        with self._driver.session(database=self._database) as session:
            result = session.run(
                """
                MATCH (p:Project {id: $project_id})-[:HAS_VERSION]->(m:ModelVersion)
                RETURN id(m) AS id,
                       m.project_id AS project_id,
                       m.version AS version,
                       m.author AS author,
                       m.content AS content,
                       m.created_at AS created_at
                ORDER BY m.version DESC
                SKIP $offset
                LIMIT $limit
                """,
                project_id=project_id,
                offset=offset,
                limit=limit,
            )

            rows = list(result)

        return [
            VersionRecord(
                id=int(row["id"]),
                project_id=row["project_id"],
                version=int(row["version"]),
                author=row["author"],
                content=row["content"],
                created_at=self._parse_time(row["created_at"]),
            )
            for row in rows
        ]
