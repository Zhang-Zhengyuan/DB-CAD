#include "access_internal.hxx"

namespace AccessUtils {
namespace Save {
spline_data *get_spline_data_subgraph(std::unordered_map<void *, Node *> &ptr2node, spline *sur) {
    spline_data *ans = new spline_data();
    ans->reversed = sur->reversed();
    SPApar_box ranges = sur->subset_box();
    ans->subset_range = getmglist_SPApar_box(ranges);
    ans->spl_node = nullptr;
    // get_spl_sur() 返回方程内部 spl_sur 的只读引用；保存侧直接读取其数据，
    // 不做拷贝（读取路径：bs3_surface_to_array 均为公有函数）。
    spl_sur *spl = (spl_sur *)&sur->get_spl_sur();
    if (spl != nullptr) {
        if (ptr2node.find(spl) == ptr2node.end()) {
            ptr2node[spl] = createnode_spl_sur_subgraph(spl); // 创建spl_sur节点
        }
        ans->spl_node = ptr2node.at(spl);
    }
    return ans;
}

bs2_curve_data get_bs2_curve_data(bs2_curve pcur) {
    bs2_curve_data ans;
    if (pcur == nullptr) {
        ans.subtype = 0; //"nullbs";
    } else {
        ans.subtype = 1;            //"nubs";
        int dim_pcur1;              // dimension
        int deg_pcur1;              // degree
        logical rat_pcur1;          // rational
        int num_ctrlpts_pcur1;      // number of control points
        SPAposition *ctrlpts_pcur1; // control points
        double *weights_pcur1;      // weights
        int num_knots_pcur1;        // number of knots
        double *knots_pcur1;        // knots
        bs2_curve_to_array(pcur, dim_pcur1, deg_pcur1, rat_pcur1, num_ctrlpts_pcur1, ctrlpts_pcur1, weights_pcur1,
                           num_knots_pcur1, knots_pcur1);
        ans.degree = deg_pcur1; // degree

        if (!bs2_curve_closed(pcur)) {
            ans.form = 0; // "open";
        } else if (!bs2_curve_periodic(pcur)) {
            ans.form = 1; // "closed";
        } else {
            ans.form = 2; // "periodic";
        }

        // 简化后节点向量的读取长度
        std::vector<double> knots_simplifier_pcur1;
        for (int i = 0; i < num_knots_pcur1; i++) {
            double last = *(knots_pcur1 + i);
            if (knots_simplifier_pcur1.empty() || last != knots_simplifier_pcur1[knots_simplifier_pcur1.size() - 2]) {
                knots_simplifier_pcur1.push_back(last);
                knots_simplifier_pcur1.push_back(1);
            } else {
                knots_simplifier_pcur1.back()++;
            }
        }
        knots_simplifier_pcur1[1]--;
        knots_simplifier_pcur1.back()--;
        ans.knots_simplifier_size = (int)knots_simplifier_pcur1.size() / 2;
        // 简化后节点向量
        size_t knots_simplifier_pcur1_size = knots_simplifier_pcur1.size();
        ans.knots_simplifier_mglist = mg_list_make_empty(knots_simplifier_pcur1_size);
        for (size_t i = 0; i < knots_simplifier_pcur1_size; i++) {
            mg_list_append(ans.knots_simplifier_mglist, mg_value_make_float(knots_simplifier_pcur1[i]));
        }

        // 控制点
        size_t ctrlpts_size = num_ctrlpts_pcur1 * 2;
        ans.ctrlpts_mglist = mg_list_make_empty(ctrlpts_size);
        for (int i = 0; i < num_ctrlpts_pcur1; i++) {
            SPAposition *last_ptr = ctrlpts_pcur1 + i;
            mg_list_append(ans.ctrlpts_mglist, mg_value_make_float(last_ptr->x()));
            mg_list_append(ans.ctrlpts_mglist, mg_value_make_float(last_ptr->y()));
        }
    }
    return ans;
}

Node *createnode_int_cur_subgraph(std::unordered_map<void *, Node *> &ptr2node,
                                  std::vector<Relationship *> &relationship_list, intcurve *icur) {
    // 子类类型：intcurve 公有 get_int_cur() 返回内部 int_cur 的只读引用，
    // type()/type_name() 是 subtype_object 的公有虚函数。
    const int_cur &cur = icur->get_int_cur();
    int subtype;
    switch (cur.type()) {
    case 22: // int_int_cur: mark as surfintcur
    case 31: {
        // int_int_cur: mark as surfintcur
        subtype = 31; //"surfintcur";
        break;
    }
    case 25: {
        // parcur
        subtype = 25; //"parcur";
        break;
    }
    case 20: // exact_int_cur, exactcur
    default: {
        subtype = 1; //"exactcur";
        break;
    }
    }
    // 样条类型：intcurve::cur() 返回逼近 bs3 曲线（公有）
    bs3_curve bs3_cur = icur->cur();
    int rational = bs3_curve_rational(bs3_cur); // 0:nurbs 1:nubs
    // degree
    int dim_cur = 0, deg_cur = 0, num_ctrlpts_cur = 0, num_knots_cur = 0;
    logical rat_cur = 0;
    SPAposition *ctrlpts_cur = nullptr;
    double *weights_cur = nullptr, *knots_cur = nullptr;
    bs3_curve_to_array(bs3_cur, dim_cur, deg_cur, rat_cur, num_ctrlpts_cur, ctrlpts_cur, weights_cur, num_knots_cur,
                       knots_cur);
    // 样条开闭
    int form;
    if (!bs3_curve_closed(bs3_cur)) {
        form = 0; // "open";
    } else if (!bs3_curve_periodic(bs3_cur)) {
        form = 1; // "closed";
    } else {
        form = 2; // "periodic";
    }

    // 简化后节点向量的读取长度
    std::vector<double> knots_simplifier;
    for (int i = 0; i < num_knots_cur; i++) {
        double last = *(knots_cur + i);
        if (knots_simplifier.empty() || last != knots_simplifier[knots_simplifier.size() - 2]) {
            knots_simplifier.push_back(last);
            knots_simplifier.push_back(1);
        } else {
            knots_simplifier.back()++;
        }
    }
    knots_simplifier[1]--;
    knots_simplifier.back()--;
    int knots_simplifier_size = (int)knots_simplifier.size() / 2;
    // 简化后节点向量
    size_t knots_simplifier_vec_size = knots_simplifier.size();
    mg_list *knots_simplifier_mglist = mg_list_make_empty(knots_simplifier_vec_size);
    for (size_t i = 0; i < knots_simplifier_vec_size; i++) {
        mg_list_append(knots_simplifier_mglist, mg_value_make_float(knots_simplifier[i]));
    }

    // 控制点
    size_t ctrlpts_size = num_ctrlpts_cur * 4;
    mg_list *ctrlpts_mglist = mg_list_make_empty(ctrlpts_size);
    for (int i = 0; i < num_ctrlpts_cur; i++) {
        SPAposition *last_ptr = ctrlpts_cur + i;
        mg_list_append(ctrlpts_mglist, mg_value_make_float(last_ptr->x()));
        mg_list_append(ctrlpts_mglist, mg_value_make_float(last_ptr->y()));
        mg_list_append(ctrlpts_mglist, mg_value_make_float(last_ptr->z()));
        if (rational)
            mg_list_append(ctrlpts_mglist, mg_value_make_float(*(weights_cur + i)));
    }
    // fitol：intcurve 公有 fitol()
    double fitol = icur->fitol();
    // surf1/surf2：intcurve::pcur(1/2) 返回堆上的 pcurve，其公有 surf() 给出定义曲面；
    // 曲面指针由 pcurve 拥有，保存侧只读取内容。
    surface *surf1 = nullptr;
    surface *surf2 = nullptr;
    pcurve *pc1 = icur->pcur(1, TRUE);
    pcurve *pc2 = icur->pcur(2, TRUE);
    if (pc1 != nullptr)
        surf1 = (surface *)&pc1->surf();
    if (pc2 != nullptr)
        surf2 = (surface *)&pc2->surf();
    surface_data *surf1_data = get_surface_data_subgraph(ptr2node, surf1);
    surface_data *surf2_data = get_surface_data_subgraph(ptr2node, surf2);
    // pcur1/pcur2：参数域曲线，pcurve 公有 cur()
    bs2_curve pcur1 = (pc1 != nullptr) ? pc1->cur() : nullptr;
    bs2_curve pcur2 = (pc2 != nullptr) ? pc2->cur() : nullptr;
    bs2_curve_data pcur1_data = get_bs2_curve_data(pcur1);
    bs2_curve_data pcur2_data = get_bs2_curve_data(pcur2);
    // safe_range：intcurve 公有 safe_range()
    SPAinterval safe_range = icur->safe_range();
    mg_list *safe_range_jsonarray = getmglist_SPAinterval(safe_range);
    // 曲线所在曲面
    int surface_tag = 0;
    if (surf1 != nullptr && surf2 == nullptr) {
        surface_tag = 1;
    } else if (surf1 == nullptr && surf2 != nullptr) {
        surface_tag = 2;
    }

    Node *ptrnode = new Node();
    ptrnode->labels.clear();
    ptrnode->labels.push_back("int_cur");
    ptrnode->properties['a'] = mg_value_make_string("int_cur");
    ptrnode->properties['b'] = mg_value_make_integer(subtype);
    ptrnode->properties['c'] = mg_value_make_integer(rational);
    ptrnode->properties['d'] = mg_value_make_integer(deg_cur);
    ptrnode->properties['e'] = mg_value_make_integer(form);
    ptrnode->properties['f'] = mg_value_make_integer(knots_simplifier_size);
    ptrnode->properties['g'] = mg_value_make_list(knots_simplifier_mglist);
    ptrnode->properties['h'] = mg_value_make_list(ctrlpts_mglist);
    ptrnode->properties['i'] = mg_value_make_float(fitol);
    ptrnode->properties['j'] = mg_value_make_integer(surf1_data->subtype);
    ptrnode->properties['k'] = mg_value_make_integer(surf2_data->subtype);
    ptrnode->properties['l'] = mg_value_make_integer(pcur1_data.subtype);
    ptrnode->properties['m'] = mg_value_make_integer(pcur2_data.subtype);
    ptrnode->properties['n'] = mg_value_make_list(safe_range_jsonarray);

    if (surf1_data->subtype == 2) {
        plane_data *sd = (plane_data *)surf1_data;
        ptrnode->properties['q'] = mg_value_make_list(sd->root_point);
        ptrnode->properties['r'] = mg_value_make_list(sd->normal);
        ptrnode->properties['s'] = mg_value_make_list(sd->u_deriv);
        ptrnode->properties['t'] = mg_value_make_integer(sd->reverse_v);
        ptrnode->properties['p'] = mg_value_make_list(sd->subset_range);
    } else if (surf1_data->subtype == 3) {
        sphere_data *sd = (sphere_data *)surf1_data;
        ptrnode->properties['q'] = mg_value_make_list(sd->centre);
        ptrnode->properties['r'] = mg_value_make_float(sd->radius);
        ptrnode->properties['s'] = mg_value_make_list(sd->uv_oridir);
        ptrnode->properties['t'] = mg_value_make_list(sd->pole_dir);
        ptrnode->properties['u'] = mg_value_make_integer(sd->reverse_v);
        ptrnode->properties['p'] = mg_value_make_list(sd->subset_range);
    } else if (surf1_data->subtype == 4) {
        torus_data *sd = (torus_data *)surf1_data;
        ptrnode->properties['q'] = mg_value_make_list(sd->centre);
        ptrnode->properties['r'] = mg_value_make_list(sd->normal);
        ptrnode->properties['s'] = mg_value_make_float(sd->major_radius);
        ptrnode->properties['t'] = mg_value_make_float(sd->minor_radius);
        ptrnode->properties['u'] = mg_value_make_list(sd->uv_oridir);
        ptrnode->properties['v'] = mg_value_make_integer(sd->reverse_v);
        ptrnode->properties['p'] = mg_value_make_list(sd->subset_range);
    } else if (surf1_data->subtype == 5) {
        cone_data *sd = (cone_data *)surf1_data;
        ptrnode->properties['q'] = mg_value_make_list(sd->base_centre);
        ptrnode->properties['r'] = mg_value_make_list(sd->base_normal);
        ptrnode->properties['s'] = mg_value_make_list(sd->base_major_axis);
        ptrnode->properties['t'] = mg_value_make_float(sd->base_radius_ratio);
        ptrnode->properties['u'] = mg_value_make_list(sd->base_subset_range);
        ptrnode->properties['v'] = mg_value_make_float(sd->sine_angle);
        ptrnode->properties['w'] = mg_value_make_float(sd->cosine_angle);
        ptrnode->properties['x'] = mg_value_make_integer(sd->reverse_u);
        ptrnode->properties['p'] = mg_value_make_list(sd->subset_range);
    } else if (surf1_data->subtype == 1) {
        spline_data *sd = (spline_data *)surf1_data;
        ptrnode->properties['q'] = mg_value_make_integer(sd->reversed);
        ptrnode->properties['p'] = mg_value_make_list(sd->subset_range);
        if (sd->spl_node != nullptr) {
            Node *b = sd->spl_node;
            Relationship *r = new Relationship(ptrnode, b);
            r->label = "int_cur_surf1_spl_ptr";
            r->properties['a'] = mg_value_make_string("int_cur_surf1_spl_ptr");
            relationship_list.push_back(r);
        }
    }

    if (surf2_data->subtype == 2) {
        plane_data *sd = (plane_data *)surf2_data;
        ptrnode->properties['z'] = mg_value_make_list(sd->root_point);
        ptrnode->properties['A'] = mg_value_make_list(sd->normal);
        ptrnode->properties['B'] = mg_value_make_list(sd->u_deriv);
        ptrnode->properties['C'] = mg_value_make_integer(sd->reverse_v);
        ptrnode->properties['y'] = mg_value_make_list(sd->subset_range);
    } else if (surf2_data->subtype == 3) {
        sphere_data *sd = (sphere_data *)surf2_data;
        ptrnode->properties['z'] = mg_value_make_list(sd->centre);
        ptrnode->properties['A'] = mg_value_make_float(sd->radius);
        ptrnode->properties['B'] = mg_value_make_list(sd->uv_oridir);
        ptrnode->properties['C'] = mg_value_make_list(sd->pole_dir);
        ptrnode->properties['D'] = mg_value_make_integer(sd->reverse_v);
        ptrnode->properties['y'] = mg_value_make_list(sd->subset_range);
    } else if (surf2_data->subtype == 4) {
        torus_data *sd = (torus_data *)surf2_data;
        ptrnode->properties['z'] = mg_value_make_list(sd->centre);
        ptrnode->properties['A'] = mg_value_make_list(sd->normal);
        ptrnode->properties['B'] = mg_value_make_float(sd->major_radius);
        ptrnode->properties['C'] = mg_value_make_float(sd->minor_radius);
        ptrnode->properties['D'] = mg_value_make_list(sd->uv_oridir);
        ptrnode->properties['E'] = mg_value_make_integer(sd->reverse_v);
        ptrnode->properties['y'] = mg_value_make_list(sd->subset_range);
    } else if (surf2_data->subtype == 5) {
        cone_data *sd = (cone_data *)surf2_data;
        ptrnode->properties['z'] = mg_value_make_list(sd->base_centre);
        ptrnode->properties['A'] = mg_value_make_list(sd->base_normal);
        ptrnode->properties['B'] = mg_value_make_list(sd->base_major_axis);
        ptrnode->properties['C'] = mg_value_make_float(sd->base_radius_ratio);
        ptrnode->properties['D'] = mg_value_make_list(sd->base_subset_range);
        ptrnode->properties['E'] = mg_value_make_float(sd->sine_angle);
        ptrnode->properties['F'] = mg_value_make_float(sd->cosine_angle);
        ptrnode->properties['G'] = mg_value_make_integer(sd->reverse_u);
        ptrnode->properties['y'] = mg_value_make_list(sd->subset_range);
    } else if (surf2_data->subtype == 1) {
        spline_data *sd = (spline_data *)surf2_data;
        ptrnode->properties['z'] = mg_value_make_integer(sd->reversed);
        ptrnode->properties['y'] = mg_value_make_list(sd->subset_range);
        if (sd->spl_node != nullptr) {
            Node *b = sd->spl_node;
            Relationship *r = new Relationship(ptrnode, b);
            r->label = "int_cur_surf2_spl_ptr";
            r->properties['a'] = mg_value_make_string("int_cur_surf2_spl_ptr");
            relationship_list.push_back(r);
        }
    }

    if (pcur1_data.subtype == 1) {
        ptrnode->properties['H'] = mg_value_make_integer(pcur1_data.degree);
        ptrnode->properties['I'] = mg_value_make_integer(pcur1_data.form);
        ptrnode->properties['J'] = mg_value_make_integer(pcur1_data.knots_simplifier_size);
        ptrnode->properties['K'] = mg_value_make_list(pcur1_data.knots_simplifier_mglist);
        ptrnode->properties['L'] = mg_value_make_list(pcur1_data.ctrlpts_mglist);
    }

    if (pcur2_data.subtype == 1) {
        ptrnode->properties['M'] = mg_value_make_integer(pcur2_data.degree);
        ptrnode->properties['N'] = mg_value_make_integer(pcur2_data.form);
        ptrnode->properties['O'] = mg_value_make_integer(pcur2_data.knots_simplifier_size);
        ptrnode->properties['P'] = mg_value_make_list(pcur2_data.knots_simplifier_mglist);
        ptrnode->properties['Q'] = mg_value_make_list(pcur2_data.ctrlpts_mglist);
    }

    ptrnode->properties['o'] = mg_value_make_integer(surface_tag);

    // pcur(1/2) 返回堆对象（temporary=TRUE 时为临时对象），由本函数负责销毁。
    if (pc1 != nullptr)
        delete pc1;
    if (pc2 != nullptr)
        delete pc2;
    delete surf1_data;
    delete surf2_data;

    return ptrnode;
}

Node *createnode_par_cur_subgraph(std::unordered_map<void *, Node *> &ptr2node,
                                  std::vector<Relationship *> &relationship_list, pcurve *pcrv) {
    // pcurve 方程类公有 get_par_cur() 给出 par_cur 定义；type() 为 subtype_object 公有虚函数。
    const par_cur &cur = pcrv->get_par_cur();
    switch (cur.type()) {
    case 32: {
        // exp_par_cur//exppc
        // 子类类型
        int subtype = 32; // exppc

        // 公有读取器：cur()/fitol()/surf()
        bs2_curve bs2_cur = pcrv->cur();
        bs2_curve_data bs2_cur_data = get_bs2_curve_data(bs2_cur);

        // 样条类型
        int rational = bs2_cur_data.subtype; // rational

        // fitol
        double fitol = pcrv->fitol();

        surface const &surf = pcrv->surf();
        surface_data *surf_data = get_surface_data_subgraph(ptr2node, (surface *)&surf);

        Node *ptrnode = new Node();
        ptrnode->labels.clear();
        ptrnode->labels.push_back("par_cur");
        ptrnode->properties['a'] = mg_value_make_string("par_cur");
        ptrnode->properties['b'] = mg_value_make_integer(subtype);
        ptrnode->properties['c'] = mg_value_make_integer(rational);
        ptrnode->properties['d'] = mg_value_make_float(fitol);
        ptrnode->properties['e'] = mg_value_make_integer(surf_data->subtype);

        if (rational == 1) {
            // nubs
            ptrnode->properties['f'] = mg_value_make_integer(bs2_cur_data.degree);
            ptrnode->properties['g'] = mg_value_make_integer(bs2_cur_data.form);
            ptrnode->properties['h'] = mg_value_make_integer(bs2_cur_data.knots_simplifier_size);
            ptrnode->properties['i'] = mg_value_make_list(bs2_cur_data.knots_simplifier_mglist);
            ptrnode->properties['j'] = mg_value_make_list(bs2_cur_data.ctrlpts_mglist);
        }

        if (surf_data->subtype == 2) {
            plane_data *sd = (plane_data *)surf_data;
            ptrnode->properties['l'] = mg_value_make_list(sd->root_point);
            ptrnode->properties['m'] = mg_value_make_list(sd->normal);
            ptrnode->properties['n'] = mg_value_make_list(sd->u_deriv);
            ptrnode->properties['o'] = mg_value_make_integer(sd->reverse_v);
            ptrnode->properties['k'] = mg_value_make_list(sd->subset_range);
        } else if (surf_data->subtype == 3) {
            sphere_data *sd = (sphere_data *)surf_data;
            ptrnode->properties['l'] = mg_value_make_list(sd->centre);
            ptrnode->properties['m'] = mg_value_make_float(sd->radius);
            ptrnode->properties['n'] = mg_value_make_list(sd->uv_oridir);
            ptrnode->properties['o'] = mg_value_make_list(sd->pole_dir);
            ptrnode->properties['p'] = mg_value_make_integer(sd->reverse_v);
            ptrnode->properties['k'] = mg_value_make_list(sd->subset_range);
        } else if (surf_data->subtype == 4) {
            torus_data *sd = (torus_data *)surf_data;
            ptrnode->properties['l'] = mg_value_make_list(sd->centre);
            ptrnode->properties['m'] = mg_value_make_list(sd->normal);
            ptrnode->properties['n'] = mg_value_make_float(sd->major_radius);
            ptrnode->properties['o'] = mg_value_make_float(sd->minor_radius);
            ptrnode->properties['p'] = mg_value_make_list(sd->uv_oridir);
            ptrnode->properties['q'] = mg_value_make_integer(sd->reverse_v);
            ptrnode->properties['k'] = mg_value_make_list(sd->subset_range);
        } else if (surf_data->subtype == 5) {
            cone_data *sd = (cone_data *)surf_data;
            ptrnode->properties['l'] = mg_value_make_list(sd->base_centre);
            ptrnode->properties['m'] = mg_value_make_list(sd->base_normal);
            ptrnode->properties['n'] = mg_value_make_list(sd->base_major_axis);
            ptrnode->properties['o'] = mg_value_make_float(sd->base_radius_ratio);
            ptrnode->properties['p'] = mg_value_make_list(sd->base_subset_range);
            ptrnode->properties['q'] = mg_value_make_float(sd->sine_angle);
            ptrnode->properties['r'] = mg_value_make_float(sd->cosine_angle);
            ptrnode->properties['s'] = mg_value_make_integer(sd->reverse_u);
            ptrnode->properties['k'] = mg_value_make_list(sd->subset_range);
        } else if (surf_data->subtype == 1) {
            spline_data *sd = (spline_data *)surf_data;
            ptrnode->properties['l'] = mg_value_make_integer(sd->reversed);
            ptrnode->properties['k'] = mg_value_make_list(sd->subset_range);
            if (sd->spl_node != nullptr) {
                Node *b = sd->spl_node;
                Relationship *r = new Relationship(ptrnode, b);
                r->label = "par_cur_surf_spl_ptr";
                r->properties['a'] = mg_value_make_string("par_cur_surf_spl_ptr");
                relationship_list.push_back(r);
            }
        }
        delete surf_data;
        return ptrnode;
    } break;
    default: {
        myerror("不支持的par_cur子类型。");
        throw std::runtime_error("Unsupported ACIS par_cur subtype");
    } break;
    }
}
} // namespace Save
} // namespace AccessUtils
