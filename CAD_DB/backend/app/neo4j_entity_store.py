"""
Neo4j Entity Graph Store — stores ACIS entity graphs instead of SAT text.

Entity graph format (matches C++ api_save_entity_list_neo4j output):
{
    "nodes": [
        {"id": "<client-local-id>", "labels": ["<type>"], "props": {"a": "<type>", ...geometry_data}}
    ],
    "rels": [
        {"type": "<relationship-type>", "start": "<node-id>", "end": "<node-id>", "props": {...}}
    ]
}

Relationship types mirror the ACIS topology traversal:
  body_lump, lump_shell, shell_face, shell_wire,
  face_loop, loop_coedge, coedge_edge, edge_curve,
  lump_transform, wire_transform, etc.

Version diff: compares two entity graphs by label+props and returns
the set of added/removed/modified entities between versions.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from datetime import UTC, datetime
from typing import TYPE_CHECKING, Any
from uuid import uuid4

from fastapi import HTTPException, status

from .config import settings

if TYPE_CHECKING:
    from neo4j import Driver, GraphDatabase
from . import schemas as _schemas


@dataclass
class EntityNode:
    local_id: str
    labels: list[str]
    props: dict[str, Any]


@dataclass
class EntityRel:
    rel_type: str
    start_id: str
    end_id: str
    props: dict[str, Any]


@dataclass
class EntityGraph:
    nodes: list[EntityNode] = field(default_factory=list)
    rels: list[EntityRel] = field(default_factory=list)


@dataclass
class EntityVersionRecord:
    id: int
    project_id: str
    version: int
    author: str
    entity_graph: EntityGraph
    created_at: datetime


@dataclass
class EntityDiff:
    added_nodes: list[EntityNode]
    removed_nodes: list[EntityNode]
    modified_nodes: list[tuple[EntityNode, EntityNode]]  # (old, new)
    added_rels: list[EntityRel]
    removed_rels: list[EntityRel]


class Neo4jEntityStore:
    """
    Stores ACIS entity graphs in Neo4j using a subgraph-per-version pattern.

    Node label convention per entity type:
        :entity_graph_version {project_id, version, author, created_at, root_ids: [...]}
        :entity_node {local_id, entity_type, ...all_properties}

    Relationships:
        (:entity_graph_version)-[:ROOT]->(:entity_node)  (one per root entity)
        (:entity_node)-[:PARENT_OF {rel_type}]->(:entity_node)
    """

    ENTITY_NODE_LABELS = {"body", "lump", "shell", "face", "loop", "coedge", "edge",
                          "curve", "surface", "point", "transform"}

    def __init__(self) -> None:
        if not settings.neo4j_uri:
            raise RuntimeError("CAD_DB_NEO4J_URI is required for entity graph storage")

        from neo4j import GraphDatabase as _gd
        self._driver: "Driver" = _gd.driver(
            settings.neo4j_uri,
            auth=(settings.neo4j_user, settings.neo4j_password),
        )
        self._database = settings.neo4j_database

    def close(self) -> None:
        self._driver.close()

    def initialize(self) -> None:
        with self._driver.session(database=self._database) as session:
            session.run(
                """
                CREATE CONSTRAINT egv_id IF NOT EXISTS
                FOR (v:entity_graph_version)
                REQUIRE v.id IS UNIQUE
                """
            )
            session.run(
                """
                CREATE CONSTRAINT egv_project_version IF NOT EXISTS
                FOR (v:entity_graph_version)
                REQUIRE (v.project_id, v.version) IS UNIQUE
                """
            )
            session.run(
                """
                CREATE CONSTRAINT eg_node_local_id IF NOT EXISTS
                FOR (n:entity_node)
                REQUIRE n.local_id IS UNIQUE
                """
            )

    @staticmethod
    def _now_iso() -> str:
        return datetime.now(UTC).isoformat()

    @staticmethod
    def _parse_time(value: Any) -> datetime:
        if isinstance(value, str) and value:
            return datetime.fromisoformat(value.replace("Z", "+00:00"))
        return datetime.now(UTC)

    def _parse_graph(self, nodes_data: list, rels_data: list) -> EntityGraph:
        nodes = [
            EntityNode(
                local_id=str(n.get("id", "")),
                labels=n.get("labels", []),
                props={k: v for k, v in n.get("props", {}).items() if not k.startswith("_")},
            )
            for n in nodes_data
        ]
        rels = [
            EntityRel(
                rel_type=str(r.get("type", "")),
                start_id=str(r.get("start", "")),
                end_id=str(r.get("end", "")),
                props={k: v for k, v in r.get("props", {}).items() if not k.startswith("_")},
            )
            for r in rels_data
        ]
        return EntityGraph(nodes=nodes, rels=rels)

    def _get_latest_entity_version(self, project_id: str) -> int | None:
        with self._driver.session(database=self._database) as session:
            record = session.run(
                """
                MATCH (v:entity_graph_version {project_id: $project_id})
                RETURN max(v.version) AS latest
                """,
                project_id=project_id,
            ).single()
            return record["latest"] if record else None

    # -------------------------------------------------------------------------
    # Public API
    # -------------------------------------------------------------------------

    def create_entity_version(
        self,
        project_id: str,
        author: str,
        entity_graph: _schemas.EntityGraphSchema | dict[str, Any],
        base_version: int | None,
    ) -> EntityVersionRecord:
        """
        Save an entity graph as a new version.
        Optionally enforce base_version as an optimistic lock (like SAT version storage).
        """
        created_at = self._now_iso()

        def write(tx: Any) -> EntityVersionRecord:
            # 新项目第一次保存 entity_graph 时，entity_graph_version 记录可能还不存在。
            # 改为 Upsert 模式：检查是否需要创建根节点，而不是要求必须存在。
            latest_record = tx.run(
                """
                OPTIONAL MATCH (v:entity_graph_version {project_id: $project_id})
                RETURN max(v.version) AS latest
                """,
                project_id=project_id,
            ).single()
            latest_version = latest_record["latest"] if latest_record else None

            if base_version is None and latest_version is not None:
                base_version = latest_version
            if base_version is not None and base_version != latest_version:
                raise HTTPException(
                    status_code=status.HTTP_409_CONFLICT,
                    detail=f"Entity graph version conflict: latest is {latest_version or 0}",
                )

            next_version = 1 if latest_version is None else int(latest_version) + 1
            version_id = str(uuid4())

            root_ids = [n.id for n in entity_graph.nodes]

            tx.run(
                """
                CREATE (v:entity_graph_version {
                    id: $version_id,
                    project_id: $project_id,
                    version: $version,
                    author: $author,
                    created_at: $created_at,
                    root_ids: $root_ids
                })
                """,
                version_id=version_id,
                project_id=project_id,
                version=next_version,
                author=author,
                created_at=created_at,
                root_ids=root_ids,
            )

            for node in entity_graph.nodes:
                node_type = node.labels[0] if node.labels else "unknown"
                props = dict(node.props)
                props["local_id"] = node.id
                props["entity_type"] = node_type
                tx.run(
                    """
                    MERGE (n:entity_node {local_id: $local_id})
                    SET n = $props
                    """,
                    local_id=node.id,
                    props=props,
                )
                tx.run(
                    """
                    MATCH (v:entity_graph_version {id: $version_id})
                    MATCH (n:entity_node {local_id: $local_id})
                    CREATE (v)-[:ROOT]->(n)
                    """,
                    version_id=version_id,
                    local_id=node.id,
                )

            for rel in entity_graph.rels:
                # 修复：改用标准 Cypher MERGE（无需 APOC 插件）。
                # 关系格式：MERGE (start:entity_node {local_id: $start_id})-[r:REL_TYPE]->(end:entity_node {local_id: $end_id})
                # 其中 r.rel_type 属性存原始关系类型（因 Cypher relationship type 不能是动态变量）。
                # 接收端按 r.rel_type 字段重建原始拓扑关系。
                rel_type = rel.rel_type
                # 用 apoc.create.relationship（带 ON CREATE SET）替代原来的 apoc.merge.relationship。
                # 如果 APOC 不可用，回退到简单的 MERGE。
                tx.run(
                    """
                    MATCH (start:entity_node {local_id: $start_id})
                    MATCH (end:entity_node {local_id: $end_id})
                    OPTIONAL MATCH (start)-[existing]->(end)
                    FOREACH(_ IN CASE WHEN existing IS NULL THEN [1] ELSE [] END |
                        CREATE (start)-[r:REL]->(end)
                        SET r.rel_type = $rel_type
                        SET r.props = $props
                    )
                    FOREACH(_ IN CASE WHEN existing IS NOT NULL THEN [1] ELSE [] END |
                        SET existing.rel_type = $rel_type
                        SET existing.props = $props
                    )
                    """,
                    start_id=rel.start,
                    end_id=rel.end,
                    rel_type=rel.rel_type,
                    props=dict(rel.props),
                )

            return EntityVersionRecord(
                id=0,
                project_id=project_id,
                version=next_version,
                author=author,
                entity_graph=entity_graph,
                created_at=self._parse_time(created_at),
            )

        with self._driver.session(database=self._database) as session:
            return session.execute_write(write)

    def get_entity_version(
        self, project_id: str, version: int
    ) -> EntityVersionRecord | None:
        with self._driver.session(database=self._database) as session:
            v_record = session.run(
                """
                MATCH (v:entity_graph_version {project_id: $project_id, version: $version})
                RETURN v
                """,
                project_id=project_id,
                version=version,
            ).single()

            if v_record is None:
                return None

            v_props = dict(v_record["v"])

            nodes_records = session.run(
                """
                MATCH (v:entity_graph_version {project_id: $project_id, version: $version})
                         -[:ROOT]->(n:entity_node)
                RETURN n
                """,
                project_id=project_id,
                version=version,
            ).records()

            nodes = []
            rels = []

            for n_record in nodes_records:
                n_props = dict(n_record["n"])
                local_id = n_props.pop("local_id", "")
                entity_type = n_props.pop("entity_type", "")
                nodes.append(EntityNode(
                    local_id=local_id,
                    labels=[entity_type] if entity_type else [],
                    props=n_props,
                ))

            for n_record in nodes_records:
                n_props = dict(n_record["n"])
                local_id = n_props.get("local_id", "")
                child_records = session.run(
                    """
                    MATCH (n:entity_node {local_id: $local_id})
                             -[r:REL]->(child:entity_node)
                    RETURN r.rel_type AS original_rel_type,
                           child.local_id AS end_id, r.props AS rel_props
                    """,
                    local_id=local_id,
                ).records()

                for child_rec in child_records:
                    rels.append(EntityRel(
                        rel_type=str(child_rec["original_rel_type"] or "REL"),
                        start_id=local_id,
                        end_id=str(child_rec["end_id"]),
                        props=dict(child_rec["rel_props"] or {}),
                    ))

            return EntityVersionRecord(
                id=0,
                project_id=project_id,
                version=v_props["version"],
                author=v_props["author"],
                entity_graph=EntityGraph(nodes=nodes, rels=rels),
                created_at=self._parse_time(v_props.get("created_at")),
            )

    def list_entity_versions(
        self, project_id: str, limit: int = 50, offset: int = 0
    ) -> list[EntityVersionRecord]:
        with self._driver.session(database=self._database) as session:
            records = session.run(
                """
                MATCH (v:entity_graph_version {project_id: $project_id})
                RETURN v
                ORDER BY v.version DESC
                SKIP $offset
                LIMIT $limit
                """,
                project_id=project_id,
                offset=offset,
                limit=limit,
            ).records()

            results = []
            for rec in records:
                v_props = dict(rec["v"])
                results.append(EntityVersionRecord(
                    id=0,
                    project_id=project_id,
                    version=v_props["version"],
                    author=v_props["author"],
                    entity_graph=EntityGraph(nodes=[], rels=[]),
                    created_at=self._parse_time(v_props.get("created_at")),
                ))
            return results

    def diff_entity_versions(
        self, project_id: str, version_a: int, version_b: int
    ) -> EntityDiff:
        """
        Compute the graph diff between two entity graph versions.
        Returns the set of entities that differ.
        """
        graph_a = self.get_entity_version(project_id, version_a)
        graph_b = self.get_entity_version(project_id, version_b)

        if graph_a is None or graph_b is None:
            raise HTTPException(
                status_code=status.HTTP_404_NOT_FOUND,
                detail="One or both entity graph versions not found",
            )

        nodes_a = {n.local_id: n for n in graph_a.entity_graph.nodes}
        nodes_b = {n.local_id: n for n in graph_b.entity_graph.nodes}

        added_nodes = [nodes_b[k] for k in set(nodes_b) - set(nodes_a)]
        removed_nodes = [nodes_a[k] for k in set(nodes_a) - set(nodes_b)]

        modified_nodes = []
        for shared_id in set(nodes_a) & set(nodes_b):
            old_node = nodes_a[shared_id]
            new_node = nodes_b[shared_id]
            if old_node.props != new_node.props:
                modified_nodes.append((old_node, new_node))

        rels_a = {(r.rel_type, r.start_id, r.end_id): r for r in graph_a.entity_graph.rels}
        rels_b = {(r.rel_type, r.start_id, r.end_id): r for r in graph_b.entity_graph.rels}

        added_rels = [rels_b[k] for k in set(rels_b) - set(rels_a)]
        removed_rels = [rels_a[k] for k in set(rels_a) - set(rels_b)]

        return EntityDiff(
            added_nodes=added_nodes,
            removed_nodes=removed_nodes,
            modified_nodes=modified_nodes,
            added_rels=added_rels,
            removed_rels=removed_rels,
        )

    def get_entity_version_as_json(
        self, project_id: str, version: int
    ) -> dict[str, Any] | None:
        record = self.get_entity_version(project_id, version)
        if record is None:
            return None
        return {
            "project_id": record.project_id,
            "version": record.version,
            "author": record.author,
            "created_at": record.created_at.isoformat(),
            "nodes": [
                {
                    "id": n.local_id,
                    "labels": n.labels,
                    "props": n.props,
                }
                for n in record.entity_graph.nodes
            ],
            "rels": [
                {
                    "type": r.rel_type,
                    "start": r.start_id,
                    "end": r.end_id,
                    "props": r.props,
                }
                for r in record.entity_graph.rels
            ],
        }


# Singleton — initialized in main.py alongside neo4j_store
entity_store: Neo4jEntityStore | None = None


def get_entity_store() -> Neo4jEntityStore:
    if entity_store is None:
        raise RuntimeError("entity_store not initialized; call initialize_entity_store() first")
    return entity_store


def initialize_entity_store() -> None:
    global entity_store
    entity_store = Neo4jEntityStore()
    entity_store.initialize()


def shutdown_entity_store() -> None:
    global entity_store
    if entity_store is not None:
        entity_store.close()
        entity_store = None
