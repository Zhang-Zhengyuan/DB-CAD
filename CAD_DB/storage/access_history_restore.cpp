#include "access_internal.hxx"

void api_restore_neo4j(const Neo4jPart &conn, int generation_id, IncrementalContext &ctx) {
    api_delete_history(); // 删除默认历史流及其下属公告上的所有实体
    ctx.lastsave_ds = nullptr;
    ctx.ptr2nodeid.clear();

    std::unordered_map<int64_t, void *> id2ptr;

    mg_map *qparams = mg_map_make_empty(2);
    mg_map_append(qparams, mg_string_make("A"), mg_value_make_string(conn.partname.c_str()));
    mg_map_append(qparams, mg_string_make("B"), mg_value_make_integer(generation_id));
    conn.execute_bolt("MATCH (q:part {a:'part',b:$A})-[r:part_generation_ptr {a:'part_generation_ptr',b:$B}]->(n) "
                      "SET q.d=$B WITH n "
                      "CALL apoc.path.subgraphAll(n, {minLevel:0,relationshipFilter: "
                      "'<'|generation_prev_ptr>|generation_new_ptr>|generation_old_ptr>'}) "
                      "YIELD relationships AS y "
                      "WITH [z IN y WHERE type(z)='generation_new_ptr' | endNode(z)] AS F,[z IN y WHERE "
                      "type(z)='generation_old_ptr' | endNode(z)] AS G "
                      "WITH [x IN F WHERE NOT (x IN G)] AS H "
                      "CALL apoc.algo.cover(H) YIELD rel RETURN H,collect(rel) ",
                      qparams);
    mg_map_destroy(qparams);

    mg_result *result;
    int rows_cnt = 0;
    int status;
    while (1) {
        status = mg_session_fetch(conn.session, &result);
        if (status == 1) {
            rows_cnt++;
            const mg_list *noderellist = mg_result_row(result);
            const uint32_t noderellist_length = mg_list_size(noderellist);
            assert(noderellist_length == 2);
            {
                const mg_value *nodelist_value = mg_list_at(noderellist, 0);
                assert(mg_value_get_type(nodelist_value) == MG_VALUE_TYPE_LIST);
                const mg_list *nodelist = mg_value_list(nodelist_value);
                const uint32_t nodelist_size = mg_list_size(nodelist);
                for (uint32_t i = 0; i < nodelist_size; i++) {
                    const mg_value *node_value = mg_list_at(nodelist, i);
                    assert(mg_value_get_type(node_value) == MG_VALUE_TYPE_NODE);
                    const mg_node *node = mg_value_node(node_value);
                    int64_t node_id = mg_node_id(node);
                    const mg_map *node_properties = mg_node_properties(node);
                    const mg_string *node_typename_mgs = mg_value_string(mg_map_at(node_properties, "a"));
                    std::string node_typename(mg_string_data(node_typename_mgs), mg_string_size(node_typename_mgs));
                    if (node_typename == "part") {
                        continue;
                    }
                    switch (AccessUtils::Restore::Neo4jNode_str2enum.at(node_typename)) {
                    case AccessUtils::Restore::Neo4jNode::body: {
                        BODY *body = nullptr;
                        api_body(body);
                        id2ptr[node_id] = body;
                        ctx.ptr2nodeid[body] = node_id;
                    } break;
                    case AccessUtils::Restore::Neo4jNode::lump: {
                        LUMP *lump = nullptr;
                        API_BEGIN;
                        lump = ACIS_NEW LUMP();
                        API_END;
                        id2ptr[node_id] = lump;
                        ctx.ptr2nodeid[lump] = node_id;
                    } break;
                    case AccessUtils::Restore::Neo4jNode::shell: {
                        SHELL *shell = nullptr;
                        API_BEGIN;
                        shell = ACIS_NEW SHELL();
                        API_END;
                        id2ptr[node_id] = shell;
                        ctx.ptr2nodeid[shell] = node_id;
                    } break;
                    case AccessUtils::Restore::Neo4jNode::subshell: {
                        SUBSHELL *subshell = nullptr;
                        API_BEGIN;
                        subshell = ACIS_NEW SUBSHELL();
                        API_END;
                        id2ptr[node_id] = subshell;
                        ctx.ptr2nodeid[subshell] = node_id;
                    } break;
                    case AccessUtils::Restore::Neo4jNode::face: {
                        FACE *face = nullptr;
                        API_BEGIN;
                        face = ACIS_NEW FACE();
                        API_END;
                        face->set_sense(mg_value_integer(mg_map_at(node_properties, "b")));
                        int sides_data = mg_value_integer(mg_map_at(node_properties, "c"));
                        face->set_sides(sides_data);
                        if (sides_data == 1) {
                            face->set_cont(mg_value_integer(mg_map_at(node_properties, "d")));
                        }
                        id2ptr[node_id] = face;
                        ctx.ptr2nodeid[face] = node_id;
                    } break;
                    case AccessUtils::Restore::Neo4jNode::loop: {
                        LOOP *loop = nullptr;
                        API_BEGIN;
                        loop = ACIS_NEW LOOP();
                        API_END;
                        id2ptr[node_id] = loop;
                        ctx.ptr2nodeid[loop] = node_id;
                    } break;
                    case AccessUtils::Restore::Neo4jNode::wire: {
                        WIRE *wire = nullptr;
                        API_BEGIN;
                        wire = ACIS_NEW WIRE();
                        API_END;
                        wire->set_cont(mg_value_integer(mg_map_at(node_properties, "b")));
                        id2ptr[node_id] = wire;
                        ctx.ptr2nodeid[wire] = node_id;
                    } break;
                    case AccessUtils::Restore::Neo4jNode::coedge: {
                        COEDGE *coedge = nullptr;
                        API_BEGIN;
                        coedge = ACIS_NEW COEDGE();
                        API_END;
                        coedge->set_sense(mg_value_integer(mg_map_at(node_properties, "b")));
                        id2ptr[node_id] = coedge;
                        ctx.ptr2nodeid[coedge] = node_id;
                    } break;
                    case AccessUtils::Restore::Neo4jNode::edge: {
                        EDGE *edge = nullptr;
                        API_BEGIN;
                        edge = ACIS_NEW EDGE();
                        API_END;
                        edge->set_sense(mg_value_integer(mg_map_at(node_properties, "b")));
                        id2ptr[node_id] = edge;
                        ctx.ptr2nodeid[edge] = node_id;
                    } break;
                    case AccessUtils::Restore::Neo4jNode::vertex: {
                        VERTEX *vertex = nullptr;
                        API_BEGIN;
                        vertex = ACIS_NEW VERTEX();
                        API_END;
                        id2ptr[node_id] = vertex;
                        ctx.ptr2nodeid[vertex] = node_id;
                    } break;
                    case AccessUtils::Restore::Neo4jNode::transform: {
                        SPAmatrix affine_part =
                            AccessUtils::Restore::parsemglist_SPAmatrix(mg_value_list(mg_map_at(node_properties, "b")));
                        SPAvector translation_part =
                            AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "c")));
                        double scaling_part = mg_value_float(mg_map_at(node_properties, "d"));
                        int rotate_flag = mg_value_integer(mg_map_at(node_properties, "e"));
                        int reflect_flag = mg_value_integer(mg_map_at(node_properties, "f"));
                        int shear_flag = mg_value_integer(mg_map_at(node_properties, "g"));
                        SPAtransf transform_data(affine_part, translation_part, scaling_part, rotate_flag, reflect_flag,
                                                 shear_flag);
                        TRANSFORM *transform = nullptr;
                        API_BEGIN;
                        transform = ACIS_NEW TRANSFORM(transform_data);
                        API_END;
                        id2ptr[node_id] = transform;
                        ctx.ptr2nodeid[transform] = node_id;
                    } break;
                    case AccessUtils::Restore::Neo4jNode::apoint: {
                        SPAposition coords_data = AccessUtils::Restore::parsemglist_SPAposition(
                            mg_value_list(mg_map_at(node_properties, "b")), 3);
                        APOINT *point = nullptr;
                        API_BEGIN;
                        point = ACIS_NEW APOINT(coords_data);
                        API_END;
                        id2ptr[node_id] = point;
                        ctx.ptr2nodeid[point] = node_id;
                    } break;
                    case AccessUtils::Restore::Neo4jNode::straight_curve: {
                        SPAposition root_point = AccessUtils::Restore::parsemglist_SPAposition(
                            mg_value_list(mg_map_at(node_properties, "c")), 3);
                        SPAvector direction =
                            AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "d")));
                        straight *def = ACIS_NEW straight(root_point, normalise(direction));
                        def->gme_set_param_scale(direction.len());
                        SPAinterval subset_range = AccessUtils::Restore::parsemglist_SPAinterval(
                            mg_value_list(mg_map_at(node_properties, "b")));
                        def->gme_set_subset_range(subset_range);
                        STRAIGHT *straight_curve = nullptr;
                        API_BEGIN;
                        straight_curve = ACIS_NEW STRAIGHT(*def);
                        API_END;
                        ACIS_DELETE def;
                        id2ptr[node_id] = straight_curve;
                        ctx.ptr2nodeid[straight_curve] = node_id;
                    } break;
                    case AccessUtils::Restore::Neo4jNode::ellipse_curve: {
                        SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(
                            mg_value_list(mg_map_at(node_properties, "c")), 3);
                        SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(
                            mg_value_list(mg_map_at(node_properties, "d")));
                        SPAvector major_axis =
                            AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "e")));
                        double radius_ratio = mg_value_float(mg_map_at(node_properties, "f"));
                        ellipse *def = ACIS_NEW ellipse(centre, normal, major_axis, radius_ratio);
                        SPAinterval subset_range = AccessUtils::Restore::parsemglist_SPAinterval(
                            mg_value_list(mg_map_at(node_properties, "b")));
                        def->gme_set_subset_range(subset_range);
                        ELLIPSE *ellipse_curve = nullptr;
                        API_BEGIN;
                        ellipse_curve = ACIS_NEW ELLIPSE(*def);
                        API_END;
                        ACIS_DELETE def;
                        id2ptr[node_id] = ellipse_curve;
                        ctx.ptr2nodeid[ellipse_curve] = node_id;
                    } break;
                    case AccessUtils::Restore::Neo4jNode::helix_curve: {
                        SPAposition axis_root = AccessUtils::Restore::parsemglist_SPAposition(
                            mg_value_list(mg_map_at(node_properties, "c")), 3);
                        SPAunit_vector axis_dir = AccessUtils::Restore::parsemglist_SPAunit_vector(
                            mg_value_list(mg_map_at(node_properties, "d")));
                        SPAvector start_disp =
                            AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "e")));
                        double pitch = mg_value_float(mg_map_at(node_properties, "f"));
                        int handedness = mg_value_integer(mg_map_at(node_properties, "g"));
                        double par_scaling = mg_value_float(mg_map_at(node_properties, "h"));
                        double taper = mg_value_float(mg_map_at(node_properties, "i"));
                        SPAinterval helix_range = AccessUtils::Restore::parsemglist_SPAinterval(
                            mg_value_list(mg_map_at(node_properties, "j")));
                        helix *def = ACIS_NEW helix(axis_root, axis_dir, start_disp, pitch, handedness, helix_range,
                                                    par_scaling, taper);
                        SPAinterval subset_range = AccessUtils::Restore::parsemglist_SPAinterval(
                            mg_value_list(mg_map_at(node_properties, "b")));
                        def->gme_set_subset_range(subset_range);
                        HELIX *helix_curve = nullptr;
                        API_BEGIN;
                        helix_curve = ACIS_NEW HELIX(*def);
                        API_END;
                        ACIS_DELETE def;
                        id2ptr[node_id] = helix_curve;
                        ctx.ptr2nodeid[helix_curve] = node_id;
                    } break;
                    case AccessUtils::Restore::Neo4jNode::plane_surface: {
                        SPAposition root_point = AccessUtils::Restore::parsemglist_SPAposition(
                            mg_value_list(mg_map_at(node_properties, "c")), 3);
                        SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(
                            mg_value_list(mg_map_at(node_properties, "d")));
                        SPAvector u_deriv =
                            AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "e")));
                        plane *def = ACIS_NEW plane(root_point, normal, u_deriv);
                        def->reverse_v = mg_value_integer(mg_map_at(node_properties, "f"));
                        def->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(
                            mg_value_list(mg_map_at(node_properties, "b"))));
                        PLANE *plane_surface = nullptr;
                        API_BEGIN;
                        plane_surface = ACIS_NEW PLANE(*def);
                        API_END;
                        ACIS_DELETE def;
                        id2ptr[node_id] = plane_surface;
                        ctx.ptr2nodeid[plane_surface] = node_id;
                    } break;
                    case AccessUtils::Restore::Neo4jNode::sphere_surface: {
                        SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(
                            mg_value_list(mg_map_at(node_properties, "c")), 3);
                        double radius = mg_value_float(mg_map_at(node_properties, "d"));
                        sphere *def = ACIS_NEW sphere(centre, radius);
                        def->uv_oridir = AccessUtils::Restore::parsemglist_SPAunit_vector(
                            mg_value_list(mg_map_at(node_properties, "e")));
                        def->pole_dir = AccessUtils::Restore::parsemglist_SPAunit_vector(
                            mg_value_list(mg_map_at(node_properties, "f")));
                        def->reverse_v = mg_value_integer(mg_map_at(node_properties, "g"));
                        def->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(
                            mg_value_list(mg_map_at(node_properties, "b"))));
                        SPHERE *sphere_surface = nullptr;
                        API_BEGIN;
                        sphere_surface = ACIS_NEW SPHERE(*def);
                        API_END;
                        ACIS_DELETE def;
                        id2ptr[node_id] = sphere_surface;
                        ctx.ptr2nodeid[sphere_surface] = node_id;
                    } break;
                    case AccessUtils::Restore::Neo4jNode::torus_surface: {
                        SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(
                            mg_value_list(mg_map_at(node_properties, "c")), 3);
                        SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(
                            mg_value_list(mg_map_at(node_properties, "d")));
                        double major_radius = mg_value_float(mg_map_at(node_properties, "e"));
                        double minor_radius = mg_value_float(mg_map_at(node_properties, "f"));
                        torus *def = ACIS_NEW torus(centre, normal, major_radius, minor_radius);
                        def->uv_oridir = AccessUtils::Restore::parsemglist_SPAunit_vector(
                            mg_value_list(mg_map_at(node_properties, "g")));
                        def->reverse_v = mg_value_integer(mg_map_at(node_properties, "h"));
                        def->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(
                            mg_value_list(mg_map_at(node_properties, "b"))));
                        TORUS *torus_surface = nullptr;
                        API_BEGIN;
                        torus_surface = ACIS_NEW TORUS(*def);
                        API_END;
                        ACIS_DELETE def;
                        id2ptr[node_id] = torus_surface;
                        ctx.ptr2nodeid[torus_surface] = node_id;
                    } break;
                    case AccessUtils::Restore::Neo4jNode::cone_surface: {
                        SPAposition base_centre = AccessUtils::Restore::parsemglist_SPAposition(
                            mg_value_list(mg_map_at(node_properties, "c")), 3);
                        SPAunit_vector base_normal = AccessUtils::Restore::parsemglist_SPAunit_vector(
                            mg_value_list(mg_map_at(node_properties, "d")));
                        SPAvector base_major_axis =
                            AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "e")));
                        double base_radius_ratio = mg_value_float(mg_map_at(node_properties, "f"));
                        ellipse *gem_base =
                            ACIS_NEW ellipse(base_centre, base_normal, base_major_axis, base_radius_ratio);
                        gem_base->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPAinterval(
                            mg_value_list(mg_map_at(node_properties, "g"))));
                        double sine_angle = mg_value_float(mg_map_at(node_properties, "h"));
                        double cosine_angle = mg_value_float(mg_map_at(node_properties, "i"));
                        cone *def = ACIS_NEW cone(*gem_base, sine_angle, cosine_angle);
                        ACIS_DELETE gem_base;
                        def->reverse_u = mg_value_integer(mg_map_at(node_properties, "j"));
                        def->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(
                            mg_value_list(mg_map_at(node_properties, "b"))));
                        CONE *cone_surface = nullptr;
                        API_BEGIN;
                        cone_surface = ACIS_NEW CONE(*def);
                        API_END;
                        ACIS_DELETE def;
                        id2ptr[node_id] = cone_surface;
                        ctx.ptr2nodeid[cone_surface] = node_id;
                    } break;
                    default: {
                        myerror("不支持的neo4j节点类型。");
                    } break;
                    }
                }
            }
            {
                const mg_value *rellist_value = mg_list_at(noderellist, 1);
                assert(mg_value_get_type(rellist_value) == MG_VALUE_TYPE_LIST);
                const mg_list *rellist = mg_value_list(rellist_value);
                const uint32_t rellist_size = mg_list_size(rellist);
                for (uint32_t i = 0; i < rellist_size; i++) {
                    const mg_value *rel_value = mg_list_at(rellist, i);
                    assert(mg_value_get_type(rel_value) == MG_VALUE_TYPE_RELATIONSHIP);
                    const mg_relationship *rel = mg_value_relationship(rel_value);
                    int64_t rel_startnode_id = mg_relationship_start_id(rel);
                    int64_t rel_endnode_id = mg_relationship_end_id(rel);
                    const mg_map *rel_properties = mg_relationship_properties(rel);
                    const mg_string *rel_typename_mgs = mg_value_string(mg_map_at(rel_properties, "a"));
                    std::string rel_typename(mg_string_data(rel_typename_mgs), mg_string_size(rel_typename_mgs));
                    if (rel_typename == "part_entity_ptr") {
                        continue;
                    }
                    switch (AccessUtils::Restore::Neo4jEdge_str2enum.at(rel_typename)) {
                    case AccessUtils::Restore::Neo4jEdge::body_lump_ptr: {
                        BODY *body = (BODY *)id2ptr.at(rel_startnode_id);
                        LUMP *lump = (LUMP *)id2ptr.at(rel_endnode_id);
                        body->set_lump(lump);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::body_wire_ptr: {
                        BODY *body = (BODY *)id2ptr.at(rel_startnode_id);
                        WIRE *wire = (WIRE *)id2ptr.at(rel_endnode_id);
                        body->set_wire(wire);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::body_transform_ptr: {
                        BODY *body = (BODY *)id2ptr.at(rel_startnode_id);
                        TRANSFORM *transform = (TRANSFORM *)id2ptr.at(rel_endnode_id);
                        body->set_transform(transform);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::lump_next_ptr: {
                        LUMP *lump = (LUMP *)id2ptr.at(rel_startnode_id);
                        LUMP *next = (LUMP *)id2ptr.at(rel_endnode_id);
                        lump->set_next(next);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::lump_shell_ptr: {
                        LUMP *lump = (LUMP *)id2ptr.at(rel_startnode_id);
                        SHELL *shell = (SHELL *)id2ptr.at(rel_endnode_id);
                        lump->set_shell(shell);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::lump_body_ptr: {
                        LUMP *lump = (LUMP *)id2ptr.at(rel_startnode_id);
                        BODY *body = (BODY *)id2ptr.at(rel_endnode_id);
                        lump->set_body(body);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::shell_next_ptr: {
                        SHELL *shell = (SHELL *)id2ptr.at(rel_startnode_id);
                        SHELL *next = (SHELL *)id2ptr.at(rel_endnode_id);
                        shell->set_next(next);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::shell_subshell_ptr: {
                        SHELL *shell = (SHELL *)id2ptr.at(rel_startnode_id);
                        SUBSHELL *subshell = (SUBSHELL *)id2ptr.at(rel_endnode_id);
                        shell->set_subshell(subshell);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::shell_face_ptr: {
                        SHELL *shell = (SHELL *)id2ptr.at(rel_startnode_id);
                        FACE *face = (FACE *)id2ptr.at(rel_endnode_id);
                        shell->set_face(face);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::shell_wire_ptr: {
                        SHELL *shell = (SHELL *)id2ptr.at(rel_startnode_id);
                        WIRE *wire = (WIRE *)id2ptr.at(rel_endnode_id);
                        shell->set_wire(wire);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::shell_lump_ptr: {
                        SHELL *shell = (SHELL *)id2ptr.at(rel_startnode_id);
                        LUMP *lump = (LUMP *)id2ptr.at(rel_endnode_id);
                        shell->set_lump(lump);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::subshell_parent_ptr: {
                        SUBSHELL *subshell = (SUBSHELL *)id2ptr.at(rel_startnode_id);
                        SUBSHELL *parent = (SUBSHELL *)id2ptr.at(rel_endnode_id);
                        subshell->set_parent(parent);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::subshell_sibling_ptr: {
                        SUBSHELL *subshell = (SUBSHELL *)id2ptr.at(rel_startnode_id);
                        SUBSHELL *sibling = (SUBSHELL *)id2ptr.at(rel_endnode_id);
                        subshell->set_sibling(sibling);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::subshell_child_ptr: {
                        SUBSHELL *subshell = (SUBSHELL *)id2ptr.at(rel_startnode_id);
                        SUBSHELL *child = (SUBSHELL *)id2ptr.at(rel_endnode_id);
                        subshell->set_child(child);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::subshell_face_ptr: {
                        SUBSHELL *subshell = (SUBSHELL *)id2ptr.at(rel_startnode_id);
                        FACE *face = (FACE *)id2ptr.at(rel_endnode_id);
                        subshell->set_face(face);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::subshell_wire_ptr: {
                        SUBSHELL *subshell = (SUBSHELL *)id2ptr.at(rel_startnode_id);
                        WIRE *wire = (WIRE *)id2ptr.at(rel_endnode_id);
                        subshell->set_wire(wire);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::wire_next_ptr: {
                        WIRE *wire = (WIRE *)id2ptr.at(rel_startnode_id);
                        WIRE *next = (WIRE *)id2ptr.at(rel_endnode_id);
                        wire->set_next(next);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::wire_coedge_ptr: {
                        WIRE *wire = (WIRE *)id2ptr.at(rel_startnode_id);
                        COEDGE *coedge = (COEDGE *)id2ptr.at(rel_endnode_id);
                        wire->set_coedge(coedge);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::wire_owner_ptr: {
                        WIRE *wire = (WIRE *)id2ptr.at(rel_startnode_id);
                        ENTITY *owner = (ENTITY *)id2ptr.at(rel_endnode_id);
                        wire->set_owner(owner);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::wire_subshell_ptr: {
                        WIRE *wire = (WIRE *)id2ptr.at(rel_startnode_id);
                        SUBSHELL *subshell = (SUBSHELL *)id2ptr.at(rel_endnode_id);
                        wire->set_subshell(subshell);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::face_next_ptr: {
                        FACE *face = (FACE *)id2ptr.at(rel_startnode_id);
                        FACE *next = (FACE *)id2ptr.at(rel_endnode_id);
                        face->set_next(next);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::face_loop_ptr: {
                        FACE *face = (FACE *)id2ptr.at(rel_startnode_id);
                        LOOP *loop = (LOOP *)id2ptr.at(rel_endnode_id);
                        face->set_loop(loop);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::face_shell_ptr: {
                        FACE *face = (FACE *)id2ptr.at(rel_startnode_id);
                        SHELL *shell = (SHELL *)id2ptr.at(rel_endnode_id);
                        face->set_shell(shell);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::face_subshell_ptr: {
                        FACE *face = (FACE *)id2ptr.at(rel_startnode_id);
                        SUBSHELL *subshell = (SUBSHELL *)id2ptr.at(rel_endnode_id);
                        face->set_subshell(subshell);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::face_geometry_ptr: {
                        FACE *face = (FACE *)id2ptr.at(rel_startnode_id);
                        SURFACE *geometry = (SURFACE *)id2ptr.at(rel_endnode_id);
                        face->set_geometry(geometry);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::loop_next_ptr: {
                        LOOP *loop = (LOOP *)id2ptr.at(rel_startnode_id);
                        LOOP *next = (LOOP *)id2ptr.at(rel_endnode_id);
                        loop->set_next(next);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::loop_start_ptr: {
                        LOOP *loop = (LOOP *)id2ptr.at(rel_startnode_id);
                        COEDGE *start = (COEDGE *)id2ptr.at(rel_endnode_id);
                        loop->set_start(start);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::loop_face_ptr: {
                        LOOP *loop = (LOOP *)id2ptr.at(rel_startnode_id);
                        FACE *face = (FACE *)id2ptr.at(rel_endnode_id);
                        loop->set_face(face);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::coedge_next_ptr: {
                        COEDGE *coedge = (COEDGE *)id2ptr.at(rel_startnode_id);
                        COEDGE *next = (COEDGE *)id2ptr.at(rel_endnode_id);
                        coedge->set_next(next);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::coedge_previous_ptr: {
                        COEDGE *coedge = (COEDGE *)id2ptr.at(rel_startnode_id);
                        COEDGE *previous = (COEDGE *)id2ptr.at(rel_endnode_id);
                        coedge->set_previous(previous);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::coedge_partner_ptr: {
                        COEDGE *coedge = (COEDGE *)id2ptr.at(rel_startnode_id);
                        COEDGE *partner = (COEDGE *)id2ptr.at(rel_endnode_id);
                        coedge->set_partner(partner);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::coedge_edge_ptr: {
                        COEDGE *coedge = (COEDGE *)id2ptr.at(rel_startnode_id);
                        EDGE *edge = (EDGE *)id2ptr.at(rel_endnode_id);
                        coedge->set_edge(edge);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::coedge_owner_ptr: {
                        COEDGE *coedge = (COEDGE *)id2ptr.at(rel_startnode_id);
                        ENTITY *owner = (ENTITY *)id2ptr.at(rel_endnode_id);
                        coedge->set_owner(owner);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::coedge_geometry_ptr: {
                        COEDGE *coedge = (COEDGE *)id2ptr.at(rel_startnode_id);
                        PCURVE *geometry = (PCURVE *)id2ptr.at(rel_endnode_id);
                        coedge->set_geometry(geometry);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::edge_start_ptr: {
                        EDGE *edge = (EDGE *)id2ptr.at(rel_startnode_id);
                        VERTEX *start = (VERTEX *)id2ptr.at(rel_endnode_id);
                        edge->set_start(start);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::edge_end_ptr: {
                        EDGE *edge = (EDGE *)id2ptr.at(rel_startnode_id);
                        VERTEX *end = (VERTEX *)id2ptr.at(rel_endnode_id);
                        edge->set_end(end);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::edge_coedge_ptr: {
                        EDGE *edge = (EDGE *)id2ptr.at(rel_startnode_id);
                        COEDGE *coedge = (COEDGE *)id2ptr.at(rel_endnode_id);
                        edge->set_coedge(coedge);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::edge_geometry_ptr: {
                        EDGE *edge = (EDGE *)id2ptr.at(rel_startnode_id);
                        CURVE *geometry = (CURVE *)id2ptr.at(rel_endnode_id);
                        edge->set_geometry(geometry);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::vertex_edge_ptr: {
                        VERTEX *vertex = (VERTEX *)id2ptr.at(rel_startnode_id);
                        EDGE *edge = (EDGE *)id2ptr.at(rel_endnode_id);
                        vertex->set_edge(edge);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::vertex_geometry_ptr: {
                        VERTEX *vertex = (VERTEX *)id2ptr.at(rel_startnode_id);
                        APOINT *geometry = (APOINT *)id2ptr.at(rel_endnode_id);
                        vertex->set_geometry(geometry);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::pcurve_ref_curve_ptr: {
                        PCURVE *pcurve = (PCURVE *)id2ptr.at(rel_startnode_id);
                        CURVE *ref_curve = (CURVE *)id2ptr.at(rel_endnode_id);
                        pcurve->set_def(ref_curve, pcurve->index(), 0, pcurve->offset());
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::pcurve_fit_ptr: {
                        PCURVE *p = (PCURVE *)id2ptr.at(rel_startnode_id);
                        par_cur *fit = (par_cur *)id2ptr.at(rel_endnode_id);
                        p->set_fit(fit);
                        fit->add_ref();
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::spline_surface_spl_ptr: {
                        SPLINE *spline_surface = (SPLINE *)id2ptr.at(rel_startnode_id);
                        spl_sur *spl = (spl_sur *)id2ptr.at(rel_endnode_id);
                        spline_surface->gme_set_spl(spl);
                        spl->add_ref();
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::intcurve_curve_fit_ptr: {
                        INTCURVE *intcurve_curve = (INTCURVE *)id2ptr.at(rel_startnode_id);
                        int_cur *fit = (int_cur *)id2ptr.at(rel_endnode_id);
                        intcurve_curve->gme_set_fit(fit);
                        fit->add_ref();
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::int_cur_surf1_spl_ptr: {
                        int_cur *ic = (int_cur *)id2ptr.at(rel_startnode_id);
                        spl_sur *surf1_spl = (spl_sur *)id2ptr.at(rel_endnode_id);
                        ic->set_surf1_spl(surf1_spl);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::int_cur_surf2_spl_ptr: {
                        int_cur *ic = (int_cur *)id2ptr.at(rel_startnode_id);
                        spl_sur *surf2_spl = (spl_sur *)id2ptr.at(rel_endnode_id);
                        ic->set_surf2_spl(surf2_spl);
                    } break;
                    case AccessUtils::Restore::Neo4jEdge::par_cur_surf_spl_ptr: {
                        par_cur *pc = (par_cur *)id2ptr.at(rel_startnode_id);
                        spl_sur *surf_spl = (spl_sur *)id2ptr.at(rel_endnode_id);
                        ((exp_par_cur *)pc)->gme_set_surf_spl(surf_spl);
                    } break;
                    default: {
                        myerror("不支持的neo4j边类型。");
                    } break;
                    }
                }
            }
        } else if (status == 0) {
            const mg_map *mgm = mg_result_summary(result);
            const uint32_t mgm_size = mg_map_size(mgm);
            for (uint32_t i = 0; i < mgm_size; i++) {
                const mg_string *itemkey = mg_map_key_at(mgm, i);
                const mg_value *itemvalue = mg_map_value_at(mgm, i);
            }
            break;
        } else {
            myerror(std::format("mg_session_fetch失败，错误信息如下：{}", mg_session_error(conn.session)));
        }
    }

    if (rows_cnt == 0) {
        myerror("恢复时指定的顶级实体图节点elementId没有找到对应的图节点！");
    }

    DELTA_STATE *thissave_ds; // 这次保存
    api_note_state(thissave_ds);
    ctx.lastsave_ds = thissave_ds;
}
