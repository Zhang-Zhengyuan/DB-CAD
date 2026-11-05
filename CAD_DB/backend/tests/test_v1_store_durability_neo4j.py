"""L3 持久性集成测试（Issue #10）：rename/delete/tag 的 Neo4j 写穿。

前提：Docker Neo4j 已运行；连不上自动跳过（marker=integration）。
"重启"用新建 Neo4jV1Store + initialize() 模拟（等价启动全量预载）。
用例自建随机项目名与组件哈希，结束后裸 Cypher 清理，不依赖被测代码。
"""

import secrets

import pytest

from app.config import settings

pytestmark = pytest.mark.integration


def _driver_or_skip():
    if not settings.neo4j_uri:
        pytest.skip("未配置 CAD_DB_NEO4J_URI")
    from neo4j import GraphDatabase

    driver = GraphDatabase.driver(
        settings.neo4j_uri, auth=(settings.neo4j_user, settings.neo4j_password)
    )
    try:
        driver.verify_connectivity()
    except Exception as exc:  # noqa: BLE001
        driver.close()
        pytest.skip(f"Neo4j 不可用: {exc}")
    return driver


def _store(driver):
    from app.v1_store import Neo4jV1Store

    store = Neo4jV1Store(driver, settings.neo4j_database)
    store.initialize()
    return store


def _cleanup(driver, project_id: str, comp_hash: str | None = None) -> None:
    with driver.session(database=settings.neo4j_database) as session:
        session.run(
            "MATCH (v:V1Version {project_id:$pid}) DETACH DELETE v", pid=project_id
        ).consume()
        session.run(
            "MATCH (t:V1Tag {project_id:$pid}) DELETE t", pid=project_id
        ).consume()
        session.run(
            "MATCH (p:V1Project {id:$pid}) DETACH DELETE p", pid=project_id
        ).consume()
        if comp_hash:
            session.run(
                "MATCH (c:V1Component {hash_sat:$h}) DETACH DELETE c", h=comp_hash
            ).consume()


def test_rename_survives_restart():
    driver = _driver_or_skip()
    store = _store(driver)
    name = f"dur-rename-{secrets.token_hex(4)}"
    record = store.create_project(name)
    try:
        store.rename_project(record.id, name + "-new")
        reloaded = _store(driver)
        assert reloaded.get_project_or_none(record.id).name == name + "-new"
    finally:
        _cleanup(driver, record.id)
        driver.close()


def test_delete_survives_restart_and_keeps_components():
    driver = _driver_or_skip()
    from app.v1_store import SceneComponent

    store = _store(driver)
    name = f"dur-delete-{secrets.token_hex(4)}"
    comp_hash = f"dur-delete-{secrets.token_hex(8)}-comp"
    record = store.create_project(name)
    try:
        store.commit_version(
            record.id, "init", "test", [SceneComponent(comp_hash, f"glb-{comp_hash}")]
        )
        store.tag_version(record.id, "guard", 1)
        store.delete_project(record.id)
        reloaded = _store(driver)
        assert reloaded.get_project_or_none(record.id) is None
        assert reloaded.get_version(record.id, 1) is None
        with driver.session(database=settings.neo4j_database) as session:
            tags = session.run(
                "MATCH (t:V1Tag {project_id:$pid}) RETURN count(t) AS n",
                pid=record.id,
            ).single()["n"]
            comps = session.run(
                "MATCH (c:V1Component {hash_sat:$h}) RETURN count(c) AS n",
                h=comp_hash,
            ).single()["n"]
        assert int(tags) == 0
        # 组件内容寻址、跨项目共享（ADR-3）：删除项目不删组件。
        assert int(comps) == 1
    finally:
        _cleanup(driver, record.id, comp_hash)
        driver.close()


def test_tag_survives_restart_and_overwrites():
    driver = _driver_or_skip()
    from app.v1_store import SceneComponent

    store = _store(driver)
    name = f"dur-tag-{secrets.token_hex(4)}"
    comp_hash = f"dur-tag-{secrets.token_hex(8)}-comp"
    record = store.create_project(name)
    try:
        component = SceneComponent(comp_hash, f"glb-{comp_hash}")
        store.commit_version(record.id, "v1", "test", [component])
        store.commit_version(
            record.id, "v2", "test", [component], base_version=1
        )
        store.tag_version(record.id, "定版", 1)
        store.tag_version(record.id, "定版", 2)  # 同名重打 → 覆盖
        reloaded = _store(driver)
        assert reloaded.list_tags(record.id) == [{"name": "定版", "version": 2}]
        with driver.session(database=settings.neo4j_database) as session:
            n = session.run(
                "MATCH (t:V1Tag {project_id:$pid, name:$tag}) RETURN count(t) AS n",
                pid=record.id,
                tag="定版",
            ).single()["n"]
        assert int(n) == 1  # MERGE 不产生重复节点
    finally:
        _cleanup(driver, record.id, comp_hash)
        driver.close()
