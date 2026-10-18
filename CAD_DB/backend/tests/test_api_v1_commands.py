"""v1 命令引擎契约测试（V3_PLAN.md P2 验收：纯 API 建模全流程）。

worker 池用进程内假件；真实 worker 的几何语义由 mesh_tests、worker selftest
与 tests/test_v1_real_commands_integration.py（integration 标记）锁定。
"""

from __future__ import annotations

import hashlib
from pathlib import Path

import pytest
from fastapi.testclient import TestClient

from app import main
from app.component_store import ComponentStore, set_component_store
from app.v1_store import initialize_v1_store, shutdown_v1_store
from app.worker_client import ImportResult, ImportedComponent, set_worker_gateway
from app.worker_pool import WorkerError, set_worker_pool

SAT_BYTES = b"fake-sat-content"


def _digest(payload: bytes) -> str:
    return hashlib.sha256(payload).hexdigest()


class FakePool:
    """进程内假几何池：维护组件内容字典，模拟 worker 的变更语义。"""

    def __init__(self) -> None:
        self.calls: list[tuple[str, int, list[str], dict]] = []
        self.fail_next: Exception | None = None

    async def execute(self, project_id, head_version, components, command):
        if self.fail_next is not None:
            failure, self.fail_next = self.fail_next, None
            raise failure
        action = command["action"]
        params = command.get("params", {})
        self.calls.append((project_id, head_version, [c.hash_sat for c in components], command))
        if action == "component.create":
            content = f"{params.get('kind')}-geom-{len(self.calls)}".encode()
            return {
                "added": [{
                    "sid": "n0", "kind": params.get("kind"),
                    "sat_bytes": _digest(content), "glb_bytes": _digest(b"glb" + content),
                    "bbox": {"min": [0, 0, 0], "max": [2, 2, 2]},
                    "color": [0.9, 0.9, 0.9], "triangles": 12, "vertices": 8,
                }],
                "modified": [], "removed": [],
            }
        if action == "component.translate":
            sid = params["component"]
            content = f"{sid}-moved-{len(self.calls)}".encode()
            return {
                "added": [],
                "modified": [{
                    "sid": sid,
                    "sat_bytes": _digest(content), "glb_bytes": _digest(b"glb" + content),
                    "bbox": {"min": [5, 0, 0], "max": [7, 2, 2]},
                    "triangles": 12, "vertices": 8,
                }],
                "removed": [],
            }
        if action == "component.delete":
            return {"added": [], "modified": [], "removed": [params["component"]]}
        if action == "boolean.unite":
            target = params["targets"][0]
            content = f"{target}-united-{len(self.calls)}".encode()
            return {
                "added": [],
                "modified": [{
                    "sid": target,
                    "sat_bytes": _digest(content), "glb_bytes": _digest(b"glb" + content),
                    "bbox": {"min": [0, 0, 0], "max": [4, 4, 4]},
                    "triangles": 24, "vertices": 16,
                }],
                "removed": [params["tool"]],
            }
        raise AssertionError(f"fake pool cannot execute {action}")


class FakeBatchWorker:
    def import_sat(self, sat_bytes: bytes) -> ImportResult:
        return ImportResult(components=[ImportedComponent(sat=sat_bytes, glb=b"glTF-fake")])


@pytest.fixture()
def client(tmp_path: Path):
    initialize_v1_store(memory=True)
    set_component_store(ComponentStore(tmp_path / "components"))
    set_worker_gateway(FakeBatchWorker())  # type: ignore[arg-type]
    pool = FakePool()
    set_worker_pool(pool)  # type: ignore[arg-type]
    yield TestClient(main.app), pool
    set_worker_pool(None)
    set_worker_gateway(None)
    set_component_store(None)
    shutdown_v1_store()


def _import_project(client: TestClient, name: str) -> dict:
    response = client.post(
        "/api/v1/files/import",
        files={"file": ("model.sat", SAT_BYTES, "application/octet-stream")},
        data={"project_name": name},
    )
    assert response.status_code == 201, response.text
    return response.json()


def _run(client: TestClient, project_id: str, payload: dict):
    return client.post(f"/api/v1/projects/{project_id}/commands", json=payload)


def test_command_schemas_catalog(client):
    client, _pool = client
    schema = client.get("/api/v1/schema/commands").json()
    assert schema["schema"] == "dbcad.commands/1"
    expected = {
        "component.create", "component.translate", "component.rotate", "component.scale",
        "component.delete", "boolean.unite", "boolean.subtract", "boolean.intersect",
        "intersector.intersect", "defeature.unrepeat",
    }
    assert expected <= set(schema["commands"])


def test_create_on_empty_project_starts_version_chain(client):
    client, pool = client
    project = client.post("/api/v1/projects", json={"name": "from-scratch"}).json()
    result = _run(client, project["id"], {
        "action": "component.create",
        "params": {"kind": "cube", "width": 10, "height": 4, "depth": 2},
    }).json()
    assert result["version"] == 1 and result["base_version"] is None
    assert [c["name"] for c in result["components"]] == ["cube"]
    scene = client.get(f"/api/v1/projects/{project['id']}/scene").json()
    assert len(scene["components"]) == 1
    assert pool.calls[-1][2] == []  # 空场景打开会话


def test_translate_replaces_component_content_in_place(client):
    client, _pool = client
    imported = _import_project(client, "move-me")
    project_id = imported["project_id"]
    sid = imported["components"][0]["sat"]

    result = _run(client, project_id, {
        "action": "component.translate",
        "params": {"component": sid, "offset": [5, 0, 0]},
    }).json()
    assert result["version"] == 2
    assert len(result["components"]) == 1
    moved = result["components"][0]
    assert moved["sat"] != sid  # 内容变了 → 新哈希
    assert moved["name"] == "component-0"  # 名称/颜色语义随槽位保留
    assert moved["bbox"]["min"] == [5.0, 0.0, 0.0]
    scene = client.get(f"/api/v1/projects/{project_id}/scene").json()
    assert scene["components"] == result["components"]


def test_boolean_and_delete_manage_components(client):
    client, _pool = client
    project_id = _import_project(client, "bool")["project_id"]
    first = _run(client, project_id, {"action": "component.create", "params": {"kind": "sphere", "radius": 3}}).json()
    tool_sid = first["components"][1]["sat"]
    target_sid = first["components"][0]["sat"]

    united = _run(client, project_id, {
        "action": "boolean.unite",
        "params": {"targets": [target_sid], "tool": tool_sid},
    }).json()
    assert len(united["components"]) == 1
    assert united["removed"] == [tool_sid]
    assert united["components"][0]["triangles"] == 24

    deleted = _run(client, project_id, {"action": "component.delete", "params": {"component": united["components"][0]["sat"]}}).json()
    assert deleted["components"] == []


def test_stale_base_version_rebase_and_conflict(client):
    client, _pool = client
    imported = _import_project(client, "rebase")
    project_id = imported["project_id"]
    sid = imported["components"][0]["sat"]

    # v2：另一个客户端平移了组件。
    _run(client, project_id, {"action": "component.translate", "params": {"component": sid, "offset": [1, 0, 0]}})

    # 持 v1 视图的新建命令：不引用旧组件 → 自动前移。
    forwarded = _run(client, project_id, {
        "action": "component.create", "params": {"kind": "cone"},
        "base_version": 1,
    })
    assert forwarded.status_code == 200 and forwarded.json()["rebased"] is True

    # 持 v1 视图引用已被 v2 变更的组件 → 几何级冲突诚实拒绝。
    conflict = _run(client, project_id, {
        "action": "component.translate",
        "params": {"component": sid, "offset": [0, 1, 0]},
        "base_version": 1,
    })
    assert conflict.status_code == 409
    detail = conflict.json()["detail"]
    assert detail["reason"] == "stale_base_version"
    assert detail["current_head"] == 3
    assert detail["conflicting_components"] == [sid]


def test_undo_redo_manifest_semantics(client):
    client, _pool = client
    project_id = _import_project(client, "undo")["project_id"]
    sid = project_id  # noqa: F841 - readability
    v2 = _run(client, project_id, {"action": "component.create", "params": {"kind": "torus"}}).json()
    assert len(v2["components"]) == 2

    undo = client.post(f"/api/v1/projects/{project_id}/undo").json()
    assert undo["version"] == 3 and undo["undone"] == 2
    scene = client.get(f"/api/v1/projects/{project_id}/scene").json()
    assert len(scene["components"]) == 1  # 回到 v1 清单

    redo = client.post(f"/api/v1/projects/{project_id}/redo").json()
    assert redo["version"] == 4
    scene = client.get(f"/api/v1/projects/{project_id}/scene").json()
    assert len(scene["components"]) == 2  # 重放 v2 清单


def test_undo_at_root_rejected(client):
    client, _pool = client
    project_id = _import_project(client, "undo-root")["project_id"]
    assert client.post(f"/api/v1/projects/{project_id}/undo").status_code == 409
    assert client.post(f"/api/v1/projects/{project_id}/redo").status_code == 409


def test_dry_run_validates_and_reports_affects(client):
    client, _pool = client
    imported = _import_project(client, "dry")
    project_id = imported["project_id"]
    sid = imported["components"][0]["sat"]
    ok = client.post(f"/api/v1/projects/{project_id}/commands/dry-run", json={
        "action": "component.translate", "params": {"component": sid, "offset": [1, 0, 0]},
    })
    assert ok.status_code == 200 and ok.json() == {"valid": True, "action": "component.translate", "affects": [sid]}
    unknown = client.post(f"/api/v1/projects/{project_id}/commands/dry-run", json={
        "action": "component.translate", "params": {"component": "nope", "offset": [1, 0, 0]},
    })
    assert unknown.status_code == 422
    bad_action = client.post(f"/api/v1/projects/{project_id}/commands/dry-run", json={
        "action": "component.explode", "params": {},
    })
    assert bad_action.status_code == 422
    missing = client.post(f"/api/v1/projects/{project_id}/commands", json={
        "action": "component.translate", "params": {"component": sid},
    })
    assert missing.status_code == 422


def test_worker_failure_is_502_and_session_recovers(client):
    client, pool = client
    project_id = _import_project(client, "resilient")["project_id"]
    pool.fail_next = WorkerError("simulated worker crash")
    failure = _run(client, project_id, {"action": "component.create", "params": {"kind": "cube"}})
    assert failure.status_code == 502
    recovered = _run(client, project_id, {"action": "component.create", "params": {"kind": "cube"}})
    assert recovered.status_code == 200 and len(recovered.json()["components"]) == 2
