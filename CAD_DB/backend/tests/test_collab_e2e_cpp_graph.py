"""E2E 闭环：真实 C++ 序列化器输出 → JsonGraphStore（真 Neo4j）→ 读回等价。

夹具 tests/fixtures/collab/probe_complex_graph.json 由 scripts/tests/acis/
probe_complex.cpp 以 PROBE_DUMP_JSON=1 导出（三部件复杂工图：布尔+圆角+抽壳 /
螺旋扫掠薄壁管[带 sat_fragment 回退字段] / 扭转放样），是 C++ 序列化端的
真实产物而非手工构造——本测试因此闭合了"C++ 编码 → 后端存储 → 读回"的
全链路字面闭环（C++ 侧保真由 L1/L1.5 守门，本测试证明 JSON 穿过 Neo4j
后逐字节等价 + 组件/实体级结构正确 + 回退字段无损）。
"""

import hashlib
import json
import os

import pytest

from app.config import settings
from app.json_collab import canonical_json, validate_graph
from app.json_graph_store import JsonGraphStore

pytestmark = pytest.mark.integration

FIXTURE = os.path.join(os.path.dirname(__file__), "fixtures", "collab", "probe_complex_graph.json")


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


@pytest.fixture()
def store():
    s = _store_or_skip()
    s.initialize()
    yield s
    s.close()


@pytest.fixture()
def project_id(store):
    pid = f"e2e-cpp-{hashlib.sha256(str(store._now()).encode()).hexdigest()[:12]}"
    with store._driver.session(database=store._database) as session:
        session.run("MERGE (p:Project {id: $pid}) SET p.name = $pid", pid=pid)
    yield pid
    with store._driver.session(database=store._database) as session:
        session.run("MATCH (v:JsonGraphVersion {project_id: $pid}) DETACH DELETE v", pid=pid)
        session.run("MATCH (c:JsonGraphComponent {project_id: $pid}) DETACH DELETE c", pid=pid)
        session.run("MATCH (r:JsonGraphRelation {project_id: $pid}) DETACH DELETE r", pid=pid)
        session.run("MATCH (e:JsonGraphEntity) WHERE NOT ()-[:HAS_JSON_ENTITY]->(e) DETACH DELETE e")
        session.run("MATCH (p:Project {id: $pid}) DETACH DELETE p", pid=pid)


def test_cpp_complex_graph_full_chain_roundtrip(store, project_id):
    """真实 C++ 输出的复杂工图：push → Neo4j → pull 投影 SHA 完全一致。"""
    with open(FIXTURE, encoding="utf-8") as f:
        graph = json.load(f)
    canonical = validate_graph(graph)  # C++ 输出必须过后端协议校验
    before_sha = hashlib.sha256(_projection(canonical).encode("utf-8")).hexdigest()

    content = {
        "entity_graph": canonical,
        "changes": [{"uuid": n["id"]} for n in canonical["nodes"] if "body" in n.get("labels", [])],
        "removed_ids": [],
        "schema": "dbcad.entity_graph.v1",
    }
    version = store.create(project_id, "e2e-cpp-test", content, None)

    pulled = store.graph(project_id, version.version)
    after_sha = hashlib.sha256(_projection(pulled).encode("utf-8")).hexdigest()
    assert after_sha == before_sha

    # 结构断言：3 组件（三部件）/ 实体节点数一致 / sat_fragment 回退字段无损
    with store._driver.session(database=store._database) as session:
        row = session.run(
            "MATCH (v:JsonGraphVersion {project_id: $pid}) WITH v "
            "OPTIONAL MATCH (v)-[:HAS_COMPONENT]->(c:JsonGraphComponent) WITH v, c "
            "OPTIONAL MATCH (c)-[:HAS_JSON_ENTITY]->(e:JsonGraphEntity) "
            "RETURN count(DISTINCT c) AS components, count(DISTINCT e) AS entities",
            pid=project_id,
        ).single()
    assert row["components"] == 3
    assert row["entities"] == len(canonical["nodes"])
    frag_nodes = [n for n in pulled["nodes"] if "sat_fragment" in n.get("props", {})]
    assert len(frag_nodes) == 1, "扫掠管的组件级 SAT 片段必须无损穿过 Neo4j"
    assert frag_nodes[0]["props"].get("fallback") == "sat_fragment"
