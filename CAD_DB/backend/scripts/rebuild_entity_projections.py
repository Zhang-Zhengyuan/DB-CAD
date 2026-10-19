"""存量组件投影重建（V3_PLAN P6 遗留收尾 / P7 工具）。

P6 之前入库的组件没有实体级投影（当时 emit 管线不产 graph.json）。本脚本
遍历组件库的全部 SAT 资产，经 worker import 单组件取得实体图，投影挂在
**原资产哈希**之下（worker 往返只用于取拓扑，root 重编以原哈希为准）。

用法：
    uv run --frozen python scripts/rebuild_entity_projections.py [--dry-run] [--limit N]

幂等：已有投影的组件自动跳过（write_entity_projection 的零写入语义）。
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from app.component_store import ComponentStore, get_component_store  # noqa: E402
from app.config import settings  # noqa: E402
from app.v1_store import Neo4jV1Store  # noqa: E402
from app.worker_client import get_worker_gateway  # noqa: E402


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--dry-run", action="store_true", help="只统计，不写入")
    parser.add_argument("--limit", type=int, default=0, help="最多处理 N 个组件（0=全部）")
    args = parser.parse_args()

    if not settings.neo4j_uri:
        print("未配置 CAD_DB_NEO4J_URI", file=sys.stderr)
        return 1

    from neo4j import GraphDatabase

    driver = GraphDatabase.driver(
        settings.neo4j_uri, auth=(settings.neo4j_user, settings.neo4j_password)
    )
    store = Neo4jV1Store(driver, settings.neo4j_database)
    store.initialize()

    assets: ComponentStore = get_component_store()
    sat_root = assets.root / "sat"
    sats = sorted(sat_root.rglob("*.sat")) if sat_root.exists() else []
    if args.limit:
        sats = sats[: args.limit]

    gateway = get_worker_gateway()
    done = skipped = failed = 0
    for index, path in enumerate(sats):
        digest = path.stem
        if store.has_entity_projection(digest):
            skipped += 1
            continue
        if args.dry_run:
            print(f"[dry-run] would rebuild {digest[:12]}")
            done += 1
            continue
        try:
            imported = gateway.import_sat(path.read_bytes())
            graph = imported.components[0].graph
            if graph is None:
                print(f"[warn] worker produced no graph for {digest[:12]}", file=sys.stderr)
                failed += 1
                continue
            outcome = store.write_entity_projection(digest, graph)
            status = "rebuilt" if outcome.get("written") else "raced-skip"
            print(
                f"[{index + 1}/{len(sats)}] {status} {digest[:12]} "
                f"entities={outcome.get('entities')}"
            )
            done += 1
        except Exception as exc:  # noqa: BLE001 - 单组件失败不中断整批
            print(f"[fail] {digest[:12]}: {exc}", file=sys.stderr)
            failed += 1

    driver.close()
    print(f"summary: rebuilt={done} skipped(existing)={skipped} failed={failed}")
    return 0 if failed == 0 else 1


if __name__ == "__main__":
    raise SystemExit(main())
