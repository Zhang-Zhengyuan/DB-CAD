"""逐实体图契约测试（L2，无数据库）。

对每种实体类型构造与 entity_graph_serializer.h 输出一致的图，
校验 validate / split / hash / merge 语义。与 C++ 侧 L1 往返测试
（scripts/tests/acis/graph_roundtrip_tests.cpp）共同构成存储链路的
单元测试层；缺口实体（spline/helix/intcurve）先以当前最小形状锁定
后端存储语义，待客户端补齐载荷后扩展夹具。
"""

import copy

import pytest

from app.json_collab import graph_hash, merge_graph, validate_graph
from app.json_graph_store import split_graph_components


def node(nid, label, props=None):
    return {"id": nid, "labels": [label], "props": props or {}}


def rel(rtype, start, end, props=None):
    return {"type": rtype, "start": start, "end": end, "props": props or {}}


def solid_graph(face_geometry, edge_geometry=None):
    """最小实心体链：body→lump→shell→face(+几何)→loop→coedge→edge(+几何)→vertex。"""
    nodes = [
        node("b1", "body", {"uuid": "b1", "entity_type": "body"}),
        node("l1", "lump", {"entity_type": "lump"}),
        node("s1", "shell", {"entity_type": "shell"}),
        node("f1", "face", {"sense": 1, "sides": 1, "cont": 0, "geometry": face_geometry}),
        node("lo1", "loop", {"entity_type": "loop"}),
        node("c1", "coedge", {"sense": 0, "entity_type": "coedge"}),
        node(
            "e1",
            "edge",
            {
                "sense": 0,
                "start_point": [0, 0, 0],
                "end_point": [1, 0, 0],
                "geometry": edge_geometry
                or {"type": "straight", "root_point": [0, 0, 0], "direction": [1, 0, 0], "range": [2, 0, 2, 1]},
            },
        ),
        node("v1", "vertex", {"position": [0, 0, 0]}),
        node("v2", "vertex", {"position": [1, 0, 0]}),
    ]
    rels = [
        rel("body_lump", "b1", "l1"),
        rel("lump_shell", "l1", "s1"),
        rel("shell_face", "s1", "f1"),
        rel("face_loop", "f1", "lo1"),
        rel("loop_start", "lo1", "c1"),
        rel("coedge_edge", "c1", "e1"),
        rel("edge_start", "e1", "v1"),
        rel("edge_end", "e1", "v2"),
    ]
    return {"nodes": nodes, "rels": rels, "serializer_build": "test-matrix"}


SURFACE_CASES = {
    "plane": {"type": "plane", "root_point": [0, 0, 0], "normal": [0, 0, 1], "u_deriv": [1, 0, 0], "uv_range": [0, 1, 0, 1]},
    "sphere": {"type": "sphere", "centre": [0, 0, 0], "radius": 5.0, "uv_range": []},
    "torus": {"type": "torus", "centre": [0, 0, 0], "normal": [0, 0, 1], "major_radius": 8.0, "minor_radius": 2.0, "uv_range": []},
    "cone": {
        "type": "cone",
        "base_centre": [0, 0, 0],
        "base_normal": [0, 0, 1],
        "base_radius_ratio": 1.0,
        "sine_angle": 0.0,
        "cosine_angle": 1.0,
        "uv_range": [0, 6.28, -10, 10],
    },
    # 缺口1：当前序列化器只发类型名；后端契约按“任意 props 均可存”锁定
    "spline": {"type": "spline"},
}

CURVE_CASES = {
    "straight": {"type": "straight", "root_point": [0, 0, 0], "direction": [1, 0, 0], "range": [2, 0, 2, 1]},
    "ellipse": {
        "type": "ellipse",
        "centre": [0, 0, 0],
        "normal": [0, 0, 1],
        "major_axis": [2, 0, 0],
        "radius_ratio": 1.0,
        "range": [2, 0, 2, 3.14],
    },
    # 缺口3 / 缺口2：同上，先锁最小形状
    "helix": {"type": "helix"},
    "intcurve": {"type": "intcurve"},
}


def assert_single_component(graph):
    components, cross = split_graph_components(graph)
    assert len(components) == 1
    assert components[0]["root_uuid"] == "b1"
    assert cross == []
    return components[0]


@pytest.mark.parametrize("gtype", sorted(SURFACE_CASES))
def test_surface_entity_matrix(gtype):
    graph = solid_graph(SURFACE_CASES[gtype])
    validate_graph(graph)
    component = assert_single_component(graph)
    assert len(component["nodes"]) == 9
    faces = [n for n in component["nodes"] if n["labels"] == ["face"]]
    assert faces[0]["props"]["geometry"]["type"] == gtype


@pytest.mark.parametrize("gtype", sorted(CURVE_CASES))
def test_curve_entity_matrix(gtype):
    graph = solid_graph({"type": "plane", "root_point": [0, 0, 0], "normal": [0, 0, 1], "u_deriv": [1, 0, 0], "uv_range": []},
                        CURVE_CASES[gtype])
    validate_graph(graph)
    component = assert_single_component(graph)
    edges = [n for n in component["nodes"] if n["labels"] == ["edge"]]
    assert edges[0]["props"]["geometry"]["type"] == gtype


def test_wire_body_matrix():
    """线框体：body→wire→coedge→edge→vertex，验证 wire 拓扑边的组件归属。"""
    graph = {
        "nodes": [
            node("b1", "body", {"uuid": "b1"}),
            node("w1", "wire", {"cont": 0}),
            node("c1", "coedge", {"sense": 0}),
            node("e1", "edge", {"sense": 0, "geometry": CURVE_CASES["straight"]}),
            node("v1", "vertex", {"position": [0, 0, 0]}),
            node("v2", "vertex", {"position": [1, 0, 0]}),
        ],
        "rels": [
            rel("body_wire", "b1", "w1"),
            rel("wire_coedge", "w1", "c1"),
            rel("coedge_edge", "c1", "e1"),
            rel("edge_start", "e1", "v1"),
            rel("edge_end", "e1", "v2"),
        ],
    }
    validate_graph(graph)
    assert_single_component(graph)


def test_transform_matrix():
    graph = solid_graph(SURFACE_CASES["plane"])
    graph["nodes"].append(node("t1", "transform", {"affine": [1, 0, 0, 0, 1, 0, 0, 0, 1], "translation": [10, 20, 30], "scaling": 2.0}))
    graph["rels"].append(rel("body_transform", "b1", "t1"))
    validate_graph(graph)
    component = assert_single_component(graph)
    assert any(n["labels"] == ["transform"] for n in component["nodes"])


def test_subshell_matrix():
    """subshell（缺口4）：后端存储语义先行锁定，待客户端序列化补齐。"""
    graph = solid_graph(SURFACE_CASES["plane"])
    graph["nodes"] += [node("ss1", "subshell"), node("f2", "face", {"sense": 1, "sides": 1, "geometry": SURFACE_CASES["plane"]})]
    graph["rels"] += [rel("shell_subshell", "s1", "ss1"), rel("subshell_face", "ss1", "f2")]
    validate_graph(graph)
    component = assert_single_component(graph)
    labels = {n["labels"][0] for n in component["nodes"]}
    assert {"subshell"} <= labels


def test_pcurve_matrix():
    """pcurve（缺口5）：作为 coedge 几何挂载时的存储语义。"""
    graph = solid_graph(SURFACE_CASES["plane"])
    graph["nodes"].append(node("p1", "pcurve", {"def_type": 0, "off_du": 0.0, "off_dv": 0.0, "rev": 0}))
    graph["rels"].append(rel("coedge_geometry", "c1", "p1"))
    validate_graph(graph)
    assert_single_component(graph)


def test_two_bodies_split_and_cross_relation():
    """双 body 图：拆成两个内容寻址组件，跨组件设计依赖边进入 relations 列表。"""
    first = solid_graph(SURFACE_CASES["sphere"])
    second = solid_graph(SURFACE_CASES["sphere"])
    for item in second["nodes"]:
        item["id"] += "-2"
        if item["labels"] == ["body"]:
            item["props"] = {"uuid": "b2"}
    for item in second["rels"]:
        item["start"] += "-2"
        item["end"] += "-2"
    graph = {
        "nodes": first["nodes"] + second["nodes"],
        "rels": first["rels"] + second["rels"] + [rel("mate_edge", "e1", "e1-2")],
    }
    validate_graph(graph)
    components, cross = split_graph_components(graph)
    assert {c["root_uuid"] for c in components} == {"b1", "b1-2"}
    assert len(cross) == 1 and cross[0]["type"] == "mate_edge"


def test_graph_hash_stability_and_sensitivity():
    base = solid_graph(SURFACE_CASES["sphere"])
    same = copy.deepcopy(base)
    assert graph_hash(base) == graph_hash(same)
    changed = copy.deepcopy(base)
    for n in changed["nodes"]:
        if n["labels"] == ["face"]:
            n["props"]["geometry"]["radius"] = 6.0
    assert graph_hash(base) != graph_hash(changed)


def test_merge_replaces_whole_component():
    """merge_graph 按 body 组件整体替换：改 radius 后节点数不变、旧组件不残留。"""
    base = solid_graph(SURFACE_CASES["sphere"])
    incoming = solid_graph(SURFACE_CASES["sphere"])
    for n in incoming["nodes"]:
        if n["labels"] == ["face"]:
            n["props"]["geometry"]["radius"] = 6.0
    merged = merge_graph(base, incoming, changes=[{"uuid": "b1"}])
    validate_graph(merged)
    assert len(merged["nodes"]) == len(base["nodes"])
    radii = [n["props"]["geometry"]["radius"] for n in merged["nodes"] if n["labels"] == ["face"]]
    assert radii == [6.0]


def test_merge_removes_component():
    base = {
        "nodes": [node("b1", "body", {"uuid": "b1"}), node("v1", "vertex", {"position": [0, 0, 0]})],
        "rels": [rel("body_lump", "b1", "v1")],
    }
    merged = merge_graph(base, {"nodes": [], "rels": []}, changes=[], removed_ids=["b1"])
    assert all(n["id"] != "b1" for n in merged["nodes"])
