"""v1 全链路集成测试：真实 C++ worker（ACIS 内核）。

P1 验收的"真栈"版本（V3_PLAN.md P1）：导入 SAT（先用 worker 自检产线的同款
SAT 字节？否——直接用 TestClient 走 HTTP，SAT 样本来自仓库 fixtures 或现场
生成）。运行条件：x64/Debug/dbcad_worker.exe 已构建。

    uv run --frozen pytest -q tests/test_v1_real_worker_integration.py -m integration

无 SAT fixture 时跳过（不失败）。
"""

from __future__ import annotations

from pathlib import Path

import pytest
from fastapi.testclient import TestClient

from app import main
from app.component_store import ComponentStore, set_component_store
from app.v1_store import initialize_v1_store, shutdown_v1_store
from app.worker_client import SubprocessWorkerGateway, set_worker_gateway

pytestmark = pytest.mark.integration

_REPO = Path(__file__).resolve().parents[3]
WORKER = _REPO / "x64" / "Debug" / "dbcad_worker.exe"
SAT_FIXTURES = list((_REPO / "scripts" / "tests" / "acis").glob("*.sat"))[:1]


@pytest.fixture(scope="module")
def worker_sat_bytes() -> bytes:
    if not WORKER.exists():
        pytest.skip(f"worker not built: {WORKER}")
    if not SAT_FIXTURES:
        pytest.skip("no SAT fixture under scripts/tests/acis")
    return SAT_FIXTURES[0].read_bytes()


@pytest.fixture()
def real_client(tmp_path: Path, worker_sat_bytes: bytes):
    initialize_v1_store(memory=True)
    set_component_store(ComponentStore(tmp_path / "components"))
    set_worker_gateway(SubprocessWorkerGateway(WORKER))
    yield TestClient(main.app), worker_sat_bytes
    set_worker_gateway(None)
    set_component_store(None)
    shutdown_v1_store()


def test_import_scene_assets_export_roundtrip(real_client):
    client, sat_bytes = real_client
    imported = client.post(
        "/api/v1/files/import",
        files={"file": ("fixture.sat", sat_bytes, "application/octet-stream")},
        data={"project_name": "integration-real"},
    )
    assert imported.status_code == 201, imported.text
    body = imported.json()
    assert body["version"] == 1 and body["components"]

    project_id = body["project_id"]
    scene = client.get(f"/api/v1/projects/{project_id}/scene").json()
    assert scene["components"] == body["components"]

    for component in body["components"]:
        sat = client.get(f"/api/v1/assets/{component['sat']}.sat")
        assert sat.status_code == 200 and len(sat.content) > 100
        glb = client.get(f"/api/v1/assets/{component['glb']}.glb")
        assert glb.status_code == 200
        assert glb.content[:4] == b"glTF" and len(glb.content) > 100

    exported = client.get(f"/api/v1/projects/{project_id}/files/export?fmt=sat")
    assert exported.status_code == 200 and len(exported.content) > 100
    whole_glb = client.get(f"/api/v1/projects/{project_id}/files/export?fmt=glb")
    assert whole_glb.status_code == 200 and whole_glb.content[:4] == b"glTF"
