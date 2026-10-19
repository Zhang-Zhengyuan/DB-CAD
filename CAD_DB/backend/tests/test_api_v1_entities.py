"""实体级投影契约测试（V3_PLAN.md P6/ADR-3）。

假件池模拟 worker 的 emit（含 graph_json）；覆盖：命令/导入两触发点写投影、
幂等零写入、root 重编（临时 sid → 组件哈希）、label 过滤查询、投影等价
（内存版 L3：读回与写入载荷一致）、旧 worker 无 graph 时不阻断。
真实 codec 的几何语义由 worker selftest 与真栈集成测试锁定。
"""

from __future__ import annotations

import hashlib
from pathlib import Path

import pytest
from fastapi.testclient import TestClient

from app import main
from app.component_store import ComponentStore, set_component_store
from app.v1_store import get_v1_store, initialize_v1_store, shutdown_v1_store
from app.worker_client import ImportResult, ImportedComponent, set_worker_gateway
from app.worker_pool import set_worker_pool

SAT_BYTES = b"fake-sat-content"


def _digest(payload: bytes) -> str:
    return hashlib.sha256(payload).hexdigest()


def _graph(root_id: str = "n0") -> dict:
    """合成实体图（形状无意义，契约真实：root top_level + labels + rels）。"""
    return _graph_variant(root_id)


def _graph_variant(root_id: str = "n0", moved_face: bool = False) -> dict:
    """moved_face=True 模拟"改一个 face"：face 内容变、其余实体不变。"""
    face_props = {"entity_type": "face", "sense": 1, "geometry": {"type": "plane"}}
    if moved_face:
        face_props = {"entity_type": "face", "sense": 1, "geometry": {"type": "sphere", "radius": 4.0}}
    return {
        "schema": "dbcad.entity_graph.v2",
        "serializer_build": "uvsafe-topology-v5-bounded",
        "nodes": [
            {
                "id": root_id,
                "labels": ["body"],
                "props": {
                    "entity_type": "body",
                    "top_level": True,
                    "uuid": root_id,
                    "fallback": "fine_grained",
                    "topology_node_count": 3,
                    "topology_rel_count": 2,
                },
            },
            {"id": "n_ptr_1", "labels": ["face"], "props": face_props},
            {
                "id": "n_ptr_2",
                "labels": ["edge"],
                "props": {"entity_type": "edge", "sense": 0},
            },
        ],
        "rels": [
            {"type": "body_lump", "start": root_id, "end": "n_ptr_1"},
            {"type": "face_loop", "start": "n_ptr_1", "end": "n_ptr_2"},
        ],
    }


class GraphFakePool:
    """假几何池：emit 携带合成 graph_json（同内容 ⇒ 同 sat 哈希 ⇒ 幂等）。"""

    def __init__(self) -> None:
        self.calls = 0

    async def execute(self, project_id, head_version, components, command):
        action = command["action"]
        if action == "component.create":
            self.calls += 1
            content = f"{command['params'].get('kind')}-geom-{self.calls}".encode()
            entry = {
                "sid": "n0",
                "kind": command["params"].get("kind"),
                "sat_bytes": _digest(content),
                "glb_bytes": _digest(b"glb" + content),
                "graph_json": _graph("n0"),
                "bbox": {"min": [0, 0, 0], "max": [2, 2, 2]},
                "color": [0.9, 0.9, 0.9],
                "triangles": 12,
                "vertices": 8,
            }
            return {"added": [entry], "modified": [], "removed": []}
        if action == "component.delete":
            return {"added": [], "modified": [], "removed": [command["params"]["component"]]}
        if action in ("component.translate", "component.rotate", "component.scale"):
            target = command["params"]["component"]
            match = next(c for c in components if c.hash_sat == target)
            content = f"moved-{target}".encode()
            entry = {
                "sid": target,
                "kind": "moved",
                "sat_bytes": _digest(content),
                "glb_bytes": _digest(b"glb" + content),
                # P8：改一个 face（内容变、其余实体 id/内容不变）
                "graph_json": _graph_variant(target, moved_face=True),
                "bbox": {"min": [0, 0, 0], "max": [3, 3, 3]},
                "color": match.color,
                "triangles": 12,
                "vertices": 8,
            }
            return {"added": [], "modified": [entry], "removed": []}
        raise AssertionError(f"unexpected action {action}")


class GraphFakeBatchWorker:
    def import_sat(self, sat_bytes: bytes) -> ImportResult:
        return ImportResult(
            components=[
                ImportedComponent(sat=sat_bytes, glb=b"glTF-fake", graph=_graph("comp-0"))
            ]
        )


@pytest.fixture()
def client(tmp_path: Path):
    initialize_v1_store(memory=True)
    set_component_store(ComponentStore(tmp_path / "components"))
    set_worker_gateway(GraphFakeBatchWorker())  # type: ignore[arg-type]
    pool = GraphFakePool()
    set_worker_pool(pool)  # type: ignore[arg-type]
    yield TestClient(main.app), pool
    set_worker_pool(None)
    set_worker_gateway(None)
    set_component_store(None)
    shutdown_v1_store()


def _create(client: TestClient, project_id: str, kind: str = "cube") -> dict:
    response = client.post(
        f"/api/v1/projects/{project_id}/commands",
        json={"action": "component.create", "params": {"kind": kind}},
    )
    assert response.status_code == 200, response.text
    return response.json()


def test_command_writes_projection_and_entities_endpoint(client):
    client, _pool = client
    project_id = client.post("/api/v1/projects", json={"name": "proj"}).json()["id"]
    result = _create(client, project_id)
    assert result["entity_projection"]["written"] == 1
    assert result["entity_projection"]["skipped"] == 0
    assert result["entity_projection"]["entities"] == 3

    payload = client.get(f"/api/v1/projects/{project_id}/entities").json()
    assert payload["version"] == 1 and payload["components"] == 1
    labels = {tuple(e["labels"]) for e in payload["entities"]}
    assert ("body",) in labels and ("face",) in labels and ("edge",) in labels

    faces = client.get(f"/api/v1/projects/{project_id}/entities", params={"label": "face"}).json()
    assert faces["count"] == 1 and faces["entities"][0]["labels"] == ["face"]
    assert faces["entities"][0]["props"]["entity_type"] == "face"
    # label 大小写不敏感（native 名 Face 同义）
    upper = client.get(f"/api/v1/projects/{project_id}/entities", params={"label": "Face"}).json()
    assert upper["count"] == 1


def test_projection_idempotent_zero_write_for_unchanged_component(client):
    client, _pool = client
    project_id = client.post("/api/v1/projects", json={"name": "proj"}).json()["id"]
    first = _create(client, project_id, kind="cube")
    second = _create(client, project_id, kind="cube")
    # 内容不同（calls 计数器递增）→ 新组件各写一次投影
    assert first["entity_projection"]["written"] == 1
    assert second["entity_projection"]["written"] == 1
    store = get_v1_store()
    assert len(store.entity_projections) == 2

    # 重复提交同一投影（同 hash_sat）→ 零写入
    outcome = store.write_entity_projection(first["added"][0]["sat"], _graph("n0"))
    assert outcome["written"] is False
    entities = client.get(f"/api/v1/projects/{project_id}/entities").json()
    assert entities["count"] == 6  # 两组件 × 3 节点，幂等不重复


def test_projection_root_renamed_to_component_hash(client):
    """L3 等价断言（内存版）：临时 root id 重编为组件哈希，rels 引用同步。"""
    client, _pool = client
    project_id = client.post("/api/v1/projects", json={"name": "proj"}).json()["id"]
    result = _create(client, project_id)
    component_hash = result["added"][0]["sat"]
    store = get_v1_store()
    projection = store.get_entity_projection(component_hash)
    assert projection is not None
    root = next(n for n in projection["nodes"] if n["props"].get("top_level"))
    assert root["id"] == component_hash and root["props"]["uuid"] == component_hash
    ids = {n["id"] for n in projection["nodes"]}
    for rel in projection["rels"]:
        assert rel["start"] in ids and rel["end"] in ids
    # root 重编后引用它的 rel 也指向组件哈希
    assert any(rel["start"] == component_hash for rel in projection["rels"])
    assert projection["serializer_build"] == "uvsafe-topology-v5-bounded"


def test_import_writes_projection(client):
    client, _pool = client
    response = client.post(
        "/api/v1/files/import",
        files={"file": ("model.sat", SAT_BYTES, "application/octet-stream")},
        data={"project_name": "imported"},
    )
    assert response.status_code == 201, response.text
    payload = response.json()
    assert payload["entity_projection"]["written"] == 1
    entities = client.get(f"/api/v1/projects/{payload['project_id']}/entities").json()
    assert entities["count"] == 3


def test_entities_unknown_version_is_404(client):
    client, _pool = client
    project_id = client.post("/api/v1/projects", json={"name": "proj"}).json()["id"]
    assert client.get(f"/api/v1/projects/{project_id}/entities").status_code == 404
    assert (
        client.get(f"/api/v1/projects/{project_id}/entities", params={"v": 9}).status_code == 404
    )


def test_projection_schema_mismatch_is_rejected_not_stored(client):
    from app.entity_projection import ProjectionError

    store = get_v1_store()
    try:
        store.write_entity_projection("deadbeef", {"schema": "something.else", "nodes": []})
        raise AssertionError("expected ProjectionError")
    except ProjectionError:
        pass
    assert "deadbeef" not in store.entity_projections


def test_modify_one_face_adds_only_changed_entities(client):
    """P8/Phase D 验收口径（契约级）：改一个 face 的二次提交——
    实体内容寻址下新增实体节点数 == 内容变化实体数（1），拓扑骨架复用，
    变更 face 挂 DERIVED_FROM 指向父版本 face。"""
    client, _pool = client
    project_id = client.post("/api/v1/projects", json={"name": "p8"}).json()["id"]
    created = _create(client, project_id, kind="cube")
    assert created["entity_projection"]["entity_added"] == 3  # 首次全量

    target = created["components"][0]["sat"]
    moved = client.post(
        f"/api/v1/projects/{project_id}/commands",
        json={"action": "component.translate", "params": {"component": target, "offset": [1, 0, 0]}},
    ).json()
    stats = moved["entity_projection"]
    assert stats["entity_added"] == 1  # 只有 face 内容变了
    assert stats["entity_shared"] == 2  # body/edge 复用
    assert stats["derived"] == 1  # 新 face DERIVED_FROM 旧 face

    store = get_v1_store()
    assert len(store.entity_nodes) == 4  # 3 原始 + 1 新 face（共享池）
    # generation 链：新 face 的 key → 旧 face 的 key
    assert all(len(parents) >= 1 for parents in store.derived_from.values())
    new_key = next(iter(store.derived_from))
    assert store.derived_from[new_key]


def test_identical_components_share_all_entities(client):
    """内容寻址共享：两个同内容组件（不同 hash）→ 实体节点零新增。"""
    client, pool = client
    project_id = client.post("/api/v1/projects", json={"name": "p8-share"}).json()["id"]
    first = _create(client, project_id, kind="cube")
    second = _create(client, project_id, kind="cube")
    assert first["entity_projection"]["entity_added"] == 3
    # 假件池同 kind 的第二次 create 内容相同（calls 计数进 sid 但 graph 内容同）
    assert second["entity_projection"]["entity_added"] == 0
    store = get_v1_store()
    assert len(store.entity_nodes) == 3  # 池不增长
    # 两个组件的 /entities 视图各自完整
    total = client.get(f"/api/v1/projects/{project_id}/entities").json()
    assert total["count"] == 6  # 2 组件 × 3 实体（视图级不折叠）


def test_diff_endpoint_reports_entity_level_changes(client):
    """P8/Phase D：/diff 端点——组件与实体内容两级对比 + generation 链数。"""
    client, _pool = client
    project_id = client.post("/api/v1/projects", json={"name": "p8-diff"}).json()["id"]
    created = _create(client, project_id, kind="cube")
    target = created["components"][0]["sat"]
    moved = client.post(
        f"/api/v1/projects/{project_id}/commands",
        json={"action": "component.translate", "params": {"component": target, "offset": [1, 0, 0]}},
    ).json()

    diff = client.get(
        f"/api/v1/projects/{project_id}/diff", params={"from_v": 1, "to_v": 2}
    ).json()
    # 组件不可变：修改 = 新 hash 替换旧 hash（集合语义 added+removed）
    assert diff["components"]["shared"] == 0
    assert diff["components"]["added"] == [moved["components"][0]["sat"]]
    assert diff["components"]["removed"] == [target]
    assert diff["entities"]["added"] == moved["entity_projection"]["entity_added"]
    assert diff["entities"]["removed"] >= 1  # 旧 face 内容退出 head 视图
    assert diff["entities"]["shared"] >= 1  # 拓扑骨架共享
    assert diff["derived_chains"] == moved["entity_projection"]["derived"]

    missing = client.get(
        f"/api/v1/projects/{project_id}/diff", params={"from_v": 1, "to_v": 99}
    )
    assert missing.status_code == 404


def test_compact_removes_orphan_entities(client):
    """P8.5 压实：版本淘汰旧组件后，其独有内容实体成孤魂，compact 清理且幂等。"""
    client, _pool = client
    project_id = client.post("/api/v1/projects", json={"name": "p8-compact"}).json()["id"]
    created = _create(client, project_id, kind="cube")
    target = created["components"][0]["sat"]
    client.post(
        f"/api/v1/projects/{project_id}/commands",
        json={"action": "component.translate", "params": {"component": target, "offset": [1, 0, 0]}},
    )
    store = get_v1_store()
    before = len(store.entity_nodes)

    # 模拟版本淘汰：清掉对旧组件投影的引用（Memory 语义下即从清单移除投影）
    store.entity_projections.pop(target)
    removed = client.post("/api/v1/admin/compact-entities").json()["removed_entities"]
    assert removed >= 1  # 旧 face 内容（唯一被旧组件引用）被清理
    assert len(store.entity_nodes) == before - removed
    # 幂等
    again = client.post("/api/v1/admin/compact-entities").json()["removed_entities"]
    assert again == 0


def test_conflict_409_reports_conflicting_entities(client):
    """P8/Phase D：stale_base_version 冲突响应带实体级 conflicting_entities。"""
    client, _pool = client
    project_id = client.post("/api/v1/projects", json={"name": "p8-409"}).json()["id"]
    created = _create(client, project_id, kind="cube")
    target = created["components"][0]["sat"]
    # 他方删除该组件（head 前移），本地再以旧 base 提交 → 409
    client.post(
        f"/api/v1/projects/{project_id}/commands",
        json={"action": "component.delete", "params": {"component": target}},
    )
    conflict = client.post(
        f"/api/v1/projects/{project_id}/commands",
        json={
            "action": "component.translate",
            "params": {"component": target, "offset": [1, 0, 0]},
            "base_version": 1,
        },
    )
    assert conflict.status_code == 409
    detail = conflict.json()["detail"]
    assert detail["conflicting_components"] == [target]
    assert len(detail["conflicting_entities"]) == 3  # 组件投影的实体键


class LegacyPoolNoGraph:
    """旧 worker（P6 前）无 graph 产物：命令链路照常，投影统计为零。"""

    async def execute(self, project_id, head_version, components, command):
        content = b"legacy-geom"
        return {
            "added": [
                {
                    "sid": "n0",
                    "kind": "cube",
                    "sat_bytes": _digest(content),
                    "glb_bytes": _digest(b"glb" + content),
                    "bbox": {"min": [0, 0, 0], "max": [1, 1, 1]},
                    "color": [0.8, 0.8, 0.8],
                    "triangles": 4,
                    "vertices": 4,
                }
            ],
            "modified": [],
            "removed": [],
        }


def test_legacy_worker_without_graph_does_not_break(client):
    client, _pool = client
    set_worker_pool(LegacyPoolNoGraph())  # type: ignore[arg-type]
    try:
        project_id = client.post("/api/v1/projects", json={"name": "legacy"}).json()["id"]
        result = _create(client, project_id)
        assert result["entity_projection"] == {
            "written": 0, "skipped": 0, "entities": 0, "entity_added": 0,
            "entity_shared": 0, "derived": 0,
        }
        entities = client.get(f"/api/v1/projects/{project_id}/entities").json()
        assert entities["count"] == 0
    finally:
        pool = GraphFakePool()
        set_worker_pool(pool)  # type: ignore[arg-type]
