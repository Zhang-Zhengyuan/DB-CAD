"""存量 JsonGraphEntity → 原生标签 + 扁平字段（Phase B v2）一次性迁移。

幂等：只补 native_v2 IS NULL 的节点；可对空库 / 已迁移库 / 混合库安全重跑。
迁移只新增字段与标签，graph() 读投影逐字节不变。

用法（backend 目录下）：
    CAD_DB_NEO4J_URI=bolt://127.0.0.1:7687 CAD_DB_NEO4J_PASSWORD=... \
        uv run python scripts/migrate_json_graph_native_v2.py [--dry-run]
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from app.config import settings  # noqa: E402
from app.json_graph_native import migrate_native_v2  # noqa: E402


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--dry-run", action="store_true", help="只统计待迁移节点，不写入")
    args = parser.parse_args()

    if not settings.neo4j_uri:
        print("未配置 CAD_DB_NEO4J_URI", file=sys.stderr)
        return 1
    from neo4j import GraphDatabase

    driver = GraphDatabase.driver(
        settings.neo4j_uri,
        auth=(settings.neo4j_user, settings.neo4j_password),
    )
    try:
        with driver.session(database=settings.neo4j_database) as session:
            pending = session.run(
                "MATCH (e:JsonGraphEntity) WHERE e.native_v2 IS NULL RETURN count(e) AS c"
            ).single()["c"]
            print(f"待迁移 JsonGraphEntity 节点：{int(pending)}")
            if args.dry_run or pending == 0:
                return 0
            report = migrate_native_v2(session)
            print(
                f"迁移完成：{report['migrated']} 个节点，"
                f"跳过复杂字段 {report['skipped_fields']} 个（权威载荷不受影响）"
            )
            return 0
    finally:
        driver.close()


if __name__ == "__main__":
    raise SystemExit(main())
