#pragma once
#include "neo4j.hxx"
#include <acis/include/lists.hxx>
#include <unordered_map>
#include <string>

struct IncrementalContext
{
    class DELTA_STATE* lastsave_ds = nullptr;
    std::unordered_map<void*, int64_t> ptr2nodeid;
};

void api_save_entity_list_neo4j(const Neo4jPart& conn, const ENTITY_LIST& entity_list,
                                std::unordered_map<void*, int64_t>& ptr2id);
void api_restore_entity_list_neo4j(const Neo4jPart& conn, const std::vector<int64_t>& id_list, ENTITY_LIST& entity_list,
                                   std::unordered_map<int64_t, void*>& id2ptr);

void api_save_neo4j(const Neo4jPart& conn, IncrementalContext& ctx);
void api_restore_neo4j(const Neo4jPart& conn, int generation_id, IncrementalContext& ctx);

void api_save_entity_list_neo4j_part(const Neo4jPart& conn, const ENTITY_LIST& entity_list);
void api_restore_entity_list_neo4j_part(const Neo4jPart& conn, ENTITY_LIST& entity_list);

int64_t count_partnode(const Neo4jPart& conn);

void acis_save_entity_list(const ENTITY_LIST& elist, const char* file_name, int major_version, int minor_version,
                           int text_mode);
void acis_save_noattrib_toplevel_active_entities(const char* file_name, int major_version, int minor_version,
                                                 int text_mode, HISTORY_STREAM* hs = NULL);
void acis_get_noattrib_toplevel_active_entities(ENTITY_LIST& elist, HISTORY_STREAM* hs = NULL);
void acis_save_history(const char* file_name, int major_version, int minor_version, int text_mode,
                       HISTORY_STREAM* hs = NULL);
void acis_restore_entity_list(ENTITY_LIST& elist, const char* file_name, int major_version, int minor_version,
                              int text_mode);

namespace AccessTest
{
    std::string read_file_to_string(std::string filename);
    std::tuple<bool, double, double, double, double> CheckTestCase(const Neo4jPart& conn, std::string testcase_name,
                                                                   const ENTITY_LIST& el);
}
