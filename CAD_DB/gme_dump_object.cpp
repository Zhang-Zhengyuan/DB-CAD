#include "gme_dump_object.hxx"

std::string indent_mapping(int indents)
{
    std::string ind;
    for (int i = 0; i < indents; i++)
    {
        ind.append("    ");
    }
    return ind;
}

template <typename... Args>
void fprintf_tabs(FILE* f, int indents, Args... args)
{
    if (f)
    {
        fprintf(f, "%s", indent_mapping(indents).c_str());
        fprintf(f, args...);
    }
}

// 基础

void dump_object(FILE* f, int indents, SPAposition const position)
{
    if (f)
    {
        fprintf_tabs(f, indents, "[x: %f, y: %f, z: %f]\n", position.x(), position.y(), position.z());
    }
}

void dump_object(FILE* f, int indents, SPAvector const vector)
{
    if (f)
    {
        fprintf_tabs(f, indents, "[%f, %f, %f]\n", vector.x(), vector.y(), vector.z());
    }
}

void dump_object(FILE* f, int indents, SPAmatrix const matrix)
{
    if (f)
    {
        dump_object(f, indents, matrix.row(0));
        dump_object(f, indents, matrix.row(1));
        dump_object(f, indents, matrix.row(2));
    }
}

void dump_object(FILE* f, int indents, SPAinterval const interval)
{
    if (f)
    {
        if (interval.finite_below())
            fprintf_tabs(f, indents, "[%f, ", interval.start_pt());
        else
            fprintf_tabs(f, indents, "[INF, ");
        if (interval.finite_above())
            fprintf(f, "%f]\n", interval.end_pt());
        else
            fprintf(f, "INF]\n");
    }
}

void dump_object(FILE* f, int indents, SPAbox* const box)
{
    if (f && box)
    {
        if (box->bounded())
        {
            fprintf_tabs(f, indents, "corner:\n");
            for (int i = 0; i < 8; i++)
            {
                dump_object(f, indents + 1, box->corner(i));
            }
            fprintf_tabs(f, indents, "x_range:");
            dump_object(f, 0, box->x_range());
            fprintf_tabs(f, indents, "y_range:");
            dump_object(f, 0, box->y_range());
            fprintf_tabs(f, indents, "z_range:");
            dump_object(f, 0, box->z_range());
        }
        else
            fprintf_tabs(f, indents, "unbounded\n");
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

void dump_object(FILE* f, int indents, law* l)
{
    if (f && l)
    {
        fprintf_tabs(f, indents, "class_name: %s\n", l->class_name());
        fprintf_tabs(f, indents, "how_many_laws: %d\n", l->how_many_laws);
        fprintf_tabs(f, indents, "id: %d\n", l->id());
#ifndef LAW_VIRTUAL_FUNCTION
        // @todo: law模块支撑测试的时候把type()给注释调了, 但是这里又有调用这个方法
        fprintf_tabs(f, indents, "type: %d\n", l->type());
#endif
        fprintf_tabs(f, indents, "string: %s\n", l->string());
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

// 几何（非持久化）

void dump_object(FILE* f, int indents, bs2_curve_def* c)
{
    /* @todo: 实现部分bs2_curve_def成员函数，输出部分参数值*/
    if (f && c)
    {
        fprintf_tabs(f, indents, "***unsupported bs2_curve_def***\n");
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

void dump_object(FILE* f, int indents, bs3_curve_def* c)
{
    /* @todo: 实现部分bs3_curve_def成员函数，输出部分参数值*/
    if (f && c)
    {
        fprintf_tabs(f, indents, "***unsupported bs3_curve_def***\n");
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

void dump_object(FILE* f, int indents, intcurve* i)
{
    if (f && i)
    {
        fprintf_tabs(f, indents, "**********DEBUG(start)**********\n");
        fprintf_tabs(f, indents, "");
        i->debug(indent_mapping(indents).c_str(), f);
        fprintf_tabs(f, 0, "\n");
        fprintf_tabs(f, indents, "**********DEBUG(end)**********\n");
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

void dump_object(FILE* f, int indents, const curve* c)
{
    if (f && c)
    {
        std::string type_name = c->type_name();
        fprintf_tabs(f, indents, "type_name: %s\n", type_name.c_str());
        fprintf_tabs(f, indents, "type: %d\n", c->type());
        fprintf_tabs(f, indents, "accurate_derivs: %d\n", c->accurate_derivs());
        fprintf_tabs(f, indents, "approx_error: %f\n", c->approx_error());
        fprintf_tabs(f, indents, "closed: %d\n", c->closed());
        fprintf_tabs(f, indents, "param_period: %f\n", c->param_period());
        fprintf_tabs(f, indents, "periodic: %d\n", c->periodic());
        fprintf_tabs(f, indents, "undef: %d\n", c->undef());
        fprintf_tabs(f, indents, "undefined: %d\n", c->undefined());
        // c->law_form()报读取访问权限冲突错误
        // fprintf_tabs(f, indents, "law:\n");
        // dump_object(f, indents + 1, c->law_form());
        if (type_name.compare("straight") == 0)
        {
            straight* s = (straight*)&c;
            fprintf_tabs(f, indents, "direction: ");
            dump_object(f, 0, s->direction);
            fprintf_tabs(f, indents, "root_point: ");
            dump_object(f, 0, s->root_point);
            fprintf_tabs(f, indents, "param_scale: %f\n", s->param_scale);
        }
        else if (type_name.compare("ellipse") == 0)
        {
            ellipse* e = (ellipse*)&c;
            fprintf_tabs(f, indents, "centre: ");
            dump_object(f, 0, e->centre);
            fprintf_tabs(f, indents, "major_axis: ");
            dump_object(f, 0, e->major_axis);
            fprintf_tabs(f, indents, "major_axis_length: %f\n", e->major_axis_length);
            fprintf_tabs(f, indents, "normal: ");
            dump_object(f, 0, e->normal);
            fprintf_tabs(f, indents, "param_off: %f\n", e->param_off);
            fprintf_tabs(f, indents, "minor_axis: ");
            dump_object(f, 0, e->minor_axis);
        }
        else if (type_name == "helix")
        {
            helix* h = (helix*)&c;
            fprintf_tabs(f, indents, "param_period: %f\n", h->param_period());
            fprintf_tabs(f, indents, "periodic: %d\n", h->periodic());
            fprintf_tabs(f, indents, "helix_range: ");
            dump_object(f, 0, h->helix_range());
            fprintf_tabs(f, indents, "axis_root: ");
            dump_object(f, 0, h->axis_root());
            fprintf_tabs(f, indents, "axis_dir: ");
            dump_object(f, 0, h->axis_dir());
            fprintf_tabs(f, indents, "start_disp: ");
            dump_object(f, 0, h->start_disp());
            fprintf_tabs(f, indents, "pitch: %f\n", h->pitch());
            fprintf_tabs(f, indents, "handedness: %d\n", h->handedness());
            fprintf_tabs(f, indents, "par_scaling: %f\n", h->par_scaling());
            fprintf_tabs(f, indents, "taper: %f\n", h->taper());
            fprintf_tabs(f, indents, "radius: %f\n", h->radius());
            fprintf_tabs(f, indents, "maj_dir: ");
            dump_object(f, 0, h->maj_dir());
        }
        else if (type_name.ends_with("intcurve"))
        {
            intcurve* i = (intcurve*)&c;
            fprintf_tabs(f, indents, "bs1_hull_angles_ok: %d\n", i->bs1_hull_angles_ok());
            fprintf_tabs(f, indents, "bs1_hull_self_intersects: %d\n", i->bs1_hull_self_intersects());
            fprintf_tabs(f, indents, "bs1_knots_on_curve: %d\n", i->bs1_knots_on_curve());
            fprintf_tabs(f, indents, "bs2_hull_angles_ok: %d\n", i->bs2_hull_angles_ok());
            fprintf_tabs(f, indents, "bs2_hull_self_intersects: %d\n", i->bs2_hull_self_intersects());
            fprintf_tabs(f, indents, "bs2_knots_on_curve: %d\n", i->bs2_knots_on_curve());
            fprintf_tabs(f, indents, "cur:\n");
            // ag_spline结构未公开，导致无法直接导出bs3_curve_def* bs3_curve的结构
            // dump_object(f, indents + 1, i->cur());
            // 使用intcurve自带的debug接口导出bs3_curve数据
            dump_object(f, indents + 1, i);
            fprintf_tabs(f, indents, "fitol: %f\n", i->fitol());
            fprintf_tabs(f, indents, "surf1:\n");
            dump_object(f, indents + 1, &(i->surf1()));
            fprintf_tabs(f, indents, "surf2:\n");
            dump_object(f, indents + 1, &(i->surf2()));
        }
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

void dump_object(FILE* f, int indents, const pcurve* p)
{
    if (f && p)
    {
        fprintf_tabs(f, indents, "accurate_knot_tangents: %d\n", p->accurate_knot_tangents());
        fprintf_tabs(f, indents, "fitol: %f\n", p->fitol());
        fprintf_tabs(f, indents, "cur:\n");
        // ag_spline结构未公开，导致无法直接导出bs2_curve_def* bs2_curve的结构
        // dump_object(f, indents + 1, p->cur());
        // 使用pcurve自带的debug接口导出bs2_curve数据
        fprintf_tabs(f, indents, "**********DEBUG(start)**********\n");
        fprintf_tabs(f, indents, "");
        p->debug(indent_mapping(indents).c_str(), f);
        fprintf_tabs(f, 0, "\n");
        fprintf_tabs(f, indents, "**********DEBUG(end)**********\n");
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

void dump_object(FILE* f, int indents, const surface* s)
{
    if (f && s)
    {
        std::string type_name = s->type_name();
        if (type_name.compare("plane") == 0)
        {
            plane* p = (plane*)s;
            fprintf_tabs(f, indents, "root_point: ");
            dump_object(f, 0, p->root_point);
            fprintf_tabs(f, indents, "normal: ");
            dump_object(f, 0, p->normal);
            fprintf_tabs(f, indents, "u_deriv: ");
            dump_object(f, 0, p->u_deriv);
            fprintf_tabs(f, indents, "reverse_v: %s\n", p->reverse_v ? "true" : "false");
        }
        else if (type_name.compare("sphere") == 0)
        {
            sphere* t = (sphere*)s;
            fprintf_tabs(f, indents, "centre: ");
            dump_object(f, 0, t->centre);
            fprintf_tabs(f, indents, "radius: %f\n", t->radius);
            fprintf_tabs(f, indents, "uv_oridir: ");
            dump_object(f, 0, t->uv_oridir);
            fprintf_tabs(f, indents, "pole_dir: ");
            dump_object(f, 0, t->pole_dir);
            fprintf_tabs(f, indents, "reverse_v: %s\n", t->reverse_v ? "true" : "false");
        }
        else if (type_name.compare("cone") == 0)
        {
            cone* c = (cone*)s;
            fprintf_tabs(f, indents, "base: \n");
            dump_object(f, indents + 1, (ellipse*)&c->base);
            fprintf_tabs(f, indents, "cosine_angle: %f\n", c->cosine_angle);
            fprintf_tabs(f, indents, "_IsCylinder: %s\n", c->_IsCylinder ? "true" : "false");
            fprintf_tabs(f, indents, "reverse_u: %s\n", c->reverse_u ? "true" : "false");
            fprintf_tabs(f, indents, "sine_angle: %f\n", c->sine_angle);
            fprintf_tabs(f, indents, "u_param_scale: %f\n", c->u_param_scale);
        }
        else if (type_name.compare("torus") == 0)
        {
            torus* t = (torus*)s;
            fprintf_tabs(f, indents, "centre: ");
            dump_object(f, 0, t->centre);
            fprintf_tabs(f, indents, "normal: ");
            dump_object(f, 0, t->normal);
            fprintf_tabs(f, indents, "major_radius: %f\n", t->major_radius);
            fprintf_tabs(f, indents, "minor_radius: %f\n", t->minor_radius);
            fprintf_tabs(f, indents, "uv_oridir: ");
            fprintf_tabs(f, indents, "reverse_v: %s\n", t->reverse_v ? "true" : "false");
            dump_object(f, 0, t->uv_oridir);
        }
        else if (type_name.ends_with("spline"))
        {
            fprintf_tabs(f, indents, "**********DEBUG(start)**********\n");
            fprintf_tabs(f, indents, "");
            s->debug(indent_mapping(indents).c_str(), f);
            fprintf_tabs(f, 0, "\n");
            fprintf_tabs(f, indents, "**********DEBUG(end)**********\n");
        }
        else if (type_name.compare("meshsurf") == 0)
        {
            /* @todo: 暂未实现*/
            meshsurf* m = (meshsurf*)s;
            fprintf_tabs(f, indents, "暂未实现\n");
        }
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

void dump_object(FILE* f, int indents, const SPAtransf* transf)
{
    if (f && transf)
    {
        fprintf_tabs(f, indents, "affine:\n");
        dump_object(f, indents + 1, transf->affine());
        fprintf_tabs(f, indents, "translation:\n");
        dump_object(f, indents + 1, transf->translation());
        fprintf_tabs(f, indents, "scaling: %f\n", transf->scaling());
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

// 几何（持久化）

void dump_object(FILE* f, int indents, class APOINT* const entity)
{
    if (f && entity)
    {
        dump_object(f, indents, entity->coords());
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

void dump_object(FILE* f, int indents, class CURVE* entity)
{
    if (f && entity)
    {
        fprintf_tabs(f, indents, "type_name: %s\n", entity->type_name());
        fprintf_tabs(f, indents, "identity: %d\n", entity->identity());
        fprintf_tabs(f, indents, "use_count: %d\n", entity->use_count());
        fprintf_tabs(f, indents, "equation:\n");
        dump_object(f, indents + 1, &(entity->equation()));
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

void dump_object(FILE* f, int indents, class PCURVE* entity)
{
    if (f && entity)
    {
        fprintf_tabs(f, indents, "type_name: %s\n", entity->type_name());
        fprintf_tabs(f, indents, "identity: %d\n", entity->identity());
        fprintf_tabs(f, indents, "index: %d\n", entity->index());
        fprintf_tabs(f, indents, "use_count: %d\n", entity->use_count());
        fprintf_tabs(f, indents, "def_pcur: \n");
        dump_object(f, indents + 1, &(entity->def_pcur()));
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

void dump_object(FILE* f, int indents, class SURFACE* entity)
{
    if (f && entity)
    {
        std::string type_name = entity->type_name();
        fprintf_tabs(f, indents, "type_name: %s\n", type_name.c_str());
        fprintf_tabs(f, indents, "equation:\n");
        dump_object(f, indents + 1, &(entity->equation()));
        fprintf_tabs(f, indents, "identity: %d\n", entity->identity());
        fprintf_tabs(f, indents, "pattern_index: %d\n", entity->pattern_index());
        fprintf_tabs(f, indents, "size: %d\n", entity->size());
        fprintf_tabs(f, indents, "use_count: %d\n", entity->use_count());
        fprintf_tabs(f, indents, "make_box:\n");
        SPAbox entity_box = entity->make_box();
        dump_object(f, indents + 1, &entity_box);
        if (type_name.compare("plane") == 0)
        {
            class PLANE* p = (class PLANE*)entity;
            fprintf_tabs(f, indents, "root_point: ");
            dump_object(f, 0, p->root_point());
            fprintf_tabs(f, indents, "normal: ");
            dump_object(f, 0, p->normal());
        }
        else if (type_name.compare("sphere") == 0)
        {
            class SPHERE* s = (class SPHERE*)entity;
            fprintf_tabs(f, indents, "centre: ");
            dump_object(f, 0, s->centre());
            fprintf_tabs(f, indents, "radius: %f\n", s->radius());
        }
        else if (type_name.compare("cone") == 0)
        {
            class CONE* c = (class CONE*)entity;
            fprintf_tabs(f, indents, "root_point: ");
            dump_object(f, 0, c->root_point());
            fprintf_tabs(f, indents, "direction: ");
            dump_object(f, 0, c->direction());
            fprintf_tabs(f, indents, "major_axis: ");
            dump_object(f, 0, c->major_axis());
            fprintf_tabs(f, indents, "radius_ratio: %f\n", c->radius_ratio());
            fprintf_tabs(f, indents, "sine_angle: %f\n", c->sine_angle());
            fprintf_tabs(f, indents, "cosine_angle: %f\n", c->cosine_angle());
        }
        else if (type_name.compare("torus") == 0)
        {
            class TORUS* t = (class TORUS*)entity;
            fprintf_tabs(f, indents, "centre: ");
            dump_object(f, 0, t->centre());
            fprintf_tabs(f, indents, "normal: ");
            dump_object(f, 0, t->normal());
            fprintf_tabs(f, indents, "major_radius: %f\n", t->major_radius());
            fprintf_tabs(f, indents, "minor_radius: %f\n", t->minor_radius());
        }
        else if (type_name.ends_with("spline") == 0)
        {
            class SPLINE* s = (class SPLINE*)entity;
            fprintf_tabs(f, indents, "equation:\n");
            dump_object(f, indents + 1, &s->equation());
        }
        else if (type_name.compare("meshsurf") == 0)
        {
            /* @todo: 暂未实现*/
            class MESHSURF* p = (class MESHSURF*)entity;
            fprintf_tabs(f, indents, "暂未实现\n");
        }
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

void dump_object(FILE* f, int indents, class TRANSFORM* entity)
{
    if (f && entity)
    {
        fprintf_tabs(f, indents, "identity: %d\n", entity->identity());
        fprintf_tabs(f, indents, "transform(SPAtransf):\n");
        dump_object(f, indents + 1, &(entity->transform()));
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

// 拓扑

void dump_object(FILE* f, int indents, class VERTEX* const entity)
{
    if (f && entity)
    {
        fprintf_tabs(f, indents, "geometry: ");
        dump_object(f, 0, entity->geometry());
        fprintf_tabs(f, indents, "bulletin_count: %d\n", entity->bulletin_count());
        fprintf_tabs(f, indents, "count_edges: %d\n", entity->count_edges());
        // fprintf_tabs(f, indents, "get_curr_tolerance: %f\n", entity->get_curr_tolerance());
        // fprintf_tabs(f, indents, "get_tolerance: %f\n", entity->get_tolerance());
        fprintf_tabs(f, indents, "identity: %d\n", entity->identity());
        // fprintf_tabs(f, indents, "is_tolerant: %d\n", entity->is_tolerant());
        fprintf_tabs(f, indents, "is_use_counted: %d\n", entity->is_use_counted());
        fprintf_tabs(f, indents, "size: %d\n", entity->size());
        fprintf_tabs(f, indents, "type_name: %s\n", entity->type_name());
        fprintf_tabs(f, indents, "use_count: %d\n", entity->use_count());
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

void dump_object(FILE* f, int indents, class EDGE* const entity)
{
    if (f && entity)
    {
        fprintf_tabs(f, indents, "identity: %d\n", entity->identity());
        fprintf_tabs(f, indents, "VERTEX(Start):\n");
        dump_object(f, indents + 1, entity->start());
        fprintf_tabs(f, indents, "VERTEX(End):\n");
        dump_object(f, indents + 1, entity->end());
        fprintf_tabs(f, indents, "bound:\n");
        dump_object(f, indents + 1, entity->bound());
        fprintf_tabs(f, indents, "start_param:%f\n", entity->start_param());
        fprintf_tabs(f, indents, "end_param:%f\n", entity->end_param());
        if (entity->start() != entity->end() || entity->geometry())
        {
            // EDGE没有退化为1个点
            fprintf_tabs(f, indents, "start_deriv: ");
            dump_object(f, 0, entity->start_deriv());
            fprintf_tabs(f, indents, "end_deriv: ");
            dump_object(f, 0, entity->end_deriv());
            fprintf_tabs(f, indents, "sense: %d\n", entity->sense());
            fprintf_tabs(f, indents, "geometry:\n");
            dump_object(f, indents + 1, entity->geometry());
        }
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

void dump_object(FILE* f, int indents, class COEDGE* const entity)
{
    if (f && entity)
    {
        fprintf_tabs(f, indents, "identity: %d\n", entity->identity());
        fprintf_tabs(f, indents, "sense: %d\n", entity->sense());
        fprintf_tabs(f, indents, "EDGE:\n");
        dump_object(f, indents + 1, entity->edge());
        fprintf_tabs(f, indents, "geometry:\n");
        dump_object(f, indents + 1, entity->geometry());
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

void dump_object(FILE* f, int indents, class WIRE* const entity)
{
    if (f && entity)
    {
        fprintf_tabs(f, indents, "identity: %d\n", entity->identity());
        int index_coedge = 0;
        class COEDGE* ptrCoedge_start = entity->coedge();
        class COEDGE* ptrCoedge = ptrCoedge_start;
        while (ptrCoedge)
        {
            fprintf_tabs(f, indents, "COEDGE %d:\n", index_coedge);
            dump_object(f, indents + 1, ptrCoedge);
            ptrCoedge = ptrCoedge->next();
            if (ptrCoedge == ptrCoedge_start) break;
            index_coedge += 1;
        }
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

void dump_object(FILE* f, int indents, class LOOP* const entity)
{
    if (f && entity)
    {
        fprintf_tabs(f, indents, "identity: %d\n", entity->identity());
        fprintf_tabs(f, indents, "classification: %d\n", entity->get_classification());
        int index_coedge = 0;
        class COEDGE* ptrCoedge_start = entity->start();
        class COEDGE* ptrCoedge = ptrCoedge_start;
        while (ptrCoedge)
        {
            fprintf_tabs(f, indents, "COEDGE %d:\n", index_coedge);
            dump_object(f, indents + 1, ptrCoedge);
            ptrCoedge = ptrCoedge->next();
            if (ptrCoedge == ptrCoedge_start) break;
            index_coedge += 1;
        }
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

void dump_object(FILE* f, int indents, class FACE* const entity)
{
    if (f && entity)
    {
        fprintf_tabs(f, indents, "identity: %d\n", entity->identity());
        fprintf_tabs(f, indents, "sense: %d\n", entity->sense());
        fprintf_tabs(f, indents, "cont: %d\n", entity->cont());
        fprintf_tabs(f, indents, "sides: %d\n", entity->sides());
        int index_loop = 0;
        class LOOP* ptrLoop = entity->loop();
        while (ptrLoop)
        {
            fprintf_tabs(f, indents, "LOOP %d:\n", index_loop);
            dump_object(f, indents + 1, ptrLoop);
            ptrLoop = ptrLoop->next();
            index_loop += 1;
        }
        class SURFACE* ptrSurface = entity->geometry();
        if (ptrSurface)
        {
            fprintf_tabs(f, indents, "geometry:\n");
            dump_object(f, indents + 1, ptrSurface);
        }
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

void dump_object(FILE* f, int indents, class SUBSHELL* const entity)
{
    if (f && entity)
    {
        fprintf_tabs(f, indents, "identity: %d\n", entity->identity());
        int index_subshell = 0;
        class SUBSHELL* ptrSubshell = entity->child();
        while (ptrSubshell)
        {
            fprintf_tabs(f, indents, "SUBSHELL %d:\n", index_subshell);
            dump_object(f, indents + 1, ptrSubshell);
            ptrSubshell = ptrSubshell->sibling();
            index_subshell += 1;
        }
        int index_face = 0;
        class FACE* ptrFace = entity->face_list();
        while (ptrFace)
        {
            fprintf_tabs(f, indents, "FACE %d:\n", index_face);
            dump_object(f, indents + 1, ptrFace);
            ptrFace = ptrFace->next_in_list();
            index_face += 1;
        }
        int index_wire = 0;
        class WIRE* ptrWire = entity->wire_list();
        while (ptrWire)
        {
            fprintf_tabs(f, indents, "WIRE %d:\n", index_wire);
            dump_object(f, indents + 1, ptrWire);
            ptrWire = ptrWire->next_in_list();
            index_wire += 1;
        }
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

void dump_object(FILE* f, int indents, class SHELL* const entity)
{
    if (f && entity)
    {
        fprintf_tabs(f, indents, "identity: %d\n", entity->identity());
        int index_subshell = 0;
        class SUBSHELL* ptrSubshell = entity->subshell();
        while (ptrSubshell)
        {
            fprintf_tabs(f, indents, "SUBSHELL %d:\n", index_subshell);
            dump_object(f, indents + 1, ptrSubshell);
            ptrSubshell = ptrSubshell->sibling();
            index_subshell += 1;
        }
        int index_face = 0;
        class FACE* ptrFace = entity->face_list();
        while (ptrFace)
        {
            fprintf_tabs(f, indents, "FACE %d:\n", index_face);
            dump_object(f, indents + 1, ptrFace);
            ptrFace = ptrFace->next_in_list();
            index_face += 1;
        }
        int index_wire = 0;
        class WIRE* ptrWire = entity->wire_list();
        while (ptrWire)
        {
            fprintf_tabs(f, indents, "WIRE %d:\n", index_wire);
            dump_object(f, indents + 1, ptrWire);
            ptrWire = ptrWire->next_in_list();
            index_wire += 1;
        }
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

void dump_object(FILE* f, int indents, class LUMP* const entity)
{
    if (f && entity)
    {
        fprintf_tabs(f, indents, "identity: %d\n", entity->identity());
        int index_shell = 0;
        class SHELL* ptrShell = entity->shell();
        while (ptrShell)
        {
            fprintf_tabs(f, indents, "SHELL %d:\n", index_shell);
            dump_object(f, indents + 1, ptrShell);
            ptrShell = ptrShell->next();
            index_shell += 1;
        }
    }
    else if (f)
        fprintf_tabs(f, indents, "nullptr\n");
}

void dump_object(FILE* f, const char* info, class BODY* const entity)
{
    if (f)
    {
        fprintf(f, "==============================BODY==============================\n");
        time_t now = time(0);
        fprintf(f, ctime(&now));
        fprintf(f, "Topic: %s\n", info);
        fprintf(f, "==============================BODY==============================\n");
        class LUMP* ptrLump = entity->lump();
        int index_lump = 0;
        int indents = 0;
        while (ptrLump)
        {
            fprintf_tabs(f, indents, "LUMP %d:\n", index_lump);
            dump_object(f, indents + 1, ptrLump);
            ptrLump = ptrLump->next();
            index_lump += 1;
        }
        class WIRE* ptrWire = entity->wire();
        int index_wire = 0;
        while (ptrWire)
        {
            fprintf_tabs(f, indents, "WIRE %d:\n", index_wire);
            dump_object(f, indents + 1, ptrWire);
            ptrWire = ptrWire->next_in_list();
            index_wire += 1;
        }
        fprintf_tabs(f, indents, "bound:\n");
        dump_object(f, indents + 1, entity->bound());
        fprintf_tabs(f, indents, "bulletin_count: %d\n", entity->bulletin_count());
        fprintf_tabs(f, indents, "identity: %d\n", entity->identity());
        fprintf_tabs(f, indents, "size: %d\n", entity->size());
        fprintf_tabs(f, indents, "transform:\n");
        dump_object(f, indents + 1, entity->transform());
        fprintf(f, "==============================END==============================\n");
    }
}

void dump_object(FILE* f, const char* info, int indents, class ENTITY* const entity)
{
    std::string type_name = entity->type_name();
    if (!type_name.compare("body"))
    {
        dump_object(f, info, (class BODY*)entity);
    }
    else if (!type_name.compare("lump"))
    {
        dump_object(f, indents, (class LUMP*)entity);
    }
    else if (!type_name.compare("shell"))
    {
        dump_object(f, indents, (class SHELL*)entity);
    }
    else if (!type_name.compare("subshell"))
    {
        dump_object(f, indents, (class SUBSHELL*)entity);
    }
    else if (!type_name.compare("face"))
    {
        dump_object(f, indents, (class FACE*)entity);
    }
    else if (!type_name.compare("loop"))
    {
        dump_object(f, indents, (class LOOP*)entity);
    }
    else if (!type_name.compare("wire"))
    {
        dump_object(f, indents, (class WIRE*)entity);
    }
    else if (!type_name.compare("coedge"))
    {
        dump_object(f, indents, (class COEDGE*)
                    entity
        );
    }
    else if (!type_name.compare("edge"))
    {
        dump_object(f, indents, (class EDGE*)entity);
    }
    else if (!type_name.compare("vertex"))
    {
        dump_object(f, indents, (class VERTEX*)entity);
    }
    else if (!type_name.compare("apoint"))
    {
        dump_object(f, indents, (class APOINT*)entity);
    }
    else if (!type_name.compare("curve"))
    {
        dump_object(f, indents, (class CURVE*)entity);
    }
    else if (!type_name.compare("pcurve"))
    {
        dump_object(f, indents, (class PCURVE*)entity);
    }
    else if (!type_name.compare("surface"))
    {
        dump_object(f, indents, (class SURFACE*)entity);
    }
    else if (!type_name.compare("transform"))
    {
        dump_object(f, indents, (class TRANSFORM*)entity);
    }
    else
    {
        fprintf(f, "!!!Unsuported type of entity: %s\n", type_name.c_str());
    }
}

void print_statistic(class ENTITY* e)
{
    printf("*****Statistic of Entity*****\n");
    if (!e)
    {
        printf("nullptr");
        return;
    }
    ENTITY_LIST el;
    api_get_vertices(e, el);
    printf("1. Num of Vertices: %d\n", el.iteration_count());
    el.clear();
    api_get_edges(e, el);
    printf("2. Num of Edges: %d\n", el.iteration_count());
    el.clear();
    api_get_coedges(e, el);
    printf("3. Num of Coedges: %d\n", el.iteration_count());
    el.clear();
    api_get_loops(e, el);
    printf("4. Num of Loops: %d\n", el.iteration_count());
    el.clear();
    api_get_faces(e, el);
    printf("5. Num of Faces: %d\n", el.iteration_count());
    el.clear();
    api_get_wires(e, el);
    printf("6. Num of Wires: %d\n", el.iteration_count());
    el.clear();
    api_get_shells(e, el);
    printf("7. Num of Shells: %d\n", el.iteration_count());
    el.clear();
    api_get_lumps(e, el);
    printf("8. Num of Lumps: %d\n", el.iteration_count());
    printf("*****End of Statistic*****\n");
}
