"""L3 真 Neo4j 集成测试：实体图 push → Neo4j → pull 完全一致性（SHA 校验）。

前提：Docker Neo4j（deploy/lib/docker.ps1 管理的 neo4j-apoc 容器）已运行；
连不上自动跳过（marker=integration）。每个用例使用独立 project id，
结束后清理本用例产生的版本/组件/实体/关系子图，不触碰既有数据。

一致性口径：push 前与 pull 后的图做规范化投影（节点/关系按 canonical JSON
排序，保留原始 id 与 props），SHA256 必须逐字节一致。
"""

import hashlib

import pytest

from app.config import settings
from app.json_collab import canonical_json, validate_graph
from app.json_graph_store import JsonGraphStore

from test_json_graph_entity_matrix import (  # noqa: E402  (pytest 将 tests 目录加入 sys.path)
    CURVE_CASES,
    SURFACE_CASES,
    solid_graph,
)

pytestmark = pytest.mark.integration


def _store_or_skip() -> JsonGraphStore:
    if not settings.neo4j_uri:
        pytest.skip("未配置 CAD_DB_NEO4J_URI")
    store = JsonGraphStore()
    try:
        store._driver.verify_connectivity()
    except Exception as exc:  # noqa: BLE001
        store.close()
        pytest.skip(f"Neo4j 不可用: {exc}")
    return store


def _projection(graph: dict) -> str:
    """规范化投影：节点/关系排序后拼接，保留 id/labels/props/type。"""
    canonical = validate_graph(graph)
    nodes = sorted(
        ({"id": n["id"], "labels": n.get("labels", []), "props": n.get("props", {})} for n in canonical["nodes"]),
        key=canonical_json,
    )
    rels = sorted(
        (
            {"type": r["type"], "start": r["start"], "end": r["end"], "props": r.get("props", {})}
            for r in canonical["rels"]
        ),
        key=canonical_json,
    )
    return canonical_json({"nodes": nodes, "rels": rels})


def _sha(projection: str) -> str:
    return hashlib.sha256(projection.encode("utf-8")).hexdigest()


@pytest.fixture()
def store():
    s = _store_or_skip()
    s.initialize()
    yield s
    s.close()


@pytest.fixture()
def project_id(store):
    pid = f"l3-sha-{hashlib.sha256(str(store._now()).encode()).hexdigest()[:12]}"
    with store._driver.session(database=store._database) as session:
        session.run(
            "MERGE (p:Project {id: $pid}) SET p.name = $pid",
            pid=pid,
        )
    yield pid
    with store._driver.session(database=store._database) as session:
        session.run(
            "MATCH (v:JsonGraphVersion {project_id: $pid}) DETACH DELETE v",
            pid=pid,
        )
        session.run(
            "MATCH (c:JsonGraphComponent {project_id: $pid}) DETACH DELETE c",
            pid=pid,
        )
        session.run(
            "MATCH (r:JsonGraphRelation {project_id: $pid}) DETACH DELETE r",
            pid=pid,
        )
        session.run(
            "MATCH (e:JsonGraphEntity) WHERE NOT ()-[:HAS_JSON_ENTITY]->(e) DETACH DELETE e"
        )
        session.run(
            "MATCH (p:Project {id: $pid}) DETACH DELETE p",
            pid=pid,
        )


def _push(store, project_id, graph, changes=None):
    content = {
        "entity_graph": graph,
        "changes": changes if changes is not None else [{"uuid": "b1"}],
        "removed_ids": [],
        "schema": "dbcad.entity_graph.v1",
    }
    latest = store.latest(project_id)
    base = latest.version if latest else None
    return store.create(project_id, "l3-test", content, base)


def _counts(store, project_id):
    with store._driver.session(database=store._database) as session:
        row = session.run(
            "MATCH (v:JsonGraphVersion {project_id: $pid}) WITH v "
            "OPTIONAL MATCH (v)-[:HAS_COMPONENT]->(c:JsonGraphComponent) WITH v, c "
            "OPTIONAL MATCH (c)-[:HAS_JSON_ENTITY]->(e:JsonGraphEntity) WITH v, c, e "
            "OPTIONAL MATCH (e)-[t:JSON_TO]->() WITH v, c, e, t "
            "OPTIONAL MATCH (v)-[:HAS_RELATION]->(r:JsonGraphRelation) "
            "RETURN count(DISTINCT v) AS versions, count(DISTINCT c) AS components, "
            "count(DISTINCT e) AS entities, count(DISTINCT t) AS internal_rels, "
            "count(DISTINCT r) AS cross_rels",
            pid=project_id,
        ).single()
    return dict(row)


ALL_CASES = {f"surface:{k}": v for k, v in SURFACE_CASES.items()} | {
    f"curve:{k}": v for k, v in CURVE_CASES.items()
}


@pytest.mark.parametrize("case", sorted(ALL_CASES))
def test_entity_graph_db_roundtrip_sha(store, project_id, case):
    """逐实体：push → Neo4j 结构断言 → pull → SHA 完全一致。"""
    graph = solid_graph(ALL_CASES[case])
    version = _push(store, project_id, graph)
    assert version.version == 1

    counts = _counts(store, project_id)
    assert counts["versions"] == 1
    assert counts["components"] == 1
    assert counts["entities"] == len(graph["nodes"])
    assert counts["internal_rels"] == len(graph["rels"])
    assert counts["cross_rels"] == 0

    pulled = store.graph(project_id, 1)
    assert _sha(_projection(pulled)) == _sha(_projection(graph)), (
        f"{case}: DB 往返 SHA 不一致\npush={_projection(graph)[:400]}\npull={_projection(pulled)[:400]}"
    )


def test_same_graph_second_push_dedup(store, project_id):
    """同图二次 push：组件/实体按内容寻址去重，SHA 仍一致。"""
    graph = solid_graph(SURFACE_CASES["sphere"])
    _push(store, project_id, graph)
    _push(store, project_id, graph)

    counts = _counts(store, project_id)
    assert counts["versions"] == 2
    assert counts["components"] == 1
    assert counts["entities"] == len(graph["nodes"])

    pulled = store.graph(project_id, 2)
    assert _sha(_projection(pulled)) == _sha(_projection(graph))


def test_component_change_replaces_whole_component(store, project_id):
    """改一个字段：当前语义为整组件替换（组件级粒度基线，Phase D 细化）。"""
    import copy

    v1_graph = solid_graph(SURFACE_CASES["sphere"])
    _push(store, project_id, v1_graph)

    v2_graph = copy.deepcopy(v1_graph)
    for n in v2_graph["nodes"]:
        if n["labels"] == ["face"]:
            n["props"]["geometry"]["radius"] = 6.0
    _push(store, project_id, v2_graph)

    counts = _counts(store, project_id)
    assert counts["components"] == 2
    assert counts["entities"] == 2 * len(v1_graph["nodes"])  # 组件级：整体替换

    assert _sha(_projection(store.graph(project_id, 1))) == _sha(_projection(v1_graph))
    assert _sha(_projection(store.graph(project_id, 2))) == _sha(_projection(v2_graph))


def test_cross_component_relation_survives(store, project_id):
    """双 body + 跨组件边：拆分、关系节点、重组后 SHA 一致。"""
    first = solid_graph(SURFACE_CASES["sphere"])
    second = solid_graph(SURFACE_CASES["torus"])
    for item in second["nodes"]:
        item["id"] += "-2"
        if item["labels"] == ["body"]:
            item["props"] = {"uuid": "b2"}
    for item in second["rels"]:
        item["start"] += "-2"
        item["end"] += "-2"
    graph = {
        "nodes": first["nodes"] + second["nodes"],
        "rels": first["rels"] + second["rels"] + [{"type": "mate_edge", "start": "e1", "end": "e1-2", "props": {}}],
    }
    _push(store, project_id, graph)

    counts = _counts(store, project_id)
    assert counts["components"] == 2
    assert counts["cross_rels"] == 1

    pulled = store.graph(project_id, 1)
    assert _sha(_projection(pulled)) == _sha(_projection(graph))
