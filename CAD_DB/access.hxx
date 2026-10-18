#pragma once
#include <acis/include/lists.hxx>
#include <cstdio>
#include <string>

// SAT 文件存取：本地持久化与交换格式的唯一入口。
// 旧 Bolt 直连 Neo4j 的存取栈已在架构 v2 重构中移除（git tag: legacy/pre-architecture-v2）。

void acis_save_entity_list(const ENTITY_LIST& elist, const char* file_name, int major_version, int minor_version, int text_mode);
void acis_save_entity_list(FILE* file, const ENTITY_LIST& elist, int major_version, int minor_version, int text_mode);
void acis_save_noattrib_toplevel_active_entities(const char* file_name, int major_version, int minor_version, int text_mode, HISTORY_STREAM* hs = NULL);
void acis_get_noattrib_toplevel_active_entities(ENTITY_LIST& elist, HISTORY_STREAM* hs = NULL);
void acis_save_history(const char* file_name, int major_version, int minor_version, int text_mode, HISTORY_STREAM* hs = NULL);
void acis_restore_entity_list(ENTITY_LIST& elist, const char* file_name, int major_version, int minor_version, int text_mode);
void acis_restore_entity_list(ENTITY_LIST& elist, FILE* file, int major_version, int minor_version, int text_mode);
