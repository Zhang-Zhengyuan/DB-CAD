#include "access_internal.hxx"

int read_part_node_version(const Neo4jPart &conn, const std::string &partname) {
    std::fprintf(stderr, "[read_part_node_version] ENTER partname=%s\n", partname.c_str());
    fflush(stderr);

    mg_map *qparams = mg_map_make_empty(1);
    mg_map_append(qparams, mg_string_make("Y"), mg_value_make_string(partname.c_str()));

    std::fprintf(stderr, "[read_part_node_version] executing bolt query\n");
    fflush(stderr);
    conn.execute_bolt("MATCH (n:part {b:$Y}) RETURN n.version", qparams);

    int version = 0;
    mg_result *result = nullptr;
    std::fprintf(stderr, "[read_part_node_version] fetching result\n");
    fflush(stderr);

    if (mg_session_fetch(conn.session, &result) == 1) {
        const mg_list *row = mg_result_row(result);
        if (row != nullptr && mg_list_size(row) >= 1) {
            const mg_value *v = mg_list_at(row, 0);
            if (v != nullptr && mg_value_get_type(v) == MG_VALUE_TYPE_INTEGER) {
                version = static_cast<int>(mg_value_integer(v));
            }
        }
    }

    std::fprintf(stderr, "[read_part_node_version] got version=%d, destroying qparams\n", version);
    fflush(stderr);
    mg_map_destroy(qparams);

    std::fprintf(stderr, "[read_part_node_version] calling discard_all_results\n");
    fflush(stderr);
    conn.discard_all_results();

    std::fprintf(stderr, "[read_part_node_version] EXIT returning %d\n", version);
    fflush(stderr);
    return version;
}

// 更新 part 节点的 version 字段（用于 save_delta 写回）
void update_part_node_version(const Neo4jPart &conn, const std::string &partname, int version,
                              const std::string &author, const std::string &timestamp) {
    mg_map *qparams = mg_map_make_empty(5);
    mg_map_append(qparams, mg_string_make("Y"), mg_value_make_string(partname.c_str()));
    mg_map_append(qparams, mg_string_make("V"), mg_value_make_integer(version));
    mg_map_append(qparams, mg_string_make("A"), mg_value_make_string(author.c_str()));
    mg_map_append(qparams, mg_string_make("T"), mg_value_make_string(timestamp.c_str()));
    conn.execute_bolt("MATCH (n:part {b:$Y}) SET n.version=$V, n.author=$A, n.updated_at=$T", qparams);
    mg_map_destroy(qparams);
    conn.discard_all_results();
}

// 检查 part 节点是否已存在
bool part_node_exists(const Neo4jPart &conn, const std::string &partname) {
    return count_partnode(conn) > 0;
}

// 获取 body 节点的 uuid 属性（用于 pull 时构建 sat_segments_by_uuid）
std::string get_body_node_uuid(const Neo4jPart &conn, int64_t body_node_id) {
    mg_map *qparams = mg_map_make_empty(1);
    mg_map_append(qparams, mg_string_make("N"), mg_value_make_integer(body_node_id));
    conn.execute_bolt("MATCH (n) WHERE id(n)=$N RETURN n.uuid", qparams);

    std::string uuid;
    mg_result *result = nullptr;
    if (mg_session_fetch(conn.session, &result) == 1) {
        const mg_list *row = mg_result_row(result);
        if (row != nullptr && mg_list_size(row) >= 1) {
            const mg_value *v = mg_list_at(row, 0);
            if (v != nullptr && mg_value_get_type(v) == MG_VALUE_TYPE_STRING) {
                const mg_string *s = mg_value_string(v);
                if (s != nullptr) {
                    uuid = std::string(mg_string_data(s), mg_string_size(s));
                }
            }
        }
    }
    if (mg_session_fetch(conn.session, &result) != 0) {
        myerror("get_body_node_uuid: mg_session_fetch failed");
    }
    mg_map_destroy(qparams);
    conn.discard_all_results();
    return uuid;
}

// ------------------------------------------------------------------------------------------------
// handleSaveDelta 核心逻辑（C++端，供 storage_bridge 调用）
//
// delta_uuids / delta_sat_segments: 本次新增/修改的 body uuid + 独立 SAT 文本
// removed_uuids: 本次删除的 body uuid
// base_version: A 端上次 push 的版本号
//
// 流程：
//   1. 从 Neo4j 读取 base_version 的完整 entity_list
//   2. 对 delta_sat_segments 逐段做 acis_restore_entity_list，合并到内存
//   3. 按 removed_uuids 调用 api_del_entity 删除
//   4. 全量覆盖写回 Neo4j（api_save_entity_list_neo4j_part）
//   5. 更新 part.version = base_version + 1
// ------------------------------------------------------------------------------------------------
bool handle_save_delta_to_neo4j(const Neo4jPart &conn, int base_version, const std::vector<std::string> &delta_uuids,
                                const std::vector<std::string> &delta_sat_segments,
                                const std::vector<std::string> &removed_uuids, const std::string &author,
                                const std::string &timestamp, int &out_new_version, std::string &out_error) {
    out_error.clear();
    out_new_version = 0;

    diaglog::clear_log();
    DIAG_LOG("[save] === handleSaveDelta ENTER === delta_uuids=%zu segments=%zu removed=%zu base_version=%d\n",
             delta_uuids.size(), delta_sat_segments.size(), removed_uuids.size(), base_version);
    DIAG_LOG("[save] partname=%s\n", conn.partname.c_str());

    std::fprintf(stderr, "[access handleSaveDelta] STEP 1: reading part_node_version\n");
    fflush(stderr);

    // 1. 检查 base_version 是否匹配
    int current_version = read_part_node_version(conn, conn.partname);
    std::fprintf(stderr, "[access handleSaveDelta] STEP 2: part_node_version=%d\n", current_version);
    fflush(stderr);

    if (base_version >= 0 && base_version != current_version) {
        out_error = std::format("Version conflict: expected {}, current {}", base_version, current_version);
        return false;
    }

    // 2. 读取 base_version 时的完整 ENTITY_LIST
    ENTITY_LIST full_entity_list;
    if (current_version > 0) {
        // part 子图存在，restore
        std::fprintf(stderr, "[access handleSaveDelta] STEP 3: restoring entity_list (current_version=%d)\n",
                     current_version);
        fflush(stderr);
        api_restore_entity_list_neo4j_part(conn, full_entity_list);
        std::fprintf(stderr, "[access handleSaveDelta] STEP 4: restore done, entity count=%d\n",
                     (int)full_entity_list.count());
        fflush(stderr);
    } else {
        // 首次 push，part 子图不存在，entity_list 保持为空
        std::fprintf(stderr, "[access handleSaveDelta] STEP 3: first push, empty entity_list\n");
        fflush(stderr);
    }

    std::fprintf(stderr, "[access handleSaveDelta] STEP 4/5: base_version=%d current_version=%d entity_list count=%d\n",
                 base_version, current_version, (int)full_entity_list.count());
    fflush(stderr);

    // 3. 建立 body pointer → uuid 的映射
    std::unordered_map<void *, std::string> body_ptr_to_uuid;
    for (ENTITY *e : full_entity_list) {
        if (is_BODY(e)) {
            body_ptr_to_uuid[(void *)e] = ""; // 暂存，后续填充
        }
    }

    // 【Bug A 修复】restore 后 ptr → uuid 映射丢失（Neo4j body 节点的 uuid 属性
    // 是单独存的，restore entity_list 时不会带回）。如果不在这里把已有 uuid
    // 重新读回内存，后续 push 时旧的 body 会被标记为 uuid=""，STEP 6.5 写回时
    // 又会跳过空 uuid，导致服务端 part 里 cube 的 uuid 永久丢失。
    // 后果：B 端再 push 时，A 端 pull 会看到 uuid 为空的 body，A 端按 uuid 做
    // set-difference 合并就会把 cube 当成"远端新增"，本地 cube 被错误删除。
    if (current_version > 0 && full_entity_list.count() > 0) {
        mg_map *restoreUuidParams = mg_map_make_empty(1);
        mg_map_append(restoreUuidParams, mg_string_make("Y"), mg_value_make_string(conn.partname.c_str()));
        conn.execute_bolt("MATCH (n:part {b:$Y})-[r:part_entity_ptr]->(b:body) "
                          "RETURN id(b), b.uuid ORDER BY toInteger(r.b)",
                          restoreUuidParams);
        mg_map_destroy(restoreUuidParams);

        std::vector<std::string> existingUuidsInOrder;
        mg_result *rresult = nullptr;
        while (mg_session_fetch(conn.session, &rresult) == 1) {
            const mg_list *rrow = mg_result_row(rresult);
            if (rrow == nullptr || mg_list_size(rrow) < 2)
                continue;
            const mg_value *uuidV = mg_list_at(rrow, 1);
            std::string existingUuid;
            if (uuidV != nullptr && mg_value_get_type(uuidV) == MG_VALUE_TYPE_STRING) {
                const mg_string *s = mg_value_string(uuidV);
                if (s != nullptr)
                    existingUuid = std::string(mg_string_data(s), mg_string_size(s));
            }
            existingUuidsInOrder.push_back(existingUuid);
        }
        conn.discard_all_results();

        // 按 entity_list 中 body 顺序，与 Neo4j 返回的 uuid 一一对应
        std::vector<ENTITY *> bodyPtrsInOrder;
        for (ENTITY *e : full_entity_list) {
            if (is_BODY(e))
                bodyPtrsInOrder.push_back(e);
        }
        if (bodyPtrsInOrder.size() == existingUuidsInOrder.size()) {
            size_t restored = 0;
            for (size_t i = 0; i < bodyPtrsInOrder.size(); ++i) {
                if (!existingUuidsInOrder[i].empty()) {
                    body_ptr_to_uuid[(void *)bodyPtrsInOrder[i]] = existingUuidsInOrder[i];
                    ++restored;
                }
            }
            std::fprintf(stderr,
                         "[access handleSaveDelta] restored %zu/%zu body uuids from Neo4j (avoiding uuid loss)\n",
                         restored, bodyPtrsInOrder.size());
        } else {
            std::fprintf(stderr,
                         "[access handleSaveDelta] WARN: body count mismatch on uuid restore "
                         "(entity_list=%zu, neo4j=%zu), uuid may be lost\n",
                         bodyPtrsInOrder.size(), existingUuidsInOrder.size());
        }
    }

    // 4. 对 delta_sat_segments 逐段做 acis_restore_entity_list，加入内存
    if (delta_uuids.size() != delta_sat_segments.size()) {
        out_error = "delta_uuids and delta_sat_segments size mismatch";
        return false;
    }

    std::fprintf(stderr, "[access handleSaveDelta] STEP 6: processing %zu delta segments\n", delta_uuids.size());
    fflush(stderr);

    for (size_t i = 0; i < delta_uuids.size(); ++i) {
        const std::string &uuid = delta_uuids[i];
        const std::string &sat_segment = delta_sat_segments[i];

        if (sat_segment.empty()) {
            std::fprintf(stderr, "[access handleSaveDelta] skip empty sat for uuid=%s\n", uuid.c_str());
            continue;
        }

        // 写 SAT 到临时文件（用 mode1_temp::make_delta_temp_path 替代已弃用的 tmpnam_s）
        FILE *satFile = nullptr;
        std::string tmpPath = mode1_temp::make_delta_temp_path("dbcad_delta_save");
        FILE *f = fopen(tmpPath.c_str(), "wb");
        if (f == nullptr) {
            out_error = "Failed to open temp SAT file for writing: " + tmpPath;
            return false;
        }

        size_t written = fwrite(sat_segment.c_str(), 1, sat_segment.size(), f);
        fflush(f);
        fclose(f);

        if (written != sat_segment.size()) {
            remove(tmpPath.c_str());
            out_error = "Failed to write full SAT segment";
            return false;
        }

        std::fprintf(stderr, "[access handleSaveDelta] STEP 7[%zu]: restoring SAT uuid=%s tmpPath=%s\n", i,
                     uuid.c_str(), tmpPath.c_str());
        fflush(stderr);

        // ACIS restore
        FILE *readF = fopen(tmpPath.c_str(), "rb");
        if (readF == nullptr) {
            remove(tmpPath.c_str());
            out_error = "Failed to open temp SAT file for reading: " + tmpPath;
            return false;
        }

        ENTITY_LIST restored;
        try {
            acis_restore_entity_list(restored, readF, 2, 0, true);
        } catch (const std::exception &ex) {
            fclose(readF);
            remove(tmpPath.c_str());
            out_error = std::string("acis_restore_entity_list failed: ") + ex.what();
            return false;
        }
        fclose(readF);
        remove(tmpPath.c_str());

        std::fprintf(stderr, "[access handleSaveDelta] STEP 8[%zu]: restored %d entities for uuid=%s\n", i,
                     (int)restored.count(), uuid.c_str());
        fflush(stderr);

        if (restored.count() == 0) {
            std::fprintf(stderr, "[access handleSaveDelta] restored 0 entities for uuid=%s\n", uuid.c_str());
            continue;
        }

        // 取第一个 body，加入 full_entity_list
        ENTITY *restoredBody = restored[0];
        if (is_BODY(restoredBody)) {
            // MODIFY 场景：先按 uuid 找到并删除 full_entity_list 里已有的同 uuid 旧 body，
            // 避免 entity_list 里出现两个 BODY（Neo4j 会建两个 body 节点，造成重复）。
            ENTITY *oldBodyToRemove = nullptr;
            for (ENTITY *e : full_entity_list) {
                if (is_BODY(e) && body_ptr_to_uuid.find((void *)e) != body_ptr_to_uuid.end() &&
                    body_ptr_to_uuid[(void *)e] == uuid) {
                    oldBodyToRemove = e;
                    break;
                }
            }
            if (oldBodyToRemove != nullptr) {
                try {
                    API_NOP_BEGIN;
                    api_del_entity(oldBodyToRemove);
                    API_NOP_END;
                } catch (const std::exception &ex) {
                    std::fprintf(stderr, "[access handleSaveDelta] api_del_entity old body uuid=%s: %s\n", uuid.c_str(),
                                 ex.what());
                }
                body_ptr_to_uuid.erase((void *)oldBodyToRemove);
            }
            full_entity_list.add(restoredBody);
            // 注册 uuid
            body_ptr_to_uuid[(void *)restoredBody] = uuid;
            std::fprintf(stderr, "[access handleSaveDelta] added body uuid=%s to entity_list (replaced=%d)\n",
                         uuid.c_str(), oldBodyToRemove != nullptr ? 1 : 0);
        } else {
            std::fprintf(stderr, "[access handleSaveDelta] restored entity is not BODY, uuid=%s\n", uuid.c_str());
        }
    }

    std::fprintf(stderr, "[access handleSaveDelta] STEP 9: processing %zu removed_uuids\n", removed_uuids.size());
    fflush(stderr);

    // 5. 按 removed_uuids 删除实体
    for (const std::string &uuid : removed_uuids) {
        bool removed = false;
        for (ENTITY *e : full_entity_list) {
            if (is_BODY(e)) {
                auto it = body_ptr_to_uuid.find((void *)e);
                if (it != body_ptr_to_uuid.end() && it->second == uuid) {
                    // 找到要删除的 body，调用 api_del_entity
                    try {
                        API_NOP_BEGIN;
                        api_del_entity(e);
                        API_NOP_END;
                    } catch (const std::exception &ex) {
                        std::fprintf(stderr, "[access handleSaveDelta] api_del_entity failed for uuid=%s: %s\n",
                                     uuid.c_str(), ex.what());
                    }
                    body_ptr_to_uuid.erase((void *)e);
                    removed = true;
                    break;
                }
            }
        }
        if (!removed) {
            std::fprintf(stderr, "[access handleSaveDelta] uuid not found for deletion: %s\n", uuid.c_str());
        }
    }

    std::fprintf(stderr, "[access handleSaveDelta] STEP 10: calling api_save_entity_list_neo4j_part, entity count=%d\n",
                 (int)full_entity_list.count());
    fflush(stderr);

    // 6. 全量覆盖写回 Neo4j
    api_save_entity_list_neo4j_part(conn, full_entity_list);

    std::fprintf(stderr, "[access handleSaveDelta] STEP 10.5: persisting body uuids to Neo4j body nodes\n");
    fflush(stderr);

    // 6.5 闭合 uuid 持久化链路：api_save_entity_list_neo4j_part 只给 body 节点写 {a:'body'}，
    // 不带 uuid。handleGetDelta 查询 `RETURN id(b), b.uuid` 永远返回空字符串，
    // 客户端 pull 时 [Mode1] added body uuid= 永远是空。
    // 这里按 entity_list 中 body 的顺序，把内存里的 body_ptr_to_uuid 写回 Neo4j body 节点的 uuid 属性。
    // 查询路径：MATCH (n:part {b:$Y})-[r:part_entity_ptr]->(b:body) RETURN id(b) ORDER BY r.b
    // 返回的 body 节点 id 顺序就是 entity_list 的 body 顺序（part_entity_ptr.b == entity_idx）。
    {
        mg_map *qparams = mg_map_make_empty(1);
        mg_map_append(qparams, mg_string_make("Y"), mg_value_make_string(conn.partname.c_str()));
        conn.execute_bolt("MATCH (n:part {b:$Y})-[r:part_entity_ptr]->(b:body) "
                          "RETURN id(b) AS bid, b.uuid AS buuid ORDER BY toInteger(r.b)",
                          qparams);
        mg_map_destroy(qparams);

        mg_result *result = nullptr;
        std::vector<int64_t> bodyIdsInOrder;
        std::vector<std::string> existingUuids;
        while (mg_session_fetch(conn.session, &result) == 1) {
            const mg_list *row = mg_result_row(result);
            if (row == nullptr || mg_list_size(row) < 1)
                continue;
            const mg_value *bidV = mg_list_at(row, 0);
            int64_t bid = mg_value_integer(bidV);
            std::string existingUuid;
            if (mg_list_size(row) >= 2) {
                const mg_value *buuidV = mg_list_at(row, 1);
                if (buuidV != nullptr && mg_value_get_type(buuidV) == MG_VALUE_TYPE_STRING) {
                    const mg_string *s = mg_value_string(buuidV);
                    if (s != nullptr)
                        existingUuid = std::string(mg_string_data(s), mg_string_size(s));
                }
            }
            bodyIdsInOrder.push_back(bid);
            existingUuids.push_back(existingUuid);
        }
        conn.discard_all_results();

        // 按 entity_list 中 body 顺序，取 body_ptr_to_uuid 中对应的 uuid
        std::vector<ENTITY *> bodyPtrsInOrder;
        for (ENTITY *e : full_entity_list) {
            if (is_BODY(e))
                bodyPtrsInOrder.push_back(e);
        }
        if (bodyPtrsInOrder.size() != bodyIdsInOrder.size()) {
            std::fprintf(
                stderr, "[access handleSaveDelta] body count mismatch: entity_list=%zu neo4j=%zu — skip uuid persist\n",
                bodyPtrsInOrder.size(), bodyIdsInOrder.size());
        } else {
            for (size_t i = 0; i < bodyPtrsInOrder.size(); ++i) {
                auto it = body_ptr_to_uuid.find((void *)bodyPtrsInOrder[i]);
                std::string targetUuid = (it != body_ptr_to_uuid.end()) ? it->second : std::string();
                if (targetUuid.empty())
                    continue;
                // 跳过同 uuid 的重复写入（已有正确 uuid）
                if (i < existingUuids.size() && existingUuids[i] == targetUuid)
                    continue;

                mg_map *setParams = mg_map_make_empty(2);
                mg_map_append(setParams, mg_string_make("B"), mg_value_make_integer(bodyIdsInOrder[i]));
                mg_map_append(setParams, mg_string_make("U"), mg_value_make_string(targetUuid.c_str()));
                conn.execute_bolt("MATCH (b:body) WHERE id(b)=$B SET b.uuid = $U", setParams);
                mg_map_destroy(setParams);
                conn.discard_all_results();
            }
            std::fprintf(stderr, "[access handleSaveDelta] persisted %zu body uuids to Neo4j\n",
                         bodyPtrsInOrder.size());
        }
    }

    std::fprintf(stderr, "[access handleSaveDelta] STEP 11: update_part_node_version\n");
    fflush(stderr);

    // 7. 更新 version 字段
    int new_version = current_version + 1;
    update_part_node_version(conn, conn.partname, new_version, author, timestamp);
    out_new_version = new_version;

    std::fprintf(stderr, "[access handleSaveDelta] STEP 12: create BridgeDeltaVersion for incremental pull replay\n");
    fflush(stderr);

    // 【Bug B 修复】把本次 delta 元数据存入 BridgeDeltaVersion 节点，供 handle_get_delta_from_neo4j
    // 在 pull 时按 base_version 回放历史、计算净 delta（只传 base_version+1..latest 之间的变更，
    // 而不是把整个 part 当前状态全量返回）。
    //
    // 字段含义：
    //   - delta_uuids / delta_sat_segments: 本次新增或修改的 body uuid + SAT 段（一一对应）
    //   - removed_uuids: 本次删除的 body uuid
    // 接收端 replay 时按 version 顺序累加，last-write-wins（同 uuid 被多次 add/remove 则最终态为准）。
    //
    // 用独立的 BridgeDeltaVersion 标签（不混用 mode0 的 BridgeVersion），
    // 避免 mode0 全量 push 写入的 BridgeVersion 节点污染 mode1 增量回放路径。
    //
    // 【简化方案】把 delta 三个数组序列化成一个 JSON 字符串存到 content_text 属性，
    // 而不是用三个独立的 Neo4j list 属性。原因：
    //   1. 避免 mg_value_make_list + 7 个键 vs mg_map_make_empty(6) 大小不匹配，
    //      某些 mgclient 版本会对 hint size 做强校验而 abort；
    //   2. 避免 mg_list 在跨 mgclient / Bolt 序列化层的兼容性陷阱
    //      （Bolt 要求 list 元素类型严格一致，但 mg_value 在跨函数传递时类型可能丢失）；
    //   3. 跟 handleCreateModel 存储 content_text 的模式保持一致，简化心智负担。
    {
        // 从 partname ("delta__{projectId}") 提取 projectId
        const std::string prefix = "delta__";
        std::string projectIdStr;
        if (conn.partname.size() > prefix.size() && conn.partname.compare(0, prefix.size(), prefix) == 0) {
            projectIdStr = conn.partname.substr(prefix.size());
        } else {
            projectIdStr = conn.partname; // 兜底：用整个 partname 作为 projectId
        }

        // 把 delta_uuids / delta_sat_segments / removed_uuids 打包成一个 JSON 字符串
        // 格式：{ "delta_uuids":[...], "delta_sat_segments":[...], "removed_uuids":[...] }
        // 手动拼字符串，避免引入 JSON 库依赖。
        std::string contentJsonStr = "{";
        contentJsonStr += "\"delta_uuids\":[";
        for (size_t i = 0; i < delta_uuids.size(); ++i) {
            if (i > 0)
                contentJsonStr += ",";
            contentJsonStr += "\"";
            // 转义 JSON 字符串中的特殊字符
            for (char c : delta_uuids[i]) {
                if (c == '"' || c == '\\')
                    contentJsonStr += '\\';
                else if (c == '\n') {
                    contentJsonStr += "\\n";
                    continue;
                } else if (c == '\r') {
                    contentJsonStr += "\\r";
                    continue;
                }
                contentJsonStr += c;
            }
            contentJsonStr += "\"";
        }
        contentJsonStr += "],\"delta_sat_segments\":[";
        for (size_t i = 0; i < delta_sat_segments.size(); ++i) {
            if (i > 0)
                contentJsonStr += ",";
            contentJsonStr += "\"";
            for (char c : delta_sat_segments[i]) {
                if (c == '"' || c == '\\')
                    contentJsonStr += '\\';
                else if (c == '\n') {
                    contentJsonStr += "\\n";
                    continue;
                } else if (c == '\r') {
                    contentJsonStr += "\\r";
                    continue;
                }
                contentJsonStr += c;
            }
            contentJsonStr += "\"";
        }
        contentJsonStr += "],\"removed_uuids\":[";
        for (size_t i = 0; i < removed_uuids.size(); ++i) {
            if (i > 0)
                contentJsonStr += ",";
            contentJsonStr += "\"";
            for (char c : removed_uuids[i]) {
                if (c == '"' || c == '\\')
                    contentJsonStr += '\\';
                else if (c == '\n') {
                    contentJsonStr += "\\n";
                    continue;
                } else if (c == '\r') {
                    contentJsonStr += "\\r";
                    continue;
                }
                contentJsonStr += c;
            }
            contentJsonStr += "\"";
        }
        contentJsonStr += "]}";

        mg_map *dvParams = mg_map_make_empty(5);
        mg_map_append(dvParams, mg_string_make("project_id"), mg_value_make_string(projectIdStr.c_str()));
        mg_map_append(dvParams, mg_string_make("version"), mg_value_make_integer(new_version));
        mg_map_append(dvParams, mg_string_make("author"), mg_value_make_string(author.c_str()));
        mg_map_append(dvParams, mg_string_make("created_at"), mg_value_make_string(timestamp.c_str()));
        mg_map_append(dvParams, mg_string_make("content_text"), mg_value_make_string(contentJsonStr.c_str()));

        // 直接 MERGE 节点而不是 OPTIONAL MATCH + CREATE，避免两次 OPTIONAL MATCH + CREATE
        // 链式调用在某些 Neo4j 版本下的 cartesian product 性能问题。
        // BridgeProject 不要求存在（mode1 可能跳过 mode0 的 handleCreateProject）。
        // 但 handleCreateModel 那边会要求 BridgeProject 存在，所以我们也保持一致。
        std::string cypher = "MERGE (v:BridgeDeltaVersion {project_id:$project_id, version:$version}) "
                             "ON CREATE SET v.author=$author, v.created_at=$created_at, v.content_text=$content_text "
                             "ON MATCH SET v.author=$author, v.created_at=$created_at, v.content_text=$content_text "
                             "RETURN id(v)";
        conn.execute_bolt(cypher.c_str(), dvParams);

        mg_result *dvResult = nullptr;
        bool created = false;
        if (mg_session_fetch(conn.session, &dvResult) == 1) {
            created = true;
        }
        conn.discard_all_results();
        mg_map_destroy(dvParams);

        std::fprintf(stderr,
                     "[access handleSaveDelta] BridgeDeltaVersion v=%d project=%s %s "
                     "contentLen=%zu delta_uuids=%zu delta_sat=%zu removed=%zu\n",
                     new_version, projectIdStr.c_str(), created ? "UPSERTED" : "FAILED", contentJsonStr.size(),
                     delta_uuids.size(), delta_sat_segments.size(), removed_uuids.size());
    }

    DIAG_LOG("[save-end] saved v=%d (was %d), entity_list count=%d\n", new_version, current_version,
             (int)full_entity_list.count());
    {
        int body_count = 0, lump_count = 0, shell_count = 0, face_count = 0;
        int body_with_lump = 0;
        for (ENTITY *e : full_entity_list) {
            if (is_BODY(e)) {
                BODY *b = (BODY *)e;
                if (b->lump() != nullptr)
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
        DIAG_LOG("[save-end] topology before write: bodies=%d (with_lump=%d) lumps=%d shells=%d faces=%d\n", body_count,
                 body_with_lump, lump_count, shell_count, face_count);
    }

    return true;
}

// ------------------------------------------------------------------------------------------------
// handleGetDelta 核心逻辑（C++端，供 storage_bridge 调用）
//
// base_version: B 端上次 sync 的版本号
//
// 流程（增量模式，新行为）：
//   1. 读取 latest_version = part_node.version
//   2. 若 base_version >= latest_version，返回空 delta
//   3. 查询 BridgeDeltaVersion 节点，version ∈ [base_version+1, latest_version]
//   4. 按 version 顺序回放 delta：
//        - delta_uuids[i] + 对应 SAT 段 → 加入 added map（last-write-wins SAT）
//        - removed_uuids → 加入 deleted set
//        - 同 uuid 后续又 add → 从 deleted set 移除，覆盖 added map 中的 SAT
//        - 同 uuid 后续又 remove → 从 added map 移除
//   5. 输出 added map 为 delta_bodies，deleted set 为 deleted_uuids
//   6. 历史缺失（gap）时降级为旧行为：返回当前 part 完整状态（无 deleted）
//
// 全量回退（旧行为）：
//   - 当 BridgeDeltaVersion 历史不完整时（比如历史版本没建过该节点），
//     返回 part 当前所有 body 的 SAT。客户端 set-diff 合并可以正常工作，
//     只是会比增量模式多传一些冗余 SAT。
// ------------------------------------------------------------------------------------------------

// 极简 JSON 解析：从 "{ ... }" 中按字段名读取字符串数组。
// 用来反序列化 BridgeDeltaVersion.content_text 里的 delta_uuids / delta_sat_segments / removed_uuids。
// 不依赖任何 JSON 库，避免引入新依赖。
// 解析失败（字段缺失）时不抛异常，输出数组保持为空。
