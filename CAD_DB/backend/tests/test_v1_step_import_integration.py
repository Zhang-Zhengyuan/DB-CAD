"""P9 STEP 导入集成测试：真 InterOp + 真 worker，abc 语料抽样模型。

链路：上传 STEP → worker InterOp 转换 → 组件（SAT/GLB/graph）→ 版本 1 →
实体投影。前提：abc_cache 语料与 dbcad_worker.exe 存在，否则 skip。
"""

from __future__ import annotations

from pathlib import Path

import pytest
from fastapi.testclient import TestClient

from app import main
from app.component_store import ComponentStore, set_component_store
from app.v1_store import initialize_v1_store, shutdown_v1_store
from app.worker_client import set_worker_gateway
from app.worker_pool import set_worker_pool

pytestmark = pytest.mark.integration

_REPO = Path(__file__).resolve().parents[3]
WORKER = _REPO / "x64" / "Debug" / "dbcad_worker.exe"
ABC = Path("D:/DBCAD/abc_cache/extracted")


@pytest.fixture()
def client(tmp_path: Path):
    if not WORKER.exists():
        pytest.skip(f"worker not built: {WORKER}")
    initialize_v1_store(memory=True)
    set_component_store(ComponentStore(tmp_path / "components"))
    set_worker_pool(None)
    from app.worker_client import SubprocessWorkerGateway

    set_worker_gateway(SubprocessWorkerGateway(WORKER))
    yield TestClient(main.app)
    set_worker_gateway(None)
    set_component_store(None)
    shutdown_v1_store()


def _first_step() -> Path | None:
    if not ABC.exists():
        return None
    return next(ABC.rglob("*.step"), None)


def test_step_import_end_to_end(client):
    step = _first_step()
    if step is None:
        pytest.skip("abc_cache 语料不存在")
    response = client.post(
        "/api/v1/files/import",
        files={"file": (step.name, step.read_bytes(), "application/step")},
        data={"project_name": "step-e2e"},
    )
    assert response.status_code == 201, response.text
    payload = response.json()
    assert payload["version"] == 1
    assert len(payload["components"]) >= 1
    # 投影入库（内容寻址，abc 模型 fallback=sat_asset 属常态）
    assert payload["entity_projection"]["written"] >= 1
    entities = client.get(
        f"/api/v1/projects/{payload['project_id']}/entities", params={"label": "face"}
    ).json()
    assert entities["count"] >= 1, "STEP 转换后的模型应有 face 实体投影"
    # 组件 SAT 资产可导出回整体 SAT（SAT 仍是几何真相）
    export = client.get(
        f"/api/v1/projects/{payload['project_id']}/files/export", params={"fmt": "sat"}
    )
    assert export.status_code == 200 and len(export.content) > 100
