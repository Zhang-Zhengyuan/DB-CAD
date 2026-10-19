// v3 实体图投影 codec 实现（V3_PLAN.md P6 / ADR-3）。
//
// 移植自 entity_graph_serializer.cpp（uvsafe-topology-v5-bounded）：遍历方式、
// 属性提取、区间/NURBS 编码、风险判定逐段对应；容器 QJson*→nlohmann、
// QHash/QQueue/QStack/QSet→std。sat_fragment 内嵌裁剪为 fallback 标记
// （v3 组件 SAT 是独立内容寻址资产，见 entity_graph_codec.h 头注）。
#include "entity_graph_codec.h"

#include <acis/include/api.hxx>
#include <acis/include/cstrapi.hxx>
#include <acis/include/kernapi.hxx>
#include <acis/include/rnd_api.hxx>
#include <acis/include/allcurve.hxx>
#include <acis/include/allsurf.hxx>
#include <acis/include/exct_spl.hxx>
#include <acis/include/rot_spl.hxx>
#include <acis/include/sps3crtn.hxx>
#include <acis/include/sp3crtn.hxx>
#include <acis/include/pcudef.hxx>
#include <acis/include/pcurve.hxx>
#include <acis/include/sps2crtn.hxx>
#include <acis/include/sps3srtn.hxx>
#include <acis/include/alltop.hxx>
#include <acis/include/body.hxx>
#include <acis/include/curve.hxx>
#include <acis/include/surface.hxx>
#include <acis/include/point.hxx>
#include <acis/include/intcurve.hxx>
#include <acis/include/transfrm.hxx>
#include <acis/include/elldef.hxx>

#include <cmath>
#include <cstdio>
#include <queue>
#include <stack>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifdef DBCAD_GRAPH_TRACE
#define GRAPH_TRACE(...) std::fprintf(stderr, "[graph] " __VA_ARGS__)
#else
#define GRAPH_TRACE(...) ((void)0)
#endif

namespace dbcad::worker {
namespace {

using nlohmann::json;

namespace codec {

// SPAposition/SPAvector/SPAunit_vector → [x, y, z]
inline json posToJson(const SPAposition& p) { return json::array({p.x(), p.y(), p.z()}); }
inline json vecToJson(const SPAvector& v) { return json::array({v.x(), v.y(), v.z()}); }
inline json unitVecToJson(const SPAunit_vector& v) { return json::array({v.x(), v.y(), v.z()}); }

// SPAinterval → [type1, val1, type2, val2]（恒 4 元素，与 v2 编码一致：
// 旧实现无限/半无限区间只输出 2-3 元素会导致对端塌缩成 [0,1]）。
inline json intervalToJson(const SPAinterval& iv) {
    json arr = json::array();
    if (iv.empty()) {
        arr.push_back(1.0);
        arr.push_back(0.0);
        arr.push_back(1.0);
        arr.push_back(0.0);
    } else {
        interval_type t = iv.type();
        if (t == interval_type::interval_finite) {
            arr.push_back(2.0);
            arr.push_back(iv.start_pt());
            arr.push_back(2.0);
            arr.push_back(iv.end_pt());
        } else if (t == interval_type::interval_finite_below) {
            arr.push_back(2.0);
            arr.push_back(iv.start_pt());
            arr.push_back(1.0);
            arr.push_back(0.0);
        } else if (t == interval_type::interval_finite_above) {
            arr.push_back(1.0);
            arr.push_back(0.0);
            arr.push_back(2.0);
            arr.push_back(iv.end_pt());
        } else {
            arr.push_back(1.0);
            arr.push_back(0.0);
            arr.push_back(1.0);
            arr.push_back(0.0);
        }
    }
    return arr;
}

// 边几何参数区间：优先曲线方程自身 param_range（绝对参数空间，与 SAT 一致）；
// 无限时输出 I I 形态。EDGE bound 与曲线参数空间存在错位（弧线区间错位会取
// 错弧段），故不混用；退化边（长度 0）须用 EDGE 自身空跨度区间。
inline json curveRangeToJson(const curve& crv, const EDGE* edge) {
    if (edge && edge->length() < SPAresabs) {
        SPAinterval er = edge->get_param_range();
        if (er.type() == interval_type::interval_finite) return intervalToJson(er);
    }
    SPAinterval r = crv.param_range();
    if (!r.empty() && r.type() == interval_type::interval_finite) {
        double period = crv.param_period();  // 非周期曲线为 0；未受限整圆求值成整周期
        if (period <= 0 || (r.end_pt() - r.start_pt()) < period - 1e-9) return intervalToJson(r);
        json unbounded = json::array({1.0, 0.0, 1.0, 0.0});
        return unbounded;  // 覆盖整周期 = 未受限周期曲线（SAT 落盘 I I）
    }
    return intervalToJson(edge->get_param_range());
}

// SPApar_box → [u0, u1, v0, v1]；仅保留真实有序有限区间（空/非有限子集
// 序列化成 [0,0,..] 会在消费端产生退化有限区间）。
inline json parBoxToJson(const SPApar_box& box) {
    json arr = json::array();
    SPAinterval uRange = box.u_range();
    SPAinterval vRange = box.v_range();
    const bool validU = !uRange.empty() && std::isfinite(uRange.start_pt()) && std::isfinite(uRange.end_pt()) &&
                        uRange.end_pt() > uRange.start_pt();
    const bool validV = !vRange.empty() && std::isfinite(vRange.start_pt()) && std::isfinite(vRange.end_pt()) &&
                        vRange.end_pt() > vRange.start_pt();
    if (validU && validV) {
        arr.push_back(uRange.start_pt());
        arr.push_back(uRange.end_pt());
        arr.push_back(vRange.start_pt());
        arr.push_back(vRange.end_pt());
    }
    return arr;
}

// SPAmatrix → [9 floats]（列主序）；SPAtransf → JSON 对象（与 v2 载荷对应）。
inline json matrixToJson(const SPAmatrix& m) {
    return json::array({m.element(0, 0), m.element(0, 1), m.element(0, 2), m.element(1, 0), m.element(1, 1),
                        m.element(1, 2), m.element(2, 0), m.element(2, 1), m.element(2, 2)});
}

inline json transfToJson(const SPAtransf& tf) {
    json obj;
    obj["affine"] = matrixToJson(tf.affine());
    obj["translation"] = vecToJson(tf.translation());
    obj["scaling"] = tf.scaling();
    obj["rotate"] = tf.rotate();
    obj["reflect"] = tf.reflect();
    obj["shear"] = tf.shear();
    return obj;
}

const char* curveTypeName(ENTITY* e) {
    if (!e) return "unknown";
    switch (e->identity(2)) {
    case STRAIGHT_ID:
        return "straight";
    case ELLIPSE_ID:
        return "ellipse";
    case HELIX_ID:
        return "helix";
    case INTCURVE_ID:
        return "intcurve";
    default:
        return "unknown_curve";
    }
}

const char* surfaceTypeName(ENTITY* e) {
    if (!e) return "unknown";
    switch (e->identity(2)) {
    case PLANE_ID:
        return "plane";
    case SPHERE_ID:
        return "sphere";
    case TORUS_ID:
        return "torus";
    case CONE_ID:
        return "cone";
    case SPLINE_ID:
        return "spline";
    default:
        return "unknown_surface";
    }
}

// 合并（不覆盖已有键；nlohmann update() 会覆盖，保持 v2 语义手写循环）。
inline void mergeJsonInto(json& dst, const json& src) {
    for (auto it = src.begin(); it != src.end(); ++it)
        if (!dst.contains(it.key())) dst[it.key()] = it.value();
}

// SURFACE 实体 → 方程对象（int_int_cur 定义源宿主面提取；未知类型返回 null）。
inline const surface* surfaceEntityEquation(SURFACE* se) {
    if (!se) return nullptr;
    switch (se->identity(2)) {
    case PLANE_ID:
        return &((PLANE*)se)->equation();
    case SPHERE_ID:
        return &((SPHERE*)se)->equation();
    case TORUS_ID:
        return &((TORUS*)se)->equation();
    case CONE_ID:
        return &((CONE*)se)->equation();
    case SPLINE_ID:
        return &((SPLINE*)se)->equation();
    default:
        return nullptr;
    }
}

// 节点向量 RLE：[knot, mult, ...]，端点多重性 -1（与 v2/Bolt 栈一致）。
inline json rleKnots(const double* knots, int n) {
    std::vector<double> ks;
    for (int i = 0; i < n; ++i) {
        double v = knots[i];
        if (ks.empty() || v != ks[ks.size() - 2]) {
            ks.push_back(v);
            ks.push_back(1);
        } else {
            ks.back()++;
        }
    }
    if (ks.size() >= 4) {
        ks[1]--;
        ks.back()--;
    }
    json arr = json::array();
    for (double v : ks) arr.push_back(v);
    return arr;
}

// spl_sur NURBS 载荷（face.geometry 与 intcurve 定义曲面共用布局）。
inline json splSurToJson(const spl_sur& ss) {
    json o;
    int dim = 0, form_u = 0, form_v = 0, pole_u = 0, pole_v = 0, num_u = 0, num_v = 0;
    int degree_u = 0, num_uknots = 0, degree_v = 0, num_vknots = 0;
    logical rational_u = 0, rational_v = 0;
    SPAposition* ctrlpts = nullptr;
    double *weights = nullptr, *uknots = nullptr, *vknots = nullptr;
    bs3_surface_to_array(ss.sur(), dim, rational_u, rational_v, form_u, form_v, pole_u, pole_v, num_u, num_v,
                         ctrlpts, weights, degree_u, num_uknots, uknots, degree_v, num_vknots, vknots, 0);
    o["rational_u"] = (int)rational_u;
    o["rational_v"] = (int)rational_v;
    o["degree_u"] = degree_u;
    o["degree_v"] = degree_v;
    o["form_u"] = form_u;
    o["form_v"] = form_v;
    o["u_singularity"] = pole_u;
    o["v_singularity"] = pole_v;
    o["num_u"] = num_u;
    o["num_v"] = num_v;
    o["uknots"] = rleKnots(uknots, num_uknots);
    o["vknots"] = rleKnots(vknots, num_vknots);
    bool rational = rational_u || rational_v;
    json ctrlArr = json::array();
    for (int i = 0; i < num_v; ++i) {
        for (int j = 0; j < num_u; ++j) {
            SPAposition* pt = ctrlpts + j * num_v + i;
            ctrlArr.push_back(pt->x());
            ctrlArr.push_back(pt->y());
            ctrlArr.push_back(pt->z());
            if (rational) ctrlArr.push_back(weights[j * num_v + i]);
        }
    }
    o["ctrlpts"] = ctrlArr;
    o["fitol"] = ss.fitol();
    return o;
}

// 曲线方程载荷（rot_spl_sur 旋转面母线）。schema 与 edge.geometry 一致。
inline json curveEquationToJson(const curve* c) {
    json o;
    if (!c) return o;
    switch (c->type()) {
    case straight_type: {
        const straight* s = (const straight*)c;
        o["type"] = "straight";
        o["root_point"] = posToJson(s->root_point);
        // direction 携带 param_scale（模长=参数比例）：只发单位向量会把
        // [0,1]×scale 的边塌缩成长度 1。
        o["direction"] = vecToJson(SPAvector(s->direction.x() * s->param_scale, s->direction.y() * s->param_scale,
                                             s->direction.z() * s->param_scale));
        break;
    }
    case ellipse_type: {
        const ellipse* e = (const ellipse*)c;
        o["type"] = "ellipse";
        o["centre"] = posToJson(e->centre);
        o["normal"] = unitVecToJson(e->normal);
        o["major_axis"] = vecToJson(e->major_axis);
        o["radius_ratio"] = e->radius_ratio;
        break;
    }
    case intcurve_type: {
        const intcurve* ic = (const intcurve*)c;
        bs3_curve bs = ic->cur();
        if (bs) {
            o["type"] = "intcurve";
            int dim = 0, deg = 0, numCtrl = 0, numKnots = 0;
            logical rat = 0;
            SPAposition* ctrl = nullptr;
            double *weights = nullptr, *knots = nullptr;
            bs3_curve_to_array(bs, dim, deg, rat, numCtrl, ctrl, weights, numKnots, knots);
            o["rational"] = (int)rat;
            o["degree"] = deg;
            o["form"] = bs3_curve_periodic(bs) ? 2 : (bs3_curve_closed(bs) ? 1 : 0);
            o["knots"] = rleKnots(knots, numKnots);
            json ct = json::array();
            for (int i = 0; i < numCtrl; ++i) {
                ct.push_back(ctrl[i].x());
                ct.push_back(ctrl[i].y());
                ct.push_back(ctrl[i].z());
                if (rat) ct.push_back(weights[i]);
            }
            o["ctrlpts"] = ct;
            o["fitol"] = ic->fitol();
        }
        break;
    }
    default:
        break;
    }
    return o;
}

// 方程级曲面载荷（intcurve 定义曲面 surf1/surf2）。schema 与 face.geometry 一致。
inline json surfaceEquationToJson(const surface* s) {
    json o;
    if (!s) return o;
    switch (s->type()) {
    case plane_type: {
        const plane* p = (const plane*)s;
        o["type"] = "plane";
        o["root_point"] = posToJson(p->root_point);
        o["normal"] = unitVecToJson(p->normal);
        o["u_deriv"] = vecToJson(p->u_deriv);
        break;
    }
    case sphere_type: {
        const sphere* p = (const sphere*)s;
        o["type"] = "sphere";
        o["centre"] = posToJson(p->centre);
        o["radius"] = p->radius;
        break;
    }
    case torus_type: {
        const torus* p = (const torus*)s;
        o["type"] = "torus";
        o["centre"] = posToJson(p->centre);
        o["normal"] = unitVecToJson(p->normal);
        o["major_radius"] = p->major_radius;
        o["minor_radius"] = p->minor_radius;
        break;
    }
    case cone_type: {
        const cone* p = (const cone*)s;
        ellipse base = p->base;
        o["type"] = "cone";
        o["base_centre"] = posToJson(base.centre);
        o["base_normal"] = unitVecToJson(base.normal);
        o["base_major_axis"] = vecToJson(base.major_axis);
        o["base_radius_ratio"] = base.radius_ratio;
        o["sine_angle"] = p->sine_angle;
        o["cosine_angle"] = p->cosine_angle;
        o["u_param_scale"] = p->u_param_scale;
        break;
    }
    case spline_type: {
        const spline* sp = (const spline*)s;
        o["type"] = "spline";
        mergeJsonInto(o, splSurToJson(sp->get_spl_sur()));
        break;
    }
    default:
        break;
    }
    return o;
}

}  // namespace codec

std::string genNodeId(const void* ptr, int typeId) {
    char buf[64];
    std::snprintf(buf, sizeof(buf), "n_%p_%d", ptr, typeId);
    return std::string(buf);
}

struct IntNode {
    std::string id;
    std::string label;
    json props = json::object();
};

bool nodeRisky(const IntNode& n, ENTITY* en) {
    const json g = n.props.contains("geometry") ? n.props["geometry"] : json::object();
    if (n.label == "face") {
        if (g.empty()) {
            GRAPH_TRACE("fallback-trigger face-no-geometry id=%s\n", n.id.c_str());
            return true;
        }
        const std::string t = g.value("type", std::string());
        if (t == "unknown_surface") {
            GRAPH_TRACE("fallback-trigger unknown-surface id=%s\n", n.id.c_str());
            return true;
        }
        if (t == "spline") {
            const int sub = g.value("spl_subtype", -1);
            // 44=rot_spl_sur 已支持过程载荷精确重建；67=sweepsur 等为过程曲面
            //（NURBS 仅拟合）→ 标记为需 SAT 资产兜底。
            if (sub != 37 && sub != 60 && sub != 63 && sub != 44) {
                GRAPH_TRACE("fallback-trigger procedural-spline subtype=%d id=%s\n", sub, n.id.c_str());
                return true;
            }
        }
    } else if (n.label == "edge") {
        // 无 geometry 的边是合法退化结构（如圆锥锥顶边），不算风险。
        const std::string t = g.value("type", std::string());
        if (t == "unknown_curve" || g.contains("nobs3")) {
            GRAPH_TRACE("fallback-trigger edge-curve-risk(%s%s) id=%s\n", t.c_str(),
                        g.contains("nobs3") ? "+nobs3" : "", n.id.c_str());
            return true;
        }
    } else if (n.label == "coedge") {
        if (g.contains("nobs2")) {
            GRAPH_TRACE("fallback-trigger pcurve-nobs2 id=%s\n", n.id.c_str());
            return true;
        }
    }
    // ATTRIB 属性链存在性（即使载荷层不支持也要保真，不能静默丢）。同时检查
    // 几何方程实体（SURFACE/CURVE/PCURVE/APOINT）上的属性——它们不是图节点，
    // 拓扑遍历结构性不可见。
    if (en) {
        if (en->attrib() != nullptr) {
            GRAPH_TRACE("fallback-trigger attrib-chain id=%s label=%s\n", n.id.c_str(), n.label.c_str());
            return true;
        }
        ENTITY* geoms[4] = {nullptr, nullptr, nullptr, nullptr};
        if (n.label == "face" && ((FACE*)en)->geometry()) geoms[0] = (ENTITY*)((FACE*)en)->geometry();
        if (n.label == "edge" && ((EDGE*)en)->geometry()) geoms[1] = (ENTITY*)((EDGE*)en)->geometry();
        if (n.label == "coedge" && ((COEDGE*)en)->geometry()) geoms[2] = (ENTITY*)((COEDGE*)en)->geometry();
        if (n.label == "vertex" && ((VERTEX*)en)->geometry()) geoms[3] = (ENTITY*)((VERTEX*)en)->geometry();
        for (ENTITY* g2 : geoms) {
            if (g2 && g2->attrib() != nullptr) {
                GRAPH_TRACE("fallback-trigger attrib-on-geometry id=%s label=%s\n", n.id.c_str(), n.label.c_str());
                return true;
            }
        }
    }
    return false;
}

}  // namespace

json serializeEntityGraph(const std::string& componentSid, ENTITY& rootEntity) {
    using namespace codec;

    json nodesJson = json::array();
    json relsJson = json::array();

    std::unordered_map<const void*, std::string> ptrToId;
    // 保留插入顺序（std::vector）——nodes 数组顺序即遍历顺序，投影消费方依赖稳定序。
    std::vector<std::pair<const void*, IntNode>> orderedNodes;

    std::queue<ENTITY*> queue;
    std::unordered_set<const void*> visited;
    queue.push(&rootEntity);
    visited.insert(&rootEntity);

    auto enqueueChild = [&](ENTITY* child) {
        if (child && visited.insert(child).second) queue.push(child);
    };

    while (!queue.empty()) {
        ENTITY* e = queue.front();
        queue.pop();
        if (!e) continue;
        const void* ePtr = static_cast<const void*>(e);
        if (ptrToId.count(ePtr)) continue;

        IntNode node;

        switch (e->identity(1)) {

        case BODY_ID: {
            BODY* body = (BODY*)e;
            node.label = "body";
            node.id = (&rootEntity == e) ? componentSid : genNodeId(e, BODY_ID);
            node.props["entity_type"] = "body";
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY*)body->lump());
            enqueueChild((ENTITY*)body->wire());
            enqueueChild((ENTITY*)body->transform());
            break;
        }
        case LUMP_ID: {
            LUMP* lump = (LUMP*)e;
            node.label = "lump";
            node.id = genNodeId(e, LUMP_ID);
            node.props["entity_type"] = "lump";
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY*)lump->shell());
            enqueueChild((ENTITY*)lump->next());
            enqueueChild((ENTITY*)lump->body());
            break;
        }
        case SHELL_ID: {
            SHELL* shell = (SHELL*)e;
            node.label = "shell";
            node.id = genNodeId(e, SHELL_ID);
            node.props["entity_type"] = "shell";
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY*)shell->next());
            enqueueChild((ENTITY*)shell->face());
            enqueueChild((ENTITY*)shell->wire());
            enqueueChild((ENTITY*)shell->lump());
            enqueueChild((ENTITY*)shell->subshell());
            break;
        }
        case SUBSHELL_ID: {
            SUBSHELL* ss = (SUBSHELL*)e;
            node.label = "subshell";
            node.id = genNodeId(e, SUBSHELL_ID);
            node.props["entity_type"] = "subshell";
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY*)ss->parent());
            enqueueChild((ENTITY*)ss->sibling());
            enqueueChild((ENTITY*)ss->child());
            enqueueChild((ENTITY*)ss->face());
            enqueueChild((ENTITY*)ss->wire());
            break;
        }
        case WIRE_ID: {
            WIRE* wire = (WIRE*)e;
            node.label = "wire";
            node.id = (&rootEntity == e) ? componentSid : genNodeId(e, WIRE_ID);
            node.props["entity_type"] = "wire";
            node.props["cont"] = wire->cont();
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY*)wire->next());
            enqueueChild((ENTITY*)wire->coedge());
            enqueueChild((ENTITY*)wire->subshell());
            enqueueChild((ENTITY*)wire->owner());
            break;
        }
        case FACE_ID: {
            FACE* face = (FACE*)e;
            node.label = "face";
            node.id = (&rootEntity == e) ? componentSid : genNodeId(e, FACE_ID);
            json props;
            props["entity_type"] = "face";
            props["sense"] = face->sense();
            props["sides"] = face->sides() ? 1 : 0;
            if (face->sides()) props["cont"] = face->cont();
            if (face->geometry()) {
                ENTITY* geom = face->geometry();
                json gj;
                gj["type"] = surfaceTypeName(geom);
                SPApar_box uvrange = ((surface*)geom)->subset_box();
                gj["uv_range"] = parBoxToJson(uvrange);
                switch (geom->identity(2)) {
                case PLANE_ID: {
                    const plane& gem = static_cast<const plane&>(((PLANE*)geom)->equation());
                    gj["root_point"] = posToJson(gem.root_point);
                    gj["normal"] = unitVecToJson(gem.normal);
                    gj["u_deriv"] = vecToJson(gem.u_deriv);
                    break;
                }
                case SPHERE_ID: {
                    const sphere& gem = static_cast<const sphere&>(((SPHERE*)geom)->equation());
                    gj["centre"] = posToJson(gem.centre);
                    gj["radius"] = gem.radius;
                    break;
                }
                case TORUS_ID: {
                    const torus& gem = static_cast<const torus&>(((TORUS*)geom)->equation());
                    gj["centre"] = posToJson(gem.centre);
                    gj["normal"] = unitVecToJson(gem.normal);
                    gj["major_radius"] = gem.major_radius;
                    gj["minor_radius"] = gem.minor_radius;
                    break;
                }
                case CONE_ID: {
                    const cone& gem = static_cast<const cone&>(((CONE*)geom)->equation());
                    ellipse base = gem.base;
                    gj["base_centre"] = posToJson(base.centre);
                    gj["base_normal"] = unitVecToJson(base.normal);
                    gj["base_major_axis"] = vecToJson(base.major_axis);
                    gj["base_radius_ratio"] = base.radius_ratio;
                    gj["sine_angle"] = gem.sine_angle;
                    gj["cosine_angle"] = gem.cosine_angle;
                    gj["u_param_scale"] = gem.u_param_scale;
                    break;
                }
                case SPLINE_ID: {
                    const spline& gem = static_cast<const spline&>(((SPLINE*)geom)->equation());
                    const spl_sur& ss = gem.get_spl_sur();
                    mergeJsonInto(gj, splSurToJson(ss));
                    gj["reversed"] = (int)gem.reversed();
                    gj["spl_subtype"] = ss.type();
                    // 旋转面过程载荷（rot_spl_sur 无 law、数据成员公开）：
                    // 母线 + 轴 + u/v 参数区间。
                    if (ss.type() == 44 && ((const rot_spl_sur&)ss).cur) {
                        const rot_spl_sur* rs = (const rot_spl_sur*)&ss;
                        json rp = curveEquationToJson(rs->cur);
                        if (!rp.empty()) {
                            gj["rot_profile"] = rp;
                            gj["rot_axis_root"] = posToJson(rs->axis_root);
                            gj["rot_axis_dir"] = unitVecToJson(rs->axis_dir);
                            SPApar_box pr = ss.param_range();
                            gj["rot_uv"] = json::array({pr.u_range().start_pt(), pr.u_range().end_pt(),
                                                        pr.v_range().start_pt(), pr.v_range().end_pt()});
                        }
                    }
                    break;
                }
                default:
                    break;
                }
                props["geometry"] = gj;
            }
            node.props = props;
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY*)face->next());
            enqueueChild((ENTITY*)face->loop());
            enqueueChild((ENTITY*)face->shell());
            break;
        }
        case LOOP_ID: {
            LOOP* loop = (LOOP*)e;
            node.label = "loop";
            node.id = genNodeId(e, LOOP_ID);
            node.props["entity_type"] = "loop";
            // loop 分类（periphery/hole/unknown…）是 SAT 落盘字段，必须保留。
            node.props["classification"] = (int)loop->get_classification();
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY*)loop->next());
            enqueueChild((ENTITY*)loop->start());
            enqueueChild((ENTITY*)loop->face());
            break;
        }
        case COEDGE_ID: {
            COEDGE* co = (COEDGE*)e;
            node.label = "coedge";
            node.id = genNodeId(e, COEDGE_ID);
            node.props["entity_type"] = "coedge";
            node.props["sense"] = co->sense();
            // pcurve 载荷（bs2 参数域拟合）。宿主曲面不重复序列化——它就是
            // 所属 face 的 geometry，消费方经拓扑关系反查。
            if (co->geometry() && is_PCURVE(co->geometry())) {
                const pcurve& pc = static_cast<const pcurve&>(((PCURVE*)co->geometry())->equation());
                bs2_curve bs2 = pc.cur();
                if (bs2 != nullptr) {
                    json gj;
                    gj["type"] = "pcurve";
                    int dim2 = 0, deg2 = 0, numCtrl2 = 0, numKnots2 = 0;
                    logical rat2 = 0;
                    SPAposition* ctrl2 = nullptr;
                    double *weights2 = nullptr, *knots2 = nullptr;
                    bs2_curve_to_array(bs2, dim2, deg2, rat2, numCtrl2, ctrl2, weights2, numKnots2, knots2);
                    int form2 = 0;
                    if (bs2_curve_closed(bs2)) form2 = bs2_curve_periodic(bs2) ? 2 : 1;
                    gj["rational"] = (int)rat2;
                    gj["degree"] = deg2;
                    gj["form"] = form2;
                    gj["num_ctrlpts"] = numCtrl2;
                    gj["knots"] = rleKnots(knots2, numKnots2);
                    json ct2 = json::array();
                    for (int i = 0; i < numCtrl2; ++i) {
                        ct2.push_back(ctrl2[i].x());
                        ct2.push_back(ctrl2[i].y());
                        if (rat2) ct2.push_back(weights2[i]);
                    }
                    gj["ctrlpts"] = ct2;
                    gj["fitol"] = pc.fitol();
                    node.props["geometry"] = gj;
                } else {
                    // 无 bs2（纯拟合式 pcurve）：载荷仅类型名 → 标记需 SAT 资产。
                    json gjF;
                    gjF["type"] = "pcurve";
                    gjF["nobs2"] = 1;
                    node.props["geometry"] = gjF;
                }
            }
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY*)co->next());
            enqueueChild((ENTITY*)co->previous());
            enqueueChild((ENTITY*)co->partner());
            enqueueChild((ENTITY*)co->edge());
            enqueueChild((ENTITY*)co->owner());
            break;
        }
        case EDGE_ID: {
            EDGE* edge = (EDGE*)e;
            node.label = "edge";
            node.id = (&rootEntity == e) ? componentSid : genNodeId(e, EDGE_ID);
            SPAposition sp, ep;
            bool haveSp = false, haveEp = false;
            if (edge->start()) {
                VERTEX* sv = edge->start();
                if (sv->geometry() && sv->geometry()->identity(0) == APOINT_ID) {
                    sp = ((APOINT*)sv->geometry())->coords();
                    haveSp = true;
                }
            }
            if (edge->end()) {
                VERTEX* ev = edge->end();
                if (ev->geometry() && ev->geometry()->identity(0) == APOINT_ID) {
                    ep = ((APOINT*)ev->geometry())->coords();
                    haveEp = true;
                }
            }
            if (edge->sense() != 0) {
                std::swap(sp, ep);
                std::swap(haveSp, haveEp);
            }
            json props;
            props["entity_type"] = "edge";
            props["sense"] = edge->sense();
            // 凸性（tangent/convex/concave/...）：丢失会影响下游评估。
            props["convexity"] = (int)edge->get_convexity();
            if (haveSp) props["start_point"] = posToJson(sp);
            if (haveEp) props["end_point"] = posToJson(ep);
            if (edge->geometry()) {
                ENTITY* geom = edge->geometry();
                json gj;
                gj["type"] = curveTypeName(geom);
                switch (geom->identity(2)) {
                case STRAIGHT_ID: {
                    const straight& gem = static_cast<const straight&>(((STRAIGHT*)geom)->equation());
                    gj["root_point"] = posToJson(gem.root_point);
                    gj["direction"] = vecToJson(SPAvector(gem.direction.x() * gem.param_scale,
                                                          gem.direction.y() * gem.param_scale,
                                                          gem.direction.z() * gem.param_scale));
                    // 曲线区间：曲线方程自身 param_range（无界 → I I）。
                    SPAinterval r = gem.param_range();
                    if (!r.empty() && r.type() == interval_type::interval_finite)
                        gj["range"] = intervalToJson(r);
                    else
                        gj["range"] = json::array({1.0, 0.0, 1.0, 0.0});
                    break;
                }
                case ELLIPSE_ID: {
                    const ellipse& gem = static_cast<const ellipse&>(((ELLIPSE*)geom)->equation());
                    gj["centre"] = posToJson(gem.centre);
                    gj["normal"] = unitVecToJson(gem.normal);
                    gj["major_axis"] = vecToJson(gem.major_axis);
                    gj["radius_ratio"] = gem.radius_ratio;
                    gj["range"] = curveRangeToJson(gem, edge);
                    break;
                }
                case HELIX_ID: {
                    const helix& gem = static_cast<const helix&>(((HELIX*)geom)->equation());
                    gj["axis_root"] = posToJson(gem.axis_root());
                    gj["axis_dir"] = unitVecToJson(gem.axis_dir());
                    gj["start_disp"] = vecToJson(gem.start_disp());
                    gj["pitch"] = gem.pitch();
                    gj["handedness"] = (int)gem.handedness();
                    gj["par_scaling"] = gem.par_scaling();
                    gj["taper"] = gem.taper();
                    gj["helix_range"] = intervalToJson(gem.helix_range());
                    gj["range"] = curveRangeToJson(gem, edge);
                    break;
                }
                case INTCURVE_ID: {
                    // 逼近 NURBS 载荷（bs3）为主表示（足够评估：体积/渲染/布尔）。
                    const intcurve& icv = static_cast<const intcurve&>(((INTCURVE*)geom)->equation());
                    bs3_curve bs = icv.cur();
                    if (bs != nullptr) {
                        int dim = 0, deg = 0, numCtrl = 0, numKnots = 0;
                        logical rat = 0;
                        SPAposition* ctrl = nullptr;
                        double *weights = nullptr, *knots = nullptr;
                        bs3_curve_to_array(bs, dim, deg, rat, numCtrl, ctrl, weights, numKnots, knots);
                        int form = 0;
                        if (bs3_curve_closed(bs)) form = bs3_curve_periodic(bs) ? 2 : 1;
                        gj["rational"] = (int)rat;
                        gj["degree"] = deg;
                        gj["form"] = form;
                        gj["knots"] = rleKnots(knots, numKnots);
                        json ctrlArr = json::array();
                        for (int i = 0; i < numCtrl; ++i) {
                            ctrlArr.push_back(ctrl[i].x());
                            ctrlArr.push_back(ctrl[i].y());
                            ctrlArr.push_back(ctrl[i].z());
                            if (rat) ctrlArr.push_back(weights[i]);
                        }
                        gj["ctrlpts"] = ctrlArr;
                        gj["fitol"] = icv.fitol();
                    } else {
                        // 无 bs3（纯定义式交线）：载荷仅类型名 → 标记需 SAT 资产。
                        gj["nobs3"] = 1;
                    }
                    gj["range"] = curveRangeToJson(icv, edge);
                    // 定义载荷：交线的定义曲面 + safe_range（其支撑面破坏恢复体
                    // 上的下游算法）。定义源两级：物化参数曲线宿主面；无公共曲面
                    // 访问器时用边的宿主面曲面（几何等价：交线落在两张宿主面上）。
                    {
                        pcurve *pc1 = icv.pcur(1, TRUE), *pc2 = icv.pcur(2, TRUE);
                        json d1, d2;
                        if (pc1) {
                            d1 = surfaceEquationToJson(&pc1->surf());
                            delete pc1;
                        }
                        if (pc2) {
                            d2 = surfaceEquationToJson(&pc2->surf());
                            delete pc2;
                        }
                        if ((d1.empty() || d2.empty()) && edge) {
                            COEDGE* co = edge->coedge();
                            if (co && d1.empty() && co->owner() && ((LOOP*)co->owner())->face() &&
                                ((LOOP*)co->owner())->face()->geometry())
                                d1 = surfaceEquationToJson(
                                    surfaceEntityEquation(((LOOP*)co->owner())->face()->geometry()));
                            if (co && d2.empty() && co->partner() && co->partner()->owner() &&
                                ((LOOP*)co->partner()->owner())->face() &&
                                ((LOOP*)co->partner()->owner())->face()->geometry())
                                d2 = surfaceEquationToJson(
                                    surfaceEntityEquation(((LOOP*)co->partner()->owner())->face()->geometry()));
                        }
                        if (!d1.empty()) gj["def_surf1"] = d1;
                        if (!d2.empty()) gj["def_surf2"] = d2;
                        gj["safe_range"] = intervalToJson(icv.safe_range());
                    }
                    break;
                }
                default:
                    break;
                }
                props["geometry"] = gj;
            }
            node.props = props;
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY*)edge->start());
            enqueueChild((ENTITY*)edge->end());
            enqueueChild((ENTITY*)edge->coedge());
            break;
        }
        case VERTEX_ID: {
            VERTEX* vtx = (VERTEX*)e;
            node.label = "vertex";
            node.id = (&rootEntity == e) ? componentSid : genNodeId(e, VERTEX_ID);
            node.props["entity_type"] = "vertex";
            if (vtx->geometry()) {
                if (vtx->geometry()->identity(0) == APOINT_ID) {
                    SPAposition pos = ((APOINT*)vtx->geometry())->coords();
                    node.props["position"] = posToJson(pos);
                }
            }
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY*)vtx->edge());
            break;
        }
        case TRANSFORM_ID: {
            TRANSFORM* tf = (TRANSFORM*)e;
            node.label = "transform";
            node.id = genNodeId(e, TRANSFORM_ID);
            node.props = transfToJson(tf->transform());
            node.props["entity_type"] = "transform";
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            break;
        }
        default:
            continue;
        }
    }

    // 第二遍：构建关系（DFS，同 v2）。
    std::stack<ENTITY*> stack2;
    std::unordered_set<const void*> visited2;
    stack2.push(&rootEntity);
    visited2.insert(&rootEntity);
    auto push2 = [&](ENTITY* child) {
        if (child && visited2.insert(child).second) stack2.push(child);
    };
    auto emitRel = [&](const char* type, const void* startPtr, const void* endPtr) {
        auto itS = ptrToId.find(startPtr);
        auto itE = ptrToId.find(endPtr);
        if (itS != ptrToId.end() && itE != ptrToId.end())
            relsJson.push_back(json{{"type", type}, {"start", itS->second}, {"end", itE->second}});
    };

    while (!stack2.empty()) {
        ENTITY* e = stack2.top();
        stack2.pop();
        if (!e) continue;
        const void* ePtr = static_cast<const void*>(e);

        switch (e->identity(1)) {
        case BODY_ID: {
            BODY* body = (BODY*)e;
            if (body->lump()) {
                emitRel("body_lump", ePtr, body->lump());
                push2((ENTITY*)body->lump());
            }
            if (body->wire()) {
                emitRel("body_wire", ePtr, body->wire());
                push2((ENTITY*)body->wire());
            }
            if (body->transform()) {
                emitRel("body_transform", ePtr, body->transform());
                push2((ENTITY*)body->transform());
            }
            break;
        }
        case LUMP_ID: {
            LUMP* lump = (LUMP*)e;
            if (lump->shell()) {
                emitRel("lump_shell", ePtr, lump->shell());
                push2((ENTITY*)lump->shell());
            }
            if (lump->next()) {
                emitRel("lump_next", ePtr, lump->next());
                push2((ENTITY*)lump->next());
            }
            if (lump->body()) {
                emitRel("lump_body", ePtr, lump->body());
                push2((ENTITY*)lump->body());
            }
            break;
        }
        case SHELL_ID: {
            SHELL* shell = (SHELL*)e;
            if (shell->next()) {
                emitRel("shell_next", ePtr, shell->next());
                push2((ENTITY*)shell->next());
            }
            if (shell->face()) {
                emitRel("shell_face", ePtr, shell->face());
                push2((ENTITY*)shell->face());
            }
            if (shell->wire()) {
                emitRel("shell_wire", ePtr, shell->wire());
                push2((ENTITY*)shell->wire());
            }
            if (shell->lump()) {
                emitRel("shell_lump", ePtr, shell->lump());
                push2((ENTITY*)shell->lump());
            }
            if (shell->subshell()) {
                emitRel("shell_subshell", ePtr, shell->subshell());
                push2((ENTITY*)shell->subshell());
            }
            break;
        }
        case SUBSHELL_ID: {
            SUBSHELL* ss = (SUBSHELL*)e;
            if (ss->parent()) {
                emitRel("subshell_parent", ePtr, ss->parent());
                push2((ENTITY*)ss->parent());
            }
            if (ss->sibling()) {
                emitRel("subshell_sibling", ePtr, ss->sibling());
                push2((ENTITY*)ss->sibling());
            }
            if (ss->child()) {
                emitRel("subshell_child", ePtr, ss->child());
                push2((ENTITY*)ss->child());
            }
            if (ss->face()) {
                emitRel("subshell_face", ePtr, ss->face());
                push2((ENTITY*)ss->face());
            }
            if (ss->wire()) {
                emitRel("subshell_wire", ePtr, ss->wire());
                push2((ENTITY*)ss->wire());
            }
            break;
        }
        case FACE_ID: {
            FACE* face = (FACE*)e;
            if (face->loop()) {
                emitRel("face_loop", ePtr, face->loop());
                push2((ENTITY*)face->loop());
            }
            if (face->next()) {
                emitRel("face_next", ePtr, face->next());
                push2((ENTITY*)face->next());
            }
            if (face->shell()) {
                emitRel("face_shell", ePtr, face->shell());
                push2((ENTITY*)face->shell());
            }
            if (face->subshell()) {
                emitRel("face_subshell", ePtr, face->subshell());
                push2((ENTITY*)face->subshell());
            }
            break;
        }
        case LOOP_ID: {
            LOOP* loop = (LOOP*)e;
            if (loop->start()) {
                emitRel("loop_start", ePtr, loop->start());
                push2((ENTITY*)loop->start());
            }
            if (loop->next()) {
                emitRel("loop_next", ePtr, loop->next());
                push2((ENTITY*)loop->next());
            }
            if (loop->face()) {
                emitRel("loop_face", ePtr, loop->face());
                push2((ENTITY*)loop->face());
            }
            break;
        }
        case WIRE_ID: {
            WIRE* wire = (WIRE*)e;
            if (wire->next()) {
                emitRel("wire_next", ePtr, wire->next());
                push2((ENTITY*)wire->next());
            }
            if (wire->coedge()) {
                emitRel("wire_coedge", ePtr, wire->coedge());
                push2((ENTITY*)wire->coedge());
            }
            if (wire->owner()) {
                emitRel("wire_owner", ePtr, wire->owner());
                push2((ENTITY*)wire->owner());
            }
            break;
        }
        case COEDGE_ID: {
            COEDGE* co = (COEDGE*)e;
            if (co->next()) {
                emitRel("coedge_next", ePtr, co->next());
                push2((ENTITY*)co->next());
            }
            if (co->previous()) {
                emitRel("coedge_previous", ePtr, co->previous());
                push2((ENTITY*)co->previous());
            }
            if (co->partner()) {
                emitRel("coedge_partner", ePtr, co->partner());
                push2((ENTITY*)co->partner());
            }
            if (co->edge()) {
                emitRel("coedge_edge", ePtr, co->edge());
                push2((ENTITY*)co->edge());
            }
            if (co->owner()) {
                emitRel("coedge_owner", ePtr, co->owner());
                push2((ENTITY*)co->owner());
            }
            break;
        }
        case EDGE_ID: {
            EDGE* edge = (EDGE*)e;
            if (edge->start()) {
                emitRel("edge_start", ePtr, edge->start());
                push2((ENTITY*)edge->start());
            }
            if (edge->end()) {
                emitRel("edge_end", ePtr, edge->end());
                push2((ENTITY*)edge->end());
            }
            if (edge->coedge()) {
                emitRel("edge_coedge", ePtr, edge->coedge());
                push2((ENTITY*)edge->coedge());
            }
            break;
        }
        case VERTEX_ID: {
            VERTEX* vertex = (VERTEX*)e;
            if (vertex->edge()) {
                emitRel("vertex_edge", ePtr, vertex->edge());
                push2((ENTITY*)vertex->edge());
            }
            break;
        }
        default:
            break;
        }
    }

    // 保真风险判定（v2 fallback 触发条件，逐条保留）：任一命中则 root 标记
    // fallback="sat_asset"（v2 为内嵌 sat_fragment；v3 组件 SAT 已是独立内容
    // 寻址资产，投影只标记，几何真相由资产承载）。
    bool risky = false;
    for (const auto& pr : orderedNodes) {
        if (nodeRisky(pr.second, (ENTITY*)pr.first)) {
            risky = true;
            break;
        }
    }

    const size_t rootNodeCount = orderedNodes.size();
    const size_t rootRelCount = relsJson.size();

    json root;
    root["schema"] = "dbcad.entity_graph.v2";
    root["serializer_build"] = "uvsafe-topology-v5-bounded";
    for (auto& entry : orderedNodes) {
        if (entry.first != (const void*)&rootEntity) continue;
        entry.second.props["top_level"] = true;
        entry.second.props["uuid"] = entry.second.id;
        entry.second.props["fallback"] = risky ? "sat_asset" : "fine_grained";
        entry.second.props["topology_node_count"] = (int)rootNodeCount;
        entry.second.props["topology_rel_count"] = (int)rootRelCount;
        break;
    }
    for (const auto& entry : orderedNodes) {
        const IntNode& n = entry.second;
        root["nodes"].push_back(json{{"id", n.id}, {"labels", json::array({n.label})}, {"props", n.props}});
    }
    root["rels"] = relsJson;
    GRAPH_TRACE("serializeEntityGraph: sid=%s nodes=%zu rels=%zu fallback=%s\n", componentSid.c_str(),
                rootNodeCount, rootRelCount, risky ? "sat_asset" : "fine_grained");
    return root;
}

}  // namespace dbcad::worker
