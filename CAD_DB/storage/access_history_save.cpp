#include "access_internal.hxx"

void api_save_neo4j(const Neo4jPart &conn, IncrementalContext &ctx) {
    // 1. 确保“部件”节点存在，并获取版本信息
    mg_map *qparams = mg_map_make_empty(1);
    mg_map_append(qparams, mg_string_make("Y"), mg_value_make_string(conn.partname.c_str()));
    conn.execute_bolt("MERGE (n:part {a:'part',b:$Y}) RETURN id(n),n.c,n.d", qparams);
    mg_map_destroy(qparams);

    // ... 解析查询结果，获取 partnodeid, partnodenextgeneration, partnodecurgeneration ...
    int64_t partnodeid, partnodenextgeneration, partnodecurgeneration;
    mg_result *result;
    if (mg_session_fetch(conn.session, &result) == 1) {
        const mg_list *mgl_partnodeid = mg_result_row(result);
        const uint32_t mgl_partnodeid_length = mg_list_size(mgl_partnodeid);
        assert(mgl_partnodeid_length == 3);
        partnodeid = mg_value_integer(mg_list_at(mgl_partnodeid, 0));

        const mg_value *mgv_partnodemaxgeneration = mg_list_at(mgl_partnodeid, 1);
        mg_value_type mgv_partnodemaxgeneration_type = mg_value_get_type(mgv_partnodemaxgeneration);
        if (mgv_partnodemaxgeneration_type == MG_VALUE_TYPE_NULL) {
            partnodenextgeneration = 1;
        } else {
            partnodenextgeneration = mg_value_integer(mgv_partnodemaxgeneration) + 1;
        }

        const mg_value *mgv_partnodecurgeneration = mg_list_at(mgl_partnodeid, 2);
        mg_value_type mgv_partnodecurgeneration_type = mg_value_get_type(mgv_partnodecurgeneration);
        if (mgv_partnodecurgeneration_type == MG_VALUE_TYPE_NULL) {
            partnodecurgeneration = -1;
        } else {
            partnodecurgeneration = mg_value_integer(mgv_partnodecurgeneration);
        }
    } else {
        myerror(std::format("mg_session_fetch失败，错误信息如下：{}", mg_session_error(conn.session)));
    }
    if (mg_session_fetch(conn.session, &result) != 0) {
        myerror(std::format("mg_session_fetch失败，错误信息如下：{}", mg_session_error(conn.session)));
    }

    // 2. 如果不是第一次保存，找到上一个版本的“generation”节点ID
    int64_t curgenerationnodeid = -1;
    if (partnodenextgeneration > 1) {
        // ... 查询获取 curgenerationnodeid ...
        qparams = mg_map_make_empty(2);
        mg_map_append(qparams, mg_string_make("A"), mg_value_make_integer(partnodeid));
        mg_map_append(qparams, mg_string_make("B"), mg_value_make_integer(partnodecurgeneration));
        conn.execute_bolt(
            "MATCH (n)-[r:part_generation_ptr {a:'part_generation_ptr',b:$B}]->(m) WHERE id(n)=$A RETURN id(m)",
            qparams);
        mg_map_destroy(qparams);
        if (mg_session_fetch(conn.session, &result) == 1) {
            const mg_list *mgl_curgenerationnodeid = mg_result_row(result);
            const uint32_t mgl_curgenerationnodeid_length = mg_list_size(mgl_curgenerationnodeid);
            assert(mgl_curgenerationnodeid_length == 1);
            curgenerationnodeid = mg_value_integer(mg_list_at(mgl_curgenerationnodeid, 0));
        } else {
            myerror(std::format("mg_session_fetch失败，错误信息如下：{}", mg_session_error(conn.session)));
        }
        if (mg_session_fetch(conn.session, &result) != 0) {
            myerror(std::format("mg_session_fetch失败，错误信息如下：{}", mg_session_error(conn.session)));
        }
    }

    // 3. 获取ACIS历史状态
    DELTA_STATE *thissave_ds; // 这次保存
    api_note_state(thissave_ds);
    {
        // ... 处理 thissave_ds 和 ctx.lastsave_ds 为空的初始情况 ...
        HISTORY_STREAM *hs;
        api_get_default_history(hs);
        if (thissave_ds == nullptr) {
            thissave_ds = hs->get_active_ds(); // 最新的差状态
        }
        if (ctx.lastsave_ds == nullptr) {
            ctx.lastsave_ds = hs->get_root_ds(); // 最早的差状态
        }
    }

    // 4. 遍历ACIS历史，找出变更的实体
    std::unordered_set<ENTITY *> created_or_updated_entity_list, deleted_entity_list;
    DELTA_STATE *this_ds = thissave_ds;
    while (this_ds != ctx.lastsave_ds) {
        BULLETIN_BOARD *bb = this_ds->bb();
        while (bb) {
            BULLETIN *b = bb->start_bulletin();
            while (b) {
                ENTITY *old_ent = b->old_entity_ptr();
                ENTITY *new_ent = b->new_entity_ptr();
                if (new_ent) {
                    created_or_updated_entity_list.insert(new_ent);
                } else if (old_ent) {
                    // new_ent==NULL && old_ent!=NULL
                    deleted_entity_list.insert(old_ent);
                }
                b = b->next();
            }
            bb = bb->next();
        }
        this_ds = this_ds->next();
    }

    // 5. 过滤实体列表
    // ... 移除在同一个delta中先创建后删除的实体 ...
    // ... 移除不支持的实体类型 (如 ANNOTATION, ATTRIB) ...
    for (auto it = created_or_updated_entity_list.begin(); it != created_or_updated_entity_list.end();) {
        if (deleted_entity_list.find(*it) != deleted_entity_list.end()) {
            deleted_entity_list.erase(*it);
            it = created_or_updated_entity_list.erase(it);
        } else {
            switch ((*it)->identity(1)) {
            case BODY_ID:
            case LUMP_ID:
            case SHELL_ID:
            case SUBSHELL_ID:
            case WIRE_ID:
            case FACE_ID:
            case LOOP_ID:
            case COEDGE_ID:
            case EDGE_ID:
            case VERTEX_ID:
            case APOINT_ID:
            case TRANSFORM_ID: {
                it++;
            } break;
            case CURVE_ID: {
                CURVE *ptr = (CURVE *)(*it);
                switch (ptr->identity(2)) {
                case STRAIGHT_ID:
                case ELLIPSE_ID:
                case HELIX_ID: {
                    it++;
                } break;
                default: {
                    it = created_or_updated_entity_list.erase(it);
                } break;
                }
            } break;
            case SURFACE_ID: {
                SURFACE *ptr = (SURFACE *)(*it);
                switch (ptr->identity(2)) {
                case PLANE_ID:
                case SPHERE_ID:
                case TORUS_ID:
                case CONE_ID: {
                    it++;
                } break;
                default: {
                    it = created_or_updated_entity_list.erase(it);
                } break;
                }
            } break;
            default: {
                it = created_or_updated_entity_list.erase(it);
            } break;
            }
        }
    }

    for (auto it = deleted_entity_list.begin(); it != deleted_entity_list.end();) {
        switch ((*it)->identity(1)) {
        case BODY_ID:
        case LUMP_ID:
        case SHELL_ID:
        case SUBSHELL_ID:
        case WIRE_ID:
        case FACE_ID:
        case LOOP_ID:
        case COEDGE_ID:
        case EDGE_ID:
        case VERTEX_ID:
        case APOINT_ID:
        case TRANSFORM_ID: {
            it++;
        } break;
        case CURVE_ID: {
            CURVE *ptr = (CURVE *)(*it);
            switch (ptr->identity(2)) {
            case STRAIGHT_ID:
            case ELLIPSE_ID:
            case HELIX_ID: {
                it++;
            } break;
            default: {
                it = deleted_entity_list.erase(it);
            } break;
            }
        } break;
        case SURFACE_ID: {
            SURFACE *ptr = (SURFACE *)(*it);
            switch (ptr->identity(2)) {
            case PLANE_ID:
            case SPHERE_ID:
            case TORUS_ID:
            case CONE_ID: {
                it++;
            } break;
            default: {
                it = deleted_entity_list.erase(it);
            } break;
            }
        } break;
        default: {
            it = deleted_entity_list.erase(it);
        } break;
        }
    }

#ifdef _DEBUG
    printf("\ncreated_or_updated_entity_list : %llu\n", created_or_updated_entity_list.size());
    for (const auto &entity_ptr : created_or_updated_entity_list) {
        printf("%s, 0x%x\n", entity_ptr->type_name(), (long)entity_ptr);
    }
    // assert(deleted_entity_list.empty());
    printf("\ndeleted_entity_list : %llu\n", deleted_entity_list.size());
    for (const auto &entity_ptr : deleted_entity_list) {
        printf("%s, 0x%x\n", entity_ptr->type_name(), (long)entity_ptr);
    }
#endif

    ctx.lastsave_ds = thissave_ds;

    // 6. 为每个变更的实体创建内存中的Node对象
    std::vector<Node *> node_list;
    // ... 其他辅助列表 ...
    std::vector<Relationship2 *> relationship_list;
    std::vector<int64_t> updated_entity_nodeid_list, deleted_entity_nodeid_list;
    std::vector<void *> updated_entity_list;
    std::unordered_map<void *, Node *> ptr2node;

    // 创建一个代表新版本的 'generation' 节点
    Node *generationnode = new Node();
    // ... 设置 generationnode 的标签和属性 ...
    node_list.push_back(generationnode);
    generationnode->labels.clear();
    generationnode->labels.push_back("generation");
    generationnode->properties['a'] = mg_value_make_string("generation");
    generationnode->properties['b'] = mg_value_make_integer(partnodenextgeneration);

    for (const auto &entity_ptr : created_or_updated_entity_list) {
        switch (entity_ptr->identity(1)) {
        case BODY_ID: {
            BODY *ptr = (BODY *)entity_ptr;
            Node *ptrnode = new Node();
            if (ctx.ptr2nodeid.find(ptr) != ctx.ptr2nodeid.end()) {
                updated_entity_nodeid_list.push_back(ctx.ptr2nodeid.at(ptr));
                updated_entity_list.push_back(ptr);
            }
            ptr2node[ptr] = ptrnode;
            node_list.push_back(ptrnode);
            ptrnode->labels.clear();
            ptrnode->labels.push_back("body");
            ptrnode->properties['a'] = mg_value_make_string("body");
        } break;
        case LUMP_ID: {
            LUMP *ptr = (LUMP *)entity_ptr;
            Node *ptrnode = new Node();
            if (ctx.ptr2nodeid.find(ptr) != ctx.ptr2nodeid.end()) {
                updated_entity_nodeid_list.push_back(ctx.ptr2nodeid.at(ptr));
                updated_entity_list.push_back(ptr);
            }
            ptr2node[ptr] = ptrnode;
            node_list.push_back(ptrnode);
            ptrnode->labels.clear();
            ptrnode->labels.push_back("lump");
            ptrnode->properties['a'] = mg_value_make_string("lump");
        } break;
        case SHELL_ID: {
            SHELL *ptr = (SHELL *)entity_ptr;
            Node *ptrnode = new Node();
            if (ctx.ptr2nodeid.find(ptr) != ctx.ptr2nodeid.end()) {
                updated_entity_nodeid_list.push_back(ctx.ptr2nodeid.at(ptr));
                updated_entity_list.push_back(ptr);
            }
            ptr2node[ptr] = ptrnode;
            node_list.push_back(ptrnode);
            ptrnode->labels.clear();
            ptrnode->labels.push_back("shell");
            ptrnode->properties['a'] = mg_value_make_string("shell");
        } break;
        case SUBSHELL_ID: {
            SUBSHELL *ptr = (SUBSHELL *)entity_ptr;
            Node *ptrnode = new Node();
            if (ctx.ptr2nodeid.find(ptr) != ctx.ptr2nodeid.end()) {
                updated_entity_nodeid_list.push_back(ctx.ptr2nodeid.at(ptr));
                updated_entity_list.push_back(ptr);
            }
            ptr2node[ptr] = ptrnode;
            node_list.push_back(ptrnode);
            ptrnode->labels.clear();
            ptrnode->labels.push_back("subshell");
            ptrnode->properties['a'] = mg_value_make_string("subshell");
        } break;
        case WIRE_ID: {
            WIRE *ptr = (WIRE *)entity_ptr;
            Node *ptrnode = new Node();
            if (ctx.ptr2nodeid.find(ptr) != ctx.ptr2nodeid.end()) {
                updated_entity_nodeid_list.push_back(ctx.ptr2nodeid.at(ptr));
                updated_entity_list.push_back(ptr);
            }
            ptr2node[ptr] = ptrnode;
            node_list.push_back(ptrnode);
            ptrnode->labels.clear();
            ptrnode->labels.push_back("wire");
            ptrnode->properties['a'] = mg_value_make_string("wire");
            ptrnode->properties['b'] = mg_value_make_integer(ptr->cont());
        } break;
        case FACE_ID: {
            FACE *ptr = (FACE *)entity_ptr;
            Node *ptrnode = new Node();
            if (ctx.ptr2nodeid.find(ptr) != ctx.ptr2nodeid.end()) {
                updated_entity_nodeid_list.push_back(ctx.ptr2nodeid.at(ptr));
                updated_entity_list.push_back(ptr);
            }
            ptr2node[ptr] = ptrnode;
            node_list.push_back(ptrnode);
            if (ptr->sides()) {
                ptrnode->labels.clear();
                ptrnode->labels.push_back("face");
                ptrnode->properties['a'] = mg_value_make_string("face");
                ptrnode->properties['b'] = mg_value_make_integer(ptr->sense());
                ptrnode->properties['c'] = mg_value_make_integer(1);
                ptrnode->properties['d'] = mg_value_make_integer(ptr->cont());
            } else {
                ptrnode->labels.clear();
                ptrnode->labels.push_back("face");
                ptrnode->properties['a'] = mg_value_make_string("face");
                ptrnode->properties['b'] = mg_value_make_integer(ptr->sense());
                ptrnode->properties['c'] = mg_value_make_integer(0);
            }
        } break;
        case LOOP_ID: {
            LOOP *ptr = (LOOP *)entity_ptr;
            Node *ptrnode = new Node();
            if (ctx.ptr2nodeid.find(ptr) != ctx.ptr2nodeid.end()) {
                updated_entity_nodeid_list.push_back(ctx.ptr2nodeid.at(ptr));
                updated_entity_list.push_back(ptr);
            }
            ptr2node[ptr] = ptrnode;
            node_list.push_back(ptrnode);
            ptrnode->labels.clear();
            ptrnode->labels.push_back("loop");
            ptrnode->properties['a'] = mg_value_make_string("loop");
        } break;
        case COEDGE_ID: {
            COEDGE *ptr = (COEDGE *)entity_ptr;
            Node *ptrnode = new Node();
            if (ctx.ptr2nodeid.find(ptr) != ctx.ptr2nodeid.end()) {
                updated_entity_nodeid_list.push_back(ctx.ptr2nodeid.at(ptr));
                updated_entity_list.push_back(ptr);
            }
            ptr2node[ptr] = ptrnode;
            node_list.push_back(ptrnode);
            ptrnode->labels.clear();
            ptrnode->labels.push_back("coedge");
            ptrnode->properties['a'] = mg_value_make_string("coedge");
            ptrnode->properties['b'] = mg_value_make_integer(ptr->sense());
        } break;
        case EDGE_ID: {
            EDGE *ptr = (EDGE *)entity_ptr;
            Node *ptrnode = new Node();
            if (ctx.ptr2nodeid.find(ptr) != ctx.ptr2nodeid.end()) {
                updated_entity_nodeid_list.push_back(ctx.ptr2nodeid.at(ptr));
                updated_entity_list.push_back(ptr);
            }
            ptr2node[ptr] = ptrnode;
            node_list.push_back(ptrnode);
            ptrnode->labels.clear();
            ptrnode->labels.push_back("edge");
            ptrnode->properties['a'] = mg_value_make_string("edge");
            ptrnode->properties['b'] = mg_value_make_integer(ptr->sense());
        } break;
        case VERTEX_ID: {
            VERTEX *ptr = (VERTEX *)entity_ptr;
            Node *ptrnode = new Node();
            if (ctx.ptr2nodeid.find(ptr) != ctx.ptr2nodeid.end()) {
                updated_entity_nodeid_list.push_back(ctx.ptr2nodeid.at(ptr));
                updated_entity_list.push_back(ptr);
            }
            ptr2node[ptr] = ptrnode;
            node_list.push_back(ptrnode);
            ptrnode->labels.clear();
            ptrnode->labels.push_back("vertex");
            ptrnode->properties['a'] = mg_value_make_string("vertex");
        } break;
        case APOINT_ID: {
            APOINT *ptr = (APOINT *)entity_ptr;
            SPAposition pos = ((APOINT *)ptr)->coords();
            Node *ptrnode = new Node();
            if (ctx.ptr2nodeid.find(ptr) != ctx.ptr2nodeid.end()) {
                updated_entity_nodeid_list.push_back(ctx.ptr2nodeid.at(ptr));
                updated_entity_list.push_back(ptr);
            }
            ptr2node[ptr] = ptrnode;
            node_list.push_back(ptrnode);
            ptrnode->labels.clear();
            ptrnode->labels.push_back("point");
            ptrnode->properties['a'] = mg_value_make_string("point");
            ptrnode->properties['b'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAposition(pos, 3));
        } break;
        case CURVE_ID: {
            CURVE *ptr = (CURVE *)entity_ptr;
            switch (ptr->identity(2)) {
            case STRAIGHT_ID: {
                STRAIGHT *ptr = (STRAIGHT *)entity_ptr;
                straight gem = ((STRAIGHT *)ptr)->gme_get_def();
                SPAposition root_point = gem.root_point;
                SPAunit_vector direction = gem.direction;
                direction.set_x(direction.x() * gem.param_scale);
                direction.set_y(direction.y() * gem.param_scale);
                direction.set_z(direction.z() * gem.param_scale);
                SPAinterval range = gem.gme_get_subset_range();
                Node *ptrnode = new Node();
                if (ctx.ptr2nodeid.find(ptr) != ctx.ptr2nodeid.end()) {
                    updated_entity_nodeid_list.push_back(ctx.ptr2nodeid.at(ptr));
                    updated_entity_list.push_back(ptr);
                }
                ptr2node[ptr] = ptrnode;
                node_list.push_back(ptrnode);
                ptrnode->labels.clear();
                ptrnode->labels.push_back("straight-curve");
                ptrnode->properties['a'] = mg_value_make_string("straight-curve");
                ptrnode->properties['b'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAinterval(range));
                ptrnode->properties['c'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAposition(root_point, 3));
                ptrnode->properties['d'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAvector(direction));
            } break;
            case ELLIPSE_ID: {
                ELLIPSE *ptr = (ELLIPSE *)entity_ptr;
                ellipse gem = ((ELLIPSE *)ptr)->gme_get_def();
                SPAposition centre = gem.centre;
                SPAunit_vector normal = gem.normal;
                SPAvector major_axis = gem.major_axis;
                SPAinterval range = gem.gme_get_subset_range();
                Node *ptrnode = new Node();
                if (ctx.ptr2nodeid.find(ptr) != ctx.ptr2nodeid.end()) {
                    updated_entity_nodeid_list.push_back(ctx.ptr2nodeid.at(ptr));
                    updated_entity_list.push_back(ptr);
                }
                ptr2node[ptr] = ptrnode;
                node_list.push_back(ptrnode);
                ptrnode->labels.clear();
                ptrnode->labels.push_back("ellipse-curve");
                ptrnode->properties['a'] = mg_value_make_string("ellipse-curve");
                ptrnode->properties['b'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAinterval(range));
                ptrnode->properties['c'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAposition(centre, 3));
                ptrnode->properties['d'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAunit_vector(normal));
                ptrnode->properties['e'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAvector(major_axis));
                ptrnode->properties['f'] = mg_value_make_float(gem.radius_ratio);
            } break;
            case HELIX_ID: {
                HELIX *ptr = (HELIX *)entity_ptr;
                helix gem = ((HELIX *)ptr)->gme_get_def();
                SPAposition axis_root = gem.axis_root();
                SPAunit_vector axis_dir = gem.axis_dir();
                SPAvector start_disp = gem.start_disp();
                SPAinterval helix_range = gem.helix_range();
                SPAinterval range = gem.gme_get_subset_range();
                Node *ptrnode = new Node();
                if (ctx.ptr2nodeid.find(ptr) != ctx.ptr2nodeid.end()) {
                    updated_entity_nodeid_list.push_back(ctx.ptr2nodeid.at(ptr));
                    updated_entity_list.push_back(ptr);
                }
                ptr2node[ptr] = ptrnode;
                node_list.push_back(ptrnode);
                ptrnode->labels.clear();
                ptrnode->labels.push_back("helix-curve");
                ptrnode->properties['a'] = mg_value_make_string("helix-curve");
                ptrnode->properties['b'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAinterval(range));
                ptrnode->properties['c'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAposition(axis_root, 3));
                ptrnode->properties['d'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAunit_vector(axis_dir));
                ptrnode->properties['e'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAvector(start_disp));
                ptrnode->properties['f'] = mg_value_make_float(gem.pitch());
                ptrnode->properties['g'] = mg_value_make_integer(gem.handedness());
                ptrnode->properties['h'] = mg_value_make_float(gem.par_scaling());
                ptrnode->properties['i'] = mg_value_make_float(gem.taper());
                ptrnode->properties['j'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAinterval(helix_range));
            } break;
            default: {
                // unknown curve
            } break;
            }
        } break;
        case SURFACE_ID: {
            SURFACE *ptr = (SURFACE *)entity_ptr;
            switch (ptr->identity(2)) {
            case PLANE_ID: {
                PLANE *ptr = (PLANE *)entity_ptr;
                plane gem = ((PLANE *)ptr)->gme_get_def();
                AccessUtils::Save::plane_data *gem_data = AccessUtils::Save::get_plane_data(&gem);
                Node *ptrnode = new Node();
                if (ctx.ptr2nodeid.find(ptr) != ctx.ptr2nodeid.end()) {
                    updated_entity_nodeid_list.push_back(ctx.ptr2nodeid.at(ptr));
                    updated_entity_list.push_back(ptr);
                }
                ptr2node[ptr] = ptrnode;
                node_list.push_back(ptrnode);
                ptrnode->labels.clear();
                ptrnode->labels.push_back("plane-surface");
                ptrnode->properties['a'] = mg_value_make_string("plane-surface");
                ptrnode->properties['b'] = mg_value_make_list(gem_data->subset_range);
                ptrnode->properties['c'] = mg_value_make_list(gem_data->root_point);
                ptrnode->properties['d'] = mg_value_make_list(gem_data->normal);
                ptrnode->properties['e'] = mg_value_make_list(gem_data->u_deriv);
                ptrnode->properties['f'] = mg_value_make_integer(gem_data->reverse_v);
                delete gem_data;
            } break;
            case SPHERE_ID: {
                SPHERE *ptr = (SPHERE *)entity_ptr;
                sphere gem = ((SPHERE *)ptr)->gme_get_def();
                AccessUtils::Save::sphere_data *gem_data = AccessUtils::Save::get_sphere_data(&gem);
                Node *ptrnode = new Node();
                if (ctx.ptr2nodeid.find(ptr) != ctx.ptr2nodeid.end()) {
                    updated_entity_nodeid_list.push_back(ctx.ptr2nodeid.at(ptr));
                    updated_entity_list.push_back(ptr);
                }
                ptr2node[ptr] = ptrnode;
                node_list.push_back(ptrnode);
                ptrnode->labels.clear();
                ptrnode->labels.push_back("sphere-surface");
                ptrnode->properties['a'] = mg_value_make_string("sphere-surface");
                ptrnode->properties['b'] = mg_value_make_list(gem_data->subset_range);
                ptrnode->properties['c'] = mg_value_make_list(gem_data->centre);
                ptrnode->properties['d'] = mg_value_make_float(gem_data->radius);
                ptrnode->properties['e'] = mg_value_make_list(gem_data->uv_oridir);
                ptrnode->properties['f'] = mg_value_make_list(gem_data->pole_dir);
                ptrnode->properties['g'] = mg_value_make_integer(gem_data->reverse_v);
                delete gem_data;
            } break;
            case TORUS_ID: {
                TORUS *ptr = (TORUS *)entity_ptr;
                torus gem = ((TORUS *)ptr)->gme_get_def();
                AccessUtils::Save::torus_data *gem_data = AccessUtils::Save::get_torus_data(&gem);
                Node *ptrnode = new Node();
                if (ctx.ptr2nodeid.find(ptr) != ctx.ptr2nodeid.end()) {
                    updated_entity_nodeid_list.push_back(ctx.ptr2nodeid.at(ptr));
                    updated_entity_list.push_back(ptr);
                }
                ptr2node[ptr] = ptrnode;
                node_list.push_back(ptrnode);
                ptrnode->labels.clear();
                ptrnode->labels.push_back("torus-surface");
                ptrnode->properties['a'] = mg_value_make_string("torus-surface");
                ptrnode->properties['b'] = mg_value_make_list(gem_data->subset_range);
                ptrnode->properties['c'] = mg_value_make_list(gem_data->centre);
                ptrnode->properties['d'] = mg_value_make_list(gem_data->normal);
                ptrnode->properties['e'] = mg_value_make_float(gem_data->major_radius);
                ptrnode->properties['f'] = mg_value_make_float(gem_data->minor_radius);
                ptrnode->properties['g'] = mg_value_make_list(gem_data->uv_oridir);
                ptrnode->properties['h'] = mg_value_make_integer(gem_data->reverse_v);
                delete gem_data;
            } break;
            case CONE_ID: {
                CONE *ptr = (CONE *)entity_ptr;
                cone gem = ((CONE *)ptr)->gme_get_def();
                AccessUtils::Save::cone_data *gem_data = AccessUtils::Save::get_cone_data(&gem);
                Node *ptrnode = new Node();
                if (ctx.ptr2nodeid.find(ptr) != ctx.ptr2nodeid.end()) {
                    updated_entity_nodeid_list.push_back(ctx.ptr2nodeid.at(ptr));
                    updated_entity_list.push_back(ptr);
                }
                ptr2node[ptr] = ptrnode;
                node_list.push_back(ptrnode);
                ptrnode->labels.clear();
                ptrnode->labels.push_back("cone-surface");
                ptrnode->properties['a'] = mg_value_make_string("cone-surface");
                ptrnode->properties['b'] = mg_value_make_list(gem_data->subset_range);
                ptrnode->properties['c'] = mg_value_make_list(gem_data->base_centre);
                ptrnode->properties['d'] = mg_value_make_list(gem_data->base_normal);
                ptrnode->properties['e'] = mg_value_make_list(gem_data->base_major_axis);
                ptrnode->properties['f'] = mg_value_make_float(gem_data->base_radius_ratio);
                ptrnode->properties['g'] = mg_value_make_list(gem_data->base_subset_range);
                ptrnode->properties['h'] = mg_value_make_float(gem_data->sine_angle);
                ptrnode->properties['i'] = mg_value_make_float(gem_data->cosine_angle);
                ptrnode->properties['j'] = mg_value_make_integer(gem_data->reverse_u);
                delete gem_data;
            } break;
            default: {
                // unknown surface
            } break;
            }
        } break;
        case TRANSFORM_ID: {
            TRANSFORM *ptr = (TRANSFORM *)entity_ptr;
            SPAtransf transf = ((TRANSFORM *)ptr)->transform();
            SPAmatrix affine = transf.affine();
            SPAvector translation = transf.translation();
            Node *ptrnode = new Node();
            if (ctx.ptr2nodeid.find(ptr) != ctx.ptr2nodeid.end()) {
                updated_entity_nodeid_list.push_back(ctx.ptr2nodeid.at(ptr));
                updated_entity_list.push_back(ptr);
            }
            ptr2node[ptr] = ptrnode;
            node_list.push_back(ptrnode);
            ptrnode->labels.clear();
            ptrnode->labels.push_back("transform");
            ptrnode->properties['a'] = mg_value_make_string("transform");
            ptrnode->properties['b'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAmatrix(affine));
            ptrnode->properties['c'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAvector(translation));
            ptrnode->properties['d'] = mg_value_make_float(transf.scaling());
            ptrnode->properties['e'] = mg_value_make_integer(transf.rotate());
            ptrnode->properties['f'] = mg_value_make_integer(transf.reflect());
            ptrnode->properties['g'] = mg_value_make_integer(transf.shear());
        } break;
        default: {
            // PATTERN, ATTRIB等其他继承于ENTITY的实体
        } break;
        }
    }

    // 7. 批量创建节点
    {
        uint32_t node_list_size = node_list.size();
        mg_list *mgl_node_list = mg_list_make_empty(node_list_size);
        int nodeidx = 0;
        for (auto node : node_list) {
            mg_map *mgm_node = mg_map_make_empty(2 + node->properties.size());
            mg_map_append(mgm_node, mg_string_make("W"), mg_value_make_integer(nodeidx));
            mg_list *mgl_labels = mg_list_make_empty((uint32_t)node->labels.size());
            for (const std::string &lbl : node->labels) {
                mg_list_append(mgl_labels, mg_value_make_string(lbl.c_str()));
            }
            mg_map_append(mgm_node, mg_string_make("X"), mg_value_make_list(mgl_labels));
            for (auto [propkey, propval] : node->properties) {
                const char propkey_str[2] = {propkey, 0};
                mg_map_append(mgm_node, mg_string_make(propkey_str), propval);
            }
            mg_list_append(mgl_node_list, mg_value_make_map(mgm_node));
            nodeidx++;
        }
        mg_map *qparams = mg_map_make_empty(1);
        mg_map_append(qparams, mg_string_make("Y"), mg_value_make_list(mgl_node_list));
        // ... 将 node_list 中的所有 Node 对象打包成 mg_list ..
        conn.execute_bolt(
            "UNWIND $Y AS Z "
            "CALL "
            "apoc.create.node(Z.X,{a:Z.a,b:Z.b,c:Z.c,d:Z.d,e:Z.e,f:Z.f,g:Z.g,h:Z.h,i:Z.i,j:Z.j,k:Z.k,l:Z.l,m:Z.m,n:Z.n,"
            "o:Z.o,p:Z.p,q:Z.q,r:Z.r,s:Z.s,t:Z.t,u:Z.u,v:Z.v,w:Z.w,x:Z.x,y:Z.y,z:Z.z,A:Z.A,B:Z.B,C:Z.C,D:Z.D,E:Z.E,F:Z."
            "F,G:Z.G,H:Z.H,I:Z.I,J:Z.J,K:Z.K,L:Z.L,M:Z.M,N:Z.N,O:Z.O,P:Z.P,Q:Z.Q}) "
            "YIELD node RETURN id(node) ORDER BY Z.W ",
            qparams);
        mg_map_destroy(qparams);
        // ... 获取返回的ID，并更新本地Node对象的id成员和全局ctx.ptr2nodeid映射 ...
    }
    {
        mg_result *result;

        for (auto node : node_list) {
            if (mg_session_fetch(conn.session, &result) == 1) {
                const mg_list *mgl_nodeid = mg_result_row(result);
                const uint32_t mgl_nodeid_length = mg_list_size(mgl_nodeid);
                assert(mgl_nodeid_length == 1);
                int64_t nodeid = mg_value_integer(mg_list_at(mgl_nodeid, 0));
                node->id = nodeid;
            } else {
                myerror(std::format("mg_session_fetch失败，错误信息如下：{}", mg_session_error(conn.session)));
            }
        }
        if (mg_session_fetch(conn.session, &result) != 0) {
            myerror(std::format("mg_session_fetch失败，错误信息如下：{}", mg_session_error(conn.session)));
        }
    }

#ifdef _DEBUG
    printf("\nupdated_entity_list : %llu\n", updated_entity_list.size());
    for (const auto &entity_ptr : updated_entity_list) {
        printf("%s, 0x%x\n", ((ENTITY *)entity_ptr)->type_name(), (long)entity_ptr);
    }
    printf("\nnew_created_entity_list : %llu\n", created_or_updated_entity_list.size() - updated_entity_list.size());

#endif

    // 所有指向update前旧节点的节点（ctx.ptr2nodeid.at(ptr)）都要再创建一条指向update后新节点（ptr2node.at(ptr)->id）的边
    {
        uint32_t updated_entity_list_size = updated_entity_list.size();
        mg_list *mgl_param_list = mg_list_make_empty(updated_entity_list_size);
        for (auto ptr : updated_entity_list) {
            mg_list *mgl_ptr_list = mg_list_make_empty(2);
            mg_list_append(mgl_ptr_list, mg_value_make_integer(ctx.ptr2nodeid.at(ptr)));
            mg_list_append(mgl_ptr_list, mg_value_make_integer(ptr2node.at(ptr)->id));
            mg_list_append(mgl_param_list, mg_value_make_list(mgl_ptr_list));
        }
        mg_map *qparams = mg_map_make_empty(1);
        mg_map_append(qparams, mg_string_make("Y"), mg_value_make_list(mgl_param_list));
        conn.execute_bolt(
            "UNWIND $Y AS t "
            "MATCH (n)<-[r]-(m) WHERE id(n)=t[0] AND NOT ('generation' IN labels(m)) WITH collect(r) AS p,t "
            "MATCH (s) WHERE id(s)=t[1] WITH s,p "
            "UNWIND p AS q "
            "CALL apoc.create.relationship(startNode(q),type(q),properties(q),s)  "
            "YIELD rel WITH count(*) AS cnt RETURN cnt ",
            qparams);
        mg_map_destroy(qparams);
        conn.discard_all_results();
    }

    // 更新节点id对照表
    for (const auto &[ptr, node] : ptr2node) {
        ctx.ptr2nodeid[ptr] = node->id;
    }

    for (const auto &entity_ptr : deleted_entity_list) {
        deleted_entity_nodeid_list.push_back(ctx.ptr2nodeid.at(entity_ptr));
    }

    // 将part与generation节点相连
    {
        Relationship2 *r = new Relationship2(partnodeid, generationnode->id);
        r->label = "part_generation_ptr";
        r->properties['a'] = mg_value_make_string("part_generation_ptr");
        r->properties['b'] = mg_value_make_integer(partnodenextgeneration);
        relationship_list.push_back(r);
    }

    // 将新的generation节点与上一个generation节点相连
    if (partnodenextgeneration > 1) {
        Relationship2 *r = new Relationship2(generationnode->id, curgenerationnodeid);
        r->label = "generation_prev_ptr";
        r->properties['a'] = mg_value_make_string("generation_prev_ptr");
        relationship_list.push_back(r);
    }

    // 创建generation_old_ptr边
    for (const auto &nodeid : updated_entity_nodeid_list) {
        Relationship2 *r = new Relationship2(generationnode->id, nodeid);
        r->label = "generation_old_ptr";
        r->properties['a'] = mg_value_make_string("generation_old_ptr");
        relationship_list.push_back(r);
    }
    for (const auto &nodeid : deleted_entity_nodeid_list) {
        Relationship2 *r = new Relationship2(generationnode->id, nodeid);
        r->label = "generation_old_ptr";
        r->properties['a'] = mg_value_make_string("generation_old_ptr");
        relationship_list.push_back(r);
    }

    for (const auto &entity_ptr : created_or_updated_entity_list) {
        {
            int64_t b = ctx.ptr2nodeid.at(entity_ptr);
            Relationship2 *r = new Relationship2(generationnode->id, b);
            r->label = "generation_new_ptr";
            r->properties['a'] = mg_value_make_string("generation_new_ptr");
            relationship_list.push_back(r);
        }
        switch (entity_ptr->identity(1)) {
        case BODY_ID: {
            BODY *ptr = (BODY *)entity_ptr;
            {
                ENTITY *__tmp_ptr = ptr->lump();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "body_lump_ptr";
                    r->properties['a'] = mg_value_make_string("body_lump_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->wire();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "body_wire_ptr";
                    r->properties['a'] = mg_value_make_string("body_wire_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->transform();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "body_transform_ptr";
                    r->properties['a'] = mg_value_make_string("body_transform_ptr");
                    relationship_list.push_back(r);
                }
            }
        } break;
        case LUMP_ID: {
            LUMP *ptr = (LUMP *)entity_ptr;
            {
                ENTITY *__tmp_ptr = ptr->next();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "lump_next_ptr";
                    r->properties['a'] = mg_value_make_string("lump_next_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->shell();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "lump_shell_ptr";
                    r->properties['a'] = mg_value_make_string("lump_shell_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->body();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "lump_body_ptr";
                    r->properties['a'] = mg_value_make_string("lump_body_ptr");
                    relationship_list.push_back(r);
                }
            }
        } break;
        case SHELL_ID: {
            SHELL *ptr = (SHELL *)entity_ptr;
            {
                ENTITY *__tmp_ptr = ptr->next();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "shell_next_ptr";
                    r->properties['a'] = mg_value_make_string("shell_next_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->subshell();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "shell_subshell_ptr";
                    r->properties['a'] = mg_value_make_string("shell_subshell_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->face();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "shell_face_ptr";
                    r->properties['a'] = mg_value_make_string("shell_face_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->wire();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "shell_wire_ptr";
                    r->properties['a'] = mg_value_make_string("shell_wire_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->lump();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "shell_lump_ptr";
                    r->properties['a'] = mg_value_make_string("shell_lump_ptr");
                    relationship_list.push_back(r);
                }
            }
        } break;
        case SUBSHELL_ID: {
            SUBSHELL *ptr = (SUBSHELL *)entity_ptr;
            {
                ENTITY *__tmp_ptr = ptr->parent();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "subshell_parent_ptr";
                    r->properties['a'] = mg_value_make_string("subshell_parent_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->sibling();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "subshell_sibling_ptr";
                    r->properties['a'] = mg_value_make_string("subshell_sibling_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->child();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "subshell_child_ptr";
                    r->properties['a'] = mg_value_make_string("subshell_child_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->face();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "subshell_face_ptr";
                    r->properties['a'] = mg_value_make_string("subshell_face_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->wire();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "subshell_wire_ptr";
                    r->properties['a'] = mg_value_make_string("subshell_wire_ptr");
                    relationship_list.push_back(r);
                }
            }
        } break;
        case WIRE_ID: {
            WIRE *ptr = (WIRE *)entity_ptr;
            {
                ENTITY *__tmp_ptr = ptr->next();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "wire_next_ptr";
                    r->properties['a'] = mg_value_make_string("wire_next_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->coedge();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "wire_coedge_ptr";
                    r->properties['a'] = mg_value_make_string("wire_coedge_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->owner();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "wire_owner_ptr";
                    r->properties['a'] = mg_value_make_string("wire_owner_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->subshell();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "wire_subshell_ptr";
                    r->properties['a'] = mg_value_make_string("wire_subshell_ptr");
                    relationship_list.push_back(r);
                }
            }
        } break;
        case FACE_ID: {
            FACE *ptr = (FACE *)entity_ptr;
            {
                ENTITY *__tmp_ptr = ptr->next();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "face_next_ptr";
                    r->properties['a'] = mg_value_make_string("face_next_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->loop();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "face_loop_ptr";
                    r->properties['a'] = mg_value_make_string("face_loop_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->shell();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "face_shell_ptr";
                    r->properties['a'] = mg_value_make_string("face_shell_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->subshell();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "face_subshell_ptr";
                    r->properties['a'] = mg_value_make_string("face_subshell_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->geometry();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "face_geometry_ptr";
                    r->properties['a'] = mg_value_make_string("face_geometry_ptr");
                    relationship_list.push_back(r);
                }
            }
        } break;
        case LOOP_ID: {
            LOOP *ptr = (LOOP *)entity_ptr;
            {
                ENTITY *__tmp_ptr = ptr->next();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "loop_next_ptr";
                    r->properties['a'] = mg_value_make_string("loop_next_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->start();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "loop_start_ptr";
                    r->properties['a'] = mg_value_make_string("loop_start_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->face();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "loop_face_ptr";
                    r->properties['a'] = mg_value_make_string("loop_face_ptr");
                    relationship_list.push_back(r);
                }
            }
        } break;
        case COEDGE_ID: {
            COEDGE *ptr = (COEDGE *)entity_ptr;
            {
                ENTITY *__tmp_ptr = ptr->next();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "coedge_next_ptr";
                    r->properties['a'] = mg_value_make_string("coedge_next_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->previous();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "coedge_previous_ptr";
                    r->properties['a'] = mg_value_make_string("coedge_previous_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->partner();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "coedge_partner_ptr";
                    r->properties['a'] = mg_value_make_string("coedge_partner_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->edge();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "coedge_edge_ptr";
                    r->properties['a'] = mg_value_make_string("coedge_edge_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->owner();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "coedge_owner_ptr";
                    r->properties['a'] = mg_value_make_string("coedge_owner_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->geometry();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "coedge_geometry_ptr";
                    r->properties['a'] = mg_value_make_string("coedge_geometry_ptr");
                    relationship_list.push_back(r);
                }
            }
        } break;
        case EDGE_ID: {
            EDGE *ptr = (EDGE *)entity_ptr;
            {
                ENTITY *__tmp_ptr = ptr->start();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "edge_start_ptr";
                    r->properties['a'] = mg_value_make_string("edge_start_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->end();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "edge_end_ptr";
                    r->properties['a'] = mg_value_make_string("edge_end_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->coedge();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "edge_coedge_ptr";
                    r->properties['a'] = mg_value_make_string("edge_coedge_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->geometry();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "edge_geometry_ptr";
                    r->properties['a'] = mg_value_make_string("edge_geometry_ptr");
                    relationship_list.push_back(r);
                }
            }
        } break;
        case VERTEX_ID: {
            VERTEX *ptr = (VERTEX *)entity_ptr;
            {
                ENTITY *__tmp_ptr = ptr->edge();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "vertex_edge_ptr";
                    r->properties['a'] = mg_value_make_string("vertex_edge_ptr");
                    relationship_list.push_back(r);
                }
            }
            {
                ENTITY *__tmp_ptr = ptr->geometry();
                if (__tmp_ptr != nullptr) {
                    int64_t a = ctx.ptr2nodeid.at(ptr);
                    int64_t b = ctx.ptr2nodeid.at(__tmp_ptr);
                    Relationship2 *r = new Relationship2(a, b);
                    r->label = "vertex_geometry_ptr";
                    r->properties['a'] = mg_value_make_string("vertex_geometry_ptr");
                    relationship_list.push_back(r);
                }
            }
        } break;
        default: {
            // PATTERN, ATTRIB等其他继承于ENTITY的实体
        } break;
        }
    }

    {
        uint32_t rel_list_size = relationship_list.size();
        mg_list *mgl_rel_list = mg_list_make_empty(rel_list_size);
        for (auto rel : relationship_list) {
            mg_map *mgm_rel = mg_map_make_empty(3 + rel->properties.size());
            mg_map_append(mgm_rel, mg_string_make("U"), mg_value_make_integer(rel->uid));
            mg_map_append(mgm_rel, mg_string_make("V"), mg_value_make_integer(rel->vid));
            mg_map_append(mgm_rel, mg_string_make("T"), mg_value_make_string(rel->label.c_str()));
            for (auto [propkey, propval] : rel->properties) {
                const char propkey_str[2] = {propkey, 0};
                mg_map_append(mgm_rel, mg_string_make(propkey_str), propval);
            }
            mg_list_append(mgl_rel_list, mg_value_make_map(mgm_rel));
        }
        mg_map *qparams = mg_map_make_empty(1);
        mg_map_append(qparams, mg_string_make("Y"), mg_value_make_list(mgl_rel_list));
        conn.execute_bolt("UNWIND $Y AS Z "
                          "MATCH (W) WHERE id(W) = Z.U "
                          "MATCH (X) WHERE id(X) = Z.V "
                          "CALL apoc.create.relationship(W,Z.T,{a:Z.a,b:Z.b},X) "
                          "YIELD rel WITH count(*) AS cnt RETURN cnt ",
                          qparams);
        mg_map_destroy(qparams);
        conn.discard_all_results();
    }

    // 设置part的当前版本（part节点的generation属性）
    qparams = mg_map_make_empty(3);
    mg_map_append(qparams, mg_string_make("A"), mg_value_make_integer(partnodeid));
    mg_map_append(qparams, mg_string_make("B"), mg_value_make_integer(partnodenextgeneration));
    conn.execute_bolt("MATCH (n) WHERE id(n)=$A SET n.c=$B SET n.d=$B", qparams);
    mg_map_destroy(qparams);
    conn.discard_all_results();

    for (auto node : node_list) {
        delete node;
    }
    for (auto rel : relationship_list) {
        delete rel;
    }
}
