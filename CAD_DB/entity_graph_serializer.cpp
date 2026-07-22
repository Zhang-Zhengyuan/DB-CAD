/**
 * entity_graph_serializer.cpp
 *
 * 实现 ACIS 实体图的 JSON 序列化/反序列化。
 *
 * 序列化逻辑（serializeACISEntityGraph）：
 *   使用与 access.cpp api_save_entity_list_neo4j 相同的 ACIS 遍历方式
 *   和属性提取逻辑，但将 mg_* 类型转换为 JSON 类型。
 *
 * 反序列化逻辑（deserializeACISEntityGraph）：
 *   返回 false（暂未实现），调用方应使用 SAT fallback。
 *   后续可扩展为调用 access.cpp 的 api_restore_entity_list_neo4j。
 */

#include "entity_graph_serializer.h"

#include <acis/include/api.hxx>
#include <acis/include/cstrapi.hxx>
#include <acis/include/kernapi.hxx>
#include <acis/include/allcurve.hxx>
#include <acis/include/allsurf.hxx>
#include <acis/include/alltop.hxx>
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
#include <QDebug>
#include <unordered_set>

// ============================================================================
// JSON 序列化命名空间
// ============================================================================

namespace JsonSerialize {

// SPAposition → [x, y, z]
inline QJsonArray posToJson(const SPAposition& p) {
    return QJsonArray{ p.x(), p.y(), p.z() };
}

// SPAvector → [x, y, z]
inline QJsonArray vecToJson(const SPAvector& v) {
    return QJsonArray{ v.x(), v.y(), v.z() };
}

// SPAunit_vector → [x, y, z]
inline QJsonArray unitVecToJson(const SPAunit_vector& v) {
    return QJsonArray{ v.x(), v.y(), v.z() };
}

// SPAinterval → [type1, val1, type2, val2]  (与 access.cpp getmglist_SPAinterval 完全一致)
inline QJsonArray intervalToJson(const SPAinterval& iv) {
    QJsonArray arr;
    if (iv.empty()) {
        arr.append(1.0); arr.append(1.0);
    } else {
        interval_type t = iv.type();
        if (t == interval_type::interval_finite) {
            arr.append(2.0); arr.append(iv.start_pt());
            arr.append(2.0); arr.append(iv.end_pt());
        } else if (t == interval_type::interval_finite_below) {
            arr.append(2.0); arr.append(iv.start_pt());
            arr.append(1.0);
        } else if (t == interval_type::interval_finite_above) {
            arr.append(1.0);
            arr.append(2.0); arr.append(iv.end_pt());
        } else {
            arr.append(1.0); arr.append(1.0);
        }
    }
    return arr;
}

// SPApar_box → [u0, u1, v0, v1]
// SPApar_box 自身没有 start/end 等便捷访问，访问它有两种方式：
//   (1) box.u_range() / box.v_range() 返回 SPAinterval，可拆 start_pt()/end_pt()
//   (2) 调用 API 函数 get_range / subset_range（在 surface 上）
// 这里用 (1)：逻辑清晰、对部分 box（range 为空）情况友好
inline QJsonArray parBoxToJson(const SPApar_box& box) {
    SPAinterval uRange = box.u_range();
    SPAinterval vRange = box.v_range();
    double u0 = 0.0, u1 = 0.0, v0 = 0.0, v1 = 0.0;
    if (!uRange.empty()) {
        u0 = uRange.start_pt();
        u1 = uRange.end_pt();
    }
    if (!vRange.empty()) {
        v0 = vRange.start_pt();
        v1 = vRange.end_pt();
    }
    return QJsonArray{ u0, u1, v0, v1 };
}

// SPAmatrix → [9 floats]（列主序，与 access.cpp getmglist_SPAmatrix 完全一致）
inline QJsonArray matrixToJson(const SPAmatrix& m) {
    return QJsonArray{
        m.element(0,0), m.element(0,1), m.element(0,2),
        m.element(1,0), m.element(1,1), m.element(1,2),
        m.element(2,0), m.element(2,1), m.element(2,2)
    };
}

// SPAtransf → JSON 对象（与 access.cpp 存 Neo4j 的属性完全对应）
inline QJsonObject transfToJson(const SPAtransf& tf) {
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
QString curveTypeName(ENTITY* e) {
    if (!e) return "unknown";
    switch (e->identity(2)) {
        case STRAIGHT_ID: return "straight";
        case ELLIPSE_ID: return "ellipse";
        case HELIX_ID: return "helix";
        case INTCURVE_ID: return "intcurve";
        default: return "unknown_curve";
    }
}

// 曲面类型名
QString surfaceTypeName(ENTITY* e) {
    if (!e) return "unknown";
    switch (e->identity(2)) {
        case PLANE_ID: return "plane";
        case SPHERE_ID: return "sphere";
        case TORUS_ID: return "torus";
        case CONE_ID: return "cone";
        case SPLINE_ID: return "spline";
        default: return "unknown_surface";
    }
}

} // namespace JsonSerialize

// ============================================================================
// 序列化：ACIS ENTITY_LIST → JSON
// ============================================================================

using namespace JsonSerialize;

static QString genNodeId(const void* ptr, int typeId) {
    char buf[64];
    snprintf(buf, sizeof(buf), "n_%p_%d", ptr, typeId);
    return QString::fromLatin1(buf);
}

// 序列化入口
QJsonObject serializeACISEntityGraph(
    const QHash<int, QString>& entityIndexToUuid,
    const ENTITY_LIST& topLevelEntities
) {
    QJsonArray nodesJson;
    QJsonArray relsJson;

    // ptr → nodeId 映射（用于关系构建）
    QHash<const void*, QString> ptrToId;

    // 内部节点结构
    struct IntNode {
        QString id;
        QString label;
        QJsonObject props;
    };
    QHash<const void*, IntNode> ptrToNode;

    QStack<ENTITY*> stack;
    std::unordered_set<const void*> visited;

    // 初始化：从顶级实体开始 BFS
    for (int i = 0; i < topLevelEntities.count(); ++i) {
        ENTITY* e = topLevelEntities[i];
        if (e && visited.insert(e).second) {
            stack.push(e);
        }
    }

    // 子节点入栈辅助
    auto push = [&](ENTITY* child) {
        if (child && visited.insert(child).second) {
            stack.push(child);
        }
    };

    while (!stack.isEmpty()) {
        ENTITY* e = stack.pop();
        if (!e) continue;
        const void* ePtr = static_cast<const void*>(e);
        if (ptrToNode.contains(ePtr)) continue;

        IntNode node;
        node.props = QJsonObject();

        switch (e->identity(1)) {

        case BODY_ID: {
            BODY* body = (BODY*)e;
            node.label = "body";
            // 用 ETI index 查 uuid
            for (int i = 0; i < topLevelEntities.count(); ++i) {
                if (topLevelEntities[i] == e) {
                    QString uuid = entityIndexToUuid.value(i);
                    if (!uuid.isEmpty()) node.id = uuid;
                    break;
                }
            }
            if (node.id.isEmpty()) node.id = genNodeId(e, BODY_ID);
            QJsonObject props;
            props["entity_type"] = "body";
            // BODY 本身不暴露 SPAtransf 接口（没有 transform() 成员）。
            // 实体的 transform 信息保存在关联的 TRANSFORM 实体节点里（push 后会被单独遍历）。
            node.props = props;
            ptrToNode[ePtr] = node;
            ptrToId[ePtr] = node.id;
            push((ENTITY*)body->lump());
            push((ENTITY*)body->wire());
            break;
        }
        case LUMP_ID: {
            LUMP* lump = (LUMP*)e;
            node.label = "lump";
            node.id = genNodeId(e, LUMP_ID);
            QJsonObject props;
            props["entity_type"] = "lump";
            // LUMP 同样不暴露 transform()，实体 transform 信息保存在 TRANSFORM 节点里
            node.props = props;
            ptrToNode[ePtr] = node;
            ptrToId[ePtr] = node.id;
            push((ENTITY*)lump->shell());
            push((ENTITY*)lump->next());
            break;
        }
        case SHELL_ID: {
            SHELL* shell = (SHELL*)e;
            node.label = "shell";
            node.id = genNodeId(e, SHELL_ID);
            node.props["entity_type"] = "shell";
            ptrToNode[ePtr] = node;
            ptrToId[ePtr] = node.id;
            push((ENTITY*)shell->next());
            push((ENTITY*)shell->face());
            push((ENTITY*)shell->wire());
            push((ENTITY*)shell->lump());
            break;
        }
        case FACE_ID: {
            FACE* face = (FACE*)e;
            node.label = "face";
            node.id = genNodeId(e, FACE_ID);
            QJsonObject props;
            props["entity_type"] = "face";
            props["sense"] = face->sense();
            props["sides"] = face->sides() ? 1 : 0;
            if (face->sides()) props["cont"] = face->cont();
            if (face->geometry()) {
                ENTITY* geom = face->geometry();
                QJsonObject gj;
                gj["type"] = surfaceTypeName(geom);
                // subset_range 是 surface 真实存在的 API（acis.cpp:119 gme_get_subset_range）
                SPApar_box uvrange = ((surface*)geom)->gme_get_subset_range();
                gj["uv_range"] = parBoxToJson(uvrange);
                switch (geom->identity(2)) {
                case PLANE_ID: {
                    plane gem = ((PLANE*)geom)->gme_get_def();
                    gj["root_point"] = posToJson(gem.root_point);
                    gj["normal"] = unitVecToJson(gem.normal);
                    gj["u_deriv"] = vecToJson(gem.u_deriv);
                    break;
                }
                case SPHERE_ID: {
                    sphere gem = ((SPHERE*)geom)->gme_get_def();
                    gj["centre"] = posToJson(gem.centre);
                    gj["radius"] = gem.radius;
                    break;
                }
                case TORUS_ID: {
                    torus gem = ((TORUS*)geom)->gme_get_def();
                    gj["centre"] = posToJson(gem.centre);
                    gj["normal"] = unitVecToJson(gem.normal);
                    gj["major_radius"] = gem.major_radius;
                    gj["minor_radius"] = gem.minor_radius;
                    break;
                }
                case CONE_ID: {
                    cone gem = ((CONE*)geom)->gme_get_def();
                    ellipse base = gem.base;
                    gj["base_centre"] = posToJson(base.centre);
                    gj["base_normal"] = unitVecToJson(base.normal);
                    gj["base_radius_ratio"] = base.radius_ratio;
                    gj["sine_angle"] = gem.sine_angle;
                    gj["cosine_angle"] = gem.cosine_angle;
                    break;
                }
                default:
                    break;
                }
                props["geometry"] = gj;
            }
            node.props = props;
            ptrToNode[ePtr] = node;
            ptrToId[ePtr] = node.id;
            push((ENTITY*)face->next());
            push((ENTITY*)face->loop());
            push((ENTITY*)face->shell());
            break;
        }
        case LOOP_ID: {
            LOOP* loop = (LOOP*)e;
            node.label = "loop";
            node.id = genNodeId(e, LOOP_ID);
            node.props["entity_type"] = "loop";
            ptrToNode[ePtr] = node;
            ptrToId[ePtr] = node.id;
            push((ENTITY*)loop->next());
            push((ENTITY*)loop->start());
            push((ENTITY*)loop->face());
            break;
        }
        case COEDGE_ID: {
            COEDGE* co = (COEDGE*)e;
            node.label = "coedge";
            node.id = genNodeId(e, COEDGE_ID);
            node.props["entity_type"] = "coedge";
            node.props["sense"] = co->sense();
            ptrToNode[ePtr] = node;
            ptrToId[ePtr] = node.id;
            push((ENTITY*)co->next());
            push((ENTITY*)co->edge());
            break;
        }
        case EDGE_ID: {
            EDGE* edge = (EDGE*)e;
            node.label = "edge";
            node.id = genNodeId(e, EDGE_ID);
            // EDGE 的两个端点 VERTEX*：start() / end() 各指向一个顶点
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
            // 按 sense 反向
            if (edge->sense() != 0) {
                std::swap(sp, ep);
                std::swap(haveSp, haveEp);
            }
            QJsonObject props;
            props["entity_type"] = "edge";
            props["sense"] = edge->sense();
            if (haveSp) props["start_point"] = posToJson(sp);
            if (haveEp) props["end_point"] = posToJson(ep);
            if (edge->geometry()) {
                ENTITY* geom = edge->geometry();
                QJsonObject gj;
                gj["type"] = curveTypeName(geom);
                switch (geom->identity(2)) {
                case STRAIGHT_ID: {
                    straight gem = ((STRAIGHT*)geom)->gme_get_def();
                    gj["root_point"] = posToJson(gem.root_point);
                    gj["direction"] = unitVecToJson(gem.direction);
                    gj["range"] = intervalToJson(gem.gme_get_subset_range());
                    break;
                }
                case ELLIPSE_ID: {
                    ellipse gem = ((ELLIPSE*)geom)->gme_get_def();
                    gj["centre"] = posToJson(gem.centre);
                    gj["normal"] = unitVecToJson(gem.normal);
                    gj["major_axis"] = vecToJson(gem.major_axis);
                    gj["radius_ratio"] = gem.radius_ratio;
                    gj["range"] = intervalToJson(gem.gme_get_subset_range());
                    break;
                }
                default:
                    break;
                }
                props["geometry"] = gj;
            }
            node.props = props;
            ptrToNode[ePtr] = node;
            ptrToId[ePtr] = node.id;
            push((ENTITY*)edge->start());
            push((ENTITY*)edge->end());
            break;
        }
        case VERTEX_ID: {
            VERTEX* vtx = (VERTEX*)e;
            node.label = "vertex";
            node.id = genNodeId(e, VERTEX_ID);
            node.props["entity_type"] = "vertex";
            if (vtx->geometry()) {
                // APOINT 是 vertex 的几何（指向 SPAposition 的点）
                if (vtx->geometry()->identity(0) == APOINT_ID) {
                    SPAposition pos = ((APOINT*)vtx->geometry())->coords();
                    node.props["position"] = posToJson(pos);
                }
            }
            ptrToNode[ePtr] = node;
            ptrToId[ePtr] = node.id;
            break;
        }
        case TRANSFORM_ID: {
            TRANSFORM* tf = (TRANSFORM*)e;
            node.label = "transform";
            node.id = genNodeId(e, TRANSFORM_ID);
            node.props = transfToJson(tf->transform());
            node.props["entity_type"] = "transform";
            ptrToNode[ePtr] = node;
            ptrToId[ePtr] = node.id;
            break;
        }
        default:
            continue;
        }
    }

    // 第二遍：构建关系
    QStack<ENTITY*> stack2;
    std::unordered_set<const void*> visited2;
    for (int i = 0; i < topLevelEntities.count(); ++i) {
        ENTITY* e = topLevelEntities[i];
        if (e && visited2.insert(e).second) stack2.push(e);
    }
    auto push2 = [&](ENTITY* child) {
        if (child && visited2.insert(child).second) stack2.push(child);
    };
    auto emitRel = [&](const QString& type, const void* startPtr, const void* endPtr) {
        auto itS = ptrToId.find(startPtr);
        auto itE = ptrToId.find(endPtr);
        if (itS != ptrToId.end() && itE != ptrToId.end()) {
            relsJson.append(QJsonObject{
                {"type", type},
                {"start", itS.value()},
                {"end", itE.value()}
            });
        }
    };

    while (!stack2.isEmpty()) {
        ENTITY* e = stack2.pop();
        if (!e) continue;
        const void* ePtr = static_cast<const void*>(e);

        switch (e->identity(1)) {
        case BODY_ID: {
            BODY* body = (BODY*)e;
            if (body->lump()) { emitRel("body_lump", ePtr, body->lump()); push2((ENTITY*)body->lump()); }
            if (body->wire()) { emitRel("body_wire", ePtr, body->wire()); push2((ENTITY*)body->wire()); }
            break;
        }
        case LUMP_ID: {
            LUMP* lump = (LUMP*)e;
            if (lump->shell()) { emitRel("lump_shell", ePtr, lump->shell()); push2((ENTITY*)lump->shell()); }
            if (lump->next()) { emitRel("lump_next", ePtr, lump->next()); push2((ENTITY*)lump->next()); }
            break;
        }
        case SHELL_ID: {
            SHELL* shell = (SHELL*)e;
            if (shell->next()) { emitRel("shell_next", ePtr, shell->next()); push2((ENTITY*)shell->next()); }
            if (shell->face()) { emitRel("shell_face", ePtr, shell->face()); push2((ENTITY*)shell->face()); }
            if (shell->wire()) { emitRel("shell_wire", ePtr, shell->wire()); push2((ENTITY*)shell->wire()); }
            if (shell->lump()) { emitRel("shell_lump", ePtr, shell->lump()); push2((ENTITY*)shell->lump()); }
            break;
        }
        case FACE_ID: {
            FACE* face = (FACE*)e;
            if (face->geometry()) { emitRel("face_geometry", ePtr, face->geometry()); push2(face->geometry()); }
            if (face->loop()) { emitRel("face_loop", ePtr, face->loop()); push2((ENTITY*)face->loop()); }
            if (face->next()) { emitRel("face_next", ePtr, face->next()); push2((ENTITY*)face->next()); }
            break;
        }
        case LOOP_ID: {
            LOOP* loop = (LOOP*)e;
            if (loop->start()) { emitRel("loop_start", ePtr, loop->start()); push2((ENTITY*)loop->start()); }
            if (loop->next()) { emitRel("loop_next", ePtr, loop->next()); push2((ENTITY*)loop->next()); }
            break;
        }
        case COEDGE_ID: {
            COEDGE* co = (COEDGE*)e;
            if (co->next()) { emitRel("coedge_next", ePtr, co->next()); push2((ENTITY*)co->next()); }
            if (co->edge()) { emitRel("coedge_edge", ePtr, co->edge()); push2((ENTITY*)co->edge()); }
            break;
        }
        case EDGE_ID: {
            EDGE* edge = (EDGE*)e;
            if (edge->start()) { emitRel("edge_start", ePtr, edge->start()); push2((ENTITY*)edge->start()); }
            if (edge->end()) { emitRel("edge_end", ePtr, edge->end()); push2((ENTITY*)edge->end()); }
            break;
        }
        default:
            break;
        }
    }

    // 构建最终 JSON
    for (auto it = ptrToNode.begin(); it != ptrToNode.end(); ++it) {
        const IntNode& n = it.value();
        QJsonObject nodeJson;
        nodeJson["id"] = n.id;
        nodeJson["labels"] = QJsonArray{ n.label };
        nodeJson["props"] = n.props;
        nodesJson.append(nodeJson);
    }

    QJsonObject root;
    root["nodes"] = nodesJson;
    root["rels"] = relsJson;
    return root;
}

// ============================================================================
// 反序列化：JSON → ACIS
// ============================================================================

namespace JsonDeserialize {

// 辅助：从 JSON 数组解析 SPAposition
static SPAposition parsePosition(const QJsonArray& arr) {
    if (arr.size() >= 3) {
        return SPAposition(arr[0].toDouble(), arr[1].toDouble(), arr[2].toDouble());
    }
    return SPAposition();
}

// 辅助：从 JSON 数组解析 SPAvector
static SPAvector parseVector(const QJsonArray& arr) {
    if (arr.size() >= 3) {
        return SPAvector(arr[0].toDouble(), arr[1].toDouble(), arr[2].toDouble());
    }
    return SPAvector();
}

// 辅助：从 JSON 数组解析 SPAunit_vector
static SPAunit_vector parseUnitVector(const QJsonArray& arr) {
    if (arr.size() >= 3) {
        return SPAunit_vector(arr[0].toDouble(), arr[1].toDouble(), arr[2].toDouble());
    }
    return SPAunit_vector(0, 0, 1);
}

// 辅助：从 JSON 数组解析 SPAmatrix（列主序）
static SPAmatrix parseMatrix(const QJsonArray& arr) {
    if (arr.size() >= 9) {
        SPAvector v1(arr[0].toDouble(), arr[1].toDouble(), arr[2].toDouble());
        SPAvector v2(arr[3].toDouble(), arr[4].toDouble(), arr[5].toDouble());
        SPAvector v3(arr[6].toDouble(), arr[7].toDouble(), arr[8].toDouble());
        return SPAmatrix(v1, v2, v3);
    }
    return SPAmatrix();
}

// 辅助：从 JSON 数组解析 SPAinterval
static SPAinterval parseInterval(const QJsonArray& arr) {
    if (arr.size() >= 2) {
        return SPAinterval(arr[0].toDouble(), arr[1].toDouble());
    }
    return SPAinterval(0, 1);
}

// 辅助：从 JSON 对象解析 surface，返回 APISESSION 分配的指针
static surface* parseSurface(const QJsonObject& gj, QString* error) {
    const QString type = gj.value("type").toString();
    if (type == "plane") {
        SPAposition root = parsePosition(gj.value("root_point").toArray());
        SPAunit_vector normal = parseUnitVector(gj.value("normal").toArray());
        SPAvector u_deriv = parseVector(gj.value("u_deriv").toArray());
        SPApar_box uvrange = SPApar_box(
            parseInterval(gj.value("uv_range").toArray()).start_pt(),
            parseInterval(gj.value("uv_range").toArray()).end_pt()
        );
        plane* def = ACIS_NEW plane(root, normal, u_deriv);
        def->gme_set_subset_range(uvrange);
        PLANE* pl = nullptr;
        API_BEGIN;
            pl = ACIS_NEW PLANE(*def);
        API_END;
        ACIS_DELETE def;
        return (surface*)pl;
    } else if (type == "sphere") {
        SPAposition centre = parsePosition(gj.value("centre").toArray());
        double radius = gj.value("radius").toDouble(1.0);
        sphere* def = ACIS_NEW sphere(centre, radius);
        SPHERE* sp = nullptr;
        API_BEGIN;
            sp = ACIS_NEW SPHERE(*def);
        API_END;
        ACIS_DELETE def;
        return (surface*)sp;
    } else if (type == "torus") {
        SPAposition centre = parsePosition(gj.value("centre").toArray());
        SPAunit_vector normal = parseUnitVector(gj.value("normal").toArray());
        double major = gj.value("major_radius").toDouble(1.0);
        double minor = gj.value("minor_radius").toDouble(0.1);
        torus* def = ACIS_NEW torus(centre, normal, major, minor);
        TORUS* tr = nullptr;
        API_BEGIN;
            tr = ACIS_NEW TORUS(*def);
        API_END;
        ACIS_DELETE def;
        return (surface*)tr;
    } else if (type == "cone") {
        SPAposition base_centre = parsePosition(gj.value("base_centre").toArray());
        SPAunit_vector base_normal = parseUnitVector(gj.value("base_normal").toArray());
        SPAvector major_axis = parseVector(gj.value("major_axis").toArray());
        double rratio = gj.value("base_radius_ratio").toDouble(1.0);
        double sine = gj.value("sine_angle").toDouble(0.0);
        double cosine = gj.value("cosine_angle").toDouble(1.0);
        // CONE 需要先创建 base ellipse，设置 subset_range，然后创建 cone
        ellipse* gem_base = ACIS_NEW ellipse(base_centre, base_normal, major_axis, rratio);
        SPAinterval base_u_range = parseInterval(gj.value("uv_range").toArray());
        gem_base->gme_set_subset_range(base_u_range);
        cone* def = ACIS_NEW cone(*gem_base, sine, cosine);
        ACIS_DELETE gem_base;
        CONE* cn = nullptr;
        API_BEGIN;
            cn = ACIS_NEW CONE(*def);
        API_END;
        ACIS_DELETE def;
        return (surface*)cn;
    }
    // 暂不支持的几面类型
    if (error) *error = QString::fromUtf8("unsupported surface type: %1").arg(type);
    return nullptr;
}

// 辅助：从 JSON 对象解析 curve，返回 APISESSION 分配的指针
static curve* parseCurve(const QJsonObject& gj, QString* error) {
    const QString type = gj.value("type").toString();
    if (type == "straight") {
        SPAposition root = parsePosition(gj.value("root_point").toArray());
        SPAvector direction = parseVector(gj.value("direction").toArray());
        SPAinterval range = parseInterval(gj.value("range").toArray());
        straight* def = ACIS_NEW straight(root, normalise(direction));
        def->gme_set_param_scale(direction.len());
        if (def) def->gme_set_subset_range(range);
        STRAIGHT* st = nullptr;
        API_BEGIN;
            st = ACIS_NEW STRAIGHT(*def);
        API_END;
        ACIS_DELETE def;
        return (curve*)st;
    } else if (type == "ellipse") {
        SPAposition centre = parsePosition(gj.value("centre").toArray());
        SPAunit_vector normal = parseUnitVector(gj.value("normal").toArray());
        SPAvector major_axis = parseVector(gj.value("major_axis").toArray());
        double ratio = gj.value("radius_ratio").toDouble(1.0);
        SPAinterval range = parseInterval(gj.value("range").toArray());
        ellipse* def = ACIS_NEW ellipse(centre, normal, major_axis, ratio);
        if (def) def->gme_set_subset_range(range);
        ELLIPSE* el = nullptr;
        API_BEGIN;
            el = ACIS_NEW ELLIPSE(*def);
        API_END;
        ACIS_DELETE def;
        return (curve*)el;
    }
    if (error) *error = QString::fromUtf8("unsupported curve type: %1").arg(type);
    return nullptr;
}

} // namespace JsonDeserialize

bool deserializeACISEntityGraph(
    const QJsonObject& graphJson,
    DeserializedEntityGraph* outResult,
    QString* errorMessage
) {
    using namespace JsonDeserialize;

    if (errorMessage) errorMessage->clear();
    if (!outResult) {
        if (errorMessage) *errorMessage = QString::fromUtf8("outResult is null");
        return false;
    }
    outResult->entities = new ENTITY_LIST();
    outResult->uuidToEntity.clear();

    API_BEGIN;

    const QJsonArray nodes = graphJson.value("nodes").toArray();
    const QJsonArray rels = graphJson.value("rels").toArray();

    if (nodes.isEmpty()) {
        if (errorMessage) *errorMessage = QString::fromUtf8("entity_graph nodes is empty");
        return false;
    }

    // id → ENTITY* 映射（用于 rels 链接）
    QHash<QString, ENTITY*> idToEntity;

    // 第一遍：创建所有实体（拓扑结构 + 几何）
    for (const QJsonValue& nv : nodes) {
        const QJsonObject node = nv.toObject();
        const QString id = node.value("id").toString();
        const QJsonArray labels = node.value("labels").toArray();
        if (labels.isEmpty()) continue;
        const QString label = labels[0].toString();
        const QJsonObject props = node.value("props").toObject();

        ENTITY* ent = nullptr;
        QString localError;

        if (label == "body") {
            BODY* body = nullptr;
            API_BEGIN;
                api_body(body);
            API_END;
            ent = (ENTITY*)body;
            // uuid 存储在 idToEntity 的 key 里
        } else if (label == "lump") {
            LUMP* lump = nullptr;
            API_BEGIN;
                lump = ACIS_NEW LUMP();
            API_END;
            ent = (ENTITY*)lump;
        } else if (label == "shell") {
            SHELL* shell = nullptr;
            API_BEGIN;
                shell = ACIS_NEW SHELL();
            API_END;
            ent = (ENTITY*)shell;
        } else if (label == "wire") {
            WIRE* wire = nullptr;
            API_BEGIN;
                wire = ACIS_NEW WIRE();
            API_END;
            ent = (ENTITY*)wire;
        } else if (label == "face") {
            FACE* face = nullptr;
            API_BEGIN;
                face = ACIS_NEW FACE();
            API_END;
            if (face) {
                face->set_sense(props.value("sense").toInt(1));
                int sides = props.value("sides").toInt(1);
                face->set_sides(sides != 0);
                if (sides != 0) {
                    face->set_cont(props.value("cont").toInt(0));
                }
                // 几何
                const QJsonObject geom = props.value("geometry").toObject();
                if (!geom.isEmpty()) {
                    surface* surf = parseSurface(geom, &localError);
                    if (surf) {
                        API_BEGIN;
                            face->set_geometry((SURFACE*)surf);
                        API_END;
                    } else {
                        // 不支持的几面类型，通过 API 删除 face
                        if (errorMessage && !localError.isEmpty()) {
                            *errorMessage = QString::fromUtf8("face geometry error: %1").arg(localError);
                        }
                        outcome r;
                        API_BEGIN;
                            r = api_del_entity(face);
                        API_END;
                        Q_UNUSED(r);
                        ent = nullptr;
                    }
                }
            }
            ent = (ENTITY*)face;
        } else if (label == "loop") {
            LOOP* loop = nullptr;
            API_BEGIN;
                loop = ACIS_NEW LOOP();
            API_END;
            ent = (ENTITY*)loop;
        } else if (label == "coedge") {
            COEDGE* co = nullptr;
            API_BEGIN;
                co = ACIS_NEW COEDGE();
            API_END;
            if (co) co->set_sense(props.value("sense").toInt(1));
            ent = (ENTITY*)co;
        } else if (label == "edge") {
            EDGE* edge = nullptr;
            API_BEGIN;
                edge = ACIS_NEW EDGE();
            API_END;
            if (edge) {
                edge->set_sense(props.value("sense").toInt(1));
                // 几何
                const QJsonObject geom = props.value("geometry").toObject();
                if (!geom.isEmpty()) {
                    curve* crv = parseCurve(geom, &localError);
                    if (crv) {
                        API_BEGIN;
                            edge->set_geometry((CURVE*)crv);
                        API_END;
                    } else {
                        if (errorMessage && !localError.isEmpty()) {
                            *errorMessage = QString::fromUtf8("edge geometry error: %1").arg(localError);
                        }
                    }
                }
                // 端点（通过 rels 链接，暂不在这里处理）
            }
            ent = (ENTITY*)edge;
        } else if (label == "vertex") {
            VERTEX* vtx = nullptr;
            API_BEGIN;
                vtx = ACIS_NEW VERTEX();
            API_END;
            ent = (ENTITY*)vtx;
        } else if (label == "transform") {
            SPAvector trans = parseVector(props.value("translation").toArray());
            SPAmatrix aff = parseMatrix(props.value("affine").toArray());
            double scaling = props.value("scaling").toDouble(1.0);
            int rotate = props.value("rotate").toInt(0);
            int reflect = props.value("reflect").toInt(0);
            int shear = props.value("shear").toInt(0);
            SPAtransf tf_data(aff, trans, scaling, rotate, reflect, shear);
            TRANSFORM* tf = nullptr;
            API_BEGIN;
                tf = ACIS_NEW TRANSFORM(tf_data);
            API_END;
            ent = (ENTITY*)tf;
        }

        if (ent) {
            idToEntity[id] = ent;
            // BODY 加入结果列表
            if (label == "body") {
                outResult->entities->add(ent);
                // uuid 映射：key = id, value = body ptr
                outResult->uuidToEntity[id] = (void*)ent;
            }
        }
    }

    // 第二遍：处理关系链接
    for (const QJsonValue& rv : rels) {
        const QJsonObject rel = rv.toObject();
        const QString type = rel.value("type").toString();
        const QString startId = rel.value("start").toString();
        const QString endId = rel.value("end").toString();

        ENTITY* startEnt = idToEntity.value(startId);
        ENTITY* endEnt = idToEntity.value(endId);
        if (!startEnt || !endEnt) continue;

        if (type == "body_lump") {
            BODY* body = (BODY*)startEnt;
            LUMP* lump = (LUMP*)endEnt;
            API_BEGIN;
                body->set_lump(lump);
            API_END;
        } else if (type == "lump_shell") {
            LUMP* lump = (LUMP*)startEnt;
            SHELL* shell = (SHELL*)endEnt;
            API_BEGIN;
                lump->set_shell(shell);
            API_END;
        } else if (type == "shell_face") {
            SHELL* shell = (SHELL*)startEnt;
            FACE* face = (FACE*)endEnt;
            API_BEGIN;
                shell->set_face(face);
            API_END;
        } else if (type == "face_loop") {
            FACE* face = (FACE*)startEnt;
            LOOP* loop = (LOOP*)endEnt;
            API_BEGIN;
                face->set_loop(loop);
            API_END;
        } else if (type == "loop_coedge") {
            LOOP* loop = (LOOP*)startEnt;
            COEDGE* co = (COEDGE*)endEnt;
            API_BEGIN;
                loop->set_start(co);
            API_END;
        } else if (type == "loop_start") {
            LOOP* loop = (LOOP*)startEnt;
            COEDGE* co = (COEDGE*)endEnt;
            API_BEGIN;
                loop->set_start(co);
            API_END;
        } else if (type == "coedge_edge") {
            COEDGE* co = (COEDGE*)startEnt;
            EDGE* edge = (EDGE*)endEnt;
            API_BEGIN;
                co->set_edge(edge);
            API_END;
        } else if (type == "edge_start" || type == "edge_end") {
            EDGE* edge = (EDGE*)startEnt;
            VERTEX* vtx = (VERTEX*)endEnt;
            if (type == "edge_start") {
                API_BEGIN;
                    edge->set_start(vtx);
                API_END;
            } else {
                API_BEGIN;
                    edge->set_end(vtx);
                API_END;
            }
        } else if (type == "body_wire") {
            BODY* body = (BODY*)startEnt;
            WIRE* wire = (WIRE*)endEnt;
            API_BEGIN;
                body->set_wire(wire);
            API_END;
        } else if (type == "shell_wire") {
            SHELL* shell = (SHELL*)startEnt;
            WIRE* wire = (WIRE*)endEnt;
            API_BEGIN;
                shell->set_wire(wire);
            API_END;
        }
    }

    API_END;
    return !outResult->entities->count() == 0;
}
