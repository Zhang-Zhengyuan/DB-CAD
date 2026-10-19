"""v1 项目/版本/组件存储（V3_PLAN.md §2：服务端权威的版本链）。

数据模型（与 Neo4j 实现共享同一形状）：
  Project   {id, name, created_at, updated_at}
  Version   {project_id, number, message, author, created_at, base_version}
            -[:CONTAINS {order}]-> Component
  Component {hash_sat(全局去重键), hash_glb, name, color, bbox, triangles, vertices}
  Tag       {project_id, name, version}          # 定版标签
  Branch    {project_id, name, head_version}     # 分支预留（M4.4 转正）

版本提交协议：commit 带 base_version 做乐观锁（过期 → 409），命令全序即版本链
（ADR-2）。组件不可变：同 hash_sat 的组件跨版本自动复用。

两个实现：MemoryV1Store（单测与无 Neo4j 环境）与 Neo4jV1Store（生产），
经 get_v1_store()/shutdown_v1_store() 单例化，模式同 json_graph_store。
"""

from __future__ import annotations

from dataclasses import dataclass, field
from datetime import UTC, datetime
from typing import Any
from uuid import uuid4

from fastapi import HTTPException, status

from .entity_projection import entity_key, match_entities, normalize_entity_projection, projection_stats


@dataclass
class ProjectRecord:
    id: str
    name: str
    created_at: datetime
    updated_at: datetime


class VersionConflict(HTTPException):
    """base_version 过期（他方已提交更新版本）。"""

    def __init__(self, current_head: int | None) -> None:
        super().__init__(
            status_code=status.HTTP_409_CONFLICT,
            detail={
                "reason": "stale_base_version",
                "current_head": current_head,
            },
        )


@dataclass
class SceneComponent:
    hash_sat: str
    hash_glb: str
    name: str = ""
    color: list[float] = field(default_factory=lambda: [0.8, 0.8, 0.8])
    bbox_min: list[float] = field(default_factory=lambda: [0.0, 0.0, 0.0])
    bbox_max: list[float] = field(default_factory=lambda: [0.0, 0.0, 0.0])
    triangles: int = 0
    vertices: int = 0
    graph: str = ""  # 实体图投影资产哈希（P6；旧组件为空）

    def to_scene_json(self) -> dict[str, Any]:
        return {
            "component_id": self.hash_sat,
            "name": self.name,
            "sat": self.hash_sat,
            "glb": self.hash_glb,
            "graph": self.graph,
            "color": self.color,
            "bbox": {"min": self.bbox_min, "max": self.bbox_max},
            "triangles": self.triangles,
            "vertices": self.vertices,
        }


@dataclass
class V1Version:
    project_id: str
    number: int
    message: str
    author: str
    created_at: datetime
    base_version: int | None
    components: list[SceneComponent]
    # undo/redo 恢复的历史状态；base_version 仍是追加版本链的直接父版本。
    restored_from: int | None = None


def _now() -> datetime:
    return datetime.now(UTC)


class MemoryV1Store:
    def __init__(self) -> None:
        self.projects: dict[str, ProjectRecord] = {}
        self.versions: dict[str, list[V1Version]] = {}
        self.tags: dict[str, dict[str, int]] = {}
        self.branches: dict[str, dict[str, int]] = {}
        # 重做栈（进程内）：保存被撤销状态的版本号，以保留逻辑历史位置。
        # 不随 Neo4j 持久化——服务重启后 redo 清空是可接受语义。
        self.redo_stacks: dict[str, list[int]] = {}
        # 实体级投影（P6/ADR-3）：hash_sat → 规范化投影 {nodes, rels,
        # serializer_build}。组件不可变 ⇒ 投影幂等，已存在即零写入。
        self.entity_projections: dict[str, dict[str, Any]] = {}
        # 实体级增量（P8/Phase D）：内容寻址实体节点池与 generation 链——
        # 同内容实体跨组件/跨版本共享（D 栈不可变实体语义）。
        self.entity_nodes: dict[str, dict[str, Any]] = {}  # entity_key → 节点
        self.derived_from: dict[str, set[str]] = {}  # child_key → {parent_key}

    # ---- 实体级投影（P6/ADR-3 + P8/Phase D）---------------------------
    def write_entity_projection(
        self,
        component_hash: str,
        graph: dict[str, Any],
        parent_hash: str | None = None,
    ) -> dict[str, Any]:
        """写入组件实体投影（幂等）；parent_hash 用于 generation 匹配。

        返回 {written, entities, rels, entity_added, entity_shared, derived}：
        entity_added = 内容寻址新增实体节点数（P8 验收口径——改一个 face
        时 ≈ 内容变化实体数，而非整个组件）。
        """
        if component_hash in self.entity_projections:
            stats = projection_stats(self.entity_projections[component_hash])
            return {"written": False, **stats, "entity_added": 0, "entity_shared": stats["entities"], "derived": 0}
        projection = normalize_entity_projection(component_hash, graph)
        parent = self.entity_projections.get(parent_hash) if parent_hash else None
        ingest = self._ingest_entities(projection, parent)
        self.entity_projections[component_hash] = projection
        return {"written": True, **projection_stats(projection), **ingest}

    def _ingest_entities(
        self, projection: dict[str, Any], parent: dict[str, Any] | None
    ) -> dict[str, int]:
        matched = match_entities(parent, projection) if parent else {}
        parent_key_by_id = (
            {str(n["id"]): entity_key(n) for n in parent["nodes"]} if parent else {}
        )
        added = shared = derived = 0
        for node in projection["nodes"]:
            key = entity_key(node)
            node["entity_key"] = key
            if key in self.entity_nodes:
                shared += 1
            else:
                self.entity_nodes[key] = {"labels": node.get("labels"), "props": node.get("props")}
                added += 1
            parent_id = matched.get(str(node["id"]))
            parent_key = parent_key_by_id.get(parent_id) if parent_id else None
            if parent_key and parent_key != key:
                self.derived_from.setdefault(key, set()).add(parent_key)
                derived += 1
        return {"entity_added": added, "entity_shared": shared, "derived": derived}

    def get_entity_projection(self, component_hash: str) -> dict[str, Any] | None:
        return self.entity_projections.get(component_hash)

    def has_entity_projection(self, component_hash: str) -> bool:
        return component_hash in self.entity_projections

    def count_derived_chains(self, component_hashes: list[str]) -> int:
        """指定组件清单内触达的 DERIVED_FROM 链数（diff 报表用；父可在清单外
        ——generation 语义就是新内容链向旧内容）。"""
        keys = {
            node.get("entity_key")
            for h in component_hashes
            if (proj := self.entity_projections.get(h))
            for node in proj["nodes"]
        }
        return sum(1 for child in self.derived_from if child in keys)

    def compact_orphan_entities(self) -> int:
        """压实（P8.5）：清理不再被任何组件投影引用的实体节点与链。
        组件不可变但版本链会淘汰引用——被淘汰内容的实体成为孤立节点。
        幂等，可定期重跑。返回清理的实体节点数。"""
        referenced = {
            node.get("entity_key")
            for proj in self.entity_projections.values()
            for node in proj["nodes"]
        }
        orphans = [k for k in self.entity_nodes if k not in referenced]
        for key in orphans:
            self.entity_nodes.pop(key, None)
        self.derived_from = {
            child: parents
            for child, parents in self.derived_from.items()
            if child in referenced
        }
        return len(orphans)

    def query_entities(
        self,
        component_hashes: list[str],
        label: str | None = None,
        limit: int = 1000,
    ) -> list[dict[str, Any]]:
        """按组件清单查实体节点；label 大小写不敏感过滤（face / Face 皆可）。"""
        wanted = {h for h in component_hashes}
        results: list[dict[str, Any]] = []
        needle = label.lower() if label else None
        for component_hash in component_hashes:
            projection = self.entity_projections.get(component_hash)
            if projection is None:
                continue
            for node in projection["nodes"]:
                labels = [str(l) for l in node.get("labels", [])]
                if needle and needle not in (l.lower() for l in labels):
                    continue
                results.append(
                    {
                        "component": component_hash,
                        "id": str(node.get("id", "")),
                        "entity_key": str(node.get("entity_key", "")),
                        "labels": labels,
                        "props": node.get("props", {}),
                    }
                )
                if len(results) >= limit:
                    return results
        return results

    def push_redo(self, project_id: str, version: int) -> None:
        self.redo_stacks.setdefault(project_id, []).append(version)

    def peek_redo(self, project_id: str) -> int | None:
        stack = self.redo_stacks.get(project_id)
        return stack[-1] if stack else None

    def pop_redo(self, project_id: str) -> int | None:
        stack = self.redo_stacks.get(project_id)
        if not stack:
            return None
        return stack.pop()

    def clear_redo(self, project_id: str) -> None:
        self.redo_stacks.pop(project_id, None)

    # ---- 项目 ----------------------------------------------------------
    def create_project(self, name: str) -> ProjectRecord:
        if any(p.name == name for p in self.projects.values()):
            raise HTTPException(status.HTTP_409_CONFLICT, "Project name already exists")
        now = _now()
        record = ProjectRecord(str(uuid4()), name, now, now)
        self.projects[record.id] = record
        self.versions[record.id] = []
        self.tags[record.id] = {}
        self.branches[record.id] = {"main": 0}
        return record

    def get_project_or_none(self, project_id: str) -> ProjectRecord | None:
        return self.projects.get(project_id)

    def list_projects(self) -> list[ProjectRecord]:
        return sorted(self.projects.values(), key=lambda p: (p.updated_at, p.id))

    def rename_project(self, project_id: str, name: str) -> ProjectRecord:
        record = self.projects[project_id]
        if any(p.name == name and p.id != project_id for p in self.projects.values()):
            raise HTTPException(status.HTTP_409_CONFLICT, "Project name already exists")
        record.name = name
        record.updated_at = _now()
        return record

    def delete_project(self, project_id: str) -> None:
        self.projects.pop(project_id, None)
        self.versions.pop(project_id, None)
        self.tags.pop(project_id, None)
        self.branches.pop(project_id, None)
        self.redo_stacks.pop(project_id, None)
        # 实体投影按组件（内容寻址）归属而非项目——组件可跨项目复用，
        # 项目删除不动投影；投影生命周期与组件资产一致。

    def touch(self, project_id: str) -> None:
        self.projects[project_id].updated_at = _now()

    # ---- 版本链 ---------------------------------------------------------
    def latest(self, project_id: str) -> V1Version | None:
        rows = self.versions.get(project_id, [])
        return rows[-1] if rows else None

    def get_version(self, project_id: str, number: int) -> V1Version | None:
        for row in self.versions.get(project_id, []):
            if row.number == number:
                return row
        return None

    def list_versions(self, project_id: str) -> list[V1Version]:
        return list(self.versions.get(project_id, []))

    def commit_version(
        self,
        project_id: str,
        message: str,
        author: str,
        components: list[SceneComponent],
        base_version: int | None = None,
        restored_from: int | None = None,
    ) -> V1Version:
        head = self.latest(project_id)
        head_number = head.number if head else None
        if head_number is not None and base_version is not None and base_version != head_number:
            raise VersionConflict(head_number)
        if head_number is None and base_version is not None:
            raise VersionConflict(None)
        number = (head.number + 1) if head else 1
        version = V1Version(
            project_id=project_id,
            number=number,
            message=message,
            author=author,
            created_at=_now(),
            base_version=head_number,
            components=components,
            restored_from=restored_from,
        )
        self.versions[project_id].append(version)
        self.touch(project_id)
        head_branch = self.branches[project_id].get("main")
        if head_branch is not None:
            self.branches[project_id]["main"] = number
        return version

    # ---- 标签 / 分支 ------------------------------------------------------
    def tag_version(self, project_id: str, name: str, version: int) -> dict[str, Any]:
        if self.get_version(project_id, version) is None:
            raise HTTPException(status.HTTP_404_NOT_FOUND, f"Version {version} not found")
        self.tags[project_id][name] = version
        self.touch(project_id)
        return {"name": name, "version": version}

    def list_tags(self, project_id: str) -> list[dict[str, Any]]:
        return [
            {"name": name, "version": version}
            for name, version in sorted(self.tags[project_id].items(), key=lambda kv: kv[1])
        ]

    def list_branches(self, project_id: str) -> list[dict[str, Any]]:
        return [
            {"name": name, "head_version": head}
            for name, head in sorted(self.branches[project_id].items())
        ]

    def create_branch(self, project_id: str, name: str, from_version: int) -> dict[str, Any]:
        if name in self.branches[project_id]:
            raise HTTPException(status.HTTP_409_CONFLICT, "Branch already exists")
        if self.get_version(project_id, from_version) is None:
            raise HTTPException(status.HTTP_404_NOT_FOUND, f"Version {from_version} not found")
        self.branches[project_id][name] = from_version
        self.touch(project_id)
        return {"name": name, "head_version": from_version}


class Neo4jV1Store(MemoryV1Store):
    """生产实现：Neo4j 为事实源，目录与版本链启动全量预载、写穿。

    P1 规模（项目数 × 版本数 = O(10^4)）下全量预载最简单且一致；命令引擎
    （P2）接入后若需要按项目惰性加载，再拆分镜像粒度。

    实体级投影（P6/ADR-3）：库内双表示——权威载荷 labels_json/props_json +
    原生标签与一级扁平字段（可查询投影，"细"档同 v2 Phase B 语义）。挂载
    (V1Component)-[:HAS_ENTITY]->(V1Entity)，拓扑关系为统一边型
    [:TOPOLOGY {rel_type}]（MERGE 不支持动态关系类型，rel_type 属性可查）。
    """

    def __init__(self, driver: Any, database: str) -> None:
        super().__init__()
        self._driver = driver
        self._database = database

    def initialize(self) -> None:
        with self._driver.session(database=self._database) as session:
            session.run(
                "CREATE CONSTRAINT v1_project_id IF NOT EXISTS "
                "FOR (p:V1Project) REQUIRE p.id IS UNIQUE"
            )
            session.run(
                "CREATE CONSTRAINT v1_component_sat IF NOT EXISTS "
                "FOR (c:V1Component) REQUIRE c.hash_sat IS UNIQUE"
            )
            for record in session.run("MATCH (p:V1Project) RETURN p"):
                props = dict(record["p"])
                self.projects[props["id"]] = ProjectRecord(
                    id=props["id"],
                    name=props["name"],
                    created_at=datetime.fromisoformat(props["created_at"]),
                    updated_at=datetime.fromisoformat(props["updated_at"]),
                )
                self.versions[props["id"]] = []
                self.tags[props["id"]] = {}
                self.branches[props["id"]] = {"main": 0}
            for record in session.run("MATCH (t:V1Tag) RETURN t"):
                props = dict(record["t"])
                self.tags.setdefault(props["project_id"], {})[props["name"]] = props["version"]
            for record in session.run(
                """
                MATCH (v:V1Version)
                OPTIONAL MATCH (v)-[r:CONTAINS]->(c:V1Component)
                RETURN v, collect([c, r.order]) AS comps
                """
            ):
                props = dict(record["v"])
                ordered = sorted(record["comps"], key=lambda pair: pair[1])
                components = [
                    SceneComponent(
                        hash_sat=c["hash_sat"],
                        hash_glb=c["hash_glb"],
                        name=c.get("name", ""),
                        color=list(c.get("color", [0.8, 0.8, 0.8])),
                        bbox_min=list(c.get("bbox_min", [0.0, 0.0, 0.0])),
                        bbox_max=list(c.get("bbox_max", [0.0, 0.0, 0.0])),
                        triangles=int(c.get("triangles", 0)),
                        vertices=int(c.get("vertices", 0)),
                        graph=c.get("hash_graph", ""),
                    )
                    for c, _ in ordered
                    if c is not None
                ]
                self.versions.setdefault(props["project_id"], []).append(
                    V1Version(
                        project_id=props["project_id"],
                        number=int(props["number"]),
                        message=props.get("message", ""),
                        author=props.get("author", ""),
                        created_at=datetime.fromisoformat(props["created_at"]),
                        base_version=props.get("base_version"),
                        components=components,
                        restored_from=props.get("restored_from"),
                    )
                )
            for rows in self.versions.values():
                rows.sort(key=lambda v: v.number)

    def create_project(self, name: str) -> ProjectRecord:
        record = super().create_project(name)
        with self._driver.session(database=self._database) as session:
            session.run(
                "CREATE (p:V1Project {id:$id, name:$name, created_at:$ca, updated_at:$ua})",
                id=record.id,
                name=record.name,
                ca=record.created_at.isoformat(),
                ua=record.updated_at.isoformat(),
            )
        return record

    def commit_version(  # type: ignore[override]
        self,
        project_id: str,
        message: str,
        author: str,
        components: list[SceneComponent],
        base_version: int | None = None,
        restored_from: int | None = None,
    ) -> V1Version:
        version = super().commit_version(
            project_id, message, author, components, base_version, restored_from
        )
        with self._driver.session(database=self._database) as session:
            session.execute_write(
                lambda tx: tx.run(
                    """
                    CREATE (v:V1Version {project_id:$pid, number:$n, message:$m,
                                        author:$a, created_at:$ca, base_version:$bv,
                                        restored_from:$rf})
                    WITH v
                    UNWIND range(0, size($comps) - 1) AS i
                    MERGE (c:V1Component {hash_sat: $comps[i].hash_sat})
                    SET c.hash_glb = $comps[i].hash_glb, c.name = $comps[i].name,
                        c.color = $comps[i].color, c.bbox_min = $comps[i].bbox_min,
                        c.bbox_max = $comps[i].bbox_max, c.triangles = $comps[i].triangles,
                        c.vertices = $comps[i].vertices,
                        c.hash_graph = CASE WHEN $comps[i].graph <> ''
                                            THEN $comps[i].graph ELSE c.hash_graph END
                    CREATE (v)-[:CONTAINS {order: i}]->(c)
                    """,
                    pid=project_id,
                    n=version.number,
                    m=message,
                    a=author,
                    ca=version.created_at.isoformat(),
                    bv=version.base_version,
                    rf=version.restored_from,
                    comps=[
                        {
                            "hash_sat": c.hash_sat,
                            "hash_glb": c.hash_glb,
                            "name": c.name,
                            "color": c.color,
                            "bbox_min": c.bbox_min,
                            "bbox_max": c.bbox_max,
                            "triangles": c.triangles,
                            "vertices": c.vertices,
                            "graph": c.graph,
                        }
                        for c in components
                    ],
                ).consume()
            )
        return version

    def write_entity_projection(  # type: ignore[override]
        self,
        component_hash: str,
        graph: dict[str, Any],
        parent_hash: str | None = None,
    ) -> dict[str, Any]:
        from .json_graph_native import entity_write_projection, native_label_set_clauses

        with self._driver.session(database=self._database) as session:
            existing = session.run(
                "MATCH (c:V1Component {hash_sat: $h})-[:HAS_ENTITY]->() RETURN 1 AS hit LIMIT 1",
                h=component_hash,
            ).single()
            if existing is not None:
                count = session.run(
                    "MATCH (:V1Component {hash_sat: $h})-[:HAS_ENTITY]->(e) "
                    "RETURN count(e) AS entities",
                    h=component_hash,
                ).single()["entities"]
                return {"written": False, "entities": int(count), "rels": -1,
                        "entity_added": 0, "entity_shared": int(count), "derived": 0}
            projection = normalize_entity_projection(component_hash, graph)
            parent = self._read_projection(session, parent_hash) if parent_hash else None
            matched = match_entities(parent, projection) if parent else {}
            parent_key_by_id = (
                {str(n["id"]): entity_key(n) for n in parent["nodes"]} if parent else {}
            )
            node_rows = []
            chains = []
            for node in projection["nodes"]:
                key = entity_key(node)
                node["entity_key"] = key
                row = entity_write_projection(node)
                row["entity_key"] = key
                node_rows.append(row)
                parent_id = matched.get(str(node["id"]))
                parent_key = parent_key_by_id.get(parent_id) if parent_id else None
                if parent_key and parent_key != key:
                    chains.append({"child": key, "parent": parent_key})
            keys = [r["entity_key"] for r in node_rows]
            existing_rows = session.run(
                "MATCH (e:V1Entity) WHERE e.entity_key IN $keys RETURN e.entity_key AS k",
                keys=keys,
            ).data()
            existing_keys = {r["k"] for r in existing_rows}
            added = len(set(keys) - existing_keys)
            shared = len(keys) - added
            session.execute_write(
                lambda tx: tx.run(
                    """
                    UNWIND $nodes AS item
                    MERGE (e:V1Entity {entity_key: item.entity_key})
                    SET e.labels_json = item.labels_json, e.props_json = item.props_json,
                        e.raw_labels = item.raw_labels, e.native_skipped = item.native_skipped,
                        e.entity_id = coalesce(e.entity_id, item.id),
                        e += item.native_fields
                    """
                    + native_label_set_clauses("item.native_labels")
                    + """
                    WITH item, e
                    MATCH (c:V1Component {hash_sat: $hash})
                    MERGE (c)-[:HAS_ENTITY {entity_id: item.id}]->(e)
                    """,
                    hash=component_hash,
                    nodes=node_rows,
                ).consume()
            )
            if chains:
                session.execute_write(
                    lambda tx: tx.run(
                        """
                        UNWIND $chains AS ch
                        MATCH (a:V1Entity {entity_key: ch.child})
                        MATCH (b:V1Entity {entity_key: ch.parent})
                        MERGE (a)-[:DERIVED_FROM {via_component: $hash}]->(b)
                        """,
                        hash=component_hash,
                        chains=chains,
                    ).consume()
                )
            rel_rows = [
                {"type": str(r.get("type", "")), "start": str(r.get("start", "")), "end": str(r.get("end", ""))}
                for r in projection["rels"]
                if r.get("start") and r.get("end")
            ]
            if rel_rows:
                session.execute_write(
                    lambda tx: tx.run(
                        """
                        UNWIND $rels AS rel
                        MATCH (c:V1Component {hash_sat: $hash})
                        MATCH (c)-[:HAS_ENTITY {entity_id: rel.start}]->(s)
                        MATCH (c)-[:HAS_ENTITY {entity_id: rel.end}]->(t)
                        MERGE (s)-[:TOPOLOGY {rel_type: rel.type, component: $hash}]->(t)
                        """,
                        hash=component_hash,
                        rels=rel_rows,
                    ).consume()
                )
            return {
                "written": True,
                **projection_stats(projection),
                "entity_added": added,
                "entity_shared": shared,
                "derived": len(chains),
            }

    @staticmethod
    def _read_projection(session: Any, component_hash: str) -> dict[str, Any] | None:
        import json as _json

        rows = session.run(
            "MATCH (:V1Component {hash_sat: $h})-[:HAS_ENTITY]->(e) "
            "RETURN e.entity_id AS id, e.labels_json AS labels_json, e.props_json AS props_json",
            h=component_hash,
        ).data()
        if not rows:
            return None
        nodes = []
        for row in rows:
            try:
                labels = _json.loads(row.get("labels_json") or "[]")
                props = _json.loads(row.get("props_json") or "{}")
            except (TypeError, ValueError):
                labels, props = [], {}
            nodes.append({"id": row["id"], "labels": labels, "props": props})
        return {"nodes": nodes, "rels": [], "serializer_build": ""}

    def has_entity_projection(  # type: ignore[override]
        self, component_hash: str
    ) -> bool:
        with self._driver.session(database=self._database) as session:
            hit = session.run(
                "MATCH (c:V1Component {hash_sat: $h})-[:HAS_ENTITY]->()"
                " RETURN 1 AS hit LIMIT 1",
                h=component_hash,
            ).single()
            return hit is not None

    def count_derived_chains(  # type: ignore[override]
        self, component_hashes: list[str]
    ) -> int:
        with self._driver.session(database=self._database) as session:
            row = session.run(
                "MATCH (a:V1Entity)-[d:DERIVED_FROM]->(b:V1Entity) "
                "WHERE d.via_component IN $hashes RETURN count(d) AS n",
                hashes=list(component_hashes),
            ).single()
            return int(row["n"]) if row else 0

    def compact_orphan_entities(  # type: ignore[override]
        self,
    ) -> int:
        with self._driver.session(database=self._database) as session:
            row = session.run(
                "MATCH (e:V1Entity) WHERE NOT EXISTS { MATCH ()-[:HAS_ENTITY]->(e) } "
                "DETACH DELETE e RETURN count(e) AS n"
            ).single()
            return int(row["n"]) if row else 0

    def query_entities(  # type: ignore[override]
        self,
        component_hashes: list[str],
        label: str | None = None,
        limit: int = 1000,
    ) -> list[dict[str, Any]]:
        import json as _json

        clause = "MATCH (c:V1Component)-[:HAS_ENTITY]->(e:V1Entity) WHERE c.hash_sat IN $hashes"
        if label:
            clause += " AND $needle IN [l IN labels(e) | toUpper(l)]"
        clause += (" RETURN c.hash_sat AS component, e.entity_id AS id, e.entity_key AS entity_key,"
                   " e.labels_json AS labels_json, e.props_json AS props_json"
                   " ORDER BY component, id LIMIT $limit")
        with self._driver.session(database=self._database) as session:
            rows = session.run(
                clause,
                hashes=list(component_hashes),
                needle=label.upper() if label else None,
                limit=limit,
            ).data()
        results = []
        for row in rows:
            try:
                labels = _json.loads(row.get("labels_json") or "[]")
                props = _json.loads(row.get("props_json") or "{}")
            except (TypeError, ValueError):
                labels, props = [], {}
            results.append(
                {
                    "component": row["component"],
                    "id": row["id"],
                    "entity_key": str(row.get("entity_key", "")),
                    "labels": labels,
                    "props": props,
                }
            )
        return results


_store: MemoryV1Store | None = None


def get_v1_store() -> MemoryV1Store:
    global _store
    if _store is None:
        raise HTTPException(status.HTTP_503_SERVICE_UNAVAILABLE, "v1 store not initialized")
    return _store


def initialize_v1_store(memory: bool = False) -> None:
    global _store
    if memory:
        _store = MemoryV1Store()
        return
    from .config import settings

    from neo4j import GraphDatabase

    driver = GraphDatabase.driver(
        settings.neo4j_uri, auth=(settings.neo4j_user, settings.neo4j_password)
    )
    store = Neo4jV1Store(driver, settings.neo4j_database)
    store.initialize()
    _store = store


def shutdown_v1_store() -> None:
    global _store
    if _store is not None and isinstance(_store, Neo4jV1Store):
        _store._driver.close()
    _store = None
