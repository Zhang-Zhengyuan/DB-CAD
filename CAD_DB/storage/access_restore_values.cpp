#include "access_internal.hxx"

namespace AccessUtils {
namespace Restore {
SPAposition parsemglist_SPAposition(const mg_list *mgl, int tag) {
    if (tag == 3) {
        return SPAposition(mg_value_float(mg_list_at(mgl, 0)), mg_value_float(mg_list_at(mgl, 1)),
                           mg_value_float(mg_list_at(mgl, 2)));
    } else if (tag == 2) {
        return SPAposition(mg_value_float(mg_list_at(mgl, 0)), mg_value_float(mg_list_at(mgl, 1)), 0);
    } else if (tag == 1) {
        return SPAposition(mg_value_float(mg_list_at(mgl, 0)), 0, 0);
    } else {
        return SPAposition(mg_value_float(mg_list_at(mgl, 0)), mg_value_float(mg_list_at(mgl, 1)),
                           mg_value_float(mg_list_at(mgl, 2)));
    }
}

SPAinterval parsemglist_SPAinterval(const mg_list *mgl) {
    const uint32_t mgl_size = mg_list_size(mgl);
    uint32_t mgl_idx = 0;
    int interval_end_idx = 0;
    double low = 1, high = 0;
    bool low_infinite = false, high_infinite = false;
    while (mgl_idx < mgl_size) {
        int64_t interval_type = mg_value_float(mg_list_at(mgl, mgl_idx));
        if (interval_type == 1.0) {
            // infinite interval
            switch (interval_end_idx) {
            case 0: {
                low_infinite = true;
            } break;
            case 1: {
                high_infinite = true;
            } break;
            default: {
                myerror("SPAinterval解析出的区间端点个数大于2。");
            } break;
            }
            mgl_idx++;
            interval_end_idx++;
        } else {
            // finite interval
            assert(interval_type == 2.0);
            double interval_end = mg_value_float(mg_list_at(mgl, mgl_idx + 1));
            switch (interval_end_idx) {
            case 0: {
                low = interval_end;
            } break;
            case 1: {
                high = interval_end;
            } break;
            default: {
                myerror("SPAinterval解析出的区间端点个数大于2。");
            } break;
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
            return SPAinterval(interval_type::interval_finite_below, low, high);
        } else {
            return SPAinterval(interval_type::interval_finite, low, high);
        }
    }
}

SPApar_box parsemglist_SPApar_box(const mg_list *mgl) {
    const uint32_t mgl_size = mg_list_size(mgl);
    uint32_t mgl_idx = 0;
    int interval_end_idx = 0;
    double u_low = 1, u_high = 0, v_low = 1, v_high = 0;
    bool u_low_infinite = false, u_high_infinite = false, v_low_infinite = false, v_high_infinite = false;
    while (mgl_idx < mgl_size) {
        int64_t interval_type = mg_value_float(mg_list_at(mgl, mgl_idx));
        if (interval_type == 1.0) {
            // infinite interval
            switch (interval_end_idx) {
            case 0: {
                u_low_infinite = true;
            } break;
            case 1: {
                u_high_infinite = true;
            } break;
            case 2: {
                v_low_infinite = true;
            } break;
            case 3: {
                v_high_infinite = true;
            } break;
            default: {
                myerror("SPApar_box解析出的区间端点个数大于4。");
            } break;
            }
            mgl_idx++;
            interval_end_idx++;
        } else {
            // finite interval
            assert(interval_type == 2.0);
            double interval_end = mg_value_float(mg_list_at(mgl, mgl_idx + 1));
            switch (interval_end_idx) {
            case 0: {
                u_low = interval_end;
            } break;
            case 1: {
                u_high = interval_end;
            } break;
            case 2: {
                v_low = interval_end;
            } break;
            case 3: {
                v_high = interval_end;
            } break;
            default: {
                myerror("SPApar_box解析出的区间端点个数大于4。");
            } break;
            }
            mgl_idx += 2;
            interval_end_idx++;
        }
    }
    if (u_low_infinite) {
        if (u_high_infinite) {
            if (v_low_infinite) {
                if (v_high_infinite) {
                    return SPApar_box(SPAinterval(interval_type::interval_infinite),
                                      SPAinterval(interval_type::interval_infinite));
                } else {
                    return SPApar_box(SPAinterval(interval_type::interval_infinite),
                                      SPAinterval(interval_type::interval_finite_above, v_low, v_high));
                }
            } else {
                if (v_high_infinite) {
                    return SPApar_box(SPAinterval(interval_type::interval_infinite),
                                      SPAinterval(interval_type::interval_finite_below, v_low, v_high));
                } else {
                    return SPApar_box(SPAinterval(interval_type::interval_infinite),
                                      SPAinterval(interval_type::interval_finite, v_low, v_high));
                }
            }
        } else {
            if (v_low_infinite) {
                if (v_high_infinite) {
                    return SPApar_box(SPAinterval(interval_type::interval_finite_above, u_low, u_high),
                                      SPAinterval(interval_type::interval_infinite));
                } else {
                    return SPApar_box(SPAinterval(interval_type::interval_finite_above, u_low, u_high),
                                      SPAinterval(interval_type::interval_finite_above, v_low, v_high));
                }
            } else {
                if (v_high_infinite) {
                    return SPApar_box(SPAinterval(interval_type::interval_finite_above, u_low, u_high),
                                      SPAinterval(interval_type::interval_finite_below, v_low, v_high));
                } else {
                    return SPApar_box(SPAinterval(interval_type::interval_finite_above, u_low, u_high),
                                      SPAinterval(interval_type::interval_finite, v_low, v_high));
                }
            }
        }
    } else {
        if (u_high_infinite) {
            if (v_low_infinite) {
                if (v_high_infinite) {
                    return SPApar_box(SPAinterval(interval_type::interval_finite_below, u_low, u_high),
                                      SPAinterval(interval_type::interval_infinite));
                } else {
                    return SPApar_box(SPAinterval(interval_type::interval_finite_below, u_low, u_high),
                                      SPAinterval(interval_type::interval_finite_above, v_low, v_high));
                }
            } else {
                if (v_high_infinite) {
                    return SPApar_box(SPAinterval(interval_type::interval_finite_below, u_low, u_high),
                                      SPAinterval(interval_type::interval_finite_below, v_low, v_high));
                } else {
                    return SPApar_box(SPAinterval(interval_type::interval_finite_below, u_low, u_high),
                                      SPAinterval(interval_type::interval_finite, v_low, v_high));
                }
            }
        } else {
            if (v_low_infinite) {
                if (v_high_infinite) {
                    return SPApar_box(SPAinterval(interval_type::interval_finite, u_low, u_high),
                                      SPAinterval(interval_type::interval_infinite));
                } else {
                    return SPApar_box(SPAinterval(interval_type::interval_finite, u_low, u_high),
                                      SPAinterval(interval_type::interval_finite_above, v_low, v_high));
                }
            } else {
                if (v_high_infinite) {
                    return SPApar_box(SPAinterval(interval_type::interval_finite, u_low, u_high),
                                      SPAinterval(interval_type::interval_finite_below, v_low, v_high));
                } else {
                    return SPApar_box(SPAinterval(interval_type::interval_finite, u_low, u_high),
                                      SPAinterval(interval_type::interval_finite, v_low, v_high));
                }
            }
        }
    }
}

SPAvector parsemglist_SPAvector(const mg_list *mgl) {
    return SPAvector(mg_value_float(mg_list_at(mgl, 0)), mg_value_float(mg_list_at(mgl, 1)),
                     mg_value_float(mg_list_at(mgl, 2)));
}

SPAunit_vector parsemglist_SPAunit_vector(const mg_list *mgl) {
    return SPAunit_vector(mg_value_float(mg_list_at(mgl, 0)), mg_value_float(mg_list_at(mgl, 1)),
                          mg_value_float(mg_list_at(mgl, 2)));
}

SPAmatrix parsemglist_SPAmatrix(const mg_list *mgl) {
    SPAvector v1(mg_value_float(mg_list_at(mgl, 0)), mg_value_float(mg_list_at(mgl, 1)),
                 mg_value_float(mg_list_at(mgl, 2)));
    SPAvector v2(mg_value_float(mg_list_at(mgl, 3)), mg_value_float(mg_list_at(mgl, 4)),
                 mg_value_float(mg_list_at(mgl, 5)));
    SPAvector v3(mg_value_float(mg_list_at(mgl, 6)), mg_value_float(mg_list_at(mgl, 7)),
                 mg_value_float(mg_list_at(mgl, 8)));
    return SPAmatrix(v1, v2, v3);
}
} // namespace Restore
} // namespace AccessUtils
