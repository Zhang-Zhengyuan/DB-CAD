#include "access_internal.hxx"

static void parseDeltaJsonArray(const std::string &json, const std::string &key, std::vector<std::string> &out) {
    out.clear();
    std::string keyPattern = "\"" + key + "\":";
    size_t keyPos = json.find(keyPattern);
    if (keyPos == std::string::npos)
        return;
    size_t arrayStart = json.find('[', keyPos);
    if (arrayStart == std::string::npos)
        return;
    size_t arrayEnd = json.find(']', arrayStart);
    if (arrayEnd == std::string::npos)
        return;

    size_t pos = arrayStart + 1;
    while (pos < arrayEnd) {
        // 跳过空白
        while (pos < arrayEnd &&
               (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == '\r' || json[pos] == ',')) {
            ++pos;
        }
        if (pos >= arrayEnd)
            break;
        if (json[pos] != '"') {
            // 数组里有非字符串元素（理论上不会），跳过到下一个逗号
            size_t next = json.find(',', pos);
            if (next == std::string::npos || next > arrayEnd)
                break;
            pos = next + 1;
            continue;
        }
        ++pos; // skip opening "
        std::string item;
        while (pos < arrayEnd && json[pos] != '"') {
            if (json[pos] == '\\' && pos + 1 < arrayEnd) {
                char esc = json[pos + 1];
                if (esc == 'n')
                    item += '\n';
                else if (esc == 'r')
                    item += '\r';
                else if (esc == 't')
                    item += '\t';
                else
                    item += esc; // \\ \" \/ \b \f 等直接保留
                pos += 2;
            } else {
                item += json[pos];
                ++pos;
            }
        }
        if (pos < arrayEnd)
            ++pos; // skip closing "
        out.push_back(std::move(item));
    }
}

static void parseDeltaContentJson(const std::string &json, std::vector<std::string> &out_delta_uuids,
                                  std::vector<std::string> &out_delta_sat_segments,
                                  std::vector<std::string> &out_removed_uuids) {
    parseDeltaJsonArray(json, "delta_uuids", out_delta_uuids);
    parseDeltaJsonArray(json, "delta_sat_segments", out_delta_sat_segments);
    parseDeltaJsonArray(json, "removed_uuids", out_removed_uuids);
}

bool handle_get_delta_from_neo4j(const Neo4jPart &conn, int base_version, int &out_latest_version,
                                 std::vector<std::string> &out_body_uuids,
                                 std::vector<std::string> &out_sat_segments_by_uuid,
                                 std::vector<std::string> &out_deleted_uuids, std::string &out_error) {
    out_error.clear();
    out_latest_version = 0;
    out_body_uuids.clear();
    out_sat_segments_by_uuid.clear();
    out_deleted_uuids.clear();

    DIAG_LOG("[get] === handleGetDelta ENTER === base_version=%d\n", base_version);
    DIAG_LOG("[get] partname=%s\n", conn.partname.c_str());

    // 1. 读取最新版本号
    out_latest_version = read_part_node_version(conn, conn.partname);

    if (out_latest_version == 0) {
        // 没有数据
        return true;
    }

    // 2. 检查 base_version
    if (base_version >= out_latest_version) {
        // B 端已是最新，无需 delta
        return true;
    }

    // ================================================================================================
    // 增量模式：基于 BridgeDeltaVersion 回放历史
    // ================================================================================================
    // 从 partname ("delta__{projectId}") 提取 projectId
    std::string projectIdStr;
    {
        const std::string prefix = "delta__";
        if (conn.partname.size() > prefix.size() && conn.partname.compare(0, prefix.size(), prefix) == 0) {
            projectIdStr = conn.partname.substr(prefix.size());
        } else {
            projectIdStr = conn.partname;
        }
    }

    // 查询 BridgeDeltaVersion：version ∈ [base_version+1, latest_version]
    struct DeltaVersionRecord {
        int version = 0;
        std::vector<std::string> delta_uuids;
        std::vector<std::string> delta_sat_segments;
        std::vector<std::string> removed_uuids;
    };

    std::map<int, DeltaVersionRecord> versionRecords; // 按 version 升序
    {
        mg_map *qparams = mg_map_make_empty(3);
        mg_map_append(qparams, mg_string_make("project_id"), mg_value_make_string(projectIdStr.c_str()));
        mg_map_append(qparams, mg_string_make("min_version"), mg_value_make_integer(base_version + 1));
        mg_map_append(qparams, mg_string_make("max_version"), mg_value_make_integer(out_latest_version));
        conn.execute_bolt(
            "MATCH (v:BridgeDeltaVersion) "
            "WHERE v.project_id = $project_id AND v.version >= $min_version AND v.version <= $max_version "
            "RETURN v.version, v.content_text ORDER BY v.version ASC",
            qparams);
        mg_map_destroy(qparams);

        mg_result *qresult = nullptr;
        while (mg_session_fetch(conn.session, &qresult) == 1) {
            const mg_list *row = mg_result_row(qresult);
            if (row == nullptr || mg_list_size(row) < 2)
                continue;

            DeltaVersionRecord rec;
            const mg_value *verV = mg_list_at(row, 0);
            if (verV != nullptr && mg_value_get_type(verV) == MG_VALUE_TYPE_INTEGER) {
                rec.version = static_cast<int>(mg_value_integer(verV));
            } else {
                continue;
            }

            // content_text 是一个 JSON 字符串：{ "delta_uuids":[...], "delta_sat_segments":[...], "removed_uuids":[...]
            // }
            const mg_value *ctV = mg_list_at(row, 1);
            if (ctV != nullptr && mg_value_get_type(ctV) == MG_VALUE_TYPE_STRING) {
                const mg_string *s = mg_value_string(ctV);
                if (s != nullptr) {
                    std::string jsonStr(mg_string_data(s), mg_string_size(s));
                    parseDeltaContentJson(jsonStr, rec.delta_uuids, rec.delta_sat_segments, rec.removed_uuids);
                }
            }

            versionRecords[rec.version] = std::move(rec);
        }
        conn.discard_all_results();
    }

    // 检查历史完整性：版本号必须连续 [base_version+1, latest_version]
    bool historyComplete = (static_cast<int>(versionRecords.size()) == (out_latest_version - base_version));
    if (historyComplete) {
        int expected = base_version + 1;
        for (const auto &kv : versionRecords) {
            if (kv.first != expected) {
                historyComplete = false;
                break;
            }
            ++expected;
        }
    }

    if (historyComplete) {
        // ========================================================================================
        // 增量回放：last-write-wins
        //   added[uuid]   = 最新的 SAT 段（同一 uuid 在多个版本中被 add/update 时取最后一个）
        //   deleted_set   = 被删除的 uuid 集合（如果后续又被 add 则移除）
        // ========================================================================================
        std::map<std::string, std::string> added;
        std::set<std::string> deleted;

        for (const auto &kv : versionRecords) {
            const DeltaVersionRecord &rec = kv.second;

            for (size_t i = 0; i < rec.delta_uuids.size(); ++i) {
                const std::string &uuid = rec.delta_uuids[i];
                std::string sat;
                if (i < rec.delta_sat_segments.size()) {
                    sat = rec.delta_sat_segments[i];
                }
                added[uuid] = sat;   // last-write-wins
                deleted.erase(uuid); // 重新被添加，不再算删除
            }

            for (const std::string &uuid : rec.removed_uuids) {
                added.erase(uuid);    // 后续又被删除，从 added map 移除
                deleted.insert(uuid); // 加入 deleted set
            }
        }

        // 输出 added map
        for (const auto &kv : added) {
            out_body_uuids.push_back(kv.first);
            out_sat_segments_by_uuid.push_back(kv.second);
        }

        // 输出 deleted set
        for (const std::string &uuid : deleted) {
            out_deleted_uuids.push_back(uuid);
        }

        std::fprintf(stderr, "[access handleGetDelta] INCREMENTAL: latest=%d base=%d added=%zu deleted=%zu\n",
                     out_latest_version, base_version, out_body_uuids.size(), out_deleted_uuids.size());
        for (size_t i = 0; i < out_body_uuids.size(); ++i) {
            std::fprintf(stderr, "  + added uuid=%s sat_len=%zu\n", out_body_uuids[i].c_str(),
                         out_sat_segments_by_uuid[i].size());
        }
        for (const std::string &u : out_deleted_uuids) {
            std::fprintf(stderr, "  - deleted uuid=%s\n", u.c_str());
        }
        return true;
    }

    // 历史不完整 → 全量回退
    std::fprintf(stderr,
                 "[access handleGetDelta] WARN: BridgeDeltaVersion history incomplete "
                 "(found %zu/%d in [%d, %d]) — falling back to full-state return\n",
                 versionRecords.size(), out_latest_version - base_version, base_version + 1, out_latest_version);

    // 3. 从 Neo4j restore 完整 ENTITY_LIST（仅在历史缺失时执行）
    ENTITY_LIST full_entity_list;
    api_restore_entity_list_neo4j_part(conn, full_entity_list);

    DIAG_LOG("[get-delta] latest_version=%d base_version=%d entity_list.count=%d\n", out_latest_version, base_version,
             (int)full_entity_list.count());
    {
        int body_count = 0, lump_count = 0, shell_count = 0, face_count = 0;
        int body_with_lump = 0;
        for (ENTITY *e : full_entity_list) {
            if (is_BODY(e)) {
                BODY *b = (BODY *)e;
                LUMP *L = b->lump();
                if (L != nullptr)
                    ++body_with_lump;
                ++body_count;
            } else if (is_LUMP(e)) {
                LUMP *L = (LUMP *)e;
                int ns = 0;
                for (SHELL *s = L->shell(); s; s = s->next()) {
                    ++ns;
                    for (FACE *f = s->face(); f; f = f->next())
                        ++face_count;
                }
                ++lump_count;
                shell_count += ns;
            }
        }
        DIAG_LOG("[get-delta] topology: bodies=%d (with_lump=%d) lumps=%d shells=%d faces=%d\n", body_count,
                 body_with_lump, lump_count, shell_count, face_count);
    }

    // 4. 遍历每个 body，序列化 SAT 并提取 uuid
    for (ENTITY *e : full_entity_list) {
        if (!is_BODY(e))
            continue;

        // 读取 uuid（从 body 节点的 uuid 属性）
        std::string uuid = get_body_node_uuid(conn, 0); // 需要通过 ptr2nodeid 映射
        // 由于 restore 后 ptr2nodeid 不在函数参数中，我们用另一种方式：
        // 从 Neo4j 查询 body 节点及其 uuid 属性

        // 为简化：先通过 serializeBodyToSatWithUuid 序列化，
        // 但 access.cpp 中没有这个函数，我们需要：
        //   a) 写 SAT 到临时文件
        //   b) 读回来作为字符串
        // 同时查询 uuid

        // 方案：对 full_entity_list 中的每个 body，单独序列化 SAT
        // 并通过单独查询获取 uuid
    }

    // 更简洁的方案：直接用 api_save_entity_list_neo4j_part 内部的方式，
    // 但我们需要 body uuid。让我通过查询 body 节点获取 uuid。

    // 查询所有 body 节点及其 uuid 属性
    mg_map *qparams = mg_map_make_empty(1);
    mg_map_append(qparams, mg_string_make("Y"), mg_value_make_string(conn.partname.c_str()));
    conn.execute_bolt("MATCH (n:part {b:$Y})-[r:part_entity_ptr]->(b:body) "
                      "RETURN id(b), b.uuid ORDER BY r.b",
                      qparams);
    mg_map_destroy(qparams);

    mg_result *result = nullptr;
    std::vector<int64_t> body_node_ids;
    while (mg_session_fetch(conn.session, &result) == 1) {
        const mg_list *row = mg_result_row(result);
        if (row != nullptr && mg_list_size(row) >= 2) {
            int64_t body_id = mg_value_integer(mg_list_at(row, 0));
            const mg_value *uuidVal = mg_list_at(row, 1);
            std::string uuidStr;
            if (uuidVal != nullptr && mg_value_get_type(uuidVal) == MG_VALUE_TYPE_STRING) {
                const mg_string *s = mg_value_string(uuidVal);
                if (s != nullptr) {
                    uuidStr = std::string(mg_string_data(s), mg_string_size(s));
                }
            }
            body_node_ids.push_back(body_id);
            out_body_uuids.push_back(uuidStr);
        }
    }
    // 注意：while 循环结束后，查询结果已全部读取，无需再调用 mg_session_fetch
    // 直接跳过 discard_all_results，避免 session 状态异常导致的 crash
    // conn.discard_all_results();  // 已注释：可能触发 "called fetch while not executing a query" 错误

    // 现在 body_node_ids 和 out_body_uuids 已经对齐。
    // 下一步：为每个 body 序列化 SAT 文本。
    // 由于 body 已经 restore 到了 full_entity_list，我们可以直接遍历。
    // 但 full_entity_list 是 ENTITY_LIST，我们需要将 body pointer 和 node id 对应起来。

    // 简化方案：通过 full_entity_list 遍历并序列化。
    // 关键：我们需要在序列化时知道 uuid。
    // 在当前设计中，body uuid 存储在 Neo4j 节点的 uuid 属性中，
    // 而 restore 后 ptr2nodeid 映射丢失了（因为是内存指针）。
    // 所以我们必须通过 Neo4j 查询获取 uuid，然后用 entity_list index 对应。

    // entity_list 的顺序和 part_entity_ptr 的 idx 一致。
    // 通过遍历 entity_list 并与 body_node_ids 匹配来建立映射。

    std::unordered_map<int64_t, std::string> nodeid_to_uuid;
    for (size_t i = 0; i < body_node_ids.size() && i < out_body_uuids.size(); ++i) {
        nodeid_to_uuid[body_node_ids[i]] = out_body_uuids[i];
    }

    // 建立 entity ptr → uuid 的映射（通过 ptr2nodeid 不可用，改为通过 SAT 序列化+Neo4j 查询）
    // 由于 restore 后的 body pointer 和 Neo4j node id 没有直接映射关系，
    // 我们采用另一种方式：直接用 body_node_ids 顺序对应 entity_list 中 body 的顺序。

    // 获取 entity_list 中的所有 body
    std::vector<ENTITY *> body_ptrs;
    for (ENTITY *e : full_entity_list) {
        if (is_BODY(e)) {
            body_ptrs.push_back(e);
        }
    }

    // body_node_ids 的顺序应该和 entity_list 中 body 的顺序一致（都按 part_entity_ptr 的 idx 排序）
    // 因为 api_restore_entity_list_neo4j_part 返回的 entity_list 就是按此顺序的。

    // 现在为每个 body 序列化 SAT
    for (size_t i = 0; i < body_ptrs.size() && i < out_body_uuids.size(); ++i) {
        ENTITY *body = body_ptrs[i];
        const std::string &uuid = out_body_uuids[i];

        // 序列化单个 body
        ENTITY_LIST single;
        single.add(body);

        FILE *satFile = nullptr;
        // 用 mode1_temp::make_delta_temp_path 替代已弃用的 tmpnam_s
        std::string tmpPath = mode1_temp::make_delta_temp_path("dbcad_delta_get");

        FILE *f = fopen(tmpPath.c_str(), "wb");
        if (f == nullptr)
            continue;

        // 设置 ACIS 版本信息
        api_save_version(2, 0);
        FileInfo fi;
        fi.set_units(1.0);
        fi.set_product_id("dbcad_mode1");
        api_set_file_info((FileIdent | FileUnits), fi);

        // 关键 1：开 sequence_save_files 让 ACIS 写 SAT schema header（schema-version / product-id），
        // 否则 pull 端 acis_restore_entity_list 读不到合法 header 会静默返回"无拓扑的 body"，
        // 表面看起来 restore 成功，但 CreateMeshFromEntity 会因为 numFaces=0/numEdges=0 FAILED。
        // 关键 2：text_mode 必须用 1（true / 文本），不能用 2；2 跟 ACIS 标准不一致。
        // 关键 3：所有直接 api_* 调用必须包 API_NOP_BEGIN/END（参考技术路线第 6.1 节）。
        outcome save_result;
        try {
            API_NOP_BEGIN;
            api_set_int_option("sequence_save_files", 1);
            save_result = api_save_entity_list(f, true, single);
            API_NOP_END;
        } catch (const std::exception &ex) {
            std::fprintf(stderr, "[access handleGetDelta] api_save_entity_list threw for uuid=%s: %s\n", uuid.c_str(),
                         ex.what());
            fclose(f);
            remove(tmpPath.c_str());
            continue;
        }

        if (!save_result.ok()) {
            fclose(f);
            remove(tmpPath.c_str());
            continue;
        }
        fclose(f);

        // 读回作为字符串
        FILE *readF = fopen(tmpPath.c_str(), "rb");
        std::string satContent;
        if (readF != nullptr) {
            char buf[8192];
            size_t n;
            while ((n = fread(buf, 1, sizeof(buf), readF)) > 0) {
                satContent.append(buf, n);
            }
            fclose(readF);
        }
        remove(tmpPath.c_str());

        out_sat_segments_by_uuid.push_back(satContent);
        std::fprintf(stderr, "[access handleGetDelta] body uuid=%s sat_len=%d\n", uuid.c_str(), (int)satContent.size());
    }

    return true;
}
