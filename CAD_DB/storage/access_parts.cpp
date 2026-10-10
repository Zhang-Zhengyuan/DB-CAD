#include "access_internal.hxx"

void api_save_entity_list_neo4j_part(const Neo4jPart &conn, const ENTITY_LIST &entity_list) {
    std::fprintf(stderr, "[api_save_entity_list_neo4j_part] ENTER, partname=%s entity_count=%d\n",
                 conn.partname.c_str(), (int)entity_list.count());
    fflush(stderr);

    mg_map *qparams = mg_map_make_empty(1);
    mg_map_append(qparams, mg_string_make("Y"), mg_value_make_string(conn.partname.c_str()));

    std::fprintf(stderr, "[api_save_entity_list_neo4j_part] deleting existing subgraph\n");
    fflush(stderr);
    // 删除旧 body 及其整个子树，然后删除旧 part node
    // DETACH DELETE body 会级联删除 lump->shell->face->loop->coedge->edge->vertex
    // OPTIONAL MATCH 避免在没有旧 body 时查询失败
    conn.execute_bolt("MATCH (n:part {b:$Y}) "
                      "OPTIONAL MATCH (n)-[r]-(b:body) "
                      "FOREACH (x IN CASE WHEN b IS NOT NULL THEN [b] ELSE [] END | DETACH DELETE x) "
                      "DETACH DELETE n",
                      qparams);
    conn.discard_all_results();

    std::fprintf(stderr, "[api_save_entity_list_neo4j_part] creating part node\n");
    fflush(stderr);
    conn.execute_bolt("CREATE (n:part {a:'part',b:$Y}) RETURN id(n)", qparams);
    mg_map_destroy(qparams);

    int64_t partnodeid;
    mg_result *result;
    if (mg_session_fetch(conn.session, &result) == 1) {
        const mg_list *mgl_partnodeid = mg_result_row(result);
        const uint32_t mgl_partnodeid_length = mg_list_size(mgl_partnodeid);
        assert(mgl_partnodeid_length == 1);
        partnodeid = mg_value_integer(mg_list_at(mgl_partnodeid, 0));
        std::fprintf(stderr, "[api_save_entity_list_neo4j_part] partnodeid=%lld\n", (long long)partnodeid);
        fflush(stderr);
    } else {
        // 查询无结果时 partnodeid 保持为 0，这可能是正常情况（如首次创建）
        partnodeid = 0;
        std::fprintf(stderr, "[api_save_entity_list_neo4j_part] no result, partnodeid=0\n");
        fflush(stderr);
    }
    // 注意：mg_session_fetch 返回 0 表示"没有更多结果"（正常情况），不要对返回 0 调用 myerror()。
    conn.discard_all_results();

    std::fprintf(stderr, "[api_save_entity_list_neo4j_part] calling api_save_entity_list_neo4j\n");
    fflush(stderr);
    std::unordered_map<void *, int64_t> ptr2elemid;
    api_save_entity_list_neo4j(conn, entity_list, ptr2elemid);

    std::fprintf(stderr, "[api_save_entity_list_neo4j_part] creating relationships, elem_count=%d\n",
                 (int)entity_list.iteration_count());
    fflush(stderr);

    std::vector<int64_t> elemid_list;
    uint32_t elemid_list_size = entity_list.iteration_count();
    mg_list *mgl_elemid_list = mg_list_make_empty(elemid_list_size);
    int64_t entity_idx = 0;
    for (ENTITY *e

         : entity_list) {
        mg_map *mgm_rel = mg_map_make_empty(2);
        mg_map_append(mgm_rel, mg_string_make("U"), mg_value_make_integer(ptr2elemid.at(e)));
        mg_map_append(mgm_rel, mg_string_make("V"), mg_value_make_integer(entity_idx));
        mg_list_append(mgl_elemid_list, mg_value_make_map(mgm_rel));
        entity_idx++;
    }
    qparams = mg_map_make_empty(2);
    mg_map_append(qparams, mg_string_make("Y"), mg_value_make_list(mgl_elemid_list));
    mg_map_append(qparams, mg_string_make("A"), mg_value_make_integer(partnodeid));
    conn.execute_bolt("UNWIND $Y AS Z "
                      "MATCH (W) WHERE id(W) = $A "
                      "MATCH (X) WHERE id(X) = Z.U "
                      "CREATE (W)-[r:part_entity_ptr {a:'part_entity_ptr',b:Z.V}]->(X) ",
                      qparams);
    mg_map_destroy(qparams);
    conn.discard_all_results();
}

void api_restore_entity_list_neo4j_part(const Neo4jPart &conn, ENTITY_LIST &entity_list) {
    std::vector<int64_t> elemid_list;
    mg_map *qparams = mg_map_make_empty(1);
    mg_map_append(qparams, mg_string_make("Y"), mg_value_make_string(conn.partname.c_str()));
    // ORDER BY toInteger(r.b) — part_entity_ptr 的属性 b 是 entity_idx（写入时 b:Z.V）。
    // 原始代码用 ORDER BY r.idx，idx 字段不存在，所有行都按 NULL 排，body 顺序随机。
    // 这会导致 api_save_entity_list_neo4j_part 的 STEP 10.5 uuid 持久化对应错乱，
    // 也会让下游 push/get 端 body 顺序不稳定。
    conn.execute_bolt("MATCH (n:part {b:$Y})-[r:part_entity_ptr]->(c) RETURN id(c) ORDER BY toInteger(r.b) ", qparams);
    mg_map_destroy(qparams);

    mg_result *result;
    int status;
    while (1) {
        status = mg_session_fetch(conn.session, &result);
        if (status == 1) {
            const mg_list *mgl_entitynodeid = mg_result_row(result);
            const uint32_t mgl_entitynodeid_length = mg_list_size(mgl_entitynodeid);
            assert(mgl_entitynodeid_length == 1);
            elemid_list.push_back(mg_value_integer(mg_list_at(mgl_entitynodeid, 0)));
        } else if (status == 0) {
            break;
        } else {
            myerror(std::format("mg_session_fetch失败，错误信息如下：{}", mg_session_error(conn.session)));
        }
    }

    std::unordered_map<int64_t, void *> elemid2ptr;
    api_restore_entity_list_neo4j(conn, elemid_list, entity_list, elemid2ptr);
}

int64_t count_partnode(const Neo4jPart &conn) {
    mg_map *qparams = mg_map_make_empty(1);
    mg_map_append(qparams, mg_string_make("Y"), mg_value_make_string(conn.partname.c_str()));
    conn.execute_bolt("MATCH (n:part {b:$Y}) RETURN count(n) ", qparams);
    mg_map_destroy(qparams);

    int64_t partnodecount;
    mg_result *result;
    if (mg_session_fetch(conn.session, &result) == 1) {
        const mg_list *mgl_partnodecount = mg_result_row(result);
        const uint32_t mgl_partnodecount_length = mg_list_size(mgl_partnodecount);
        assert(mgl_partnodecount_length == 1);
        partnodecount = mg_value_integer(mg_list_at(mgl_partnodecount, 0));
    } else {
        myerror(std::format("mg_session_fetch失败，错误信息如下：{}", mg_session_error(conn.session)));
    }
    if (mg_session_fetch(conn.session, &result) != 0) {
        myerror(std::format("mg_session_fetch失败，错误信息如下：{}", mg_session_error(conn.session)));
    }

    return partnodecount;
}

// ================================================================================================
// Mode1 Delta Push: 基于 body UUID 的增量存储
// ================================================================================================

// 读取 part 节点的当前 version（不存在则返回 0）
