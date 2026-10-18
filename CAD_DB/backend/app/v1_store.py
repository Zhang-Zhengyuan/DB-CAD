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

from .neo4j_store import ProjectRecord


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

    def to_scene_json(self) -> dict[str, Any]:
        return {
            "component_id": self.hash_sat,
            "name": self.name,
            "sat": self.hash_sat,
            "glb": self.hash_glb,
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


def _now() -> datetime:
    return datetime.now(UTC)


class MemoryV1Store:
    def __init__(self) -> None:
        self.projects: dict[str, ProjectRecord] = {}
        self.versions: dict[str, list[V1Version]] = {}
        self.tags: dict[str, dict[str, int]] = {}
        self.branches: dict[str, dict[str, int]] = {}
        # 撤销栈（进程内）：undo 压入被撤销清单，redo 弹出重放。
        # 不随 Neo4j 持久化——服务重启后 redo 清空是可接受语义。
        self.redo_stacks: dict[str, list[list[SceneComponent]]] = {}

    def push_redo(self, project_id: str, components: list[SceneComponent]) -> None:
        self.redo_stacks.setdefault(project_id, []).append(list(components))

    def pop_redo(self, project_id: str) -> list[SceneComponent] | None:
        stack = self.redo_stacks.get(project_id)
        if not stack:
            return None
        return stack.pop()

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
    ) -> V1Version:
        version = super().commit_version(project_id, message, author, components, base_version)
        with self._driver.session(database=self._database) as session:
            session.execute_write(
                lambda tx: tx.run(
                    """
                    CREATE (v:V1Version {project_id:$pid, number:$n, message:$m,
                                        author:$a, created_at:$ca, base_version:$bv})
                    WITH v
                    UNWIND range(0, size($comps) - 1) AS i
                    MERGE (c:V1Component {hash_sat: $comps[i].hash_sat})
                    SET c.hash_glb = $comps[i].hash_glb, c.name = $comps[i].name,
                        c.color = $comps[i].color, c.bbox_min = $comps[i].bbox_min,
                        c.bbox_max = $comps[i].bbox_max, c.triangles = $comps[i].triangles,
                        c.vertices = $comps[i].vertices
                    CREATE (v)-[:CONTAINS {order: i}]->(c)
                    """,
                    pid=project_id,
                    n=version.number,
                    m=message,
                    a=author,
                    ca=version.created_at.isoformat(),
                    bv=version.base_version,
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
                        }
                        for c in components
                    ],
                ).consume()
            )
        return version


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
