"""v1 API 契约测试（V3_PLAN.md §2 / P1 验收）。

验收链路：建项目 → 导 SAT → scene/asset 可取 → 定版/版本详情 → 分支 →
导出 → WS 事件 → 409 乐观锁。worker 用内存假件（真实 C++ worker 的
manifest/GLB 结构由 mesh_tests + dbcad_worker selftest 锁定）。
"""

from __future__ import annotations

import hashlib
from pathlib import Path

import pytest
from fastapi.testclient import TestClient

from app import main
from app.component_store import ComponentStore, set_component_store
from app.v1_store import MemoryV1Store, initialize_v1_store, shutdown_v1_store
from app.worker_client import ImportResult, ImportedComponent, set_worker_gateway


class FakeWorker:
    """产出固定几何字节的假 worker：SAT 任意字节，GLB 带合法头。"""

    def __init__(self) -> None:
        self.import_calls: list[bytes] = []

    def import_sat(self, sat_bytes: bytes) -> ImportResult:
        self.import_calls.append(sat_bytes)
        return ImportResult(
            components=[
                ImportedComponent(
                    sat=sat_bytes,
                    glb=b"glTF-fake-body-0",
                    bbox_min=[-1.0, -2.0, -3.0],
                    bbox_max=[4.0, 5.0, 6.0],
                    color=[0.1, 0.2, 0.3],
                    triangles=42,
                    vertices=21,
                )
            ]
        )

    def merge_sat(self, parts: list[bytes]) -> bytes:
        return b"MERGED::" + b"".join(parts)

    def export_glb(self, parts: list[bytes]) -> bytes:
        return b"glTF-merged"


@pytest.fixture()
def client(tmp_path: Path, monkeypatch: pytest.MonkeyPatch):
    # 不进入 TestClient 上下文 ⇒ 不触发 lifespan ⇒ 不触碰真实 Neo4j
    #（与既有 v2 测试同一封闭模式），存储/worker 全部为进程内替身。
    initialize_v1_store(memory=True)
    set_component_store(ComponentStore(tmp_path / "components"))
    fake = FakeWorker()
    set_worker_gateway(fake)  # type: ignore[arg-type]
    yield TestClient(main.app), fake
    set_worker_gateway(None)
    set_component_store(None)
    shutdown_v1_store()


SAT_BYTES = b"fake-sat-content"


def _import_project(client: TestClient, name: str = "demo") -> dict:
    response = client.post(
        "/api/v1/files/import",
        files={"file": ("model.sat", SAT_BYTES, "application/octet-stream")},
        data={"project_name": name},
    )
    assert response.status_code == 201, response.text
    return response.json()


def test_contract_paths_are_frozen():
    """计划表 §2 的全部端点必须在 OpenAPI 里（契约冻结守门）。"""
    spec = TestClient(main.app).get("/openapi.json").json()
    paths = set(spec["paths"])
    expected = {
        "/api/v1/projects",
        "/api/v1/projects/{project_id}",
        "/api/v1/projects/{project_id}/versions",
        "/api/v1/projects/{project_id}/versions/{version}",
        "/api/v1/projects/{project_id}/branches",
        "/api/v1/projects/{project_id}/scene",
        "/api/v1/projects/{project_id}/commands",
        "/api/v1/projects/{project_id}/commands/dry-run",
        "/api/v1/projects/{project_id}/undo",
        "/api/v1/projects/{project_id}/redo",
        "/api/v1/projects/{project_id}/files/export",
        "/api/v1/assets/{asset}",
        "/api/v1/files/import",
        "/api/v1/schema/commands",
    }
    missing = expected - paths
    assert not missing, f"v1 contract paths missing from OpenAPI: {missing}"


def test_project_lifecycle(client):
    client, _fake = client
    created = client.post("/api/v1/projects", json={"name": "alpha"}).json()
    assert created["head_version"] is None
    listed = client.get("/api/v1/projects").json()
    assert any(p["id"] == created["id"] for p in listed)
    renamed = client.patch(f"/api/v1/projects/{created['id']}", json={"name": "beta"})
    assert renamed.status_code == 200 and renamed.json()["name"] == "beta"
    dup = client.post("/api/v1/projects", json={"name": "beta"})
    assert dup.status_code == 409
    assert client.delete(f"/api/v1/projects/{created['id']}").status_code == 204
    assert client.get(f"/api/v1/projects/{created['id']}").status_code == 404


def test_import_creates_version_and_components(client):
    client, fake = client
    assert fake.import_calls == []
    result = _import_project(client)
    assert result["version"] == 1
    assert len(result["components"]) == 1
    component = result["components"][0]
    assert component["triangles"] == 42
    assert component["bbox"] == {"min": [-1.0, -2.0, -3.0], "max": [4.0, 5.0, 6.0]}
    assert fake.import_calls == [SAT_BYTES]

    # 内容寻址：hash 即 sha256，且资产端点可取、强缓存。
    digest = hashlib.sha256(SAT_BYTES).hexdigest()
    assert component["sat"] == digest
    sat = client.get(f"/api/v1/assets/{digest}.sat")
    assert sat.status_code == 200 and sat.content == SAT_BYTES
    assert sat.headers["cache-control"].endswith("immutable")
    assert sat.headers["etag"] == f'"{digest}"'
    glb = client.get(f"/api/v1/assets/{component['glb']}.glb")
    assert glb.status_code == 200 and glb.content == b"glTF-fake-body-0"

    scene = client.get(f"/api/v1/projects/{result['project_id']}/scene").json()
    assert scene["version"] == 1 and scene["components"] == [component]


def test_import_requires_exactly_one_project_reference(client):
    client, _fake = client
    both = client.post(
        "/api/v1/files/import",
        files={"file": ("m.sat", SAT_BYTES, "application/octet-stream")},
        data={"project_name": "x", "project_id": "y"},
    )
    assert both.status_code == 422
    neither = client.post(
        "/api/v1/files/import",
        files={"file": ("m.sat", SAT_BYTES, "application/octet-stream")},
    )
    assert neither.status_code == 422
    unknown = client.post(
        "/api/v1/files/import",
        files={"file": ("m.sat", SAT_BYTES, "application/octet-stream")},
        data={"project_id": "missing"},
    )
    assert unknown.status_code == 404


def test_second_import_appends_version_and_dedupes_components(client):
    client, _fake = client
    first = _import_project(client, "chain")
    project_id = first["project_id"]
    second = client.post(
        "/api/v1/files/import",
        files={"file": ("again.sat", SAT_BYTES, "application/octet-stream")},
        data={"project_id": project_id},
    ).json()
    assert second["version"] == 2
    assert second["components"][0]["sat"] == first["components"][0]["sat"]
    detail = client.get(f"/api/v1/projects/{project_id}/versions/2").json()
    assert detail["base_version"] == 1 and detail["component_count"] == 1
    listed = client.get(f"/api/v1/projects/{project_id}/versions").json()
    assert [v["version"] for v in listed["versions"]] == [1, 2]


def test_tags_branches_and_scene_by_version(client):
    client, _fake = client
    imported = _import_project(client, "tagged")
    project_id = imported["project_id"]
    tag = client.post(
        f"/api/v1/projects/{project_id}/versions",
        json={"name": "v1.0", "version": 1, "author": "qa"},
    )
    assert tag.status_code == 201
    assert client.get(f"/api/v1/projects/{project_id}/versions").json()["tags"] == [
        {"name": "v1.0", "version": 1}
    ]
    branch = client.post(
        f"/api/v1/projects/{project_id}/branches",
        json={"name": "experiment", "from_version": 1},
    )
    assert branch.status_code == 201
    branches = client.get(f"/api/v1/projects/{project_id}/branches").json()["branches"]
    assert {"name": "experiment", "head_version": 1} in branches
    old_scene = client.get(f"/api/v1/projects/{project_id}/scene?v=1").json()
    assert old_scene["version"] == 1
    assert client.get(f"/api/v1/projects/{project_id}/scene?v=99").status_code == 404


def test_export_sat_and_glb(client):
    client, _fake = client
    imported = _import_project(client, "export-me")
    project_id = imported["project_id"]
    sat = client.get(f"/api/v1/projects/{project_id}/files/export?fmt=sat")
    assert sat.status_code == 200 and sat.content == b"MERGED::" + SAT_BYTES
    glb = client.get(f"/api/v1/projects/{project_id}/files/export?fmt=glb&v=1")
    assert glb.status_code == 200 and glb.content == b"glTF-merged"
    stl = client.get(f"/api/v1/projects/{project_id}/files/export?fmt=stl")
    assert stl.status_code == 501


def test_command_schema_endpoint_contract(client):
    """命令端点自 P2 起真实实现（见 test_api_v1_commands.py）；此处锁定
    schema 目录的契约形状。"""
    client, _fake = client
    schema = client.get("/api/v1/schema/commands").json()
    assert schema["schema"] == "dbcad.commands/1"
    assert "component.create" in schema["commands"]
    assert "boolean.unite" in schema["commands"]


def _receive_until(ws, event_type: str, limit: int = 10) -> dict:
    """跳过 presence 等伴随事件，等到目标类型。"""
    for _ in range(limit):
        event = ws.receive_json()
        if event["type"] == event_type:
            return event
    raise AssertionError(f"event {event_type} not received")


def test_websocket_receives_commit_events(client):
    client, _fake = client
    with client.websocket_connect("/api/v1/events?projects=p1&user=alice") as ws:
        assert ws.receive_json()["type"] == "subscribed"
        join = _receive_until(ws, "presence")
        assert join["action"] == "join" and join["user"] == "alice"
        assert join["users"] == ["alice"]
        _import_project(client, "ws-project")
        # p1 未订阅本项目：该项目不会收到 version 事件（超时由测试框架抛）。
    created = client.get("/api/v1/projects").json()
    target = next(p for p in created if p["name"] == "ws-project")
    with client.websocket_connect(f"/api/v1/events?projects={target['id']}&user=bob") as ws:
        assert ws.receive_json()["type"] == "subscribed"
        second = client.post(
            "/api/v1/files/import",
            files={"file": ("m.sat", SAT_BYTES, "application/octet-stream")},
            data={"project_id": target["id"]},
        )
        assert second.status_code == 201
        event = _receive_until(ws, "version.committed")
        assert event["version"] == 2


def test_websocket_edit_intent_broadcast(client):
    client, _fake = client
    project_id = _import_project(client, "intent")["project_id"]
    with client.websocket_connect(f"/api/v1/events?projects={project_id}&user=alice") as ws:
        ws.receive_json()
        _receive_until(ws, "presence")
        with client.websocket_connect(
            f"/api/v1/events?projects={project_id}&user=bob"
        ) as bob_ws:
            bob_ws.receive_json()
            seen = _receive_until(ws, "presence")
            assert set(seen["users"]) == {"alice", "bob"}
            bob_ws.send_json({"type": "edit_intent", "component": "comp-x"})
            intent = _receive_until(ws, "edit_intent")
            assert intent["user"] == "bob"
            assert intent["intents"] == {"bob": "comp-x"}


def test_component_store_content_addressing(tmp_path: Path):
    store = ComponentStore(tmp_path / "cs")
    digest = store.put("sat", b"payload")
    assert digest == hashlib.sha256(b"payload").hexdigest()
    assert store.put("sat", b"payload") == digest  # 幂等
    assert store.read("sat", digest) == b"payload"
    assert store.read("sat", "0" * 64) is None
    with pytest.raises(ValueError):
        store.put("exe", b"x")


def test_memory_store_optimistic_lock():
    store = MemoryV1Store()
    project = store.create_project("lock")
    component = store.commit_version(project.id, "v1", "a", [])
    assert component.number == 1
    stale = store.commit_version(project.id, "v2", "b", [], base_version=1)
    assert stale.number == 2
    from app.v1_store import VersionConflict

    with pytest.raises(VersionConflict) as conflict:
        store.commit_version(project.id, "v3", "c", [], base_version=1)
    assert conflict.value.detail["reason"] == "stale_base_version"
    assert conflict.value.detail["current_head"] == 2
    fresh = store.commit_version(project.id, "v4", "d", [], base_version=2)
    assert fresh.number == 3


def test_concurrent_imports_same_content_are_idempotent(client):
    """并发导入同内容：组件库零竞态，同哈希幂等（P1 验收：无竞态）。"""
    import concurrent.futures

    client, _fake = client
    with concurrent.futures.ThreadPoolExecutor(max_workers=4) as pool:
        futures = [
            pool.submit(
                client.post,
                "/api/v1/files/import",
                files={"file": ("m.sat", SAT_BYTES, "application/octet-stream")},
                data={"project_name": f"race-{index}"},
            )
            for index in range(4)
        ]
        results = [future.result() for future in futures]
    assert all(response.status_code == 201 for response in results)
    digests = {response.json()["components"][0]["sat"] for response in results}
    assert len(digests) == 1  # 同内容同哈希：组件全局去重
    for response in results:
        body = response.json()
        assert client.get(f"/api/v1/assets/{body['components'][0]['sat']}.sat").status_code == 200
