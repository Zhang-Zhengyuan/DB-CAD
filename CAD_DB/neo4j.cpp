#include "common.hxx"
#include "neo4j.hxx"
#include <QtWidgets/QApplication>
#include <format>
#include <stdexcept>

Neo4jPart::Neo4jPart(const char* host, int port_bolt, const char* un, const char* pw, const std::string& pn) :partname(pn) {
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
        if (session) {
            mg_session_destroy(session);
            session = nullptr;
        }
        throw std::runtime_error(std::format("Neo4j数据库连接失败：{}", err));
    }
}

void Neo4jPart::execute_bolt(const char* statement, const mg_map* parameters) const {
    if (!session) {
        throw std::runtime_error("尝试在无效的会话上执行Bolt查询");
    }
    if (mg_session_run(session, statement, parameters, NULL, NULL, NULL) < 0) {
        const char* err = mg_session_error(session);
        if (err) {
            FILE* f = fopen("error.log", "w");
            fprintf(f, "%s\n", err);
            fclose(f);
            printf("Error saved to error.log\n");
        }
        if (session) {
            mg_session_destroy(session);
        }
        throw std::runtime_error(std::format("执行Cypher语句失败：{}", err ? err : "unknown error"));
    }
    if (mg_session_pull(session, NULL) < 0) {
        const char* err = mg_session_error(session);
        if (session) {
            mg_session_destroy(session);
        }
        throw std::runtime_error(std::format("拉取Cypher语句执行结果失败：{}", err ? err : "unknown error"));
    }
}

void Neo4jPart::discard_all_results() const {
    mg_result* result;
    int status;
    while (1) {
        status = mg_session_fetch(session, &result);
        if (status == 0) {
            break;
        } else if (status != 1) {
            throw std::runtime_error(std::format("mg_session_fetch失败，错误信息如下：{}", mg_session_error(session)));
        }
    }
}