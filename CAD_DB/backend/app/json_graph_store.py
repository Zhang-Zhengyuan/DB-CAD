"""Neo4j persistence for bridge-free JSON entity graph collaboration."""

from __future__ import annotations

from dataclasses import dataclass
from datetime import UTC, datetime
import json
import hashlib
from typing import Any
from uuid import uuid4

from fastapi import HTTPException, status

from .config import settings
from .json_collab import graph_component, validate_graph


EMPTY_GRAPH: dict[str, Any] = {"nodes": [], "rels": []}


def _canonical(value: Any) -> str:
    return json.dumps(value, ensure_ascii=False, sort_keys=True, separators=(",", ":"))


def _content_hash(value: Any) -> str:
    return hashlib.sha256(_canonical(value).encode("utf-8")).hexdigest()


def _component_hash(nodes: list[dict[str, Any]], rels: list[dict[str, Any]]) -> str:
    """Hash only topology payload, never per-request serializer metadata.

    Schema/build metadata belongs to the version manifest. Including it in a
    component hash would create a second copy of an unchanged Body whenever
    the client adds a harmless metadata field.
    """
    return _content_hash({"nodes": nodes, "rels": rels})


def split_graph_components(graph: Any) -> tuple[list[dict[str, Any]], list[dict[str, Any]]]:
    """Split a graph into immutable Body components and cross-component edges."""
    canonical = validate_graph(graph or EMPTY_GRAPH)
    body_ids = [
        node["id"] for node in canonical["nodes"]
        if "body" in {str(label).lower() for label in node.get("labels", [])}
    ]
    components: list[dict[str, Any]] = []
    owner: set[str] = set()
    for root in body_ids:
        component = graph_component(canonical, [root])
        ids = {node["id"] for node in component["nodes"]} - owner
        if not ids:
            continue
        component = validate_graph({
            "nodes": [node for node in component["nodes"] if node["id"] in ids],
            "rels": [rel for rel in component["rels"] if rel["start"] in ids and rel["end"] in ids],
            **{key: value for key, value in component.items() if key not in {"nodes", "rels"}},
        })
        component["root_uuid"] = root
        component["component_hash"] = _component_hash(component["nodes"], component["rels"])
        components.append(component)
        owner.update(ids)

    remaining = {node["id"] for node in canonical["nodes"]} - owner
    if remaining:
        component = validate_graph({
            "nodes": [node for node in canonical["nodes"] if node["id"] in remaining],
            "rels": [rel for rel in canonical["rels"] if rel["start"] in remaining and rel["end"] in remaining],
            **{key: value for key, value in canonical.items() if key not in {"nodes", "rels"}},
        })
        component["root_uuid"] = "__orphan__"
        component["component_hash"] = _component_hash(component["nodes"], component["rels"])
        components.append(component)
        owner.update(remaining)

    internal = {
        (rel["start"], rel["end"], rel["type"], _canonical(rel.get("props", {})))
        for component in components for rel in component["rels"]
    }
    relations = [
        rel for rel in canonical["rels"]
        if (rel["start"], rel["end"], rel["type"], _canonical(rel.get("props", {}))) not in internal
    ]
    return components, relations


@dataclass(frozen=True)
class JsonGraphVersion:
    project_id: str
    version: int
    author: str
    content: dict[str, Any]
    created_at: datetime


class JsonGraphStore:
    """Stores immutable components plus version manifests in Neo4j.

    A full graph is a read projection assembled from the manifest. No ACIS
    symbols cross this boundary.
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
                "CREATE CONSTRAINT json_graph_entity_component_unique IF NOT EXISTS "
                "FOR (e:JsonGraphEntity) REQUIRE (e.component_hash, e.entity_id) IS UNIQUE"
            )
            session.run(
                "CREATE CONSTRAINT json_graph_component_unique IF NOT EXISTS "
                "FOR (c:JsonGraphComponent) REQUIRE (c.project_id, c.component_hash) IS UNIQUE"
            )
            session.run(
                "CREATE CONSTRAINT json_graph_relation_unique IF NOT EXISTS "
                "FOR (r:JsonGraphRelation) REQUIRE (r.project_id, r.relation_hash) IS UNIQUE"
            )

    @staticmethod
    def _now() -> datetime:
        return datetime.now(UTC)

    @staticmethod
    def _from_node(node: Any) -> JsonGraphVersion:
        props = dict(node)
        raw = props.get("delta_json", props.get("content_json", "{}"))
        content = json.loads(raw) if isinstance(raw, str) else dict(raw or {})
        return JsonGraphVersion(
            project_id=str(props["project_id"]),
            version=int(props["version"]),
            author=str(props.get("author", "")),
            content=content,
            created_at=datetime.fromisoformat(str(props["created_at"]).replace("Z", "+00:00")),
        )

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

    def graph(self, project_id: str, version: int) -> dict[str, Any]:
        """Materialize one manifest by traversing shared components and edges."""
        with self._driver.session(database=self._database) as session:
            metadata: dict[str, Any] = {}
            version_row = session.run(
                "MATCH (v:JsonGraphVersion {project_id: $project_id, version: $version}) RETURN v LIMIT 1",
                project_id=project_id, version=version,
            ).single()
            if version_row:
                props = dict(version_row["v"])
                if props.get("storage_mode") != "component_manifest_v1":
                    raw = props.get("content_json", "{}")
                    if isinstance(raw, str):
                        try:
                            legacy = json.loads(raw)
                            graph = legacy.get("entity_graph")
                            if isinstance(graph, dict):
                                return validate_graph(graph)
                        except (TypeError, ValueError):
                            pass
                else:
                    raw_delta = props.get("delta_json", "{}")
                    try:
                        decoded_delta = json.loads(raw_delta) if isinstance(raw_delta, str) else dict(raw_delta or {})
                        metadata = {
                            key: decoded_delta[key]
                            for key in ("schema", "serializer_build")
                            if key in decoded_delta
                        }
                    except (TypeError, ValueError):
                        metadata = {}
            nodes = session.run(
                "MATCH (v:JsonGraphVersion {project_id: $project_id, version: $version})"
                "-[:HAS_COMPONENT]->(:JsonGraphComponent)-[:HAS_JSON_ENTITY]->(e:JsonGraphEntity) "
                "RETURN e.entity_id AS id, e.labels_json AS labels_json, e.props_json AS props_json",
                project_id=project_id, version=version,
            )
            node_values = [{"id": str(row["id"]), "labels": json.loads(row["labels_json"]),
                            "props": json.loads(row["props_json"])} for row in nodes]
            rels = session.run(
                "MATCH (v:JsonGraphVersion {project_id: $project_id, version: $version})"
                "-[:HAS_COMPONENT]->(:JsonGraphComponent)-[:HAS_JSON_ENTITY]->(s:JsonGraphEntity)"
                "-[r:JSON_TO]->(e:JsonGraphEntity) "
                "RETURN s.entity_id AS start, e.entity_id AS end, r.type AS type, r.props_json AS props_json "
                "UNION ALL "
                "MATCH (v:JsonGraphVersion {project_id: $project_id, version: $version})"
                "-[:HAS_RELATION]->(r:JsonGraphRelation) "
                "RETURN r.start AS start, r.end AS end, r.type AS type, r.props_json AS props_json",
                project_id=project_id, version=version,
            )
            rel_values = [{"start": str(row["start"]), "end": str(row["end"]), "type": str(row["type"]),
                           "props": json.loads(row["props_json"])} for row in rels]
        return validate_graph({"nodes": node_values, "rels": rel_values, **metadata})

    def create(
        self,
        project_id: str,
        author: str,
        content: dict[str, Any],
        base_version: int | None,
    ) -> JsonGraphVersion:
        now = self._now()
        graph = validate_graph(content.get("entity_graph", EMPTY_GRAPH))
        components, relations = split_graph_components(graph)
        delta_content = {
            "changes": content.get("changes", []),
            "removed_ids": content.get("removed_ids", []),
            "schema": content.get("schema", "dbcad.entity_graph.v1"),
            "serializer_build": graph.get("serializer_build", ""),
            "storage_mode": "component_manifest_v1",
            "graph_hash": content.get("graph_hash", ""),
        }
        content_json = json.dumps(delta_content, ensure_ascii=False, separators=(",", ":"))

        def write(tx: Any) -> JsonGraphVersion:
            # Lock the project before reading the head. An asyncio lock only
            # protects one FastAPI process; Neo4j's write lock also serializes
            # commits from other workers and hosts.
            project = tx.run(
                "MATCH (p {id: $project_id}) "
                "SET p.json_graph_write_token = $token RETURN p",
                project_id=project_id,
                token=str(uuid4()),
            ).single()
            if project is None:
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
                "delta_json: $content_json, graph_hash: $graph_hash, storage_mode: 'component_manifest_v1'}) "
                "CREATE (p)-[:HAS_JSON_GRAPH_VERSION]->(v) RETURN v",
                id=str(uuid4()), project_id=project_id, version=next_version,
                author=author, created_at=now.isoformat(), content_json=content_json,
                graph_hash=str(content.get("graph_hash", "")),
            ).single()
            if row is None:
                raise HTTPException(status_code=500, detail="Failed to create JSON graph version")

            # Keep an explicit immutable version chain. The manifest is the
            # source of truth; components and relations are shared records.
            if latest_version is not None:
                tx.run(
                    "MATCH (v:JsonGraphVersion {project_id: $project_id, version: $version}), "
                    "(prev:JsonGraphVersion {project_id: $project_id, version: $previous_version}) "
                    "CREATE (v)-[:PREVIOUS_JSON_VERSION]->(prev)",
                    project_id=project_id,
                    version=next_version,
                    previous_version=int(latest_version),
                )

            version_id = str(row["v"]["id"])
            for component in components:
                component_hash = str(component["component_hash"])
                # Migrate components created by the earlier implementation,
                # whose hash included serializer metadata. Reuse an existing
                # same-root component when its nodes/relations are identical.
                existing_rows = tx.run(
                    "MATCH (c:JsonGraphComponent {project_id: $project_id, root_uuid: $root_uuid}) "
                    "RETURN c.component_hash AS component_hash, c.content_json AS content_json",
                    project_id=project_id, root_uuid=str(component["root_uuid"]),
                )
                for existing in existing_rows:
                    try:
                        existing_content = json.loads(existing["content_json"] or "{}")
                    except (TypeError, ValueError):
                        continue
                    if _component_hash(existing_content.get("nodes", []), existing_content.get("rels", [])) == component_hash:
                        component_hash = str(existing["component_hash"])
                        break
                component_content = {
                    "nodes": component["nodes"],
                    "rels": component["rels"],
                    **{key: value for key, value in component.items()
                       if key not in {"nodes", "rels", "root_uuid", "component_hash"}},
                }
                tx.run(
                    "MERGE (c:JsonGraphComponent {project_id: $project_id, component_hash: $component_hash}) "
                    "ON CREATE SET c.id = $id, c.root_uuid = $root_uuid, c.content_json = $content_json, "
                    "c.node_count = $node_count, c.rel_count = $rel_count, c.created_at = $created_at",
                    project_id=project_id, component_hash=component_hash, id=str(uuid4()),
                    root_uuid=str(component["root_uuid"]),
                    content_json=json.dumps(component_content, ensure_ascii=False, separators=(",", ":")),
                    node_count=len(component["nodes"]), rel_count=len(component["rels"]),
                    created_at=now.isoformat(),
                )
                tx.run(
                    "MATCH (v:JsonGraphVersion {id: $version_id}), "
                    "(c:JsonGraphComponent {project_id: $project_id, component_hash: $component_hash}) "
                    "MERGE (v)-[:HAS_COMPONENT {root_uuid: $root_uuid}]->(c)",
                    version_id=version_id, project_id=project_id,
                    component_hash=component_hash, root_uuid=str(component["root_uuid"]),
                )
                # Populate immutable entities exactly once. MERGE prevents a
                # later version from duplicating unchanged component payloads.
                tx.run(
                    "MATCH (c:JsonGraphComponent {project_id: $project_id, component_hash: $component_hash}) "
                    "WITH c WHERE NOT (c)-[:HAS_JSON_ENTITY]->() "
                    "UNWIND $nodes AS item "
                    "MERGE (e:JsonGraphEntity {component_hash: $component_hash, entity_id: item.id}) "
                    "ON CREATE SET e.labels_json = item.labels_json, e.props_json = item.props_json "
                    "CREATE (c)-[:HAS_JSON_ENTITY]->(e)",
                    project_id=project_id, component_hash=component_hash,
                    nodes=[{"id": str(node["id"]),
                            "labels_json": json.dumps(node.get("labels", []), ensure_ascii=False, separators=(",", ":")),
                            "props_json": json.dumps(node.get("props", {}), ensure_ascii=False, separators=(",", ":"))}
                           for node in component["nodes"]],
                )
                if component["rels"]:
                    tx.run(
                        "UNWIND $rels AS item "
                        "MATCH (s:JsonGraphEntity {component_hash: $component_hash, entity_id: item.start}) "
                        "MATCH (e:JsonGraphEntity {component_hash: $component_hash, entity_id: item.end}) "
                        "MERGE (s)-[:JSON_TO {type: item.type, props_json: item.props_json}]->(e)",
                        component_hash=component_hash,
                        rels=[{"type": str(rel["type"]), "start": str(rel["start"]), "end": str(rel["end"]),
                               "props_json": json.dumps(rel.get("props", {}), ensure_ascii=False, separators=(",", ":"))}
                              for rel in component["rels"]],
                    )
            if relations:
                tx.run(
                    "UNWIND $rels AS item "
                    "MERGE (r:JsonGraphRelation {project_id: $project_id, relation_hash: item.relation_hash}) "
                    "ON CREATE SET r.start = item.start, r.end = item.end, r.type = item.type, r.props_json = item.props_json "
                    "WITH r MATCH (v:JsonGraphVersion {id: $version_id}) MERGE (v)-[:HAS_RELATION]->(r)",
                    project_id=project_id, version_id=version_id,
                    rels=[{"relation_hash": _content_hash(rel), "start": str(rel["start"]), "end": str(rel["end"]),
                           "type": str(rel["type"]), "props_json": json.dumps(rel.get("props", {}), ensure_ascii=False, separators=(",", ":"))}
                          for rel in relations],
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
