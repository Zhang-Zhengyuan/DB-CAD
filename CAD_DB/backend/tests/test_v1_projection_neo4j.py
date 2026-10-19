"""L3 真 Neo4j 集成测试（P6/ADR-3 + P8/Phase D）：V1 实体投影。

前提：Docker Neo4j 已运行（deploy/lib/docker.ps1）；连不上自动跳过
（marker=integration）。用例自建 Neo4jV1Store、独立组件哈希，结束后清理
本用例产生的 V1 子图（V1Entity 按 entity_key 定向删除）。

验收对应 V3_PLAN P6/P8：细档原生查询；内容寻址实体共享；DERIVED_FROM
generation 链（改一个 face → 仅新 face 入库并指向父版本 face）。
"""

import pytest

from app.config import settings

pytestmark = pytest.mark.integration


def _store_or_skip():
    if not settings.neo4j_uri:
        pytest.skip("未配置 CAD_DB_NEO4J_URI")
    from neo4j import GraphDatabase

    driver = GraphDatabase.driver(settings.neo4j_uri, auth=(settings.neo4j_user, settings.neo4j_password))
    try:
        driver.verify_connectivity()
    except Exception as exc:  # noqa: BLE001
        driver.close()
        pytest.skip(f"Neo4j 不可用: {exc}")
    from app.v1_store import Neo4jV1Store

    store = Neo4jV1Store(driver, settings.neo4j_database)
    store.initialize()
    return store


def _graph() -> dict:
    return {
        "schema": "dbcad.entity_graph.v2",
        "serializer_build": "uvsafe-topology-v5-bounded",
        "nodes": [
            {
                "id": "temp-root",
                "labels": ["body"],
                "props": {
                    "entity_type": "body",
                    "top_level": True,
                    "uuid": "temp-root",
                    "fallback": "fine_grained",
                },
            },
            {
                "id": "n1",
                "labels": ["face"],
                "props": {
                    "entity_type": "face",
                    "sense": 1,
                    "geometry": {"type": "plane", "normal": [0.0, 0.0, 1.0]},
                },
            },
            {
                "id": "n2",
                "labels": ["face"],
                "props": {
                    "entity_type": "face",
                    "sense": 0,
                    "geometry": {"type": "sphere", "radius": 4.0},
                },
            },
            {
                "id": "n3",
                "labels": ["weird_type"],
                "props": {"entity_type": "weird_type", "payload": {"deep": [1, 2]}},
            },
        ],
        "rels": [
            {"type": "body_lump", "start": "temp-root", "end": "n1"},
            {"type": "body_lump", "start": "temp-root", "end": "n2"},
            {"type": "face_loop", "start": "n1", "end": "n3"},
        ],
    }


def _modified_graph() -> dict:
    """子组件：仅 n1 的几何从 plane 改为 sphere——其余实体内容不变。"""
    graph = _graph()
    for node in graph["nodes"]:
        if node["id"] == "n1":
            node["props"] = {
                "entity_type": "face",
                "sense": 1,
                "geometry": {"type": "cone", "sine_angle": 0.5},
            }
    return graph


def _cleanup(store, project_id: str, *component_hashes: str) -> None:
    with store._driver.session(database=store._database) as session:
        session.run(
            "MATCH (v:V1Version {project_id: $pid}) DETACH DELETE v", pid=project_id
        ).consume()
        # 先删组件（连带 HAS_ENTITY 边），再删无引用实体——顺序颠倒会留下孤魂。
        for component_hash in component_hashes:
            session.run(
                "MATCH (c:V1Component {hash_sat: $h})"
                " WHERE NOT ()-[:CONTAINS]->(c) DETACH DELETE c",
                h=component_hash,
            ).consume()
        session.run(
            "MATCH (e:V1Entity) WHERE e.entity_id IN ['n1', 'n2', 'n3', 'temp-root'] "
            " AND NOT EXISTS { MATCH ()-[:HAS_ENTITY]->(e) } DETACH DELETE e"
        ).consume()
        for component_hash in component_hashes:
            session.run(
                "MATCH (e:V1Entity {entity_id: $h}) "
                " WHERE NOT EXISTS { MATCH ()-[:HAS_ENTITY]->(e) } DETACH DELETE e",
                h=component_hash,
            ).consume()
        session.run(
            "MATCH (p:V1Project {id: $pid}) DETACH DELETE p", pid=project_id
        ).consume()


def test_neo4j_entity_projection_native_labels_and_generation_chain():
    import uuid

    store = _store_or_skip()
    from app.v1_store import SceneComponent

    component_hash = "p6test" + "a" * 58
    child_hash = "p8test" + "b" * 58
    record = store.create_project(f"p6-l3-{uuid.uuid4().hex[:8]}")
    try:
        store.commit_version(
            record.id,
            message="p6 projection",
            author="test",
            components=[SceneComponent(hash_sat=component_hash, hash_glb="glb-x")],
        )
        outcome = store.write_entity_projection(component_hash, _graph())
        assert outcome["written"] is True and outcome["entities"] == 4
        assert outcome["entity_added"] == 4 and outcome["entity_shared"] == 0

        with store._driver.session(database=store._database) as session:
            # 细档验收：原生标签 + 一级扁平字段（geometry.type → geometry_type）
            rows = session.run(
                "MATCH (f:Face) WHERE f.geometry_type = 'plane' AND f.sense = 1 "
                "MATCH (c:V1Component {hash_sat: $h})-[:HAS_ENTITY]->(f) "
                "RETURN f.entity_id AS id",
                h=component_hash,
            ).data()
            assert [r["id"] for r in rows] == ["n1"]

            # 未知类型显式落 :Unknown，绝不静默（raw_labels 保留原值）
            unknown = session.run(
                "MATCH (c:V1Component {hash_sat: $h})-[:HAS_ENTITY]->(u:Unknown) "
                "RETURN u.entity_id AS id, u.raw_labels AS raw",
                h=component_hash,
            ).data()
            assert len(unknown) == 1 and unknown[0]["id"] == "n3"
            assert "weird_type" in unknown[0]["raw"]

            # 拓扑关系统一边型 TOPOLOGY {rel_type}（内容寻址下带 component 归属）
            rels = session.run(
                "MATCH (c:V1Component {hash_sat: $h})-[:HAS_ENTITY]->(s:V1Entity)"
                "-[t:TOPOLOGY]->(e:V1Entity) RETURN t.rel_type AS type ORDER BY type",
                h=component_hash,
            ).data()
            assert [r["type"] for r in rels] == ["body_lump", "body_lump", "face_loop"]

            root = session.run(
                "MATCH (c:V1Component {hash_sat: $h})-[:HAS_ENTITY]->(b:Body) "
                "WHERE b.entity_id = $h RETURN b.uuid AS uuid",
                h=component_hash,
            ).data()
            assert root and root[0]["uuid"] == component_hash

        # 读回等价（L3 口径）
        entities = store.query_entities([component_hash])
        assert len(entities) == 4
        faces = store.query_entities([component_hash], label="face")
        assert {e["id"] for e in faces} == {"n1", "n2"}

        # 幂等：重复写入零新增
        again = store.write_entity_projection(component_hash, _graph())
        assert again["written"] is False

        # P8/Phase D：改一个 face 的子组件 → 内容寻址共享 + DERIVED_FROM 链
        store.commit_version(
            record.id,
            message="modify face",
            author="test",
            components=[SceneComponent(hash_sat=child_hash, hash_glb="glb-y")],
        )
        child = store.write_entity_projection(
            child_hash, _modified_graph(), parent_hash=component_hash
        )
        assert child["entity_added"] == 1  # 只有内容变化的 face 是新实体节点
        assert child["entity_shared"] == 3
        assert child["derived"] == 1
        with store._driver.session(database=store._database) as session:
            chain = session.run(
                "MATCH (a:V1Entity)-[d:DERIVED_FROM]->(b:V1Entity) "
                "WHERE d.via_component = $h RETURN a.entity_id AS new_id, "
                "b.entity_id AS old_id, labels(a) AS labels",
                h=child_hash,
            ).data()
            assert len(chain) == 1 and chain[0]["old_id"] == "n1"
            assert "Face" in chain[0]["labels"]
    finally:
        _cleanup(store, record.id, component_hash, child_hash)
        store._driver.close()
