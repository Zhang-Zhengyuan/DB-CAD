"""撤销来源的真实 Neo4j 往返：只连接显式指定的隔离测试数据库。"""

import os
from uuid import uuid4

import pytest
from fastapi.testclient import TestClient
from neo4j import GraphDatabase

from app import main, v1_store
from app.v1_store import Neo4jV1Store, SceneComponent

pytestmark = pytest.mark.integration


@pytest.mark.parametrize("last_action", ["undo", "redo"])
def test_undo_continues_after_store_reload(monkeypatch, last_action):
    uri = os.environ.get("CAD_DB_UNDO_TEST_NEO4J_URI")
    if not uri:
        pytest.skip("需要显式指定 CAD_DB_UNDO_TEST_NEO4J_URI（隔离测试库）")
    password = os.environ.get("CAD_DB_UNDO_TEST_NEO4J_PASSWORD")
    auth = ("neo4j", password) if password else None
    driver = GraphDatabase.driver(uri, auth=auth)
    store = Neo4jV1Store(driver, "neo4j")
    project_id = None
    hashes = [f"undo-test-{uuid4().hex}" for _ in range(3)]
    try:
        store.initialize()
        project_id = store.create_project(f"undo-reload-{uuid4().hex}").id
        monkeypatch.setattr(v1_store, "_store", store)
        client = TestClient(main.app)
        snapshots = []
        for count in range(1, 4):
            version = store.commit_version(
                project_id, "component.create", "test",
                [SceneComponent(h, f"glb-{h}") for h in hashes[:count]],
                base_version=count - 1 if count > 1 else None,
            )
            snapshots.append([c.to_scene_json() for c in version.components])
        assert client.post(f"/api/v1/projects/{project_id}/undo").status_code == 200
        if last_action == "redo":
            assert client.post(f"/api/v1/projects/{project_id}/redo").status_code == 200
        source = 2 if last_action == "undo" else 3

        # 模拟重启：从真实库重新加载版本；进程内 redo 栈按原契约不持久化。
        reloaded = Neo4jV1Store(driver, "neo4j")
        reloaded.initialize()
        monkeypatch.setattr(v1_store, "_store", reloaded)
        assert reloaded.get_version(project_id, 1).restored_from is None
        assert reloaded.latest(project_id).restored_from == source
        assert reloaded.latest(project_id).base_version == (3 if source == 2 else 4)
        assert client.post(f"/api/v1/projects/{project_id}/redo").status_code == 409
        for expected in reversed(snapshots[:source - 1]):
            response = client.post(f"/api/v1/projects/{project_id}/undo")
            assert response.status_code == 200, response.text
            scene = client.get(f"/api/v1/projects/{project_id}/scene").json()
            assert scene["components"] == expected
        assert client.post(f"/api/v1/projects/{project_id}/undo").status_code == 409
    finally:
        if project_id:
            with driver.session(database="neo4j") as session:
                session.run("MATCH (v:V1Version {project_id:$pid}) DETACH DELETE v", pid=project_id).consume()
                session.run("MATCH (p:V1Project {id:$pid}) DETACH DELETE p", pid=project_id).consume()
                session.run(
                    "MATCH (c:V1Component) WHERE c.hash_sat IN $hashes "
                    "AND NOT ()-[:CONTAINS]->(c) DETACH DELETE c", hashes=hashes,
                ).consume()
        driver.close()
