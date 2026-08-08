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
inline QJsonArray parBoxToJson(const SPApar_box& box) {
    QJsonArray arr;
    SPAinterval uRange = box.u_range();
    SPAinterval vRange = box.v_range();
    // 格式与 parseParBox 一致：[u_start, u_end, v_start, v_end]
    // u_range/v_range 可能是 empty（退化），用 0..1 占位
    double u0 = 0.0, u1 = 1.0, v0 = 0.0, v1 = 1.0;
    if (!uRange.empty()) { u0 = uRange.start_pt(); u1 = uRange.end_pt(); }
    if (!vRange.empty()) { v0 = vRange.start_pt(); v1 = vRange.end_pt(); }
    arr.append(u0); arr.append(u1);
    arr.append(v0); arr.append(v1);
    return arr;
}

// 辅助：从 JSON 数组 [u0, u1, v0, v1] 解析 SPApar_box
static SPApar_box parseParBox(const QJsonArray& arr) {
    if (arr.size() >= 4) {
        return SPApar_box(
            SPAinterval(arr[0].toDouble(), arr[1].toDouble()),
            SPAinterval(arr[2].toDouble(), arr[3].toDouble())
        );
    }
    return SPApar_box();
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
    // 关键：必须用 std::vector（保留插入顺序）而非 QHash（迭代无序），
    // 否则 nodes 数组的 body 节点顺序与用户 addEntity 顺序不一致，
    // 导致远端 merge loop 中 remoteBodies[0]/[1] 与 A 端错位。
    std::vector<std::pair<const void*, IntNode>> orderedNodes;

    QQueue<ENTITY*> queue;
    std::unordered_set<const void*> visited;

    // 初始化：从顶级实体开始 BFS
    // topLevelEntities 的顺序就是用户 addEntity 的顺序，这是我们要保留的
    for (int i = 0; i < topLevelEntities.count(); ++i) {
        ENTITY* e = topLevelEntities[i];
        if (e && visited.insert(e).second) {
            queue.enqueue(e);
        }
    }

    // 子节点入队辅助
    auto enqueueChild = [&](ENTITY* child) {
        if (child && visited.insert(child).second) {
            queue.enqueue(child);
        }
    };

    while (!queue.isEmpty()) {
        ENTITY* e = queue.dequeue();
        if (!e) continue;
        const void* ePtr = static_cast<const void*>(e);
        if (ptrToId.contains(ePtr)) continue;

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
            node.props = props;
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY*)body->lump());
            enqueueChild((ENTITY*)body->wire());
            break;
        }
        case LUMP_ID: {
            LUMP* lump = (LUMP*)e;
            node.label = "lump";
            node.id = genNodeId(e, LUMP_ID);
            QJsonObject props;
            props["entity_type"] = "lump";
            node.props = props;
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY*)lump->shell());
            enqueueChild((ENTITY*)lump->next());
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
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY*)co->next());
            enqueueChild((ENTITY*)co->edge());
            break;
        }
        case EDGE_ID: {
            EDGE* edge = (EDGE*)e;
            node.label = "edge";
            node.id = genNodeId(e, EDGE_ID);
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
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
            enqueueChild((ENTITY*)edge->start());
            enqueueChild((ENTITY*)edge->end());
            break;
        }
        case VERTEX_ID: {
            VERTEX* vtx = (VERTEX*)e;
            node.label = "vertex";
            node.id = genNodeId(e, VERTEX_ID);
            node.props["entity_type"] = "vertex";
            if (vtx->geometry()) {
                if (vtx->geometry()->identity(0) == APOINT_ID) {
                    SPAposition pos = ((APOINT*)vtx->geometry())->coords();
                    node.props["position"] = posToJson(pos);
                }
            }
            orderedNodes.push_back({ePtr, node});
            ptrToId[ePtr] = node.id;
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
            // 几何内嵌在 face.props.geometry 中，不再发布 face_geometry 关系
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
    fprintf(stderr, "[Collab] serializeACISEntityGraph: nodes total=%zu\n", orderedNodes.size());
    int bodyIdx = 0;
    for (const auto& entry : orderedNodes) {
        const IntNode& n = entry.second;
        QJsonObject nodeJson;
        nodeJson["id"] = n.id;
        nodeJson["labels"] = QJsonArray{ n.label };
        nodeJson["props"] = n.props;
        if (n.label == "body") {
            fprintf(stderr, "[Collab] serialize: body node idx=%d id=%s\n", bodyIdx, n.id.toUtf8().constData());
            bodyIdx++;
        }
        if (n.label == "face") {
            // 调试：打印 face 的完整 props（含 geometry 字段）
            QJsonDocument fd(n.props);
            QString fs = QString::fromUtf8(fd.toJson(QJsonDocument::Compact));
            fprintf(stderr, "[Collab] serialize: face id=%s props=%s\n",
                    n.id.toUtf8().constData(), fs.toUtf8().constData());
        }
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
// 格式: [type1, val1, type2, val2]
static SPAinterval parseInterval(const QJsonArray& arr) {
    if (arr.size() >= 4) {
        double start = 0.0, end = 1.0;
        int type1 = arr[0].toInt(1);
        if (type1 == 2) start = arr[1].toDouble();
        int type2 = arr[2].toInt(1);
        if (type2 == 2) end = arr[3].toDouble();
        return SPAinterval(start, end);
    }
    return SPAinterval(0, 1);
}

// 辅助：从 JSON 对象解析 surface，返回 APISESSION 分配的指针
// 注意：此函数应始终在 API_BEGIN/API_END 事务块内调用
static surface* parseSurface(const QJsonObject& gj, QString* error) {
    const QString type = gj.value("type").toString();
    if (type == "plane") {
        SPAposition root = parsePosition(gj.value("root_point").toArray());
        SPAunit_vector normal = parseUnitVector(gj.value("normal").toArray());
        SPAvector u_deriv = parseVector(gj.value("u_deriv").toArray());
        plane* def = ACIS_NEW plane(root, normal, u_deriv);
        // 仅在 uv_range 有效时设置 subset_range。
        // A 端原始 PLANE 的 gme_get_subset_range() 默认是 empty，
        // 若强制设置退化的 uv_range（如 [0,0,0,0]）会破坏后续 facet 行为，
        // 导致 B 端 CreateMeshFromEntity 拿到 0 face coord。
        SPApar_box uvrange = parseParBox(gj.value("uv_range").toArray());
        if (!uvrange.u_range().empty() && !uvrange.v_range().empty()) {
            def->gme_set_subset_range(uvrange);
        }
        PLANE* pl = ACIS_NEW PLANE(*def);
        ACIS_DELETE def;
        return (surface*)pl;
    } else if (type == "sphere") {
        SPAposition centre = parsePosition(gj.value("centre").toArray());
        double radius = gj.value("radius").toDouble(1.0);
        sphere* def = ACIS_NEW sphere(centre, radius);
        SPHERE* sp = ACIS_NEW SPHERE(*def);
        ACIS_DELETE def;
        return (surface*)sp;
    } else if (type == "torus") {
        SPAposition centre = parsePosition(gj.value("centre").toArray());
        SPAunit_vector normal = parseUnitVector(gj.value("normal").toArray());
        double major = gj.value("major_radius").toDouble(1.0);
        double minor = gj.value("minor_radius").toDouble(0.1);
        torus* def = ACIS_NEW torus(centre, normal, major, minor);
        TORUS* tr = ACIS_NEW TORUS(*def);
        ACIS_DELETE def;
        return (surface*)tr;
    } else if (type == "cone") {
        SPAposition base_centre = parsePosition(gj.value("base_centre").toArray());
        SPAunit_vector base_normal = parseUnitVector(gj.value("base_normal").toArray());
        SPAvector major_axis = parseVector(gj.value("major_axis").toArray());
        double rratio = gj.value("base_radius_ratio").toDouble(1.0);
        double sine = gj.value("sine_angle").toDouble(0.0);
        double cosine = gj.value("cosine_angle").toDouble(1.0);
        ellipse* gem_base = ACIS_NEW ellipse(base_centre, base_normal, major_axis, rratio);
        SPAinterval base_u_range = parseInterval(gj.value("uv_range").toArray());
        if (!base_u_range.empty()) {
            gem_base->gme_set_subset_range(base_u_range);
        }
        cone* def = ACIS_NEW cone(*gem_base, sine, cosine);
        ACIS_DELETE gem_base;
        CONE* cn = ACIS_NEW CONE(*def);
        ACIS_DELETE def;
        return (surface*)cn;
    }
    if (error) *error = QString::fromUtf8("unsupported surface type: %1").arg(type);
    return nullptr;
}

// 辅助：从 JSON 对象解析 curve，返回 APISESSION 分配的指针
// 注意：此函数应始终在 API_BEGIN/API_END 事务块内调用
static curve* parseCurve(const QJsonObject& gj, QString* error) {
    const QString type = gj.value("type").toString();
    if (type == "straight") {
        SPAposition root = parsePosition(gj.value("root_point").toArray());
        SPAvector direction = parseVector(gj.value("direction").toArray());
        straight* def = ACIS_NEW straight(root, normalise(direction));
        def->gme_set_param_scale(direction.len());
        SPAinterval range = parseInterval(gj.value("range").toArray());
        if (!range.empty()) {
            def->gme_set_subset_range(range);
        }
        STRAIGHT* st = ACIS_NEW STRAIGHT(*def);
        ACIS_DELETE def;
        return (curve*)st;
    } else if (type == "ellipse") {
        SPAposition centre = parsePosition(gj.value("centre").toArray());
        SPAunit_vector normal = parseUnitVector(gj.value("normal").toArray());
        SPAvector major_axis = parseVector(gj.value("major_axis").toArray());
        double ratio = gj.value("radius_ratio").toDouble(1.0);
        ellipse* def = ACIS_NEW ellipse(centre, normal, major_axis, ratio);
        SPAinterval range = parseInterval(gj.value("range").toArray());
        if (!range.empty()) {
            def->gme_set_subset_range(range);
        }
        ELLIPSE* el = ACIS_NEW ELLIPSE(*def);
        ACIS_DELETE def;
        return (curve*)el;
    }
    if (error) *error = QString::fromUtf8("unsupported curve type: %1").arg(type);
    return nullptr;
}

} // namespace JsonDeserialize

/**
 * 反序列化 JSON entity_graph 为 ACIS ENTITY_LIST
 *
 * 严格分三阶段执行（参考 access.cpp api_restore_entity_list_neo4j）：
 *   Pass 1: 创建所有实体骨架（全部包裹在 API_BEGIN/API_END 内）
 *   Pass 2: 设置几何属性（全部包裹在 API_BEGIN/API_END 内）
 *   Pass 3: 处理拓扑链接
 *
 * @param graphJson JSON 对象，包含 nodes[] 和 rels[]
 * @param outResult 输出结果，包含 entities 和 uuidToEntity 映射
 * @param errorMessage 错误信息
 * @return 成功返回 true
 */
bool deserializeACISEntityGraph(
    const QJsonObject& graphJson,
    DeserializedEntityGraph* outResult,
    QString* errorMessage
) {
    using namespace JsonDeserialize;

    fprintf(stderr, "[Collab] deserializeACISEntityGraph: ENTER, nodes=%lld\n", (long long)graphJson.value("nodes").toArray().size());

    if (errorMessage) errorMessage->clear();
    if (!outResult) {
        if (errorMessage) *errorMessage = QString::fromUtf8("outResult is null");
        return false;
    }
    outResult->entities = new ENTITY_LIST();
    outResult->uuidToEntity.clear();

    const QJsonArray nodes = graphJson.value("nodes").toArray();
    const QJsonArray rels = graphJson.value("rels").toArray();

    if (nodes.isEmpty()) {
        if (errorMessage) *errorMessage = QString::fromUtf8("entity_graph nodes is empty");
        delete outResult->entities;
        outResult->entities = nullptr;
        return false;
    }

    // id → ENTITY* 映射（用于后续阶段）
    QHash<QString, ENTITY*> idToEntity;

    // 统计
    int createdBody = 0, createdLump = 0, createdShell = 0, createdWire = 0,
        createdFace = 0, createdLoop = 0, createdCoedge = 0, createdEdge = 0,
        createdVertex = 0, createdTransform = 0, skippedUnknown = 0;

    // =========================================================================
    // Pass 1: 创建所有实体骨架（参考 access.cpp，全部包裹在 API_BEGIN/API_END）
    // 禁止在此阶段设置几何或拓扑链接
    // =========================================================================
    {
    for (const QJsonValue& nv : nodes) {
        const QJsonObject node = nv.toObject();
        const QString id = node.value("id").toString();
        const QJsonArray labels = node.value("labels").toArray();
        if (labels.isEmpty()) {
            qWarning() << "[Collab] deserialize: node has no labels, id=" << id;
            continue;
        }
        const QString label = labels[0].toString();

        ENTITY* ent = nullptr;

        if (label == "body") {
            BODY* body = nullptr;
            api_body(body);
            if (body) {
                ent = (ENTITY*)body;
                createdBody++;
            } else {
                qWarning().noquote() << "[Collab] deserialize: api_body failed, skipping body id=" << id;
            }
        } else if (label == "lump") {
            LUMP* lump = nullptr;
            API_BEGIN;
                lump = ACIS_NEW LUMP();
            API_END;
            ent = (ENTITY*)lump;
            createdLump++;
        } else if (label == "shell") {
            SHELL* shell = nullptr;
            API_BEGIN;
                shell = ACIS_NEW SHELL();
            API_END;
            ent = (ENTITY*)shell;
            createdShell++;
        } else if (label == "wire") {
            WIRE* wire = nullptr;
            API_BEGIN;
                wire = ACIS_NEW WIRE();
            API_END;
            ent = (ENTITY*)wire;
            createdWire++;
        } else if (label == "face") {
            FACE* face = nullptr;
            API_BEGIN;
                face = ACIS_NEW FACE();
            API_END;
            ent = (ENTITY*)face;
            createdFace++;
        } else if (label == "loop") {
            LOOP* loop = nullptr;
            API_BEGIN;
                loop = ACIS_NEW LOOP();
            API_END;
            ent = (ENTITY*)loop;
            createdLoop++;
        } else if (label == "coedge") {
            COEDGE* co = nullptr;
            API_BEGIN;
                co = ACIS_NEW COEDGE();
            API_END;
            ent = (ENTITY*)co;
            createdCoedge++;
        } else if (label == "edge") {
            EDGE* edge = nullptr;
            API_BEGIN;
                edge = ACIS_NEW EDGE();
            API_END;
            ent = (ENTITY*)edge;
            createdEdge++;
        } else if (label == "vertex") {
            VERTEX* vtx = nullptr;
            API_BEGIN;
                vtx = ACIS_NEW VERTEX();
            API_END;
            ent = (ENTITY*)vtx;
            createdVertex++;
        } else if (label == "transform") {
            SPAvector trans = parseVector(node.value("props").toObject().value("translation").toArray());
            SPAmatrix aff = parseMatrix(node.value("props").toObject().value("affine").toArray());
            double scaling = node.value("props").toObject().value("scaling").toDouble(1.0);
            int rotate = node.value("props").toObject().value("rotate").toInt(0);
            int reflect = node.value("props").toObject().value("reflect").toInt(0);
            int shear = node.value("props").toObject().value("shear").toInt(0);
            SPAtransf tf_data(aff, trans, scaling, rotate, reflect, shear);
            TRANSFORM* tf = nullptr;
            API_BEGIN;
                tf = ACIS_NEW TRANSFORM(tf_data);
            API_END;
            ent = (ENTITY*)tf;
            createdTransform++;
        } else if (label == "straight") {
            SPAposition root = parsePosition(node.value("props").toObject().value("root_point").toArray());
            SPAvector direction = parseVector(node.value("props").toObject().value("direction").toArray());
            SPAinterval range = parseInterval(node.value("props").toObject().value("range").toArray());
            straight* def = ACIS_NEW straight(root, normalise(direction));
            def->gme_set_param_scale(direction.len());
            def->gme_set_subset_range(range);
            STRAIGHT* st = nullptr;
            API_BEGIN;
                st = ACIS_NEW STRAIGHT(*def);
            API_END;
            ACIS_DELETE def;
            ent = (ENTITY*)st;
        } else if (label == "ellipse") {
            SPAposition centre = parsePosition(node.value("props").toObject().value("centre").toArray());
            SPAunit_vector normal = parseUnitVector(node.value("props").toObject().value("normal").toArray());
            SPAvector major_axis = parseVector(node.value("props").toObject().value("major_axis").toArray());
            double ratio = node.value("props").toObject().value("radius_ratio").toDouble(1.0);
            SPAinterval range = parseInterval(node.value("props").toObject().value("range").toArray());
            ellipse* def = ACIS_NEW ellipse(centre, normal, major_axis, ratio);
            def->gme_set_subset_range(range);
            ELLIPSE* el = nullptr;
            API_BEGIN;
                el = ACIS_NEW ELLIPSE(*def);
            API_END;
            ACIS_DELETE def;
            ent = (ENTITY*)el;
        } else if (label == "apoint") {
            SPAposition coords = parsePosition(node.value("props").toObject().value("position").toArray());
            APOINT* apt = nullptr;
            API_BEGIN;
                apt = ACIS_NEW APOINT(coords);
            API_END;
            ent = (ENTITY*)apt;
        } else {
            qWarning() << "[Collab] deserialize: unknown label=" << label << "id=" << id;
            skippedUnknown++;
            continue;
        }

        if (ent) {
            idToEntity[id] = ent;
            if (label == "body") {
                outResult->entities->add(ent);
                outResult->uuidToEntity[id] = (void*)ent;
            }
        }
    }
    }

    qDebug().noquote() << "[Collab] deserialize: pass1 done. body=" << createdBody << "lump=" << createdLump
                       << "shell=" << createdShell << "wire=" << createdWire << "face=" << createdFace
                       << "loop=" << createdLoop << "coedge=" << createdCoedge << "edge=" << createdEdge
                       << "vertex=" << createdVertex << "transform=" << createdTransform
                       << "skipped=" << skippedUnknown << "idToEntity.size=" << idToEntity.size();
    fprintf(stderr, "[Collab] deserialize: PASS 2 (geometry attrs)\n");

    // =========================================================================
    // Pass 2: 设置几何属性（包裹在 API_BEGIN/API_END 内）
    // 参考 access.cpp geometry 设置方式
    // =========================================================================
    {
    for (const QJsonValue& nv : nodes) {
        const QJsonObject node = nv.toObject();
        const QString id = node.value("id").toString();
        const QJsonArray labels = node.value("labels").toArray();
        if (labels.isEmpty()) continue;
        const QString label = labels[0].toString();
        const QJsonObject props = node.value("props").toObject();

        ENTITY* ent = idToEntity.value(id);
        if (!ent) continue;

        if (label == "face") {
            FACE* face = (FACE*)ent;
            const QJsonObject geom = props.value("geometry").toObject();
            surface* surf = nullptr;
            QString err;
            // parseSurface 内部使用 ACIS_NEW PLANE/SPHERE/...，必须整个在 API_BEGIN 内执行
            if (!geom.isEmpty()) {
                API_BEGIN;
                    surf = parseSurface(geom, &err);
                API_END;
            }
            API_BEGIN;
                face->set_sense(props.value("sense").toInt(1));
                int sides = props.value("sides").toInt(1);
                face->set_sides(sides != 0);
                if (sides != 0) face->set_cont(props.value("cont").toInt(0));
                if (surf) {
                    face->set_geometry((SURFACE*)surf);
                }
            API_END;
            if (surf == nullptr && !geom.isEmpty()) {
                qWarning().noquote() << "[Collab] deserialize face geometry:" << err;
            }
        } else if (label == "coedge") {
            COEDGE* co = (COEDGE*)ent;
            co->set_sense(props.value("sense").toInt(1));
        } else if (label == "edge") {
            EDGE* edge = (EDGE*)ent;
            const QJsonObject geom = props.value("geometry").toObject();
            curve* crv = nullptr;
            QString err;
            // parseCurve 内部使用 ACIS_NEW STRAIGHT/ELLIPSE/...，必须整个在 API_BEGIN 内执行
            if (!geom.isEmpty()) {
                API_BEGIN;
                    crv = parseCurve(geom, &err);
                API_END;
            }
            API_BEGIN;
                edge->set_sense(props.value("sense").toInt(1));
                if (crv) {
                    edge->set_geometry((CURVE*)crv);
                }
            API_END;
            if (crv == nullptr && !geom.isEmpty()) {
                qWarning().noquote() << "[Collab] deserialize edge geometry:" << err;
            }
        } else if (label == "vertex") {
            VERTEX* vtx = (VERTEX*)ent;
            const QJsonArray posArr = props.value("position").toArray();
            if (posArr.size() >= 3) {
                SPAposition pos = parsePosition(posArr);
                // 与 access.cpp apoint case 一致：APOINT 创建必须包在 API_BEGIN 内
                API_BEGIN;
                    APOINT* apt = ACIS_NEW APOINT(pos);
                    vtx->set_geometry(apt);
                API_END;
            }
        }
    }
    }

    fprintf(stderr, "[Collab] deserialize: PASS 2 done\n");
    fprintf(stderr, "[Collab] deserializeACISEntityGraph: PASS 3 (topology links)\n");
    // =========================================================================
    // Pass 3: 处理拓扑链接（参考 access.cpp api_restore_entity_list_neo4j）
    // 关系类型名与序列化端一致，不带 _ptr 后缀
    // =========================================================================
    int linkSuccess = 0, linkFail = 0;
    for (const QJsonValue& rv : rels) {
        const QJsonObject rel = rv.toObject();
        const QString type = rel.value("type").toString();
        const QString startId = rel.value("start").toString();
        const QString endId = rel.value("end").toString();

        ENTITY* startEnt = idToEntity.value(startId);
        ENTITY* endEnt = idToEntity.value(endId);
        if (!startEnt || !endEnt) {
            linkFail++;
            continue;
        }

        if (type == "body_lump") {
            ((BODY*)startEnt)->set_lump((LUMP*)endEnt);
            linkSuccess++;
        } else if (type == "body_wire") {
            ((BODY*)startEnt)->set_wire((WIRE*)endEnt);
            linkSuccess++;
        } else if (type == "body_transform") {
            ((BODY*)startEnt)->set_transform((TRANSFORM*)endEnt);
            linkSuccess++;
        } else if (type == "lump_shell") {
            ((LUMP*)startEnt)->set_shell((SHELL*)endEnt);
            linkSuccess++;
        } else if (type == "shell_lump") {
            ((SHELL*)startEnt)->set_lump((LUMP*)endEnt);
            linkSuccess++;
        } else if (type == "shell_face") {
            ((SHELL*)startEnt)->set_face((FACE*)endEnt);
            linkSuccess++;
        } else if (type == "shell_wire") {
            ((SHELL*)startEnt)->set_wire((WIRE*)endEnt);
            linkSuccess++;
        } else if (type == "shell_next") {
            ((SHELL*)startEnt)->set_next((SHELL*)endEnt);
            linkSuccess++;
        } else if (type == "wire_coedge") {
            ((WIRE*)startEnt)->set_coedge((COEDGE*)endEnt);
            linkSuccess++;
        } else if (type == "wire_next") {
            ((WIRE*)startEnt)->set_next((WIRE*)endEnt);
            linkSuccess++;
        } else if (type == "face_loop") {
            ((FACE*)startEnt)->set_loop((LOOP*)endEnt);
            linkSuccess++;
        } else if (type == "face_next") {
            ((FACE*)startEnt)->set_next((FACE*)endEnt);
            linkSuccess++;
        } else if (type == "loop_start") {
            ((LOOP*)startEnt)->set_start((COEDGE*)endEnt);
            linkSuccess++;
        } else if (type == "loop_face") {
            ((LOOP*)startEnt)->set_face((FACE*)endEnt);
            linkSuccess++;
        } else if (type == "loop_next") {
            ((LOOP*)startEnt)->set_next((LOOP*)endEnt);
            linkSuccess++;
        } else if (type == "coedge_next") {
            ((COEDGE*)startEnt)->set_next((COEDGE*)endEnt);
            linkSuccess++;
        } else if (type == "coedge_edge") {
            ((COEDGE*)startEnt)->set_edge((EDGE*)endEnt);
            linkSuccess++;
        } else if (type == "edge_start") {
            ((EDGE*)startEnt)->set_start((VERTEX*)endEnt);
            linkSuccess++;
        } else if (type == "edge_end") {
            ((EDGE*)startEnt)->set_end((VERTEX*)endEnt);
            linkSuccess++;
        } else if (type == "edge_coedge") {
            ((EDGE*)startEnt)->set_coedge((COEDGE*)endEnt);
            linkSuccess++;
        } else if (type == "lump_next") {
            ((LUMP*)startEnt)->set_next((LUMP*)endEnt);
            linkSuccess++;
        }
    }

    qDebug().noquote() << "[Collab] deserialize: pass3 done. linkSuccess=" << linkSuccess << "linkFail=" << linkFail << "rels.size=" << rels.size();
    fprintf(stderr, "[Collab] deserializeACISEntityGraph: EXIT, returning %s\n", outResult->entities->count() > 0 ? "true" : "false");

    // 关键：deserialize 完成后立即对所有 body 调用 api_facet_entity，
    // 让 face 上注册 mesh attribute (af_serializable_mesh)，否则后续
    // get_triangles_from_faceted_face 中 GetSerializableMesh(face) 返回 nullptr，
    // 导致 B 端 CreateMeshFromEntity 拿到 0 个 face coord。
    // 注意：必须包在 API_BEGIN/API_END 内。
    {
        API_BEGIN;
        for (ENTITY* ent = outResult->entities->first(); ent; ent = outResult->entities->next()) {
            if (is_BODY(ent)) {
                outcome fc = api_facet_entity(ent);
                fprintf(stderr, "[Collab] deserialize: post-facet body=%p ok=%d err=%d\n",
                        (void*)ent, fc.ok() ? 1 : 0, (int)fc.error_number());
                // 深度诊断已移除
                ENTITY_LIST faces;
                api_get_faces(ent, faces);
            }
        }
        API_END;
    }
    return outResult->entities->count() > 0;
}
