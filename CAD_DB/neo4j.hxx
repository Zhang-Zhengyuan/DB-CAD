#pragma once
#include <string>
#include <mgclient-1.4.2/mgclient.h>

class Neo4jPart {
public:
    mg_session* session = nullptr;
    std::string partname;
    Neo4jPart(const char* host, int port_bolt, const char* un, const char* pw, const std::string& pn);
    void execute_bolt(const char* statement, const mg_map* parameters) const;
    void discard_all_results() const;

    ~Neo4jPart() {
        if (session) {
            mg_session_destroy(session);
            session = nullptr;
        }
    }

    Neo4jPart(const Neo4jPart&) = delete;
    Neo4jPart& operator=(const Neo4jPart&) = delete;
};