/**
 * entity_graph_serializer.cpp
 *
 * 实现 ACIS 实体图的 JSON 序列化；反序列化位于 entity_graph_deserializer.cpp。
 *
 * 序列化逻辑（serializeACISEntityGraph）：
 *   使用与 access.cpp api_save_entity_list_neo4j 相同的 ACIS 遍历方式
 *   和属性提取逻辑，但将 mg_* 类型转换为 JSON 类型。
 *
 * 反序列化逻辑（deserializeACISEntityGraph）：
 *   严格分三阶段（参考 access.cpp api_restore_entity_list_neo4j）：
 *   1. Pass1: 创建所有实体骨架（全部包裹在 API_BEGIN/API_END 内）
 *   2. Pass2: 设置几何属性（全部包裹在 API_BEGIN/API_END 内）
 *   3. Pass3: 处理拓扑链接
 */

#include "entity_graph_serializer.h"

#include <acis/include/api.hxx>
#include <acis/include/cstrapi.hxx>
#include <acis/include/kernapi.hxx>
#include <acis/include/fct_utl.hxx>
#include <acis/include/rnd_api.hxx>
#include <acis/include/allcurve.hxx>
#include <acis/include/allsurf.hxx>
#include <acis/include/exct_spl.hxx>
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
#include <acis/include/bulletin.hxx>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QStack>
#include <QQueue>
#include <QDebug>
#include <unordered_set>
#include <cmath>

// ============================================================================
// JSON 序列化命名空间
// ============================================================================

namespace JsonSerialize {

// SPAposition → [x, y, z]
inline QJsonArray posToJson(const SPAposition &p) {
    return QJsonArray{p.x(), p.y(), p.z()};
}

// SPAvector → [x, y, z]
inline QJsonArray vecToJson(const SPAvector &v) {
    return QJsonArray{v.x(), v.y(), v.z()};
}

// SPAunit_vector → [x, y, z]
inline QJsonArray unitVecToJson(const SPAunit_vector &v) {
    return QJsonArray{v.x(), v.y(), v.z()};
}

// SPAinterval → [type1, val1, type2, val2]  (与 access.cpp getmglist_SPAinterval 一致)
// 恒定输出 4 元素：旧实现对无限/半无限区间只输出 2–3 元素，对端 parseInterval
// 要求 ≥4 元素而回退成 [0,1]，导致无限底层直线的边参数区间整体塌缩。
inline QJsonArray intervalToJson(const SPAinterval &iv) {
    QJsonArray arr;
    if (iv.empty()) {
        arr.append(1.0);
        arr.append(0.0);
        arr.append(1.0);
        arr.append(0.0);
    } else {
        interval_type t = iv.type();
        if (t == interval_type::interval_finite) {
            arr.append(2.0);
            arr.append(iv.start_pt());
            arr.append(2.0);
            arr.append(iv.end_pt());
        } else if (t == interval_type::interval_finite_below) {
            arr.append(2.0);
            arr.append(iv.start_pt());
            arr.append(1.0);
            arr.append(0.0);
        } else if (t == interval_type::interval_finite_above) {
            arr.append(1.0);
            arr.append(0.0);
            arr.append(2.0);
            arr.append(iv.end_pt());
        } else {
            arr.append(1.0);
            arr.append(0.0);
            arr.append(1.0);
            arr.append(0.0);
        }
    }
    return arr;
}

// 边的参数区间（EDGE::param_bound）：有效 ACIS 边必有有限范围。
// 取 EDGE 自身范围而非底层曲线方程的 param_range（后者对无限直线为无限区间，
// 曾导致对端回退 [0,1]、边参数塌缩）。方向保留（sense 由 edge->sense 承载）。
// 边几何的参数区间：优先取曲线方程自身的 param_range（绝对参数空间，与 SAT
// 一致）；仅当曲线区间无限（如未受限 straight）时回退到 EDGE param_bound。
// 教训（G10 圆角弧边）：EDGE bound 与曲线参数空间存在错位（弧线曲线区间
// [-π/2,0] 而 bound 为 [0,π/2]），直接用 bound 限制曲线会取错弧段。
inline QJsonArray curveRangeToJson(const curve &crv, const EDGE *edge) {
    SPAinterval r = crv.param_range();
    if (!r.empty() && r.type() == interval_type::interval_finite) {
        double period = crv.param_period(); // 非周期曲线为 0；未受限整圆求值成整周期
        if (period <= 0 || (r.end_pt() - r.start_pt()) < period - 1e-9) {
            return intervalToJson(r); // 真子区间 → 限制曲线
        }
        QJsonArray unbounded;
        unbounded.append(1.0);
        unbounded.append(0.0);
        unbounded.append(1.0);
        unbounded.append(0.0);
        return unbounded; // 覆盖整周期 = 未受限周期曲线，保持自然状态（SAT 落盘 I I）
    }
    return intervalToJson(edge->get_param_range());
}

// SPApar_box → [u0, u1, v0, v1]
inline QJsonArray parBoxToJson(const SPApar_box &box) {
    QJsonArray arr;
    SPAinterval uRange = box.u_range();
    SPAinterval vRange = box.v_range();
    // 格式与 parseParBox 一致：[u_start, u_end, v_start, v_end]
    // ACIS may expose an empty or non-finite subset range for an analytic
    // surface. Serializing that as [0,0,...] creates a degenerate finite
    // range on the receiving client and makes api_facet_entity produce
    // edges but no face mesh. Preserve only a real, ordered finite range.
    const bool validU = !uRange.empty() && std::isfinite(uRange.start_pt()) && std::isfinite(uRange.end_pt()) &&
                        uRange.end_pt() > uRange.start_pt();
    const bool validV = !vRange.empty() && std::isfinite(vRange.start_pt()) && std::isfinite(vRange.end_pt()) &&
                        vRange.end_pt() > vRange.start_pt();
    if (validU && validV) {
        arr.append(uRange.start_pt());
        arr.append(uRange.end_pt());
        arr.append(vRange.start_pt());
        arr.append(vRange.end_pt());
    }
    return arr;
}

// SPAmatrix → [9 floats]（列主序，与 access.cpp getmglist_SPAmatrix 完全一致）
inline QJsonArray matrixToJson(const SPAmatrix &m) {
    return QJsonArray{m.element(0, 0), m.element(0, 1), m.element(0, 2), m.element(1, 0), m.element(1, 1),
                      m.element(1, 2), m.element(2, 0), m.element(2, 1), m.element(2, 2)};
}

// SPAtransf → JSON 对象（与 access.cpp 存 Neo4j 的属性完全对应）
inline QJsonObject transfToJson(const SPAtransf &tf) {
    QJsonObject obj;
    obj["affine"] = matrixToJson(tf.affine());
    obj["translation"] = vecToJson(tf.translation());
    obj["scaling"] = tf.scaling();
    obj["rotate"] = tf.rotate();
    obj["reflect"] = tf.reflect();
    obj["shear"] = tf.shear();
    return obj;
}

// 曲线类型名
QString curveTypeName(ENTITY *e) {
    if (!e)
        return "unknown";
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

// 曲面类型名
QString surfaceTypeName(ENTITY *e) {
    if (!e)
        return "unknown";
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

} // namespace JsonSerialize

// ============================================================================
// 序列化：ACIS ENTITY_LIST → JSON
// ============================================================================

using namespace JsonSerialize;

static QString genNodeId(const void *ptr, int typeId) {
    char buf[64];
    snprintf(buf, sizeof(buf), "n_%p_%d", ptr, typeId);
    return QString::fromLatin1(buf);
}

// 序列化入口
QJsonObject serializeACISEntityGraph(const QHash<int, QString> &entityIndexToUuid,
                                     const ENTITY_LIST &topLevelEntities) {
    std::fprintf(stderr, "[Collab] entity_graph_serializer build=uvsafe-topology-v5-bounded\n");
    QJsonArray nodesJson;
    QJsonArray relsJson;

    // ptr → nodeId 映射（用于关系构建）
    QHash<const void *, QString> ptrToId;

    // 内部节点结构
    struct IntNode {
        QString id;
        QString label;
        QJsonObject props;
    };
    // 关键：必须用 std::vector（保留插入顺序）而非 QHash（迭代无序），
    // 否则 nodes 数组的 body 节点顺序与用户 addEntity 顺序不一致，
    // 导致远端 merge loop 中 remoteBodies[0]/[1] 与 A 端错位。
    std::vector<std::pair<const void *, IntNode>> orderedNodes;

    QQueue<ENTITY *> queue;
    std::unordered_set<const void *> visited;

    // 初始化：从顶级实体开始 BFS
    // topLevelEntities 的顺序就是用户 addEntity 的顺序，这是我们要保留的
    for (int i = 0; i < topLevelEntities.count(); ++i) {
        ENTITY *e = topLevelEntities[i];
        if (e && visited.insert(e).second) {
            queue.enqueue(e);
        }
    }

    // 子节点入队辅助
    auto enqueueChild = [&](ENTITY *child) {
        if (child && visited.insert(child).second) {
            queue.enqueue(child);
        }
    };

    while (!queue.isEmpty()) {
        ENTITY *e = queue.dequeue();
        if (!e)
            continue;
        const void *ePtr = static_cast<const void *>(e);
        if (ptrToId.contains(ePtr))
            continue;

        IntNode node;
        node.props = QJsonObject();

        switch (e->identity(1)) {

        case BODY_ID: {
            BODY *body = (BODY *)e;
            node.label = "body";
            // 用 ETI index 查 uuid
            for (int i = 0; i < topLevelEntities.count(); ++i) {
                if (topLevelEntities[i] == e) {
                    QString uuid = entityIndexToUuid.value(i);
                    if (!uuid.isEmpty())
                        node.id = uuid;
                    break;
                }
            }
            if (node.id.isEmpty())
                node.id = genNodeId(e, BODY_ID);
            QJsonObject props;
            props["entity_type"] = "body";
            node.props = props;
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY *)body->lump());
            enqueueChild((ENTITY *)body->wire());
            enqueueChild((ENTITY *)body->transform());
            break;
        }
        case LUMP_ID: {
            LUMP *lump = (LUMP *)e;
            node.label = "lump";
            node.id = genNodeId(e, LUMP_ID);
            QJsonObject props;
            props["entity_type"] = "lump";
            node.props = props;
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY *)lump->shell());
            enqueueChild((ENTITY *)lump->next());
            enqueueChild((ENTITY *)lump->body());
            break;
        }
        case SHELL_ID: {
            SHELL *shell = (SHELL *)e;
            node.label = "shell";
            node.id = genNodeId(e, SHELL_ID);
            node.props["entity_type"] = "shell";
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY *)shell->next());
            enqueueChild((ENTITY *)shell->face());
            enqueueChild((ENTITY *)shell->wire());
            enqueueChild((ENTITY *)shell->lump());
            enqueueChild((ENTITY *)shell->subshell());
            break;
        }
        case SUBSHELL_ID: {
            // 缺口4：subshell 骨架（三向自引用树 + 直属 face/wire）
            SUBSHELL *ss = (SUBSHELL *)e;
            node.label = "subshell";
            node.id = genNodeId(e, SUBSHELL_ID);
            node.props["entity_type"] = "subshell";
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY *)ss->parent());
            enqueueChild((ENTITY *)ss->sibling());
            enqueueChild((ENTITY *)ss->child());
            enqueueChild((ENTITY *)ss->face());
            enqueueChild((ENTITY *)ss->wire());
            break;
        }
        case WIRE_ID: {
            WIRE *wire = (WIRE *)e;
            node.label = "wire";
            node.id = genNodeId(e, WIRE_ID);
            node.props["entity_type"] = "wire";
            node.props["cont"] = wire->cont();
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY *)wire->next());
            enqueueChild((ENTITY *)wire->coedge());
            enqueueChild((ENTITY *)wire->subshell());
            enqueueChild((ENTITY *)wire->owner());
            break;
        }
        case FACE_ID: {
            FACE *face = (FACE *)e;
            node.label = "face";
            node.id = genNodeId(e, FACE_ID);
            QJsonObject props;
            props["entity_type"] = "face";
            props["sense"] = face->sense();
            props["sides"] = face->sides() ? 1 : 0;
            if (face->sides())
                props["cont"] = face->cont();
            if (face->geometry()) {
                ENTITY *geom = face->geometry();
                QJsonObject gj;
                gj["type"] = surfaceTypeName(geom);
                SPApar_box uvrange = ((surface *)geom)->subset_box();
                gj["uv_range"] = parBoxToJson(uvrange);
                switch (geom->identity(2)) {
                case PLANE_ID: {
                    const plane &gem = static_cast<const plane &>(((PLANE *)geom)->equation());
                    gj["root_point"] = posToJson(gem.root_point);
                    gj["normal"] = unitVecToJson(gem.normal);
                    gj["u_deriv"] = vecToJson(gem.u_deriv);
                    break;
                }
                case SPHERE_ID: {
                    const sphere &gem = static_cast<const sphere &>(((SPHERE *)geom)->equation());
                    gj["centre"] = posToJson(gem.centre);
                    gj["radius"] = gem.radius;
                    break;
                }
                case TORUS_ID: {
                    const torus &gem = static_cast<const torus &>(((TORUS *)geom)->equation());
                    gj["centre"] = posToJson(gem.centre);
                    gj["normal"] = unitVecToJson(gem.normal);
                    gj["major_radius"] = gem.major_radius;
                    gj["minor_radius"] = gem.minor_radius;
                    break;
                }
                case CONE_ID: {
                    const cone &gem = static_cast<const cone &>(((CONE *)geom)->equation());
                    ellipse base = gem.base;
                    gj["base_centre"] = posToJson(base.centre);
                    gj["base_normal"] = unitVecToJson(base.normal);
                    // base_major_axis 此前漏发，对端 parseVector 拿到空数组会得到
                    // 未初始化向量，锥面直接损坏（SAT 显示 1e-310 量级垃圾值）
                    gj["base_major_axis"] = vecToJson(base.major_axis);
                    gj["base_radius_ratio"] = base.radius_ratio;
                    gj["sine_angle"] = gem.sine_angle;
                    gj["cosine_angle"] = gem.cosine_angle;
                    // u_param_scale：cone 构造器 para 参数默认 0，不传则 u 方向
                    // 参数化丢失（SAT 中 major/para 均归零），曲面不可用
                    gj["u_param_scale"] = gem.u_param_scale;
                    break;
                }
                case SPLINE_ID: {
                    // 缺口1：NURBS 曲面载荷（bs3_surface），布局与 Bolt 栈一致
                    //（控制点按 v 外层、u 内层，索引 u*num_v+v；节点向量 RLE 端点-1）
                    const spline &gem = static_cast<const spline &>(((SPLINE *)geom)->equation());
                    const spl_sur &ss = gem.get_spl_sur();
                    int dim = 0, form_u = 0, form_v = 0, pole_u = 0, pole_v = 0, num_u = 0, num_v = 0;
                    int degree_u = 0, num_uknots = 0, degree_v = 0, num_vknots = 0;
                    logical rational_u = 0, rational_v = 0;
                    SPAposition *ctrlpts = nullptr;
                    double *weights = nullptr, *uknots = nullptr, *vknots = nullptr;
                    bs3_surface_to_array(ss.sur(), dim, rational_u, rational_v, form_u, form_v, pole_u, pole_v,
                                         num_u, num_v, ctrlpts, weights, degree_u, num_uknots, uknots, degree_v,
                                         num_vknots, vknots, 0);
                    auto rleEncode = [](const double *knots, int n) {
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
                        QJsonArray arr;
                        for (double v : ks)
                            arr.append(v);
                        return arr;
                    };
                    gj["rational_u"] = (int)rational_u;
                    gj["rational_v"] = (int)rational_v;
                    gj["degree_u"] = degree_u;
                    gj["degree_v"] = degree_v;
                    gj["form_u"] = form_u;
                    gj["form_v"] = form_v;
                    gj["u_singularity"] = pole_u;
                    gj["v_singularity"] = pole_v;
                    gj["num_u"] = num_u;
                    gj["num_v"] = num_v;
                    gj["uknots"] = rleEncode(uknots, num_uknots);
                    gj["vknots"] = rleEncode(vknots, num_vknots);
                    bool rational = rational_u || rational_v;
                    QJsonArray ctrlArr;
                    for (int i = 0; i < num_v; ++i) {
                        for (int j = 0; j < num_u; ++j) {
                            SPAposition *pt = ctrlpts + j * num_v + i;
                            ctrlArr.append(pt->x());
                            ctrlArr.append(pt->y());
                            ctrlArr.append(pt->z());
                            if (rational)
                                ctrlArr.append(weights[j * num_v + i]);
                        }
                    }
                    gj["ctrlpts"] = ctrlArr;
                    gj["fitol"] = ss.fitol();
                    gj["reversed"] = (int)gem.reversed();
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
            enqueueChild((ENTITY *)face->next());
            enqueueChild((ENTITY *)face->loop());
            enqueueChild((ENTITY *)face->shell());
            break;
        }
        case LOOP_ID: {
            LOOP *loop = (LOOP *)e;
            node.label = "loop";
            node.id = genNodeId(e, LOOP_ID);
            node.props["entity_type"] = "loop";
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY *)loop->next());
            enqueueChild((ENTITY *)loop->start());
            enqueueChild((ENTITY *)loop->face());
            break;
        }
        case COEDGE_ID: {
            COEDGE *co = (COEDGE *)e;
            node.label = "coedge";
            node.id = genNodeId(e, COEDGE_ID);
            node.props["entity_type"] = "coedge";
            node.props["sense"] = co->sense();
            // 缺口5：pcurve 载荷（bs2 参数域拟合）。宿主曲面不重复序列化——
            // 它就是所属 face 的 geometry，恢复端经拓扑关系反查。
            if (co->geometry() && is_PCURVE(co->geometry())) {
                const pcurve &pc = static_cast<const pcurve &>(((PCURVE *)co->geometry())->equation());
                bs2_curve bs2 = pc.cur();
                if (bs2 != nullptr) {
                    QJsonObject gj;
                    gj["type"] = "pcurve";
                    int dim2 = 0, deg2 = 0, numCtrl2 = 0, numKnots2 = 0;
                    logical rat2 = 0;
                    SPAposition *ctrl2 = nullptr;
                    double *weights2 = nullptr, *knots2 = nullptr;
                    bs2_curve_to_array(bs2, dim2, deg2, rat2, numCtrl2, ctrl2, weights2, numKnots2, knots2);
                    int form2 = 0;
                    if (bs2_curve_closed(bs2))
                        form2 = bs2_curve_periodic(bs2) ? 2 : 1;
                    gj["rational"] = (int)rat2;
                    gj["degree"] = deg2;
                    gj["form"] = form2;
                    gj["num_ctrlpts"] = numCtrl2;
                    std::vector<double> ks2;
                    for (int i = 0; i < numKnots2; ++i) {
                        double v = knots2[i];
                        if (ks2.empty() || v != ks2[ks2.size() - 2]) {
                            ks2.push_back(v);
                            ks2.push_back(1);
                        } else {
                            ks2.back()++;
                        }
                    }
                    if (ks2.size() >= 4) {
                        ks2[1]--;
                        ks2.back()--;
                    }
                    QJsonArray kn2;
                    for (double v : ks2)
                        kn2.append(v);
                    gj["knots"] = kn2;
                    QJsonArray ct2;
                    for (int i = 0; i < numCtrl2; ++i) {
                        ct2.append(ctrl2[i].x());
                        ct2.append(ctrl2[i].y());
                        if (rat2)
                            ct2.append(weights2[i]);
                    }
                    gj["ctrlpts"] = ct2;
                    gj["fitol"] = pc.fitol();
                    node.props["geometry"] = gj;
                }
            }
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY *)co->next());
            enqueueChild((ENTITY *)co->previous());
            enqueueChild((ENTITY *)co->partner());
            enqueueChild((ENTITY *)co->edge());
            enqueueChild((ENTITY *)co->owner());
            break;
        }
        case EDGE_ID: {
            EDGE *edge = (EDGE *)e;
            node.label = "edge";
            node.id = genNodeId(e, EDGE_ID);
            SPAposition sp, ep;
            bool haveSp = false, haveEp = false;
            if (edge->start()) {
                VERTEX *sv = edge->start();
                if (sv->geometry() && sv->geometry()->identity(0) == APOINT_ID) {
                    sp = ((APOINT *)sv->geometry())->coords();
                    haveSp = true;
                }
            }
            if (edge->end()) {
                VERTEX *ev = edge->end();
                if (ev->geometry() && ev->geometry()->identity(0) == APOINT_ID) {
                    ep = ((APOINT *)ev->geometry())->coords();
                    haveEp = true;
                }
            }
            if (edge->sense() != 0) {
                std::swap(sp, ep);
                std::swap(haveSp, haveEp);
            }
            QJsonObject props;
            props["entity_type"] = "edge";
            props["sense"] = edge->sense();
            // 凸性（tangent/convex/concave/...）：圆角边为 tangent；丢失会导致
            // SAT 落盘 @7 unknown 且影响下游评估
            props["convexity"] = (int)edge->get_convexity();
            if (haveSp)
                props["start_point"] = posToJson(sp);
            if (haveEp)
                props["end_point"] = posToJson(ep);
            if (edge->geometry()) {
                ENTITY *geom = edge->geometry();
                QJsonObject gj;
                gj["type"] = curveTypeName(geom);
                switch (geom->identity(2)) {
                case STRAIGHT_ID: {
                    const straight &gem = static_cast<const straight &>(((STRAIGHT *)geom)->equation());
                    gj["root_point"] = posToJson(gem.root_point);
                    // direction 携带 param_scale（模长=参数比例，与 Bolt 栈一致）：
                    // 只发单位向量会把 [0,1]×scale 的边塌缩成长度 1（G13 连接边实锤）
                    gj["direction"] = vecToJson(SPAvector(gem.direction.x() * gem.param_scale,
                                                          gem.direction.y() * gem.param_scale,
                                                          gem.direction.z() * gem.param_scale));
                    gj["range"] = curveRangeToJson(gem, edge);
                    break;
                }
                case ELLIPSE_ID: {
                    const ellipse &gem = static_cast<const ellipse &>(((ELLIPSE *)geom)->equation());
                    gj["centre"] = posToJson(gem.centre);
                    gj["normal"] = unitVecToJson(gem.normal);
                    gj["major_axis"] = vecToJson(gem.major_axis);
                    gj["radius_ratio"] = gem.radius_ratio;
                    gj["range"] = curveRangeToJson(gem, edge);
                    break;
                }
                case HELIX_ID: {
                    // 8 字段完整载荷（缺口3）：与 Bolt 栈 access_history_save 一致
                    const helix &gem = static_cast<const helix &>(((HELIX *)geom)->equation());
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
                    // 缺口2：逼近 NURBS 载荷（bs3）。端曲面/参数曲线留待后续
                    // 精确重建阶段；bs3 为主表示已足够评估（体积/渲染/布尔）。
                    const intcurve &icv = static_cast<const intcurve &>(((INTCURVE *)geom)->equation());
                    bs3_curve bs = icv.cur();
                    if (bs != nullptr) {
                        int dim = 0, deg = 0, numCtrl = 0, numKnots = 0;
                        logical rat = 0;
                        SPAposition *ctrl = nullptr;
                        double *weights = nullptr, *knots = nullptr;
                        bs3_curve_to_array(bs, dim, deg, rat, numCtrl, ctrl, weights, numKnots, knots);
                        int form = 0;
                        if (bs3_curve_closed(bs))
                            form = bs3_curve_periodic(bs) ? 2 : 1;
                        gj["rational"] = (int)rat;
                        gj["degree"] = deg;
                        gj["form"] = form;
                        // 节点向量 RLE [knot, mult, ...]，端点多重性 -1（与 Bolt 栈一致）
                        std::vector<double> ks;
                        for (int i = 0; i < numKnots; ++i) {
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
                        QJsonArray knotArr;
                        for (double v : ks)
                            knotArr.append(v);
                        gj["knots"] = knotArr;
                        QJsonArray ctrlArr;
                        for (int i = 0; i < numCtrl; ++i) {
                            ctrlArr.append(ctrl[i].x());
                            ctrlArr.append(ctrl[i].y());
                            ctrlArr.append(ctrl[i].z());
                            if (rat)
                                ctrlArr.append(weights[i]);
                        }
                        gj["ctrlpts"] = ctrlArr;
                        gj["fitol"] = icv.fitol();
                    }
                    gj["range"] = curveRangeToJson(icv, edge);
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
            enqueueChild((ENTITY *)edge->start());
            enqueueChild((ENTITY *)edge->end());
            enqueueChild((ENTITY *)edge->coedge());
            break;
        }
        case VERTEX_ID: {
            VERTEX *vtx = (VERTEX *)e;
            node.label = "vertex";
            node.id = genNodeId(e, VERTEX_ID);
            node.props["entity_type"] = "vertex";
            if (vtx->geometry()) {
                if (vtx->geometry()->identity(0) == APOINT_ID) {
                    SPAposition pos = ((APOINT *)vtx->geometry())->coords();
                    node.props["position"] = posToJson(pos);
                }
            }
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY *)vtx->edge());
            break;
        }
        case TRANSFORM_ID: {
            TRANSFORM *tf = (TRANSFORM *)e;
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

    // 第二遍：构建关系
    QStack<ENTITY *> stack2;
    std::unordered_set<const void *> visited2;
    for (int i = 0; i < topLevelEntities.count(); ++i) {
        ENTITY *e = topLevelEntities[i];
        if (e && visited2.insert(e).second)
            stack2.push(e);
    }
    auto push2 = [&](ENTITY *child) {
        if (child && visited2.insert(child).second)
            stack2.push(child);
    };
    auto emitRel = [&](const QString &type, const void *startPtr, const void *endPtr) {
        auto itS = ptrToId.find(startPtr);
        auto itE = ptrToId.find(endPtr);
        if (itS != ptrToId.end() && itE != ptrToId.end()) {
            relsJson.append(QJsonObject{{"type", type}, {"start", itS.value()}, {"end", itE.value()}});
        }
    };

    while (!stack2.isEmpty()) {
        ENTITY *e = stack2.pop();
        if (!e)
            continue;
        const void *ePtr = static_cast<const void *>(e);

        switch (e->identity(1)) {
        case BODY_ID: {
            BODY *body = (BODY *)e;
            if (body->lump()) {
                emitRel("body_lump", ePtr, body->lump());
                push2((ENTITY *)body->lump());
            }
            if (body->wire()) {
                emitRel("body_wire", ePtr, body->wire());
                push2((ENTITY *)body->wire());
            }
            if (body->transform()) {
                emitRel("body_transform", ePtr, body->transform());
                push2((ENTITY *)body->transform());
            }
            break;
        }
        case LUMP_ID: {
            LUMP *lump = (LUMP *)e;
            if (lump->shell()) {
                emitRel("lump_shell", ePtr, lump->shell());
                push2((ENTITY *)lump->shell());
            }
            if (lump->next()) {
                emitRel("lump_next", ePtr, lump->next());
                push2((ENTITY *)lump->next());
            }
            if (lump->body()) {
                emitRel("lump_body", ePtr, lump->body());
                push2((ENTITY *)lump->body());
            }
            break;
        }
        case SHELL_ID: {
            SHELL *shell = (SHELL *)e;
            if (shell->next()) {
                emitRel("shell_next", ePtr, shell->next());
                push2((ENTITY *)shell->next());
            }
            if (shell->face()) {
                emitRel("shell_face", ePtr, shell->face());
                push2((ENTITY *)shell->face());
            }
            if (shell->wire()) {
                emitRel("shell_wire", ePtr, shell->wire());
                push2((ENTITY *)shell->wire());
            }
            if (shell->lump()) {
                emitRel("shell_lump", ePtr, shell->lump());
                push2((ENTITY *)shell->lump());
            }
            if (shell->subshell()) {
                emitRel("shell_subshell", ePtr, shell->subshell());
                push2((ENTITY *)shell->subshell());
            }
            break;
        }
        case SUBSHELL_ID: {
            SUBSHELL *ss = (SUBSHELL *)e;
            if (ss->parent()) {
                emitRel("subshell_parent", ePtr, ss->parent());
                push2((ENTITY *)ss->parent());
            }
            if (ss->sibling()) {
                emitRel("subshell_sibling", ePtr, ss->sibling());
                push2((ENTITY *)ss->sibling());
            }
            if (ss->child()) {
                emitRel("subshell_child", ePtr, ss->child());
                push2((ENTITY *)ss->child());
            }
            if (ss->face()) {
                emitRel("subshell_face", ePtr, ss->face());
                push2((ENTITY *)ss->face());
            }
            if (ss->wire()) {
                emitRel("subshell_wire", ePtr, ss->wire());
                push2((ENTITY *)ss->wire());
            }
            break;
        }
        case FACE_ID: {
            FACE *face = (FACE *)e;
            // 几何内嵌在 face.props.geometry 中，不再发布 face_geometry 关系
            if (face->loop()) {
                emitRel("face_loop", ePtr, face->loop());
                push2((ENTITY *)face->loop());
            }
            if (face->next()) {
                emitRel("face_next", ePtr, face->next());
                push2((ENTITY *)face->next());
            }
            if (face->shell()) {
                emitRel("face_shell", ePtr, face->shell());
                push2((ENTITY *)face->shell());
            }
            if (face->subshell()) {
                emitRel("face_subshell", ePtr, face->subshell());
                push2((ENTITY *)face->subshell());
            }
            break;
        }
        case LOOP_ID: {
            LOOP *loop = (LOOP *)e;
            if (loop->start()) {
                emitRel("loop_start", ePtr, loop->start());
                push2((ENTITY *)loop->start());
            }
            if (loop->next()) {
                emitRel("loop_next", ePtr, loop->next());
                push2((ENTITY *)loop->next());
            }
            if (loop->face()) {
                emitRel("loop_face", ePtr, loop->face());
                push2((ENTITY *)loop->face());
            }
            break;
        }
        case WIRE_ID: {
            WIRE *wire = (WIRE *)e;
            if (wire->next()) {
                emitRel("wire_next", ePtr, wire->next());
                push2((ENTITY *)wire->next());
            }
            if (wire->coedge()) {
                emitRel("wire_coedge", ePtr, wire->coedge());
                push2((ENTITY *)wire->coedge());
            }
            if (wire->owner()) {
                emitRel("wire_owner", ePtr, wire->owner());
                push2((ENTITY *)wire->owner());
            }
            break;
        }
        case COEDGE_ID: {
            COEDGE *co = (COEDGE *)e;
            if (co->next()) {
                emitRel("coedge_next", ePtr, co->next());
                push2((ENTITY *)co->next());
            }
            if (co->previous()) {
                emitRel("coedge_previous", ePtr, co->previous());
                push2((ENTITY *)co->previous());
            }
            if (co->partner()) {
                emitRel("coedge_partner", ePtr, co->partner());
                push2((ENTITY *)co->partner());
            }
            if (co->edge()) {
                emitRel("coedge_edge", ePtr, co->edge());
                push2((ENTITY *)co->edge());
            }
            if (co->owner()) {
                emitRel("coedge_owner", ePtr, co->owner());
                push2((ENTITY *)co->owner());
            }
            break;
        }
        case EDGE_ID: {
            EDGE *edge = (EDGE *)e;
            if (edge->start()) {
                emitRel("edge_start", ePtr, edge->start());
                push2((ENTITY *)edge->start());
            }
            if (edge->end()) {
                emitRel("edge_end", ePtr, edge->end());
                push2((ENTITY *)edge->end());
            }
            if (edge->coedge()) {
                emitRel("edge_coedge", ePtr, edge->coedge());
                push2((ENTITY *)edge->coedge());
            }
            break;
        }
        case VERTEX_ID: {
            VERTEX *vertex = (VERTEX *)e;
            if (vertex->edge()) {
                emitRel("vertex_edge", ePtr, vertex->edge());
                push2((ENTITY *)vertex->edge());
            }
            break;
        }
        default:
            break;
        }
    }

    // 构建最终 JSON
    fprintf(stderr, "[Collab] serializeACISEntityGraph: nodes total=%zu\n", orderedNodes.size());
    int bodyIdx = 0;
    for (const auto &entry : orderedNodes) {
        const IntNode &n = entry.second;
        QJsonObject nodeJson;
        nodeJson["id"] = n.id;
        nodeJson["labels"] = QJsonArray{n.label};
        nodeJson["props"] = n.props;
        if (n.label == "body") {
            fprintf(stderr, "[Collab] serialize: body node idx=%d id=%s\n", bodyIdx, n.id.toUtf8().constData());
            bodyIdx++;
        }
        if (n.label == "face") {
            // 调试：打印 face 的完整 props（含 geometry 字段）
            QJsonDocument fd(n.props);
            QString fs = QString::fromUtf8(fd.toJson(QJsonDocument::Compact));
            fprintf(stderr, "[Collab] serialize: face id=%s props=%s\n", n.id.toUtf8().constData(),
                    fs.toUtf8().constData());
        }
        nodesJson.append(nodeJson);
    }

    QJsonObject root;
    root["nodes"] = nodesJson;
    root["rels"] = relsJson;
    root["serializer_build"] = QStringLiteral("uvsafe-topology-v5-bounded");
    return root;
}
