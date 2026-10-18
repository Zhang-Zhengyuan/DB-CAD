"""L3（Phase B v2）：JsonGraphEntity 原生标签 + 扁平字段的库内结构断言。

前提：真 Neo4j（deploy 容器）可达；连不上自动跳过（marker=integration）。
覆盖（STORAGE_CONSOLIDATION_PLAN Phase B 验收）：
- 原生标签与扁平字段可查（``MATCH (f:Face) WHERE f.geometry_type='spline'``）；
- 同图二次 push 内容寻址去重幂等（实体节点数不变，原生投影仍在）；
- 存量迁移幂等，且 graph() 读投影迁移前后逐字节等价；
- 未识别标签落 :Unknown（显式兜底，不静默）。

每个用例独立 project id，结束后清理本用例子图。
"""

import json
import uuid

import pytest

from app.config import settings
from app.json_collab import validate_graph
from app.json_graph_native import migrate_native_v2
from app.json_graph_store import JsonGraphStore

from test_json_graph_entity_matrix import (  # noqa: E402  (pytest 将 tests 目录加入 sys.path)
    SURFACE_CASES,
    solid_graph,
)

pytestmark = pytest.mark.integration

STORE_LABELS = {"JsonGraphVersion", "JsonGraphComponent", "JsonGraphEntity", "JsonGraphRelation"}


def _store_or_skip() -> JsonGraphStore:
    if not settings.neo4j_uri:
        pytest.skip("未配置 CAD_DB_NEO4J_URI")
    store = JsonGraphStore()
    try:
        store._driver.verify_connectivity()
        store.initialize()  # 顺带验证 v2 索引语句（幂等）
    except Exception as exc:  # noqa: BLE001
        store.close()
        pytest.skip(f"Neo4j 不可用: {exc}")
    return store


def _ensure_project(store: JsonGraphStore, project_id: str) -> None:
    with store._driver.session(database=settings.neo4j_database) as session:
        session.run("MERGE (p:Project {id: $pid}) SET p.name = $pid", pid=project_id).consume()


def _cleanup(store: JsonGraphStore, project_id: str) -> None:
    with store._driver.session(database=settings.neo4j_database) as session:
        session.run("MATCH (p:Project {id: $pid}) DETACH DELETE p", pid=project_id).consume()
        session.run(
            "MATCH (v:JsonGraphVersion {project_id: $project_id})"
            " OPTIONAL MATCH (v)-[:HAS_COMPONENT]->(c:JsonGraphComponent)"
            " OPTIONAL MATCH (c)-[:HAS_JSON_ENTITY]->(e:JsonGraphEntity)"
            " OPTIONAL MATCH (v)-[:HAS_RELATION]->(r:JsonGraphRelation)"
            " DETACH DELETE v, c, e, r",
            project_id=project_id,
        ).consume()


def _canonical(graph: dict) -> str:
    canonical = validate_graph(graph)
    return json.dumps(canonical, ensure_ascii=False, sort_keys=True, separators=(",", ":"))


def _push(store: JsonGraphStore, project_id: str, graph: dict, base: int | None) -> int:
    version = store.create(
        project_id,
        author="native-v2-test",
        content={"entity_graph": graph, "changes": [], "removed_ids": []},
        base_version=base,
    )
    return version.version


@pytest.fixture()
def spline_solid():
    graph = solid_graph(SURFACE_CASES["spline"])
    # 让 geometry_type 断言有区分度：平面 face 与样条 face 并存。
    graph["nodes"].append(
        {
            "id": "f2",
            "labels": ["face"],
            "props": {"sense": -1, "sides": 1, "cont": 0, "geometry": {"type": "plane", "root_point": [0, 0, 0], "normal": [0, 0, 1]}},
        }
    )
    graph["rels"].append({"type": "shell_face", "start": "s1", "end": "f2", "props": {}})
    return graph


def test_native_labels_and_flat_fields(spline_solid):
    store = _store_or_skip()
    project_id = str(uuid.uuid4())
    try:
        _ensure_project(store, project_id)
        _push(store, project_id, spline_solid, None)
        with store._driver.session(database=settings.neo4j_database) as session:
            bodies = session.run(
                "MATCH (p {id: $project_id})-[:HAS_JSON_GRAPH_VERSION]->(v)"
                "-[:HAS_COMPONENT]->(:JsonGraphComponent)-[:HAS_JSON_ENTITY]->(b:Body) "
                "RETURN count(b) AS c",
                project_id=project_id,
            ).single()["c"]
            spline_faces = session.run(
                "MATCH (f:Face) WHERE f.geometry_type = 'spline' RETURN count(f) AS c"
            ).single()["c"]
            plane_faces = session.run(
                "MATCH (f:Face {sense: -1}) WHERE f.geometry_type = 'plane' RETURN count(f) AS c"
            ).single()["c"]
            edges = session.run(
                "MATCH (p {id: $project_id})-[:HAS_JSON_GRAPH_VERSION]->(v)"
                "-[:HAS_COMPONENT]->(:JsonGraphComponent)-[:HAS_JSON_ENTITY]->(e:Edge) "
                "WHERE e.sense = 0 RETURN count(e) AS c",
                project_id=project_id,
            ).single()["c"]
            vertices = session.run(
                "MATCH (p {id: $project_id})-[:HAS_JSON_GRAPH_VERSION]->(v)"
                "-[:HAS_COMPONENT]->(:JsonGraphComponent)-[:HAS_JSON_ENTITY]->(n:Vertex) "
                "RETURN count(n) AS c",
                project_id=project_id,
            ).single()["c"]
            internal_rels = session.run(
                "MATCH (:Body)-[:JSON_TO]->() RETURN count(*) AS c"
            ).single()["c"]
            native_marked = session.run(
                "MATCH (p {id: $project_id})-[:HAS_JSON_GRAPH_VERSION]->(v)"
                "-[:HAS_COMPONENT]->(:JsonGraphComponent)-[:HAS_JSON_ENTITY]->(e:JsonGraphEntity) "
                "WHERE e.native_v2 = true RETURN count(e) AS c",
                project_id=project_id,
            ).single()["c"]
        assert bodies == 1, "Body 原生标签可查"
        assert spline_faces >= 1, "MATCH (f:Face) WHERE f.geometry_type='spline' 可用"
        assert plane_faces == 1, "扁平字段 sense/geometry_type 可查"
        assert edges == 1 and vertices == 2, "Edge/Vertex 原生标签与标量字段可查"
        assert internal_rels >= 1, "JSON_TO 拓扑边保留"
        assert native_marked == len(spline_solid["nodes"]), "全部实体带 native_v2 标记"
    finally:
        _cleanup(store, project_id)
        store.close()


def test_second_push_deduplicates_entities(spline_solid):
    store = _store_or_skip()
    project_id = str(uuid.uuid4())
    try:
        _ensure_project(store, project_id)
        v1 = _push(store, project_id, spline_solid, None)
        # 完全相同的图再次 push：组件内容寻址去重，实体节点不得翻倍。
        v2 = _push(store, project_id, spline_solid, v1)
        with store._driver.session(database=settings.neo4j_database) as session:
            entities = session.run(
                "MATCH (p {id: $project_id})-[:HAS_JSON_GRAPH_VERSION]->(v)"
                "-[:HAS_COMPONENT]->(:JsonGraphComponent)-[:HAS_JSON_ENTITY]->(e:JsonGraphEntity) "
                "RETURN count(DISTINCT e) AS c",
                project_id=project_id,
            ).single()["c"]
            versions = session.run(
                "MATCH (v:JsonGraphVersion {project_id: $project_id}) RETURN count(v) AS c",
                project_id=project_id,
            ).single()["c"]
        assert (v1, v2) == (1, 2) and versions == 2
        assert entities == len(spline_solid["nodes"]), "同图二次 push 实体节点数不变（幂等去重）"
    finally:
        _cleanup(store, project_id)
        store.close()


def test_migration_idempotent_and_graph_equal(spline_solid):
    store = _store_or_skip()
    project_id = str(uuid.uuid4())
    try:
        _ensure_project(store, project_id)
        _push(store, project_id, spline_solid, None)
        before = _canonical(store.graph(project_id, 1))
        with store._driver.session(database=settings.neo4j_database) as session:
            # 模拟存量库：用 map 投影彻底还原为 v1 形态（只留权威键），
            # 再剥掉全部原生标签。
            session.run(
                "MATCH (e:JsonGraphEntity) "
                "SET e = {labels_json: e.labels_json, props_json: e.props_json, component_hash: e.component_hash, entity_id: e.entity_id}"
            ).consume()
            for label in ("Body", "Lump", "Shell", "Subshell", "Face", "Loop", "Coedge", "Edge", "Vertex", "Wire", "Transform", "Unknown"):
                session.run(f"MATCH (e:JsonGraphEntity) REMOVE e:{label}").consume()
            report1 = migrate_native_v2(session)
            assert report1["migrated"] == len(spline_solid["nodes"]), "存量节点全部补齐"
            report2 = migrate_native_v2(session)
            assert report2["migrated"] == 0, "迁移幂等（重跑零处理）"
            spline_faces = session.run(
                "MATCH (f:Face) WHERE f.geometry_type = 'spline' RETURN count(f) AS c"
            ).single()["c"]
        after = _canonical(store.graph(project_id, 1))
        assert before == after, "迁移前后 graph() 读投影逐字节等价"
        assert spline_faces >= 1, "迁移后原生查询可用"
    finally:
        _cleanup(store, project_id)
        store.close()


def test_unknown_label_falls_back_explicitly():
    store = _store_or_skip()
    project_id = str(uuid.uuid4())
    graph = {
        "nodes": [
            {"id": "b1", "labels": ["body"], "props": {"entity_type": "body"}},
            {
                "id": "weird",
                "labels": ["future-thing"],
                "props": {"payload": {"deep": {"x": 1}}, "kind": 7},
            },
        ],
        "rels": [{"type": "body_lump", "start": "b1", "end": "weird", "props": {}}],
    }
    try:
        _ensure_project(store, project_id)
        _push(store, project_id, graph, None)
        with store._driver.session(database=settings.neo4j_database) as session:
            unknown = session.run(
                "MATCH (u:Unknown) WHERE u.kind = 7 RETURN count(u) AS c"
            ).single()["c"]
            raw = session.run(
                "MATCH (e:JsonGraphEntity {entity_id: 'weird'}) RETURN e.raw_labels AS raw, e.native_skipped AS skipped, e.props_json AS props"
            ).single()
        assert unknown == 1, "未识别标签显式落 :Unknown"
        assert json.loads(raw["raw"]) == ["future-thing"], "原始标签保留"
        assert int(raw["skipped"]) >= 1, "复杂嵌套值计入 skipped 统计"
        assert json.loads(raw["props"])["payload"] == {"deep": {"x": 1}}, "权威载荷无损"
    finally:
        _cleanup(store, project_id)
        store.close()


def test_version_tags():
    """M4.3 定版：命名标签创建/列举/移动（微版本链即自动同步）。"""
    store = _store_or_skip()
    project_id = str(uuid.uuid4())
    try:
        _ensure_project(store, project_id)
        v1 = _push(store, project_id, solid_graph(SURFACE_CASES["plane"]), None)
        with store._driver.session(database=settings.neo4j_database) as session:
            session.run(
                "CREATE CONSTRAINT json_graph_tag_unique IF NOT EXISTS "
                "FOR (t:JsonGraphTag) REQUIRE (t.project_id, t.name) IS UNIQUE"
            ).consume()
        store.tag_version(project_id, "release-1", v1)
        tags = store.list_tags(project_id)
        assert tags == [{"name": "release-1", "version": v1, "created_at": tags[0]["created_at"]}]
        # 同名重打 = 移动标签指向（新版本）。
        v2 = _push(store, project_id, solid_graph(SURFACE_CASES["sphere"]), v1)
        store.tag_version(project_id, "release-1", v2)
        tags = store.list_tags(project_id)
        assert len(tags) == 1 and tags[0]["version"] == v2
        # 不存在的版本 → 404。
        from fastapi import HTTPException

        with pytest.raises(HTTPException):
            store.tag_version(project_id, "ghost", 999)
    finally:
        with store._driver.session(database=settings.neo4j_database) as session:
            session.run(
                "MATCH (t:JsonGraphTag {project_id: $pid}) DETACH DELETE t", pid=project_id
            ).consume()
        _cleanup(store, project_id)
        store.close()
