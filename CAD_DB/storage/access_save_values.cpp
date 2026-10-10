#include "access_internal.hxx"

namespace AccessUtils {
namespace Save {
std::string getstring_number(double num) {
    return std::format("{}", num);
}

std::string getstring_number(int num) {
    return std::to_string(num);
}

mg_list *getmglist_SPAposition(const SPAposition &ptr, int tag) {
    mg_list *ans;
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

mg_list *getmglist_SPAinterval(const SPAinterval &ptr) {
    mg_list *ans;
    if (ptr.empty()) {
        ans = mg_list_make_empty(2);
        mg_list_append(ans, mg_value_make_float(1.0)); // 1:infinite 2:finite
        mg_list_append(ans, mg_value_make_float(1.0));
    } else {
        interval_type type = ptr.type();
        if (type == interval_type::interval_finite) {
            if (ptr.start_pt() > ptr.end_pt())
                goto interval_infinite;
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

mg_list *getmglist_SPApar_box(const SPApar_box &ptr) {
    mg_list *ans = mg_list_make_empty(8);
    SPAinterval range_u = ptr.u_range();
    if (range_u.empty()) {
        mg_list_append(ans, mg_value_make_float(1.0)); // 1:infinite 2:finite
        mg_list_append(ans, mg_value_make_float(1.0));
    } else {
        interval_type type = range_u.type();
        if (type == interval_type::interval_finite) {
            if (range_u.start_pt() > range_u.end_pt())
                goto interval_infinite_u;
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
            if (range_v.start_pt() > range_v.end_pt())
                goto interval_infinite_v;
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

mg_list *getmglist_SPAvector(const SPAvector &ptr) {
    mg_list *ans = mg_list_make_empty(3);
    mg_list_append(ans, mg_value_make_float(ptr.x()));
    mg_list_append(ans, mg_value_make_float(ptr.y()));
    mg_list_append(ans, mg_value_make_float(ptr.z()));
    return ans;
}

mg_list *getmglist_SPAunit_vector(const SPAunit_vector &ptr) {
    mg_list *ans = mg_list_make_empty(3);
    mg_list_append(ans, mg_value_make_float(ptr.x()));
    mg_list_append(ans, mg_value_make_float(ptr.y()));
    mg_list_append(ans, mg_value_make_float(ptr.z()));
    return ans;
}

mg_list *getmglist_SPAmatrix(const SPAmatrix &ptr) {
    mg_list *ans = mg_list_make_empty(9);
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
} // namespace Save
} // namespace AccessUtils
