#include "DbcadAPI.h"

#include <stdexcept>
#include <unordered_map>
#include <utility>

#include "acis.hxx"
#include "neo4j.hxx"
#include "gme_mesh.hxx"

extern void api_save_entity_list_neo4j_part(const Neo4jPart& conn, const ENTITY_LIST& entity_list);
extern void api_restore_entity_list_neo4j_part(const Neo4jPart& conn, ENTITY_LIST& entity_list);
extern int64_t count_partnode(const Neo4jPart& conn);
extern void api_save_neo4j(const Neo4jPart& conn, struct IncrementalContext& ctx);
extern void api_restore_neo4j(const Neo4jPart& conn, int generation_id, struct IncrementalContext& ctx);
extern void acis_get_noattrib_toplevel_active_entities(ENTITY_LIST& elist, class HISTORY_STREAM* hs = nullptr);
extern bool CreateMeshFromEntityList(ENTITY_LIST& el, GmeMesh::DisplayData& dd);

struct IncrementalContext
{
    class DELTA_STATE* lastsave_ds = nullptr;
    std::unordered_map<void*, int64_t> ptr2nodeid;
};

namespace dbcad
{
    bool InitializeEnvironment()
    {
        if (0 == initialize_acis()) return false;
        mg_init(); // 初始化 Memgraph/Neo4j 客户端
        return true;
    }

    void TerminateEnvironment()
    {
        mg_finalize();
        terminate_acis(2);
    }

    struct ModelerSession::Impl
    {
        DbConfig config;
        std::string partName;
        IncrementalContext incCtx; // 每个 Session 有独立的增量上下文

        Impl(DbConfig c, std::string p) : config(std::move(c)), partName(std::move(p))
        {
        }

        std::unique_ptr<Neo4jPart> CreateConnection()
        {
            return std::make_unique<Neo4jPart>(
                config.host.c_str(),
                config.port,
                config.username.c_str(),
                config.password.c_str(),
                partName
            );
        }
    };

    ModelerSession::ModelerSession(const DbConfig& config, const std::string& partName)
        : pImpl(std::make_unique<Impl>(config, partName))
    {
    }

    ModelerSession::~ModelerSession() = default;

    void ModelerSession::GetActiveEntities(ENTITY_LIST& outList)
    {
        acis_get_noattrib_toplevel_active_entities(outList);
    }

    void ModelerSession::SavePartFull(const ENTITY_LIST& entityList) const
    {
        auto conn = pImpl->CreateConnection();
        api_save_entity_list_neo4j_part(*conn, entityList);
    }

    void ModelerSession::LoadPartFull(ENTITY_LIST& outEntityList) const
    {
        auto conn = pImpl->CreateConnection();
        int64_t count = count_partnode(*conn);
        if (count == 0) throw std::runtime_error("Part does not exist.");
        if (count > 1) throw std::runtime_error("Part is not unique in DB.");

        api_restore_entity_list_neo4j_part(*conn, outEntityList);
    }

    int64_t ModelerSession::CheckPartExists() const
    {
        auto conn = pImpl->CreateConnection();
        return count_partnode(*conn);
    }

    void ModelerSession::SavePartIncremental() const
    {
        auto conn = pImpl->CreateConnection();
        api_save_neo4j(*conn, pImpl->incCtx);
    }

    void ModelerSession::LoadPartIncremental(int generation, ENTITY_LIST& outEntityList) const
    {
        auto conn = pImpl->CreateConnection();
        int64_t count = count_partnode(*conn);
        if (count == 0) throw std::runtime_error("Part does not exist.");

        api_restore_neo4j(*conn, generation, pImpl->incCtx);
        acis_get_noattrib_toplevel_active_entities(outEntityList);
    }

    bool ModelerSession::GenerateMesh(ENTITY_LIST& el, GmeMesh::DisplayData& dd)
    {
        return CreateMeshFromEntityList(el, dd);
    }
}


