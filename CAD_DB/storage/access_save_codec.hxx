#pragma once
// Internal save codecs for ACIS topology properties.
namespace AccessUtils {
namespace Save {
std::string getstring_number(double num);

std::string getstring_number(int num);

mg_list *getmglist_SPAposition(const SPAposition &ptr, int tag);

mg_list *getmglist_SPAinterval(const SPAinterval &ptr);

mg_list *getmglist_SPApar_box(const SPApar_box &ptr);

mg_list *getmglist_SPAvector(const SPAvector &ptr);

mg_list *getmglist_SPAunit_vector(const SPAunit_vector &ptr);

mg_list *getmglist_SPAmatrix(const SPAmatrix &ptr);

Node *createnode_spl_sur_subgraph(spl_sur *sur);

struct surface_data {
    int subtype;
};

struct plane_data : surface_data {
    mg_list *root_point;
    mg_list *normal;
    mg_list *u_deriv;
    int reverse_v;
    mg_list *subset_range;
};

struct sphere_data : surface_data {
    mg_list *centre;
    double radius;
    mg_list *uv_oridir;
    mg_list *pole_dir;
    int reverse_v;
    mg_list *subset_range;
};

struct torus_data : surface_data {
    mg_list *centre;
    mg_list *normal;
    double major_radius;
    double minor_radius;
    mg_list *uv_oridir;
    int reverse_v;
    mg_list *subset_range;
};

struct cone_data : surface_data {
    mg_list *base_centre;
    mg_list *base_normal;
    mg_list *base_major_axis;
    double base_radius_ratio;
    mg_list *base_subset_range;
    double sine_angle;
    double cosine_angle;
    int reverse_u;
    mg_list *subset_range;
};

struct spline_data : surface_data {
    int reversed;
    Node *spl_node;
    int64_t spl_id;
    mg_list *subset_range;
};

plane_data *get_plane_data(plane *sur);

sphere_data *get_sphere_data(sphere *sur);

torus_data *get_torus_data(torus *sur);

cone_data *get_cone_data(cone *sur);
;

spline_data *get_spline_data_subgraph(std::unordered_map<void *, Node *> &ptr2node, spline *sur);
;

surface_data *get_surface_data_subgraph(std::unordered_map<void *, Node *> &ptr2node, surface *sur);

struct bs2_curve_data {
    int subtype;
    int degree;
    int form;
    int knots_simplifier_size;
    mg_list *knots_simplifier_mglist;
    mg_list *ctrlpts_mglist;
};

bs2_curve_data get_bs2_curve_data(bs2_curve pcur);

Node *createnode_int_cur_subgraph(std::unordered_map<void *, Node *> &ptr2node,
                                  std::vector<Relationship *> &relationship_list, intcurve *icur);

Node *createnode_par_cur_subgraph(std::unordered_map<void *, Node *> &ptr2node,
                                  std::vector<Relationship *> &relationship_list, pcurve *pcrv);

} // namespace Save
} // namespace AccessUtils
