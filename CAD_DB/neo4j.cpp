#include "common.hxx"
#include "neo4j.hxx"
#include <format>
#include <stdexcept>

Neo4jPart::Neo4jPart(const char* host, int port_bolt, const char* un, const char* pw,
                     const std::string& pn) : partname(pn)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    mg_session_params* params = mg_session_params_make();
    mg_session_params_set_host(params, host);
    mg_session_params_set_port(params, (uint16_t)port_bolt);
    mg_session_params_set_username(params, un ? un : "");
    mg_session_params_set_password(params, pw ? pw : "");
    mg_session_params_set_sslmode(params, MG_SSLMODE_DISABLE);
    //若要启用SSL功能，请换成这句：mg_session_params_set_sslmode(params, MG_SSLMODE_REQUIRE);
    int status = mg_connect(params, &session);
    mg_session_params_destroy(params);
    if (status < 0) {
        const char* err = session ? mg_session_error(session) : "unknown error";
        const std::string errMsg = err ? err : "unknown error";
        if (session) {
            mg_session_destroy(session);
            session = nullptr;
        }
        throw std::runtime_error(std::format("Neo4j数据库连接失败：{}", errMsg));
    }
}

void Neo4jPart::execute_bolt(const char* statement, const mg_map* parameters) const {
    if (!session) {
        throw std::runtime_error("尝试在无效的会话上执行Bolt查询");
    }
    if (mg_session_run(session, statement, parameters, NULL, NULL, NULL) < 0) {
        const char* err = mg_session_error(session);
        const std::string errMsg = err ? err : "unknown error";
        if (err) {
            FILE* f = fopen("error.log", "w");
            if (f) {
                fprintf(f, "%s\n", err);
                fclose(f);
                printf("Error saved to error.log\n");
            }
        }
        if (session) {
            mg_session_destroy(session);
            session = nullptr;
        }
        throw std::runtime_error(std::format("执行Cypher语句失败：{}", errMsg));
    }
    if (mg_session_pull(session, NULL) < 0) {
        const char* err = mg_session_error(session);
        const std::string errMsg = err ? err : "unknown error";
        if (session) {
            mg_session_destroy(session);
            session = nullptr;
        }
        throw std::runtime_error(std::format("拉取Cypher语句执行结果失败：{}", errMsg));
    }
}

void Neo4jPart::discard_all_results() const {
    if (!session) {
        return;
    }
    mg_result* result;
    int status;
    int count = 0;
    while (1)
    {
        status = mg_session_fetch(session, &result);
        count++;
        if (status == 0)
        {
            // 正常退出
            break;
        } else if (status != 1) {
            // 错误，但 session 可能已损坏
            std::fprintf(stderr, "[Neo4jPart discard_all_results] mg_session_fetch returned %d after %d iterations, error: %s\n",
                         status, count, mg_session_error(session) ? mg_session_error(session) : "unknown");
            // 不再 throw，避免 abort
            return;
        }
        // status == 1: 成功获取一行结果，继续循环
        if (count > 100000) {
            std::fprintf(stderr, "[Neo4jPart discard_all_results] WARNING: too many results (>100000), breaking\n");
            return;
        }
    }
}
