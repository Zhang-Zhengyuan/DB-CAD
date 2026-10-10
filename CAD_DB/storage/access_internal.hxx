#pragma once
// Private graph codecs and dependencies; public entry points remain in access.hxx.
#include <deque>
#include <functional>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <filesystem>

#include "common.hxx"
#include "neo4j.hxx"
#include "access.hxx"
#include <acis/include/allcurve.hxx>
#include <acis/include/allsurf.hxx>
#include <acis/include/alltop.hxx>
#include <glaze/glaze.hpp>
#include <acis/include/api.hxx>
#include <acis/include/cstrapi.hxx>
#include <acis/include/exct_int.hxx>
#include <acis/include/exct_spl.hxx>
#include <acis/include/exp_par.hxx>
#include <acis/include/fileinfo.hxx>
#include <acis/include/kernapi.hxx>
#include <acis/include/int_int.hxx>
#include <acis/include/par_int.hxx>
#include <acis/include/pcurve.hxx>
#include <acis/include/point.hxx>
#include <acis/include/sps2crtn.hxx>
#include <acis/include/sps3crtn.hxx>

#include <acis/include/sp3srtn.hxx>
#include <acis/include/sps3srtn.hxx>
#include <acis/include/surf_int.hxx>
#include <acis/include/transfrm.hxx>
#include <acis/include/bulletin.hxx>

#include <atomic>
#include <cstdio>
#include <cstring>
#include <mutex>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

// 把诊断日志同时写到 stderr 和 CAD_DB/log/bridge.err.log，方便跨进程收集
namespace diaglog {
inline std::mutex g_log_mutex;
inline const char *kLogDir = "CAD_DB/log";
inline const char *kLogPath = "CAD_DB/log/bridge.err.log";
inline void ensure_dir() {
#ifdef _WIN32
    _mkdir("CAD_DB");
    _mkdir("CAD_DB/log");
#else
    mkdir("CAD_DB", 0755);
    mkdir("CAD_DB/log", 0755);
#endif
}
inline void write(const char *msg) {
    std::lock_guard<std::mutex> lock(g_log_mutex);
    std::fputs(msg, stderr);
    std::fflush(stderr);
    static bool dir_checked = false;
    if (!dir_checked) {
        ensure_dir();
        dir_checked = true;
    }
    FILE *fp = std::fopen(kLogPath, "a");
    if (fp != nullptr) {
        std::fputs(msg, fp);
        std::fclose(fp);
    }
}
inline void clear_log() {
    ensure_dir();
    FILE *fp = std::fopen(kLogPath, "w");
    if (fp != nullptr)
        std::fclose(fp);
}
} // namespace diaglog
#define DIAG_LOG(...)                                                                                                  \
    do {                                                                                                               \
        char buf[1024];                                                                                                \
        std::snprintf(buf, sizeof(buf), __VA_ARGS__);                                                                  \
        diaglog::write(buf);                                                                                           \
    } while (0)

// 生成一个 mode1 delta 用的 SAT 临时文件路径。
// 用 std::filesystem::temp_directory_path() + 进程内单调递增计数，避免 tmpnam_s 的安全性警告。
namespace mode1_temp {
inline std::atomic<int> g_seq{0};
inline std::string make_delta_temp_path(const char *tag) {
    int idx = g_seq.fetch_add(1);
    namespace fs = std::filesystem;
    std::error_code ec;
    fs::path dir = fs::temp_directory_path(ec);
    if (ec)
        dir = fs::current_path();
    std::string name = std::string(tag) + "_" + std::to_string(idx) + ".sat";
    return (dir / name).string();
}
} // namespace mode1_temp

class Node {
  public:
    int64_t id;
    std::vector<std::string> labels; // 存 C++ std::string，析构安全
    std::unordered_map<char, mg_value *> properties;

    Node() {}
};

class Relationship {
  public:
    Node *u;
    Node *v;
    std::string label; // 存 C++ std::string，析构安全
    std::unordered_map<char, mg_value *> properties;

    Relationship(Node *u1, Node *v1) : u(u1), v(v1) {}
};

class Relationship2 {
  public:
    int64_t uid;
    int64_t vid;
    std::string label; // 存 C++ std::string，析构安全
    std::unordered_map<char, mg_value *> properties;

    Relationship2(int64_t uid1, int64_t vid1) : uid(uid1), vid(vid1) {}
};

// -------------------------------------------------------------------------------------------------------
// ! 宏定义
#ifdef _MSC_VER
// 用于宏拼接
#define STR(a) STR2(a)
#define STR2(a) #a
#define CAT(a, b) CAT_I(a, b)
#define CAT_I(a, b) CAT_II(~, a##b)
#define CAT_II(_, res) res
#define MSVC_EXPAND(...) __VA_ARGS__
#define _GET_MACRO_ARGS_COUNT_KERN(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, __COUNT, ...) \
    __COUNT
#define GET_MACRO_ARGS_COUNT(...)                                                                                      \
    MSVC_EXPAND(_GET_MACRO_ARGS_COUNT_KERN(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))
#define ITERATE_MACRO_WITH_PARAM0(__macro, ...)                                                                        \
    MSVC_EXPAND(CAT(__macro##_, GET_MACRO_ARGS_COUNT(__VA_ARGS__))(__VA_ARGS__))
#define ITERATE_MACRO_WITH_PARAM1(__macro, __param1, ...)                                                              \
    MSVC_EXPAND(CAT(__macro##_, GET_MACRO_ARGS_COUNT(__VA_ARGS__))(__param1, __VA_ARGS__))
#define ITERATE_MACRO_WITH_PARAM2(__macro, __param1, __param2, ...)                                                    \
    MSVC_EXPAND(CAT(__macro##_, GET_MACRO_ARGS_COUNT(__VA_ARGS__))(__param1, __param2, __VA_ARGS__))
#endif

// 更新实体指针记录的迭代宏(neo4j)
#define _API_PUSH_PTR_NEO4J_SUBGRAPH_1(ptr, entity_label, _1) _API_PUSH_PTR_NEO4J_SUBGRAPH(ptr, entity_label, _1)
#define _API_PUSH_PTR_NEO4J_SUBGRAPH_2(ptr, entity_label, _1, _2)                                                      \
    _API_PUSH_PTR_NEO4J_SUBGRAPH_1(ptr, entity_label, _1) _API_PUSH_PTR_NEO4J_SUBGRAPH(ptr, entity_label, _2)
#define _API_PUSH_PTR_NEO4J_SUBGRAPH_3(ptr, entity_label, _1, _2, _3)                                                  \
    _API_PUSH_PTR_NEO4J_SUBGRAPH_2(ptr, entity_label, _1, _2) _API_PUSH_PTR_NEO4J_SUBGRAPH(ptr, entity_label, _3)
#define _API_PUSH_PTR_NEO4J_SUBGRAPH_4(ptr, entity_label, _1, _2, _3, _4)                                              \
    _API_PUSH_PTR_NEO4J_SUBGRAPH_3(ptr, entity_label, _1, _2, _3) _API_PUSH_PTR_NEO4J_SUBGRAPH(ptr, entity_label, _4)
#define _API_PUSH_PTR_NEO4J_SUBGRAPH_5(ptr, entity_label, _1, _2, _3, _4, _5)                                          \
    _API_PUSH_PTR_NEO4J_SUBGRAPH_4(ptr, entity_label, _1, _2, _3, _4)                                                  \
    _API_PUSH_PTR_NEO4J_SUBGRAPH(ptr, entity_label, _5)
#define _API_PUSH_PTR_NEO4J_SUBGRAPH_6(ptr, entity_label, _1, _2, _3, _4, _5, _6)                                      \
    _API_PUSH_PTR_NEO4J_SUBGRAPH_5(ptr, entity_label, _1, _2, _3, _4, _5)                                              \
    _API_PUSH_PTR_NEO4J_SUBGRAPH(ptr, entity_label, _6)
#define _API_PUSH_PTR_NEO4J_SUBGRAPH_7(ptr, entity_label, _1, _2, _3, _4, _5, _6, _7)                                  \
    _API_PUSH_PTR_NEO4J_SUBGRAPH_6(ptr, entity_label, _1, _2, _3, _4, _5, _6)                                          \
    _API_PUSH_PTR_NEO4J_SUBGRAPH(ptr, entity_label, _7)
#define _API_PUSH_PTR_NEO4J_SUBGRAPH_8(ptr, entity_label, _1, _2, _3, _4, _5, _6, _7, _8)                              \
    _API_PUSH_PTR_NEO4J_SUBGRAPH_7(ptr, entity_label, _1, _2, _3, _4, _5, _6, _7)                                      \
    _API_PUSH_PTR_NEO4J_SUBGRAPH(ptr, entity_label, _8)
#define _API_PUSH_PTR_NEO4J_SUBGRAPH(__ptr, __entity_label, __member_func_name)                                        \
    {                                                                                                                  \
        ENTITY *__tmp_ptr = __ptr->__member_func_name();                                                               \
        if (__tmp_ptr != nullptr) {                                                                                    \
            if (ptr2node.find(__tmp_ptr) == ptr2node.end()) {                                                          \
                Node *a = ptr2node.at(__ptr);                                                                          \
                ptr2node[__tmp_ptr] = new Node();                                                                      \
                Relationship *r = new Relationship(a, ptr2node[__tmp_ptr]);                                            \
                r->label = STR(CAT(__entity_label##_, __member_func_name##_ptr));                                      \
                r->properties['a'] = mg_value_make_string(STR(CAT(__entity_label##_, __member_func_name##_ptr)));      \
                relationship_list.push_back(r);                                                                        \
                que.push_back(__tmp_ptr);                                                                              \
            } else {                                                                                                   \
                Node *a = ptr2node.at(__ptr);                                                                          \
                Node *b = ptr2node.at(__tmp_ptr);                                                                      \
                Relationship *r = new Relationship(a, b);                                                              \
                r->label = STR(CAT(__entity_label##_, __member_func_name##_ptr));                                      \
                r->properties['a'] = mg_value_make_string(STR(CAT(__entity_label##_, __member_func_name##_ptr)));      \
                relationship_list.push_back(r);                                                                        \
            }                                                                                                          \
        }                                                                                                              \
    }

// -------------------------------------------------------------------------------------------------------

#include "access_save_codec.hxx"
#include "access_restore_codec.hxx"

// Internal lookup shared by legacy delta Push and Pull.
int read_part_node_version(const Neo4jPart &conn, const std::string &partname);
std::string get_body_node_uuid(const Neo4jPart &conn, int64_t body_node_id);
