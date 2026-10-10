#include "access_internal.hxx"

void api_restore_entity_list_neo4j(const Neo4jPart &conn, const std::vector<int64_t> &id_list, ENTITY_LIST &entity_list,
                                   std::unordered_map<int64_t, void *> &id2ptr) {
    uint32_t id_list_size = id_list.size();
    for (uint32_t id_list_idx = 0; id_list_idx < id_list_size; id_list_idx++) {
        mg_map *qparams = mg_map_make_empty(1);
        mg_map_append(qparams, mg_string_make("d"), mg_value_make_integer(id_list[id_list_idx]));
        // relationshipFilter: '>' (OUTGOING) — 从 body 出发沿 body_lump / lump_shell / shell_face 等出边
        // 走到所有子拓扑节点（vertex / edge / loop / coedge / face / shell / lump ...）。
        // 原始代码用 '<' (INCOMING) 是反向方向，只能回到 part → body 那一条边，
        // 永远不会追到子实体，所以 restore 出来的 body 是"空壳"（lump/shell/face/edge 全为 0）。
        conn.execute_bolt("MATCH (n) WHERE id(n) = $d "
                          "CALL apoc.path.subgraphAll(n, {minLevel:0,relationshipFilter: '>'}) YIELD nodes AS x, "
                          "relationships AS y return x,y ",
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
                    DIAG_LOG("[restore-neo4j] subgraphAll returned nodes=%u\n", nodelist_size);
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
                            DIAG_LOG("[restore-body] node_id=%lld body=%p\n", (long long)node_id, (void *)body);
                        } break;
                        case AccessUtils::Restore::Neo4jNode::lump: {
                            LUMP *lump = nullptr;
                            API_BEGIN;
                            lump = ACIS_NEW LUMP();
                            API_END;
                            id2ptr[node_id] = lump;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::shell: {
                            SHELL *shell = nullptr;
                            API_BEGIN;
                            shell = ACIS_NEW SHELL();
                            API_END;
                            id2ptr[node_id] = shell;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::subshell: {
                            SUBSHELL *subshell = nullptr;
                            API_BEGIN;
                            subshell = ACIS_NEW SUBSHELL();
                            API_END;
                            id2ptr[node_id] = subshell;
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
                        } break;
                        case AccessUtils::Restore::Neo4jNode::loop: {
                            LOOP *loop = nullptr;
                            API_BEGIN;
                            loop = ACIS_NEW LOOP();
                            API_END;
                            id2ptr[node_id] = loop;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::wire: {
                            WIRE *wire = nullptr;
                            API_BEGIN;
                            wire = ACIS_NEW WIRE();
                            API_END;
                            wire->set_cont(mg_value_integer(mg_map_at(node_properties, "b")));
                            id2ptr[node_id] = wire;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::coedge: {
                            COEDGE *coedge = nullptr;
                            API_BEGIN;
                            coedge = ACIS_NEW COEDGE();
                            API_END;
                            coedge->set_sense(mg_value_integer(mg_map_at(node_properties, "b")));
                            id2ptr[node_id] = coedge;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::edge: {
                            EDGE *edge = nullptr;
                            API_BEGIN;
                            edge = ACIS_NEW EDGE();
                            API_END;
                            edge->set_sense(mg_value_integer(mg_map_at(node_properties, "b")));
                            id2ptr[node_id] = edge;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::vertex: {
                            VERTEX *vertex = nullptr;
                            API_BEGIN;
                            vertex = ACIS_NEW VERTEX();
                            API_END;
                            id2ptr[node_id] = vertex;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::transform: {
                            SPAmatrix affine_part = AccessUtils::Restore::parsemglist_SPAmatrix(
                                mg_value_list(mg_map_at(node_properties, "b")));
                            SPAvector translation_part = AccessUtils::Restore::parsemglist_SPAvector(
                                mg_value_list(mg_map_at(node_properties, "c")));
                            double scaling_part = mg_value_float(mg_map_at(node_properties, "d"));
                            int rotate_flag = mg_value_integer(mg_map_at(node_properties, "e"));
                            int reflect_flag = mg_value_integer(mg_map_at(node_properties, "f"));
                            int shear_flag = mg_value_integer(mg_map_at(node_properties, "g"));
                            SPAtransf transform_data(affine_part, translation_part, scaling_part, rotate_flag,
                                                     reflect_flag, shear_flag);
                            TRANSFORM *transform = nullptr;
                            API_BEGIN;
                            transform = ACIS_NEW TRANSFORM(transform_data);
                            API_END;
                            id2ptr[node_id] = transform;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::apoint: {
                            SPAposition coords_data = AccessUtils::Restore::parsemglist_SPAposition(
                                mg_value_list(mg_map_at(node_properties, "b")), 3);
                            APOINT *point = nullptr;
                            API_BEGIN;
                            point = ACIS_NEW APOINT(coords_data);
                            API_END;
                            id2ptr[node_id] = point;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::straight_curve: {
                            SPAposition root_point = AccessUtils::Restore::parsemglist_SPAposition(
                                mg_value_list(mg_map_at(node_properties, "c")), 3);
                            SPAvector direction = AccessUtils::Restore::parsemglist_SPAvector(
                                mg_value_list(mg_map_at(node_properties, "d")));
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
                        } break;
                        case AccessUtils::Restore::Neo4jNode::ellipse_curve: {
                            SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(
                                mg_value_list(mg_map_at(node_properties, "c")), 3);
                            SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                mg_value_list(mg_map_at(node_properties, "d")));
                            SPAvector major_axis = AccessUtils::Restore::parsemglist_SPAvector(
                                mg_value_list(mg_map_at(node_properties, "e")));
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
                        } break;
                        case AccessUtils::Restore::Neo4jNode::helix_curve: {
                            SPAposition axis_root = AccessUtils::Restore::parsemglist_SPAposition(
                                mg_value_list(mg_map_at(node_properties, "c")), 3);
                            SPAunit_vector axis_dir = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                mg_value_list(mg_map_at(node_properties, "d")));
                            SPAvector start_disp = AccessUtils::Restore::parsemglist_SPAvector(
                                mg_value_list(mg_map_at(node_properties, "e")));
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
                        } break;
                        case AccessUtils::Restore::Neo4jNode::intcurve_curve: {
                            intcurve *def = ACIS_NEW intcurve();
                            def->gme_set_rev(mg_value_integer(mg_map_at(node_properties, "c")));
                            def->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPAinterval(
                                mg_value_list(mg_map_at(node_properties, "b"))));
                            INTCURVE *intcurve_curve = nullptr;
                            API_BEGIN;
                            intcurve_curve = ACIS_NEW INTCURVE(*def);
                            API_END;
                            ACIS_DELETE def;
                            id2ptr[node_id] = intcurve_curve;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::int_cur: {
                            int cur_subtype = mg_value_integer(mg_map_at(node_properties, "b"));
                            int cur_rational_flag = mg_value_integer(mg_map_at(node_properties, "c"));
                            int cur_degree = mg_value_integer(mg_map_at(node_properties, "d"));
                            int cur_closed_periodic = mg_value_integer(mg_map_at(node_properties, "e"));
                            int cur_closed, cur_periodic;
                            if (cur_closed_periodic == 0) {
                                cur_closed = 0;
                                cur_periodic = 0;
                            } else if (cur_closed_periodic == 1) {
                                cur_closed = 1;
                                cur_periodic = 0;
                            } else if (cur_closed_periodic == 2) {
                                cur_closed = 1;
                                cur_periodic = 1;
                            }
                            int cur_knots_num = mg_value_integer(mg_map_at(node_properties, "f"));
                            const mg_list *knots_simplifier_mglist = mg_value_list(mg_map_at(node_properties, "g"));
                            const mg_list *ctrlpts_mglist = mg_value_list(mg_map_at(node_properties, "h"));
                            double cur_fitol = mg_value_float(mg_map_at(node_properties, "i"));
                            int surf1_type = mg_value_integer(mg_map_at(node_properties, "j"));
                            int surf2_type = mg_value_integer(mg_map_at(node_properties, "k"));
                            int pcur1_type = mg_value_integer(mg_map_at(node_properties, "l"));
                            int pcur2_type = mg_value_integer(mg_map_at(node_properties, "m"));
                            SPAinterval safe_range = AccessUtils::Restore::parsemglist_SPAinterval(
                                mg_value_list(mg_map_at(node_properties, "n")));
                            int cur_surface_tag = mg_value_integer(mg_map_at(node_properties, "o"));

                            std::deque<double> cur_knots;
                            for (int i = 0; i < cur_knots_num; i++) {
                                double knot = mg_value_float(mg_list_at(knots_simplifier_mglist, i * 2));
                                int knot_cnt = mg_value_float(mg_list_at(knots_simplifier_mglist, i * 2 + 1));
                                for (int j = 0; j < knot_cnt; j++)
                                    cur_knots.push_back(knot);
                            }
                            cur_knots.push_front(cur_knots.front());
                            cur_knots.push_back(cur_knots.back());

                            // control points
                            int cur_ctrl_points_num = cur_knots.size() - cur_degree - 1;
                            SPAposition *cur_ctrl_points = ACIS_NEW SPAposition[cur_ctrl_points_num];
                            double *cur_weights = nullptr;
                            if (cur_rational_flag) {
                                cur_weights = new double[cur_ctrl_points_num];
                            }
                            int cur_ctrl_points_idx = 0;
                            for (int i = 0; i < cur_ctrl_points_num; i++) {
                                SPAposition ctrl_point =
                                    SPAposition(mg_value_float(mg_list_at(ctrlpts_mglist, cur_ctrl_points_idx)),
                                                mg_value_float(mg_list_at(ctrlpts_mglist, cur_ctrl_points_idx + 1)),
                                                mg_value_float(mg_list_at(ctrlpts_mglist, cur_ctrl_points_idx + 2)));
                                cur_ctrl_points_idx += 3;
                                if (cur_rational_flag) {
                                    double weight = mg_value_float(mg_list_at(ctrlpts_mglist, cur_ctrl_points_idx));
                                    cur_ctrl_points_idx++;
                                    cur_weights[i] = weight;
                                }
                                cur_ctrl_points[i] = ctrl_point;
                            }

                            // create bs3_surface
                            bs3_curve bs_cur = bs3_curve_from_ctrlpts(
                                cur_degree, cur_rational_flag, cur_closed, cur_periodic, cur_ctrl_points_num,
                                cur_ctrl_points, cur_rational_flag ? cur_weights : nullptr, SPAresabs.value(),
                                cur_knots.size(), std::vector<double>(cur_knots.begin(), cur_knots.end()).data(),
                                SPAresnor.value());

                            if (cur_closed_periodic == 0) {
                                bs3_curve_set_open(bs_cur);
                            } else if (cur_closed_periodic == 1) {
                                bs3_curve_set_closed(bs_cur);
                            } else if (cur_closed_periodic == 2) {
                                bs3_curve_set_periodic(bs_cur);
                            }

                            ACIS_DELETE[] cur_ctrl_points;
                            if (cur_rational_flag) {
                                delete[] cur_weights;
                            }

                            // surf1
                            surface *surf1 = nullptr;
                            if (surf1_type == 2) {
                                SPAposition root_point = AccessUtils::Restore::parsemglist_SPAposition(
                                    mg_value_list(mg_map_at(node_properties, "q")), 3);
                                SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                    mg_value_list(mg_map_at(node_properties, "r")));
                                SPAvector u_deriv = AccessUtils::Restore::parsemglist_SPAvector(
                                    mg_value_list(mg_map_at(node_properties, "s")));
                                plane *ret_plane = ACIS_NEW plane(root_point, normal, u_deriv);
                                ret_plane->reverse_v = mg_value_integer(mg_map_at(node_properties, "t"));
                                ret_plane->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(
                                    mg_value_list(mg_map_at(node_properties, "p"))));
                                surf1 = ret_plane;
                            } else if (surf1_type == 3) {
                                SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(
                                    mg_value_list(mg_map_at(node_properties, "q")), 3);
                                double radius = mg_value_float(mg_map_at(node_properties, "r"));
                                sphere *ret_sphere = ACIS_NEW sphere(centre, radius);
                                ret_sphere->uv_oridir = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                    mg_value_list(mg_map_at(node_properties, "s")));
                                ret_sphere->pole_dir = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                    mg_value_list(mg_map_at(node_properties, "t")));
                                ret_sphere->reverse_v = mg_value_integer(mg_map_at(node_properties, "u"));
                                ret_sphere->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(
                                    mg_value_list(mg_map_at(node_properties, "p"))));
                                surf1 = ret_sphere;
                            } else if (surf1_type == 4) {
                                SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(
                                    mg_value_list(mg_map_at(node_properties, "q")), 3);
                                SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                    mg_value_list(mg_map_at(node_properties, "r")));
                                double major_radius = mg_value_float(mg_map_at(node_properties, "s"));
                                double minor_radius = mg_value_float(mg_map_at(node_properties, "t"));
                                torus *ret_torus = ACIS_NEW torus(centre, normal, major_radius, minor_radius);
                                ret_torus->uv_oridir = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                    mg_value_list(mg_map_at(node_properties, "u")));
                                ret_torus->reverse_v = mg_value_integer(mg_map_at(node_properties, "v"));
                                ret_torus->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(
                                    mg_value_list(mg_map_at(node_properties, "p"))));
                                surf1 = ret_torus;
                            } else if (surf1_type == 5) {
                                SPAposition base_centre = AccessUtils::Restore::parsemglist_SPAposition(
                                    mg_value_list(mg_map_at(node_properties, "q")), 3);
                                SPAunit_vector base_normal = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                    mg_value_list(mg_map_at(node_properties, "r")));
                                SPAvector base_major_axis = AccessUtils::Restore::parsemglist_SPAvector(
                                    mg_value_list(mg_map_at(node_properties, "s")));
                                double base_radius_ratio = mg_value_float(mg_map_at(node_properties, "t"));
                                ellipse *gem_base =
                                    ACIS_NEW ellipse(base_centre, base_normal, base_major_axis, base_radius_ratio);
                                gem_base->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPAinterval(
                                    mg_value_list(mg_map_at(node_properties, "u"))));
                                double sine_angle = mg_value_float(mg_map_at(node_properties, "v"));
                                double cosine_angle = mg_value_float(mg_map_at(node_properties, "w"));
                                cone *ret_cone = ACIS_NEW cone(*gem_base, sine_angle, cosine_angle);
                                ACIS_DELETE gem_base;
                                ret_cone->reverse_u = mg_value_integer(mg_map_at(node_properties, "x"));
                                ret_cone->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(
                                    mg_value_list(mg_map_at(node_properties, "p"))));
                                surf1 = ret_cone;
                            } else if (surf1_type == 1) {
                                spline *ret_spline = ACIS_NEW spline();
                                ret_spline->gme_set_rev(mg_value_integer(mg_map_at(node_properties, "q")));
                                ret_spline->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(
                                    mg_value_list(mg_map_at(node_properties, "p"))));
                                surf1 = ret_spline;
                            }

                            // surf2
                            surface *surf2 = nullptr;
                            if (surf2_type == 2) {
                                SPAposition root_point = AccessUtils::Restore::parsemglist_SPAposition(
                                    mg_value_list(mg_map_at(node_properties, "z")), 3);
                                SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                    mg_value_list(mg_map_at(node_properties, "A")));
                                SPAvector u_deriv = AccessUtils::Restore::parsemglist_SPAvector(
                                    mg_value_list(mg_map_at(node_properties, "B")));
                                plane *ret_plane = ACIS_NEW plane(root_point, normal, u_deriv);
                                ret_plane->reverse_v = mg_value_integer(mg_map_at(node_properties, "C"));
                                ret_plane->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(
                                    mg_value_list(mg_map_at(node_properties, "y"))));
                                surf2 = ret_plane;
                            } else if (surf2_type == 3) {
                                SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(
                                    mg_value_list(mg_map_at(node_properties, "z")), 3);
                                double radius = mg_value_float(mg_map_at(node_properties, "A"));
                                sphere *ret_sphere = ACIS_NEW sphere(centre, radius);
                                ret_sphere->uv_oridir = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                    mg_value_list(mg_map_at(node_properties, "B")));
                                ret_sphere->pole_dir = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                    mg_value_list(mg_map_at(node_properties, "C")));
                                ret_sphere->reverse_v = mg_value_integer(mg_map_at(node_properties, "D"));
                                ret_sphere->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(
                                    mg_value_list(mg_map_at(node_properties, "y"))));
                                surf2 = ret_sphere;
                            } else if (surf2_type == 4) {
                                SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(
                                    mg_value_list(mg_map_at(node_properties, "z")), 3);
                                SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                    mg_value_list(mg_map_at(node_properties, "A")));
                                double major_radius = mg_value_float(mg_map_at(node_properties, "B"));
                                double minor_radius = mg_value_float(mg_map_at(node_properties, "C"));
                                torus *ret_torus = ACIS_NEW torus(centre, normal, major_radius, minor_radius);
                                ret_torus->uv_oridir = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                    mg_value_list(mg_map_at(node_properties, "D")));
                                ret_torus->reverse_v = mg_value_integer(mg_map_at(node_properties, "E"));
                                ret_torus->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(
                                    mg_value_list(mg_map_at(node_properties, "y"))));
                                surf2 = ret_torus;
                            } else if (surf2_type == 5) {
                                SPAposition base_centre = AccessUtils::Restore::parsemglist_SPAposition(
                                    mg_value_list(mg_map_at(node_properties, "z")), 3);
                                SPAunit_vector base_normal = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                    mg_value_list(mg_map_at(node_properties, "A")));
                                SPAvector base_major_axis = AccessUtils::Restore::parsemglist_SPAvector(
                                    mg_value_list(mg_map_at(node_properties, "B")));
                                double base_radius_ratio = mg_value_float(mg_map_at(node_properties, "C"));
                                ellipse *gem_base =
                                    ACIS_NEW ellipse(base_centre, base_normal, base_major_axis, base_radius_ratio);
                                gem_base->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPAinterval(
                                    mg_value_list(mg_map_at(node_properties, "D"))));
                                double sine_angle = mg_value_float(mg_map_at(node_properties, "E"));
                                double cosine_angle = mg_value_float(mg_map_at(node_properties, "F"));
                                cone *ret_cone = ACIS_NEW cone(*gem_base, sine_angle, cosine_angle);
                                ACIS_DELETE gem_base;
                                ret_cone->reverse_u = mg_value_integer(mg_map_at(node_properties, "G"));
                                ret_cone->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(
                                    mg_value_list(mg_map_at(node_properties, "y"))));
                                surf2 = ret_cone;
                            } else if (surf2_type == 1) {
                                spline *ret_spline = ACIS_NEW spline();
                                ret_spline->gme_set_rev(mg_value_integer(mg_map_at(node_properties, "z")));
                                ret_spline->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(
                                    mg_value_list(mg_map_at(node_properties, "y"))));
                                surf2 = ret_spline;
                            }

                            // pcur1
                            bs2_curve pcur1 = nullptr;
                            if (pcur1_type != 0) {
                                int pcur1_degree = mg_value_integer(mg_map_at(node_properties, "H"));
                                int pcur1_closed_periodic = mg_value_integer(mg_map_at(node_properties, "I"));
                                int pcur1_closed, pcur1_periodic;
                                if (pcur1_closed_periodic == 0) {
                                    pcur1_closed = 0;
                                    pcur1_periodic = 0;
                                } else if (pcur1_closed_periodic == 1) {
                                    pcur1_closed = 1;
                                    pcur1_periodic = 0;
                                } else if (pcur1_closed_periodic == 2) {
                                    pcur1_closed = 1;
                                    pcur1_periodic = 1;
                                }
                                int pcur1_knots_num = mg_value_integer(mg_map_at(node_properties, "J"));
                                const mg_list *knots_simplifier_mglist = mg_value_list(mg_map_at(node_properties, "K"));
                                const mg_list *ctrlpts_mglist = mg_value_list(mg_map_at(node_properties, "L"));

                                std::deque<double> pcur1_knots;
                                for (int i = 0; i < pcur1_knots_num; i++) {
                                    double knot = mg_value_float(mg_list_at(knots_simplifier_mglist, i * 2));
                                    int knot_cnt = mg_value_float(mg_list_at(knots_simplifier_mglist, i * 2 + 1));
                                    for (int j = 0; j < knot_cnt; j++)
                                        pcur1_knots.push_back(knot);
                                }
                                pcur1_knots.push_front(pcur1_knots.front());
                                pcur1_knots.push_back(pcur1_knots.back());

                                // control points
                                int pcur1_ctrl_points_num = pcur1_knots.size() - pcur1_degree - 1;
                                SPAposition *pcur1_ctrl_points = ACIS_NEW SPAposition[pcur1_ctrl_points_num];
                                for (int i = 0; i < pcur1_ctrl_points_num; i++) {
                                    SPAposition ctrl_point =
                                        SPAposition(mg_value_float(mg_list_at(ctrlpts_mglist, i * 2)),
                                                    mg_value_float(mg_list_at(ctrlpts_mglist, i * 2 + 1)), 0);
                                    pcur1_ctrl_points[i] = ctrl_point;
                                }
                                // create bs2_curve
                                bs2_curve pcur1_bs2_cur_created = bs2_curve_from_ctrlpts(
                                    pcur1_degree, 0, pcur1_closed, pcur1_periodic, pcur1_ctrl_points_num,
                                    pcur1_ctrl_points, nullptr, SPAresabs.value(), pcur1_knots.size(),
                                    std::vector<double>(pcur1_knots.begin(), pcur1_knots.end()).data(),
                                    SPAresnor.value());
                                ACIS_DELETE[] pcur1_ctrl_points;
                                pcur1 = pcur1_bs2_cur_created;
                            }

                            // pcur2
                            bs2_curve pcur2 = nullptr;
                            if (pcur2_type != 0) {
                                int pcur2_degree = mg_value_integer(mg_map_at(node_properties, "M"));
                                int pcur2_closed_periodic = mg_value_integer(mg_map_at(node_properties, "N"));
                                int pcur2_closed, pcur2_periodic;
                                if (pcur2_closed_periodic == 0) {
                                    pcur2_closed = 0;
                                    pcur2_periodic = 0;
                                } else if (pcur2_closed_periodic == 1) {
                                    pcur2_closed = 1;
                                    pcur2_periodic = 0;
                                } else if (pcur2_closed_periodic == 2) {
                                    pcur2_closed = 1;
                                    pcur2_periodic = 1;
                                }
                                int pcur2_knots_num = mg_value_integer(mg_map_at(node_properties, "O"));
                                const mg_list *knots_simplifier_mglist = mg_value_list(mg_map_at(node_properties, "P"));
                                const mg_list *ctrlpts_mglist = mg_value_list(mg_map_at(node_properties, "Q"));

                                std::deque<double> pcur2_knots;
                                for (int i = 0; i < pcur2_knots_num; i++) {
                                    double knot = mg_value_float(mg_list_at(knots_simplifier_mglist, i * 2));
                                    int knot_cnt = mg_value_float(mg_list_at(knots_simplifier_mglist, i * 2 + 1));
                                    for (int j = 0; j < knot_cnt; j++)
                                        pcur2_knots.push_back(knot);
                                }
                                pcur2_knots.push_front(pcur2_knots.front());
                                pcur2_knots.push_back(pcur2_knots.back());

                                // control points
                                int pcur2_ctrl_points_num = pcur2_knots.size() - pcur2_degree - 1;
                                SPAposition *pcur2_ctrl_points = ACIS_NEW SPAposition[pcur2_ctrl_points_num];
                                for (int i = 0; i < pcur2_ctrl_points_num; i++) {
                                    SPAposition ctrl_point =
                                        SPAposition(mg_value_float(mg_list_at(ctrlpts_mglist, i * 2)),
                                                    mg_value_float(mg_list_at(ctrlpts_mglist, i * 2 + 1)), 0);
                                    pcur2_ctrl_points[i] = ctrl_point;
                                }
                                // create bs2_curve
                                bs2_curve pcur2_bs2_cur_created = bs2_curve_from_ctrlpts(
                                    pcur2_degree, 0, pcur2_closed, pcur2_periodic, pcur2_ctrl_points_num,
                                    pcur2_ctrl_points, nullptr, SPAresabs.value(), pcur2_knots.size(),
                                    std::vector<double>(pcur2_knots.begin(), pcur2_knots.end()).data(),
                                    SPAresnor.value());
                                ACIS_DELETE[] pcur2_ctrl_points;
                                pcur2 = pcur2_bs2_cur_created;
                            }

                            int_cur *Int_cur = nullptr;
                            if (cur_subtype == 25) {
                                // parcur
                                logical surface_tag;
                                if (cur_surface_tag == 0) {
                                    myerror("int_cur子类类型为parcur，但surface_tag为空。");
                                } else {
                                    if (cur_surface_tag == 1) {
                                        surface_tag = TRUE;
                                    } else if (cur_surface_tag == 2) {
                                        surface_tag = FALSE;
                                    } else {
                                        myerror(std::format(
                                            "int_cur子类类型为parcur，但surface_tag不是surf1或surf2，而是：{}",
                                            cur_surface_tag));
                                    }
                                }
                                Int_cur =
                                    ACIS_NEW par_int_cur(bs_cur, cur_fitol, *surf1, *surf2, pcur1, pcur2, surface_tag);
                            } else if (cur_subtype == 1) {
                                // exactcur
                                Int_cur = ACIS_NEW exact_int_cur(bs_cur, *surf1, *surf2, pcur1, pcur2);
                                Int_cur->gme_set_fitol_data(cur_fitol); // fitol
                            } else if (cur_subtype == 31) {
                                Int_cur = ACIS_NEW int_int_cur(NULL, bs_cur, cur_fitol, *surf1, *surf2, pcur1, pcur2);
                            } else {
                                myerror(std::format("不支持的int_cur子类类型：{}", cur_subtype));
                            }
                            ACIS_DELETE surf1;
                            ACIS_DELETE surf2;
                            Int_cur->gme_set_safe_range(safe_range); // range
                            id2ptr[node_id] = Int_cur;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::plane_surface: {
                            SPAposition root_point = AccessUtils::Restore::parsemglist_SPAposition(
                                mg_value_list(mg_map_at(node_properties, "c")), 3);
                            SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                mg_value_list(mg_map_at(node_properties, "d")));
                            SPAvector u_deriv = AccessUtils::Restore::parsemglist_SPAvector(
                                mg_value_list(mg_map_at(node_properties, "e")));
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
                        } break;
                        case AccessUtils::Restore::Neo4jNode::cone_surface: {
                            SPAposition base_centre = AccessUtils::Restore::parsemglist_SPAposition(
                                mg_value_list(mg_map_at(node_properties, "c")), 3);
                            SPAunit_vector base_normal = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                mg_value_list(mg_map_at(node_properties, "d")));
                            SPAvector base_major_axis = AccessUtils::Restore::parsemglist_SPAvector(
                                mg_value_list(mg_map_at(node_properties, "e")));
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
                        } break;
                        case AccessUtils::Restore::Neo4jNode::spline_surface: {
                            spline *def = ACIS_NEW spline();
                            def->gme_set_rev(mg_value_integer(mg_map_at(node_properties, "c")));
                            def->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(
                                mg_value_list(mg_map_at(node_properties, "b"))));
                            SPLINE *spline_surface = nullptr;
                            API_BEGIN;
                            spline_surface = ACIS_NEW SPLINE(*def);
                            API_END;
                            ACIS_DELETE def;
                            id2ptr[node_id] = spline_surface;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::spl_sur: {
                            int rational_u = mg_value_integer(mg_map_at(node_properties, "c"));
                            int rational_v = mg_value_integer(mg_map_at(node_properties, "p"));
                            int degree_u = mg_value_integer(mg_map_at(node_properties, "d"));
                            int degree_v = mg_value_integer(mg_map_at(node_properties, "e"));
                            int closed_u = mg_value_integer(mg_map_at(node_properties, "f"));
                            int closed_v = mg_value_integer(mg_map_at(node_properties, "g"));
                            int u_singularity = mg_value_integer(mg_map_at(node_properties, "h"));
                            int v_singularity = mg_value_integer(mg_map_at(node_properties, "i"));
                            int knots_u_num = mg_value_integer(mg_map_at(node_properties, "j"));
                            int knots_v_num = mg_value_integer(mg_map_at(node_properties, "k"));
                            const mg_list *knots_simplifier_u_mglist = mg_value_list(mg_map_at(node_properties, "l"));
                            const mg_list *knots_simplifier_v_mglist = mg_value_list(mg_map_at(node_properties, "m"));
                            const mg_list *ctrlpts_mglist = mg_value_list(mg_map_at(node_properties, "n"));
                            double fitol = mg_value_float(mg_map_at(node_properties, "o"));

                            std::deque<double> knots_u, knots_v;
                            for (uint32_t i = 0; i < knots_u_num; i++) {
                                double knot = mg_value_float(mg_list_at(knots_simplifier_u_mglist, i * 2));
                                int knot_cnt = mg_value_float(mg_list_at(knots_simplifier_u_mglist, i * 2 + 1));
                                for (int j = 0; j < knot_cnt; j++)
                                    knots_u.push_back(knot);
                            }
                            for (int i = 0; i < knots_v_num; i++) {
                                double knot = mg_value_float(mg_list_at(knots_simplifier_v_mglist, i * 2));
                                int knot_cnt = mg_value_float(mg_list_at(knots_simplifier_v_mglist, i * 2 + 1));
                                for (int j = 0; j < knot_cnt; j++)
                                    knots_v.push_back(knot);
                            }
                            knots_u.push_front(knots_u.front());
                            knots_u.push_back(knots_u.back());
                            knots_v.push_front(knots_v.front());
                            knots_v.push_back(knots_v.back());

                            // control points
                            int ctrl_points_num_u = knots_u.size() - degree_u - 1;
                            int ctrl_points_num_v = knots_v.size() - degree_v - 1;
                            SPAposition *ctrl_points = ACIS_NEW SPAposition[ctrl_points_num_u * ctrl_points_num_v];
                            double *weights = nullptr;
                            if (rational_u || rational_v) {
                                weights = new double[ctrl_points_num_u * ctrl_points_num_v];
                            }
                            int ctrl_points_idx = 0;
                            for (int i = 0; i < ctrl_points_num_v; i++) {
                                for (int j = 0; j < ctrl_points_num_u; j++) {
                                    SPAposition ctrl_point =
                                        SPAposition(mg_value_float(mg_list_at(ctrlpts_mglist, ctrl_points_idx)),
                                                    mg_value_float(mg_list_at(ctrlpts_mglist, ctrl_points_idx + 1)),
                                                    mg_value_float(mg_list_at(ctrlpts_mglist, ctrl_points_idx + 2)));
                                    ctrl_points[j * ctrl_points_num_v + i] = ctrl_point;
                                    ctrl_points_idx += 3;
                                    if (rational_u || rational_v) {
                                        double weight = mg_value_float(mg_list_at(ctrlpts_mglist, ctrl_points_idx));
                                        ctrl_points_idx++;
                                        weights[j * ctrl_points_num_v + i] = weight;
                                    }
                                }
                            }

                            bs3_surface bs_sur = bs3_surface_from_ctrlpts(
                                degree_u, rational_u, closed_u, u_singularity, ctrl_points_num_u, degree_v, rational_v,
                                closed_v, v_singularity, ctrl_points_num_v, ctrl_points, weights, SPAresabs.value(),
                                knots_u.size(), std::vector<double>(knots_u.begin(), knots_u.end()).data(),
                                knots_v.size(), std::vector<double>(knots_v.begin(), knots_v.end()).data(),
                                SPAresnor.value());
                            ACIS_DELETE[] ctrl_points;
                            if (rational_u || rational_v) {
                                delete[] weights;
                            }
                            spl_sur *spl_sur = ACIS_NEW exact_spl_sur(bs_sur);
                            spl_sur->gme_set_fitol_data(fitol);
                            id2ptr[node_id] = spl_sur;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::pcurve: {
                            int def_type = mg_value_integer(mg_map_at(node_properties, "b"));
                            if (def_type != 0) {
                                double du = mg_value_float(mg_map_at(node_properties, "c"));
                                double dv = mg_value_float(mg_map_at(node_properties, "d"));
                                PCURVE *pcurve = nullptr;
                                API_BEGIN;
                                pcurve = ACIS_NEW PCURVE(nullptr, def_type, 0, SPApar_vec(du, dv));
                                API_END;
                                id2ptr[node_id] = pcurve;
                            } else {
                                pcurve *def = ACIS_NEW pcurve();
                                def->gme_set_rev(mg_value_integer(mg_map_at(node_properties, "e")));
                                double du = mg_value_float(mg_map_at(node_properties, "c"));
                                double dv = mg_value_float(mg_map_at(node_properties, "d"));
                                def->gme_set_off(SPApar_vec(du, dv));
                                PCURVE *pcurve = nullptr;
                                API_BEGIN;
                                pcurve = ACIS_NEW PCURVE(*def);
                                API_END;
                                ACIS_DELETE def;
                                id2ptr[node_id] = pcurve;
                            }
                        } break;
                        case AccessUtils::Restore::Neo4jNode::par_cur: {
                            int subtype = mg_value_integer(mg_map_at(node_properties, "b"));
                            if (subtype == 32) {
                                int degree = mg_value_integer(mg_map_at(node_properties, "f"));
                                int closed_periodic = mg_value_integer(mg_map_at(node_properties, "g"));
                                int closed, periodic;
                                if (closed_periodic == 0) {
                                    // open
                                    closed = 0;
                                    periodic = 0;
                                } else if (closed_periodic == 1) {
                                    // closed
                                    closed = 1;
                                    periodic = 0;
                                } else if (closed_periodic == 2) {
                                    // periodic
                                    closed = 1;
                                    periodic = 1;
                                }
                                int knots_num = mg_value_integer(mg_map_at(node_properties, "h"));
                                const mg_list *knots_simplifier_mglist = mg_value_list(mg_map_at(node_properties, "i"));
                                const mg_list *ctrlpts_mglist = mg_value_list(mg_map_at(node_properties, "j"));
                                double fitol = mg_value_float(mg_map_at(node_properties, "d"));

                                std::deque<double> knots;
                                for (int i = 0; i < knots_num; i++) {
                                    double knot = mg_value_float(mg_list_at(knots_simplifier_mglist, i * 2));
                                    int knot_cnt = mg_value_float(mg_list_at(knots_simplifier_mglist, i * 2 + 1));
                                    for (int j = 0; j < knot_cnt; j++)
                                        knots.push_back(knot);
                                }
                                knots.push_front(knots.front());
                                knots.push_back(knots.back());

                                // control points
                                int ctrl_points_num = knots.size() - degree - 1;
                                SPAposition *ctrl_points = ACIS_NEW SPAposition[ctrl_points_num];
                                for (int i = 0; i < ctrl_points_num; i++) {
                                    SPAposition ctrl_point =
                                        SPAposition(mg_value_float(mg_list_at(ctrlpts_mglist, i * 2)),
                                                    mg_value_float(mg_list_at(ctrlpts_mglist, i * 2 + 1)), 0);
                                    ctrl_points[i] = ctrl_point;
                                }
                                bs2_curve bs_cur = bs2_curve_from_ctrlpts(
                                    degree, 0, closed, periodic, ctrl_points_num, ctrl_points, nullptr, DBL_MIN,
                                    knots.size(), std::vector<double>(knots.begin(), knots.end()).data(), DBL_MIN);
                                ACIS_DELETE[] ctrl_points;

                                int surf_type = mg_value_integer(mg_map_at(node_properties, "e"));

                                // surf
                                surface *surf = nullptr;
                                if (surf_type == 2) {
                                    SPAposition root_point = AccessUtils::Restore::parsemglist_SPAposition(
                                        mg_value_list(mg_map_at(node_properties, "l")), 3);
                                    SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                        mg_value_list(mg_map_at(node_properties, "m")));
                                    SPAvector u_deriv = AccessUtils::Restore::parsemglist_SPAvector(
                                        mg_value_list(mg_map_at(node_properties, "n")));
                                    plane *ret_plane = ACIS_NEW plane(root_point, normal, u_deriv);
                                    ret_plane->reverse_v = mg_value_integer(mg_map_at(node_properties, "o"));
                                    ret_plane->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(
                                        mg_value_list(mg_map_at(node_properties, "k"))));
                                    surf = ret_plane;
                                } else if (surf_type == 3) {
                                    SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(
                                        mg_value_list(mg_map_at(node_properties, "l")), 3);
                                    double radius = mg_value_float(mg_map_at(node_properties, "m"));
                                    sphere *ret_sphere = ACIS_NEW sphere(centre, radius);
                                    ret_sphere->uv_oridir = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                        mg_value_list(mg_map_at(node_properties, "n")));
                                    ret_sphere->pole_dir = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                        mg_value_list(mg_map_at(node_properties, "o")));
                                    ret_sphere->reverse_v = mg_value_integer(mg_map_at(node_properties, "p"));
                                    ret_sphere->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(
                                        mg_value_list(mg_map_at(node_properties, "k"))));
                                    surf = ret_sphere;
                                } else if (surf_type == 4) {
                                    SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(
                                        mg_value_list(mg_map_at(node_properties, "l")), 3);
                                    SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                        mg_value_list(mg_map_at(node_properties, "m")));
                                    double major_radius = mg_value_float(mg_map_at(node_properties, "n"));
                                    double minor_radius = mg_value_float(mg_map_at(node_properties, "o"));
                                    torus *ret_torus = ACIS_NEW torus(centre, normal, major_radius, minor_radius);
                                    ret_torus->uv_oridir = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                        mg_value_list(mg_map_at(node_properties, "p")));
                                    ret_torus->reverse_v = mg_value_integer(mg_map_at(node_properties, "q"));
                                    ret_torus->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(
                                        mg_value_list(mg_map_at(node_properties, "k"))));
                                    surf = ret_torus;
                                } else if (surf_type == 5) {
                                    SPAposition base_centre = AccessUtils::Restore::parsemglist_SPAposition(
                                        mg_value_list(mg_map_at(node_properties, "l")), 3);
                                    SPAunit_vector base_normal = AccessUtils::Restore::parsemglist_SPAunit_vector(
                                        mg_value_list(mg_map_at(node_properties, "m")));
                                    SPAvector base_major_axis = AccessUtils::Restore::parsemglist_SPAvector(
                                        mg_value_list(mg_map_at(node_properties, "n")));
                                    double base_radius_ratio = mg_value_float(mg_map_at(node_properties, "o"));
                                    ellipse *gem_base =
                                        ACIS_NEW ellipse(base_centre, base_normal, base_major_axis, base_radius_ratio);
                                    gem_base->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPAinterval(
                                        mg_value_list(mg_map_at(node_properties, "p"))));
                                    double sine_angle = mg_value_float(mg_map_at(node_properties, "q"));
                                    double cosine_angle = mg_value_float(mg_map_at(node_properties, "r"));
                                    cone *ret_cone = ACIS_NEW cone(*gem_base, sine_angle, cosine_angle);
                                    ACIS_DELETE gem_base;
                                    ret_cone->reverse_u = mg_value_integer(mg_map_at(node_properties, "s"));
                                    ret_cone->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(
                                        mg_value_list(mg_map_at(node_properties, "k"))));
                                    surf = ret_cone;
                                } else if (surf_type == 1) {
                                    spline *ret_spline = ACIS_NEW spline();
                                    ret_spline->gme_set_rev(mg_value_integer(mg_map_at(node_properties, "l")));
                                    ret_spline->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(
                                        mg_value_list(mg_map_at(node_properties, "k"))));
                                    surf = ret_spline;
                                }
                                par_cur *par_cur =
                                    exp_par_cur::gme_exp_par_cur_public_constructor(bs_cur, fitol, -1.0, *surf);
                                ACIS_DELETE surf;
                                id2ptr[node_id] = par_cur;
                            } else {
                                myerror("不支持的par_cur子类型。");
                            }
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
                    DIAG_LOG("[restore-neo4j] subgraphAll returned relationships=%u\n", rellist_size);
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
                            DIAG_LOG("[restore-rel] body_lump_ptr: body=%p lump=%p\n", (void *)body, (void *)lump);
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
                            DIAG_LOG("[restore-rel] lump_shell_ptr: lump=%p shell=%p\n", (void *)lump, (void *)shell);
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
                            DIAG_LOG("[restore-rel] shell_face_ptr: shell=%p face=%p\n", (void *)shell, (void *)face);
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
    }

    for (const int64_t s : id_list) {
        entity_list.add((ENTITY *)id2ptr.at(s));
    }

    DIAG_LOG("[restore-end] entity_list.count=%d\n", (int)entity_list.count());
    for (ENTITY *e : entity_list) {
        if (e == nullptr) {
            DIAG_LOG("[restore-end] WARNING: null entity in list\n");
            continue;
        }
        const char *tag = e->identity() == BODY_ID    ? "BODY"
                          : e->identity() == LUMP_ID  ? "LUMP"
                          : e->identity() == SHELL_ID ? "SHELL"
                          : e->identity() == FACE_ID  ? "FACE"
                                                      : "OTHER";
        DIAG_LOG("[restore-end] entity=%p type=%s\n", (void *)e, tag);
    }
}

// =====================================================================
// Collab 增量 delta 计算（不涉及 Neo4j IO）
// =====================================================================
