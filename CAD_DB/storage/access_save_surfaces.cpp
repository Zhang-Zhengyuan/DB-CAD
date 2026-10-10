#include "access_internal.hxx"

namespace AccessUtils {
namespace Save {
Node *createnode_spl_sur_subgraph(spl_sur *sur) {
    // 子类类型
    int subtype;
    switch (sur->type()) {
    case 44: {
        // rot_spl_sur
        myerror("不支持的spl_sur子类型rot_spl_sur。");
    }
    case 37: // exact_spl_sur/exactsur
    case 60: // exact_spl_sur/exactsur
    default: {
        subtype = 1; // exactsur
        break;
    }
    }
    // degree_u, degree_v
    int dim = 0, form_u = 0, form_v = 0, pole_u = 0, pole_v = 0, num_u = 0, num_v = 0, degree_u = 0, num_uknots = 0,
        degree_v = 0, num_vknots = 0;
    logical rational_u = 0, rational_v = 0;
    SPAposition *ctrlpts = nullptr;
    double *weights = nullptr, *uknots = nullptr, *vknots = nullptr;
    bs3_surface_to_array(sur->sur(), dim, rational_u, rational_v, form_u, form_v, pole_u, pole_v, num_u, num_v, ctrlpts,
                         weights, degree_u, num_uknots, uknots, degree_v, num_vknots, vknots, 0);
    // 样条开闭u,v
    int closed_u = sur->gme_get_closed_in_u(); // 0:open 1:closed 2:periodic 3:unknown
    int closed_v = sur->gme_get_closed_in_v();
    // u_singularity, v_singularity
    int u_singularity = sur->gme_get_u_singularity(); // 0:none 1:low 2:high 3:both 4:unknown
    int v_singularity = sur->gme_get_v_singularity();
    // 简化后节点向量的读取长度
    std::vector<double> knots_simplifier_u;
    for (int i = 0; i < num_uknots; i++) {
        double last = *(uknots + i);
        if (knots_simplifier_u.empty() || last != knots_simplifier_u[knots_simplifier_u.size() - 2]) {
            knots_simplifier_u.push_back(last);
            knots_simplifier_u.push_back(1);
        } else {
            knots_simplifier_u.back()++;
        }
    }
    knots_simplifier_u[1]--;
    knots_simplifier_u.back()--;
    int knots_simplifier_u_size = (int)knots_simplifier_u.size() / 2;
    std::vector<double> knots_simplifier_v;
    for (int i = 0; i < num_vknots; i++) {
        double last = *(vknots + i);
        if (knots_simplifier_v.empty() || last != knots_simplifier_v[knots_simplifier_v.size() - 2]) {
            knots_simplifier_v.push_back(last);
            knots_simplifier_v.push_back(1);
        } else {
            knots_simplifier_v.back()++;
        }
    }
    knots_simplifier_v[1]--;
    knots_simplifier_v.back()--;
    int knots_simplifier_v_size = (int)knots_simplifier_v.size() / 2;
    // 简化后节点向量
    size_t knots_simplifier_u_vec_size = knots_simplifier_u.size();
    mg_list *knots_simplifier_u_mglist = mg_list_make_empty(knots_simplifier_u_vec_size);
    for (size_t i = 0; i < knots_simplifier_u_vec_size; i++) {
        mg_list_append(knots_simplifier_u_mglist, mg_value_make_float(knots_simplifier_u[i]));
    }
    size_t knots_simplifier_v_vec_size = knots_simplifier_v.size();
    mg_list *knots_simplifier_v_mglist = mg_list_make_empty(knots_simplifier_v_vec_size);
    for (size_t i = 0; i < knots_simplifier_v_vec_size; i++) {
        mg_list_append(knots_simplifier_v_mglist, mg_value_make_float(knots_simplifier_v[i]));
    }
    // 控制点
    size_t ctrlpts_size;
    if (rational_u || rational_v) {
        ctrlpts_size = num_v * num_u * 4;
    } else {
        ctrlpts_size = num_v * num_u * 3;
    }
    mg_list *ctrlpts_mglist = mg_list_make_empty(ctrlpts_size);
    for (size_t i = 0; i < num_v; i++) {
        for (size_t j = 0; j < num_u; j++) {
            SPAposition *last_ptr = ctrlpts + j * num_v + i;
            mg_list_append(ctrlpts_mglist, mg_value_make_float(last_ptr->x()));
            mg_list_append(ctrlpts_mglist, mg_value_make_float(last_ptr->y()));
            mg_list_append(ctrlpts_mglist, mg_value_make_float(last_ptr->z()));
            if (rational_u || rational_v)
                mg_list_append(ctrlpts_mglist, mg_value_make_float(*(weights + j * num_v + i)));
        }
    }
    // fitol
    double fitol = sur->fitol();

    Node *ptrnode = new Node();
    ptrnode->labels.clear();
    ptrnode->labels.push_back("spl_sur");
    ptrnode->properties['a'] = mg_value_make_string("spl_sur");
    ptrnode->properties['b'] = mg_value_make_integer(subtype);
    ptrnode->properties['c'] = mg_value_make_integer(rational_u);
    ptrnode->properties['p'] = mg_value_make_integer(rational_v);
    ptrnode->properties['d'] = mg_value_make_integer(degree_u);
    ptrnode->properties['e'] = mg_value_make_integer(degree_v);
    ptrnode->properties['f'] = mg_value_make_integer(closed_u);
    ptrnode->properties['g'] = mg_value_make_integer(closed_v);
    ptrnode->properties['h'] = mg_value_make_integer(u_singularity);
    ptrnode->properties['i'] = mg_value_make_integer(v_singularity);
    ptrnode->properties['j'] = mg_value_make_integer(knots_simplifier_u_size);
    ptrnode->properties['k'] = mg_value_make_integer(knots_simplifier_v_size);
    ptrnode->properties['l'] = mg_value_make_list(knots_simplifier_u_mglist);
    ptrnode->properties['m'] = mg_value_make_list(knots_simplifier_v_mglist);
    ptrnode->properties['n'] = mg_value_make_list(ctrlpts_mglist);
    ptrnode->properties['o'] = mg_value_make_float(fitol);
    return ptrnode;
}

plane_data *get_plane_data(plane *sur) {
    plane_data *ans = new plane_data();
    ans->root_point = getmglist_SPAposition(sur->root_point, 3);
    ans->normal = getmglist_SPAunit_vector(sur->normal);
    ans->u_deriv = getmglist_SPAvector(sur->u_deriv);
    ans->reverse_v = sur->reverse_v;
    SPApar_box ranges = sur->gme_get_subset_range();
    ans->subset_range = getmglist_SPApar_box(ranges);
    return ans;
}

sphere_data *get_sphere_data(sphere *sur) {
    sphere_data *ans = new sphere_data();
    ans->centre = getmglist_SPAposition(sur->centre, 3);
    ans->radius = sur->radius;
    ans->uv_oridir = getmglist_SPAunit_vector(sur->uv_oridir);
    ans->pole_dir = getmglist_SPAunit_vector(sur->pole_dir);
    ans->reverse_v = sur->reverse_v;
    SPApar_box ranges = sur->gme_get_subset_range();
    ans->subset_range = getmglist_SPApar_box(ranges);
    return ans;
}

torus_data *get_torus_data(torus *sur) {
    torus_data *ans = new torus_data();
    ans->centre = getmglist_SPAposition(sur->centre, 3);
    ans->normal = getmglist_SPAunit_vector(sur->normal);
    ans->major_radius = sur->major_radius;
    ans->minor_radius = sur->minor_radius;
    ans->uv_oridir = getmglist_SPAunit_vector(sur->uv_oridir);
    ans->reverse_v = sur->reverse_v;
    SPApar_box ranges = sur->gme_get_subset_range();
    ans->subset_range = getmglist_SPApar_box(ranges);
    return ans;
}

cone_data *get_cone_data(cone *sur) {
    cone_data *ans = new cone_data();
    ellipse sur_base = sur->base;
    ans->base_centre = getmglist_SPAposition(sur_base.centre, 3);
    ans->base_normal = getmglist_SPAunit_vector(sur_base.normal);
    ans->base_major_axis = getmglist_SPAvector(sur_base.major_axis);
    ans->base_radius_ratio = sur_base.radius_ratio;
    SPAinterval range = sur_base.gme_get_subset_range();
    ans->base_subset_range = getmglist_SPAinterval(range);
    ans->sine_angle = sur->sine_angle;
    ans->cosine_angle = sur->cosine_angle;
    ans->reverse_u = sur->reverse_u;
    SPApar_box ranges = sur->gme_get_subset_range();
    ans->subset_range = getmglist_SPApar_box(ranges);
    return ans;
}

surface_data *get_surface_data_subgraph(std::unordered_map<void *, Node *> &ptr2node, surface *sur) {
    if (sur == nullptr) {
        surface_data *ans = new surface_data();
        ans->subtype = 0; // "null_surface";
        return ans;
    } else {
        switch (sur->type()) {
        case spline_type: {
            spline_data *ans = get_spline_data_subgraph(ptr2node, (spline *)sur);
            ans->subtype = 1; // "spline";
            return ans;
        } break;
        case plane_type: {
            plane_data *ans = get_plane_data((plane *)sur);
            ans->subtype = 2; // "plane";
            return ans;
        } break;
        case sphere_type: {
            sphere_data *ans = get_sphere_data((sphere *)sur);
            ans->subtype = 3; // "sphere";
            return ans;
        } break;
        case torus_type: {
            torus_data *ans = get_torus_data((torus *)sur);
            ans->subtype = 4; // "torus";
            return ans;
        } break;
        case cone_type: {
            cone_data *ans = get_cone_data((cone *)sur);
            ans->subtype = 5; // "cone";
            return ans;
        } break;
        default: {
            myerror("不支持的surface子类型。");
            throw std::runtime_error("Unsupported ACIS surface subtype");
        } break;
        }
    }
}
} // namespace Save
} // namespace AccessUtils
