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


class Neo4jStore:
    def __init__(self) -> None:
        if not settings.neo4j_uri:
            raise RuntimeError(
                "CAD_DB_NEO4J_URI is required when storage_backend=neo4j"
            )

        self._driver: Driver = GraphDatabase.driver(
            settings.neo4j_uri,
            auth=(settings.neo4j_user, settings.neo4j_password),
        )
        self._database = settings.neo4j_database

    def close(self) -> None:
        self._driver.close()

    def initialize(self) -> None:
        with self._driver.session(database=self._database) as session:
            session.run(
                "CREATE CONSTRAINT project_id_unique IF NOT EXISTS FOR (p:Project) REQUIRE p.id IS UNIQUE"
            )
            session.run(
                "CREATE CONSTRAINT project_name_unique IF NOT EXISTS FOR (p:Project) REQUIRE p.name IS UNIQUE"
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

    def _project_from_node(self, node: Any) -> ProjectRecord:
        props = dict(node)
        return ProjectRecord(
            id=str(props.get("id", "")),
            name=str(props.get("name", "")),
            created_at=self._parse_time_or_now(props.get("created_at")),
            updated_at=self._parse_time_or_now(props.get("updated_at")),
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
            raise HTTPException(
                status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
                detail="Failed to create project",
            )

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
