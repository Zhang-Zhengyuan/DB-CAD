"""P2 验收集成测试：真实常驻 worker（ACIS）上纯 API 跑通建模全流程。

链路：建项目 → 参数化创建（体/面/线）→ 变换 → 布尔 → 场景/资产 → 撤销 →
导出。运行条件：x64/Debug/dbcad_worker.exe 已构建。

    uv run --frozen pytest -q tests/test_v1_real_commands_integration.py -m integration
"""

from __future__ import annotations

from pathlib import Path

import pytest
from fastapi.testclient import TestClient

from app import main
from app.component_store import ComponentStore, set_component_store
from app.v1_store import initialize_v1_store, shutdown_v1_store
from app.worker_pool import GeometryPool, set_worker_pool

pytestmark = pytest.mark.integration

_REPO = Path(__file__).resolve().parents[3]
WORKER = _REPO / "x64" / "Debug" / "dbcad_worker.exe"


@pytest.fixture()
def client(tmp_path: Path):
    if not WORKER.exists():
        pytest.skip(f"worker not built: {WORKER}")
    initialize_v1_store(memory=True)
    set_component_store(ComponentStore(tmp_path / "components"))
    pool = GeometryPool(max_sessions=2)
    set_worker_pool(pool)
    yield TestClient(main.app)
    set_worker_pool(None)
    set_component_store(None)
    shutdown_v1_store()


def _command(client: TestClient, project_id: str, payload: dict) -> dict:
    response = client.post(f"/api/v1/projects/{project_id}/commands", json=payload)
    assert response.status_code == 200, response.text
    return response.json()


def test_full_modeling_flow_on_real_kernel(client):
    project = client.post("/api/v1/projects", json={"name": "p2-acceptance"}).json()
    project_id = project["id"]

    cube = _command(client, project_id, {
        "action": "component.create",
        "params": {"kind": "cube", "width": 10, "height": 10, "depth": 10},
    })
    assert cube["version"] == 1
    cube_component = cube["components"][0]
    assert cube_component["name"] == "cube"
    assert cube_component["triangles"] == 12
    assert cube_component["bbox"] == {"min": [-5.0, -5.0, -5.0], "max": [5.0, 5.0, 5.0]}

    sphere = _command(client, project_id, {
        "action": "component.create",
        "params": {"kind": "sphere", "radius": 4, "position": [12, 0, 0]},
    })
    assert sphere["version"] == 2 and len(sphere["components"]) == 2
    sphere_component = sphere["components"][1]
    assert sphere_component["bbox"]["min"] == [8.0, -4.0, -4.0]

    moved = _command(client, project_id, {
        "action": "component.translate",
        "params": {"component": cube_component["sat"], "offset": [-20, 0, 0]},
    })
    assert moved["version"] == 3
    moved_cube = moved["components"][0]
    assert moved_cube["sat"] != cube_component["sat"]
    assert moved_cube["bbox"]["max"] == [-15.0, 5.0, 5.0]

    united = _command(client, project_id, {
        "action": "boolean.unite",
        "params": {"targets": [moved_cube["sat"]], "tool": sphere_component["sat"]},
    })
    assert united["version"] == 4
    assert len(united["components"]) == 1  # 工具并入目标
    assert united["removed"] == [sphere_component["sat"]]
    # 两体分离（相距 20 > r4+5）：并集应是两个 lump，包围盒跨两者。
    united_bbox = united["components"][0]["bbox"]
    assert united_bbox["min"][0] < -24 and united_bbox["max"][0] > 15

    scene = client.get(f"/api/v1/projects/{project_id}/scene").json()
    assert scene["version"] == 4 and len(scene["components"]) == 1
    glb = client.get(f"/api/v1/assets/{scene['components'][0]['glb']}.glb")
    assert glb.status_code == 200 and glb.content[:4] == b"glTF"

    undo = client.post(f"/api/v1/projects/{project_id}/undo").json()
    assert undo["version"] == 5 and undo["undone"] == 4
    scene = client.get(f"/api/v1/projects/{project_id}/scene").json()
    assert len(scene["components"]) == 2  # 回到布尔前

    redo = client.post(f"/api/v1/projects/{project_id}/redo").json()
    assert redo["version"] == 6
    scene = client.get(f"/api/v1/projects/{project_id}/scene").json()
    assert len(scene["components"]) == 1

    exported = client.get(f"/api/v1/projects/{project_id}/files/export?fmt=sat")
    assert exported.status_code == 200 and len(exported.content) > 100


def test_face_and_edge_primitives_are_components(client):
    project = client.post("/api/v1/projects", json={"name": "faces-lines"}).json()
    project_id = project["id"]
    created = _command(client, project_id, {"action": "component.create", "params": {"kind": "plane"}})
    plane = created["components"][0]
    assert plane["triangles"] >= 2  # 单面也有网格
    line = _command(client, project_id, {"action": "component.create", "params": {"kind": "arc"}})
    arc = line["components"][1]
    assert arc["triangles"] == 0  # 纯线图元：只有边折线
    arc_glb = client.get(f"/api/v1/assets/{arc['glb']}.glb")
    assert arc_glb.status_code == 200 and arc_glb.content[:4] == b"glTF"
