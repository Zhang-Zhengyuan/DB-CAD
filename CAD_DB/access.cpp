#include "access_internal.hxx"

void acis_save_entity_list(const ENTITY_LIST &elist, const char *file_name, int major_version, int minor_version,
                           int text_mode) {
    API_NOP_BEGIN;
    api_save_version(major_version, minor_version);
    FileInfo fileinfo;
    fileinfo.set_units(1.0);
    fileinfo.set_product_id("SimpleApi");
    result = api_set_file_info((FileIdent | FileUnits), fileinfo);
    result = api_set_int_option("sequence_save_files", 1);
    FILE *save_file;
    fopen_s(&save_file, file_name, "wb");
    if (!save_file) {
        myerror("打开文件失败，文件名为" + std::string(file_name));
    } else {
        result = api_save_entity_list(save_file, text_mode, elist);
        fclose(save_file);
    }
    API_NOP_END;
}

void acis_save_entity_list(FILE *file, const ENTITY_LIST &elist, int major_version, int minor_version, int text_mode) {
    API_NOP_BEGIN;
    api_save_version(major_version, minor_version);
    FileInfo fileinfo;
    fileinfo.set_units(1.0);
    fileinfo.set_product_id("SimpleApi");
    result = api_set_file_info((FileIdent | FileUnits), fileinfo);
    result = api_set_int_option("sequence_save_files", 1);
    if (!file) {
        myerror("打开文件失败，文件句柄为空");
    } else {
        result = api_save_entity_list(file, text_mode, elist);
        fflush(file);
    }
    API_NOP_END;
}

void acis_get_noattrib_toplevel_active_entities(ENTITY_LIST &elist, HISTORY_STREAM *hs) {
    if (hs == NULL) {
        api_get_default_history(hs);
    }
    api_get_active_entities(hs, elist, 1); // 参数1表示只获取顶级实体

    // 去除（不保存）annotation、attrib等类型实体
    elist.init();
    ENTITY *tmpentity = nullptr;
    while (tmpentity = elist.next()) {
        if (is_ANNOTATION(tmpentity) || is_ATTRIB_TAG(tmpentity)) {
            elist.remove(tmpentity);
        }
    }
}

void acis_save_noattrib_toplevel_active_entities(const char *file_name, int major_version, int minor_version,
                                                 int text_mode, HISTORY_STREAM *hs) {
    ENTITY_LIST elist;
    acis_get_noattrib_toplevel_active_entities(elist, hs);
    acis_save_entity_list(elist, file_name, major_version, minor_version, text_mode);
}

void acis_save_history(const char *file_name, int major_version, int minor_version, int text_mode, HISTORY_STREAM *hs) {
    API_NOP_BEGIN;
    api_save_version(major_version, minor_version);
    FileInfo fileinfo;
    fileinfo.set_units(1.0);
    fileinfo.set_product_id("SimpleApi");
    result = api_set_file_info((FileIdent | FileUnits), fileinfo);
    result = api_set_int_option("sequence_save_files", 1);
    FILE *save_file;
    fopen_s(&save_file, file_name, "wb");
    if (!save_file) {
        myerror("打开文件失败，文件名为" + std::string(file_name));
    } else {
        result = api_save_history(save_file, text_mode, hs);
        fclose(save_file);
    }
    API_NOP_END;
}

void acis_restore_entity_list(ENTITY_LIST &elist, const char *file_name, int major_version, int minor_version,
                              int text_mode) {
    API_BEGIN;
    api_save_version(major_version, minor_version);
    FILE *save_file;
    fopen_s(&save_file, file_name, "rb");
    if (!save_file) {
        myerror("打开文件失败，文件名为" + std::string(file_name));
    } else {
        result = api_restore_entity_list(save_file, text_mode, elist);
        fclose(save_file);
    }
    API_END;
}

void acis_restore_entity_list(ENTITY_LIST &elist, FILE *file, int major_version, int minor_version, int text_mode) {
    API_BEGIN;
    api_save_version(major_version, minor_version);
    if (!file) {
        myerror("打开文件失败，文件句柄为空");
    } else {
        result = api_restore_entity_list(file, text_mode, elist);
    }
    API_END;
}

std::string AccessTest::read_file_to_string(std::string filename) {
    std::ifstream t(filename);
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);
    return buffer;
}

std::tuple<bool, double, double, double, double> AccessTest::CheckTestCase(const Neo4jPart &conn,
                                                                           std::string testcase_name,
                                                                           const ENTITY_LIST &el) {
    TMDF;

    std::string acis_save_filename_str = std::format("acis_{}_save.sat", testcase_name);
    const char *acis_save_filename = acis_save_filename_str.c_str();

    // ACIS接口保存
    TMST;
    acis_save_entity_list(el, acis_save_filename, 2, 0, true);
    TMED;
    double acis_save_duration = TMDR;

    // ACIS接口恢复
    ENTITY_LIST el_restore_acis;
    TMST;
    acis_restore_entity_list(el_restore_acis, acis_save_filename, 2, 0, true);
    TMED;
    double acis_restore_duration = TMDR;

    // 删除ACIS接口保存的文件
    // remove(acis_save_filename);

    // neo4j接口保存
    std::unordered_map<void *, int64_t> ptr2elemid;
    TMST;
    api_save_entity_list_neo4j(conn, el, ptr2elemid);
    TMED;
    double neo4j_save_duration = TMDR;

    // neo4j接口恢复
    ENTITY_LIST el_restore_neo4j;
    std::vector<int64_t> elemid_list;
    for (ENTITY *e

         : el) {
        elemid_list.push_back(ptr2elemid.at(e));
    }
    std::unordered_map<int64_t, void *> elemid2ptr;
    TMST;
    api_restore_entity_list_neo4j(conn, elemid_list, el_restore_neo4j, elemid2ptr);
    TMED;
    double neo4j_restore_duration = TMDR;
    // 删除neo4j接口保存的节点和边
    // for (const int64_t id : elemid_list_subgraph) {
    //     conn.query("MATCH (n) WHERE id(n) = $d "
    //         "CALL apoc.path.subgraphAll(n, {minLevel:0}) YIELD nodes FOREACH(n IN nodes | DETACH DELETE n) ",
    //         nlohmann::json{
    //                                                                                                    {"d", id},
    //         });
    // }
    conn.execute_bolt("match(n) call { with n detach delete n } in transactions of 10000 rows", NULL);
    conn.discard_all_results();

    // 对el_restore_neo4j和el_restore_acis判等
    std::string acis_check_filename_str = std::format("acis_{}_check.sat", testcase_name);
    std::string neo4j_check_filename_str = std::format("neo4j_{}_check.sat", testcase_name);
    acis_save_entity_list(el_restore_acis, acis_check_filename_str.c_str(), 2, 0, true);
    acis_save_entity_list(el_restore_neo4j, neo4j_check_filename_str.c_str(), 2, 0, true);
    // 比较时略过SAT文件头
    int newline_1_pos, newline_2_pos;
    std::string acis_check_file_str = read_file_to_string(acis_check_filename_str);
    newline_1_pos = acis_check_file_str.find('\n');
    newline_2_pos = acis_check_file_str.find('\n', newline_1_pos + 1);
    std::string acis_check_file_str_remove_header = acis_check_file_str.substr(newline_2_pos + 1);
    std::string neo4j_check_file_str = read_file_to_string(neo4j_check_filename_str);
    newline_1_pos = neo4j_check_file_str.find('\n');
    newline_2_pos = neo4j_check_file_str.find('\n', newline_1_pos + 1);
    std::string neo4j_check_file_str_remove_header = neo4j_check_file_str.substr(newline_2_pos + 1);
    bool testresult = acis_check_file_str_remove_header == neo4j_check_file_str_remove_header;

    // 删除ACIS接口保存的文件
    // remove(acis_check_filename_str.c_str());
    // remove(neo4j_subgraph_check_filename_str.c_str());

    return std::make_tuple(testresult, neo4j_save_duration, acis_save_duration, neo4j_restore_duration,
                           acis_restore_duration);
}
