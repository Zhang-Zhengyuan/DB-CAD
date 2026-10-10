"""The administrator view must keep Push bodies opt-in and read old manifests."""

import json

from app.neo4j_webui import versions as neo4j_webui


def test_mode1_detail_reads_legacy_and_new_audit_without_exposing_raw(monkeypatch):
    raw = '{"entity_graph":{"nodes":[],"rels":[]},"changes":[]}'
    for stored_separately in (False, True):
        manifest = {
            "changes": [],
            "audit": {
                "payload_format": "json",
                "contains_sat": False,
                "request_bytes": len(raw.encode("utf-8")),
            },
        }
        if not stored_separately:
            manifest["audit"]["request_json"] = raw

        def single(query, _params):
            if "HAS_JSON_GRAPH_VERSION" in query:
                return {
                    "project_id": "p",
                    "version": 1,
                    "author": "a",
                    "created_at": "2026-10-02T00:00:00Z",
                    "manifest_chars": 100,
                    "graph_hash": "hash",
                }
            if "v.storage_mode AS storage_mode" in query:
                return {
                    "storage_mode": "component_manifest_v1",
                    "delta_json": json.dumps(manifest),
                    "db_write_ms": 12.5,
                }
            if "v.request_json AS request_json" in query:
                return {"request_json": raw if stored_separately else None}
            return {"count": 0}

        monkeypatch.setattr(neo4j_webui, "_run_single_cypher", single)
        monkeypatch.setattr(neo4j_webui, "_run_cypher", lambda *_: [])
        summary = neo4j_webui.get_json_version_detail("project", 1, include_raw=False)
        assert summary["request_bytes"] == len(raw.encode("utf-8"))
        assert "request_json" not in json.dumps(summary)
        assert "raw_push_json" not in summary
        detail = neo4j_webui.get_json_version_detail("project", 1, include_raw=True)
        assert detail["raw_push_json"] == raw


def test_mode0_detail_keeps_sat_payload_opt_in(monkeypatch):
    sat = "SAT body text"
    raw = json.dumps({"type": "submit_model", "content": {"sat": sat}})

    def single(_query, _params):
        return {
            "v": {
                "version": 1,
                "author": "a",
                "created_at": "2026-10-02T00:00:00Z",
                "project_id": "p",
                "content_json": json.dumps({"sat": sat}),
                "audit_json": json.dumps(
                    {
                        "payload_format": "sat",
                        "contains_sat": True,
                        "request_bytes": len(raw.encode("utf-8")),
                        "request_json": raw,
                        "db_write_ms": 12.5,
                    }
                ),
            }
        }

    monkeypatch.setattr(neo4j_webui, "_run_single_cypher", single)
    monkeypatch.setattr(neo4j_webui, "_run_cypher", lambda *_: [])
    summary = neo4j_webui.get_mode0_version_detail("project", 1, include_raw=False)
    assert summary["sat_bytes"] == len(sat.encode("utf-8"))
    assert summary["contains_sat"] is True
    assert "raw_sat" not in summary and "raw_push_json" not in summary
    detail = neo4j_webui.get_mode0_version_detail("project", 1, include_raw=True)
    assert detail["raw_sat"] == sat
    assert detail["raw_push_json"] == raw
