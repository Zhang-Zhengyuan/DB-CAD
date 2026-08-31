#pragma once
#include "neo4j.hxx"
#include <acis/include/lists.hxx>
#include <cstdio>
#include <unordered_map>
#include <string>
#include <vector>

struct IncrementalContext
{
    class DELTA_STATE* lastsave_ds = nullptr;
    std::unordered_map<void*, int64_t> ptr2nodeid;
};

// 协作增量 delta 计算结果（不含任何 Neo4j 操作）
struct CollabDelta {
    std::vector<class ENTITY*> created_or_updated; // 新增或修改的 top-level bodies
    std::vector<class ENTITY*> deleted;            // 已删除的 top-level bodies
};

// 计算 [ctx.lastsave_ds, 当前最新] 之间的 top-level body delta。
// created_or_updated 包含 root body（BODY_ID），
// deleted 包含已被标记删除的 root body。
// 调用后 ctx.lastsave_ds 会推进到当前最新状态。
// 注意：只计算 root body（BODY_ID），不包含 LUMP/SHELL 等子实体。
void api_compute_delta_since(IncrementalContext& ctx, CollabDelta& delta);

// 将 ctx.lastsave_ds 推进到当前最新 delta 状态。
// 在 submit_delta 收到 submit_accepted 后调用，防止 delta 漂移。
void api_advance_delta_since(IncrementalContext& ctx);

// 获取当前 ACIS 最新 delta 状态指针（不含 Neo4j IO）
void api_note_current_state(class DELTA_STATE*& out_current_ds);

void api_save_entity_list_neo4j(const Neo4jPart& conn, const ENTITY_LIST& entity_list,
                                std::unordered_map<void*, int64_t>& ptr2id);
void api_restore_entity_list_neo4j(const Neo4jPart& conn, const std::vector<int64_t>& id_list, ENTITY_LIST& entity_list,
                                   std::unordered_map<int64_t, void*>& id2ptr);

void api_save_neo4j(const Neo4jPart& conn, IncrementalContext& ctx);
void api_restore_neo4j(const Neo4jPart& conn, int generation_id, IncrementalContext& ctx);

void api_save_entity_list_neo4j_part(const Neo4jPart& conn, const ENTITY_LIST& entity_list);
void api_restore_entity_list_neo4j_part(const Neo4jPart& conn, ENTITY_LIST& entity_list);

int64_t count_partnode(const Neo4jPart& conn);

// ================================================================================================
// Mode1 Delta Push/Pull（C++端，供 storage_bridge 调用）
// handle_save_delta_to_neo4j:
//   - base_version: A 端上次 push 的版本号
//   - delta_uuids + delta_sat_segments: 本次新增/修改的 body uuid + 独立 SAT 文本
//   - removed_uuids: 本次删除的 body uuid
//   - author + timestamp: 用于更新 part 节点
//   - out_new_version: 输出新版本号
//   - 返回 true 成功，false 失败（out_error 包含错误信息）
// ================================================================================================
bool handle_save_delta_to_neo4j(
    const Neo4jPart& conn,
    int base_version,
    const std::vector<std::string>& delta_uuids,
    const std::vector<std::string>& delta_sat_segments,
    const std::vector<std::string>& removed_uuids,
    const std::string& author,
    const std::string& timestamp,
    int& out_new_version,
    std::string& out_error
);

// ================================================================================================
// handle_get_delta_from_neo4j:
//   - base_version: B 端上次 sync 的版本号
//   - out_latest_version: 输出最新版本号
//   - out_body_uuids: 输出本次增量涉及的 body uuid 列表（仅 base_version+1..latest 之间的新增/修改）
//   - out_sat_segments_by_uuid: 输出与 body uuid 一一对应的 SAT 段列表
//   - out_deleted_uuids: 输出 base_version+1..latest 之间被删除的 body uuid 列表（last-write-wins）
//   - 返回 true 成功，false 失败（out_error 包含错误信息）
//
// 【Mode1 增量 pull】从 BridgeDeltaVersion 节点回放 base_version+1..latest 的历史，
// 只把期间真正变化的 body（added/modified/removed）发给客户端，而不是返回整个 part 当前状态。
// 历史缺失（v=N 的 BridgeDeltaVersion 不存在）时降级为全量回退（旧行为）。
// ================================================================================================
bool handle_get_delta_from_neo4j(
    const Neo4jPart& conn,
    int base_version,
    int& out_latest_version,
    std::vector<std::string>& out_body_uuids,
    std::vector<std::string>& out_sat_segments_by_uuid,
    std::vector<std::string>& out_deleted_uuids,
    std::string& out_error
);

void acis_save_entity_list(const ENTITY_LIST& elist, const char* file_name, int major_version, int minor_version, int text_mode);
void acis_save_entity_list(FILE* file, const ENTITY_LIST& elist, int major_version, int minor_version, int text_mode);
void acis_save_noattrib_toplevel_active_entities(const char* file_name, int major_version, int minor_version, int text_mode, HISTORY_STREAM* hs = NULL);
void acis_get_noattrib_toplevel_active_entities(ENTITY_LIST& elist, HISTORY_STREAM* hs = NULL);
void acis_save_history(const char* file_name, int major_version, int minor_version, int text_mode, HISTORY_STREAM* hs = NULL);
void acis_restore_entity_list(ENTITY_LIST& elist, const char* file_name, int major_version, int minor_version, int text_mode);
void acis_restore_entity_list(ENTITY_LIST& elist, FILE* file, int major_version, int minor_version, int text_mode);
namespace AccessTest {
    std::string read_file_to_string(std::string filename);
    std::tuple<bool, double, double, double, double> CheckTestCase(const Neo4jPart& conn, std::string testcase_name,
                                                                   const ENTITY_LIST& el);
}
