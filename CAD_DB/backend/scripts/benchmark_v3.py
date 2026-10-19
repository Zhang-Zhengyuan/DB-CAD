"""v3 存储链路压测（V3_PLAN P9-2，abc_cache 语料）。

对 N 个 abc STEP 模型逐个走全链路：worker import（STEP→组件 SAT/GLB/graph）
→ 投影写入（内容寻址实体 + generation 链）→ 统计与计时。产出：
  - 控制台汇总 + JSON 明细（--out）
  - docs/BENCHMARK_v3.md 的数据源

用途：① 性能基准；② codec 对真实工程模型语料的覆盖度（fallback 分布、
unknown 类型、崩溃/错误清单）——暴露并钉死存储链路 bug。

用法：
    uv run --frozen python scripts/benchmark_v3.py [--limit N] [--out results.json]
"""

from __future__ import annotations

import argparse
import hashlib
import json
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from app.entity_projection import write_projections  # noqa: E402
from app.v1_store import MemoryV1Store  # noqa: E402
from app.worker_client import WorkerError, get_worker_gateway  # noqa: E402

ABC_ROOT = Path("D:/DBCAD/abc_cache/extracted")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--limit", type=int, default=50, help="抽样模型数（默认 50）")
    parser.add_argument("--stride", type=int, default=1, help="抽样步长（隔 stride 取一）")
    parser.add_argument("--out", type=str, default="", help="JSON 明细输出路径")
    args = parser.parse_args()

    steps = sorted(ABC_ROOT.rglob("*.step"))[:: max(1, args.stride)][: args.limit]
    if not steps:
        print("no abc STEP found", file=sys.stderr)
        return 1

    gateway = get_worker_gateway()
    store = MemoryV1Store()  # 投影写入路径与生产同构（Neo4j 之外的真实语义）
    rows = []
    errors = []
    t_import = t_project = 0.0
    totals = {"components": 0, "entities": 0, "entity_added": 0, "entity_shared": 0,
              "derived": 0, "fallback_sat_asset": 0, "fallback_fine": 0, "unknown_labels": 0}

    for index, step in enumerate(steps, 1):
        try:
            started = time.perf_counter()
            imported = gateway.import_sat(step.read_bytes(), step.name)
            t_import += time.perf_counter() - started
        except WorkerError as exc:
            errors.append({"file": step.name, "stage": "import", "error": str(exc)[-300:]})
            print(f"[{index}/{len(steps)}] FAIL import {step.name}: {str(exc)[-120:]}", file=sys.stderr)
            continue
        pairs = []
        comps = len(imported.components)
        started = time.perf_counter()
        try:
            for part in imported.components:
                if part.graph is not None:
                    # 与生产同构：hash_sat = sha256(SAT 字节)
                    pairs.append((hashlib.sha256(part.sat).hexdigest(), part.graph, None))
            stats = write_projections(store, pairs)
        except Exception as exc:  # noqa: BLE001 - 单模型失败不中断整批
            errors.append({"file": step.name, "stage": "projection", "error": str(exc)[-300:]})
            print(f"[{index}/{len(steps)}] FAIL projection {step.name}: {exc}", file=sys.stderr)
            continue
        t_project += time.perf_counter() - started
        fallback = {"sat_asset": 0, "fine_grained": 0}
        unknown = 0
        for _, graph, _ in pairs:
            for node in graph.get("nodes", []):
                props = node.get("props", {})
                if props.get("top_level"):
                    fb = props.get("fallback")
                    if fb in fallback:
                        fallback[fb] += 1
                for label in node.get("labels", []):
                    if label == "unknown":
                        unknown += 1
        totals["components"] += comps
        totals["entities"] += stats["entities"]
        totals["entity_added"] += stats["entity_added"]
        totals["entity_shared"] += stats["entity_shared"]
        totals["derived"] += stats["derived"]
        totals["fallback_sat_asset"] += fallback["sat_asset"]
        totals["fallback_fine"] += fallback["fine_grained"]
        totals["unknown_labels"] += unknown
        rows.append({
            "file": step.name,
            "components": comps,
            "entities": stats["entities"],
            "entity_added": stats["entity_added"],
            "entity_shared": stats["entity_shared"],
            "derived": stats["derived"],
            "fallback": fallback,
            "unknown_labels": unknown,
        })
        print(f"[{index}/{len(steps)}] {step.name}: {comps} comps, "
              f"{stats['entities']} ents (added {stats['entity_added']}, shared {stats['entity_shared']})")

    summary = {
        "models": len(steps),
        "ok": len(rows),
        "failed": len(errors),
        "import_seconds": round(t_import, 2),
        "projection_seconds": round(t_project, 2),
        "totals": totals,
        "content_dedup_ratio": round(
            totals["entity_shared"] / max(1, totals["entities"]), 3
        ),
        "errors": errors[:50],
    }
    print(json.dumps(summary, ensure_ascii=False, indent=2))
    if args.out:
        Path(args.out).write_text(
            json.dumps({"summary": summary, "rows": rows}, ensure_ascii=False, indent=1),
            encoding="utf-8",
        )
        print(f"detail -> {args.out}")
    return 0 if not errors else 1


if __name__ == "__main__":
    raise SystemExit(main())
