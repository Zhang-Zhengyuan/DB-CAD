// JSON graph restoration: skeletons, geometry and topology retain their original order.
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
#include <cmath>

// ============================================================================
// 反序列化：JSON → ACIS
// ============================================================================

namespace JsonDeserialize {

// 辅助：从 JSON 数组 [u0, u1, v0, v1] 解析 SPApar_box
static SPApar_box parseParBox(const QJsonArray &arr) {
    if (arr.size() >= 4) {
        const double u0 = arr[0].toDouble();
        const double u1 = arr[1].toDouble();
        const double v0 = arr[2].toDouble();
        const double v1 = arr[3].toDouble();
        if (std::isfinite(u0) && std::isfinite(u1) && std::isfinite(v0) && std::isfinite(v1) && u1 > u0 && v1 > v0) {
            return SPApar_box(SPAinterval(u0, u1), SPAinterval(v0, v1));
        }
    }
    return SPApar_box();
}

// 辅助：从 JSON 数组解析 SPAposition
static SPAposition parsePosition(const QJsonArray &arr) {
    if (arr.size() >= 3) {
        return SPAposition(arr[0].toDouble(), arr[1].toDouble(), arr[2].toDouble());
    }
    return SPAposition();
}

// 辅助：从 JSON 数组解析 SPAvector
static SPAvector parseVector(const QJsonArray &arr) {
    if (arr.size() >= 3) {
        return SPAvector(arr[0].toDouble(), arr[1].toDouble(), arr[2].toDouble());
    }
    return SPAvector();
}

// 辅助：从 JSON 数组解析 SPAunit_vector
static SPAunit_vector parseUnitVector(const QJsonArray &arr) {
    if (arr.size() >= 3) {
        return SPAunit_vector(arr[0].toDouble(), arr[1].toDouble(), arr[2].toDouble());
    }
    return SPAunit_vector(0, 0, 1);
}

// 辅助：从 JSON 数组解析 SPAmatrix（列主序）
static SPAmatrix parseMatrix(const QJsonArray &arr) {
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
static SPAinterval parseInterval(const QJsonArray &arr) {
    if (arr.size() >= 4) {
        double start = 0.0, end = 1.0;
        int type1 = arr[0].toInt(1);
        if (type1 == 2)
            start = arr[1].toDouble();
        int type2 = arr[2].toInt(1);
        if (type2 == 2)
            end = arr[3].toDouble();
        return SPAinterval(start, end);
    }
    return SPAinterval(0, 1);
}

// 辅助：从 JSON 对象解析 surface，返回 APISESSION 分配的指针
// 注意：此函数应始终在 API_BEGIN/API_END 事务块内调用
static surface *parseSurface(const QJsonObject &gj, QString *error) {
    const QString type = gj.value("type").toString();
    if (type == "plane") {
        SPAposition root = parsePosition(gj.value("root_point").toArray());
        SPAunit_vector normal = parseUnitVector(gj.value("normal").toArray());
        SPAvector u_deriv = parseVector(gj.value("u_deriv").toArray());
        plane *def = ACIS_NEW plane(root, normal, u_deriv);
        // 仅在 uv_range 有效时设置 subset_range。
        // A 端原始 PLANE 的 gme_get_subset_range() 默认是 empty，
        // 若强制设置退化的 uv_range（如 [0,0,0,0]）会破坏后续 facet 行为，
        // 导致 B 端 CreateMeshFromEntity 拿到 0 face coord。
        SPApar_box uvrange = parseParBox(gj.value("uv_range").toArray());
        if (!uvrange.u_range().empty() && !uvrange.v_range().empty()) {
            def->gme_set_subset_range(uvrange);
        }
        PLANE *pl = ACIS_NEW PLANE(*def);
        ACIS_DELETE def;
        return (surface *)pl;
    } else if (type == "sphere") {
        SPAposition centre = parsePosition(gj.value("centre").toArray());
        double radius = gj.value("radius").toDouble(1.0);
        sphere *def = ACIS_NEW sphere(centre, radius);
        SPHERE *sp = ACIS_NEW SPHERE(*def);
        ACIS_DELETE def;
        return (surface *)sp;
    } else if (type == "torus") {
        SPAposition centre = parsePosition(gj.value("centre").toArray());
        SPAunit_vector normal = parseUnitVector(gj.value("normal").toArray());
        double major = gj.value("major_radius").toDouble(1.0);
        double minor = gj.value("minor_radius").toDouble(0.1);
        torus *def = ACIS_NEW torus(centre, normal, major, minor);
        TORUS *tr = ACIS_NEW TORUS(*def);
        ACIS_DELETE def;
        return (surface *)tr;
    } else if (type == "cone") {
        SPAposition base_centre = parsePosition(gj.value("base_centre").toArray());
        SPAunit_vector base_normal = parseUnitVector(gj.value("base_normal").toArray());
        SPAvector major_axis = parseVector(gj.value("major_axis").toArray());
        double rratio = gj.value("base_radius_ratio").toDouble(1.0);
        double sine = gj.value("sine_angle").toDouble(0.0);
        double cosine = gj.value("cosine_angle").toDouble(1.0);
        ellipse *gem_base = ACIS_NEW ellipse(base_centre, base_normal, major_axis, rratio);
        SPAinterval base_u_range = parseInterval(gj.value("uv_range").toArray());
        if (!base_u_range.empty()) {
            gem_base->gme_set_subset_range(base_u_range);
        }
        cone *def = ACIS_NEW cone(*gem_base, sine, cosine);
        ACIS_DELETE gem_base;
        CONE *cn = ACIS_NEW CONE(*def);
        ACIS_DELETE def;
        return (surface *)cn;
    }
    if (error)
        *error = QString::fromUtf8("unsupported surface type: %1").arg(type);
    return nullptr;
}

// 辅助：从 JSON 对象解析 curve，返回 APISESSION 分配的指针
// 注意：此函数应始终在 API_BEGIN/API_END 事务块内调用
static curve *parseCurve(const QJsonObject &gj, QString *error) {
    const QString type = gj.value("type").toString();
    if (type == "straight") {
        SPAposition root = parsePosition(gj.value("root_point").toArray());
        SPAvector direction = parseVector(gj.value("direction").toArray());
        straight *def = ACIS_NEW straight(root, normalise(direction));
        def->gme_set_param_scale(direction.len());
        SPAinterval range = parseInterval(gj.value("range").toArray());
        if (!range.empty()) {
            def->gme_set_subset_range(range);
        }
        STRAIGHT *st = ACIS_NEW STRAIGHT(*def);
        ACIS_DELETE def;
        return (curve *)st;
    } else if (type == "ellipse") {
        SPAposition centre = parsePosition(gj.value("centre").toArray());
        SPAunit_vector normal = parseUnitVector(gj.value("normal").toArray());
        SPAvector major_axis = parseVector(gj.value("major_axis").toArray());
        double ratio = gj.value("radius_ratio").toDouble(1.0);
        ellipse *def = ACIS_NEW ellipse(centre, normal, major_axis, ratio);
        SPAinterval range = parseInterval(gj.value("range").toArray());
        if (!range.empty()) {
            def->gme_set_subset_range(range);
        }
        ELLIPSE *el = ACIS_NEW ELLIPSE(*def);
        ACIS_DELETE def;
        return (curve *)el;
    }
    if (error)
        *error = QString::fromUtf8("unsupported curve type: %1").arg(type);
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
bool deserializeACISEntityGraph(const QJsonObject &graphJson, DeserializedEntityGraph *outResult,
                                QString *errorMessage) {
    using namespace JsonDeserialize;

    fprintf(stderr, "[Collab] entity_graph_serializer deserialize build=uvsafe-topology-v5-bounded\n");

    fprintf(stderr, "[Collab] deserializeACISEntityGraph: ENTER, nodes=%lld\n",
            (long long)graphJson.value("nodes").toArray().size());

    if (errorMessage)
        errorMessage->clear();
    if (!outResult) {
        if (errorMessage)
            *errorMessage = QString::fromUtf8("outResult is null");
        return false;
    }
    outResult->entities = new ENTITY_LIST();
    outResult->uuidToEntity.clear();

    const QJsonArray nodes = graphJson.value("nodes").toArray();
    const QJsonArray rels = graphJson.value("rels").toArray();

    if (nodes.isEmpty()) {
        if (errorMessage)
            *errorMessage = QString::fromUtf8("entity_graph nodes is empty");
        delete outResult->entities;
        outResult->entities = nullptr;
        return false;
    }

    // id → ENTITY* 映射（用于后续阶段）
    QHash<QString, ENTITY *> idToEntity;

    // 统计
    int createdBody = 0, createdLump = 0, createdShell = 0, createdWire = 0, createdFace = 0, createdLoop = 0,
        createdCoedge = 0, createdEdge = 0, createdVertex = 0, createdTransform = 0, skippedUnknown = 0;

    // =========================================================================
    // Pass 1: 创建所有实体骨架（参考 access.cpp，全部包裹在 API_BEGIN/API_END）
    // 禁止在此阶段设置几何或拓扑链接
    // =========================================================================
    {
        for (const QJsonValue &nv : nodes) {
            const QJsonObject node = nv.toObject();
            const QString id = node.value("id").toString();
            const QJsonArray labels = node.value("labels").toArray();
            if (labels.isEmpty()) {
                qWarning() << "[Collab] deserialize: node has no labels, id=" << id;
                continue;
            }
            const QString label = labels[0].toString();

            ENTITY *ent = nullptr;

            if (label == "body") {
                BODY *body = nullptr;
                api_body(body);
                if (body) {
                    ent = (ENTITY *)body;
                    createdBody++;
                } else {
                    qWarning().noquote() << "[Collab] deserialize: api_body failed, skipping body id=" << id;
                }
            } else if (label == "lump") {
                LUMP *lump = nullptr;
                API_BEGIN;
                lump = ACIS_NEW LUMP();
                API_END;
                ent = (ENTITY *)lump;
                createdLump++;
            } else if (label == "shell") {
                SHELL *shell = nullptr;
                API_BEGIN;
                shell = ACIS_NEW SHELL();
                API_END;
                ent = (ENTITY *)shell;
                createdShell++;
            } else if (label == "wire") {
                WIRE *wire = nullptr;
                API_BEGIN;
                wire = ACIS_NEW WIRE();
                API_END;
                ent = (ENTITY *)wire;
                createdWire++;
            } else if (label == "face") {
                FACE *face = nullptr;
                API_BEGIN;
                face = ACIS_NEW FACE();
                API_END;
                ent = (ENTITY *)face;
                createdFace++;
            } else if (label == "loop") {
                LOOP *loop = nullptr;
                API_BEGIN;
                loop = ACIS_NEW LOOP();
                API_END;
                ent = (ENTITY *)loop;
                createdLoop++;
            } else if (label == "coedge") {
                COEDGE *co = nullptr;
                API_BEGIN;
                co = ACIS_NEW COEDGE();
                API_END;
                ent = (ENTITY *)co;
                createdCoedge++;
            } else if (label == "edge") {
                EDGE *edge = nullptr;
                API_BEGIN;
                edge = ACIS_NEW EDGE();
                API_END;
                ent = (ENTITY *)edge;
                createdEdge++;
            } else if (label == "vertex") {
                VERTEX *vtx = nullptr;
                API_BEGIN;
                vtx = ACIS_NEW VERTEX();
                API_END;
                ent = (ENTITY *)vtx;
                createdVertex++;
            } else if (label == "transform") {
                SPAvector trans = parseVector(node.value("props").toObject().value("translation").toArray());
                SPAmatrix aff = parseMatrix(node.value("props").toObject().value("affine").toArray());
                double scaling = node.value("props").toObject().value("scaling").toDouble(1.0);
                int rotate = node.value("props").toObject().value("rotate").toInt(0);
                int reflect = node.value("props").toObject().value("reflect").toInt(0);
                int shear = node.value("props").toObject().value("shear").toInt(0);
                SPAtransf tf_data(aff, trans, scaling, rotate, reflect, shear);
                TRANSFORM *tf = nullptr;
                API_BEGIN;
                tf = ACIS_NEW TRANSFORM(tf_data);
                API_END;
                ent = (ENTITY *)tf;
                createdTransform++;
            } else if (label == "straight") {
                SPAposition root = parsePosition(node.value("props").toObject().value("root_point").toArray());
                SPAvector direction = parseVector(node.value("props").toObject().value("direction").toArray());
                SPAinterval range = parseInterval(node.value("props").toObject().value("range").toArray());
                straight *def = ACIS_NEW straight(root, normalise(direction));
                def->gme_set_param_scale(direction.len());
                def->gme_set_subset_range(range);
                STRAIGHT *st = nullptr;
                API_BEGIN;
                st = ACIS_NEW STRAIGHT(*def);
                API_END;
                ACIS_DELETE def;
                ent = (ENTITY *)st;
            } else if (label == "ellipse") {
                SPAposition centre = parsePosition(node.value("props").toObject().value("centre").toArray());
                SPAunit_vector normal = parseUnitVector(node.value("props").toObject().value("normal").toArray());
                SPAvector major_axis = parseVector(node.value("props").toObject().value("major_axis").toArray());
                double ratio = node.value("props").toObject().value("radius_ratio").toDouble(1.0);
                SPAinterval range = parseInterval(node.value("props").toObject().value("range").toArray());
                ellipse *def = ACIS_NEW ellipse(centre, normal, major_axis, ratio);
                def->gme_set_subset_range(range);
                ELLIPSE *el = nullptr;
                API_BEGIN;
                el = ACIS_NEW ELLIPSE(*def);
                API_END;
                ACIS_DELETE def;
                ent = (ENTITY *)el;
            } else if (label == "apoint") {
                SPAposition coords = parsePosition(node.value("props").toObject().value("position").toArray());
                APOINT *apt = nullptr;
                API_BEGIN;
                apt = ACIS_NEW APOINT(coords);
                API_END;
                ent = (ENTITY *)apt;
            } else {
                qWarning() << "[Collab] deserialize: unknown label=" << label << "id=" << id;
                skippedUnknown++;
                continue;
            }

            if (ent) {
                idToEntity[id] = ent;
                if (label == "body") {
                    outResult->entities->add(ent);
                    outResult->uuidToEntity[id] = (void *)ent;
                }
            }
        }
    }

    qDebug().noquote() << "[Collab] deserialize: pass1 done. body=" << createdBody << "lump=" << createdLump
                       << "shell=" << createdShell << "wire=" << createdWire << "face=" << createdFace
                       << "loop=" << createdLoop << "coedge=" << createdCoedge << "edge=" << createdEdge
                       << "vertex=" << createdVertex << "transform=" << createdTransform << "skipped=" << skippedUnknown
                       << "idToEntity.size=" << idToEntity.size();
    fprintf(stderr, "[Collab] deserialize: PASS 2 (geometry attrs)\n");

    // =========================================================================
    // Pass 2: 设置几何属性（包裹在 API_BEGIN/API_END 内）
    // 参考 access.cpp geometry 设置方式
    // =========================================================================
    {
        for (const QJsonValue &nv : nodes) {
            const QJsonObject node = nv.toObject();
            const QString id = node.value("id").toString();
            const QJsonArray labels = node.value("labels").toArray();
            if (labels.isEmpty())
                continue;
            const QString label = labels[0].toString();
            const QJsonObject props = node.value("props").toObject();

            ENTITY *ent = idToEntity.value(id);
            if (!ent)
                continue;

            if (label == "face") {
                FACE *face = (FACE *)ent;
                const QJsonObject geom = props.value("geometry").toObject();
                surface *surf = nullptr;
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
                if (sides != 0)
                    face->set_cont(props.value("cont").toInt(0));
                if (surf) {
                    face->set_geometry((SURFACE *)surf);
                }
                API_END;
                if (surf == nullptr && !geom.isEmpty()) {
                    qWarning().noquote() << "[Collab] deserialize face geometry:" << err;
                }
            } else if (label == "coedge") {
                COEDGE *co = (COEDGE *)ent;
                co->set_sense(props.value("sense").toInt(1));
            } else if (label == "edge") {
                EDGE *edge = (EDGE *)ent;
                const QJsonObject geom = props.value("geometry").toObject();
                curve *crv = nullptr;
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
                    edge->set_geometry((CURVE *)crv);
                }
                API_END;
                if (crv == nullptr && !geom.isEmpty()) {
                    qWarning().noquote() << "[Collab] deserialize edge geometry:" << err;
                }
            } else if (label == "wire") {
                ((WIRE *)ent)->set_cont(props.value("cont").toInt(0));
            } else if (label == "vertex") {
                VERTEX *vtx = (VERTEX *)ent;
                const QJsonArray posArr = props.value("position").toArray();
                if (posArr.size() >= 3) {
                    SPAposition pos = parsePosition(posArr);
                    // 与 access.cpp apoint case 一致：APOINT 创建必须包在 API_BEGIN 内
                    API_BEGIN;
                    APOINT *apt = ACIS_NEW APOINT(pos);
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
    for (const QJsonValue &rv : rels) {
        const QJsonObject rel = rv.toObject();
        const QString type = rel.value("type").toString();
        const QString startId = rel.value("start").toString();
        const QString endId = rel.value("end").toString();

        ENTITY *startEnt = idToEntity.value(startId);
        ENTITY *endEnt = idToEntity.value(endId);
        if (!startEnt || !endEnt) {
            linkFail++;
            continue;
        }

        if (type == "body_lump") {
            ((BODY *)startEnt)->set_lump((LUMP *)endEnt);
            linkSuccess++;
        } else if (type == "body_wire") {
            ((BODY *)startEnt)->set_wire((WIRE *)endEnt);
            linkSuccess++;
        } else if (type == "body_transform") {
            ((BODY *)startEnt)->set_transform((TRANSFORM *)endEnt);
            linkSuccess++;
        } else if (type == "lump_shell") {
            ((LUMP *)startEnt)->set_shell((SHELL *)endEnt);
            linkSuccess++;
        } else if (type == "shell_lump") {
            ((SHELL *)startEnt)->set_lump((LUMP *)endEnt);
            linkSuccess++;
        } else if (type == "shell_face") {
            ((SHELL *)startEnt)->set_face((FACE *)endEnt);
            linkSuccess++;
        } else if (type == "shell_wire") {
            ((SHELL *)startEnt)->set_wire((WIRE *)endEnt);
            linkSuccess++;
        } else if (type == "shell_next") {
            ((SHELL *)startEnt)->set_next((SHELL *)endEnt);
            linkSuccess++;
        } else if (type == "wire_coedge") {
            ((WIRE *)startEnt)->set_coedge((COEDGE *)endEnt);
            linkSuccess++;
        } else if (type == "wire_next") {
            ((WIRE *)startEnt)->set_next((WIRE *)endEnt);
            linkSuccess++;
        } else if (type == "face_loop") {
            ((FACE *)startEnt)->set_loop((LOOP *)endEnt);
            linkSuccess++;
        } else if (type == "face_next") {
            ((FACE *)startEnt)->set_next((FACE *)endEnt);
            linkSuccess++;
        } else if (type == "face_shell") {
            ((FACE *)startEnt)->set_shell((SHELL *)endEnt);
            linkSuccess++;
        } else if (type == "loop_start") {
            ((LOOP *)startEnt)->set_start((COEDGE *)endEnt);
            linkSuccess++;
        } else if (type == "loop_face") {
            ((LOOP *)startEnt)->set_face((FACE *)endEnt);
            linkSuccess++;
        } else if (type == "loop_next") {
            ((LOOP *)startEnt)->set_next((LOOP *)endEnt);
            linkSuccess++;
        } else if (type == "coedge_next") {
            ((COEDGE *)startEnt)->set_next((COEDGE *)endEnt);
            linkSuccess++;
        } else if (type == "coedge_previous") {
            ((COEDGE *)startEnt)->set_previous((COEDGE *)endEnt);
            linkSuccess++;
        } else if (type == "coedge_partner") {
            ((COEDGE *)startEnt)->set_partner((COEDGE *)endEnt);
            linkSuccess++;
        } else if (type == "coedge_edge") {
            ((COEDGE *)startEnt)->set_edge((EDGE *)endEnt);
            linkSuccess++;
        } else if (type == "coedge_owner") {
            ((COEDGE *)startEnt)->set_owner((ENTITY *)endEnt);
            linkSuccess++;
        } else if (type == "edge_start") {
            ((EDGE *)startEnt)->set_start((VERTEX *)endEnt);
            linkSuccess++;
        } else if (type == "edge_end") {
            ((EDGE *)startEnt)->set_end((VERTEX *)endEnt);
            linkSuccess++;
        } else if (type == "edge_coedge") {
            ((EDGE *)startEnt)->set_coedge((COEDGE *)endEnt);
            linkSuccess++;
        } else if (type == "vertex_edge") {
            ((VERTEX *)startEnt)->set_edge((EDGE *)endEnt);
            linkSuccess++;
        } else if (type == "lump_next") {
            ((LUMP *)startEnt)->set_next((LUMP *)endEnt);
            linkSuccess++;
        } else if (type == "lump_body") {
            ((LUMP *)startEnt)->set_body((BODY *)endEnt);
            linkSuccess++;
        } else if (type == "wire_owner") {
            ((WIRE *)startEnt)->set_owner((ENTITY *)endEnt);
            linkSuccess++;
        }
    }

    // Neo4j/JSON graphs can arrive with only the forward pointers emitted by
    // older clients. Rebuild the ACIS owner/back pointers after all forward
    // links are present; faceting relies on these links even though
    // api_get_faces may still enumerate the forward topology without them.
    {
        API_BEGIN;
        for (ENTITY *root = outResult->entities->first(); root; root = outResult->entities->next()) {
            if (!is_BODY(root))
                continue;
            BODY *body = (BODY *)root;
            int lumpGuard = 0;
            for (LUMP *lump = body->lump(); lump && lumpGuard++ < 1000; lump = lump->next()) {
                int shellGuard = 0;
                for (SHELL *shell = lump->shell(); shell && shellGuard++ < 1000; shell = shell->next()) {
                    int faceGuard = 0;
                    for (FACE *face = shell->face(); face && faceGuard++ < 10000; face = face->next()) {
                        face->set_shell(shell);
                        if (face->loop())
                            face->loop()->set_face(face);
                        int loopGuard = 0;
                        for (LOOP *loop = face->loop(); loop && loopGuard++ < 1000; loop = loop->next()) {
                            int coedgeGuard = 0;
                            for (COEDGE *co = loop->start(); co && coedgeGuard++ < 10000; co = co->next()) {
                                co->set_owner(loop);
                                if (co->edge())
                                    co->edge()->set_coedge(co);
                                if (co->next())
                                    co->next()->set_previous(co);
                                if (co->partner())
                                    co->partner()->set_partner(co);
                            }
                        }
                    }
                }
            }
        }
        API_END;
    }

    qDebug().noquote() << "[Collab] deserialize: pass3 done. linkSuccess=" << linkSuccess << "linkFail=" << linkFail
                       << "rels.size=" << rels.size();
    fprintf(stderr, "[Collab] deserializeACISEntityGraph: EXIT, returning %s\n",
            outResult->entities->count() > 0 ? "true" : "false");

    // Do not facet here.  Pull runs on the Qt GUI thread, and ACIS faceting
    // may perform an unbounded secondary-geometry computation on a graph
    // restored without PCURVEs.  The display layer performs one bounded facet
    // attempt when it builds the mesh and has a planar fallback for faces that
    // remain unfaceted.
    fprintf(stderr, "[Collab] deserialize: structure-only complete; faceting deferred to display\n");
    return outResult->entities->count() > 0;
}
