// JSON graph restoration: skeletons, geometry and topology retain their original order.
#include "entity_graph_serializer.h"

#include <acis/include/api.hxx>
#include <acis/include/cstrapi.hxx>
#include <acis/include/kernapi.hxx>
#include <acis/include/fct_utl.hxx>
#include <acis/include/rnd_api.hxx>
#include <acis/include/allcurve.hxx>
#include <acis/include/sps3crtn.hxx>
#include <acis/include/sp3crtn.hxx>
#include <acis/include/sps3srtn.hxx>
#include <acis/include/exct_spl.hxx>
#include <acis/include/pcudef.hxx>
#include <acis/include/pcurve.hxx>
#include <acis/include/sps2crtn.hxx>
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
#include <QSet>
#include <QDebug>
#include <unordered_set>
#include <cmath>
#include <deque>
#include <vector>

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
    return SPAposition(0, 0, 0);
}

// 辅助：从 JSON 数组解析 SPAvector
static SPAvector parseVector(const QJsonArray &arr) {
    if (arr.size() >= 3) {
        return SPAvector(arr[0].toDouble(), arr[1].toDouble(), arr[2].toDouble());
    }
    // SPAvector() 默认构造不置零（SAT 出现 1e-310 垃圾值），必须显式给确定值
    return SPAvector(0, 0, 0);
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
// 格式: [type1, val1, type2, val2]（恒 4 元素）；兼容旧格式 2–3 元素
// unbounded 置 true 表示区间为无限/半无限（旧图或无限区间），调用方应跳过 limit
static SPAinterval parseInterval(const QJsonArray &arr, bool *unbounded = nullptr) {
    if (unbounded)
        *unbounded = true;
    if (arr.size() >= 4) {
        int type1 = arr[0].toInt(1);
        int type2 = arr[2].toInt(1);
        if (type1 == 2 && type2 == 2) {
            if (unbounded)
                *unbounded = false;
            return SPAinterval(arr[1].toDouble(), arr[3].toDouble());
        }
    }
    return SPAinterval();
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
        // A 端原始 PLANE 的 subset_box() 默认是 empty，
        // 若强制设置退化的 uv_range（如 [0,0,0,0]）会破坏后续 facet 行为，
        // 导致 B 端 CreateMeshFromEntity 拿到 0 face coord。
        SPApar_box uvrange = parseParBox(gj.value("uv_range").toArray());
        if (!uvrange.u_range().empty() && !uvrange.v_range().empty()) {
            def->limit(uvrange);
        }
        PLANE *pl = ACIS_NEW PLANE(*def);
        ACIS_DELETE def;
        return (surface *)pl;
    } else if (type == "sphere") {
        SPAposition centre = parsePosition(gj.value("centre").toArray());
        double radius = gj.value("radius").toDouble(1.0);
        sphere *def = ACIS_NEW sphere(centre, radius);
        // 补丁面（如圆角球角片）依赖 subset 范围；全曲面时 uv_range 为空跳过
        SPApar_box uvrange = parseParBox(gj.value("uv_range").toArray());
        if (!uvrange.u_range().empty() && !uvrange.v_range().empty()) {
            def->limit(uvrange);
        }
        SPHERE *sp = ACIS_NEW SPHERE(*def);
        ACIS_DELETE def;
        return (surface *)sp;
    } else if (type == "torus") {
        SPAposition centre = parsePosition(gj.value("centre").toArray());
        SPAunit_vector normal = parseUnitVector(gj.value("normal").toArray());
        double major = gj.value("major_radius").toDouble(1.0);
        double minor = gj.value("minor_radius").toDouble(0.1);
        torus *def = ACIS_NEW torus(centre, normal, major, minor);
        SPApar_box uvrange2 = parseParBox(gj.value("uv_range").toArray());
        if (!uvrange2.u_range().empty() && !uvrange2.v_range().empty()) {
            def->limit(uvrange2);
        }
        TORUS *tr = ACIS_NEW TORUS(*def);
        ACIS_DELETE def;
        return (surface *)tr;
    } else if (type == "cone") {
        SPAposition base_centre = parsePosition(gj.value("base_centre").toArray());
        SPAunit_vector base_normal = parseUnitVector(gj.value("base_normal").toArray());
        // 字段名必须与序列化端一致：base_major_axis（旧代码读 "major_axis" 拿到
        // 空数组，配合未置零默认值产生垃圾锥面）
        SPAvector major_axis = parseVector(gj.value("base_major_axis").toArray());
        double rratio = gj.value("base_radius_ratio").toDouble(1.0);
        double sine = gj.value("sine_angle").toDouble(0.0);
        double cosine = gj.value("cosine_angle").toDouble(1.0);
        // para（u 参数化）不传时构造器默认 0，曲面损坏；旧图无此字段按 1 回退
        double uScale = gj.value("u_param_scale").toDouble(1.0);
        cone *def = ACIS_NEW cone(base_centre, base_normal, major_axis, rratio, sine, cosine, uScale);
        // uv_range 是裸值 [u0,u1,v0,v1]（parBoxToJson），仅在 u/v 均有效时限制曲面
        QJsonArray uv = gj.value("uv_range").toArray();
        if (uv.size() >= 4 && uv[1].toDouble() > uv[0].toDouble() && uv[3].toDouble() > uv[2].toDouble()) {
            def->limit(SPApar_box(SPAinterval(uv[0].toDouble(), uv[1].toDouble()),
                                  SPAinterval(uv[2].toDouble(), uv[3].toDouble())));
        }
        CONE *cn = ACIS_NEW CONE(*def);
        ACIS_DELETE def;
        return (surface *)cn;
    } else if (type == "spline") {
        QJsonArray ctrlArr = gj.value("ctrlpts").toArray();
        if (ctrlArr.size() < 12) {
            if (error)
                *error = QString::fromUtf8("spline payload missing (legacy graph)");
            return nullptr;
        }
        int num_u = gj.value("num_u").toInt(0);
        int num_v = gj.value("num_v").toInt(0);
        if (num_u <= 0 || num_v <= 0) {
            if (error)
                *error = QString::fromUtf8("spline num_u/num_v invalid");
            return nullptr;
        }
        bool rational_u = gj.value("rational_u").toInt(0) != 0;
        bool rational_v = gj.value("rational_v").toInt(0) != 0;
        bool rational = rational_u || rational_v;
        auto expandKnots = [](const QJsonArray &arr, std::deque<double> &out) {
            for (int i = 0; i * 2 + 1 < arr.size(); ++i) {
                double k = arr[i * 2].toDouble();
                int cnt = (int)arr[i * 2 + 1].toDouble();
                for (int j = 0; j < cnt; ++j)
                    out.push_back(k);
            }
            out.push_front(out.front());
            out.push_back(out.back());
        };
        std::deque<double> uq, vq;
        expandKnots(gj.value("uknots").toArray(), uq);
        expandKnots(gj.value("vknots").toArray(), vq);
        std::vector<double> ku(uq.begin(), uq.end()), kv(vq.begin(), vq.end());
        int stride = rational ? 4 : 3;
        std::vector<SPAposition> ctrl(num_u * num_v);
        std::vector<double> weights;
        if (rational)
            weights.resize(num_u * num_v, 1.0);
        for (int i = 0; i < num_v; ++i) {
            for (int j = 0; j < num_u; ++j) {
                int flat = (i * num_u + j) * stride;
                ctrl[j * num_v + i] = SPAposition(ctrlArr[flat].toDouble(), ctrlArr[flat + 1].toDouble(),
                                                  ctrlArr[flat + 2].toDouble());
                if (rational)
                    weights[j * num_v + i] = ctrlArr[flat + 3].toDouble();
            }
        }
        int pole_u = gj.value("u_singularity").toInt(0);
        int pole_v = gj.value("v_singularity").toInt(0);
        bs3_surface bs = bs3_surface_from_ctrlpts(
            gj.value("degree_u").toInt(3), (logical)(rational_u ? 1 : 0), gj.value("form_u").toInt(0), pole_u, num_u,
            gj.value("degree_v").toInt(3), (logical)(rational_v ? 1 : 0), gj.value("form_v").toInt(0), pole_v, num_v,
            ctrl.data(), rational ? weights.data() : nullptr, SPAresabs.value(), (int)ku.size(), ku.data(),
            (int)kv.size(), kv.data(), SPAresnor.value());
        // exact_spl_sur 承载 NURBS 定义；fitol 由内核默认维护
        spl_sur *ss = ACIS_NEW exact_spl_sur(bs);
        spline *def = ACIS_NEW spline(ss);
        if (gj.value("reversed").toInt(0) != 0) {
            *def = -(*def);
        }
        SPApar_box uvrange3 = parseParBox(gj.value("uv_range").toArray());
        if (!uvrange3.u_range().empty() && !uvrange3.v_range().empty()) {
            def->limit(uvrange3);
        }
        SPLINE *sp = ACIS_NEW SPLINE(*def);
        ACIS_DELETE def;
        return (surface *)sp;
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
        // SAT 惯例：直线恢复为无界（SAT 落盘 I I），边的参数区间由起终顶点推导。
        // 不能按序列化区间 limit：get_param_range() 是归一化值（真实区间可能是
        // [-0.8,0] 而序列化成 [0,0.8]），limit 后端顶点投影不进曲线 → 长度退化 0
        //（探针实证：抽壳连接边 0.8 → 0）。
        straight *def = ACIS_NEW straight(root, normalise(direction), direction.len());
        STRAIGHT *st = ACIS_NEW STRAIGHT(*def);
        ACIS_DELETE def;
        return (curve *)st;
    } else if (type == "ellipse") {
        SPAposition centre = parsePosition(gj.value("centre").toArray());
        SPAunit_vector normal = parseUnitVector(gj.value("normal").toArray());
        SPAvector major_axis = parseVector(gj.value("major_axis").toArray());
        double ratio = gj.value("radius_ratio").toDouble(1.0);
        ellipse *def = ACIS_NEW ellipse(centre, normal, major_axis, ratio);
        bool unbounded = true;
        SPAinterval range = parseInterval(gj.value("range").toArray(), &unbounded);
        if (!unbounded && !range.empty()) {
            def->limit(range);
        }
        ELLIPSE *el = ACIS_NEW ELLIPSE(*def);
        ACIS_DELETE def;
        return (curve *)el;
    } else if (type == "helix") {
        if (!gj.contains("axis_root") || !gj.contains("helix_range")) {
            // 旧图仅有类型名，无载荷（缺口3 历史数据），维持原拒绝行为
            if (error)
                *error = QString::fromUtf8("helix payload missing (legacy graph)");
            return nullptr;
        }
        SPAposition axis_root = parsePosition(gj.value("axis_root").toArray());
        SPAunit_vector axis_dir = parseUnitVector(gj.value("axis_dir").toArray());
        SPAvector start_disp = parseVector(gj.value("start_disp").toArray());
        double pitch = gj.value("pitch").toDouble(1.0);
        int handedness = gj.value("handedness").toInt(1);
        double par_scaling = gj.value("par_scaling").toDouble(1.0);
        double taper = gj.value("taper").toDouble(0.0);
        bool hrUnbounded = true;
        SPAinterval helix_range = parseInterval(gj.value("helix_range").toArray(), &hrUnbounded);
        if (hrUnbounded)
            helix_range = SPAinterval(0.0, 1.0);
        helix *def = ACIS_NEW helix(axis_root, axis_dir, start_disp, pitch, (logical)handedness, helix_range,
                                    par_scaling, taper);
        bool unbounded = true;
        SPAinterval range = parseInterval(gj.value("range").toArray(), &unbounded);
        if (!unbounded && !range.empty()) {
            def->limit(range);
        }
        HELIX *hx = ACIS_NEW HELIX(*def);
        ACIS_DELETE def;
        return (curve *)hx;
    } else if (type == "intcurve") {
        QJsonArray knotArr = gj.value("knots").toArray();
        QJsonArray ctrlArr = gj.value("ctrlpts").toArray();
        if (knotArr.size() < 4 || ctrlArr.size() < 3) {
            // 旧图仅有类型名（缺口2 历史数据），维持原拒绝行为
            if (error)
                *error = QString::fromUtf8("intcurve payload missing (legacy graph)");
            return nullptr;
        }
        int degree = gj.value("degree").toInt(3);
        bool rational = gj.value("rational").toInt(0) != 0;
        int form = gj.value("form").toInt(0);
        // 展开 RLE 并补端点多重性（与 Bolt 栈恢复逻辑一致）
        std::deque<double> knots;
        for (int i = 0; i * 2 + 1 < knotArr.size(); ++i) {
            double k = knotArr[i * 2].toDouble();
            int cnt = (int)knotArr[i * 2 + 1].toDouble();
            for (int j = 0; j < cnt; ++j)
                knots.push_back(k);
        }
        knots.push_front(knots.front());
        knots.push_back(knots.back());
        int stride = rational ? 4 : 3;
        int numCtrl = (int)ctrlArr.size() / stride;
        std::vector<SPAposition> ctrl(numCtrl);
        std::vector<double> weights;
        if (rational)
            weights.resize(numCtrl, 1.0);
        for (int i = 0; i < numCtrl; ++i) {
            ctrl[i] = SPAposition(ctrlArr[i * stride].toDouble(), ctrlArr[i * stride + 1].toDouble(),
                                  ctrlArr[i * stride + 2].toDouble());
            if (rational)
                weights[i] = ctrlArr[i * stride + 3].toDouble();
        }
        std::vector<double> knotsVec(knots.begin(), knots.end());
        bs3_curve bs = bs3_curve_from_ctrlpts(degree, (logical)(rational ? 1 : 0), (logical)(form >= 1 ? 1 : 0),
                                              (logical)(form == 2 ? 1 : 0), numCtrl, ctrl.data(),
                                              rational ? weights.data() : nullptr, SPAresabs.value(),
                                              (int)knotsVec.size(), knotsVec.data(), SPAresnor.value());
        if (form == 0)
            bs3_curve_set_open(bs);
        else if (form == 1)
            bs3_curve_set_closed(bs);
        else
            bs3_curve_set_periodic(bs);
        double fitol = gj.value("fitol").toDouble(1e-6);
        // 文档化路径："exact B-spline from bs3_curve"——bs3 为唯一主表示，
        // 双端曲面留空（精确交线重建是后续增强）
        intcurve *def =
            ACIS_NEW intcurve(bs, fitol, SpaAcis::NullObj::get_surface(), SpaAcis::NullObj::get_surface());
        bool unbounded = true;
        SPAinterval range = parseInterval(gj.value("range").toArray(), &unbounded);
        if (!unbounded && !range.empty()) {
            def->limit(range);
        }
        INTCURVE *ic = ACIS_NEW INTCURVE(*def);
        ACIS_DELETE def;
        return (curve *)ic;
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

    // =====================================================================
    // SAT 片段回退（fail-safe 的恢复端）：body 节点带 props.sat_fragment 的
    // 组件整体走内核 SAT 读回（100% 一致，含 ATTRIB/过程曲面定义/边参数化），
    // 该组件全部节点跳过 pass1/2/3（内核已完整重建，避免二次 set_* 损坏）。
    // 片段恢复失败则该组件回退普通图重建（不劣于现状）。
    // =========================================================================
    QSet<QString> fragmentIds;
    int fragmentBodies = 0;
    {
        QHash<QString, QSet<QString>> adj;
        for (const QJsonValue &rv : rels) {
            const QJsonObject r = rv.toObject();
            const QString s = r.value("start").toString(), t = r.value("end").toString();
            adj[s].insert(t);
            adj[t].insert(s);
        }
        for (const QJsonValue &nv : nodes) {
            const QJsonObject node = nv.toObject();
            const QJsonArray labels = node.value("labels").toArray();
            if (labels.isEmpty() || labels[0].toString() != "body")
                continue;
            const QString id = node.value("id").toString();
            const QString frag = node.value("props").toObject().value("sat_fragment").toString();
            if (frag.isEmpty())
                continue;
            bool ok = false;
            FILE *tf = tmpfile();
            if (tf) {
                const QByteArray bytes = frag.toLatin1();
                if (fwrite(bytes.constData(), 1, (size_t)bytes.size(), tf) == (size_t)bytes.size()) {
                    rewind(tf);
                    ENTITY_LIST lst;
                    if (api_restore_entity_list(tf, TRUE, lst).ok()) {
                        BODY *rb = nullptr;
                        for (ENTITY *en = lst.first(); en; en = lst.next())
                            if (!rb && is_BODY(en))
                                rb = (BODY *)en;
                        if (rb) {
                            for (ENTITY *en = lst.first(); en; en = lst.next())
                                outResult->entities->add(en);
                            outResult->uuidToEntity[id] = (void *)rb;
                            fragmentBodies++;
                            // 组件闭包全部跳过图重建
                            QSet<QString> seen{id};
                            QQueue<QString> q;
                            q.enqueue(id);
                            while (!q.isEmpty()) {
                                const QString cur = q.dequeue();
                                fragmentIds.insert(cur);
                                for (const QString &nb : adj.value(cur))
                                    if (!seen.contains(nb)) {
                                        seen.insert(nb);
                                        q.enqueue(nb);
                                    }
                            }
                            fprintf(stderr,
                                    "[Collab] deserialize: sat_fragment restored body=%s entities=%d (skips graph rebuild)\n",
                                    id.toUtf8().constData(), lst.count());
                            ok = true;
                        } else {
                            // 无 BODY（异常片段）：清掉已恢复实体，回退图重建
                            for (ENTITY *en = lst.first(); en; en = lst.next())
                                api_del_entity(en);
                        }
                    }
                }
                fclose(tf);
            }
            if (!ok)
                qWarning() << "[Collab] deserialize: sat_fragment restore FAILED for body" << id
                           << "- falling back to graph rebuild";
        }
    }

    // 统计
    int createdBody = 0, createdLump = 0, createdShell = 0, createdWire = 0, createdFace = 0, createdLoop = 0,
        createdCoedge = 0, createdSubshell = 0, createdEdge = 0, createdVertex = 0, createdTransform = 0, skippedUnknown = 0;

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

            if (fragmentIds.contains(id))
                continue; // SAT 片段组件：内核已整组件重建，pass1/2/3 全跳过

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
            } else if (label == "subshell") {
                SUBSHELL *ss = nullptr;
                API_BEGIN;
                ss = ACIS_NEW SUBSHELL();
                API_END;
                ent = (ENTITY *)ss;
                createdSubshell++;
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
                // 平移/缩放按实体存储值原样重建。api_transform_entity 会把几何
                // 重定位并把剩余变换存入 TRANSFORM 实体——分量必须逐字往返，
                // 任何"补偿"都会破坏作用（G9 教训：曾误加 /scaling 导致偏移）。
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
                straight *def = ACIS_NEW straight(root, normalise(direction), direction.len());
                def->limit(range);
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
                def->limit(range);
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
                       << "satFragments=" << fragmentBodies << "idToEntity.size=" << idToEntity.size();
    fprintf(stderr, "[Collab] deserialize: PASS 2 (geometry attrs)\n");

    // pcurve 装配所需的拓扑反查：coedge → owner loop → face（宿主曲面）
    QHash<QString, QString> loopToFace, coedgeToOwnerLoop;
    for (const QJsonValue &rv : rels) {
        const QJsonObject r = rv.toObject();
        const QString rt = r.value("type").toString();
        if (rt == "face_loop")
            loopToFace.insert(r.value("end").toString(), r.value("start").toString());
        else if (rt == "coedge_owner")
            coedgeToOwnerLoop.insert(r.value("start").toString(), r.value("end").toString());
    }
    QHash<QString, surface *> faceSurfaces;
    QSet<QString> splineFaces;

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
                    faceSurfaces.insert(id, surf);
                    if (!geom.isEmpty() && geom.value("type").toString() == "spline")
                        splineFaces.insert(id);
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
                if (props.contains("convexity")) {
                    edge->set_convexity((EDGE_cvty)props.value("convexity").toInt(9));
                }
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
    // Pass 2.5：pcurve 装配。惰性 pcurve 在序列化端常未物化（cur()==null），
    // 统一处理：有 bs2 载荷用载荷；否则 pcurve(3D 曲线, 曲面, 容差) 由 ACIS
    // 重新拟合——与原版语义等价（原 pcurve 即同一对曲线/曲面的拟合）。
    {
        int fittedPcurve = 0, payloadPcurve = 0;
        for (const QJsonValue &nv : nodes) {
            const QJsonObject node25 = nv.toObject();
            const QJsonArray labels25 = node25.value("labels").toArray();
            if (labels25.isEmpty() || labels25[0].toString() != "coedge")
                continue;
            const QString cid = node25.value("id").toString();
            COEDGE *co = (COEDGE *)idToEntity.value(cid, nullptr);
            if (co == nullptr)
                continue;
            surface *hostSurf = nullptr;
            const QString ownerLoop = coedgeToOwnerLoop.value(cid);
            if (!ownerLoop.isEmpty()) {
                const QString faceId = loopToFace.value(ownerLoop);
                if (!faceId.isEmpty() && splineFaces.contains(faceId))
                    hostSurf = faceSurfaces.value(faceId, nullptr);
            }
            if (hostSurf == nullptr)
                continue;
            CURVE *edgeCurve = nullptr;
            for (const QJsonValue &rv : rels) {
                const QJsonObject r = rv.toObject();
                if (r.value("type").toString() == "coedge_edge" && r.value("start").toString() == cid) {
                    ENTITY *ed = idToEntity.value(r.value("end").toString(), nullptr);
                    if (ed != nullptr && is_EDGE(ed))
                        edgeCurve = ((EDGE *)ed)->geometry();
                    break;
                }
            }
            const QJsonObject pgeo = node25.value("props").toObject().value("geometry").toObject();
            bool done = false;
            if (pgeo.value("type").toString() == "pcurve" && pgeo.contains("ctrlpts")) {
                const int deg = pgeo.value("degree").toInt(3);
                const bool rat = pgeo.value("rational").toInt(0) != 0;
                const int form = pgeo.value("form").toInt(0);
                std::deque<double> ks;
                const QJsonArray kn = pgeo.value("knots").toArray();
                for (int i = 0; i * 2 + 1 < kn.size(); ++i) {
                    double k = kn[i * 2].toDouble();
                    int cnt = (int)kn[i * 2 + 1].toDouble();
                    for (int j = 0; j < cnt; ++j)
                        ks.push_back(k);
                }
                if (!ks.empty()) {
                    ks.push_front(ks.front());
                    ks.push_back(ks.back());
                    std::vector<double> kv(ks.begin(), ks.end());
                    const int stride = rat ? 3 : 2;
                    const int numCtrl = pgeo.value("num_ctrlpts").toInt(0);
                    const QJsonArray ct = pgeo.value("ctrlpts").toArray();
                    if (numCtrl > 0 && (int)ct.size() >= numCtrl * stride) {
                        std::vector<SPAposition> c2(numCtrl, SPAposition(0, 0, 0));
                        std::vector<double> w2;
                        if (rat)
                            w2.resize(numCtrl, 1.0);
                        for (int i = 0; i < numCtrl; ++i) {
                            c2[i] = SPAposition(ct[i * stride].toDouble(), ct[i * stride + 1].toDouble(), 0);
                            if (rat)
                                w2[i] = ct[i * stride + 2].toDouble();
                        }
                        API_BEGIN;
                        bs2_curve bs2 = bs2_curve_from_ctrlpts(
                            deg, (logical)(rat ? 1 : 0), (logical)(form >= 1 ? 1 : 0),
                            (logical)(form == 2 ? 1 : 0), numCtrl, c2.data(), rat ? w2.data() : nullptr, DBL_MIN,
                            (int)kv.size(), kv.data(), DBL_MIN);
                        // hostSurf 实为 SURFACE 实体指针（parseSurface 返回实体），必须取 equation()
                        // 作方程对象传入——直接解引用实体是类型混淆，求值即 AV（G13 教训）
                        pcurve *pdef = ACIS_NEW pcurve(bs2, 1.0, ((SURFACE *)hostSurf)->equation());
                        PCURVE *pent = ACIS_NEW PCURVE(*pdef);
                        ACIS_DELETE pdef;
                        co->set_geometry(pent);
                        API_END;
                        payloadPcurve++;
                        done = true;
                    }
                }
            }
            if (!done && edgeCurve != nullptr) {
                API_BEGIN;
                pcurve *pdef = ACIS_NEW pcurve(edgeCurve->equation(), ((SURFACE *)hostSurf)->equation(), SPAresabs);
                PCURVE *pent = ACIS_NEW PCURVE(*pdef);
                ACIS_DELETE pdef;
                co->set_geometry(pent);
                API_END;
                fittedPcurve++;
            }
        }
        fprintf(stderr, "[Collab] deserialize: pcurve assembly payload=%d fitted=%d\n", payloadPcurve, fittedPcurve);
    }
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
        } else if (type == "shell_subshell") {
            ((SHELL *)startEnt)->set_subshell((SUBSHELL *)endEnt);
            linkSuccess++;
        } else if (type == "subshell_parent") {
            ((SUBSHELL *)startEnt)->set_parent((SUBSHELL *)endEnt);
            linkSuccess++;
        } else if (type == "subshell_sibling") {
            ((SUBSHELL *)startEnt)->set_sibling((SUBSHELL *)endEnt);
            linkSuccess++;
        } else if (type == "subshell_child") {
            ((SUBSHELL *)startEnt)->set_child((SUBSHELL *)endEnt);
            linkSuccess++;
        } else if (type == "subshell_face") {
            ((SUBSHELL *)startEnt)->set_face((FACE *)endEnt);
            linkSuccess++;
        } else if (type == "subshell_wire") {
            ((SUBSHELL *)startEnt)->set_wire((WIRE *)endEnt);
            linkSuccess++;
        } else if (type == "face_subshell") {
            ((FACE *)startEnt)->set_subshell((SUBSHELL *)endEnt);
            linkSuccess++;
        } else if (type == "wire_subshell") {
            ((WIRE *)startEnt)->set_subshell((SUBSHELL *)endEnt);
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
