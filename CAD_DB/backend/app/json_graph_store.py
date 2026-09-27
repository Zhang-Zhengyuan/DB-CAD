"""Neo4j persistence for bridge-free JSON entity graph collaboration."""

from __future__ import annotations

from dataclasses import dataclass
from datetime import UTC, datetime
import json
from typing import Any
from uuid import uuid4

from fastapi import HTTPException, status

from .config import settings


@dataclass(frozen=True)
class JsonGraphVersion:
    project_id: str
    version: int
    author: str
    content: dict[str, Any]
    created_at: datetime


class JsonGraphStore:
    """Stores complete canonical snapshots and a previous-version pointer.

    Nodes and relationships inside the CAD graph remain JSON.  Neo4j is used
    here only as a durable version store; no ACIS symbols cross this boundary.
    """

    def __init__(self) -> None:
        if not settings.neo4j_uri:
            raise RuntimeError("CAD_DB_NEO4J_URI is required for direct JSON collaboration")
        from neo4j import GraphDatabase

        self._driver = GraphDatabase.driver(
            settings.neo4j_uri,
            auth=(settings.neo4j_user, settings.neo4j_password),
        )
        self._database = settings.neo4j_database

    def close(self) -> None:
        self._driver.close()

    def initialize(self) -> None:
        with self._driver.session(database=self._database) as session:
            session.run(
                "CREATE CONSTRAINT json_graph_version_unique IF NOT EXISTS "
                "FOR (v:JsonGraphVersion) REQUIRE (v.project_id, v.version) IS UNIQUE"
            )
            session.run(
                "CREATE CONSTRAINT json_graph_entity_unique IF NOT EXISTS "
                "FOR (e:JsonGraphEntity) REQUIRE (e.version_id, e.entity_id) IS UNIQUE"
            )

    @staticmethod
    def _now() -> datetime:
        return datetime.now(UTC)

    @staticmethod
    def _from_node(node: Any) -> JsonGraphVersion:
        props = dict(node)
        raw = props.get("content_json", "{}")
        content = json.loads(raw) if isinstance(raw, str) else dict(raw or {})
        return JsonGraphVersion(
            project_id=str(props["project_id"]),
            version=int(props["version"]),
            author=str(props.get("author", "")),
            content=content,
            created_at=datetime.fromisoformat(str(props["created_at"]).replace("Z", "+00:00")),
        )

    def _project_exists(self, session: Any, project_id: str) -> bool:
        row = session.run(
            "MATCH (p {id: $project_id}) RETURN p LIMIT 1", project_id=project_id
        ).single()
        return row is not None

    def latest(self, project_id: str) -> JsonGraphVersion | None:
        with self._driver.session(database=self._database) as session:
            row = session.run(
                "MATCH (v:JsonGraphVersion {project_id: $project_id}) "
                "RETURN v ORDER BY v.version DESC LIMIT 1",
                project_id=project_id,
            ).single()
        return self._from_node(row["v"]) if row else None

    def get(self, project_id: str, version: int) -> JsonGraphVersion | None:
        with self._driver.session(database=self._database) as session:
            row = session.run(
                "MATCH (v:JsonGraphVersion {project_id: $project_id, version: $version}) RETURN v LIMIT 1",
                project_id=project_id,
                version=version,
            ).single()
        return self._from_node(row["v"]) if row else None

    def list_after(self, project_id: str, version: int) -> list[JsonGraphVersion]:
        with self._driver.session(database=self._database) as session:
            rows = session.run(
                "MATCH (v:JsonGraphVersion {project_id: $project_id}) "
                "WHERE v.version > $version RETURN v ORDER BY v.version ASC",
                project_id=project_id,
                version=version,
            )
            return [self._from_node(row["v"]) for row in rows]

    def create(
        self,
        project_id: str,
        author: str,
        content: dict[str, Any],
        base_version: int | None,
    ) -> JsonGraphVersion:
        now = self._now()
        content_json = json.dumps(content, ensure_ascii=False, separators=(",", ":"))

        def write(tx: Any) -> JsonGraphVersion:
            if not self._project_exists(tx, project_id):
                raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Project not found")
            latest = tx.run(
                "MATCH (v:JsonGraphVersion {project_id: $project_id}) "
                "RETURN max(v.version) AS latest",
                project_id=project_id,
            ).single()
            latest_version = latest["latest"] if latest else None
            if base_version is not None and base_version != latest_version:
                raise HTTPException(
                    status_code=status.HTTP_409_CONFLICT,
                    detail={
                        "reason": "stale_base",
                        "latest_version": latest_version or 0,
                        "base_version": base_version,
                    },
                )
            # A direct JSON stream is deliberately independent from SAT model
            # versions.  Its first commit must use base_version=null.
            if base_version is None and latest_version is not None:
                raise HTTPException(
                    status_code=status.HTTP_409_CONFLICT,
                    detail={"reason": "base_version_required", "latest_version": latest_version},
                )
            next_version = 1 if latest_version is None else int(latest_version) + 1
            row = tx.run(
                "MATCH (p {id: $project_id}) "
                "CREATE (v:JsonGraphVersion {id: $id, project_id: $project_id, "
                "version: $version, author: $author, created_at: $created_at, "
                "content_json: $content_json}) "
                "CREATE (p)-[:HAS_JSON_GRAPH_VERSION]->(v) RETURN v",
                id=str(uuid4()), project_id=project_id, version=next_version,
                author=author, created_at=now.isoformat(), content_json=content_json,
            ).single()
            if row is None:
                raise HTTPException(status_code=500, detail="Failed to create JSON graph version")

            # Keep an explicit immutable version chain.  The complete JSON
            # snapshot remains the compatibility/read path, while the
            # per-version entity records below make the Neo4j representation
            # queryable without decoding SAT or the whole snapshot.
            if latest_version is not None:
                tx.run(
                    "MATCH (v:JsonGraphVersion {project_id: $project_id, version: $version}), "
                    "(prev:JsonGraphVersion {project_id: $project_id, version: $previous_version}) "
                    "CREATE (v)-[:PREVIOUS_JSON_VERSION]->(prev)",
                    project_id=project_id,
                    version=next_version,
                    previous_version=int(latest_version),
                )

            graph = content.get("entity_graph", {})
            nodes = graph.get("nodes", []) if isinstance(graph, dict) else []
            rels = graph.get("rels", []) if isinstance(graph, dict) else []
            version_id = str(row["v"]["id"])
            if nodes:
                tx.run(
                    "MATCH (v:JsonGraphVersion {id: $version_id}) "
                    "UNWIND $nodes AS item "
                    "CREATE (e:JsonGraphEntity {version_id: $version_id, "
                    "project_id: $project_id, entity_id: item.id, "
                    "labels_json: item.labels_json, props_json: item.props_json}) "
                    "CREATE (v)-[:HAS_JSON_ENTITY]->(e)",
                    version_id=version_id,
                    project_id=project_id,
                    nodes=[
                        {
                            "id": str(node["id"]),
                            "labels_json": json.dumps(node.get("labels", []), ensure_ascii=False, separators=(",", ":")),
                            "props_json": json.dumps(node.get("props", {}), ensure_ascii=False, separators=(",", ":")),
                        }
                        for node in nodes
                    ],
                )
            if rels:
                tx.run(
                    "MATCH (v:JsonGraphVersion {id: $version_id})-[:HAS_JSON_ENTITY]->(s:JsonGraphEntity), "
                    "(v)-[:HAS_JSON_ENTITY]->(e:JsonGraphEntity) "
                    "UNWIND $rels AS item "
                    "WITH v, s, e, item WHERE s.entity_id = item.start AND e.entity_id = item.end "
                    "CREATE (s)-[:JSON_TO {type: item.type, props_json: item.props_json}]->(e)",
                    version_id=version_id,
                    rels=[
                        {
                            "type": str(rel["type"]),
                            "start": str(rel["start"]),
                            "end": str(rel["end"]),
                            "props_json": json.dumps(rel.get("props", {}), ensure_ascii=False, separators=(",", ":")),
                        }
                        for rel in rels
                    ],
                )
            return self._from_node(row["v"])

        with self._driver.session(database=self._database) as session:
            return session.execute_write(write)


_store: JsonGraphStore | None = None


def get_json_graph_store() -> JsonGraphStore:
    global _store
    if _store is None:
        _store = JsonGraphStore()
        _store.initialize()
    return _store


def shutdown_json_graph_store() -> None:
    global _store
    if _store is not None:
        _store.close()
        _store = None
