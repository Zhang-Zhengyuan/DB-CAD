static void createPrimitive(const int id, ENTITY*& ptrEntity, std::vector<std::vector<SPAposition>>& handles, outcome& status) {
    BODY* ptrBody = nullptr;
    FACE* ptrFace = nullptr;
    EDGE* ptrEdge = nullptr;
    switch (id) {
        case BASIC_ENTITIES::E_CUBE: {
            status = api_solid_block(SPAposition(0.0, 0.0, 0.0), SPAposition(1.0, 1.0, 1.0), ptrBody);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_SPHERE: {
            status = api_make_sphere(1.0, ptrBody);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_CONE: {
            status = api_solid_cylinder_cone(SPAposition(0.0, 0.0, 0.0), SPAposition(0.0, 1.0, 0.0), 0.5, 0.5, 0.0, nullptr, ptrBody);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_CYLINDER: {
            status = api_solid_cylinder_cone(SPAposition(0.0, 0.0, 0.0), SPAposition(0.0, 1.0, 0.0), 0.5, 0.5, 0.5, nullptr, ptrBody);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_TORUS: {
            status = api_solid_torus(SPAposition(0.0, 0.0, 0.0), 1.0, 0.5, ptrBody);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_PRISM: {
            status = api_make_prism(1.0, 1.0, 1.0, 6, ptrBody);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_PYRAMID: {
            status = api_make_pyramid(1.0, 1.0, 1.0, 0.0, 4, ptrBody);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_WIGGLE: {
            status = api_wiggle(1.0, 1.0, 1.0, -1, 1, -2, 2, 1, ptrBody);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_PLANE: {
            SPAvector v = SPAvector(0.0, 0.0, 1.0);
            status = api_face_plane(SPAposition(0.0, 0.0, 0.0), 1.0, 1.0, &v, ptrFace);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_PPLANE: {
            status = api_make_plface(SPAposition(0.0, 0.0, 0.0), SPAposition(0.5, -1.0, 0.0), SPAposition(-1.0, 0.5, 0.0), ptrFace);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_CPLANE: {
            status = api_make_planar_disk(SPAposition(0.0, 0.0, 0.0), SPAunit_vector(0.0, 0.0, 1.0), 1.0, ptrFace);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_SPHERES: {
            SPAvector v = SPAvector(0.0, 1.0, 0.0);
            status = api_face_sphere(SPAposition(0.0, 0.0, 0.0), 1.0, -90, 90, 0, 360, &v, ptrFace);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_PSPHERES: {
            SPAposition center{ 0, 0, 0 };
            auto norm = SPAunit_vector(0, 0, 1);
            status = api_face_sphere(center, 1, -33, 45, 45, 266, &norm, ptrFace);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_CONIC_SIDE: {
            SPAposition p = SPAposition(1, 0, 0.0);
            status = api_face_cylinder_cone(SPAposition(0.0, 0.0, 0.0), SPAvector(0.0, 0.0, 1.0), 1.0, 0.5, 0, 360, 1.0, &p, ptrFace);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_CYLINDER_SIDE: {
            SPAposition p = SPAposition(1, 0, 0.0);
            status = api_face_cylinder_cone(SPAposition(0.0, 0.0, 0.0), SPAvector(0.0, 0.0, 1.0), 1.0, 1.0, 0, 360, 1.0, &p, ptrFace);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_PCONIC: {
            SPAposition p = SPAposition(0.0, 1.0, 0.0);
            status = api_face_cylinder_cone(SPAposition(0.0, 0.0, 0.0), SPAvector(0.0, 0.0, 1.0), 1.0, 0.5, 0, 100, 1.0, &p, ptrFace);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_TORUSS: {
            status = api_make_trface(SPAposition(0.0, 0.0, 0.0), SPAunit_vector(0.0, 0.0, 1.0), 2.0, 0.5, SPAposition(1.0, 0.0, 0), 0, 2 * M_PI, 0, 2 * M_PI, ptrFace);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_PTORUSS: {
            status = api_make_trface(SPAposition(0.0, 0.0, 0.0), SPAunit_vector(0.0, 0.0, 1.0), 2.0, 0.5, SPAposition(1.0, 0.0, 0), 0, 1, 0, 1, ptrFace);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_LAWS: {
            const char* law_equation1 = "vec(x,y,sin(x)*cos(y))";
            law* ptrLaw = nullptr;
            status = api_str_to_law(law_equation1, &ptrLaw);
            if (!status.ok()) return;
            status = api_face_law(ptrLaw, -10.0, 10.0, -10.0, 10.0, ptrFace);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_BSPLINE_CTRLPTS: {
            int rows_v = 0;
            int rows_u = 0;
            std::vector<SPAposition> pos;
            std::vector<double> knots_u;
            std::vector<double> knots_v;
            if (handles.size()) {
                rows_v = handles.size();
                rows_u = handles[0].size();
                for (auto p : handles) pos.insert(std::end(pos), std::begin(p), std::end(p));
                knots_u.resize(2 * rows_u);
                knots_v.resize(2 * rows_v);
                for(int i=0;i<rows_u;++i) { knots_u[i]=0.0; knots_u[i+rows_u]=1.0; }
                for(int i=0;i<rows_v;++i) { knots_v[i]=0.0; knots_v[i+rows_v]=1.0; }
            } else {
                rows_u = 4;
                rows_v = 4;
                knots_u.resize(2 * rows_u);
                knots_v.resize(2 * rows_v);
                for (int i = 0; i < rows_u; i++) {
                    std::vector<SPAposition> ps;
                    for (int j = 0; j < rows_u; j++) {
                        pos.push_back(SPAposition(i, j, (i + j) % 2));
                        ps.push_back(pos[i * rows_v + j]);
                    }
                    handles.push_back(ps);
                    knots_u[i] = 0.0;
                    knots_u[i + rows_u] = 1.0;
                    knots_v[i] = 0.0;
                    knots_v[i + rows_u] = 1.0;
                }
            }
            status = api_mk_fa_spl_ctrlpts(3, false, 0, 0, rows_u, 3, false, 0, 0, rows_v, &pos[0], nullptr, 0.001, 2 * rows_u, knots_u.data(), 2 * rows_v, knots_v.data(), 0.0, ptrFace);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_BSPLINE_FIT: {
            splgrid* ptrSplgrid = ACIS_NEW splgrid();
            int rows_v = 0;
            int rows_u = 0;
            std::vector<SPAposition> pos;
            if (handles.size()) {
                rows_v = handles.size();
                rows_u = handles[0].size();
                for (auto p : handles) pos.insert(std::end(pos), std::begin(p), std::end(p));
            } else {
                rows_u = 5;
                rows_v = 5;
                for (int i = 0; i < rows_u; i++) {
                    std::vector<SPAposition> ps;
                    for (int j = 0; j < rows_v; j++) {
                        pos.push_back(SPAposition(-i, j, (i + j) % 2));
                        ps.push_back(pos[i * rows_v + j]);
                    }
                    handles.push_back(ps);
                }
            }
            ptrSplgrid->set_gridpts_array(&pos[0], rows_v, rows_u);
            ptrSplgrid->set_u_start_array(nullptr);
            ptrSplgrid->set_u_end_array(nullptr);
            ptrSplgrid->set_v_start_array(nullptr);
            ptrSplgrid->set_v_end_array(nullptr);
            status = api_face_spl_apprx(ptrSplgrid, ptrFace);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_LINE: {
            SPAposition s = SPAposition(0.0, 0.0, 0.0);
            SPAposition e = SPAposition(1.0, 0.0, 0.0);
            status = api_curve_line(s, e, ptrEdge);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_ARC: {
            SPAposition p = SPAposition(0.0, 0.0, 0.0);
            status = api_curve_arc(p, 1.0, 0.0, 2.0, ptrEdge);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_ELLIPSE: {
            SPAposition c = SPAposition(0.0, 0.0, 0.0);
            SPAposition m = SPAposition(1.0, 0.0, 0.0);
            status = api_curve_ellipse(c, m, 1.0, 0.0, 3.14, ptrEdge);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_CONIC: {
            SPAposition s = SPAposition(0.0, 0.0, 0.0);
            SPAunit_vector sv = SPAunit_vector(1.0, 1.0, 0.0);
            SPAposition e = SPAposition(1.0, 0.0, 0.0);
            SPAunit_vector ev = SPAunit_vector(1.0, -1.0, 0.0);
            status = api_mk_ed_conic(s, sv, e, ev, 0.5, ptrEdge);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_HELIX: {
            SPAposition s = SPAposition(0.0, 0.0, 0.0);
            SPAvector sv = SPAvector(1.0, 1.0, 0.0);
            SPAposition e = SPAposition(1.0, 0.0, 0.0);
            status = api_edge_helix(s, e, sv, 1.0, 0.5, 1, ptrEdge);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_SPIRAL: {
            SPAposition c = SPAposition(0.0, 0.0, 0.0);
            SPAvector cv = SPAvector(0.0, 0.0, 10.0);
            SPAposition s = SPAposition(5.0, 0.0, 5.0);
            status = api_edge_spiral(c, cv, s, 2.0, 50.0, ptrEdge);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_SPRING: {
            SPAposition d = SPAposition(0.0, 10.0, 0.0);
            SPAvector dv = SPAvector(0.0, 0.0, 10.0);
            SPAposition s = SPAposition(0.0, 0.0, 0.0);
            const char* law_equation1 = "5+(4/(1+((x/4)-5)^2))";
            law* ptrLaw = nullptr;
            status = api_str_to_law(law_equation1, &ptrLaw);
            if (!status.ok()) return;
            double thread_distance_array[10] = { 1.0, 2.0, 3.0, 1.0, 2.0, 3.0, 1.0, 2.0, 3.0, 1.0 };
            double rotation_angle_array[10] = { 1.0, 2.0, 3.0, 1.0, 2.0, 3.0, 1.0, 2.0, 3.0, 1.0 };
            double transition_height_array[10] = { 1.0, 2.0, 3.0, 1.0, 2.0, 3.0, 1.0, 2.0, 3.0, 1.0 };
            double transition_angle_array[10] = { 1.0, 2.0, 3.0, 1.0, 2.0, 3.0, 1.0, 2.0, 3.0, 1.0 };
            status = api_edge_spring_law(d, dv, s, ptrLaw, 1, 4, thread_distance_array, rotation_angle_array, transition_height_array, transition_angle_array, ptrEdge);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_LAWC: {
            const char* law_equation1 = "vec(cos(x),sin(x),x/5)";
            law* ptrLaw = nullptr;
            status = api_str_to_law(law_equation1, &ptrLaw);
            if (!status.ok()) return;
            status = api_edge_law(ptrLaw, 0.0, 50.0, ptrEdge);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_BSPLINE_INTRP: {
            splgrid* ptrSplgrid = ACIS_NEW splgrid();
            int rows_v = 0;
            int rows_u = 0;
            std::vector<SPAposition> pos;
            if (handles.size()) {
                rows_v = handles.size();
                rows_u = handles[0].size();
                for (auto p : handles) pos.insert(std::end(pos), std::begin(p), std::end(p));
            } else {
                rows_v = 5;
                rows_u = 5;
                for (int i = 0; i < rows_u; i++) {
                    std::vector<SPAposition> ps;
                    for (int j = 0; j < rows_v; j++) {
                        pos.push_back(SPAposition(-i, j, (i + j) % 2));
                        ps.push_back(pos[i * rows_v + j]);
                    }
                    handles.push_back(ps);
                }
            }
            ptrSplgrid->set_gridpts_array(&pos[0], rows_v, rows_u);
            ptrSplgrid->set_u_start_array(nullptr);
            ptrSplgrid->set_u_end_array(nullptr);
            ptrSplgrid->set_v_start_array(nullptr);
            ptrSplgrid->set_v_end_array(nullptr);
            status = api_face_spl_intrp(ptrSplgrid, ptrFace);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_BEZIER: {
            if (!handles.size()) {
                std::vector<SPAposition> ps;
                ps.push_back(SPAposition(-5.0, -4.0, 0.0));
                ps.push_back(SPAposition(-1.0, 3.0, 0.0));
                ps.push_back(SPAposition(1.0, 2.5, 0.0));
                ps.push_back(SPAposition(4.0, -6.0, 0.0));
                handles.push_back(ps);
            }
            status = api_curve_bezier(handles[0][0], handles[0][1], handles[0][2], handles[0][3], ptrEdge);
            if (!status.ok()) return;
            break;
        }
        case BASIC_ENTITIES::E_BSPLINE: {
            if (handles.size()) {
                status = api_curve_spline(handles[0].size(), &handles[0][0], nullptr, nullptr, ptrEdge);
                if (!status.ok()) return;
            } else {
                ptrEntity = ACIS_NEW ENTITY();
            }
            break;
        }
        default: {
            break;
        }
    }
    if (ptrBody)
        ptrEntity = ptrBody;
    else if (ptrFace) {
        ptrFace->set_cont(BOTH_OUTSIDE);
        ptrFace->set_sides(DOUBLE_SIDED);
        ptrEntity = ptrFace;
    } else if (ptrEdge)
        ptrEntity = ptrEdge;
}
