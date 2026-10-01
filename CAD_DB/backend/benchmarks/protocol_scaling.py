"""Reproducible protocol-only scaling experiment; no ACIS, HTTP, or Neo4j."""
from __future__ import annotations

import csv
import json
from pathlib import Path
import statistics
import sys
import time
import tracemalloc

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))
from app.json_collab import graph_component, merge_graph

OUT = Path(__file__).resolve().parent


def body_graph(index: int) -> dict:
    # Synthetic counts observed for a cube in log_A: 60 topology nodes.
    counts = [('body', 1), ('lump', 1), ('shell', 1), ('face', 6),
              ('loop', 6), ('coedge', 24), ('edge', 12), ('vertex', 8), ('transform', 1)]
    nodes = []
    by_kind = {}
    for kind, count in counts:
        ids = [f'b{index}' if kind == 'body' else f'b{index}-{kind}-{n}' for n in range(count)]
        by_kind[kind] = ids
        nodes.extend({'id': node_id, 'labels': [kind], 'props': {'sample': index, 'ordinal': n}}
                     for n, node_id in enumerate(ids))
    rels = []
    chain = [('body', 'lump', 'body_lump'), ('lump', 'shell', 'lump_shell'),
             ('shell', 'face', 'shell_face'), ('face', 'loop', 'face_loop'),
             ('loop', 'coedge', 'loop_start'), ('coedge', 'edge', 'coedge_edge'),
             ('edge', 'vertex', 'edge_start')]
    for src, dst, relation in chain:
        for n, target in enumerate(by_kind[dst]):
            rels.append({'type': relation, 'start': by_kind[src][n % len(by_kind[src])],
                         'end': target, 'props': {}})
    rels.append({'type': 'body_transform', 'start': by_kind['body'][0],
                 'end': by_kind['transform'][0], 'props': {}})
    return {'nodes': nodes, 'rels': rels, 'serializer_build': 'synthetic-cube-topology-v1'}


def compact_bytes(value: dict) -> int:
    return len(json.dumps(value, ensure_ascii=False, separators=(',', ':')).encode('utf-8'))


def measure(size: int, repeats: int = 15) -> dict:
    bodies = [body_graph(i) for i in range(size)]
    full = {'nodes': [n for b in bodies for n in b['nodes']],
            'rels': [r for b in bodies for r in b['rels']],
            'serializer_build': 'synthetic-cube-topology-v1'}
    delta = graph_component(full, ['b0'])
    assert len(delta['nodes']) == 60 and len(delta['rels']) == 59
    times = []
    peaks = []
    for _ in range(repeats):
        tracemalloc.start()
        started = time.perf_counter_ns()
        merged = merge_graph(full, delta, [{'uuid': 'b0', 'changeType': 'MODIFY'}])
        times.append((time.perf_counter_ns() - started) / 1_000_000)
        peaks.append(tracemalloc.get_traced_memory()[1])
        tracemalloc.stop()
        assert len(merged['nodes']) == len(full['nodes'])
    return {'bodies': size, 'full_nodes': len(full['nodes']), 'full_rels': len(full['rels']),
            'delta_nodes': len(delta['nodes']), 'delta_rels': len(delta['rels']),
            'full_json_bytes': compact_bytes(full), 'delta_json_bytes': compact_bytes(delta),
            'delta_fraction': round(compact_bytes(delta) / compact_bytes(full), 5),
            'merge_median_ms': round(statistics.median(times), 3),
            'merge_peak_kib': round(statistics.median(peaks) / 1024, 1)}


def main() -> None:
    rows = [measure(size) for size in (1, 10, 100)]
    with (OUT / 'protocol_scaling.csv').open('w', newline='', encoding='utf-8') as f:
        writer = csv.DictWriter(f, fieldnames=rows[0].keys())
        writer.writeheader()
        writer.writerows(rows)
    import matplotlib.pyplot as plt
    fig, ax = plt.subplots(figsize=(7.2, 4.4))
    xs = list(range(len(rows)))
    width = 0.35
    ax.bar([x - width / 2 for x in xs], [r['full_json_bytes'] / 1024 for r in rows], width, label='Full JSON graph')
    ax.bar([x + width / 2 for x in xs], [r['delta_json_bytes'] / 1024 for r in rows], width, label='One-body JSON delta')
    ax.set_xticks(xs, [str(r['bodies']) for r in rows])
    ax.set_xlabel('Synthetic cube-like bodies in project')
    ax.set_ylabel('Compact UTF-8 JSON payload (KiB)')
    ax.set_title('Measured protocol payload; generated fixture, no network')
    ax.legend()
    ax.grid(axis='y', alpha=.2)
    fig.tight_layout()
    fig.savefig(OUT / 'protocol_scaling.svg')
    plt.close(fig)
    print(json.dumps(rows, ensure_ascii=False, indent=2))


if __name__ == '__main__':
    main()
