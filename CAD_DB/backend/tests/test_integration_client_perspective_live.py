import json
import os
import time
from uuid import uuid4

import httpx
import pytest


DEFAULT_SAT = """700 0 1 0\nBegin-of-ACIS-data 2 0 0\nEnd-of-ACIS-data\n"""


def _base_url() -> str:
    return os.getenv("CAD_DB_INTEGRATION_BASE_URL", "http://127.0.0.1:8000").rstrip("/")


def _bridge_url() -> str:
    return os.getenv("CAD_DB_STORAGE_BRIDGE_URL", "http://127.0.0.1:8100").rstrip("/")


def _headers() -> dict[str, str]:
    password = os.getenv("CAD_DB_API_PASSWORD", "")
    return {"X-API-Password": password} if password else {}


def _sat_payload() -> str:
    sat_file = os.getenv("CAD_DB_INTEGRATION_SAT_FILE", "").strip()
    if sat_file:
        try:
            with open(sat_file, "r", encoding="utf-8", errors="ignore") as f:
                return f.read()
        except Exception:
            pass

    sat_inline = os.getenv("CAD_DB_INTEGRATION_SAT_CONTENT", "").strip()
    if sat_inline:
        return sat_inline

    return DEFAULT_SAT


def _request(client: httpx.Client, method: str, path: str, **kwargs) -> httpx.Response:
    return client.request(method, f"{_base_url()}{path}", headers=_headers(), timeout=15.0, **kwargs)


def _safe_json(response: httpx.Response):
    try:
        return response.json()
    except Exception:
        return {"raw": response.text}


def _collect_diagnostics(save_resp: httpx.Response, project_id: str) -> dict:
    diagnostics: dict[str, object] = {
        "save": {
            "status": save_resp.status_code,
            "body": _safe_json(save_resp),
        },
        "project_id": project_id,
        "base_url": _base_url(),
        "bridge_url": _bridge_url(),
        "env": {
            "HTTP_PROXY": os.getenv("HTTP_PROXY", ""),
            "HTTPS_PROXY": os.getenv("HTTPS_PROXY", ""),
            "NO_PROXY": os.getenv("NO_PROXY", ""),
        },
    }

    with httpx.Client(trust_env=False) as client:
        try:
            health = client.get(f"{_base_url()}/health", timeout=5.0)
            diagnostics["fastapi_health"] = {"status": health.status_code, "body": _safe_json(health)}
        except Exception as ex:
            diagnostics["fastapi_health"] = {"error": str(ex)}

        try:
            bridge_health = client.get(f"{_bridge_url()}/health", timeout=5.0)
            diagnostics["bridge_health"] = {"status": bridge_health.status_code, "body": _safe_json(bridge_health)}
        except Exception as ex:
            diagnostics["bridge_health"] = {"error": str(ex)}

        try:
            latest = _request(client, "GET", f"/projects/{project_id}/models/latest")
            diagnostics["latest_model"] = {"status": latest.status_code, "body": _safe_json(latest)}
        except Exception as ex:
            diagnostics["latest_model"] = {"error": str(ex)}

    return diagnostics


def _try_auto_repair_save(client: httpx.Client, project_id: str, payload: dict) -> httpx.Response:
    first = _request(client, "POST", f"/projects/{project_id}/models", json=payload)
    if first.status_code < 500:
        return first

    # 自动修复策略：出现 5xx 时等待后重试一次（覆盖偶发链路抖动）
    time.sleep(0.5)
    second = _request(client, "POST", f"/projects/{project_id}/models", json=payload)
    return second


@pytest.mark.integration
def test_client_perspective_save_model_with_diagnostics_and_auto_repair() -> None:
    with httpx.Client(trust_env=False) as client:
        try:
            health = _request(client, "GET", "/health")
        except Exception:
            pytest.skip("FastAPI service is not running")

        if health.status_code != 200:
            pytest.skip("FastAPI service is not healthy")

        project_name = f"it-client-save-{uuid4().hex}"
        create_resp = _request(client, "POST", "/projects", json={"name": project_name})
        assert create_resp.status_code == 201, create_resp.text
        project_id = create_resp.json()["id"]

        save_payload = {
            "author": "integration-client",
            "content": {"sat": _sat_payload()},
            "base_version": None,
        }

        save_resp = _try_auto_repair_save(client, project_id, save_payload)
        if save_resp.status_code != 201:
            diagnostics = _collect_diagnostics(save_resp, project_id)
            pytest.fail(
                "Client-perspective save failed. Diagnostics:\n"
                + json.dumps(diagnostics, ensure_ascii=False, indent=2)
            )

        body = save_resp.json()
        assert int(body.get("version", 0)) >= 1
