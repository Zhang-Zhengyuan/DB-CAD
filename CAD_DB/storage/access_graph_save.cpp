#include "access_internal.hxx"

void api_save_entity_list_neo4j(const Neo4jPart &conn, const ENTITY_LIST &entity_list,
                                std::unordered_map<void *, int64_t> &ptr2id) {
    TMDF;

    double db_execution_duration = 0;
    for (ENTITY *entity_list_item

         : entity_list) {
        std::unordered_set<void *> visited;
        std::deque<ENTITY *> que;
        std::string elemid0, elemid1;

        std::unordered_map<void *, Node *> ptr2node;
        std::vector<Relationship *> relationship_list;

        if (ptr2node.count(entity_list_item) == 0) {
            ptr2node[entity_list_item] = new Node();
        }
        que.push_back(entity_list_item);

        while (!que.empty()) {
            ENTITY *entity_ptr = que.front();
            que.pop_front();
            if (entity_ptr == nullptr || visited.find(entity_ptr) != visited.end())
                continue;
            switch (entity_ptr->identity(1)) {
            case BODY_ID: {
                BODY *ptr = (BODY *)entity_ptr;
                ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, body, lump, wire, transform);
                Node *ptrnode = ptr2node.at(ptr);
                ptrnode->labels.clear();
                ptrnode->labels.push_back("body");
                ptrnode->properties['a'] = mg_value_make_string("body");
                DIAG_LOG("[save-body] BODY=%p lump=%p wire=%p transform=%p\n", (void *)ptr, (void *)ptr->lump(),
                         (void *)ptr->wire(), (void *)ptr->transform());
                if (ptr->lump() != nullptr) {
                    LUMP *L = ptr->lump();
                    int n_shells = 0;
                    for (SHELL *s = L->shell(); s != nullptr; s = s->next()) {
                        ++n_shells;
                        int n_faces = 0;
                        for (FACE *f = s->face(); f != nullptr; f = f->next())
                            ++n_faces;
                        DIAG_LOG("[save-body]   lump->shell=%p n_shell=%d faces=%d\n", (void *)s, n_shells, n_faces);
                    }
                    DIAG_LOG("[save-body]   total shells in first lump=%d\n", n_shells);
                } else {
                    DIAG_LOG("[save-body]   lump=NULL!\n");
                }
            } break;
            case LUMP_ID: {
                LUMP *ptr = (LUMP *)entity_ptr;
                // 必须用 next 遍历 lump 链表，否则 shell->next()->... 等后续 lump 全丢失
                ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, lump, next, shell, body);
                Node *ptrnode = ptr2node.at(ptr);
                ptrnode->labels.clear();
                ptrnode->labels.push_back("lump");
                ptrnode->properties['a'] = mg_value_make_string("lump");
            } break;
            case SHELL_ID: {
                SHELL *ptr = (SHELL *)entity_ptr;
                // 必须用 next 遍历 shell 链表，否则 shell->next()->... 等后续 shell 全丢失
                ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, shell, next, subshell, face, wire, lump);
                Node *ptrnode = ptr2node.at(ptr);
                ptrnode->labels.clear();
                ptrnode->labels.push_back("shell");
                ptrnode->properties['a'] = mg_value_make_string("shell");
            } break;
            case SUBSHELL_ID: {
                SUBSHELL *ptr = (SUBSHELL *)entity_ptr;
                ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, subshell, parent, sibling, child, face,
                                          wire);
                Node *ptrnode = ptr2node.at(ptr);
                ptrnode->labels.clear();
                ptrnode->labels.push_back("subshell");
                ptrnode->properties['a'] = mg_value_make_string("subshell");
            } break;
            case WIRE_ID: {
                WIRE *ptr = (WIRE *)entity_ptr;
                ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, wire, next, coedge, owner, subshell);
                Node *ptrnode = ptr2node.at(ptr);
                ptrnode->labels.clear();
                ptrnode->labels.push_back("wire");
                ptrnode->properties['a'] = mg_value_make_string("wire");
                ptrnode->properties['b'] = mg_value_make_integer(ptr->cont());
            } break;
            case FACE_ID: {
                FACE *ptr = (FACE *)entity_ptr;
                ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, face, next, loop, shell, subshell,
                                          geometry);
                if (ptr->sides()) {
                    Node *ptrnode = ptr2node.at(ptr);
                    ptrnode->labels.clear();
                    ptrnode->labels.push_back("face");
                    ptrnode->properties['a'] = mg_value_make_string("face");
                    ptrnode->properties['b'] = mg_value_make_integer(ptr->sense());
                    ptrnode->properties['c'] = mg_value_make_integer(1);
                    ptrnode->properties['d'] = mg_value_make_integer(ptr->cont());
                } else {
                    Node *ptrnode = ptr2node.at(ptr);
                    ptrnode->labels.clear();
                    ptrnode->labels.push_back("face");
                    ptrnode->properties['a'] = mg_value_make_string("face");
                    ptrnode->properties['b'] = mg_value_make_integer(ptr->sense());
                    ptrnode->properties['c'] = mg_value_make_integer(0);
                }
            } break;
            case LOOP_ID: {
                LOOP *ptr = (LOOP *)entity_ptr;
                ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, loop, next, start, face);
                Node *ptrnode = ptr2node.at(ptr);
                ptrnode->labels.clear();
                ptrnode->labels.push_back("loop");
                ptrnode->properties['a'] = mg_value_make_string("loop");
            } break;
            case COEDGE_ID: {
                COEDGE *ptr = (COEDGE *)entity_ptr;
                ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, coedge, next, previous, partner, edge,
                                          owner, geometry);
                Node *ptrnode = ptr2node.at(ptr);
                ptrnode->labels.clear();
                ptrnode->labels.push_back("coedge");
                ptrnode->properties['a'] = mg_value_make_string("coedge");
                ptrnode->properties['b'] = mg_value_make_integer(ptr->sense());
            } break;
            case EDGE_ID: {
                EDGE *ptr = (EDGE *)entity_ptr;
                ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, edge, start, end, coedge, geometry);
                Node *ptrnode = ptr2node.at(ptr);
                ptrnode->labels.clear();
                ptrnode->labels.push_back("edge");
                ptrnode->properties['a'] = mg_value_make_string("edge");
                ptrnode->properties['b'] = mg_value_make_integer(ptr->sense());
            } break;
            case VERTEX_ID: {
                VERTEX *ptr = (VERTEX *)entity_ptr;
                ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, vertex, edge, geometry);
                Node *ptrnode = ptr2node.at(ptr);
                ptrnode->labels.clear();
                ptrnode->labels.push_back("vertex");
                ptrnode->properties['a'] = mg_value_make_string("vertex");
            } break;
            case PCURVE_ID: {
                PCURVE *ptr = (PCURVE *)entity_ptr;
                int def_type = ((PCURVE *)ptr)->gme_get_def_type();
                if (def_type != 0) {
                    ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, pcurve, ref_curve);
                    SPApar_vec offset = ((PCURVE *)ptr)->offset();
                    Node *ptrnode = ptr2node.at(ptr);
                    ptrnode->labels.clear();
                    ptrnode->labels.push_back("pcurve");
                    ptrnode->properties['a'] = mg_value_make_string("pcurve");
                    ptrnode->properties['b'] = mg_value_make_integer(def_type);
                    ptrnode->properties['c'] = mg_value_make_float(double(offset.du));
                    ptrnode->properties['d'] = mg_value_make_float(double(offset.dv));
                } else {
                    pcurve pcur = ((PCURVE *)ptr)->gme_get_def();

                    int rev = pcur.reversed();
                    SPApar_vec offset = pcur.offset();
                    par_cur *cur = pcur.gme_get_fit();
                    if (cur != nullptr) {
                        if (ptr2node.find(cur) == ptr2node.end()) {
                            ptr2node[cur] =
                                AccessUtils::Save::createnode_par_cur_subgraph(ptr2node, relationship_list, cur);
                        }
                        Node *a = ptr2node.at(ptr);
                        Node *b = ptr2node.at(cur);
                        Relationship *r = new Relationship(a, b);
                        r->label = "pcurve_fit_ptr";
                        r->properties['a'] = mg_value_make_string("pcurve_fit_ptr");
                        relationship_list.push_back(r);
                    }
                    Node *ptrnode = ptr2node.at(ptr);
                    ptrnode->labels.clear();
                    ptrnode->labels.push_back("pcurve");
                    ptrnode->properties['a'] = mg_value_make_string("pcurve");
                    ptrnode->properties['b'] = mg_value_make_integer(def_type);
                    ptrnode->properties['c'] = mg_value_make_float(double(offset.du));
                    ptrnode->properties['d'] = mg_value_make_float(double(offset.dv));
                    ptrnode->properties['e'] = mg_value_make_integer(rev);
                }
            } break;
            case APOINT_ID: {
                APOINT *ptr = (APOINT *)entity_ptr;
                SPAposition pos = ((APOINT *)ptr)->coords();
                Node *ptrnode = ptr2node.at(ptr);
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
                    Node *ptrnode = ptr2node.at(ptr);
                    ptrnode->labels.clear();
                    ptrnode->labels.push_back("straight-curve");
                    ptrnode->properties['a'] = mg_value_make_string("straight-curve");
                    ptrnode->properties['b'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAinterval(range));
                    ptrnode->properties['c'] =
                        mg_value_make_list(AccessUtils::Save::getmglist_SPAposition(root_point, 3));
                    ptrnode->properties['d'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAvector(direction));
                } break;
                case ELLIPSE_ID: {
                    ELLIPSE *ptr = (ELLIPSE *)entity_ptr;
                    ellipse gem = ((ELLIPSE *)ptr)->gme_get_def();
                    SPAposition centre = gem.centre;
                    SPAunit_vector normal = gem.normal;
                    SPAvector major_axis = gem.major_axis;
                    SPAinterval range = gem.gme_get_subset_range();
                    Node *ptrnode = ptr2node.at(ptr);
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
                    Node *ptrnode = ptr2node.at(ptr);
                    ptrnode->labels.clear();
                    ptrnode->labels.push_back("helix-curve");
                    ptrnode->properties['a'] = mg_value_make_string("helix-curve");
                    ptrnode->properties['b'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAinterval(range));
                    ptrnode->properties['c'] =
                        mg_value_make_list(AccessUtils::Save::getmglist_SPAposition(axis_root, 3));
                    ptrnode->properties['d'] =
                        mg_value_make_list(AccessUtils::Save::getmglist_SPAunit_vector(axis_dir));
                    ptrnode->properties['e'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAvector(start_disp));
                    ptrnode->properties['f'] = mg_value_make_float(gem.pitch());
                    ptrnode->properties['g'] = mg_value_make_integer(gem.handedness());
                    ptrnode->properties['h'] = mg_value_make_float(gem.par_scaling());
                    ptrnode->properties['i'] = mg_value_make_float(gem.taper());
                    ptrnode->properties['j'] =
                        mg_value_make_list(AccessUtils::Save::getmglist_SPAinterval(helix_range));
                } break;
                case INTCURVE_ID: {
                    INTCURVE *ptr = (INTCURVE *)entity_ptr;
                    intcurve gem = ((INTCURVE *)ptr)->gme_get_def();
                    SPAinterval range = gem.gme_get_subset_range();
                    int_cur *cur = gem.gme_get_fit();
                    gem.cur();
                    if (cur != nullptr) {
                        if (ptr2node.find(cur) == ptr2node.end()) {
                            ptr2node[cur] =
                                AccessUtils::Save::createnode_int_cur_subgraph(ptr2node, relationship_list, cur);
                        }
                        Node *a = ptr2node.at(ptr);
                        Node *b = ptr2node.at(cur);
                        Relationship *r = new Relationship(a, b);
                        r->label = "intcurve-curve_fit_ptr";
                        r->properties['a'] = mg_value_make_string("intcurve-curve_fit_ptr");
                        relationship_list.push_back(r);
                    }
                    Node *ptrnode = ptr2node.at(ptr);
                    ptrnode->labels.clear();
                    ptrnode->labels.push_back("intcurve-curve");
                    ptrnode->properties['a'] = mg_value_make_string("intcurve-curve");
                    ptrnode->properties['b'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAinterval(range));
                    ptrnode->properties['c'] = mg_value_make_integer(gem.reversed());
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
                    Node *ptrnode = ptr2node.at(ptr);
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
                    Node *ptrnode = ptr2node.at(ptr);
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
                    Node *ptrnode = ptr2node.at(ptr);
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
                    Node *ptrnode = ptr2node.at(ptr);
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
                case SPLINE_ID: {
                    SPLINE *ptr = (SPLINE *)entity_ptr;
                    spline gem = ((SPLINE *)ptr)->gme_get_def();
                    AccessUtils::Save::spline_data *gem_data =
                        AccessUtils::Save::get_spline_data_subgraph(ptr2node, &gem);
                    if (gem_data->spl_node != nullptr) {
                        Node *a = ptr2node.at(ptr);
                        Node *b = gem_data->spl_node;
                        Relationship *r = new Relationship(a, b);
                        r->label = "spline-surface_spl_ptr";
                        r->properties['a'] = mg_value_make_string("spline-surface_spl_ptr");
                        relationship_list.push_back(r);
                    }
                    Node *ptrnode = ptr2node.at(ptr);
                    ptrnode->labels.clear();
                    ptrnode->labels.push_back("spline-surface");
                    ptrnode->properties['a'] = mg_value_make_string("spline-surface");
                    ptrnode->properties['b'] = mg_value_make_list(gem_data->subset_range);
                    ptrnode->properties['c'] = mg_value_make_integer(gem_data->reversed);
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
                Node *ptrnode = ptr2node.at(ptr);
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
                DIAG_LOG("[save-neo4j] BFS default case: ptr=%p identity(1)=%d identity()=%d\n", (void *)entity_ptr,
                         entity_ptr->identity(1), entity_ptr->identity());
            } break;
            }
            visited.insert(entity_ptr);
        }
        DIAG_LOG("[save-neo4j] BFS done: ptr2node.size=%zu relationship_list.size=%zu\n", ptr2node.size(),
                 relationship_list.size());
        {
            // 统计每种节点类型
            int n_body = 0, n_lump = 0, n_shell = 0, n_face = 0, n_loop = 0, n_edge = 0, n_vert = 0, n_other = 0;
            for (auto [ptr, node] : ptr2node) {
                ENTITY *e = (ENTITY *)ptr;
                const char *type_name = "(null)";
                switch (e->identity()) {
                case BODY_ID:
                    type_name = "BODY";
                    break;
                case LUMP_ID:
                    type_name = "LUMP";
                    break;
                case SHELL_ID:
                    type_name = "SHELL";
                    break;
                case FACE_ID:
                    type_name = "FACE";
                    break;
                case LOOP_ID:
                    type_name = "LOOP";
                    break;
                case EDGE_ID:
                    type_name = "EDGE";
                    break;
                case VERTEX_ID:
                    type_name = "VERTEX";
                    break;
                case COEDGE_ID:
                    type_name = "COEDGE";
                    break;
                case TRANSFORM_ID:
                    type_name = "TRANSFORM";
                    break;
                default: {
                    int id1 = e->identity(1);
                    if (id1 == BODY_ID)
                        type_name = "BODY(id1)";
                    else if (id1 == LUMP_ID)
                        type_name = "LUMP(id1)";
                    else if (id1 == SHELL_ID)
                        type_name = "SHELL(id1)";
                    else if (id1 == FACE_ID)
                        type_name = "FACE(id1)";
                    else if (id1 == LOOP_ID)
                        type_name = "LOOP(id1)";
                    else if (id1 == EDGE_ID)
                        type_name = "EDGE(id1)";
                    else if (id1 == VERTEX_ID)
                        type_name = "VERTEX(id1)";
                    else
                        type_name = "OTHER";
                    break;
                }
                }
                const char *label = "(null)";
                if (!node->labels.empty())
                    label = node->labels[0].c_str();
                if (strcmp(label, "body") == 0)
                    ++n_body;
                else if (strcmp(label, "lump") == 0)
                    ++n_lump;
                else if (strcmp(label, "shell") == 0)
                    ++n_shell;
                else if (strcmp(label, "face") == 0)
                    ++n_face;
                else if (strcmp(label, "loop") == 0)
                    ++n_loop;
                else if (strcmp(label, "edge") == 0)
                    ++n_edge;
                else if (strcmp(label, "vertex") == 0)
                    ++n_vert;
                else
                    ++n_other;
                DIAG_LOG("[save-neo4j] node ptr=%p type=%s label=%s\n", (void *)e, type_name, label);
            }
            DIAG_LOG("[save-neo4j] node counts: body=%d lump=%d shell=%d face=%d loop=%d edge=%d vertex=%d other=%d\n",
                     n_body, n_lump, n_shell, n_face, n_loop, n_edge, n_vert, n_other);
            DIAG_LOG("[save-neo4j] rel counts: %zu\n", relationship_list.size());
        }
        {
            uint32_t node_list_size = ptr2node.size();
            mg_list *mgl_node_list = mg_list_make_empty(node_list_size);
            int nodeidx = 0;
            for (auto [ptr, node] : ptr2node) {
                mg_map *mgm_node = mg_map_make_empty(2 + node->properties.size());
                mg_map_append(mgm_node, mg_string_make("W"), mg_value_make_integer(nodeidx));
                // 将 std::vector<std::string> labels 转换为 mg_list
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
            TMST;
            conn.execute_bolt(
                "UNWIND $Y AS Z "
                "CALL "
                "apoc.create.node(Z.X,{a:Z.a,b:Z.b,c:Z.c,d:Z.d,e:Z.e,f:Z.f,g:Z.g,h:Z.h,i:Z.i,j:Z.j,k:Z.k,l:Z.l,m:Z.m,n:"
                "Z.n,o:Z.o,p:Z.p,q:Z.q,r:Z.r,s:Z.s,t:Z.t,u:Z.u,v:Z.v,w:Z.w,x:Z.x,y:Z.y,z:Z.z,A:Z.A,B:Z.B,C:Z.C,D:Z.D,E:"
                "Z.E,F:Z.F,G:Z.G,H:Z.H,I:Z.I,J:Z.J,K:Z.K,L:Z.L,M:Z.M,N:Z.N,O:Z.O,P:Z.P,Q:Z.Q}) "
                "YIELD node RETURN id(node) ORDER BY Z.W ",
                qparams);
            TMED;
            db_execution_duration += TMDR;
            mg_map_destroy(qparams);
        }
        {
            mg_result *result;
            for (auto [ptr, node] : ptr2node) {
                if (mg_session_fetch(conn.session, &result) == 1) {
                    const mg_list *mgl_nodeid = mg_result_row(result);
                    const uint32_t mgl_nodeid_length = mg_list_size(mgl_nodeid);
                    assert(mgl_nodeid_length == 1);
                    int64_t nodeid = mg_value_integer(mg_list_at(mgl_nodeid, 0));
                    node->id = nodeid;
                    ptr2id[ptr] = nodeid;
                } else {
                    myerror(std::format("mg_session_fetch失败，错误信息如下：{}", mg_session_error(conn.session)));
                }
            }
            if (mg_session_fetch(conn.session, &result) != 0) {
                myerror(std::format("mg_session_fetch失败，错误信息如下：{}", mg_session_error(conn.session)));
            }
        }
        {
            uint32_t rel_list_size = relationship_list.size();
            mg_list *mgl_rel_list = mg_list_make_empty(rel_list_size);
            for (auto rel : relationship_list) {
                mg_map *mgm_rel = mg_map_make_empty(3 + rel->properties.size());
                mg_map_append(mgm_rel, mg_string_make("U"), mg_value_make_integer(rel->u->id));
                mg_map_append(mgm_rel, mg_string_make("V"), mg_value_make_integer(rel->v->id));
                mg_map_append(mgm_rel, mg_string_make("T"), mg_value_make_string(rel->label.c_str()));
                for (auto [propkey, propval] : rel->properties) {
                    const char propkey_str[2] = {propkey, 0};
                    mg_map_append(mgm_rel, mg_string_make(propkey_str), propval);
                }
                mg_list_append(mgl_rel_list, mg_value_make_map(mgm_rel));
            }
            mg_map *qparams = mg_map_make_empty(1);
            mg_map_append(qparams, mg_string_make("Y"), mg_value_make_list(mgl_rel_list));
            TMST;
            // 关键修复：用 `WITH count(*) AS cnt RETURN cnt` 而不是 `RETURN id(rel) LIMIT 1`。
            //
            // 第一层 bug（已发现，已修）：原来用 "YIELD rel RETURN null LIMIT 0" 时
            // LIMIT 0 把整条 query 的副作用（apoc.create.relationship）一起砍掉，
            // 225 条拓扑关系一条都没真正写进 DB。
            //
            // 第二层 bug（本轮发现）：上一轮把 LIMIT 0 改成 LIMIT 1 也不行！
            // 在 Neo4j 5.15 + APOC 5 下，UNWIND + apoc.create.relationship 是行级流式
            // procedure，LIMIT 1 会在第一行满足后短路掉 UNWIND 的后续迭代，导致
            // 225 条里只有 1 条被真正创建。日志里 "[save-neo4j] AFTER write: total rels in DB = 927"
            // （仅 +2，比期望少 224）就是这个证据。
            //
            // Python neo4j 5.28 驱动对照测试结果（5 条 lump）：
            //   UNWIND + apoc.create.relationship + RETURN id(rel) LIMIT 1  → 1 条
            //   UNWIND + apoc.create.relationship + RETURN id(rel)          → 5 条 ✅
            //   UNWIND + apoc.create.relationship + RETURN null             → 5 条 ✅
            //   UNWIND + apoc.create.relationship + WITH count(*) RETURN cnt → 5 条 ✅
            //
            // 选 WITH count(*) RETURN cnt 是因为：副作用完整执行 + 只回 1 行
            // （single integer），不拖 rel id 回 C++，最省内存；C++ 端接着
            // discard_all_results() 丢掉这 1 行即可。
            conn.execute_bolt("UNWIND $Y AS Z "
                              "MATCH (W) WHERE id(W) = Z.U "
                              "MATCH (X) WHERE id(X) = Z.V "
                              "CALL apoc.create.relationship(W,Z.T,{a:Z.a},X) "
                              "YIELD rel WITH count(*) AS cnt RETURN cnt ",
                              qparams);
            TMED;
            db_execution_duration += TMDR;
            mg_map_destroy(qparams);
            conn.discard_all_results(); // 丢弃 relationship write 的结果
            // 诊断：检查写入后 Neo4j 里实际有多少关系。
            // 注意：每次 execute_bolt 必须用自己的 qparams；上一段关系写入的 qparams
            // 已经在 line 2440 mg_map_destroy(qparams) 释放了，必须重新分配。
            // 此外不再依赖 APOC 插件，subgraphAll 在不带 APOC 的镜像下会直接报错。
            {
                mg_map *qp = mg_map_make_empty(1);
                conn.execute_bolt("MATCH ()-[r]->() RETURN count(r) AS rel_count", qp);
                mg_map_destroy(qp);
                mg_result *r;
                if (mg_session_fetch(conn.session, &r) == 1) {
                    const mg_list *row = mg_result_row(r);
                    int64_t cnt = mg_value_integer(mg_list_at(row, 0));
                    DIAG_LOG("[save-neo4j] AFTER write: total rels in DB = %lld\n", cnt);
                }
                conn.discard_all_results();
                // subgraphAll 走 APOC，没有插件会 crash；用普通 MATCH/COUNT 替代即可
                qp = mg_map_make_empty(1);
                conn.execute_bolt("MATCH (n:part) RETURN count(n) AS part_count", qp);
                mg_map_destroy(qp);
                if (mg_session_fetch(conn.session, &r) == 1) {
                    const mg_list *row = mg_result_row(r);
                    int64_t nc = mg_value_integer(mg_list_at(row, 0));
                    DIAG_LOG("[save-neo4j] AFTER write: part nodes = %lld\n", nc);
                }
                conn.discard_all_results(); // 丢弃诊断结果
            }
            DIAG_LOG("[save-neo4j] written %d relationships to Neo4j\n", (int)rel_list_size);
        }

        for (auto [ptr, node] : ptr2node) {
            delete node;
        }
        for (auto rel : relationship_list) {
            delete rel;
        }
    }
    std::cout << "save_neo4j: " << db_execution_duration << " ms" << std::endl;
}
