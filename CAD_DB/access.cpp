#include <deque>
#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "common.hxx"
#include "neo4j.hxx"
#include "access.hxx"
#include <glaze/glaze.hpp>

#include <acis/include/allcurve.hxx>
#include <acis/include/allsurf.hxx>
#include <acis/include/alltop.hxx>
#include <acis/include/api.hxx>
#include <acis/include/cstrapi.hxx>
#include <acis/include/exct_int.hxx>
#include <acis/include/exct_spl.hxx>
#include <acis/include/exp_par.hxx>
#include <acis/include/fileinfo.hxx>
#include <acis/include/kernapi.hxx>
#include <acis/include/par_int.hxx>
#include <acis/include/int_int.hxx>
#include <acis/include/pcurve.hxx>
#include <acis/include/point.hxx>
#include <acis/include/sps2crtn.hxx>
#include <acis/include/sps3crtn.hxx>
#include <acis/include/sp3srtn.hxx>
#include <acis/include/sps3srtn.hxx>
#include <acis/include/surf_int.hxx>
#include <acis/include/transfrm.hxx>
#include <acis/include/bulletin.hxx>

class Node {
public:
    int64_t id;
    mg_list* labels;
    std::unordered_map<char, mg_value*> properties;
    Node() {}
};
class Node_postgresql {
public:
    int id;
    std::string type;
    std::optional<std::string> properties;
    Node_postgresql() {}
};
class Relationship {
public:
    Node* u;
    Node* v;
    mg_value* label;
    std::unordered_map<char, mg_value*> properties;
    Relationship(Node* u1, Node* v1) : u(u1), v(v1) {}
};
class Relationship_postgresql {
public:
    Node_postgresql* u;
    Node_postgresql* v;
    std::string type;
    Relationship_postgresql(Node_postgresql* u1, Node_postgresql* v1) : u(u1), v(v1) {}
};
class Relationship2 {
public:
    int64_t uid;
    int64_t vid;
    mg_value* label;
    std::unordered_map<char, mg_value*> properties;
    Relationship2(int64_t uid1, int64_t vid1) : uid(uid1), vid(vid1) {}
};

struct glz_wire {
    int cont;
    struct glaze {
        using T = glz_wire;
        static constexpr auto value = glz::array(
            &T::cont
        );
    };
};
struct glz_face {
    int sense;
    int sides;
    std::optional<int> cont;
    struct glaze {
        using T = glz_face;
        static constexpr auto value = glz::array(
            &T::sense,
            &T::sides,
            &T::cont
        );
    };
};
struct glz_SPAinterval {
    bool lowfinite;
    std::optional<double> low;
    bool highfinite;
    std::optional<double> high;
    struct glaze {
        using T = glz_SPAinterval;
        static constexpr auto value = glz::array(
            &T::lowfinite,
            &T::low,
            &T::highfinite,
            &T::high
        );
    };
};
struct glz_sphere_surface {
    glz_SPAinterval subset_range_u;
    glz_SPAinterval subset_range_v;
    std::array<double, 3> centre;
    double radius;
    std::array<double, 3> uv_oridir;
    std::array<double, 3> pole_dir;
    bool reverse_v;
    struct glaze {
        using T = glz_sphere_surface;
        static constexpr auto value = glz::array(
            &T::subset_range_u,
            &T::subset_range_v,
            &T::centre,
            &T::radius,
            &T::uv_oridir,
            &T::pole_dir,
            &T::reverse_v
        );
    };
};
struct glz_transform {
    std::array<double, 9> affine;
    std::array<double, 3> translation;
    double scaling;
    int rotate;
    int reflect;
    int shear;
    struct glaze {
        using T = glz_transform;
        static constexpr auto value = glz::array(
            &T::affine,
            &T::translation,
            &T::scaling,
            &T::rotate,
            &T::reflect,
            &T::shear
        );
    };
};

// -------------------------------------------------------------------------------------------------------
// ! 宏定义
#ifdef _MSC_VER
// 用于宏拼接
#    define STR(a) STR2(a)
#    define STR2(a) #a
#    define CAT(a, b) CAT_I(a, b)
#    define CAT_I(a, b) CAT_II(~, a##b)
#    define CAT_II(_, res) res
#    define MSVC_EXPAND(...) __VA_ARGS__
#    define _GET_MACRO_ARGS_COUNT_KERN(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, __COUNT, ...) __COUNT
#    define GET_MACRO_ARGS_COUNT(...) MSVC_EXPAND(_GET_MACRO_ARGS_COUNT_KERN(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))
#    define ITERATE_MACRO_WITH_PARAM0(__macro,  ...) MSVC_EXPAND(CAT(__macro##_, GET_MACRO_ARGS_COUNT(__VA_ARGS__))(__VA_ARGS__))
#    define ITERATE_MACRO_WITH_PARAM1(__macro, __param1, ...) MSVC_EXPAND(CAT(__macro##_, GET_MACRO_ARGS_COUNT(__VA_ARGS__))(__param1, __VA_ARGS__))
#    define ITERATE_MACRO_WITH_PARAM2(__macro, __param1, __param2, ...) MSVC_EXPAND(CAT(__macro##_, GET_MACRO_ARGS_COUNT(__VA_ARGS__))(__param1, __param2, __VA_ARGS__))
#endif


// 更新实体指针记录的迭代宏(neo4j)
#define _API_PUSH_PTR_NEO4J_SUBGRAPH_1(ptr, entity_label, _1) _API_PUSH_PTR_NEO4J_SUBGRAPH(ptr, entity_label, _1)
#define _API_PUSH_PTR_NEO4J_SUBGRAPH_2(ptr, entity_label, _1, _2) _API_PUSH_PTR_NEO4J_SUBGRAPH_1(ptr, entity_label, _1) _API_PUSH_PTR_NEO4J_SUBGRAPH(ptr, entity_label, _2)
#define _API_PUSH_PTR_NEO4J_SUBGRAPH_3(ptr, entity_label, _1, _2, _3) _API_PUSH_PTR_NEO4J_SUBGRAPH_2(ptr, entity_label, _1, _2) _API_PUSH_PTR_NEO4J_SUBGRAPH(ptr, entity_label, _3)
#define _API_PUSH_PTR_NEO4J_SUBGRAPH_4(ptr, entity_label, _1, _2, _3, _4) _API_PUSH_PTR_NEO4J_SUBGRAPH_3(ptr, entity_label, _1, _2, _3) _API_PUSH_PTR_NEO4J_SUBGRAPH(ptr, entity_label, _4)
#define _API_PUSH_PTR_NEO4J_SUBGRAPH_5(ptr, entity_label, _1, _2, _3, _4, _5) _API_PUSH_PTR_NEO4J_SUBGRAPH_4(ptr, entity_label, _1, _2, _3, _4) _API_PUSH_PTR_NEO4J_SUBGRAPH(ptr, entity_label, _5)
#define _API_PUSH_PTR_NEO4J_SUBGRAPH_6(ptr, entity_label, _1, _2, _3, _4, _5, _6) _API_PUSH_PTR_NEO4J_SUBGRAPH_5(ptr, entity_label, _1, _2, _3, _4, _5) _API_PUSH_PTR_NEO4J_SUBGRAPH(ptr, entity_label, _6)
#define _API_PUSH_PTR_NEO4J_SUBGRAPH_7(ptr, entity_label, _1, _2, _3, _4, _5, _6, _7) _API_PUSH_PTR_NEO4J_SUBGRAPH_6(ptr, entity_label, _1, _2, _3, _4, _5, _6) _API_PUSH_PTR_NEO4J_SUBGRAPH(ptr, entity_label, _7)
#define _API_PUSH_PTR_NEO4J_SUBGRAPH_8(ptr, entity_label, _1, _2, _3, _4, _5, _6, _7, _8) _API_PUSH_PTR_NEO4J_SUBGRAPH_7(ptr, entity_label, _1, _2, _3, _4, _5, _6, _7) _API_PUSH_PTR_NEO4J_SUBGRAPH(ptr, entity_label, _8)
#define _API_PUSH_PTR_NEO4J_SUBGRAPH(__ptr, __entity_label, __member_func_name)                                                                                         \
    {                                                                                                                                                          \
        ENTITY* __tmp_ptr = __ptr->__member_func_name();                                                                                                       \
        if(__tmp_ptr != nullptr) {                                                                                                                             \
            if(ptr2node.find(__tmp_ptr) == ptr2node.end()) {                                                                                           \
                Node* a = ptr2node.at(__ptr); \
                ptr2node[__tmp_ptr] = new Node(); \
                Relationship* r = new Relationship(a, ptr2node[__tmp_ptr]); \
                r->label = mg_value_make_string(STR(CAT(__entity_label##_, __member_func_name##_ptr)));\
                r->properties['a'] = mg_value_make_string(STR(CAT(__entity_label##_, __member_func_name##_ptr))) ;\
                relationship_list.push_back(r);\
                que.push_back(__tmp_ptr);\
            } else {                                                                                                                                           \
                Node* a = ptr2node.at(__ptr); \
                Node* b = ptr2node.at(__tmp_ptr); \
                Relationship* r = new Relationship(a, b); \
                r->label = mg_value_make_string(STR(CAT(__entity_label##_, __member_func_name##_ptr)));\
                r->properties['a'] = mg_value_make_string(STR(CAT(__entity_label##_, __member_func_name##_ptr)));\
                relationship_list.push_back(r);\
            }                                                                                                                                                  \
        }                                                                                                                                                      \
    }

// 更新实体指针记录的迭代宏(postgresql)
#define _API_PUSH_PTR_POSTGRESQL_1(ptr, entity_label, _1) _API_PUSH_PTR_POSTGRESQL(ptr, entity_label, _1)
#define _API_PUSH_PTR_POSTGRESQL_2(ptr, entity_label, _1, _2) _API_PUSH_PTR_POSTGRESQL_1(ptr, entity_label, _1) _API_PUSH_PTR_POSTGRESQL(ptr, entity_label, _2)
#define _API_PUSH_PTR_POSTGRESQL_3(ptr, entity_label, _1, _2, _3) _API_PUSH_PTR_POSTGRESQL_2(ptr, entity_label, _1, _2) _API_PUSH_PTR_POSTGRESQL(ptr, entity_label, _3)
#define _API_PUSH_PTR_POSTGRESQL_4(ptr, entity_label, _1, _2, _3, _4) _API_PUSH_PTR_POSTGRESQL_3(ptr, entity_label, _1, _2, _3) _API_PUSH_PTR_POSTGRESQL(ptr, entity_label, _4)
#define _API_PUSH_PTR_POSTGRESQL_5(ptr, entity_label, _1, _2, _3, _4, _5) _API_PUSH_PTR_POSTGRESQL_4(ptr, entity_label, _1, _2, _3, _4) _API_PUSH_PTR_POSTGRESQL(ptr, entity_label, _5)
#define _API_PUSH_PTR_POSTGRESQL_6(ptr, entity_label, _1, _2, _3, _4, _5, _6) _API_PUSH_PTR_POSTGRESQL_5(ptr, entity_label, _1, _2, _3, _4, _5) _API_PUSH_PTR_POSTGRESQL(ptr, entity_label, _6)
#define _API_PUSH_PTR_POSTGRESQL_7(ptr, entity_label, _1, _2, _3, _4, _5, _6, _7) _API_PUSH_PTR_POSTGRESQL_6(ptr, entity_label, _1, _2, _3, _4, _5, _6) _API_PUSH_PTR_POSTGRESQL(ptr, entity_label, _7)
#define _API_PUSH_PTR_POSTGRESQL_8(ptr, entity_label, _1, _2, _3, _4, _5, _6, _7, _8) _API_PUSH_PTR_POSTGRESQL_7(ptr, entity_label, _1, _2, _3, _4, _5, _6, _7) _API_PUSH_PTR_POSTGRESQL(ptr, entity_label, _8)
#define _API_PUSH_PTR_POSTGRESQL(__ptr, __entity_label, __member_func_name)                                                                                         \
    {                                                                                                                                                          \
        ENTITY* __tmp_ptr = __ptr->__member_func_name();                                                                                                       \
        if(__tmp_ptr != nullptr) {                                                                                                                             \
            if(ptr2node.find(__tmp_ptr) == ptr2node.end()) {                                                                                           \
                Node_postgresql* a = ptr2node.at(__ptr); \
                ptr2node[__tmp_ptr] = new Node_postgresql(); \
                Relationship_postgresql* r = new Relationship_postgresql(a, ptr2node[__tmp_ptr]); \
                r->type = STR(CAT(__entity_label##_, __member_func_name##_ptr));\
                relationship_list.push_back(r);\
                que.push_back(__tmp_ptr);\
            } else {                                                                                                                                           \
                Node_postgresql* a = ptr2node.at(__ptr); \
                Node_postgresql* b = ptr2node.at(__tmp_ptr); \
                Relationship_postgresql* r = new Relationship_postgresql(a, b); \
                r->type = STR(CAT(__entity_label##_, __member_func_name##_ptr));\
                relationship_list.push_back(r);\
            }                                                                                                                                                  \
        }                                                                                                                                                      \
    }



// -------------------------------------------------------------------------------------------------------



namespace AccessUtils {
    namespace Save {
        inline std::string getstring_number(double num) {
            return std::format("{}", num);
        }
        inline std::string getstring_number(int num) {
            return std::to_string(num);
        }
        mg_list* getmglist_SPAposition(const SPAposition& ptr, int tag) {
            mg_list* ans;
            if (tag == 3) {
                ans = mg_list_make_empty(3);
                mg_list_append(ans, mg_value_make_float(ptr.x()));
                mg_list_append(ans, mg_value_make_float(ptr.y()));
                mg_list_append(ans, mg_value_make_float(ptr.z()));
            } else if (tag == 2) {
                ans = mg_list_make_empty(2);
                mg_list_append(ans, mg_value_make_float(ptr.x()));
                mg_list_append(ans, mg_value_make_float(ptr.y()));
            } else {
                assert(tag == 1);
                ans = mg_list_make_empty(1);
                mg_list_append(ans, mg_value_make_float(ptr.x()));
            }
            return ans;
        }
        mg_list* getmglist_SPAinterval(const SPAinterval& ptr) {
            mg_list* ans;
            if (ptr.empty()) {
                ans = mg_list_make_empty(2);
                mg_list_append(ans, mg_value_make_float(1.0)); // 1:infinite 2:finite
                mg_list_append(ans, mg_value_make_float(1.0));
            } else {
                interval_type type = ptr.type();
                if (type == interval_type::interval_finite) {
                    if (ptr.start_pt() > ptr.end_pt()) goto interval_infinite;
                    ans = mg_list_make_empty(4);
                    mg_list_append(ans, mg_value_make_float(2.0));
                    mg_list_append(ans, mg_value_make_float(ptr.start_pt()));
                    mg_list_append(ans, mg_value_make_float(2.0));
                    mg_list_append(ans, mg_value_make_float(ptr.end_pt()));
                } else if (type == interval_type::interval_finite_below) {
                    ans = mg_list_make_empty(3);
                    mg_list_append(ans, mg_value_make_float(2.0));
                    mg_list_append(ans, mg_value_make_float(ptr.start_pt()));
                    mg_list_append(ans, mg_value_make_float(1.0));
                } else if (type == interval_type::interval_finite_above) {
                    ans = mg_list_make_empty(3);
                    mg_list_append(ans, mg_value_make_float(1.0));
                    mg_list_append(ans, mg_value_make_float(2.0));
                    mg_list_append(ans, mg_value_make_float(ptr.end_pt()));
                } else {
                interval_infinite:
                    ans = mg_list_make_empty(2);
                    mg_list_append(ans, mg_value_make_float(1.0));
                    mg_list_append(ans, mg_value_make_float(1.0));
                }
            }
            return ans;
        }
        glz_SPAinterval getglz_SPAinterval(const SPAinterval& ptr) {
            glz_SPAinterval ans;
            if (ptr.empty()) {
                ans = { false,std::nullopt,false,std::nullopt };
            } else {
                interval_type type = ptr.type();
                if (type == interval_type::interval_finite) {
                    if (ptr.start_pt() > ptr.end_pt()) goto interval_infinite;
                    ans = { true,ptr.start_pt(),true,ptr.end_pt() };
                } else if (type == interval_type::interval_finite_below) {
                    ans = { true,ptr.start_pt(),false,std::nullopt };
                } else if (type == interval_type::interval_finite_above) {
                    ans = { false,std::nullopt,true,ptr.end_pt() };
                } else {
                interval_infinite:
                    ans = { false,std::nullopt,false,std::nullopt };
                }
            }
            return ans;
        }
        mg_list* getmglist_SPApar_box(const SPApar_box& ptr) {
            mg_list* ans = mg_list_make_empty(8);
            SPAinterval range_u = ptr.u_range();
            if (range_u.empty()) {
                mg_list_append(ans, mg_value_make_float(1.0)); // 1:infinite 2:finite
                mg_list_append(ans, mg_value_make_float(1.0));
            } else {
                interval_type type = range_u.type();
                if (type == interval_type::interval_finite) {
                    if (range_u.start_pt() > range_u.end_pt()) goto interval_infinite_u;
                    mg_list_append(ans, mg_value_make_float(2.0));
                    mg_list_append(ans, mg_value_make_float(range_u.start_pt()));
                    mg_list_append(ans, mg_value_make_float(2.0));
                    mg_list_append(ans, mg_value_make_float(range_u.end_pt()));
                } else if (type == interval_type::interval_finite_below) {
                    mg_list_append(ans, mg_value_make_float(2.0));
                    mg_list_append(ans, mg_value_make_float(range_u.start_pt()));
                    mg_list_append(ans, mg_value_make_float(1.0));
                } else if (type == interval_type::interval_finite_above) {
                    mg_list_append(ans, mg_value_make_float(1.0));
                    mg_list_append(ans, mg_value_make_float(2.0));
                    mg_list_append(ans, mg_value_make_float(range_u.end_pt()));
                } else {
                interval_infinite_u:
                    mg_list_append(ans, mg_value_make_float(1.0));
                    mg_list_append(ans, mg_value_make_float(1.0));
                }
            }
            SPAinterval range_v = ptr.v_range();
            if (range_v.empty()) {
                mg_list_append(ans, mg_value_make_float(1.0)); // 1:infinite 2:finite
                mg_list_append(ans, mg_value_make_float(1.0));
            } else {
                interval_type type = range_v.type();
                if (type == interval_type::interval_finite) {
                    if (range_v.start_pt() > range_v.end_pt()) goto interval_infinite_v;
                    mg_list_append(ans, mg_value_make_float(2.0));
                    mg_list_append(ans, mg_value_make_float(range_v.start_pt()));
                    mg_list_append(ans, mg_value_make_float(2.0));
                    mg_list_append(ans, mg_value_make_float(range_v.end_pt()));
                } else if (type == interval_type::interval_finite_below) {
                    mg_list_append(ans, mg_value_make_float(2.0));
                    mg_list_append(ans, mg_value_make_float(range_v.start_pt()));
                    mg_list_append(ans, mg_value_make_float(1.0));
                } else if (type == interval_type::interval_finite_above) {
                    mg_list_append(ans, mg_value_make_float(1.0));
                    mg_list_append(ans, mg_value_make_float(2.0));
                    mg_list_append(ans, mg_value_make_float(range_v.end_pt()));
                } else {
                interval_infinite_v:
                    mg_list_append(ans, mg_value_make_float(1.0));
                    mg_list_append(ans, mg_value_make_float(1.0));
                }
            }
            return ans;
        }
        mg_list* getmglist_SPAvector(const SPAvector& ptr) {
            mg_list* ans = mg_list_make_empty(3);
            mg_list_append(ans, mg_value_make_float(ptr.x()));
            mg_list_append(ans, mg_value_make_float(ptr.y()));
            mg_list_append(ans, mg_value_make_float(ptr.z()));
            return ans;
        }
        mg_list* getmglist_SPAunit_vector(const SPAunit_vector& ptr) {
            mg_list* ans = mg_list_make_empty(3);
            mg_list_append(ans, mg_value_make_float(ptr.x()));
            mg_list_append(ans, mg_value_make_float(ptr.y()));
            mg_list_append(ans, mg_value_make_float(ptr.z()));
            return ans;
        }
        mg_list* getmglist_SPAmatrix(const SPAmatrix& ptr) {
            mg_list* ans = mg_list_make_empty(9);
            mg_list_append(ans, mg_value_make_float(ptr.element(0, 0)));
            mg_list_append(ans, mg_value_make_float(ptr.element(0, 1)));
            mg_list_append(ans, mg_value_make_float(ptr.element(0, 2)));
            mg_list_append(ans, mg_value_make_float(ptr.element(1, 0)));
            mg_list_append(ans, mg_value_make_float(ptr.element(1, 1)));
            mg_list_append(ans, mg_value_make_float(ptr.element(1, 2)));
            mg_list_append(ans, mg_value_make_float(ptr.element(2, 0)));
            mg_list_append(ans, mg_value_make_float(ptr.element(2, 1)));
            mg_list_append(ans, mg_value_make_float(ptr.element(2, 2)));
            return ans;
        }
        Node* createnode_spl_sur_subgraph(spl_sur* sur) {
            // 子类类型
            int subtype;
            switch (sur->type()) {
                case 44: {  // rot_spl_sur
                    myerror("不支持的spl_sur子类型rot_spl_sur。");
                }
                case 37:  // exact_spl_sur/exactsur
                case 60:  // exact_spl_sur/exactsur
                default: {
                    subtype = 1;//exactsur
                    break;
                }
            }
            // degree_u, degree_v
            int dim = 0, form_u = 0, form_v = 0, pole_u = 0, pole_v = 0, num_u = 0, num_v = 0, degree_u = 0, num_uknots = 0, degree_v = 0, num_vknots = 0;
            logical rational_u = 0, rational_v = 0;
            SPAposition* ctrlpts = nullptr;
            double* weights = nullptr, * uknots = nullptr, * vknots = nullptr;
            bs3_surface_to_array(sur->sur(), dim, rational_u, rational_v, form_u, form_v, pole_u, pole_v, num_u, num_v, ctrlpts, weights, degree_u, num_uknots, uknots, degree_v, num_vknots, vknots, 0);
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
            mg_list* knots_simplifier_u_mglist = mg_list_make_empty(knots_simplifier_u_vec_size);
            for (size_t i = 0; i < knots_simplifier_u_vec_size; i++) {
                mg_list_append(knots_simplifier_u_mglist, mg_value_make_float(knots_simplifier_u[i]));
            }
            size_t knots_simplifier_v_vec_size = knots_simplifier_v.size();
            mg_list* knots_simplifier_v_mglist = mg_list_make_empty(knots_simplifier_v_vec_size);
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
            mg_list* ctrlpts_mglist = mg_list_make_empty(ctrlpts_size);
            for (size_t i = 0; i < num_v; i++) {
                for (size_t j = 0; j < num_u; j++) {
                    SPAposition* last_ptr = ctrlpts + j * num_v + i;
                    mg_list_append(ctrlpts_mglist, mg_value_make_float(last_ptr->x()));
                    mg_list_append(ctrlpts_mglist, mg_value_make_float(last_ptr->y()));
                    mg_list_append(ctrlpts_mglist, mg_value_make_float(last_ptr->z()));
                    if (rational_u || rational_v) mg_list_append(ctrlpts_mglist, mg_value_make_float(*(weights + j * num_v + i)));
                }
            }
            // fitol
            double fitol = sur->fitol();

            Node* ptrnode = new Node();
            ptrnode->labels = mg_list_make_empty(1);
            mg_list_append(ptrnode->labels, mg_value_make_string("spl_sur"));
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
        struct surface_data {
            int subtype;
        };
        struct plane_data : surface_data {
            mg_list* root_point;
            mg_list* normal;
            mg_list* u_deriv;
            int reverse_v;
            mg_list* subset_range;
        };
        struct sphere_data : surface_data {
            mg_list* centre;
            double radius;
            mg_list* uv_oridir;
            mg_list* pole_dir;
            int reverse_v;
            mg_list* subset_range;
        };
        struct torus_data : surface_data {
            mg_list* centre;
            mg_list* normal;
            double major_radius;
            double minor_radius;
            mg_list* uv_oridir;
            int reverse_v;
            mg_list* subset_range;
        };
        struct cone_data : surface_data {
            mg_list* base_centre;
            mg_list* base_normal;
            mg_list* base_major_axis;
            double base_radius_ratio;
            mg_list* base_subset_range;
            double sine_angle;
            double cosine_angle;
            int reverse_u;
            mg_list* subset_range;
        };
        struct spline_data : surface_data {
            int reversed;
            Node* spl_node;
            int64_t spl_id;
            mg_list* subset_range;
        };
        plane_data* get_plane_data(plane* sur) {
            plane_data* ans = new plane_data();
            ans->root_point = getmglist_SPAposition(sur->root_point, 3);
            ans->normal = getmglist_SPAunit_vector(sur->normal);
            ans->u_deriv = getmglist_SPAvector(sur->u_deriv);
            ans->reverse_v = sur->reverse_v;
            SPApar_box ranges = sur->gme_get_subset_range();
            ans->subset_range = getmglist_SPApar_box(ranges);
            return ans;
        }
        sphere_data* get_sphere_data(sphere* sur) {
            sphere_data* ans = new sphere_data();
            ans->centre = getmglist_SPAposition(sur->centre, 3);
            ans->radius = sur->radius;
            ans->uv_oridir = getmglist_SPAunit_vector(sur->uv_oridir);
            ans->pole_dir = getmglist_SPAunit_vector(sur->pole_dir);
            ans->reverse_v = sur->reverse_v;
            SPApar_box ranges = sur->gme_get_subset_range();
            ans->subset_range = getmglist_SPApar_box(ranges);
            return ans;
        }
        torus_data* get_torus_data(torus* sur) {
            torus_data* ans = new torus_data();
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
        cone_data* get_cone_data(cone* sur) {
            cone_data* ans = new cone_data();
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
        };
        spline_data* get_spline_data_subgraph(std::unordered_map<void*, Node*>& ptr2node, spline* sur) {
            spline_data* ans = new spline_data();
            ans->reversed = sur->reversed();
            SPApar_box ranges = sur->gme_get_subset_range();
            ans->subset_range = getmglist_SPApar_box(ranges);
            ans->spl_node = nullptr;
            spl_sur* spl = sur->gme_get_spl();
            if (spl != nullptr) {
                if (ptr2node.find(spl) == ptr2node.end()) {
                    ptr2node[spl] = createnode_spl_sur_subgraph(spl);  // 创建spl_sur节点
                }
                ans->spl_node = ptr2node.at(spl);
            }
            return ans;
        };
        surface_data* get_surface_data_subgraph(std::unordered_map<void*, Node*>& ptr2node, surface* sur) {
            if (sur == nullptr) {
                surface_data* ans = new surface_data();
                ans->subtype = 0;// "null_surface";
                return ans;
            } else {
                switch (sur->type()) {
                    case spline_type: {
                        spline_data* ans = get_spline_data_subgraph(ptr2node, (spline*)sur);
                        ans->subtype = 1;// "spline";
                        return ans;
                    } break;
                    case plane_type: {
                        plane_data* ans = get_plane_data((plane*)sur);
                        ans->subtype = 2;// "plane";
                        return ans;
                    } break;
                    case sphere_type: {
                        sphere_data* ans = get_sphere_data((sphere*)sur);
                        ans->subtype = 3;// "sphere";
                        return ans;
                    } break;
                    case torus_type: {
                        torus_data* ans = get_torus_data((torus*)sur);
                        ans->subtype = 4;// "torus";
                        return ans;
                    } break;
                    case cone_type: {
                        cone_data* ans = get_cone_data((cone*)sur);
                        ans->subtype = 5;// "cone";
                        return ans;
                    } break;
                    default: {
                        myerror("不支持的surface子类型。");
                    } break;
                }
            }
        }
        struct bs2_curve_data {
            int subtype;
            int degree;
            int form;
            int knots_simplifier_size;
            mg_list* knots_simplifier_mglist;
            mg_list* ctrlpts_mglist;
        };
        bs2_curve_data get_bs2_curve_data(bs2_curve pcur) {
            bs2_curve_data ans;
            if (pcur == nullptr) {
                ans.subtype = 0;//"nullbs";
            } else {
                ans.subtype = 1;//"nubs";
                int dim_pcur1;               // dimension
                int deg_pcur1;               // degree
                logical rat_pcur1;           // rational
                int num_ctrlpts_pcur1;       // number of control points
                SPAposition* ctrlpts_pcur1;  // control points
                double* weights_pcur1;       // weights
                int num_knots_pcur1;         // number of knots
                double* knots_pcur1;         // knots
                bs2_curve_to_array(pcur, dim_pcur1, deg_pcur1, rat_pcur1, num_ctrlpts_pcur1, ctrlpts_pcur1, weights_pcur1, num_knots_pcur1, knots_pcur1);
                ans.degree = deg_pcur1;  // degree

                if (!bs2_curve_closed(pcur)) {
                    ans.form = 0;// "open";
                } else if (!bs2_curve_periodic(pcur)) {
                    ans.form = 1;// "closed";
                } else {
                    ans.form = 2;// "periodic";
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
                    SPAposition* last_ptr = ctrlpts_pcur1 + i;
                    mg_list_append(ans.ctrlpts_mglist, mg_value_make_float(last_ptr->x()));
                    mg_list_append(ans.ctrlpts_mglist, mg_value_make_float(last_ptr->y()));
                }
            }
            return ans;
        }
        Node* createnode_int_cur_subgraph(std::unordered_map<void*, Node*>& ptr2node, std::vector<Relationship*>& relationship_list, int_cur* cur) {
            // 子类类型
            int subtype;
            int tmpp = cur->type();
            switch (cur->type()) {
                case 22:    // int_int_cur: mark as surfintcur
                case 31: {  // int_int_cur: mark as surfintcur
                    subtype = 31;//"surfintcur";
                    break;
                }
                case 25: {  // parcur
                    subtype = 25;//"parcur";
                    break;
                }
                case 20:  // exact_int_cur, exactcur
                default: {
                    subtype = 1;//"exactcur";
                    break;
                }
            }
            // 样条类型
            bs3_curve bs3_cur = cur->gme_get_cur_data();
            int rational = bs3_curve_rational(bs3_cur); // 0:nurbs 1:nubs
            // degree
            int dim_cur = 0, deg_cur = 0, num_ctrlpts_cur = 0, num_knots_cur = 0;
            logical rat_cur = 0;
            SPAposition* ctrlpts_cur = nullptr;
            double* weights_cur = nullptr, * knots_cur = nullptr;
            bs3_curve_to_array(bs3_cur, dim_cur, deg_cur, rat_cur, num_ctrlpts_cur, ctrlpts_cur, weights_cur, num_knots_cur, knots_cur);
            // 样条开闭
            int form;
            if (!bs3_curve_closed(bs3_cur)) {
                form = 0;// "open";
            } else if (!bs3_curve_periodic(bs3_cur)) {
                form = 1;// "closed";
            } else {
                form = 2;// "periodic";
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
            mg_list* knots_simplifier_mglist = mg_list_make_empty(knots_simplifier_vec_size);
            for (size_t i = 0; i < knots_simplifier_vec_size; i++) {
                mg_list_append(knots_simplifier_mglist, mg_value_make_float(knots_simplifier[i]));
            }

            // 控制点
            size_t ctrlpts_size = num_ctrlpts_cur * 4;
            mg_list* ctrlpts_mglist = mg_list_make_empty(ctrlpts_size);
            for (int i = 0; i < num_ctrlpts_cur; i++) {
                SPAposition* last_ptr = ctrlpts_cur + i;
                mg_list_append(ctrlpts_mglist, mg_value_make_float(last_ptr->x()));
                mg_list_append(ctrlpts_mglist, mg_value_make_float(last_ptr->y()));
                mg_list_append(ctrlpts_mglist, mg_value_make_float(last_ptr->z()));
                if (rational) mg_list_append(ctrlpts_mglist, mg_value_make_float(*(weights_cur + i)));
            }
            // fitol
            double fitol = cur->gme_get_fitol_data();
            // surf1
            surface* surf1 = cur->gme_get_surf1_data();  // surf1
            surface_data* surf1_data = get_surface_data_subgraph(ptr2node, surf1);
            // surf2
            surface* surf2 = cur->gme_get_surf2_data();  // surf2
            surface_data* surf2_data = get_surface_data_subgraph(ptr2node, surf2);
            // pcur1
            bs2_curve pcur1 = cur->gme_get_pcur1_data();  // pcur1
            bs2_curve_data pcur1_data = get_bs2_curve_data(pcur1);
            // pcur2
            bs2_curve pcur2 = cur->gme_get_pcur2_data();  // pcur2
            bs2_curve_data pcur2_data = get_bs2_curve_data(pcur2);
            // safe_range
            SPAinterval safe_range = cur->gme_get_safe_range();
            mg_list* safe_range_jsonarray = getmglist_SPAinterval(safe_range);
            // 曲线所在曲面
            int surface_tag = 0;
            if (surf1 != nullptr && surf2 == nullptr) {
                surface_tag = 1;
            } else if (surf1 == nullptr && surf2 != nullptr) {
                surface_tag = 2;
            }

            Node* ptrnode = new Node();
            ptrnode->labels = mg_list_make_empty(1);
            mg_list_append(ptrnode->labels, mg_value_make_string("int_cur"));
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
                plane_data* sd = (plane_data*)surf1_data;
                ptrnode->properties['q'] = mg_value_make_list(sd->root_point);
                ptrnode->properties['r'] = mg_value_make_list(sd->normal);
                ptrnode->properties['s'] = mg_value_make_list(sd->u_deriv);
                ptrnode->properties['t'] = mg_value_make_integer(sd->reverse_v);
                ptrnode->properties['p'] = mg_value_make_list(sd->subset_range);
            } else if (surf1_data->subtype == 3) {
                sphere_data* sd = (sphere_data*)surf1_data;
                ptrnode->properties['q'] = mg_value_make_list(sd->centre);
                ptrnode->properties['r'] = mg_value_make_float(sd->radius);
                ptrnode->properties['s'] = mg_value_make_list(sd->uv_oridir);
                ptrnode->properties['t'] = mg_value_make_list(sd->pole_dir);
                ptrnode->properties['u'] = mg_value_make_integer(sd->reverse_v);
                ptrnode->properties['p'] = mg_value_make_list(sd->subset_range);
            } else if (surf1_data->subtype == 4) {
                torus_data* sd = (torus_data*)surf1_data;
                ptrnode->properties['q'] = mg_value_make_list(sd->centre);
                ptrnode->properties['r'] = mg_value_make_list(sd->normal);
                ptrnode->properties['s'] = mg_value_make_float(sd->major_radius);
                ptrnode->properties['t'] = mg_value_make_float(sd->minor_radius);
                ptrnode->properties['u'] = mg_value_make_list(sd->uv_oridir);
                ptrnode->properties['v'] = mg_value_make_integer(sd->reverse_v);
                ptrnode->properties['p'] = mg_value_make_list(sd->subset_range);
            } else if (surf1_data->subtype == 5) {
                cone_data* sd = (cone_data*)surf1_data;
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
                spline_data* sd = (spline_data*)surf1_data;
                ptrnode->properties['q'] = mg_value_make_integer(sd->reversed);
                ptrnode->properties['p'] = mg_value_make_list(sd->subset_range);
                if (sd->spl_node != nullptr) {
                    Node* b = sd->spl_node;
                    Relationship* r = new Relationship(ptrnode, b);
                    r->label = mg_value_make_string("int_cur_surf1_spl_ptr");
                    r->properties['a'] = mg_value_make_string("int_cur_surf1_spl_ptr");
                    relationship_list.push_back(r);
                }
            }

            if (surf2_data->subtype == 2) {
                plane_data* sd = (plane_data*)surf2_data;
                ptrnode->properties['z'] = mg_value_make_list(sd->root_point);
                ptrnode->properties['A'] = mg_value_make_list(sd->normal);
                ptrnode->properties['B'] = mg_value_make_list(sd->u_deriv);
                ptrnode->properties['C'] = mg_value_make_integer(sd->reverse_v);
                ptrnode->properties['y'] = mg_value_make_list(sd->subset_range);
            } else if (surf2_data->subtype == 3) {
                sphere_data* sd = (sphere_data*)surf2_data;
                ptrnode->properties['z'] = mg_value_make_list(sd->centre);
                ptrnode->properties['A'] = mg_value_make_float(sd->radius);
                ptrnode->properties['B'] = mg_value_make_list(sd->uv_oridir);
                ptrnode->properties['C'] = mg_value_make_list(sd->pole_dir);
                ptrnode->properties['D'] = mg_value_make_integer(sd->reverse_v);
                ptrnode->properties['y'] = mg_value_make_list(sd->subset_range);
            } else if (surf2_data->subtype == 4) {
                torus_data* sd = (torus_data*)surf2_data;
                ptrnode->properties['z'] = mg_value_make_list(sd->centre);
                ptrnode->properties['A'] = mg_value_make_list(sd->normal);
                ptrnode->properties['B'] = mg_value_make_float(sd->major_radius);
                ptrnode->properties['C'] = mg_value_make_float(sd->minor_radius);
                ptrnode->properties['D'] = mg_value_make_list(sd->uv_oridir);
                ptrnode->properties['E'] = mg_value_make_integer(sd->reverse_v);
                ptrnode->properties['y'] = mg_value_make_list(sd->subset_range);
            } else if (surf2_data->subtype == 5) {
                cone_data* sd = (cone_data*)surf2_data;
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
                spline_data* sd = (spline_data*)surf2_data;
                ptrnode->properties['z'] = mg_value_make_integer(sd->reversed);
                ptrnode->properties['y'] = mg_value_make_list(sd->subset_range);
                if (sd->spl_node != nullptr) {
                    Node* b = sd->spl_node;
                    Relationship* r = new Relationship(ptrnode, b);
                    r->label = mg_value_make_string("int_cur_surf2_spl_ptr");
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

            delete surf1_data;
            delete surf2_data;

            return ptrnode;
        }
        Node* createnode_par_cur_subgraph(std::unordered_map<void*, Node*>& ptr2node, std::vector<Relationship*>& relationship_list, par_cur* cur) {
            switch (cur->type()) {
                case 32: {  // exp_par_cur//exppc
                    // 子类类型
                    int subtype = 32; // exppc

                    exp_par_cur* pcur = (exp_par_cur*)cur;
                    bs2_curve bs2_cur = pcur->gme_get_cur_data();
                    bs2_curve_data bs2_cur_data = get_bs2_curve_data(bs2_cur);

                    // 样条类型
                    int rational = bs2_cur_data.subtype;  // rational

                    // fitol
                    double fitol = pcur->gme_get_fitol_data();

                    surface* surf = pcur->gme_get_surf_data();
                    surface_data* surf_data = get_surface_data_subgraph(ptr2node, surf);

                    Node* ptrnode = new Node();
                    ptrnode->labels = mg_list_make_empty(1);
                    mg_list_append(ptrnode->labels, mg_value_make_string("par_cur"));
                    ptrnode->properties['a'] = mg_value_make_string("par_cur");
                    ptrnode->properties['b'] = mg_value_make_integer(subtype);
                    ptrnode->properties['c'] = mg_value_make_integer(rational);
                    ptrnode->properties['d'] = mg_value_make_float(fitol);
                    ptrnode->properties['e'] = mg_value_make_integer(surf_data->subtype);

                    if (rational == 1) { //nubs
                        ptrnode->properties['f'] = mg_value_make_integer(bs2_cur_data.degree);
                        ptrnode->properties['g'] = mg_value_make_integer(bs2_cur_data.form);
                        ptrnode->properties['h'] = mg_value_make_integer(bs2_cur_data.knots_simplifier_size);
                        ptrnode->properties['i'] = mg_value_make_list(bs2_cur_data.knots_simplifier_mglist);
                        ptrnode->properties['j'] = mg_value_make_list(bs2_cur_data.ctrlpts_mglist);
                    }

                    if (surf_data->subtype == 2) {
                        plane_data* sd = (plane_data*)surf_data;
                        ptrnode->properties['l'] = mg_value_make_list(sd->root_point);
                        ptrnode->properties['m'] = mg_value_make_list(sd->normal);
                        ptrnode->properties['n'] = mg_value_make_list(sd->u_deriv);
                        ptrnode->properties['o'] = mg_value_make_integer(sd->reverse_v);
                        ptrnode->properties['k'] = mg_value_make_list(sd->subset_range);
                    } else if (surf_data->subtype == 3) {
                        sphere_data* sd = (sphere_data*)surf_data;
                        ptrnode->properties['l'] = mg_value_make_list(sd->centre);
                        ptrnode->properties['m'] = mg_value_make_float(sd->radius);
                        ptrnode->properties['n'] = mg_value_make_list(sd->uv_oridir);
                        ptrnode->properties['o'] = mg_value_make_list(sd->pole_dir);
                        ptrnode->properties['p'] = mg_value_make_integer(sd->reverse_v);
                        ptrnode->properties['k'] = mg_value_make_list(sd->subset_range);
                    } else if (surf_data->subtype == 4) {
                        torus_data* sd = (torus_data*)surf_data;
                        ptrnode->properties['l'] = mg_value_make_list(sd->centre);
                        ptrnode->properties['m'] = mg_value_make_list(sd->normal);
                        ptrnode->properties['n'] = mg_value_make_float(sd->major_radius);
                        ptrnode->properties['o'] = mg_value_make_float(sd->minor_radius);
                        ptrnode->properties['p'] = mg_value_make_list(sd->uv_oridir);
                        ptrnode->properties['q'] = mg_value_make_integer(sd->reverse_v);
                        ptrnode->properties['k'] = mg_value_make_list(sd->subset_range);
                    } else if (surf_data->subtype == 5) {
                        cone_data* sd = (cone_data*)surf_data;
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
                        spline_data* sd = (spline_data*)surf_data;
                        ptrnode->properties['l'] = mg_value_make_integer(sd->reversed);
                        ptrnode->properties['k'] = mg_value_make_list(sd->subset_range);
                        if (sd->spl_node != nullptr) {
                            Node* b = sd->spl_node;
                            Relationship* r = new Relationship(ptrnode, b);
                            r->label = mg_value_make_string("par_cur_surf_spl_ptr");
                            r->properties['a'] = mg_value_make_string("par_cur_surf_spl_ptr");
                            relationship_list.push_back(r);
                        }
                    }
                    delete surf_data;
                    return ptrnode;
                } break;
                default: {
                    myerror("不支持的par_cur子类型。");
                } break;
            }
        }
    }
    namespace Restore {
        SPAposition parsemglist_SPAposition(const mg_list* mgl, int tag) {
            if (tag == 3) {
                return SPAposition(mg_value_float(mg_list_at(mgl, 0)), mg_value_float(mg_list_at(mgl, 1)), mg_value_float(mg_list_at(mgl, 2)));
            } else if (tag == 2) {
                return SPAposition(mg_value_float(mg_list_at(mgl, 0)), mg_value_float(mg_list_at(mgl, 1)), 0);
            } else if (tag == 1) {
                return SPAposition(mg_value_float(mg_list_at(mgl, 0)), 0, 0);
            } else {
                return SPAposition(mg_value_float(mg_list_at(mgl, 0)), mg_value_float(mg_list_at(mgl, 1)), mg_value_float(mg_list_at(mgl, 2)));
            }
        }
        SPAinterval parsemglist_SPAinterval(const mg_list* mgl) {
            const uint32_t mgl_size = mg_list_size(mgl);
            uint32_t mgl_idx = 0;
            int interval_end_idx = 0;
            double low = 1, high = 0;
            bool low_infinite = false, high_infinite = false;
            while (mgl_idx < mgl_size) {
                int64_t interval_type = mg_value_float(mg_list_at(mgl, mgl_idx));
                if (interval_type == 1.0) { //infinite interval
                    switch (interval_end_idx) {
                        case 0: {
                            low_infinite = true;
                        }break;
                        case 1: {
                            high_infinite = true;
                        }break;
                        default: {
                            myerror("SPAinterval解析出的区间端点个数大于2。");
                        }break;
                    }
                    mgl_idx++;
                    interval_end_idx++;
                } else { //finite interval
                    assert(interval_type == 2.0);
                    double interval_end = mg_value_float(mg_list_at(mgl, mgl_idx + 1));
                    switch (interval_end_idx) {
                        case 0: {
                            low = interval_end;
                        }break;
                        case 1: {
                            high = interval_end;
                        }break;
                        default: {
                            myerror("SPAinterval解析出的区间端点个数大于2。");
                        }break;
                    }
                    mgl_idx += 2;
                    interval_end_idx++;
                }
            }
            if (low_infinite) {
                if (high_infinite) {
                    return SPAinterval(interval_type::interval_infinite);
                } else {
                    return SPAinterval(interval_type::interval_finite_above, low, high);
                }
            } else {
                if (high_infinite) {
                    return  SPAinterval(interval_type::interval_finite_below, low, high);
                } else {
                    return  SPAinterval(interval_type::interval_finite, low, high);
                }
            }
        }
        SPAinterval parseglz_SPAinterval(const glz_SPAinterval& ts) {
            if (!ts.lowfinite) {
                if (!ts.highfinite) {
                    return SPAinterval(interval_type::interval_infinite);
                } else {
                    return SPAinterval(interval_type::interval_finite_above, ts.low.value(), ts.high.value());
                }
            } else {
                if (!ts.highfinite) {
                    return  SPAinterval(interval_type::interval_finite_below, ts.low.value(), ts.high.value());
                } else {
                    return  SPAinterval(interval_type::interval_finite, ts.low.value(), ts.high.value());
                }
            }
        }
        SPApar_box parsemglist_SPApar_box(const mg_list* mgl) {
            const uint32_t mgl_size = mg_list_size(mgl);
            uint32_t mgl_idx = 0;
            int interval_end_idx = 0;
            double u_low = 1, u_high = 0, v_low = 1, v_high = 0;
            bool u_low_infinite = false, u_high_infinite = false, v_low_infinite = false, v_high_infinite = false;
            while (mgl_idx < mgl_size) {
                int64_t interval_type = mg_value_float(mg_list_at(mgl, mgl_idx));
                if (interval_type == 1.0) { //infinite interval
                    switch (interval_end_idx) {
                        case 0: {
                            u_low_infinite = true;
                        }break;
                        case 1: {
                            u_high_infinite = true;
                        }break;
                        case 2: {
                            v_low_infinite = true;
                        }break;
                        case 3: {
                            v_high_infinite = true;
                        }break;
                        default: {
                            myerror("SPApar_box解析出的区间端点个数大于4。");
                        }break;
                    }
                    mgl_idx++;
                    interval_end_idx++;
                } else { //finite interval
                    assert(interval_type == 2.0);
                    double interval_end = mg_value_float(mg_list_at(mgl, mgl_idx + 1));
                    switch (interval_end_idx) {
                        case 0: {
                            u_low = interval_end;
                        }break;
                        case 1: {
                            u_high = interval_end;
                        }break;
                        case 2: {
                            v_low = interval_end;
                        }break;
                        case 3: {
                            v_high = interval_end;
                        }break;
                        default: {
                            myerror("SPApar_box解析出的区间端点个数大于4。");
                        }break;
                    }
                    mgl_idx += 2;
                    interval_end_idx++;
                }
            }
            if (u_low_infinite) {
                if (u_high_infinite) {
                    if (v_low_infinite) {
                        if (v_high_infinite) {
                            return SPApar_box(SPAinterval(interval_type::interval_infinite), SPAinterval(interval_type::interval_infinite));
                        } else {
                            return SPApar_box(SPAinterval(interval_type::interval_infinite), SPAinterval(interval_type::interval_finite_above, v_low, v_high));
                        }
                    } else {
                        if (v_high_infinite) {
                            return SPApar_box(SPAinterval(interval_type::interval_infinite), SPAinterval(interval_type::interval_finite_below, v_low, v_high));
                        } else {
                            return SPApar_box(SPAinterval(interval_type::interval_infinite), SPAinterval(interval_type::interval_finite, v_low, v_high));
                        }
                    }
                } else {
                    if (v_low_infinite) {
                        if (v_high_infinite) {
                            return SPApar_box(SPAinterval(interval_type::interval_finite_above, u_low, u_high), SPAinterval(interval_type::interval_infinite));
                        } else {
                            return SPApar_box(SPAinterval(interval_type::interval_finite_above, u_low, u_high), SPAinterval(interval_type::interval_finite_above, v_low, v_high));
                        }
                    } else {
                        if (v_high_infinite) {
                            return SPApar_box(SPAinterval(interval_type::interval_finite_above, u_low, u_high), SPAinterval(interval_type::interval_finite_below, v_low, v_high));
                        } else {
                            return SPApar_box(SPAinterval(interval_type::interval_finite_above, u_low, u_high), SPAinterval(interval_type::interval_finite, v_low, v_high));
                        }
                    }
                }
            } else {
                if (u_high_infinite) {
                    if (v_low_infinite) {
                        if (v_high_infinite) {
                            return SPApar_box(SPAinterval(interval_type::interval_finite_below, u_low, u_high), SPAinterval(interval_type::interval_infinite));
                        } else {
                            return SPApar_box(SPAinterval(interval_type::interval_finite_below, u_low, u_high), SPAinterval(interval_type::interval_finite_above, v_low, v_high));
                        }
                    } else {
                        if (v_high_infinite) {
                            return SPApar_box(SPAinterval(interval_type::interval_finite_below, u_low, u_high), SPAinterval(interval_type::interval_finite_below, v_low, v_high));
                        } else {
                            return SPApar_box(SPAinterval(interval_type::interval_finite_below, u_low, u_high), SPAinterval(interval_type::interval_finite, v_low, v_high));
                        }
                    }
                } else {
                    if (v_low_infinite) {
                        if (v_high_infinite) {
                            return SPApar_box(SPAinterval(interval_type::interval_finite, u_low, u_high), SPAinterval(interval_type::interval_infinite));
                        } else {
                            return SPApar_box(SPAinterval(interval_type::interval_finite, u_low, u_high), SPAinterval(interval_type::interval_finite_above, v_low, v_high));
                        }
                    } else {
                        if (v_high_infinite) {
                            return SPApar_box(SPAinterval(interval_type::interval_finite, u_low, u_high), SPAinterval(interval_type::interval_finite_below, v_low, v_high));
                        } else {
                            return SPApar_box(SPAinterval(interval_type::interval_finite, u_low, u_high), SPAinterval(interval_type::interval_finite, v_low, v_high));
                        }
                    }
                }
            }

        }
        SPAvector parsemglist_SPAvector(const mg_list* mgl) {
            return SPAvector(mg_value_float(mg_list_at(mgl, 0)), mg_value_float(mg_list_at(mgl, 1)), mg_value_float(mg_list_at(mgl, 2)));
        }
        SPAunit_vector parsemglist_SPAunit_vector(const mg_list* mgl) {
            return SPAunit_vector(mg_value_float(mg_list_at(mgl, 0)), mg_value_float(mg_list_at(mgl, 1)), mg_value_float(mg_list_at(mgl, 2)));
        }
        SPAmatrix parsemglist_SPAmatrix(const mg_list* mgl) {
            SPAvector v1(mg_value_float(mg_list_at(mgl, 0)), mg_value_float(mg_list_at(mgl, 1)), mg_value_float(mg_list_at(mgl, 2)));
            SPAvector v2(mg_value_float(mg_list_at(mgl, 3)), mg_value_float(mg_list_at(mgl, 4)), mg_value_float(mg_list_at(mgl, 5)));
            SPAvector v3(mg_value_float(mg_list_at(mgl, 6)), mg_value_float(mg_list_at(mgl, 7)), mg_value_float(mg_list_at(mgl, 8)));
            return SPAmatrix(v1, v2, v3);
        }
        enum class Neo4jNode {
            body,
            lump,
            shell,
            subshell,
            wire,
            face,
            loop,
            coedge,
            edge,
            vertex,
            pcurve,
            apoint,
            straight_curve,
            ellipse_curve,
            helix_curve,
            intcurve_curve,
            plane_surface,
            sphere_surface,
            torus_surface,
            cone_surface,
            spline_surface,
            transform,
            spl_sur,
            int_cur,
            par_cur
        };
        const std::unordered_map<std::string, Neo4jNode> Neo4jNode_str2enum = {
          {"body",           Neo4jNode::body          },
          {"lump",           Neo4jNode::lump          },
          {"shell",          Neo4jNode::shell         },
          {"subshell",       Neo4jNode::subshell      },
          {"wire",           Neo4jNode::wire          },
          {"face",           Neo4jNode::face          },
          {"loop",           Neo4jNode::loop          },
          {"coedge",         Neo4jNode::coedge        },
          {"edge",           Neo4jNode::edge          },
          {"vertex",         Neo4jNode::vertex        },
          {"pcurve",         Neo4jNode::pcurve        },
          {"point",          Neo4jNode::apoint        },
          {"straight-curve", Neo4jNode::straight_curve},
          {"ellipse-curve",  Neo4jNode::ellipse_curve },
          {"helix-curve",    Neo4jNode::helix_curve   },
          {"intcurve-curve", Neo4jNode::intcurve_curve},
          {"plane-surface",  Neo4jNode::plane_surface },
          {"sphere-surface", Neo4jNode::sphere_surface},
          {"torus-surface",  Neo4jNode::torus_surface },
          {"cone-surface",   Neo4jNode::cone_surface  },
          {"spline-surface", Neo4jNode::spline_surface},
          {"transform",      Neo4jNode::transform     },
          {"spl_sur",        Neo4jNode::spl_sur       },
          {"int_cur",        Neo4jNode::int_cur       },
          {"par_cur",        Neo4jNode::par_cur       },
        };

        enum class Neo4jEdge {
            body_lump_ptr,
            body_wire_ptr,
            body_transform_ptr,
            lump_next_ptr,
            lump_shell_ptr,
            lump_body_ptr,
            shell_next_ptr,
            shell_subshell_ptr,
            shell_face_ptr,
            shell_wire_ptr,
            shell_lump_ptr,
            subshell_parent_ptr,
            subshell_sibling_ptr,
            subshell_child_ptr,
            subshell_face_ptr,
            subshell_wire_ptr,
            wire_next_ptr,
            wire_coedge_ptr,
            wire_owner_ptr,
            wire_subshell_ptr,
            face_next_ptr,
            face_loop_ptr,
            face_shell_ptr,
            face_subshell_ptr,
            face_geometry_ptr,
            loop_next_ptr,
            loop_start_ptr,
            loop_face_ptr,
            coedge_next_ptr,
            coedge_previous_ptr,
            coedge_partner_ptr,
            coedge_edge_ptr,
            coedge_owner_ptr,
            coedge_geometry_ptr,
            edge_start_ptr,
            edge_end_ptr,
            edge_coedge_ptr,
            edge_geometry_ptr,
            vertex_edge_ptr,
            vertex_geometry_ptr,
            pcurve_ref_curve_ptr,
            pcurve_fit_ptr,
            spline_surface_spl_ptr,
            intcurve_curve_fit_ptr,
            int_cur_surf1_spl_ptr,
            int_cur_surf2_spl_ptr,
            par_cur_surf_spl_ptr
        };
        const std::unordered_map<std::string, Neo4jEdge> Neo4jEdge_str2enum = {
          {"body_lump_ptr",          Neo4jEdge::body_lump_ptr         },
          {"body_wire_ptr",          Neo4jEdge::body_wire_ptr         },
          {"body_transform_ptr",     Neo4jEdge::body_transform_ptr    },
          {"lump_next_ptr",          Neo4jEdge::lump_next_ptr         },
          {"lump_shell_ptr",         Neo4jEdge::lump_shell_ptr        },
          {"lump_body_ptr",          Neo4jEdge::lump_body_ptr         },
          {"shell_next_ptr",         Neo4jEdge::shell_next_ptr        },
          {"shell_subshell_ptr",     Neo4jEdge::shell_subshell_ptr    },
          {"shell_face_ptr",         Neo4jEdge::shell_face_ptr        },
          {"shell_wire_ptr",         Neo4jEdge::shell_wire_ptr        },
          {"shell_lump_ptr",         Neo4jEdge::shell_lump_ptr        },
          {"subshell_parent_ptr",    Neo4jEdge::subshell_parent_ptr   },
          {"subshell_sibling_ptr",   Neo4jEdge::subshell_sibling_ptr  },
          {"subshell_child_ptr",     Neo4jEdge::subshell_child_ptr    },
          {"subshell_face_ptr",      Neo4jEdge::subshell_face_ptr     },
          {"subshell_wire_ptr",      Neo4jEdge::subshell_wire_ptr     },
          {"wire_next_ptr",          Neo4jEdge::wire_next_ptr         },
          {"wire_coedge_ptr",        Neo4jEdge::wire_coedge_ptr       },
          {"wire_owner_ptr",         Neo4jEdge::wire_owner_ptr        },
          {"wire_subshell_ptr",      Neo4jEdge::wire_subshell_ptr     },
          {"face_next_ptr",          Neo4jEdge::face_next_ptr         },
          {"face_loop_ptr",          Neo4jEdge::face_loop_ptr         },
          {"face_shell_ptr",         Neo4jEdge::face_shell_ptr        },
          {"face_subshell_ptr",      Neo4jEdge::face_subshell_ptr     },
          {"face_geometry_ptr",      Neo4jEdge::face_geometry_ptr     },
          {"loop_next_ptr",          Neo4jEdge::loop_next_ptr         },
          {"loop_start_ptr",         Neo4jEdge::loop_start_ptr        },
          {"loop_face_ptr",          Neo4jEdge::loop_face_ptr         },
          {"coedge_next_ptr",        Neo4jEdge::coedge_next_ptr       },
          {"coedge_previous_ptr",    Neo4jEdge::coedge_previous_ptr   },
          {"coedge_partner_ptr",     Neo4jEdge::coedge_partner_ptr    },
          {"coedge_edge_ptr",        Neo4jEdge::coedge_edge_ptr       },
          {"coedge_owner_ptr",       Neo4jEdge::coedge_owner_ptr      },
          {"coedge_geometry_ptr",    Neo4jEdge::coedge_geometry_ptr   },
          {"edge_start_ptr",         Neo4jEdge::edge_start_ptr        },
          {"edge_end_ptr",           Neo4jEdge::edge_end_ptr          },
          {"edge_coedge_ptr",        Neo4jEdge::edge_coedge_ptr       },
          {"edge_geometry_ptr",      Neo4jEdge::edge_geometry_ptr     },
          {"vertex_edge_ptr",        Neo4jEdge::vertex_edge_ptr       },
          {"vertex_geometry_ptr",    Neo4jEdge::vertex_geometry_ptr   },
          {"pcurve_ref_curve_ptr",   Neo4jEdge::pcurve_ref_curve_ptr  },
          {"pcurve_fit_ptr",         Neo4jEdge::pcurve_fit_ptr        },
          {"spline-surface_spl_ptr", Neo4jEdge::spline_surface_spl_ptr},
          {"intcurve-curve_fit_ptr", Neo4jEdge::intcurve_curve_fit_ptr},
          {"int_cur_surf1_spl_ptr",  Neo4jEdge::int_cur_surf1_spl_ptr },
          {"int_cur_surf2_spl_ptr",  Neo4jEdge::int_cur_surf2_spl_ptr },
          {"par_cur_surf_spl_ptr",   Neo4jEdge::par_cur_surf_spl_ptr  }
        };

    }
}

void api_save_entity_list_neo4j(const Neo4jPart& conn, const ENTITY_LIST& entity_list, std::unordered_map<void*, int64_t>& ptr2id) {
    TMDF;

    double db_execution_duration = 0;
    for (ENTITY* entity_list_item : entity_list) {
        std::unordered_set<void*> visited;
        std::deque<ENTITY*> que;
        std::string elemid0, elemid1;

        std::unordered_map<void*, Node*> ptr2node;
        std::vector<Relationship*> relationship_list;

        if (ptr2node.count(entity_list_item) == 0) {
            ptr2node[entity_list_item] = new Node();
        }
        que.push_back(entity_list_item);


        while (!que.empty()) {
            ENTITY* entity_ptr = que.front();
            que.pop_front();
            if (entity_ptr == nullptr || visited.find(entity_ptr) != visited.end()) continue;
            switch (entity_ptr->identity(1)) {
                case BODY_ID: {
                    BODY* ptr = (BODY*)entity_ptr;
                    ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, body, lump, wire, transform);
                    Node* ptrnode = ptr2node.at(ptr);
                    ptrnode->labels = mg_list_make_empty(1);
                    mg_list_append(ptrnode->labels, mg_value_make_string("body"));
                    ptrnode->properties['a'] = mg_value_make_string("body");
                } break;
                case LUMP_ID: {
                    LUMP* ptr = (LUMP*)entity_ptr;
                    ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, lump, next, shell, body);
                    Node* ptrnode = ptr2node.at(ptr);
                    ptrnode->labels = mg_list_make_empty(1);
                    mg_list_append(ptrnode->labels, mg_value_make_string("lump"));
                    ptrnode->properties['a'] = mg_value_make_string("lump");
                } break;
                case SHELL_ID: {
                    SHELL* ptr = (SHELL*)entity_ptr;
                    ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, shell, next, subshell, face, wire, lump);
                    Node* ptrnode = ptr2node.at(ptr);
                    ptrnode->labels = mg_list_make_empty(1);
                    mg_list_append(ptrnode->labels, mg_value_make_string("shell"));
                    ptrnode->properties['a'] = mg_value_make_string("shell");
                } break;
                case SUBSHELL_ID: {
                    SUBSHELL* ptr = (SUBSHELL*)entity_ptr;
                    ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, subshell, parent, sibling, child, face, wire);
                    Node* ptrnode = ptr2node.at(ptr);
                    ptrnode->labels = mg_list_make_empty(1);
                    mg_list_append(ptrnode->labels, mg_value_make_string("subshell"));
                    ptrnode->properties['a'] = mg_value_make_string("subshell");
                } break;
                case WIRE_ID: {
                    WIRE* ptr = (WIRE*)entity_ptr;
                    ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, wire, next, coedge, owner, subshell);
                    Node* ptrnode = ptr2node.at(ptr);
                    ptrnode->labels = mg_list_make_empty(1);
                    mg_list_append(ptrnode->labels, mg_value_make_string("wire"));
                    ptrnode->properties['a'] = mg_value_make_string("wire");
                    ptrnode->properties['b'] = mg_value_make_integer(ptr->cont());
                } break;
                case FACE_ID: {
                    FACE* ptr = (FACE*)entity_ptr;
                    ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, face, next, loop, shell, subshell, geometry);
                    if (ptr->sides()) {
                        Node* ptrnode = ptr2node.at(ptr);
                        ptrnode->labels = mg_list_make_empty(1);
                        mg_list_append(ptrnode->labels, mg_value_make_string("face"));
                        ptrnode->properties['a'] = mg_value_make_string("face");
                        ptrnode->properties['b'] = mg_value_make_integer(ptr->sense());
                        ptrnode->properties['c'] = mg_value_make_integer(1);
                        ptrnode->properties['d'] = mg_value_make_integer(ptr->cont());
                    } else {
                        Node* ptrnode = ptr2node.at(ptr);
                        ptrnode->labels = mg_list_make_empty(1);
                        mg_list_append(ptrnode->labels, mg_value_make_string("face"));
                        ptrnode->properties['a'] = mg_value_make_string("face");
                        ptrnode->properties['b'] = mg_value_make_integer(ptr->sense());
                        ptrnode->properties['c'] = mg_value_make_integer(0);
                    }
                } break;
                case LOOP_ID: {
                    LOOP* ptr = (LOOP*)entity_ptr;
                    ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, loop, next, start, face);
                    Node* ptrnode = ptr2node.at(ptr);
                    ptrnode->labels = mg_list_make_empty(1);
                    mg_list_append(ptrnode->labels, mg_value_make_string("loop"));
                    ptrnode->properties['a'] = mg_value_make_string("loop");
                } break;
                case COEDGE_ID: {
                    COEDGE* ptr = (COEDGE*)entity_ptr;
                    ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, coedge, next, previous, partner, edge, owner, geometry);
                    Node* ptrnode = ptr2node.at(ptr);
                    ptrnode->labels = mg_list_make_empty(1);
                    mg_list_append(ptrnode->labels, mg_value_make_string("coedge"));
                    ptrnode->properties['a'] = mg_value_make_string("coedge");
                    ptrnode->properties['b'] = mg_value_make_integer(ptr->sense());
                } break;
                case EDGE_ID: {
                    EDGE* ptr = (EDGE*)entity_ptr;
                    ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, edge, start, end, coedge, geometry);
                    Node* ptrnode = ptr2node.at(ptr);
                    ptrnode->labels = mg_list_make_empty(1);
                    mg_list_append(ptrnode->labels, mg_value_make_string("edge"));
                    ptrnode->properties['a'] = mg_value_make_string("edge");
                    ptrnode->properties['b'] = mg_value_make_integer(ptr->sense());
                } break;
                case VERTEX_ID: {
                    VERTEX* ptr = (VERTEX*)entity_ptr;
                    ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, vertex, edge, geometry);
                    Node* ptrnode = ptr2node.at(ptr);
                    ptrnode->labels = mg_list_make_empty(1);
                    mg_list_append(ptrnode->labels, mg_value_make_string("vertex"));
                    ptrnode->properties['a'] = mg_value_make_string("vertex");
                } break;
                case PCURVE_ID: {
                    PCURVE* ptr = (PCURVE*)entity_ptr;
                    int def_type = ((PCURVE*)ptr)->gme_get_def_type();
                    if (def_type != 0) {
                        ITERATE_MACRO_WITH_PARAM2(_API_PUSH_PTR_NEO4J_SUBGRAPH, ptr, pcurve, ref_curve);
                        SPApar_vec offset = ((PCURVE*)ptr)->offset();
                        Node* ptrnode = ptr2node.at(ptr);
                        ptrnode->labels = mg_list_make_empty(1);
                        mg_list_append(ptrnode->labels, mg_value_make_string("pcurve"));
                        ptrnode->properties['a'] = mg_value_make_string("pcurve");
                        ptrnode->properties['b'] = mg_value_make_integer(def_type);
                        ptrnode->properties['c'] = mg_value_make_float(double(offset.du));
                        ptrnode->properties['d'] = mg_value_make_float(double(offset.dv));
                    } else {
                        pcurve pcur = ((PCURVE*)ptr)->gme_get_def();

                        int rev = pcur.reversed();
                        SPApar_vec offset = pcur.offset();
                        par_cur* cur = pcur.gme_get_fit();
                        if (cur != nullptr) {
                            if (ptr2node.find(cur) == ptr2node.end()) {
                                ptr2node[cur] = AccessUtils::Save::createnode_par_cur_subgraph(ptr2node, relationship_list, cur);
                            }
                            Node* a = ptr2node.at(ptr);
                            Node* b = ptr2node.at(cur);
                            Relationship* r = new Relationship(a, b);
                            r->label = mg_value_make_string("pcurve_fit_ptr");
                            r->properties['a'] = mg_value_make_string("pcurve_fit_ptr");
                            relationship_list.push_back(r);
                        }
                        Node* ptrnode = ptr2node.at(ptr);
                        ptrnode->labels = mg_list_make_empty(1);
                        mg_list_append(ptrnode->labels, mg_value_make_string("pcurve"));
                        ptrnode->properties['a'] = mg_value_make_string("pcurve");
                        ptrnode->properties['b'] = mg_value_make_integer(def_type);
                        ptrnode->properties['c'] = mg_value_make_float(double(offset.du));
                        ptrnode->properties['d'] = mg_value_make_float(double(offset.dv));
                        ptrnode->properties['e'] = mg_value_make_integer(rev);
                    }
                } break;
                case APOINT_ID: {
                    APOINT* ptr = (APOINT*)entity_ptr;
                    SPAposition pos = ((APOINT*)ptr)->coords();
                    Node* ptrnode = ptr2node.at(ptr);
                    ptrnode->labels = mg_list_make_empty(1);
                    mg_list_append(ptrnode->labels, mg_value_make_string("point"));
                    ptrnode->properties['a'] = mg_value_make_string("point");
                    ptrnode->properties['b'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAposition(pos, 3));
                } break;
                case CURVE_ID: {
                    CURVE* ptr = (CURVE*)entity_ptr;
                    switch (ptr->identity(2)) {
                        case STRAIGHT_ID: {
                            STRAIGHT* ptr = (STRAIGHT*)entity_ptr;
                            straight gem = ((STRAIGHT*)ptr)->gme_get_def();
                            SPAposition root_point = gem.root_point;
                            SPAunit_vector direction = gem.direction;
                            direction.set_x(direction.x() * gem.param_scale);
                            direction.set_y(direction.y() * gem.param_scale);
                            direction.set_z(direction.z() * gem.param_scale);
                            SPAinterval range = gem.gme_get_subset_range();
                            Node* ptrnode = ptr2node.at(ptr);
                            ptrnode->labels = mg_list_make_empty(1);
                            mg_list_append(ptrnode->labels, mg_value_make_string("straight-curve"));
                            ptrnode->properties['a'] = mg_value_make_string("straight-curve");
                            ptrnode->properties['b'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAinterval(range));
                            ptrnode->properties['c'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAposition(root_point, 3));
                            ptrnode->properties['d'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAvector(direction));
                        } break;
                        case ELLIPSE_ID: {
                            ELLIPSE* ptr = (ELLIPSE*)entity_ptr;
                            ellipse gem = ((ELLIPSE*)ptr)->gme_get_def();
                            SPAposition centre = gem.centre;
                            SPAunit_vector normal = gem.normal;
                            SPAvector major_axis = gem.major_axis;
                            SPAinterval range = gem.gme_get_subset_range();
                            Node* ptrnode = ptr2node.at(ptr);
                            ptrnode->labels = mg_list_make_empty(1);
                            mg_list_append(ptrnode->labels, mg_value_make_string("ellipse-curve"));
                            ptrnode->properties['a'] = mg_value_make_string("ellipse-curve");
                            ptrnode->properties['b'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAinterval(range));
                            ptrnode->properties['c'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAposition(centre, 3));
                            ptrnode->properties['d'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAunit_vector(normal));
                            ptrnode->properties['e'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAvector(major_axis));
                            ptrnode->properties['f'] = mg_value_make_float(gem.radius_ratio);
                        } break;
                        case HELIX_ID: {
                            HELIX* ptr = (HELIX*)entity_ptr;
                            helix gem = ((HELIX*)ptr)->gme_get_def();
                            SPAposition axis_root = gem.axis_root();
                            SPAunit_vector axis_dir = gem.axis_dir();
                            SPAvector start_disp = gem.start_disp();
                            SPAinterval helix_range = gem.helix_range();
                            SPAinterval range = gem.gme_get_subset_range();
                            Node* ptrnode = ptr2node.at(ptr);
                            ptrnode->labels = mg_list_make_empty(1);
                            mg_list_append(ptrnode->labels, mg_value_make_string("helix-curve"));
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
                        case INTCURVE_ID: {
                            INTCURVE* ptr = (INTCURVE*)entity_ptr;
                            intcurve gem = ((INTCURVE*)ptr)->gme_get_def();
                            SPAinterval range = gem.gme_get_subset_range();
                            int_cur* cur = gem.gme_get_fit();
                            gem.cur();
                            if (cur != nullptr) {
                                if (ptr2node.find(cur) == ptr2node.end()) {
                                    ptr2node[cur] = AccessUtils::Save::createnode_int_cur_subgraph(ptr2node, relationship_list, cur);
                                }
                                Node* a = ptr2node.at(ptr);
                                Node* b = ptr2node.at(cur);
                                Relationship* r = new Relationship(a, b);
                                r->label = mg_value_make_string("intcurve-curve_fit_ptr");
                                r->properties['a'] = mg_value_make_string("intcurve-curve_fit_ptr");
                                relationship_list.push_back(r);
                            }
                            Node* ptrnode = ptr2node.at(ptr);
                            ptrnode->labels = mg_list_make_empty(1);
                            mg_list_append(ptrnode->labels, mg_value_make_string("intcurve-curve"));
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
                    SURFACE* ptr = (SURFACE*)entity_ptr;
                    switch (ptr->identity(2)) {
                        case PLANE_ID: {
                            PLANE* ptr = (PLANE*)entity_ptr;
                            plane gem = ((PLANE*)ptr)->gme_get_def();
                            AccessUtils::Save::plane_data* gem_data = AccessUtils::Save::get_plane_data(&gem);
                            Node* ptrnode = ptr2node.at(ptr);
                            ptrnode->labels = mg_list_make_empty(1);
                            mg_list_append(ptrnode->labels, mg_value_make_string("plane-surface"));
                            ptrnode->properties['a'] = mg_value_make_string("plane-surface");
                            ptrnode->properties['b'] = mg_value_make_list(gem_data->subset_range);
                            ptrnode->properties['c'] = mg_value_make_list(gem_data->root_point);
                            ptrnode->properties['d'] = mg_value_make_list(gem_data->normal);
                            ptrnode->properties['e'] = mg_value_make_list(gem_data->u_deriv);
                            ptrnode->properties['f'] = mg_value_make_integer(gem_data->reverse_v);
                            delete gem_data;
                        } break;
                        case SPHERE_ID: {
                            SPHERE* ptr = (SPHERE*)entity_ptr;
                            sphere gem = ((SPHERE*)ptr)->gme_get_def();
                            AccessUtils::Save::sphere_data* gem_data = AccessUtils::Save::get_sphere_data(&gem);
                            Node* ptrnode = ptr2node.at(ptr);
                            ptrnode->labels = mg_list_make_empty(1);
                            mg_list_append(ptrnode->labels, mg_value_make_string("sphere-surface"));
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
                            TORUS* ptr = (TORUS*)entity_ptr;
                            torus gem = ((TORUS*)ptr)->gme_get_def();
                            AccessUtils::Save::torus_data* gem_data = AccessUtils::Save::get_torus_data(&gem);
                            Node* ptrnode = ptr2node.at(ptr);
                            ptrnode->labels = mg_list_make_empty(1);
                            mg_list_append(ptrnode->labels, mg_value_make_string("torus-surface"));
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
                            CONE* ptr = (CONE*)entity_ptr;
                            cone gem = ((CONE*)ptr)->gme_get_def();
                            AccessUtils::Save::cone_data* gem_data = AccessUtils::Save::get_cone_data(&gem);
                            Node* ptrnode = ptr2node.at(ptr);
                            ptrnode->labels = mg_list_make_empty(1);
                            mg_list_append(ptrnode->labels, mg_value_make_string("cone-surface"));
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
                            SPLINE* ptr = (SPLINE*)entity_ptr;
                            spline gem = ((SPLINE*)ptr)->gme_get_def();
                            AccessUtils::Save::spline_data* gem_data = AccessUtils::Save::get_spline_data_subgraph(ptr2node, &gem);
                            if (gem_data->spl_node != nullptr) {
                                Node* a = ptr2node.at(ptr);
                                Node* b = gem_data->spl_node;
                                Relationship* r = new Relationship(a, b);
                                r->label = mg_value_make_string("spline-surface_spl_ptr");
                                r->properties['a'] = mg_value_make_string("spline-surface_spl_ptr");
                                relationship_list.push_back(r);
                            }
                            Node* ptrnode = ptr2node.at(ptr);
                            ptrnode->labels = mg_list_make_empty(1);
                            mg_list_append(ptrnode->labels, mg_value_make_string("spline-surface"));
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
                    TRANSFORM* ptr = (TRANSFORM*)entity_ptr;
                    SPAtransf transf = ((TRANSFORM*)ptr)->transform();
                    SPAmatrix affine = transf.affine();
                    SPAvector translation = transf.translation();
                    Node* ptrnode = ptr2node.at(ptr);
                    ptrnode->labels = mg_list_make_empty(1);
                    mg_list_append(ptrnode->labels, mg_value_make_string("transform"));
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
            visited.insert(entity_ptr);
        }
        {
            uint32_t node_list_size = ptr2node.size();
            mg_list* mgl_node_list = mg_list_make_empty(node_list_size);
            int nodeidx = 0;
            for (auto [ptr, node] : ptr2node) {
                mg_map* mgm_node = mg_map_make_empty(2 + node->properties.size());
                mg_map_append(mgm_node, mg_string_make("W"), mg_value_make_integer(nodeidx));
                mg_map_append(mgm_node, mg_string_make("X"), mg_value_make_list(node->labels));
                for (auto [propkey, propval] : node->properties) {
                    const char propkey_str[2] = { propkey, 0 };
                    mg_map_append(mgm_node, mg_string_make(propkey_str), propval);
                }
                mg_list_append(mgl_node_list, mg_value_make_map(mgm_node));
                nodeidx++;
            }
            mg_map* qparams = mg_map_make_empty(1);
            mg_map_append(qparams, mg_string_make("Y"), mg_value_make_list(mgl_node_list));
            TMST;
            conn.execute_bolt("UNWIND $Y AS Z "
                "CALL apoc.create.node(Z.X,{a:Z.a,b:Z.b,c:Z.c,d:Z.d,e:Z.e,f:Z.f,g:Z.g,h:Z.h,i:Z.i,j:Z.j,k:Z.k,l:Z.l,m:Z.m,n:Z.n,o:Z.o,p:Z.p,q:Z.q,r:Z.r,s:Z.s,t:Z.t,u:Z.u,v:Z.v,w:Z.w,x:Z.x,y:Z.y,z:Z.z,A:Z.A,B:Z.B,C:Z.C,D:Z.D,E:Z.E,F:Z.F,G:Z.G,H:Z.H,I:Z.I,J:Z.J,K:Z.K,L:Z.L,M:Z.M,N:Z.N,O:Z.O,P:Z.P,Q:Z.Q}) "
                "YIELD node RETURN id(node) ORDER BY Z.W ", qparams);
            TMED;
            db_execution_duration += TMDR;
            mg_map_destroy(qparams);
        }
        {
            mg_result* result;
            for (auto [ptr, node] : ptr2node) {
                if (mg_session_fetch(conn.session, &result) == 1) {
                    const mg_list* mgl_nodeid = mg_result_row(result);
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
            mg_list* mgl_rel_list = mg_list_make_empty(rel_list_size);
            for (auto rel : relationship_list) {
                mg_map* mgm_rel = mg_map_make_empty(3 + rel->properties.size());
                mg_map_append(mgm_rel, mg_string_make("U"), mg_value_make_integer(rel->u->id));
                mg_map_append(mgm_rel, mg_string_make("V"), mg_value_make_integer(rel->v->id));
                mg_map_append(mgm_rel, mg_string_make("T"), rel->label);
                for (auto [propkey, propval] : rel->properties) {
                    const char propkey_str[2] = { propkey, 0 };
                    mg_map_append(mgm_rel, mg_string_make(propkey_str), propval);
                }
                mg_list_append(mgl_rel_list, mg_value_make_map(mgm_rel));
            }
            mg_map* qparams = mg_map_make_empty(1);
            mg_map_append(qparams, mg_string_make("Y"), mg_value_make_list(mgl_rel_list));
            TMST;
            conn.execute_bolt("UNWIND $Y AS Z "
                "MATCH (W) WHERE id(W) = Z.U "
                "MATCH (X) WHERE id(X) = Z.V "
                "CALL apoc.create.relationship(W,Z.T,{a:Z.a},X)  "
                "YIELD rel RETURN null LIMIT 0 ", qparams);
            TMED;
            db_execution_duration += TMDR;
            mg_map_destroy(qparams);
            conn.discard_all_results();
        }


        for (auto [ptr, node] : ptr2node) {
            delete node;
        }
        for (auto rel : relationship_list) {
            delete rel;
        }
    }
    qDebug() << "save_neo4j:" << db_execution_duration;
}



void api_restore_entity_list_neo4j(const Neo4jPart& conn, const std::vector<int64_t>& id_list, ENTITY_LIST& entity_list, std::unordered_map<int64_t, void*>& id2ptr) {
    uint32_t id_list_size = id_list.size();
    for (uint32_t id_list_idx = 0; id_list_idx < id_list_size; id_list_idx++) {
        mg_map* qparams = mg_map_make_empty(1);
        mg_map_append(qparams, mg_string_make("d"), mg_value_make_integer(id_list[id_list_idx]));
        conn.execute_bolt("MATCH (n) WHERE id(n) = $d "
            "CALL apoc.path.subgraphAll(n, {minLevel:0}) YIELD nodes AS x, relationships AS y return x,y ", qparams);
        mg_map_destroy(qparams);

        mg_result* result;
        int rows_cnt = 0;
        int status;
        while (1) {
            status = mg_session_fetch(conn.session, &result);
            if (status == 1) {
                rows_cnt++;
                const mg_list* noderellist = mg_result_row(result);
                const uint32_t noderellist_length = mg_list_size(noderellist);
                assert(noderellist_length == 2);
                {
                    const mg_value* nodelist_value = mg_list_at(noderellist, 0);
                    assert(mg_value_get_type(nodelist_value) == MG_VALUE_TYPE_LIST);
                    const mg_list* nodelist = mg_value_list(nodelist_value);
                    const uint32_t nodelist_size = mg_list_size(nodelist);
                    for (uint32_t i = 0; i < nodelist_size; i++) {
                        const mg_value* node_value = mg_list_at(nodelist, i);
                        assert(mg_value_get_type(node_value) == MG_VALUE_TYPE_NODE);
                        const mg_node* node = mg_value_node(node_value);
                        int64_t node_id = mg_node_id(node);
                        const mg_map* node_properties = mg_node_properties(node);
                        const mg_string* node_typename_mgs = mg_value_string(mg_map_at(node_properties, "a"));
                        std::string node_typename(mg_string_data(node_typename_mgs), mg_string_size(node_typename_mgs));
                        if (node_typename == "part") { continue; }
                        switch (AccessUtils::Restore::Neo4jNode_str2enum.at(node_typename)) {
                            case AccessUtils::Restore::Neo4jNode::body: {
                                BODY* body = nullptr;
                                api_body(body);
                                id2ptr[node_id] = body;
                            } break;
                            case AccessUtils::Restore::Neo4jNode::lump: {
                                LUMP* lump = nullptr;
                                API_BEGIN;
                                lump = ACIS_NEW LUMP();
                                API_END;
                                id2ptr[node_id] = lump;
                            } break;
                            case AccessUtils::Restore::Neo4jNode::shell: {
                                SHELL* shell = nullptr;
                                API_BEGIN;
                                shell = ACIS_NEW SHELL();
                                API_END;
                                id2ptr[node_id] = shell;
                            } break;
                            case AccessUtils::Restore::Neo4jNode::subshell: {
                                SUBSHELL* subshell = nullptr;
                                API_BEGIN;
                                subshell = ACIS_NEW SUBSHELL();
                                API_END;
                                id2ptr[node_id] = subshell;
                            } break;
                            case AccessUtils::Restore::Neo4jNode::face: {
                                FACE* face = nullptr;
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
                                LOOP* loop = nullptr;
                                API_BEGIN;
                                loop = ACIS_NEW LOOP();
                                API_END;
                                id2ptr[node_id] = loop;
                            } break;
                            case AccessUtils::Restore::Neo4jNode::wire: {
                                WIRE* wire = nullptr;
                                API_BEGIN;
                                wire = ACIS_NEW WIRE();
                                API_END;
                                wire->set_cont(mg_value_integer(mg_map_at(node_properties, "b")));
                                id2ptr[node_id] = wire;
                            } break;
                            case AccessUtils::Restore::Neo4jNode::coedge: {
                                COEDGE* coedge = nullptr;
                                API_BEGIN;
                                coedge = ACIS_NEW COEDGE();
                                API_END;
                                coedge->set_sense(mg_value_integer(mg_map_at(node_properties, "b")));
                                id2ptr[node_id] = coedge;
                            } break;
                            case AccessUtils::Restore::Neo4jNode::edge: {
                                EDGE* edge = nullptr;
                                API_BEGIN;
                                edge = ACIS_NEW EDGE();
                                API_END;
                                edge->set_sense(mg_value_integer(mg_map_at(node_properties, "b")));
                                id2ptr[node_id] = edge;
                            } break;
                            case AccessUtils::Restore::Neo4jNode::vertex: {
                                VERTEX* vertex = nullptr;
                                API_BEGIN;
                                vertex = ACIS_NEW VERTEX();
                                API_END;
                                id2ptr[node_id] = vertex;
                            } break;
                            case AccessUtils::Restore::Neo4jNode::transform: {
                                SPAmatrix affine_part = AccessUtils::Restore::parsemglist_SPAmatrix(mg_value_list(mg_map_at(node_properties, "b")));
                                SPAvector translation_part = AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "c")));
                                double scaling_part = mg_value_float(mg_map_at(node_properties, "d"));
                                int rotate_flag = mg_value_integer(mg_map_at(node_properties, "e"));
                                int reflect_flag = mg_value_integer(mg_map_at(node_properties, "f"));
                                int shear_flag = mg_value_integer(mg_map_at(node_properties, "g"));
                                SPAtransf transform_data(affine_part, translation_part, scaling_part, rotate_flag, reflect_flag, shear_flag);
                                TRANSFORM* transform = nullptr;
                                API_BEGIN;
                                transform = ACIS_NEW TRANSFORM(transform_data);
                                API_END;
                                id2ptr[node_id] = transform;
                            } break;
                            case AccessUtils::Restore::Neo4jNode::apoint: {
                                SPAposition coords_data = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "b")), 3);
                                APOINT* point = nullptr;
                                API_BEGIN;
                                point = ACIS_NEW APOINT(coords_data);
                                API_END;
                                id2ptr[node_id] = point;
                            } break;
                            case AccessUtils::Restore::Neo4jNode::straight_curve: {
                                SPAposition root_point = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "c")), 3);
                                SPAvector direction = AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "d")));
                                straight* def = ACIS_NEW straight(root_point, normalise(direction));
                                def->gme_set_param_scale(direction.len());
                                SPAinterval subset_range = AccessUtils::Restore::parsemglist_SPAinterval(mg_value_list(mg_map_at(node_properties, "b")));
                                def->gme_set_subset_range(subset_range);
                                STRAIGHT* straight_curve = nullptr;
                                API_BEGIN;
                                straight_curve = ACIS_NEW STRAIGHT(*def);
                                API_END;
                                ACIS_DELETE def;
                                id2ptr[node_id] = straight_curve;
                            } break;
                            case AccessUtils::Restore::Neo4jNode::ellipse_curve: {
                                SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "c")), 3);
                                SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "d")));
                                SPAvector major_axis = AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "e")));
                                double radius_ratio = mg_value_float(mg_map_at(node_properties, "f"));
                                ellipse* def = ACIS_NEW ellipse(centre, normal, major_axis, radius_ratio);
                                SPAinterval subset_range = AccessUtils::Restore::parsemglist_SPAinterval(mg_value_list(mg_map_at(node_properties, "b")));
                                def->gme_set_subset_range(subset_range);
                                ELLIPSE* ellipse_curve = nullptr;
                                API_BEGIN;
                                ellipse_curve = ACIS_NEW ELLIPSE(*def);
                                API_END;
                                ACIS_DELETE def;
                                id2ptr[node_id] = ellipse_curve;
                            } break;
                            case AccessUtils::Restore::Neo4jNode::helix_curve: {
                                SPAposition axis_root = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "c")), 3);
                                SPAunit_vector axis_dir = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "d")));
                                SPAvector start_disp = AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "e")));
                                double pitch = mg_value_float(mg_map_at(node_properties, "f"));
                                int handedness = mg_value_integer(mg_map_at(node_properties, "g"));
                                double par_scaling = mg_value_float(mg_map_at(node_properties, "h"));
                                double taper = mg_value_float(mg_map_at(node_properties, "i"));
                                SPAinterval helix_range = AccessUtils::Restore::parsemglist_SPAinterval(mg_value_list(mg_map_at(node_properties, "j")));
                                helix* def = ACIS_NEW helix(axis_root, axis_dir, start_disp, pitch, handedness, helix_range, par_scaling, taper);
                                SPAinterval subset_range = AccessUtils::Restore::parsemglist_SPAinterval(mg_value_list(mg_map_at(node_properties, "b")));
                                def->gme_set_subset_range(subset_range);
                                HELIX* helix_curve = nullptr;
                                API_BEGIN;
                                helix_curve = ACIS_NEW HELIX(*def);
                                API_END;
                                ACIS_DELETE def;
                                id2ptr[node_id] = helix_curve;
                            } break;
                            case AccessUtils::Restore::Neo4jNode::intcurve_curve: {
                                intcurve* def = ACIS_NEW intcurve();
                                def->gme_set_rev(mg_value_integer(mg_map_at(node_properties, "c")));
                                def->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPAinterval(mg_value_list(mg_map_at(node_properties, "b"))));
                                INTCURVE* intcurve_curve = nullptr;
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
                                const mg_list* knots_simplifier_mglist = mg_value_list(mg_map_at(node_properties, "g"));
                                const mg_list* ctrlpts_mglist = mg_value_list(mg_map_at(node_properties, "h"));
                                double cur_fitol = mg_value_float(mg_map_at(node_properties, "i"));
                                int surf1_type = mg_value_integer(mg_map_at(node_properties, "j"));
                                int surf2_type = mg_value_integer(mg_map_at(node_properties, "k"));
                                int pcur1_type = mg_value_integer(mg_map_at(node_properties, "l"));
                                int pcur2_type = mg_value_integer(mg_map_at(node_properties, "m"));
                                SPAinterval safe_range = AccessUtils::Restore::parsemglist_SPAinterval(mg_value_list(mg_map_at(node_properties, "n")));
                                int cur_surface_tag = mg_value_integer(mg_map_at(node_properties, "o"));

                                std::deque<double> cur_knots;
                                for (int i = 0; i < cur_knots_num; i++) {
                                    double knot = mg_value_float(mg_list_at(knots_simplifier_mglist, i * 2));
                                    int knot_cnt = mg_value_float(mg_list_at(knots_simplifier_mglist, i * 2 + 1));
                                    for (int j = 0; j < knot_cnt; j++) cur_knots.push_back(knot);
                                }
                                cur_knots.push_front(cur_knots.front());
                                cur_knots.push_back(cur_knots.back());

                                // control points
                                int cur_ctrl_points_num = cur_knots.size() - cur_degree - 1;
                                SPAposition* cur_ctrl_points = ACIS_NEW SPAposition[cur_ctrl_points_num];
                                double* cur_weights = nullptr;
                                if (cur_rational_flag) {
                                    cur_weights = new double[cur_ctrl_points_num];
                                }
                                int cur_ctrl_points_idx = 0;
                                for (int i = 0; i < cur_ctrl_points_num; i++) {
                                    SPAposition ctrl_point = SPAposition(mg_value_float(mg_list_at(ctrlpts_mglist, cur_ctrl_points_idx)), mg_value_float(mg_list_at(ctrlpts_mglist, cur_ctrl_points_idx + 1)), mg_value_float(mg_list_at(ctrlpts_mglist, cur_ctrl_points_idx + 2)));
                                    cur_ctrl_points_idx += 3;
                                    if (cur_rational_flag) {
                                        double weight = mg_value_float(mg_list_at(ctrlpts_mglist, cur_ctrl_points_idx));
                                        cur_ctrl_points_idx++;
                                        cur_weights[i] = weight;
                                    }
                                    cur_ctrl_points[i] = ctrl_point;
                                }

                                // create bs3_surface
                                bs3_curve bs_cur = bs3_curve_from_ctrlpts(cur_degree, cur_rational_flag, cur_closed, cur_periodic, cur_ctrl_points_num, cur_ctrl_points, cur_rational_flag ? cur_weights : nullptr, SPAresabs.value(), cur_knots.size(),
                                    std::vector<double>(cur_knots.begin(), cur_knots.end()).data(), SPAresnor.value());

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
                                surface* surf1 = nullptr;
                                if (surf1_type == 2) {
                                    SPAposition root_point = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "q")), 3);
                                    SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "r")));
                                    SPAvector u_deriv = AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "s")));
                                    plane* ret_plane = ACIS_NEW plane(root_point, normal, u_deriv);
                                    ret_plane->reverse_v = mg_value_integer(mg_map_at(node_properties, "t"));
                                    ret_plane->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "p"))));
                                    surf1 = ret_plane;
                                } else if (surf1_type == 3) {
                                    SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "q")), 3);
                                    double radius = mg_value_float(mg_map_at(node_properties, "r"));
                                    sphere* ret_sphere = ACIS_NEW sphere(centre, radius);
                                    ret_sphere->uv_oridir = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "s")));
                                    ret_sphere->pole_dir = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "t")));
                                    ret_sphere->reverse_v = mg_value_integer(mg_map_at(node_properties, "u"));
                                    ret_sphere->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "p"))));
                                    surf1 = ret_sphere;
                                } else if (surf1_type == 4) {
                                    SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "q")), 3);
                                    SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "r")));
                                    double major_radius = mg_value_float(mg_map_at(node_properties, "s"));
                                    double minor_radius = mg_value_float(mg_map_at(node_properties, "t"));
                                    torus* ret_torus = ACIS_NEW torus(centre, normal, major_radius, minor_radius);
                                    ret_torus->uv_oridir = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "u")));
                                    ret_torus->reverse_v = mg_value_integer(mg_map_at(node_properties, "v"));
                                    ret_torus->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "p"))));
                                    surf1 = ret_torus;
                                } else if (surf1_type == 5) {
                                    SPAposition base_centre = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "q")), 3);
                                    SPAunit_vector base_normal = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "r")));
                                    SPAvector base_major_axis = AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "s")));
                                    double base_radius_ratio = mg_value_float(mg_map_at(node_properties, "t"));
                                    ellipse* gem_base = ACIS_NEW ellipse(base_centre, base_normal, base_major_axis, base_radius_ratio);
                                    gem_base->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPAinterval(mg_value_list(mg_map_at(node_properties, "u"))));
                                    double sine_angle = mg_value_float(mg_map_at(node_properties, "v"));
                                    double cosine_angle = mg_value_float(mg_map_at(node_properties, "w"));
                                    cone* ret_cone = ACIS_NEW cone(*gem_base, sine_angle, cosine_angle);
                                    ACIS_DELETE gem_base;
                                    ret_cone->reverse_u = mg_value_integer(mg_map_at(node_properties, "x"));
                                    ret_cone->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "p"))));
                                    surf1 = ret_cone;
                                } else if (surf1_type == 1) {
                                    spline* ret_spline = ACIS_NEW spline();
                                    ret_spline->gme_set_rev(mg_value_integer(mg_map_at(node_properties, "q")));
                                    ret_spline->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "p"))));
                                    surf1 = ret_spline;
                                }

                                // surf2
                                surface* surf2 = nullptr;
                                if (surf2_type == 2) {
                                    SPAposition root_point = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "z")), 3);
                                    SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "A")));
                                    SPAvector u_deriv = AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "B")));
                                    plane* ret_plane = ACIS_NEW plane(root_point, normal, u_deriv);
                                    ret_plane->reverse_v = mg_value_integer(mg_map_at(node_properties, "C"));
                                    ret_plane->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "y"))));
                                    surf2 = ret_plane;
                                } else if (surf2_type == 3) {
                                    SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "z")), 3);
                                    double radius = mg_value_float(mg_map_at(node_properties, "A"));
                                    sphere* ret_sphere = ACIS_NEW sphere(centre, radius);
                                    ret_sphere->uv_oridir = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "B")));
                                    ret_sphere->pole_dir = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "C")));
                                    ret_sphere->reverse_v = mg_value_integer(mg_map_at(node_properties, "D"));
                                    ret_sphere->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "y"))));
                                    surf2 = ret_sphere;
                                } else if (surf2_type == 4) {
                                    SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "z")), 3);
                                    SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "A")));
                                    double major_radius = mg_value_float(mg_map_at(node_properties, "B"));
                                    double minor_radius = mg_value_float(mg_map_at(node_properties, "C"));
                                    torus* ret_torus = ACIS_NEW torus(centre, normal, major_radius, minor_radius);
                                    ret_torus->uv_oridir = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "D")));
                                    ret_torus->reverse_v = mg_value_integer(mg_map_at(node_properties, "E"));
                                    ret_torus->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "y"))));
                                    surf2 = ret_torus;
                                } else if (surf2_type == 5) {
                                    SPAposition base_centre = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "z")), 3);
                                    SPAunit_vector base_normal = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "A")));
                                    SPAvector base_major_axis = AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "B")));
                                    double base_radius_ratio = mg_value_float(mg_map_at(node_properties, "C"));
                                    ellipse* gem_base = ACIS_NEW ellipse(base_centre, base_normal, base_major_axis, base_radius_ratio);
                                    gem_base->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPAinterval(mg_value_list(mg_map_at(node_properties, "D"))));
                                    double sine_angle = mg_value_float(mg_map_at(node_properties, "E"));
                                    double cosine_angle = mg_value_float(mg_map_at(node_properties, "F"));
                                    cone* ret_cone = ACIS_NEW cone(*gem_base, sine_angle, cosine_angle);
                                    ACIS_DELETE gem_base;
                                    ret_cone->reverse_u = mg_value_integer(mg_map_at(node_properties, "G"));
                                    ret_cone->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "y"))));
                                    surf2 = ret_cone;
                                } else if (surf2_type == 1) {
                                    spline* ret_spline = ACIS_NEW spline();
                                    ret_spline->gme_set_rev(mg_value_integer(mg_map_at(node_properties, "z")));
                                    ret_spline->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "y"))));
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
                                    const mg_list* knots_simplifier_mglist = mg_value_list(mg_map_at(node_properties, "K"));
                                    const mg_list* ctrlpts_mglist = mg_value_list(mg_map_at(node_properties, "L"));

                                    std::deque<double> pcur1_knots;
                                    for (int i = 0; i < pcur1_knots_num; i++) {
                                        double knot = mg_value_float(mg_list_at(knots_simplifier_mglist, i * 2));
                                        int knot_cnt = mg_value_float(mg_list_at(knots_simplifier_mglist, i * 2 + 1));
                                        for (int j = 0; j < knot_cnt; j++) pcur1_knots.push_back(knot);
                                    }
                                    pcur1_knots.push_front(pcur1_knots.front());
                                    pcur1_knots.push_back(pcur1_knots.back());

                                    // control points
                                    int pcur1_ctrl_points_num = pcur1_knots.size() - pcur1_degree - 1;
                                    SPAposition* pcur1_ctrl_points = ACIS_NEW SPAposition[pcur1_ctrl_points_num];
                                    for (int i = 0; i < pcur1_ctrl_points_num; i++) {
                                        SPAposition ctrl_point = SPAposition(mg_value_float(mg_list_at(ctrlpts_mglist, i * 2)), mg_value_float(mg_list_at(ctrlpts_mglist, i * 2 + 1)), 0);
                                        pcur1_ctrl_points[i] = ctrl_point;
                                    }
                                    // create bs2_curve
                                    bs2_curve pcur1_bs2_cur_created = bs2_curve_from_ctrlpts(pcur1_degree, 0, pcur1_closed, pcur1_periodic, pcur1_ctrl_points_num, pcur1_ctrl_points, nullptr, SPAresabs.value(), pcur1_knots.size(),
                                        std::vector<double>(pcur1_knots.begin(), pcur1_knots.end()).data(), SPAresnor.value());
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
                                    const mg_list* knots_simplifier_mglist = mg_value_list(mg_map_at(node_properties, "P"));
                                    const mg_list* ctrlpts_mglist = mg_value_list(mg_map_at(node_properties, "Q"));

                                    std::deque<double> pcur2_knots;
                                    for (int i = 0; i < pcur2_knots_num; i++) {
                                        double knot = mg_value_float(mg_list_at(knots_simplifier_mglist, i * 2));
                                        int knot_cnt = mg_value_float(mg_list_at(knots_simplifier_mglist, i * 2 + 1));
                                        for (int j = 0; j < knot_cnt; j++) pcur2_knots.push_back(knot);
                                    }
                                    pcur2_knots.push_front(pcur2_knots.front());
                                    pcur2_knots.push_back(pcur2_knots.back());

                                    // control points
                                    int pcur2_ctrl_points_num = pcur2_knots.size() - pcur2_degree - 1;
                                    SPAposition* pcur2_ctrl_points = ACIS_NEW SPAposition[pcur2_ctrl_points_num];
                                    for (int i = 0; i < pcur2_ctrl_points_num; i++) {
                                        SPAposition ctrl_point = SPAposition(mg_value_float(mg_list_at(ctrlpts_mglist, i * 2)), mg_value_float(mg_list_at(ctrlpts_mglist, i * 2 + 1)), 0);
                                        pcur2_ctrl_points[i] = ctrl_point;
                                    }
                                    // create bs2_curve
                                    bs2_curve pcur2_bs2_cur_created = bs2_curve_from_ctrlpts(pcur2_degree, 0, pcur2_closed, pcur2_periodic, pcur2_ctrl_points_num, pcur2_ctrl_points, nullptr, SPAresabs.value(), pcur2_knots.size(),
                                        std::vector<double>(pcur2_knots.begin(), pcur2_knots.end()).data(), SPAresnor.value());
                                    ACIS_DELETE[] pcur2_ctrl_points;
                                    pcur2 = pcur2_bs2_cur_created;
                                }

                                int_cur* Int_cur = nullptr;
                                if (cur_subtype == 25) { // parcur
                                    logical surface_tag;
                                    if (cur_surface_tag == 0) {
                                        myerror("int_cur子类类型为parcur，但surface_tag为空。");
                                    } else {
                                        if (cur_surface_tag == 1) {
                                            surface_tag = TRUE;
                                        } else if (cur_surface_tag == 2) {
                                            surface_tag = FALSE;
                                        } else {
                                            myerror(std::format("int_cur子类类型为parcur，但surface_tag不是surf1或surf2，而是：{}", cur_surface_tag));
                                        }
                                    }
                                    Int_cur = ACIS_NEW par_int_cur(bs_cur, cur_fitol, *surf1, *surf2, pcur1, pcur2, surface_tag);
                                } else if (cur_subtype == 1) { // exactcur
                                    Int_cur = ACIS_NEW exact_int_cur(bs_cur, *surf1, *surf2, pcur1, pcur2);
                                    Int_cur->gme_set_fitol_data(cur_fitol);   // fitol
                                } else if (cur_subtype == 31) {
                                    Int_cur = ACIS_NEW int_int_cur(NULL, bs_cur, cur_fitol, *surf1, *surf2, pcur1, pcur2);
                                } else {
                                    myerror(std::format("不支持的int_cur子类类型：{}", cur_subtype));
                                }
                                ACIS_DELETE surf1;
                                ACIS_DELETE surf2;
                                Int_cur->gme_set_safe_range(safe_range);  // range
                                id2ptr[node_id] = Int_cur;
                            } break;
                            case AccessUtils::Restore::Neo4jNode::plane_surface: {
                                SPAposition root_point = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "c")), 3);
                                SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "d")));
                                SPAvector u_deriv = AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "e")));
                                plane* def = ACIS_NEW plane(root_point, normal, u_deriv);
                                def->reverse_v = mg_value_integer(mg_map_at(node_properties, "f"));
                                def->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "b"))));
                                PLANE* plane_surface = nullptr;
                                API_BEGIN;
                                plane_surface = ACIS_NEW PLANE(*def);
                                API_END;
                                ACIS_DELETE def;
                                id2ptr[node_id] = plane_surface;
                            } break;
                            case AccessUtils::Restore::Neo4jNode::sphere_surface: {
                                SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "c")), 3);
                                double radius = mg_value_float(mg_map_at(node_properties, "d"));
                                sphere* def = ACIS_NEW sphere(centre, radius);
                                def->uv_oridir = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "e")));
                                def->pole_dir = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "f")));
                                def->reverse_v = mg_value_integer(mg_map_at(node_properties, "g"));
                                def->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "b"))));
                                SPHERE* sphere_surface = nullptr;
                                API_BEGIN;
                                sphere_surface = ACIS_NEW SPHERE(*def);
                                API_END;
                                ACIS_DELETE def;
                                id2ptr[node_id] = sphere_surface;
                            } break;
                            case AccessUtils::Restore::Neo4jNode::torus_surface: {
                                SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "c")), 3);
                                SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "d")));
                                double major_radius = mg_value_float(mg_map_at(node_properties, "e"));
                                double minor_radius = mg_value_float(mg_map_at(node_properties, "f"));
                                torus* def = ACIS_NEW torus(centre, normal, major_radius, minor_radius);
                                def->uv_oridir = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "g")));
                                def->reverse_v = mg_value_integer(mg_map_at(node_properties, "h"));
                                def->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "b"))));
                                TORUS* torus_surface = nullptr;
                                API_BEGIN;
                                torus_surface = ACIS_NEW TORUS(*def);
                                API_END;
                                ACIS_DELETE def;
                                id2ptr[node_id] = torus_surface;
                            } break;
                            case AccessUtils::Restore::Neo4jNode::cone_surface: {
                                SPAposition base_centre = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "c")), 3);
                                SPAunit_vector base_normal = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "d")));
                                SPAvector base_major_axis = AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "e")));
                                double base_radius_ratio = mg_value_float(mg_map_at(node_properties, "f"));
                                ellipse* gem_base = ACIS_NEW ellipse(base_centre, base_normal, base_major_axis, base_radius_ratio);
                                gem_base->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPAinterval(mg_value_list(mg_map_at(node_properties, "g"))));
                                double sine_angle = mg_value_float(mg_map_at(node_properties, "h"));
                                double cosine_angle = mg_value_float(mg_map_at(node_properties, "i"));
                                cone* def = ACIS_NEW cone(*gem_base, sine_angle, cosine_angle);
                                ACIS_DELETE gem_base;
                                def->reverse_u = mg_value_integer(mg_map_at(node_properties, "j"));
                                def->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "b"))));
                                CONE* cone_surface = nullptr;
                                API_BEGIN;
                                cone_surface = ACIS_NEW CONE(*def);
                                API_END;
                                ACIS_DELETE def;
                                id2ptr[node_id] = cone_surface;
                            } break;
                            case AccessUtils::Restore::Neo4jNode::spline_surface: {
                                spline* def = ACIS_NEW spline();
                                def->gme_set_rev(mg_value_integer(mg_map_at(node_properties, "c")));
                                def->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "b"))));
                                SPLINE* spline_surface = nullptr;
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
                                const mg_list* knots_simplifier_u_mglist = mg_value_list(mg_map_at(node_properties, "l"));
                                const mg_list* knots_simplifier_v_mglist = mg_value_list(mg_map_at(node_properties, "m"));
                                const mg_list* ctrlpts_mglist = mg_value_list(mg_map_at(node_properties, "n"));
                                double fitol = mg_value_float(mg_map_at(node_properties, "o"));

                                std::deque<double> knots_u, knots_v;
                                for (uint32_t i = 0; i < knots_u_num; i++) {
                                    double knot = mg_value_float(mg_list_at(knots_simplifier_u_mglist, i * 2));
                                    int knot_cnt = mg_value_float(mg_list_at(knots_simplifier_u_mglist, i * 2 + 1));
                                    for (int j = 0; j < knot_cnt; j++) knots_u.push_back(knot);
                                }
                                for (int i = 0; i < knots_v_num; i++) {
                                    double knot = mg_value_float(mg_list_at(knots_simplifier_v_mglist, i * 2));
                                    int knot_cnt = mg_value_float(mg_list_at(knots_simplifier_v_mglist, i * 2 + 1));
                                    for (int j = 0; j < knot_cnt; j++) knots_v.push_back(knot);
                                }
                                knots_u.push_front(knots_u.front());
                                knots_u.push_back(knots_u.back());
                                knots_v.push_front(knots_v.front());
                                knots_v.push_back(knots_v.back());

                                // control points
                                int ctrl_points_num_u = knots_u.size() - degree_u - 1;
                                int ctrl_points_num_v = knots_v.size() - degree_v - 1;
                                SPAposition* ctrl_points = ACIS_NEW SPAposition[ctrl_points_num_u * ctrl_points_num_v];
                                double* weights = nullptr;
                                if (rational_u || rational_v) {
                                    weights = new double[ctrl_points_num_u * ctrl_points_num_v];
                                }
                                int ctrl_points_idx = 0;
                                for (int i = 0; i < ctrl_points_num_v; i++) {
                                    for (int j = 0; j < ctrl_points_num_u; j++) {
                                        SPAposition ctrl_point = SPAposition(mg_value_float(mg_list_at(ctrlpts_mglist, ctrl_points_idx)), mg_value_float(mg_list_at(ctrlpts_mglist, ctrl_points_idx + 1)), mg_value_float(mg_list_at(ctrlpts_mglist, ctrl_points_idx + 2)));
                                        ctrl_points[j * ctrl_points_num_v + i] = ctrl_point;
                                        ctrl_points_idx += 3;
                                        if (rational_u || rational_v) {
                                            double weight = mg_value_float(mg_list_at(ctrlpts_mglist, ctrl_points_idx));
                                            ctrl_points_idx++;
                                            weights[j * ctrl_points_num_v + i] = weight;
                                        }
                                    }
                                }

                                bs3_surface bs_sur = bs3_surface_from_ctrlpts(degree_u, rational_u, closed_u, u_singularity, ctrl_points_num_u, degree_v, rational_v, closed_v, v_singularity, ctrl_points_num_v, ctrl_points, weights, SPAresabs.value(), knots_u.size(),
                                    std::vector<double>(knots_u.begin(), knots_u.end()).data(), knots_v.size(), std::vector<double>(knots_v.begin(), knots_v.end()).data(), SPAresnor.value());
                                ACIS_DELETE[] ctrl_points;
                                if (rational_u || rational_v) {
                                    delete[] weights;
                                }
                                spl_sur* spl_sur = ACIS_NEW exact_spl_sur(bs_sur);
                                spl_sur->gme_set_fitol_data(fitol);
                                id2ptr[node_id] = spl_sur;
                            } break;
                            case AccessUtils::Restore::Neo4jNode::pcurve: {
                                int def_type = mg_value_integer(mg_map_at(node_properties, "b"));
                                if (def_type != 0) {
                                    double du = mg_value_float(mg_map_at(node_properties, "c"));
                                    double dv = mg_value_float(mg_map_at(node_properties, "d"));
                                    PCURVE* pcurve = nullptr;
                                    API_BEGIN;
                                    pcurve = ACIS_NEW PCURVE(nullptr, def_type, 0, SPApar_vec(du, dv));
                                    API_END;
                                    id2ptr[node_id] = pcurve;
                                } else {
                                    pcurve* def = ACIS_NEW pcurve();
                                    def->gme_set_rev(mg_value_integer(mg_map_at(node_properties, "e")));
                                    double du = mg_value_float(mg_map_at(node_properties, "c"));
                                    double dv = mg_value_float(mg_map_at(node_properties, "d"));
                                    def->gme_set_off(SPApar_vec(du, dv));
                                    PCURVE* pcurve = nullptr;
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
                                    if (closed_periodic == 0) { //open
                                        closed = 0;
                                        periodic = 0;
                                    } else if (closed_periodic == 1) { //closed
                                        closed = 1;
                                        periodic = 0;
                                    } else if (closed_periodic == 2) { //periodic
                                        closed = 1;
                                        periodic = 1;
                                    }
                                    int knots_num = mg_value_integer(mg_map_at(node_properties, "h"));
                                    const mg_list* knots_simplifier_mglist = mg_value_list(mg_map_at(node_properties, "i"));
                                    const mg_list* ctrlpts_mglist = mg_value_list(mg_map_at(node_properties, "j"));
                                    double fitol = mg_value_float(mg_map_at(node_properties, "d"));

                                    std::deque<double> knots;
                                    for (int i = 0; i < knots_num; i++) {
                                        double knot = mg_value_float(mg_list_at(knots_simplifier_mglist, i * 2));
                                        int knot_cnt = mg_value_float(mg_list_at(knots_simplifier_mglist, i * 2 + 1));
                                        for (int j = 0; j < knot_cnt; j++) knots.push_back(knot);
                                    }
                                    knots.push_front(knots.front());
                                    knots.push_back(knots.back());

                                    // control points
                                    int ctrl_points_num = knots.size() - degree - 1;
                                    SPAposition* ctrl_points = ACIS_NEW SPAposition[ctrl_points_num];
                                    for (int i = 0; i < ctrl_points_num; i++) {
                                        SPAposition ctrl_point = SPAposition(mg_value_float(mg_list_at(ctrlpts_mglist, i * 2)), mg_value_float(mg_list_at(ctrlpts_mglist, i * 2 + 1)), 0);
                                        ctrl_points[i] = ctrl_point;
                                    }
                                    bs2_curve bs_cur = bs2_curve_from_ctrlpts(degree, 0, closed, periodic, ctrl_points_num, ctrl_points, nullptr, DBL_MIN, knots.size(), std::vector<double>(knots.begin(), knots.end()).data(), DBL_MIN);
                                    ACIS_DELETE[] ctrl_points;

                                    int surf_type = mg_value_integer(mg_map_at(node_properties, "e"));

                                    // surf
                                    surface* surf = nullptr;
                                    if (surf_type == 2) {
                                        SPAposition root_point = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "l")), 3);
                                        SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "m")));
                                        SPAvector u_deriv = AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "n")));
                                        plane* ret_plane = ACIS_NEW plane(root_point, normal, u_deriv);
                                        ret_plane->reverse_v = mg_value_integer(mg_map_at(node_properties, "o"));
                                        ret_plane->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "k"))));
                                        surf = ret_plane;
                                    } else if (surf_type == 3) {
                                        SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "l")), 3);
                                        double radius = mg_value_float(mg_map_at(node_properties, "m"));
                                        sphere* ret_sphere = ACIS_NEW sphere(centre, radius);
                                        ret_sphere->uv_oridir = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "n")));
                                        ret_sphere->pole_dir = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "o")));
                                        ret_sphere->reverse_v = mg_value_integer(mg_map_at(node_properties, "p"));
                                        ret_sphere->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "k"))));
                                        surf = ret_sphere;
                                    } else if (surf_type == 4) {
                                        SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "l")), 3);
                                        SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "m")));
                                        double major_radius = mg_value_float(mg_map_at(node_properties, "n"));
                                        double minor_radius = mg_value_float(mg_map_at(node_properties, "o"));
                                        torus* ret_torus = ACIS_NEW torus(centre, normal, major_radius, minor_radius);
                                        ret_torus->uv_oridir = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "p")));
                                        ret_torus->reverse_v = mg_value_integer(mg_map_at(node_properties, "q"));
                                        ret_torus->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "k"))));
                                        surf = ret_torus;
                                    } else if (surf_type == 5) {
                                        SPAposition base_centre = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "l")), 3);
                                        SPAunit_vector base_normal = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "m")));
                                        SPAvector base_major_axis = AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "n")));
                                        double base_radius_ratio = mg_value_float(mg_map_at(node_properties, "o"));
                                        ellipse* gem_base = ACIS_NEW ellipse(base_centre, base_normal, base_major_axis, base_radius_ratio);
                                        gem_base->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPAinterval(mg_value_list(mg_map_at(node_properties, "p"))));
                                        double sine_angle = mg_value_float(mg_map_at(node_properties, "q"));
                                        double cosine_angle = mg_value_float(mg_map_at(node_properties, "r"));
                                        cone* ret_cone = ACIS_NEW cone(*gem_base, sine_angle, cosine_angle);
                                        ACIS_DELETE gem_base;
                                        ret_cone->reverse_u = mg_value_integer(mg_map_at(node_properties, "s"));
                                        ret_cone->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "k"))));
                                        surf = ret_cone;
                                    } else if (surf_type == 1) {
                                        spline* ret_spline = ACIS_NEW spline();
                                        ret_spline->gme_set_rev(mg_value_integer(mg_map_at(node_properties, "l")));
                                        ret_spline->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "k"))));
                                        surf = ret_spline;
                                    }
                                    par_cur* par_cur = exp_par_cur::gme_exp_par_cur_public_constructor(bs_cur, fitol, -1.0, *surf);
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
                    const mg_value* rellist_value = mg_list_at(noderellist, 1);
                    assert(mg_value_get_type(rellist_value) == MG_VALUE_TYPE_LIST);
                    const mg_list* rellist = mg_value_list(rellist_value);
                    const uint32_t rellist_size = mg_list_size(rellist);
                    for (uint32_t i = 0; i < rellist_size; i++) {
                        const mg_value* rel_value = mg_list_at(rellist, i);
                        assert(mg_value_get_type(rel_value) == MG_VALUE_TYPE_RELATIONSHIP);
                        const mg_relationship* rel = mg_value_relationship(rel_value);
                        int64_t rel_startnode_id = mg_relationship_start_id(rel);
                        int64_t rel_endnode_id = mg_relationship_end_id(rel);
                        const mg_map* rel_properties = mg_relationship_properties(rel);
                        const mg_string* rel_typename_mgs = mg_value_string(mg_map_at(rel_properties, "a"));
                        std::string rel_typename(mg_string_data(rel_typename_mgs), mg_string_size(rel_typename_mgs));
                        if (rel_typename == "part_entity_ptr") { continue; }
                        switch (AccessUtils::Restore::Neo4jEdge_str2enum.at(rel_typename)) {
                            case AccessUtils::Restore::Neo4jEdge::body_lump_ptr: {
                                BODY* body = (BODY*)id2ptr.at(rel_startnode_id);
                                LUMP* lump = (LUMP*)id2ptr.at(rel_endnode_id);
                                body->set_lump(lump);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::body_wire_ptr: {
                                BODY* body = (BODY*)id2ptr.at(rel_startnode_id);
                                WIRE* wire = (WIRE*)id2ptr.at(rel_endnode_id);
                                body->set_wire(wire);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::body_transform_ptr: {
                                BODY* body = (BODY*)id2ptr.at(rel_startnode_id);
                                TRANSFORM* transform = (TRANSFORM*)id2ptr.at(rel_endnode_id);
                                body->set_transform(transform);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::lump_next_ptr: {
                                LUMP* lump = (LUMP*)id2ptr.at(rel_startnode_id);
                                LUMP* next = (LUMP*)id2ptr.at(rel_endnode_id);
                                lump->set_next(next);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::lump_shell_ptr: {
                                LUMP* lump = (LUMP*)id2ptr.at(rel_startnode_id);
                                SHELL* shell = (SHELL*)id2ptr.at(rel_endnode_id);
                                lump->set_shell(shell);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::lump_body_ptr: {
                                LUMP* lump = (LUMP*)id2ptr.at(rel_startnode_id);
                                BODY* body = (BODY*)id2ptr.at(rel_endnode_id);
                                lump->set_body(body);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::shell_next_ptr: {
                                SHELL* shell = (SHELL*)id2ptr.at(rel_startnode_id);
                                SHELL* next = (SHELL*)id2ptr.at(rel_endnode_id);
                                shell->set_next(next);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::shell_subshell_ptr: {
                                SHELL* shell = (SHELL*)id2ptr.at(rel_startnode_id);
                                SUBSHELL* subshell = (SUBSHELL*)id2ptr.at(rel_endnode_id);
                                shell->set_subshell(subshell);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::shell_face_ptr: {
                                SHELL* shell = (SHELL*)id2ptr.at(rel_startnode_id);
                                FACE* face = (FACE*)id2ptr.at(rel_endnode_id);
                                shell->set_face(face);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::shell_wire_ptr: {
                                SHELL* shell = (SHELL*)id2ptr.at(rel_startnode_id);
                                WIRE* wire = (WIRE*)id2ptr.at(rel_endnode_id);
                                shell->set_wire(wire);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::shell_lump_ptr: {
                                SHELL* shell = (SHELL*)id2ptr.at(rel_startnode_id);
                                LUMP* lump = (LUMP*)id2ptr.at(rel_endnode_id);
                                shell->set_lump(lump);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::subshell_parent_ptr: {
                                SUBSHELL* subshell = (SUBSHELL*)id2ptr.at(rel_startnode_id);
                                SUBSHELL* parent = (SUBSHELL*)id2ptr.at(rel_endnode_id);
                                subshell->set_parent(parent);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::subshell_sibling_ptr: {
                                SUBSHELL* subshell = (SUBSHELL*)id2ptr.at(rel_startnode_id);
                                SUBSHELL* sibling = (SUBSHELL*)id2ptr.at(rel_endnode_id);
                                subshell->set_sibling(sibling);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::subshell_child_ptr: {
                                SUBSHELL* subshell = (SUBSHELL*)id2ptr.at(rel_startnode_id);
                                SUBSHELL* child = (SUBSHELL*)id2ptr.at(rel_endnode_id);
                                subshell->set_child(child);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::subshell_face_ptr: {
                                SUBSHELL* subshell = (SUBSHELL*)id2ptr.at(rel_startnode_id);
                                FACE* face = (FACE*)id2ptr.at(rel_endnode_id);
                                subshell->set_face(face);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::subshell_wire_ptr: {
                                SUBSHELL* subshell = (SUBSHELL*)id2ptr.at(rel_startnode_id);
                                WIRE* wire = (WIRE*)id2ptr.at(rel_endnode_id);
                                subshell->set_wire(wire);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::wire_next_ptr: {
                                WIRE* wire = (WIRE*)id2ptr.at(rel_startnode_id);
                                WIRE* next = (WIRE*)id2ptr.at(rel_endnode_id);
                                wire->set_next(next);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::wire_coedge_ptr: {
                                WIRE* wire = (WIRE*)id2ptr.at(rel_startnode_id);
                                COEDGE* coedge = (COEDGE*)id2ptr.at(rel_endnode_id);
                                wire->set_coedge(coedge);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::wire_owner_ptr: {
                                WIRE* wire = (WIRE*)id2ptr.at(rel_startnode_id);
                                ENTITY* owner = (ENTITY*)id2ptr.at(rel_endnode_id);
                                wire->set_owner(owner);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::wire_subshell_ptr: {
                                WIRE* wire = (WIRE*)id2ptr.at(rel_startnode_id);
                                SUBSHELL* subshell = (SUBSHELL*)id2ptr.at(rel_endnode_id);
                                wire->set_subshell(subshell);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::face_next_ptr: {
                                FACE* face = (FACE*)id2ptr.at(rel_startnode_id);
                                FACE* next = (FACE*)id2ptr.at(rel_endnode_id);
                                face->set_next(next);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::face_loop_ptr: {
                                FACE* face = (FACE*)id2ptr.at(rel_startnode_id);
                                LOOP* loop = (LOOP*)id2ptr.at(rel_endnode_id);
                                face->set_loop(loop);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::face_shell_ptr: {
                                FACE* face = (FACE*)id2ptr.at(rel_startnode_id);
                                SHELL* shell = (SHELL*)id2ptr.at(rel_endnode_id);
                                face->set_shell(shell);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::face_subshell_ptr: {
                                FACE* face = (FACE*)id2ptr.at(rel_startnode_id);
                                SUBSHELL* subshell = (SUBSHELL*)id2ptr.at(rel_endnode_id);
                                face->set_subshell(subshell);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::face_geometry_ptr: {
                                FACE* face = (FACE*)id2ptr.at(rel_startnode_id);
                                SURFACE* geometry = (SURFACE*)id2ptr.at(rel_endnode_id);
                                face->set_geometry(geometry);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::loop_next_ptr: {
                                LOOP* loop = (LOOP*)id2ptr.at(rel_startnode_id);
                                LOOP* next = (LOOP*)id2ptr.at(rel_endnode_id);
                                loop->set_next(next);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::loop_start_ptr: {
                                LOOP* loop = (LOOP*)id2ptr.at(rel_startnode_id);
                                COEDGE* start = (COEDGE*)id2ptr.at(rel_endnode_id);
                                loop->set_start(start);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::loop_face_ptr: {
                                LOOP* loop = (LOOP*)id2ptr.at(rel_startnode_id);
                                FACE* face = (FACE*)id2ptr.at(rel_endnode_id);
                                loop->set_face(face);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::coedge_next_ptr: {
                                COEDGE* coedge = (COEDGE*)id2ptr.at(rel_startnode_id);
                                COEDGE* next = (COEDGE*)id2ptr.at(rel_endnode_id);
                                coedge->set_next(next);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::coedge_previous_ptr: {
                                COEDGE* coedge = (COEDGE*)id2ptr.at(rel_startnode_id);
                                COEDGE* previous = (COEDGE*)id2ptr.at(rel_endnode_id);
                                coedge->set_previous(previous);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::coedge_partner_ptr: {
                                COEDGE* coedge = (COEDGE*)id2ptr.at(rel_startnode_id);
                                COEDGE* partner = (COEDGE*)id2ptr.at(rel_endnode_id);
                                coedge->set_partner(partner);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::coedge_edge_ptr: {
                                COEDGE* coedge = (COEDGE*)id2ptr.at(rel_startnode_id);
                                EDGE* edge = (EDGE*)id2ptr.at(rel_endnode_id);
                                coedge->set_edge(edge);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::coedge_owner_ptr: {
                                COEDGE* coedge = (COEDGE*)id2ptr.at(rel_startnode_id);
                                ENTITY* owner = (ENTITY*)id2ptr.at(rel_endnode_id);
                                coedge->set_owner(owner);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::coedge_geometry_ptr: {
                                COEDGE* coedge = (COEDGE*)id2ptr.at(rel_startnode_id);
                                PCURVE* geometry = (PCURVE*)id2ptr.at(rel_endnode_id);
                                coedge->set_geometry(geometry);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::edge_start_ptr: {
                                EDGE* edge = (EDGE*)id2ptr.at(rel_startnode_id);
                                VERTEX* start = (VERTEX*)id2ptr.at(rel_endnode_id);
                                edge->set_start(start);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::edge_end_ptr: {
                                EDGE* edge = (EDGE*)id2ptr.at(rel_startnode_id);
                                VERTEX* end = (VERTEX*)id2ptr.at(rel_endnode_id);
                                edge->set_end(end);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::edge_coedge_ptr: {
                                EDGE* edge = (EDGE*)id2ptr.at(rel_startnode_id);
                                COEDGE* coedge = (COEDGE*)id2ptr.at(rel_endnode_id);
                                edge->set_coedge(coedge);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::edge_geometry_ptr: {
                                EDGE* edge = (EDGE*)id2ptr.at(rel_startnode_id);
                                CURVE* geometry = (CURVE*)id2ptr.at(rel_endnode_id);
                                edge->set_geometry(geometry);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::vertex_edge_ptr: {
                                VERTEX* vertex = (VERTEX*)id2ptr.at(rel_startnode_id);
                                EDGE* edge = (EDGE*)id2ptr.at(rel_endnode_id);
                                vertex->set_edge(edge);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::vertex_geometry_ptr: {
                                VERTEX* vertex = (VERTEX*)id2ptr.at(rel_startnode_id);
                                APOINT* geometry = (APOINT*)id2ptr.at(rel_endnode_id);
                                vertex->set_geometry(geometry);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::pcurve_ref_curve_ptr: {
                                PCURVE* pcurve = (PCURVE*)id2ptr.at(rel_startnode_id);
                                CURVE* ref_curve = (CURVE*)id2ptr.at(rel_endnode_id);
                                pcurve->set_def(ref_curve, pcurve->index(), 0, pcurve->offset());
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::pcurve_fit_ptr: {
                                PCURVE* p = (PCURVE*)id2ptr.at(rel_startnode_id);
                                par_cur* fit = (par_cur*)id2ptr.at(rel_endnode_id);
                                p->set_fit(fit);
                                fit->add_ref();
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::spline_surface_spl_ptr: {
                                SPLINE* spline_surface = (SPLINE*)id2ptr.at(rel_startnode_id);
                                spl_sur* spl = (spl_sur*)id2ptr.at(rel_endnode_id);
                                spline_surface->gme_set_spl(spl);
                                spl->add_ref();
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::intcurve_curve_fit_ptr: {
                                INTCURVE* intcurve_curve = (INTCURVE*)id2ptr.at(rel_startnode_id);
                                int_cur* fit = (int_cur*)id2ptr.at(rel_endnode_id);
                                intcurve_curve->gme_set_fit(fit);
                                fit->add_ref();
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::int_cur_surf1_spl_ptr: {
                                int_cur* ic = (int_cur*)id2ptr.at(rel_startnode_id);
                                spl_sur* surf1_spl = (spl_sur*)id2ptr.at(rel_endnode_id);
                                ic->set_surf1_spl(surf1_spl);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::int_cur_surf2_spl_ptr: {
                                int_cur* ic = (int_cur*)id2ptr.at(rel_startnode_id);
                                spl_sur* surf2_spl = (spl_sur*)id2ptr.at(rel_endnode_id);
                                ic->set_surf2_spl(surf2_spl);
                            } break;
                            case AccessUtils::Restore::Neo4jEdge::par_cur_surf_spl_ptr: {
                                par_cur* pc = (par_cur*)id2ptr.at(rel_startnode_id);
                                spl_sur* surf_spl = (spl_sur*)id2ptr.at(rel_endnode_id);
                                ((exp_par_cur*)pc)->gme_set_surf_spl(surf_spl);
                            } break;
                            default: {
                                myerror("不支持的neo4j边类型。");
                            } break;
                        }
                    }
                }
            } else if (status == 0) {
                const mg_map* mgm = mg_result_summary(result);
                const uint32_t mgm_size = mg_map_size(mgm);
                for (uint32_t i = 0; i < mgm_size; i++) {
                    const mg_string* itemkey = mg_map_key_at(mgm, i);
                    const mg_value* itemvalue = mg_map_value_at(mgm, i);
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
        entity_list.add((ENTITY*)id2ptr.at(s));
    }
}

extern DELTA_STATE* lastsave_ds;
extern std::unordered_map<void*, int64_t> ptr2nodeid;
DELTA_STATE* lastsave_ds = nullptr;
std::unordered_map<void*, int64_t> ptr2nodeid;

void api_save_neo4j(const Neo4jPart& conn) {
    // 1. 确保“部件”节点存在，并获取版本信息
    mg_map* qparams = mg_map_make_empty(1);
    mg_map_append(qparams, mg_string_make("Y"), mg_value_make_string(conn.partname.c_str()));
    conn.execute_bolt("MERGE (n:part {a:'part',b:$Y}) RETURN id(n),n.c,n.d", qparams);
    mg_map_destroy(qparams);

    // ... 解析查询结果，获取 partnodeid, partnodenextgeneration, partnodecurgeneration ...
    int64_t partnodeid, partnodenextgeneration, partnodecurgeneration;
    mg_result* result;
    if (mg_session_fetch(conn.session, &result) == 1) {
        const mg_list* mgl_partnodeid = mg_result_row(result);
        const uint32_t mgl_partnodeid_length = mg_list_size(mgl_partnodeid);
        assert(mgl_partnodeid_length == 3);
        partnodeid = mg_value_integer(mg_list_at(mgl_partnodeid, 0));

        const mg_value* mgv_partnodemaxgeneration = mg_list_at(mgl_partnodeid, 1);
        mg_value_type mgv_partnodemaxgeneration_type = mg_value_get_type(mgv_partnodemaxgeneration);
        if (mgv_partnodemaxgeneration_type == MG_VALUE_TYPE_NULL) {
            partnodenextgeneration = 1;
        } else {
            partnodenextgeneration = mg_value_integer(mgv_partnodemaxgeneration) + 1;
        }

        const mg_value* mgv_partnodecurgeneration = mg_list_at(mgl_partnodeid, 2);
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
        conn.execute_bolt("MATCH (n)-[r:part_generation_ptr {a:'part_generation_ptr',b:$B}]->(m) WHERE id(n)=$A RETURN id(m)", qparams);
        mg_map_destroy(qparams);
        if (mg_session_fetch(conn.session, &result) == 1) {
            const mg_list* mgl_curgenerationnodeid = mg_result_row(result);
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
    DELTA_STATE* thissave_ds; //这次保存
    api_note_state(thissave_ds);
    {   
        // ... 处理 thissave_ds 和 lastsave_ds 为空的初始情况 ...
        HISTORY_STREAM* hs;
        api_get_default_history(hs);
        if (thissave_ds == nullptr) {
            thissave_ds = hs->get_active_ds(); //最新的差状态
        }
        if (lastsave_ds == nullptr) {
            lastsave_ds = hs->get_root_ds(); //最早的差状态
        }
    }

    // 4. 遍历ACIS历史，找出变更的实体
    std::unordered_set<ENTITY*> created_or_updated_entity_list, deleted_entity_list;
    DELTA_STATE* this_ds = thissave_ds;
    while (this_ds != lastsave_ds) {
        BULLETIN_BOARD* bb = this_ds->bb();
        while (bb) {
            BULLETIN* b = bb->start_bulletin();
            while (b) {
                ENTITY* old_ent = b->old_entity_ptr();
                ENTITY* new_ent = b->new_entity_ptr();
                if (new_ent) {
                    created_or_updated_entity_list.insert(new_ent);
                } else if (old_ent) { // new_ent==NULL && old_ent!=NULL
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
                    CURVE* ptr = (CURVE*)(*it);
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
                    SURFACE* ptr = (SURFACE*)(*it);
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
                CURVE* ptr = (CURVE*)(*it);
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
                SURFACE* ptr = (SURFACE*)(*it);
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
    for (const auto& entity_ptr : created_or_updated_entity_list) {
        printf("%s, 0x%x\n", entity_ptr->type_name(), (long)entity_ptr);
    }
    //assert(deleted_entity_list.empty());
    printf("\ndeleted_entity_list : %llu\n", deleted_entity_list.size());
    for (const auto& entity_ptr : deleted_entity_list) {
        printf("%s, 0x%x\n", entity_ptr->type_name(), (long)entity_ptr);
    }
#endif


    lastsave_ds = thissave_ds;

    // 6. 为每个变更的实体创建内存中的Node对象
    std::vector<Node*> node_list;
    // ... 其他辅助列表 ...
    std::vector<Relationship2*> relationship_list;
    std::vector<int64_t> updated_entity_nodeid_list, deleted_entity_nodeid_list;
    std::vector<void*> updated_entity_list;
    std::unordered_map<void*, Node*> ptr2node;

    // 创建一个代表新版本的 'generation' 节点
    Node* generationnode = new Node();
    // ... 设置 generationnode 的标签和属性 ...
    node_list.push_back(generationnode);
    generationnode->labels = mg_list_make_empty(1);
    mg_list_append(generationnode->labels, mg_value_make_string("generation"));
    generationnode->properties['a'] = mg_value_make_string("generation");
    generationnode->properties['b'] = mg_value_make_integer(partnodenextgeneration);

    for (const auto& entity_ptr : created_or_updated_entity_list) {
        switch (entity_ptr->identity(1)) {
            case BODY_ID: {
                BODY* ptr = (BODY*)entity_ptr;
                Node* ptrnode = new Node();
                if (ptr2nodeid.find(ptr) != ptr2nodeid.end()) {
                    updated_entity_nodeid_list.push_back(ptr2nodeid.at(ptr));
                    updated_entity_list.push_back(ptr);
                }
                ptr2node[ptr] = ptrnode;
                node_list.push_back(ptrnode);
                ptrnode->labels = mg_list_make_empty(1);
                mg_list_append(ptrnode->labels, mg_value_make_string("body"));
                ptrnode->properties['a'] = mg_value_make_string("body");
            } break;
            case LUMP_ID: {
                LUMP* ptr = (LUMP*)entity_ptr;
                Node* ptrnode = new Node();
                if (ptr2nodeid.find(ptr) != ptr2nodeid.end()) {
                    updated_entity_nodeid_list.push_back(ptr2nodeid.at(ptr));
                    updated_entity_list.push_back(ptr);
                }
                ptr2node[ptr] = ptrnode;
                node_list.push_back(ptrnode);
                ptrnode->labels = mg_list_make_empty(1);
                mg_list_append(ptrnode->labels, mg_value_make_string("lump"));
                ptrnode->properties['a'] = mg_value_make_string("lump");
            } break;
            case SHELL_ID: {
                SHELL* ptr = (SHELL*)entity_ptr;
                Node* ptrnode = new Node();
                if (ptr2nodeid.find(ptr) != ptr2nodeid.end()) {
                    updated_entity_nodeid_list.push_back(ptr2nodeid.at(ptr));
                    updated_entity_list.push_back(ptr);
                }
                ptr2node[ptr] = ptrnode;
                node_list.push_back(ptrnode);
                ptrnode->labels = mg_list_make_empty(1);
                mg_list_append(ptrnode->labels, mg_value_make_string("shell"));
                ptrnode->properties['a'] = mg_value_make_string("shell");
            } break;
            case SUBSHELL_ID: {
                SUBSHELL* ptr = (SUBSHELL*)entity_ptr;
                Node* ptrnode = new Node();
                if (ptr2nodeid.find(ptr) != ptr2nodeid.end()) {
                    updated_entity_nodeid_list.push_back(ptr2nodeid.at(ptr));
                    updated_entity_list.push_back(ptr);
                }
                ptr2node[ptr] = ptrnode;
                node_list.push_back(ptrnode);
                ptrnode->labels = mg_list_make_empty(1);
                mg_list_append(ptrnode->labels, mg_value_make_string("subshell"));
                ptrnode->properties['a'] = mg_value_make_string("subshell");
            } break;
            case WIRE_ID: {
                WIRE* ptr = (WIRE*)entity_ptr;
                Node* ptrnode = new Node();
                if (ptr2nodeid.find(ptr) != ptr2nodeid.end()) {
                    updated_entity_nodeid_list.push_back(ptr2nodeid.at(ptr));
                    updated_entity_list.push_back(ptr);
                }
                ptr2node[ptr] = ptrnode;
                node_list.push_back(ptrnode);
                ptrnode->labels = mg_list_make_empty(1);
                mg_list_append(ptrnode->labels, mg_value_make_string("wire"));
                ptrnode->properties['a'] = mg_value_make_string("wire");
                ptrnode->properties['b'] = mg_value_make_integer(ptr->cont());
            } break;
            case FACE_ID: {
                FACE* ptr = (FACE*)entity_ptr;
                Node* ptrnode = new Node();
                if (ptr2nodeid.find(ptr) != ptr2nodeid.end()) {
                    updated_entity_nodeid_list.push_back(ptr2nodeid.at(ptr));
                    updated_entity_list.push_back(ptr);
                }
                ptr2node[ptr] = ptrnode;
                node_list.push_back(ptrnode);
                if (ptr->sides()) {
                    ptrnode->labels = mg_list_make_empty(1);
                    mg_list_append(ptrnode->labels, mg_value_make_string("face"));
                    ptrnode->properties['a'] = mg_value_make_string("face");
                    ptrnode->properties['b'] = mg_value_make_integer(ptr->sense());
                    ptrnode->properties['c'] = mg_value_make_integer(1);
                    ptrnode->properties['d'] = mg_value_make_integer(ptr->cont());
                } else {
                    ptrnode->labels = mg_list_make_empty(1);
                    mg_list_append(ptrnode->labels, mg_value_make_string("face"));
                    ptrnode->properties['a'] = mg_value_make_string("face");
                    ptrnode->properties['b'] = mg_value_make_integer(ptr->sense());
                    ptrnode->properties['c'] = mg_value_make_integer(0);
                }
            } break;
            case LOOP_ID: {
                LOOP* ptr = (LOOP*)entity_ptr;
                Node* ptrnode = new Node();
                if (ptr2nodeid.find(ptr) != ptr2nodeid.end()) {
                    updated_entity_nodeid_list.push_back(ptr2nodeid.at(ptr));
                    updated_entity_list.push_back(ptr);
                }
                ptr2node[ptr] = ptrnode;
                node_list.push_back(ptrnode);
                ptrnode->labels = mg_list_make_empty(1);
                mg_list_append(ptrnode->labels, mg_value_make_string("loop"));
                ptrnode->properties['a'] = mg_value_make_string("loop");
            } break;
            case COEDGE_ID: {
                COEDGE* ptr = (COEDGE*)entity_ptr;
                Node* ptrnode = new Node();
                if (ptr2nodeid.find(ptr) != ptr2nodeid.end()) {
                    updated_entity_nodeid_list.push_back(ptr2nodeid.at(ptr));
                    updated_entity_list.push_back(ptr);
                }
                ptr2node[ptr] = ptrnode;
                node_list.push_back(ptrnode);
                ptrnode->labels = mg_list_make_empty(1);
                mg_list_append(ptrnode->labels, mg_value_make_string("coedge"));
                ptrnode->properties['a'] = mg_value_make_string("coedge");
                ptrnode->properties['b'] = mg_value_make_integer(ptr->sense());
            } break;
            case EDGE_ID: {
                EDGE* ptr = (EDGE*)entity_ptr;
                Node* ptrnode = new Node();
                if (ptr2nodeid.find(ptr) != ptr2nodeid.end()) {
                    updated_entity_nodeid_list.push_back(ptr2nodeid.at(ptr));
                    updated_entity_list.push_back(ptr);
                }
                ptr2node[ptr] = ptrnode;
                node_list.push_back(ptrnode);
                ptrnode->labels = mg_list_make_empty(1);
                mg_list_append(ptrnode->labels, mg_value_make_string("edge"));
                ptrnode->properties['a'] = mg_value_make_string("edge");
                ptrnode->properties['b'] = mg_value_make_integer(ptr->sense());
            } break;
            case VERTEX_ID: {
                VERTEX* ptr = (VERTEX*)entity_ptr;
                Node* ptrnode = new Node();
                if (ptr2nodeid.find(ptr) != ptr2nodeid.end()) {
                    updated_entity_nodeid_list.push_back(ptr2nodeid.at(ptr));
                    updated_entity_list.push_back(ptr);
                }
                ptr2node[ptr] = ptrnode;
                node_list.push_back(ptrnode);
                ptrnode->labels = mg_list_make_empty(1);
                mg_list_append(ptrnode->labels, mg_value_make_string("vertex"));
                ptrnode->properties['a'] = mg_value_make_string("vertex");
            } break;
            case APOINT_ID: {
                APOINT* ptr = (APOINT*)entity_ptr;
                SPAposition pos = ((APOINT*)ptr)->coords();
                Node* ptrnode = new Node();
                if (ptr2nodeid.find(ptr) != ptr2nodeid.end()) {
                    updated_entity_nodeid_list.push_back(ptr2nodeid.at(ptr));
                    updated_entity_list.push_back(ptr);
                }
                ptr2node[ptr] = ptrnode;
                node_list.push_back(ptrnode);
                ptrnode->labels = mg_list_make_empty(1);
                mg_list_append(ptrnode->labels, mg_value_make_string("point"));
                ptrnode->properties['a'] = mg_value_make_string("point");
                ptrnode->properties['b'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAposition(pos, 3));
            } break;
            case CURVE_ID: {
                CURVE* ptr = (CURVE*)entity_ptr;
                switch (ptr->identity(2)) {
                    case STRAIGHT_ID: {
                        STRAIGHT* ptr = (STRAIGHT*)entity_ptr;
                        straight gem = ((STRAIGHT*)ptr)->gme_get_def();
                        SPAposition root_point = gem.root_point;
                        SPAunit_vector direction = gem.direction;
                        direction.set_x(direction.x() * gem.param_scale);
                        direction.set_y(direction.y() * gem.param_scale);
                        direction.set_z(direction.z() * gem.param_scale);
                        SPAinterval range = gem.gme_get_subset_range();
                        Node* ptrnode = new Node();
                        if (ptr2nodeid.find(ptr) != ptr2nodeid.end()) {
                            updated_entity_nodeid_list.push_back(ptr2nodeid.at(ptr));
                            updated_entity_list.push_back(ptr);
                        }
                        ptr2node[ptr] = ptrnode;
                        node_list.push_back(ptrnode);
                        ptrnode->labels = mg_list_make_empty(1);
                        mg_list_append(ptrnode->labels, mg_value_make_string("straight-curve"));
                        ptrnode->properties['a'] = mg_value_make_string("straight-curve");
                        ptrnode->properties['b'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAinterval(range));
                        ptrnode->properties['c'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAposition(root_point, 3));
                        ptrnode->properties['d'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAvector(direction));
                    } break;
                    case ELLIPSE_ID: {
                        ELLIPSE* ptr = (ELLIPSE*)entity_ptr;
                        ellipse gem = ((ELLIPSE*)ptr)->gme_get_def();
                        SPAposition centre = gem.centre;
                        SPAunit_vector normal = gem.normal;
                        SPAvector major_axis = gem.major_axis;
                        SPAinterval range = gem.gme_get_subset_range();
                        Node* ptrnode = new Node();
                        if (ptr2nodeid.find(ptr) != ptr2nodeid.end()) {
                            updated_entity_nodeid_list.push_back(ptr2nodeid.at(ptr));
                            updated_entity_list.push_back(ptr);
                        }
                        ptr2node[ptr] = ptrnode;
                        node_list.push_back(ptrnode);
                        ptrnode->labels = mg_list_make_empty(1);
                        mg_list_append(ptrnode->labels, mg_value_make_string("ellipse-curve"));
                        ptrnode->properties['a'] = mg_value_make_string("ellipse-curve");
                        ptrnode->properties['b'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAinterval(range));
                        ptrnode->properties['c'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAposition(centre, 3));
                        ptrnode->properties['d'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAunit_vector(normal));
                        ptrnode->properties['e'] = mg_value_make_list(AccessUtils::Save::getmglist_SPAvector(major_axis));
                        ptrnode->properties['f'] = mg_value_make_float(gem.radius_ratio);
                    } break;
                    case HELIX_ID: {
                        HELIX* ptr = (HELIX*)entity_ptr;
                        helix gem = ((HELIX*)ptr)->gme_get_def();
                        SPAposition axis_root = gem.axis_root();
                        SPAunit_vector axis_dir = gem.axis_dir();
                        SPAvector start_disp = gem.start_disp();
                        SPAinterval helix_range = gem.helix_range();
                        SPAinterval range = gem.gme_get_subset_range();
                        Node* ptrnode = new Node();
                        if (ptr2nodeid.find(ptr) != ptr2nodeid.end()) {
                            updated_entity_nodeid_list.push_back(ptr2nodeid.at(ptr));
                            updated_entity_list.push_back(ptr);
                        }
                        ptr2node[ptr] = ptrnode;
                        node_list.push_back(ptrnode);
                        ptrnode->labels = mg_list_make_empty(1);
                        mg_list_append(ptrnode->labels, mg_value_make_string("helix-curve"));
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
                SURFACE* ptr = (SURFACE*)entity_ptr;
                switch (ptr->identity(2)) {
                    case PLANE_ID: {
                        PLANE* ptr = (PLANE*)entity_ptr;
                        plane gem = ((PLANE*)ptr)->gme_get_def();
                        AccessUtils::Save::plane_data* gem_data = AccessUtils::Save::get_plane_data(&gem);
                        Node* ptrnode = new Node();
                        if (ptr2nodeid.find(ptr) != ptr2nodeid.end()) {
                            updated_entity_nodeid_list.push_back(ptr2nodeid.at(ptr));
                            updated_entity_list.push_back(ptr);
                        }
                        ptr2node[ptr] = ptrnode;
                        node_list.push_back(ptrnode);
                        ptrnode->labels = mg_list_make_empty(1);
                        mg_list_append(ptrnode->labels, mg_value_make_string("plane-surface"));
                        ptrnode->properties['a'] = mg_value_make_string("plane-surface");
                        ptrnode->properties['b'] = mg_value_make_list(gem_data->subset_range);
                        ptrnode->properties['c'] = mg_value_make_list(gem_data->root_point);
                        ptrnode->properties['d'] = mg_value_make_list(gem_data->normal);
                        ptrnode->properties['e'] = mg_value_make_list(gem_data->u_deriv);
                        ptrnode->properties['f'] = mg_value_make_integer(gem_data->reverse_v);
                        delete gem_data;
                    } break;
                    case SPHERE_ID: {
                        SPHERE* ptr = (SPHERE*)entity_ptr;
                        sphere gem = ((SPHERE*)ptr)->gme_get_def();
                        AccessUtils::Save::sphere_data* gem_data = AccessUtils::Save::get_sphere_data(&gem);
                        Node* ptrnode = new Node();
                        if (ptr2nodeid.find(ptr) != ptr2nodeid.end()) {
                            updated_entity_nodeid_list.push_back(ptr2nodeid.at(ptr));
                            updated_entity_list.push_back(ptr);
                        }
                        ptr2node[ptr] = ptrnode;
                        node_list.push_back(ptrnode);
                        ptrnode->labels = mg_list_make_empty(1);
                        mg_list_append(ptrnode->labels, mg_value_make_string("sphere-surface"));
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
                        TORUS* ptr = (TORUS*)entity_ptr;
                        torus gem = ((TORUS*)ptr)->gme_get_def();
                        AccessUtils::Save::torus_data* gem_data = AccessUtils::Save::get_torus_data(&gem);
                        Node* ptrnode = new Node();
                        if (ptr2nodeid.find(ptr) != ptr2nodeid.end()) {
                            updated_entity_nodeid_list.push_back(ptr2nodeid.at(ptr));
                            updated_entity_list.push_back(ptr);
                        }
                        ptr2node[ptr] = ptrnode;
                        node_list.push_back(ptrnode);
                        ptrnode->labels = mg_list_make_empty(1);
                        mg_list_append(ptrnode->labels, mg_value_make_string("torus-surface"));
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
                        CONE* ptr = (CONE*)entity_ptr;
                        cone gem = ((CONE*)ptr)->gme_get_def();
                        AccessUtils::Save::cone_data* gem_data = AccessUtils::Save::get_cone_data(&gem);
                        Node* ptrnode = new Node();
                        if (ptr2nodeid.find(ptr) != ptr2nodeid.end()) {
                            updated_entity_nodeid_list.push_back(ptr2nodeid.at(ptr));
                            updated_entity_list.push_back(ptr);
                        }
                        ptr2node[ptr] = ptrnode;
                        node_list.push_back(ptrnode);
                        ptrnode->labels = mg_list_make_empty(1);
                        mg_list_append(ptrnode->labels, mg_value_make_string("cone-surface"));
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
                TRANSFORM* ptr = (TRANSFORM*)entity_ptr;
                SPAtransf transf = ((TRANSFORM*)ptr)->transform();
                SPAmatrix affine = transf.affine();
                SPAvector translation = transf.translation();
                Node* ptrnode = new Node();
                if (ptr2nodeid.find(ptr) != ptr2nodeid.end()) {
                    updated_entity_nodeid_list.push_back(ptr2nodeid.at(ptr));
                    updated_entity_list.push_back(ptr);
                }
                ptr2node[ptr] = ptrnode;
                node_list.push_back(ptrnode);
                ptrnode->labels = mg_list_make_empty(1);
                mg_list_append(ptrnode->labels, mg_value_make_string("transform"));
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
        mg_list* mgl_node_list = mg_list_make_empty(node_list_size);
        int nodeidx = 0;
        for (auto node : node_list) {
            mg_map* mgm_node = mg_map_make_empty(2 + node->properties.size());
            mg_map_append(mgm_node, mg_string_make("W"), mg_value_make_integer(nodeidx));
            mg_map_append(mgm_node, mg_string_make("X"), mg_value_make_list(node->labels));
            for (auto [propkey, propval] : node->properties) {
                const char propkey_str[2] = { propkey, 0 };
                mg_map_append(mgm_node, mg_string_make(propkey_str), propval);
            }
            mg_list_append(mgl_node_list, mg_value_make_map(mgm_node));
            nodeidx++;
        }
        mg_map* qparams = mg_map_make_empty(1);
        mg_map_append(qparams, mg_string_make("Y"), mg_value_make_list(mgl_node_list));
        // ... 将 node_list 中的所有 Node 对象打包成 mg_list ..
        conn.execute_bolt("UNWIND $Y AS Z "
            "CALL apoc.create.node(Z.X,{a:Z.a,b:Z.b,c:Z.c,d:Z.d,e:Z.e,f:Z.f,g:Z.g,h:Z.h,i:Z.i,j:Z.j,k:Z.k,l:Z.l,m:Z.m,n:Z.n,o:Z.o,p:Z.p,q:Z.q,r:Z.r,s:Z.s,t:Z.t,u:Z.u,v:Z.v,w:Z.w,x:Z.x,y:Z.y,z:Z.z,A:Z.A,B:Z.B,C:Z.C,D:Z.D,E:Z.E,F:Z.F,G:Z.G,H:Z.H,I:Z.I,J:Z.J,K:Z.K,L:Z.L,M:Z.M,N:Z.N,O:Z.O,P:Z.P,Q:Z.Q}) "
            "YIELD node RETURN id(node) ORDER BY Z.W ", qparams);
        mg_map_destroy(qparams);
        // ... 获取返回的ID，并更新本地Node对象的id成员和全局ptr2nodeid映射 ...
    }
    {
        mg_result* result;

        for (auto node : node_list) {
            if (mg_session_fetch(conn.session, &result) == 1) {
                const mg_list* mgl_nodeid = mg_result_row(result);
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
    for (const auto& entity_ptr : updated_entity_list) {
        printf("%s, 0x%x\n", ((ENTITY*)entity_ptr)->type_name(), (long)entity_ptr);
    }
    printf("\nnew_created_entity_list : %llu\n", created_or_updated_entity_list.size() - updated_entity_list.size());

#endif

    //所有指向update前旧节点的节点（ptr2nodeid.at(ptr)）都要再创建一条指向update后新节点（ptr2node.at(ptr)->id）的边
    {
        uint32_t updated_entity_list_size = updated_entity_list.size();
        mg_list* mgl_param_list = mg_list_make_empty(updated_entity_list_size);
        for (auto ptr : updated_entity_list) {
            mg_list* mgl_ptr_list = mg_list_make_empty(2);
            mg_list_append(mgl_ptr_list, mg_value_make_integer(ptr2nodeid.at(ptr)));
            mg_list_append(mgl_ptr_list, mg_value_make_integer(ptr2node.at(ptr)->id));
            mg_list_append(mgl_param_list, mg_value_make_list(mgl_ptr_list));
        }
        mg_map* qparams = mg_map_make_empty(1);
        mg_map_append(qparams, mg_string_make("Y"), mg_value_make_list(mgl_param_list));
        conn.execute_bolt("UNWIND $Y AS t "
            "MATCH (n)<-[r]-(m) WHERE id(n)=t[0] AND NOT ('generation' IN labels(m)) WITH collect(r) AS p,t "
            "MATCH (s) WHERE id(s)=t[1] WITH s,p "
            "UNWIND p AS q "
            "CALL apoc.create.relationship(startNode(q),type(q),properties(q),s)  "
            "YIELD rel RETURN null LIMIT 0 ", qparams);
        mg_map_destroy(qparams);
        conn.discard_all_results();
    }

    //更新节点id对照表
    for (const auto& [ptr, node] : ptr2node) {
        ptr2nodeid[ptr] = node->id;
    }

    for (const auto& entity_ptr : deleted_entity_list) {
        deleted_entity_nodeid_list.push_back(ptr2nodeid.at(entity_ptr));
    }

    //将part与generation节点相连
    {
        Relationship2* r = new Relationship2(partnodeid, generationnode->id); r->label = mg_value_make_string("part_generation_ptr"); r->properties['a'] = mg_value_make_string("part_generation_ptr"); r->properties['b'] = mg_value_make_integer(partnodenextgeneration); relationship_list.push_back(r);
    }

    //将新的generation节点与上一个generation节点相连
    if (partnodenextgeneration > 1) {
        Relationship2* r = new Relationship2(generationnode->id, curgenerationnodeid); r->label = mg_value_make_string("generation_prev_ptr"); r->properties['a'] = mg_value_make_string("generation_prev_ptr"); relationship_list.push_back(r);
    }

    //创建generation_old_ptr边
    for (const auto& nodeid : updated_entity_nodeid_list) {
        Relationship2* r = new Relationship2(generationnode->id, nodeid); r->label = mg_value_make_string("generation_old_ptr"); r->properties['a'] = mg_value_make_string("generation_old_ptr"); relationship_list.push_back(r);
    }
    for (const auto& nodeid : deleted_entity_nodeid_list) {
        Relationship2* r = new Relationship2(generationnode->id, nodeid); r->label = mg_value_make_string("generation_old_ptr"); r->properties['a'] = mg_value_make_string("generation_old_ptr"); relationship_list.push_back(r);
    }

    for (const auto& entity_ptr : created_or_updated_entity_list) {
        {
            int64_t b = ptr2nodeid.at(entity_ptr); Relationship2* r = new Relationship2(generationnode->id, b); r->label = mg_value_make_string("generation_new_ptr"); r->properties['a'] = mg_value_make_string("generation_new_ptr"); relationship_list.push_back(r);
        }
        switch (entity_ptr->identity(1)) {
            case BODY_ID: {
                BODY* ptr = (BODY*)entity_ptr;
                {
                    ENTITY* __tmp_ptr = ptr->lump(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("body_lump_ptr"); r->properties['a'] = mg_value_make_string("body_lump_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->wire(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("body_wire_ptr"); r->properties['a'] = mg_value_make_string("body_wire_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->transform(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("body_transform_ptr"); r->properties['a'] = mg_value_make_string("body_transform_ptr"); relationship_list.push_back(r);
                    }
                }
            } break;
            case LUMP_ID: {
                LUMP* ptr = (LUMP*)entity_ptr;
                {
                    ENTITY* __tmp_ptr = ptr->next(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("lump_next_ptr"); r->properties['a'] = mg_value_make_string("lump_next_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->shell(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("lump_shell_ptr"); r->properties['a'] = mg_value_make_string("lump_shell_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->body(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("lump_body_ptr"); r->properties['a'] = mg_value_make_string("lump_body_ptr"); relationship_list.push_back(r);
                    }
                }
            } break;
            case SHELL_ID: {
                SHELL* ptr = (SHELL*)entity_ptr;
                {
                    ENTITY* __tmp_ptr = ptr->next(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("shell_next_ptr"); r->properties['a'] = mg_value_make_string("shell_next_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->subshell(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("shell_subshell_ptr"); r->properties['a'] = mg_value_make_string("shell_subshell_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->face(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("shell_face_ptr"); r->properties['a'] = mg_value_make_string("shell_face_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->wire(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("shell_wire_ptr"); r->properties['a'] = mg_value_make_string("shell_wire_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->lump(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("shell_lump_ptr"); r->properties['a'] = mg_value_make_string("shell_lump_ptr"); relationship_list.push_back(r);
                    }
                }
            } break;
            case SUBSHELL_ID: {
                SUBSHELL* ptr = (SUBSHELL*)entity_ptr;
                {
                    ENTITY* __tmp_ptr = ptr->parent(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("subshell_parent_ptr"); r->properties['a'] = mg_value_make_string("subshell_parent_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->sibling(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("subshell_sibling_ptr"); r->properties['a'] = mg_value_make_string("subshell_sibling_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->child(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("subshell_child_ptr"); r->properties['a'] = mg_value_make_string("subshell_child_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->face(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("subshell_face_ptr"); r->properties['a'] = mg_value_make_string("subshell_face_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->wire(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("subshell_wire_ptr"); r->properties['a'] = mg_value_make_string("subshell_wire_ptr"); relationship_list.push_back(r);
                    }
                }
            } break;
            case WIRE_ID: {
                WIRE* ptr = (WIRE*)entity_ptr;
                {
                    ENTITY* __tmp_ptr = ptr->next(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("wire_next_ptr"); r->properties['a'] = mg_value_make_string("wire_next_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->coedge(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("wire_coedge_ptr"); r->properties['a'] = mg_value_make_string("wire_coedge_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->owner(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("wire_owner_ptr"); r->properties['a'] = mg_value_make_string("wire_owner_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->subshell(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("wire_subshell_ptr"); r->properties['a'] = mg_value_make_string("wire_subshell_ptr"); relationship_list.push_back(r);
                    }
                }
            } break;
            case FACE_ID: {
                FACE* ptr = (FACE*)entity_ptr;
                {
                    ENTITY* __tmp_ptr = ptr->next(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("face_next_ptr"); r->properties['a'] = mg_value_make_string("face_next_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->loop(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("face_loop_ptr"); r->properties['a'] = mg_value_make_string("face_loop_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->shell(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("face_shell_ptr"); r->properties['a'] = mg_value_make_string("face_shell_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->subshell(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("face_subshell_ptr"); r->properties['a'] = mg_value_make_string("face_subshell_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->geometry(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("face_geometry_ptr"); r->properties['a'] = mg_value_make_string("face_geometry_ptr"); relationship_list.push_back(r);
                    }
                }
            } break;
            case LOOP_ID: {
                LOOP* ptr = (LOOP*)entity_ptr;
                {
                    ENTITY* __tmp_ptr = ptr->next(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("loop_next_ptr"); r->properties['a'] = mg_value_make_string("loop_next_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->start(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("loop_start_ptr"); r->properties['a'] = mg_value_make_string("loop_start_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->face(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("loop_face_ptr"); r->properties['a'] = mg_value_make_string("loop_face_ptr"); relationship_list.push_back(r);
                    }
                }
            } break;
            case COEDGE_ID: {
                COEDGE* ptr = (COEDGE*)entity_ptr;
                {
                    ENTITY* __tmp_ptr = ptr->next(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("coedge_next_ptr"); r->properties['a'] = mg_value_make_string("coedge_next_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->previous(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("coedge_previous_ptr"); r->properties['a'] = mg_value_make_string("coedge_previous_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->partner(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("coedge_partner_ptr"); r->properties['a'] = mg_value_make_string("coedge_partner_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->edge(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("coedge_edge_ptr"); r->properties['a'] = mg_value_make_string("coedge_edge_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->owner(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("coedge_owner_ptr"); r->properties['a'] = mg_value_make_string("coedge_owner_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->geometry(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("coedge_geometry_ptr"); r->properties['a'] = mg_value_make_string("coedge_geometry_ptr"); relationship_list.push_back(r);
                    }
                }
            } break;
            case EDGE_ID: {
                EDGE* ptr = (EDGE*)entity_ptr;
                {
                    ENTITY* __tmp_ptr = ptr->start(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("edge_start_ptr"); r->properties['a'] = mg_value_make_string("edge_start_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->end(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("edge_end_ptr"); r->properties['a'] = mg_value_make_string("edge_end_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->coedge(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("edge_coedge_ptr"); r->properties['a'] = mg_value_make_string("edge_coedge_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->geometry(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("edge_geometry_ptr"); r->properties['a'] = mg_value_make_string("edge_geometry_ptr"); relationship_list.push_back(r);
                    }
                }
            } break;
            case VERTEX_ID: {
                VERTEX* ptr = (VERTEX*)entity_ptr;
                {
                    ENTITY* __tmp_ptr = ptr->edge(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("vertex_edge_ptr"); r->properties['a'] = mg_value_make_string("vertex_edge_ptr"); relationship_list.push_back(r);
                    }
                }
                {
                    ENTITY* __tmp_ptr = ptr->geometry(); if (__tmp_ptr != nullptr) {
                        int64_t a = ptr2nodeid.at(ptr); int64_t b = ptr2nodeid.at(__tmp_ptr); Relationship2* r = new Relationship2(a, b); r->label = mg_value_make_string("vertex_geometry_ptr"); r->properties['a'] = mg_value_make_string("vertex_geometry_ptr"); relationship_list.push_back(r);
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
        mg_list* mgl_rel_list = mg_list_make_empty(rel_list_size);
        for (auto rel : relationship_list) {
            mg_map* mgm_rel = mg_map_make_empty(3 + rel->properties.size());
            mg_map_append(mgm_rel, mg_string_make("U"), mg_value_make_integer(rel->uid));
            mg_map_append(mgm_rel, mg_string_make("V"), mg_value_make_integer(rel->vid));
            mg_map_append(mgm_rel, mg_string_make("T"), rel->label);
            for (auto [propkey, propval] : rel->properties) {
                const char propkey_str[2] = { propkey, 0 };
                mg_map_append(mgm_rel, mg_string_make(propkey_str), propval);
            }
            mg_list_append(mgl_rel_list, mg_value_make_map(mgm_rel));
        }
        mg_map* qparams = mg_map_make_empty(1);
        mg_map_append(qparams, mg_string_make("Y"), mg_value_make_list(mgl_rel_list));
        conn.execute_bolt("UNWIND $Y AS Z "
            "MATCH (W) WHERE id(W) = Z.U "
            "MATCH (X) WHERE id(X) = Z.V "
            "CALL apoc.create.relationship(W,Z.T,{a:Z.a,b:Z.b},X)  "
            "YIELD rel RETURN null LIMIT 0 ", qparams);
        mg_map_destroy(qparams);
        conn.discard_all_results();
    }

    //设置part的当前版本（part节点的generation属性）
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

void api_restore_neo4j(const Neo4jPart& conn, int generation_id) {
    api_delete_history(); //删除默认历史流及其下属公告上的所有实体
    lastsave_ds = nullptr;
    ptr2nodeid.clear();

    std::unordered_map<int64_t, void*> id2ptr;

    mg_map* qparams = mg_map_make_empty(2);
    mg_map_append(qparams, mg_string_make("A"), mg_value_make_string(conn.partname.c_str()));
    mg_map_append(qparams, mg_string_make("B"), mg_value_make_integer(generation_id));
    conn.execute_bolt("MATCH (q:part {a:'part',b:$A})-[r:part_generation_ptr {a:'part_generation_ptr',b:$B}]->(n) "
        "SET q.d=$B WITH n "
        "CALL apoc.path.subgraphAll(n, {minLevel:0,relationshipFilter: 'generation_prev_ptr>|generation_new_ptr>|generation_old_ptr>'}) "
        "YIELD relationships AS y "
        "WITH [z IN y WHERE type(z)='generation_new_ptr' | endNode(z)] AS F,[z IN y WHERE type(z)='generation_old_ptr' | endNode(z)] AS G "
        "WITH [x IN F WHERE NOT (x IN G)] AS H "
        "CALL apoc.algo.cover(H) YIELD rel RETURN H,collect(rel) ", qparams);
    mg_map_destroy(qparams);

    mg_result* result;
    int rows_cnt = 0;
    int status;
    while (1) {
        status = mg_session_fetch(conn.session, &result);
        if (status == 1) {
            rows_cnt++;
            const mg_list* noderellist = mg_result_row(result);
            const uint32_t noderellist_length = mg_list_size(noderellist);
            assert(noderellist_length == 2);
            {
                const mg_value* nodelist_value = mg_list_at(noderellist, 0);
                assert(mg_value_get_type(nodelist_value) == MG_VALUE_TYPE_LIST);
                const mg_list* nodelist = mg_value_list(nodelist_value);
                const uint32_t nodelist_size = mg_list_size(nodelist);
                for (uint32_t i = 0; i < nodelist_size; i++) {
                    const mg_value* node_value = mg_list_at(nodelist, i);
                    assert(mg_value_get_type(node_value) == MG_VALUE_TYPE_NODE);
                    const mg_node* node = mg_value_node(node_value);
                    int64_t node_id = mg_node_id(node);
                    const mg_map* node_properties = mg_node_properties(node);
                    const mg_string* node_typename_mgs = mg_value_string(mg_map_at(node_properties, "a"));
                    std::string node_typename(mg_string_data(node_typename_mgs), mg_string_size(node_typename_mgs));
                    if (node_typename == "part") { continue; }
                    switch (AccessUtils::Restore::Neo4jNode_str2enum.at(node_typename)) {
                        case AccessUtils::Restore::Neo4jNode::body: {
                            BODY* body = nullptr;
                            api_body(body);
                            id2ptr[node_id] = body;
                            ptr2nodeid[body] = node_id;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::lump: {
                            LUMP* lump = nullptr;
                            API_BEGIN;
                            lump = ACIS_NEW LUMP();
                            API_END;
                            id2ptr[node_id] = lump;
                            ptr2nodeid[lump] = node_id;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::shell: {
                            SHELL* shell = nullptr;
                            API_BEGIN;
                            shell = ACIS_NEW SHELL();
                            API_END;
                            id2ptr[node_id] = shell;
                            ptr2nodeid[shell] = node_id;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::subshell: {
                            SUBSHELL* subshell = nullptr;
                            API_BEGIN;
                            subshell = ACIS_NEW SUBSHELL();
                            API_END;
                            id2ptr[node_id] = subshell;
                            ptr2nodeid[subshell] = node_id;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::face: {
                            FACE* face = nullptr;
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
                            ptr2nodeid[face] = node_id;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::loop: {
                            LOOP* loop = nullptr;
                            API_BEGIN;
                            loop = ACIS_NEW LOOP();
                            API_END;
                            id2ptr[node_id] = loop;
                            ptr2nodeid[loop] = node_id;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::wire: {
                            WIRE* wire = nullptr;
                            API_BEGIN;
                            wire = ACIS_NEW WIRE();
                            API_END;
                            wire->set_cont(mg_value_integer(mg_map_at(node_properties, "b")));
                            id2ptr[node_id] = wire;
                            ptr2nodeid[wire] = node_id;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::coedge: {
                            COEDGE* coedge = nullptr;
                            API_BEGIN;
                            coedge = ACIS_NEW COEDGE();
                            API_END;
                            coedge->set_sense(mg_value_integer(mg_map_at(node_properties, "b")));
                            id2ptr[node_id] = coedge;
                            ptr2nodeid[coedge] = node_id;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::edge: {
                            EDGE* edge = nullptr;
                            API_BEGIN;
                            edge = ACIS_NEW EDGE();
                            API_END;
                            edge->set_sense(mg_value_integer(mg_map_at(node_properties, "b")));
                            id2ptr[node_id] = edge;
                            ptr2nodeid[edge] = node_id;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::vertex: {
                            VERTEX* vertex = nullptr;
                            API_BEGIN;
                            vertex = ACIS_NEW VERTEX();
                            API_END;
                            id2ptr[node_id] = vertex;
                            ptr2nodeid[vertex] = node_id;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::transform: {
                            SPAmatrix affine_part = AccessUtils::Restore::parsemglist_SPAmatrix(mg_value_list(mg_map_at(node_properties, "b")));
                            SPAvector translation_part = AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "c")));
                            double scaling_part = mg_value_float(mg_map_at(node_properties, "d"));
                            int rotate_flag = mg_value_integer(mg_map_at(node_properties, "e"));
                            int reflect_flag = mg_value_integer(mg_map_at(node_properties, "f"));
                            int shear_flag = mg_value_integer(mg_map_at(node_properties, "g"));
                            SPAtransf transform_data(affine_part, translation_part, scaling_part, rotate_flag, reflect_flag, shear_flag);
                            TRANSFORM* transform = nullptr;
                            API_BEGIN;
                            transform = ACIS_NEW TRANSFORM(transform_data);
                            API_END;
                            id2ptr[node_id] = transform;
                            ptr2nodeid[transform] = node_id;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::apoint: {
                            SPAposition coords_data = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "b")), 3);
                            APOINT* point = nullptr;
                            API_BEGIN;
                            point = ACIS_NEW APOINT(coords_data);
                            API_END;
                            id2ptr[node_id] = point;
                            ptr2nodeid[point] = node_id;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::straight_curve: {
                            SPAposition root_point = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "c")), 3);
                            SPAvector direction = AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "d")));
                            straight* def = ACIS_NEW straight(root_point, normalise(direction));
                            def->gme_set_param_scale(direction.len());
                            SPAinterval subset_range = AccessUtils::Restore::parsemglist_SPAinterval(mg_value_list(mg_map_at(node_properties, "b")));
                            def->gme_set_subset_range(subset_range);
                            STRAIGHT* straight_curve = nullptr;
                            API_BEGIN;
                            straight_curve = ACIS_NEW STRAIGHT(*def);
                            API_END;
                            ACIS_DELETE def;
                            id2ptr[node_id] = straight_curve;
                            ptr2nodeid[straight_curve] = node_id;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::ellipse_curve: {
                            SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "c")), 3);
                            SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "d")));
                            SPAvector major_axis = AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "e")));
                            double radius_ratio = mg_value_float(mg_map_at(node_properties, "f"));
                            ellipse* def = ACIS_NEW ellipse(centre, normal, major_axis, radius_ratio);
                            SPAinterval subset_range = AccessUtils::Restore::parsemglist_SPAinterval(mg_value_list(mg_map_at(node_properties, "b")));
                            def->gme_set_subset_range(subset_range);
                            ELLIPSE* ellipse_curve = nullptr;
                            API_BEGIN;
                            ellipse_curve = ACIS_NEW ELLIPSE(*def);
                            API_END;
                            ACIS_DELETE def;
                            id2ptr[node_id] = ellipse_curve;
                            ptr2nodeid[ellipse_curve] = node_id;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::helix_curve: {
                            SPAposition axis_root = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "c")), 3);
                            SPAunit_vector axis_dir = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "d")));
                            SPAvector start_disp = AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "e")));
                            double pitch = mg_value_float(mg_map_at(node_properties, "f"));
                            int handedness = mg_value_integer(mg_map_at(node_properties, "g"));
                            double par_scaling = mg_value_float(mg_map_at(node_properties, "h"));
                            double taper = mg_value_float(mg_map_at(node_properties, "i"));
                            SPAinterval helix_range = AccessUtils::Restore::parsemglist_SPAinterval(mg_value_list(mg_map_at(node_properties, "j")));
                            helix* def = ACIS_NEW helix(axis_root, axis_dir, start_disp, pitch, handedness, helix_range, par_scaling, taper);
                            SPAinterval subset_range = AccessUtils::Restore::parsemglist_SPAinterval(mg_value_list(mg_map_at(node_properties, "b")));
                            def->gme_set_subset_range(subset_range);
                            HELIX* helix_curve = nullptr;
                            API_BEGIN;
                            helix_curve = ACIS_NEW HELIX(*def);
                            API_END;
                            ACIS_DELETE def;
                            id2ptr[node_id] = helix_curve;
                            ptr2nodeid[helix_curve] = node_id;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::plane_surface: {
                            SPAposition root_point = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "c")), 3);
                            SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "d")));
                            SPAvector u_deriv = AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "e")));
                            plane* def = ACIS_NEW plane(root_point, normal, u_deriv);
                            def->reverse_v = mg_value_integer(mg_map_at(node_properties, "f"));
                            def->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "b"))));
                            PLANE* plane_surface = nullptr;
                            API_BEGIN;
                            plane_surface = ACIS_NEW PLANE(*def);
                            API_END;
                            ACIS_DELETE def;
                            id2ptr[node_id] = plane_surface;
                            ptr2nodeid[plane_surface] = node_id;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::sphere_surface: {
                            SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "c")), 3);
                            double radius = mg_value_float(mg_map_at(node_properties, "d"));
                            sphere* def = ACIS_NEW sphere(centre, radius);
                            def->uv_oridir = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "e")));
                            def->pole_dir = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "f")));
                            def->reverse_v = mg_value_integer(mg_map_at(node_properties, "g"));
                            def->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "b"))));
                            SPHERE* sphere_surface = nullptr;
                            API_BEGIN;
                            sphere_surface = ACIS_NEW SPHERE(*def);
                            API_END;
                            ACIS_DELETE def;
                            id2ptr[node_id] = sphere_surface;
                            ptr2nodeid[sphere_surface] = node_id;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::torus_surface: {
                            SPAposition centre = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "c")), 3);
                            SPAunit_vector normal = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "d")));
                            double major_radius = mg_value_float(mg_map_at(node_properties, "e"));
                            double minor_radius = mg_value_float(mg_map_at(node_properties, "f"));
                            torus* def = ACIS_NEW torus(centre, normal, major_radius, minor_radius);
                            def->uv_oridir = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "g")));
                            def->reverse_v = mg_value_integer(mg_map_at(node_properties, "h"));
                            def->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "b"))));
                            TORUS* torus_surface = nullptr;
                            API_BEGIN;
                            torus_surface = ACIS_NEW TORUS(*def);
                            API_END;
                            ACIS_DELETE def;
                            id2ptr[node_id] = torus_surface;
                            ptr2nodeid[torus_surface] = node_id;
                        } break;
                        case AccessUtils::Restore::Neo4jNode::cone_surface: {
                            SPAposition base_centre = AccessUtils::Restore::parsemglist_SPAposition(mg_value_list(mg_map_at(node_properties, "c")), 3);
                            SPAunit_vector base_normal = AccessUtils::Restore::parsemglist_SPAunit_vector(mg_value_list(mg_map_at(node_properties, "d")));
                            SPAvector base_major_axis = AccessUtils::Restore::parsemglist_SPAvector(mg_value_list(mg_map_at(node_properties, "e")));
                            double base_radius_ratio = mg_value_float(mg_map_at(node_properties, "f"));
                            ellipse* gem_base = ACIS_NEW ellipse(base_centre, base_normal, base_major_axis, base_radius_ratio);
                            gem_base->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPAinterval(mg_value_list(mg_map_at(node_properties, "g"))));
                            double sine_angle = mg_value_float(mg_map_at(node_properties, "h"));
                            double cosine_angle = mg_value_float(mg_map_at(node_properties, "i"));
                            cone* def = ACIS_NEW cone(*gem_base, sine_angle, cosine_angle);
                            ACIS_DELETE gem_base;
                            def->reverse_u = mg_value_integer(mg_map_at(node_properties, "j"));
                            def->gme_set_subset_range(AccessUtils::Restore::parsemglist_SPApar_box(mg_value_list(mg_map_at(node_properties, "b"))));
                            CONE* cone_surface = nullptr;
                            API_BEGIN;
                            cone_surface = ACIS_NEW CONE(*def);
                            API_END;
                            ACIS_DELETE def;
                            id2ptr[node_id] = cone_surface;
                            ptr2nodeid[cone_surface] = node_id;
                        } break;
                        default: {
                            myerror("不支持的neo4j节点类型。");
                        } break;
                    }
                }
            }
            {
                const mg_value* rellist_value = mg_list_at(noderellist, 1);
                assert(mg_value_get_type(rellist_value) == MG_VALUE_TYPE_LIST);
                const mg_list* rellist = mg_value_list(rellist_value);
                const uint32_t rellist_size = mg_list_size(rellist);
                for (uint32_t i = 0; i < rellist_size; i++) {
                    const mg_value* rel_value = mg_list_at(rellist, i);
                    assert(mg_value_get_type(rel_value) == MG_VALUE_TYPE_RELATIONSHIP);
                    const mg_relationship* rel = mg_value_relationship(rel_value);
                    int64_t rel_startnode_id = mg_relationship_start_id(rel);
                    int64_t rel_endnode_id = mg_relationship_end_id(rel);
                    const mg_map* rel_properties = mg_relationship_properties(rel);
                    const mg_string* rel_typename_mgs = mg_value_string(mg_map_at(rel_properties, "a"));
                    std::string rel_typename(mg_string_data(rel_typename_mgs), mg_string_size(rel_typename_mgs));
                    if (rel_typename == "part_entity_ptr") { continue; }
                    switch (AccessUtils::Restore::Neo4jEdge_str2enum.at(rel_typename)) {
                        case AccessUtils::Restore::Neo4jEdge::body_lump_ptr: {
                            BODY* body = (BODY*)id2ptr.at(rel_startnode_id);
                            LUMP* lump = (LUMP*)id2ptr.at(rel_endnode_id);
                            body->set_lump(lump);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::body_wire_ptr: {
                            BODY* body = (BODY*)id2ptr.at(rel_startnode_id);
                            WIRE* wire = (WIRE*)id2ptr.at(rel_endnode_id);
                            body->set_wire(wire);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::body_transform_ptr: {
                            BODY* body = (BODY*)id2ptr.at(rel_startnode_id);
                            TRANSFORM* transform = (TRANSFORM*)id2ptr.at(rel_endnode_id);
                            body->set_transform(transform);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::lump_next_ptr: {
                            LUMP* lump = (LUMP*)id2ptr.at(rel_startnode_id);
                            LUMP* next = (LUMP*)id2ptr.at(rel_endnode_id);
                            lump->set_next(next);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::lump_shell_ptr: {
                            LUMP* lump = (LUMP*)id2ptr.at(rel_startnode_id);
                            SHELL* shell = (SHELL*)id2ptr.at(rel_endnode_id);
                            lump->set_shell(shell);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::lump_body_ptr: {
                            LUMP* lump = (LUMP*)id2ptr.at(rel_startnode_id);
                            BODY* body = (BODY*)id2ptr.at(rel_endnode_id);
                            lump->set_body(body);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::shell_next_ptr: {
                            SHELL* shell = (SHELL*)id2ptr.at(rel_startnode_id);
                            SHELL* next = (SHELL*)id2ptr.at(rel_endnode_id);
                            shell->set_next(next);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::shell_subshell_ptr: {
                            SHELL* shell = (SHELL*)id2ptr.at(rel_startnode_id);
                            SUBSHELL* subshell = (SUBSHELL*)id2ptr.at(rel_endnode_id);
                            shell->set_subshell(subshell);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::shell_face_ptr: {
                            SHELL* shell = (SHELL*)id2ptr.at(rel_startnode_id);
                            FACE* face = (FACE*)id2ptr.at(rel_endnode_id);
                            shell->set_face(face);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::shell_wire_ptr: {
                            SHELL* shell = (SHELL*)id2ptr.at(rel_startnode_id);
                            WIRE* wire = (WIRE*)id2ptr.at(rel_endnode_id);
                            shell->set_wire(wire);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::shell_lump_ptr: {
                            SHELL* shell = (SHELL*)id2ptr.at(rel_startnode_id);
                            LUMP* lump = (LUMP*)id2ptr.at(rel_endnode_id);
                            shell->set_lump(lump);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::subshell_parent_ptr: {
                            SUBSHELL* subshell = (SUBSHELL*)id2ptr.at(rel_startnode_id);
                            SUBSHELL* parent = (SUBSHELL*)id2ptr.at(rel_endnode_id);
                            subshell->set_parent(parent);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::subshell_sibling_ptr: {
                            SUBSHELL* subshell = (SUBSHELL*)id2ptr.at(rel_startnode_id);
                            SUBSHELL* sibling = (SUBSHELL*)id2ptr.at(rel_endnode_id);
                            subshell->set_sibling(sibling);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::subshell_child_ptr: {
                            SUBSHELL* subshell = (SUBSHELL*)id2ptr.at(rel_startnode_id);
                            SUBSHELL* child = (SUBSHELL*)id2ptr.at(rel_endnode_id);
                            subshell->set_child(child);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::subshell_face_ptr: {
                            SUBSHELL* subshell = (SUBSHELL*)id2ptr.at(rel_startnode_id);
                            FACE* face = (FACE*)id2ptr.at(rel_endnode_id);
                            subshell->set_face(face);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::subshell_wire_ptr: {
                            SUBSHELL* subshell = (SUBSHELL*)id2ptr.at(rel_startnode_id);
                            WIRE* wire = (WIRE*)id2ptr.at(rel_endnode_id);
                            subshell->set_wire(wire);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::wire_next_ptr: {
                            WIRE* wire = (WIRE*)id2ptr.at(rel_startnode_id);
                            WIRE* next = (WIRE*)id2ptr.at(rel_endnode_id);
                            wire->set_next(next);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::wire_coedge_ptr: {
                            WIRE* wire = (WIRE*)id2ptr.at(rel_startnode_id);
                            COEDGE* coedge = (COEDGE*)id2ptr.at(rel_endnode_id);
                            wire->set_coedge(coedge);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::wire_owner_ptr: {
                            WIRE* wire = (WIRE*)id2ptr.at(rel_startnode_id);
                            ENTITY* owner = (ENTITY*)id2ptr.at(rel_endnode_id);
                            wire->set_owner(owner);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::wire_subshell_ptr: {
                            WIRE* wire = (WIRE*)id2ptr.at(rel_startnode_id);
                            SUBSHELL* subshell = (SUBSHELL*)id2ptr.at(rel_endnode_id);
                            wire->set_subshell(subshell);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::face_next_ptr: {
                            FACE* face = (FACE*)id2ptr.at(rel_startnode_id);
                            FACE* next = (FACE*)id2ptr.at(rel_endnode_id);
                            face->set_next(next);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::face_loop_ptr: {
                            FACE* face = (FACE*)id2ptr.at(rel_startnode_id);
                            LOOP* loop = (LOOP*)id2ptr.at(rel_endnode_id);
                            face->set_loop(loop);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::face_shell_ptr: {
                            FACE* face = (FACE*)id2ptr.at(rel_startnode_id);
                            SHELL* shell = (SHELL*)id2ptr.at(rel_endnode_id);
                            face->set_shell(shell);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::face_subshell_ptr: {
                            FACE* face = (FACE*)id2ptr.at(rel_startnode_id);
                            SUBSHELL* subshell = (SUBSHELL*)id2ptr.at(rel_endnode_id);
                            face->set_subshell(subshell);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::face_geometry_ptr: {
                            FACE* face = (FACE*)id2ptr.at(rel_startnode_id);
                            SURFACE* geometry = (SURFACE*)id2ptr.at(rel_endnode_id);
                            face->set_geometry(geometry);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::loop_next_ptr: {
                            LOOP* loop = (LOOP*)id2ptr.at(rel_startnode_id);
                            LOOP* next = (LOOP*)id2ptr.at(rel_endnode_id);
                            loop->set_next(next);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::loop_start_ptr: {
                            LOOP* loop = (LOOP*)id2ptr.at(rel_startnode_id);
                            COEDGE* start = (COEDGE*)id2ptr.at(rel_endnode_id);
                            loop->set_start(start);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::loop_face_ptr: {
                            LOOP* loop = (LOOP*)id2ptr.at(rel_startnode_id);
                            FACE* face = (FACE*)id2ptr.at(rel_endnode_id);
                            loop->set_face(face);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::coedge_next_ptr: {
                            COEDGE* coedge = (COEDGE*)id2ptr.at(rel_startnode_id);
                            COEDGE* next = (COEDGE*)id2ptr.at(rel_endnode_id);
                            coedge->set_next(next);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::coedge_previous_ptr: {
                            COEDGE* coedge = (COEDGE*)id2ptr.at(rel_startnode_id);
                            COEDGE* previous = (COEDGE*)id2ptr.at(rel_endnode_id);
                            coedge->set_previous(previous);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::coedge_partner_ptr: {
                            COEDGE* coedge = (COEDGE*)id2ptr.at(rel_startnode_id);
                            COEDGE* partner = (COEDGE*)id2ptr.at(rel_endnode_id);
                            coedge->set_partner(partner);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::coedge_edge_ptr: {
                            COEDGE* coedge = (COEDGE*)id2ptr.at(rel_startnode_id);
                            EDGE* edge = (EDGE*)id2ptr.at(rel_endnode_id);
                            coedge->set_edge(edge);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::coedge_owner_ptr: {
                            COEDGE* coedge = (COEDGE*)id2ptr.at(rel_startnode_id);
                            ENTITY* owner = (ENTITY*)id2ptr.at(rel_endnode_id);
                            coedge->set_owner(owner);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::coedge_geometry_ptr: {
                            COEDGE* coedge = (COEDGE*)id2ptr.at(rel_startnode_id);
                            PCURVE* geometry = (PCURVE*)id2ptr.at(rel_endnode_id);
                            coedge->set_geometry(geometry);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::edge_start_ptr: {
                            EDGE* edge = (EDGE*)id2ptr.at(rel_startnode_id);
                            VERTEX* start = (VERTEX*)id2ptr.at(rel_endnode_id);
                            edge->set_start(start);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::edge_end_ptr: {
                            EDGE* edge = (EDGE*)id2ptr.at(rel_startnode_id);
                            VERTEX* end = (VERTEX*)id2ptr.at(rel_endnode_id);
                            edge->set_end(end);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::edge_coedge_ptr: {
                            EDGE* edge = (EDGE*)id2ptr.at(rel_startnode_id);
                            COEDGE* coedge = (COEDGE*)id2ptr.at(rel_endnode_id);
                            edge->set_coedge(coedge);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::edge_geometry_ptr: {
                            EDGE* edge = (EDGE*)id2ptr.at(rel_startnode_id);
                            CURVE* geometry = (CURVE*)id2ptr.at(rel_endnode_id);
                            edge->set_geometry(geometry);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::vertex_edge_ptr: {
                            VERTEX* vertex = (VERTEX*)id2ptr.at(rel_startnode_id);
                            EDGE* edge = (EDGE*)id2ptr.at(rel_endnode_id);
                            vertex->set_edge(edge);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::vertex_geometry_ptr: {
                            VERTEX* vertex = (VERTEX*)id2ptr.at(rel_startnode_id);
                            APOINT* geometry = (APOINT*)id2ptr.at(rel_endnode_id);
                            vertex->set_geometry(geometry);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::pcurve_ref_curve_ptr: {
                            PCURVE* pcurve = (PCURVE*)id2ptr.at(rel_startnode_id);
                            CURVE* ref_curve = (CURVE*)id2ptr.at(rel_endnode_id);
                            pcurve->set_def(ref_curve, pcurve->index(), 0, pcurve->offset());
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::pcurve_fit_ptr: {
                            PCURVE* p = (PCURVE*)id2ptr.at(rel_startnode_id);
                            par_cur* fit = (par_cur*)id2ptr.at(rel_endnode_id);
                            p->set_fit(fit);
                            fit->add_ref();
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::spline_surface_spl_ptr: {
                            SPLINE* spline_surface = (SPLINE*)id2ptr.at(rel_startnode_id);
                            spl_sur* spl = (spl_sur*)id2ptr.at(rel_endnode_id);
                            spline_surface->gme_set_spl(spl);
                            spl->add_ref();
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::intcurve_curve_fit_ptr: {
                            INTCURVE* intcurve_curve = (INTCURVE*)id2ptr.at(rel_startnode_id);
                            int_cur* fit = (int_cur*)id2ptr.at(rel_endnode_id);
                            intcurve_curve->gme_set_fit(fit);
                            fit->add_ref();
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::int_cur_surf1_spl_ptr: {
                            int_cur* ic = (int_cur*)id2ptr.at(rel_startnode_id);
                            spl_sur* surf1_spl = (spl_sur*)id2ptr.at(rel_endnode_id);
                            ic->set_surf1_spl(surf1_spl);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::int_cur_surf2_spl_ptr: {
                            int_cur* ic = (int_cur*)id2ptr.at(rel_startnode_id);
                            spl_sur* surf2_spl = (spl_sur*)id2ptr.at(rel_endnode_id);
                            ic->set_surf2_spl(surf2_spl);
                        } break;
                        case AccessUtils::Restore::Neo4jEdge::par_cur_surf_spl_ptr: {
                            par_cur* pc = (par_cur*)id2ptr.at(rel_startnode_id);
                            spl_sur* surf_spl = (spl_sur*)id2ptr.at(rel_endnode_id);
                            ((exp_par_cur*)pc)->gme_set_surf_spl(surf_spl);
                        } break;
                        default: {
                            myerror("不支持的neo4j边类型。");
                        } break;
                    }
                }
            }
        } else if (status == 0) {
            const mg_map* mgm = mg_result_summary(result);
            const uint32_t mgm_size = mg_map_size(mgm);
            for (uint32_t i = 0; i < mgm_size; i++) {
                const mg_string* itemkey = mg_map_key_at(mgm, i);
                const mg_value* itemvalue = mg_map_value_at(mgm, i);
            }
            break;
        } else {
            myerror(std::format("mg_session_fetch失败，错误信息如下：{}", mg_session_error(conn.session)));
        }
    }

    if (rows_cnt == 0) {
        myerror("恢复时指定的顶级实体图节点elementId没有找到对应的图节点！");
    }

    DELTA_STATE* thissave_ds; //这次保存
    api_note_state(thissave_ds);
    lastsave_ds = thissave_ds;
}


void api_save_entity_list_neo4j_part(const Neo4jPart& conn, const ENTITY_LIST& entity_list) {
    mg_map* qparams = mg_map_make_empty(1);
    mg_map_append(qparams, mg_string_make("Y"), mg_value_make_string(conn.partname.c_str()));
    conn.execute_bolt("MATCH (n:part {b:$Y}) CALL apoc.path.subgraphAll(n, {minLevel:0}) YIELD nodes FOREACH(n IN nodes | DETACH DELETE n)", qparams);
    conn.discard_all_results();
    conn.execute_bolt("CREATE (n:part {a:'part',b:$Y}) RETURN id(n)", qparams);
    mg_map_destroy(qparams);

    int64_t partnodeid;
    mg_result* result;
    if (mg_session_fetch(conn.session, &result) == 1) {
        const mg_list* mgl_partnodeid = mg_result_row(result);
        const uint32_t mgl_partnodeid_length = mg_list_size(mgl_partnodeid);
        assert(mgl_partnodeid_length == 1);
        partnodeid = mg_value_integer(mg_list_at(mgl_partnodeid, 0));
    } else {
        myerror(std::format("mg_session_fetch失败，错误信息如下：{}", mg_session_error(conn.session)));
    }
    if (mg_session_fetch(conn.session, &result) != 0) {
        myerror(std::format("mg_session_fetch失败，错误信息如下：{}", mg_session_error(conn.session)));
    }

    std::unordered_map<void*, int64_t> ptr2elemid;
    api_save_entity_list_neo4j(conn, entity_list, ptr2elemid);

    std::vector<int64_t> elemid_list;
    uint32_t elemid_list_size = entity_list.iteration_count();
    mg_list* mgl_elemid_list = mg_list_make_empty(elemid_list_size);
    int64_t entity_idx = 0;
    for (ENTITY* e : entity_list) {
        mg_map* mgm_rel = mg_map_make_empty(2);
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
        "CREATE (W)-[r:part_entity_ptr {a:'part_entity_ptr',b:Z.V}]->(X) ", qparams);
    mg_map_destroy(qparams);
    conn.discard_all_results();
}

void api_restore_entity_list_neo4j_part(const Neo4jPart& conn, ENTITY_LIST& entity_list) {
    std::vector<int64_t> elemid_list;
    mg_map* qparams = mg_map_make_empty(1);
    mg_map_append(qparams, mg_string_make("Y"), mg_value_make_string(conn.partname.c_str()));
    conn.execute_bolt("MATCH (n:part {b:$Y})-[r:part_entity_ptr]->(c) RETURN id(c) ORDER BY r.idx ", qparams);
    mg_map_destroy(qparams);

    mg_result* result;
    int status;
    while (1) {
        status = mg_session_fetch(conn.session, &result);
        if (status == 1) {
            const mg_list* mgl_entitynodeid = mg_result_row(result);
            const uint32_t mgl_entitynodeid_length = mg_list_size(mgl_entitynodeid);
            assert(mgl_entitynodeid_length == 1);
            elemid_list.push_back(mg_value_integer(mg_list_at(mgl_entitynodeid, 0)));
        } else if (status == 0) {
            break;
        } else {
            myerror(std::format("mg_session_fetch失败，错误信息如下：{}", mg_session_error(conn.session)));
        }
    }

    std::unordered_map<int64_t, void*> elemid2ptr;
    api_restore_entity_list_neo4j(conn, elemid_list, entity_list, elemid2ptr);
}

int64_t count_partnode(const Neo4jPart& conn) {
    mg_map* qparams = mg_map_make_empty(1);
    mg_map_append(qparams, mg_string_make("Y"), mg_value_make_string(conn.partname.c_str()));
    conn.execute_bolt("MATCH (n:part {b:$Y}) RETURN count(n) ", qparams);
    mg_map_destroy(qparams);

    int64_t partnodecount;
    mg_result* result;
    if (mg_session_fetch(conn.session, &result) == 1) {
        const mg_list* mgl_partnodecount = mg_result_row(result);
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

void acis_save_entity_list(const ENTITY_LIST& elist, const char* file_name, int major_version, int minor_version, int text_mode) {
    API_NOP_BEGIN;
    api_save_version(major_version, minor_version);
    FileInfo fileinfo;
    fileinfo.set_units(1.0);
    fileinfo.set_product_id("SimpleApi");
    result = api_set_file_info((FileIdent | FileUnits), fileinfo);
    result = api_set_int_option("sequence_save_files", 1);
    FILE* save_file;
    fopen_s(&save_file, file_name, "wb");
    if (!save_file) {
        myerror("打开文件失败，文件名为" + std::string(file_name));
    } else {
        result = api_save_entity_list(save_file, text_mode, elist);
        fclose(save_file);
    }
    API_NOP_END;
}

void acis_get_noattrib_toplevel_active_entities(ENTITY_LIST& elist, HISTORY_STREAM* hs) {
    if (hs == NULL) {
        api_get_default_history(hs);
    }
    api_get_active_entities(hs, elist, 1); //参数1表示只获取顶级实体

    //去除（不保存）annotation、attrib等类型实体
    elist.init();
    ENTITY* tmpentity = nullptr;
    while (tmpentity = elist.next()) {
        if (is_ANNOTATION(tmpentity) || is_ATTRIB_TAG(tmpentity)) {
            elist.remove(tmpentity);
        }
    }
}

void acis_save_noattrib_toplevel_active_entities(const char* file_name, int major_version, int minor_version, int text_mode, HISTORY_STREAM* hs) {
    ENTITY_LIST elist;
    acis_get_noattrib_toplevel_active_entities(elist, hs);
    acis_save_entity_list(elist, file_name, major_version, minor_version, text_mode);
}

void acis_save_history(const char* file_name, int major_version, int minor_version, int text_mode, HISTORY_STREAM* hs) {
    API_NOP_BEGIN;
    api_save_version(major_version, minor_version);
    FileInfo fileinfo;
    fileinfo.set_units(1.0);
    fileinfo.set_product_id("SimpleApi");
    result = api_set_file_info((FileIdent | FileUnits), fileinfo);
    result = api_set_int_option("sequence_save_files", 1);
    FILE* save_file;
    fopen_s(&save_file, file_name, "wb");
    if (!save_file) {
        myerror("打开文件失败，文件名为" + std::string(file_name));
    } else {
        result = api_save_history(save_file, text_mode, hs);
        fclose(save_file);
    }
    API_NOP_END;
}

void acis_restore_entity_list(ENTITY_LIST& elist, const char* file_name, int major_version, int minor_version, int text_mode) {
    API_BEGIN;
    api_save_version(major_version, minor_version);
    FILE* save_file;
    fopen_s(&save_file, file_name, "rb");
    if (!save_file) {
        myerror("打开文件失败，文件名为" + std::string(file_name));
    } else {
        result = api_restore_entity_list(save_file, text_mode, elist);
        fclose(save_file);
    }
    API_END;
}

std::string AccessTest::read_file_to_string(std::string filename) {
    std::ifstream t(filename);
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);
    return buffer;
}
std::tuple<bool, double, double, double, double> AccessTest::CheckTestCase(const Neo4jPart& conn, std::string testcase_name, const ENTITY_LIST& el) {
    TMDF;

    std::string acis_save_filename_str = std::format("acis_{}_save.sat", testcase_name);
    const char* acis_save_filename = acis_save_filename_str.c_str();


    //ACIS接口保存
    TMST;
    acis_save_entity_list(el, acis_save_filename, 2, 0, true);
    TMED;
    double acis_save_duration = TMDR;

    //ACIS接口恢复
    ENTITY_LIST el_restore_acis;
    TMST;
    acis_restore_entity_list(el_restore_acis, acis_save_filename, 2, 0, true);
    TMED;
    double acis_restore_duration = TMDR;

    //删除ACIS接口保存的文件
    //remove(acis_save_filename);


    //neo4j接口保存
    std::unordered_map<void*, int64_t> ptr2elemid;
    TMST;
    api_save_entity_list_neo4j(conn, el, ptr2elemid);
    TMED;
    double neo4j_save_duration = TMDR;

    //neo4j接口恢复
    ENTITY_LIST el_restore_neo4j;
    std::vector<int64_t> elemid_list;
    for (ENTITY* e : el) {
        elemid_list.push_back(ptr2elemid.at(e));
    }
    std::unordered_map<int64_t, void*> elemid2ptr;
    TMST;
    api_restore_entity_list_neo4j(conn, elemid_list, el_restore_neo4j, elemid2ptr);
    TMED;
    double neo4j_restore_duration = TMDR;
    //删除neo4j接口保存的节点和边
    //for (const int64_t id : elemid_list_subgraph) {
    //    conn.query("MATCH (n) WHERE id(n) = $d "
    //        "CALL apoc.path.subgraphAll(n, {minLevel:0}) YIELD nodes FOREACH(n IN nodes | DETACH DELETE n) ", nlohmann::json{
    //                                                                                                   {"d", id},
    //        });
    //}
    conn.execute_bolt("match(n) call { with n detach delete n } in transactions of 10000 rows", NULL);
    conn.discard_all_results();


    //对el_restore_neo4j和el_restore_acis判等
    std::string acis_check_filename_str = std::format("acis_{}_check.sat", testcase_name);
    std::string neo4j_check_filename_str = std::format("neo4j_{}_check.sat", testcase_name);
    acis_save_entity_list(el_restore_acis, acis_check_filename_str.c_str(), 2, 0, true);
    acis_save_entity_list(el_restore_neo4j, neo4j_check_filename_str.c_str(), 2, 0, true);
    //比较时略过SAT文件头
    int newline_1_pos, newline_2_pos;
    std::string acis_check_file_str = read_file_to_string(acis_check_filename_str);
    newline_1_pos = acis_check_file_str.find('\n');
    newline_2_pos = acis_check_file_str.find('\n', newline_1_pos + 1);
    std::string acis_check_file_str_remove_header = acis_check_file_str.substr(newline_2_pos + 1);
    std::string neo4j_check_file_str = read_file_to_string(neo4j_check_filename_str);
    newline_1_pos = neo4j_check_file_str.find('\n');
    newline_2_pos = neo4j_check_file_str.find('\n', newline_1_pos + 1);
    std::string neo4j_check_file_str_remove_header = neo4j_check_file_str.substr(newline_2_pos + 1);
    bool testresult = acis_check_file_str_remove_header == neo4j_check_file_str_remove_header;

    //删除ACIS接口保存的文件
    //remove(acis_check_filename_str.c_str());
    //remove(neo4j_subgraph_check_filename_str.c_str());

    return std::make_tuple(testresult, neo4j_save_duration, acis_save_duration, neo4j_restore_duration, acis_restore_duration);
}