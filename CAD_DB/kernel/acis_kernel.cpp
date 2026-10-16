#include "geometry_kernel.h"
#include "legacy_storage.h"
#include "../acis.hxx"
#include "../access.hxx"
#include "../gme_dump_object.hxx"
#include "../entity_graph_serializer.h"
#include "acis/include/alltop.hxx"
#include "acis/include/boolapi.hxx"
#include "acis/include/cstrapi.hxx"
#include "acis/include/intrapi.hxx"
#include "acis/include/insanity_list.hxx"
#include "acis/include/kernapi.hxx"
#include "acis/include/rnd_api.hxx"
#include "acis/include/splgrid.hxx"
#include "acis/include/transfrm.hxx"
#include "acis/include/point.hxx"
#include "acis/include/part_api.hxx"
#include "acis/include/debug.hxx"
#include <QTemporaryFile>
#include <QDir>
#include <QScopeGuard>
#include <stdexcept>
#include <unordered_map>
#include <set>
#include <fstream>
#include <filesystem>
#include "acis/include/ckoutcom.hxx"
#include "../common.hxx"
extern bool CreateMeshFromEntity(ENTITY*, GmeMesh::DisplayData&);
#include "acis_primitives.inc"

namespace dbcad {
static std::uint64_t nextShapeId=1;
static void require(outcome value) {
    if (!value.ok()) throw std::runtime_error(process(value));
}
class AcisKernel final : public GeometryKernel {
    int level=0;
    std::unordered_map<std::uint64_t, ENTITY*> entities;
    std::unordered_map<ENTITY*, std::uint64_t> reverse;
public:
    AcisKernel() {
        level=initialize_acis();
        if(level<2) { terminate_acis(level); level=0; throw std::runtime_error("ACIS初始化失败"); }
        try { resetHistory(); } catch(...) { terminate_acis(level); throw; }
    }
    ~AcisKernel() override {
        // All SDK objects must be destroyed before thread-local modeller state.
        {
            ENTITY_LIST remaining;
            for(auto& item:entities) remaining.add(item.second);
            if(remaining.count()) api_del_entity_list(remaining);
            entities.clear(); reverse.clear();
        }
        terminate_acis(level);
    }
    ENTITY* native(ShapeHandle h) const {
        auto it=entities.find(h.value);
        if(it==entities.end()) throw std::runtime_error("形状句柄已失效");
        return it->second;
    }
    bool empty() const { return entities.empty(); }
    ShapeHandle adopt(ENTITY* e) {
        if(!e) return {};
        auto it=reverse.find(e);
        if(it!=reverse.end()) return {it->second};
        auto id=nextShapeId++; entities.emplace(id,e); reverse.emplace(e,id); return {id};
    }
    void forget(ShapeHandle h) {
        auto it=entities.find(h.value);
        if(it!=entities.end()) { reverse.erase(it->second); entities.erase(it); }
    }
    ENTITY_LIST nativeList(const ShapeList& values) const {
        ENTITY_LIST list; for(auto h:values) if(h) list.add(native(h)); return list;
    }
    ShapeList adoptList(ENTITY_LIST& list) {
        ShapeList result; for(ENTITY* e:list) result.push_back(adopt(e)); return result;
    }
    ShapeHandle draft() override {
        ENTITY* entity=nullptr;
        API_BEGIN;
        entity=ACIS_NEW ENTITY();
        API_END;
        require(result); return adopt(entity);
    }
    ShapeHandle create(int id, HandleGrid& handles) override {
        if(!handles.empty()) {
            auto width=handles.front().size();
            if(width==0) throw std::runtime_error("控制点不能为空");
            for(auto& row:handles) if(row.size()!=width) throw std::runtime_error("控制点网格宽度不一致");
            if(id==E_BEZIER && width<4) throw std::runtime_error("贝塞尔曲线需要四个控制点");
        }
        std::vector<std::vector<SPAposition>> points;
        for(auto& row:handles) { points.emplace_back(); for(auto p:row) points.back().emplace_back(p.x(),p.y(),p.z()); }
        ENTITY* e=nullptr;
        API_BEGIN;
        createPrimitive(id,e,points,result);
        API_END;
        require(result);
        handles.clear(); for(auto& row:points) { handles.emplace_back(); for(auto p:row) handles.back().emplace_back(p.x(),p.y(),p.z()); }
        if(!e) throw std::runtime_error("内核未生成几何");
        return adopt(e);
    }
    ShapeKind kind(ShapeHandle h) const override {
        ENTITY* e=native(h);
        if(is_BODY(e)) return ShapeKind::Body;
        if(is_FACE(e)) return ShapeKind::Face;
        if(is_EDGE(e)) return ShapeKind::Edge;
        if(is_VERTEX(e)) return ShapeKind::Vertex;
        return ShapeKind::Unknown;
    }
    void translate(ShapeHandle h, Translation t, bool bake) override {
        ENTITY* e=native(h);
        require(api_transform_entity(e,translate_transf(SPAvector(t.offset.x(),t.offset.y(),t.offset.z()))));
        if(bake && is_BODY(e)) require(api_change_body_trans((BODY*)e,nullptr));
    }
    void copyColor(ShapeHandle a, ShapeHandle b) override {
        rgb_color color; logical found=FALSE;
        require(api_rh_get_entity_rgb(native(a),color,TRUE,found));
        if(found) require(api_rh_set_entity_rgb(native(b),color));
    }
    void erase(const ShapeList& shapes) override {
        ENTITY_LIST list;
        for(auto h:shapes) if(h) list.add(native(h));
        if(list.count()) { require(api_del_entity_list(list)); }
        for(auto h:shapes) forget(h);
    }
    bool mesh(ShapeHandle h, GmeMesh::DisplayData& data) override {
        data={}; data.owner=h;
        bool built=CreateMeshFromEntity(native(h),data);
        for(auto& face:data.faceMesh) for(unsigned int i=0;i<face.numIndices;++i) {
            auto index=3*data.triangles[face.baseIndex+i];
            face.bounds.include({data.faceCoords[index],data.faceCoords[index+1],data.faceCoords[index+2]});
        }
        for(auto& edge:data.edgeMesh) for(unsigned int i=0;i+2<edge.numIndices;i+=3) {
            auto index=edge.baseVertex+i;
            edge.bounds.include({data.edgeCoords[index],data.edgeCoords[index+1],data.edgeCoords[index+2]});
        }
        return built && (!data.triangles.empty() || !data.edgeCoords.empty() || !data.vertexCoords.empty());
    }
    void resetHistory() {
        HISTORY_STREAM* hs=get_default_stream(true);
        delete_all_delta_states(hs,true); hs->clear(); initialize_delta_states();
        DELTA_STATE* root=nullptr; require(api_ensure_empty_root_state(hs,root));
    }
    void noteState() override { DELTA_STATE* ds=nullptr; require(api_note_state(ds)); }
    QJsonObject encodeGraph(const QHash<int,QString>& uuids, const ShapeList& shapes) override {
        auto list=nativeList(shapes); return serializeACISEntityGraph(uuids,list);
    }
    bool decodeGraph(const QJsonObject& graph, DecodedGraph& result, QString& error) override {
        DeserializedEntityGraph decoded{nullptr,{}};
        if(!deserializeACISEntityGraph(graph,&decoded,&error)) return false;
        result.shapes.clear();
        for(auto it=decoded.uuidToEntity.cbegin();it!=decoded.uuidToEntity.cend();++it)
            result.shapes.insert(it.key(),adopt(static_cast<ENTITY*>(it.value())));
        delete decoded.entities; return true;
    }
    QByteArray writeNative(const ShapeList& shapes) override {
        QTemporaryFile temp(QDir::tempPath()+"/dbcad_kernel_XXXXXX.sat");
        if(!temp.open()) throw std::runtime_error("无法创建内核临时文件");
        QString path=temp.fileName(); temp.close();
        FILE* file=_wfopen(reinterpret_cast<const wchar_t*>(path.utf16()),L"wb");
        if(!file) throw std::runtime_error("无法打开内核临时文件");
        outcome status;
        API_NOP_BEGIN;
        api_save_version(2,0);
        FileInfo info; info.set_units(1.0); info.set_product_id("demo_qt");
        result=api_set_file_info(FileIdent|FileUnits,info);
        if(result.ok()) result=api_set_int_option("sequence_save_files",1);
        if(result.ok()) { ENTITY_LIST list=nativeList(shapes); result=api_save_entity_list(file,true,list); }
        API_NOP_END;
        status=result;
        fclose(file); require(status);
        QFile input(path); if(!input.open(QIODevice::ReadOnly)) throw std::runtime_error("无法读取内核临时文件");
        return input.readAll();
    }
    ShapeList readNative(const QByteArray& data) override {
        QTemporaryFile temp(QDir::tempPath()+"/dbcad_kernel_XXXXXX.sat");
        if(!temp.open() || temp.write(data)!=data.size()) throw std::runtime_error("无法写入内核临时文件");
        QString path=temp.fileName(); temp.close();
        FILE* file=_wfopen(reinterpret_cast<const wchar_t*>(path.utf16()),L"rb");
        if(!file) throw std::runtime_error("无法读取内核临时文件");
        ENTITY_LIST list; outcome status;
        API_BEGIN;
        api_save_version(2,0); result=api_restore_entity_list(file,true,list);
        API_END;
        status=result;
        fclose(file);
        if(!status.ok()) { if(list.count()) api_del_entity_list(list); require(status); }
        return adoptList(list);
    }
    void dump(ShapeHandle h,const std::string& name,FILE* file,bool debug) override {
        if(debug) {
            api_set_int_option("brief_surface_debug",false);
            api_set_int_option("brief_curve_debug",false);
            api_set_int_option("brief_pcurve_debug",false);
            debug_entity(native(h),file);
        } else dump_object(file,name.c_str(),0,native(h));
    }
    void statistics(ShapeHandle h) override { print_statistic(native(h)); }
    ShapeList operate(OPERATOR_TYPES op,int subtype,const ShapeList& inputs) override;
};
static std::unique_ptr<GeometryKernel> instance;
std::unique_ptr<GeometryKernel> createGeometryKernel() { return std::make_unique<AcisKernel>(); }
GeometryKernel& kernel() { if(!instance) instance=createGeometryKernel(); return *instance; }
void shutdownKernel() { instance.reset(); }

ShapeList AcisKernel::operate(OPERATOR_TYPES op,int subtype,const ShapeList& inputs) {
    const auto minimum=op==OPERATOR_DEFEATURE?1u:2u;
    if(inputs.size()<minimum) throw std::runtime_error("未选择足够实体");
    ENTITY_LIST outputs;
    ENTITY_LIST temporaryBodies;
    const auto cleanup=qScopeGuard([&]() { if(temporaryBodies.count()) api_del_entity_list(temporaryBodies); });
    auto toBody=[&](ShapeHandle h) -> BODY* {
        ENTITY* e=native(h); BODY* b=nullptr;
        if(is_BODY(e)) return (BODY*)e;
        if(is_FACE(e)) { FACE* faces[]={(FACE*)e}; require(api_sheet_from_ff(1,faces,b)); }
        else if(is_EDGE(e)) {
            // api_make_ewire attaches its input edges. Always give it a private copy.
            ENTITY* copy=nullptr;
            require(api_deep_down_copy_entity(e,copy));
            EDGE* edges[]={(EDGE*)copy};
            auto status=api_make_ewire(1,edges,b);
            if(!status.ok()) { api_del_entity(copy); require(status); }
        }
        if(!b) throw std::runtime_error("该形状无法参与实体运算");
        temporaryBodies.add(b); return b;
    };
    if(op==OPERATOR_BOOLEAN) {
        BODY* a=toBody(inputs[0]); BODY* b=toBody(inputs[1]); BODY* output=nullptr;
        BOOL_TYPE operation;
        switch(static_cast<BooleanOperation>(subtype)) {
            case BooleanOperation::Unite: operation=UNION; break;
            case BooleanOperation::Intersect: operation=INTERSECTION; break;
            case BooleanOperation::Subtract: operation=SUBTRACTION; break;
            default: throw std::runtime_error("未知布尔运算");
        }
        // Retain inputs: document/history ownership is explicit and callers choose removal.
        require(api_boolean(b,a,operation,NDBOOL_KEEP_BOTH,output));
        if(output) outputs.add(output);
    } else if(op==OPERATOR_INTERSECTOR) {
        ENTITY* a=native(inputs[0]); ENTITY* b=native(inputs[1]); BODY* output=nullptr;
        if(is_FACE(a)&&is_FACE(b)) require(api_fafa_int((FACE*)a,(FACE*)b,output));
        else if(is_EDGE(a)&&is_FACE(b)) {
            ENTITY_LIST* list=nullptr; auto status=api_edfa_int((EDGE*)a,(FACE*)b,list);
            if(list) { outputs.add(*list); delete list; } require(status);
        }
        else if(is_FACE(a)&&is_EDGE(b)) {
            ENTITY_LIST* list=nullptr; auto status=api_edfa_int((EDGE*)b,(FACE*)a,list);
            if(list) { outputs.add(*list); delete list; } require(status);
        }
        else if(is_EDGE(a)&&is_EDGE(b)) {
            curve_curve_int* intersections=nullptr;
            require(api_inter_ed_ed((EDGE*)a,(EDGE*)b,intersections));
            if(intersections) require(api_ed_inters_to_ents((EDGE*)a,intersections,outputs));
        } else throw std::runtime_error("当前形状组合不支持求交");
        if(output) outputs.add(output);
    } else if(op==OPERATOR_DEFEATURE && subtype==DEFEATURE_UNREPEAT) {
        std::vector<BODY*> bodies; for(auto h:inputs) bodies.push_back(toBody(h));
        std::set<size_t> removed;
        for(size_t i=0;i<bodies.size();++i) for(size_t j=0;j<bodies.size();++j) {
            if(i==j || removed.contains(i) || removed.contains(j)) continue;
            BODY* a=nullptr; BODY* b=nullptr;
            require(api_boolean(bodies[j],bodies[i],SUBTRACTION,NDBOOL_KEEP_BOTH,a));
            require(api_boolean(bodies[i],bodies[j],SUBTRACTION,NDBOOL_KEEP_BOTH,b));
            if(a && !a->lump()) removed.insert(i);
            if(b && !b->lump()) removed.insert(j);
            if(a) api_del_entity(a); if(b) api_del_entity(b);
        }
        for(size_t i=0;i<bodies.size();++i) if(!removed.contains(i)) {
            ENTITY* copy=nullptr; require(api_deep_down_copy_entity(bodies[i],copy)); outputs.add(copy);
        }
    } else throw std::runtime_error("当前内核尚未实现该运算");
    return adoptList(outputs);
}
void initializeStorage() { mg_init(); }
void shutdownStorage() { mg_finalize(); }
static Neo4jPart connection(const DbConfig& c,const std::string& part) {
    return Neo4jPart(c.host.c_str(),c.port,c.username.c_str(),c.password.c_str(),part);
}
std::int64_t legacyPartCount(const DbConfig& c,const std::string& part) { auto db=connection(c,part); return count_partnode(db); }
void saveLegacyPart(const DbConfig& c,const std::string& part,const ShapeList& shapes,bool incremental) {
    auto db=connection(c,part);
    if(incremental) { IncrementalContext ctx; api_save_neo4j(db,ctx); }
    else { auto list=static_cast<AcisKernel&>(kernel()).nativeList(shapes); api_save_entity_list_neo4j_part(db,list); }
}
ShapeList loadLegacyPart(const DbConfig& c,const std::string& part,int generation) {
    auto db=connection(c,part); ENTITY_LIST list;
    if(generation>0 && !static_cast<AcisKernel&>(kernel()).empty())
        throw std::runtime_error("旧版增量加载需要空文档");
    if(generation>0) { IncrementalContext ctx; api_restore_neo4j(db,generation,ctx); acis_get_noattrib_toplevel_active_entities(list); }
    else api_restore_entity_list_neo4j_part(db,list);
    return static_cast<AcisKernel&>(kernel()).adoptList(list);
}
void runLegacyAccess(const DbConfig& config) {
    Neo4jPart conn(config.host.c_str(), config.port, config.username.c_str(), config.password.c_str(), "");

    for (const auto &entry : std::filesystem::directory_iterator(".\\testcases")) {
        if (entry.is_regular_file()) {
            std::string filepath = entry.path().string();
            std::string extname = std::filesystem::path(filepath).extension().string();
            std::transform(extname.begin(), extname.end(), extname.begin(), tolower); // 忽略大小写
            if (extname == ".sat") {
                std::string TestCaseName = std::filesystem::path(filepath).stem().string();
                ENTITY_LIST el;
                acis_restore_entity_list(el, filepath.c_str(), 2, 0, true);

                auto [testresult, neo4j_save_duration, acis_save_duration, neo4j_restore_duration,
                      acis_restore_duration] = AccessTest::CheckTestCase(conn, TestCaseName, el);
                api_del_entity_list(el);

                std::ofstream logfs("testlog.txt", std::ios::app);
                logfs << std::format("=======Test: {}=======", filepath) << std::endl;
                logfs << (testresult ? "PASS" : "FAIL") << "\t数据库存:" << neo4j_save_duration
                      << "ms\t文件存:" << acis_save_duration
                      << "ms\t数据库存/文件存:" << neo4j_save_duration / acis_save_duration
                      << "\t数据库取:" << neo4j_restore_duration << "ms\t文件取:" << acis_restore_duration
                      << "ms\t数据库取/文件取:" << neo4j_restore_duration / acis_restore_duration << std::endl;
                logfs.close();
            }
        }
    }

}

void runLegacyIncremental(const DbConfig& config) {
    TMDF;

    {

        std::ofstream logfs("incremental_dighole_testlog.txt", std::ios::app);
        BODY *block;
        check_outcome(api_solid_block(SPAposition(0.0, 0.0, 0.0), SPAposition(1000.0, 2000.0, 300.0), block));
        bool is_through = false;
        int holecnt = 0;
        for (int i = 10; i <= 990; i += 20) {      // 50个
            for (int j = 10; j <= 1990; j += 20) { // 100个
                BODY *hole;
                check_outcome(api_solid_cylinder_cone(SPAposition(i, j, is_through ? -10.0 : 100.0),
                                                      SPAposition(i, j, 310.0), 5.0, 5.0, 5.0, nullptr, hole));
                check_outcome(api_subtract(hole, block));
                holecnt++;

                std::string acis_save_filename_str = std::format("acis_partTest_save_{}.sat", holecnt);
                const char *acis_save_filename = acis_save_filename_str.c_str();
                std::string acis_save_check_filename_str = std::format("check_acis_partTest_save_{}.sat", holecnt);
                const char *acis_save_check_filename = acis_save_check_filename_str.c_str();
                std::string conn_check_filename_str = std::format("check_incrementalTest_{}.sat", holecnt);
                const char *conn_check_filename = conn_check_filename_str.c_str();

                Neo4jPart conn_incremental(config.host.c_str(), config.port, config.username.c_str(),
                                           config.password.c_str(), "incrementalTest");

                // 增量存
                TMST;
                IncrementalContext incCtx_incrementalSave;
                api_save_neo4j(conn_incremental, incCtx_incrementalSave);
                TMED;
                double neo4j_incremental_save_duration = TMDR;

                // ACIS存
                TMST;
                acis_save_noattrib_toplevel_active_entities(acis_save_filename, 2, 0, true);
                TMED;
                double acis_save_duration = TMDR;

                is_through = !is_through;
            }
        }
    }

}

}
