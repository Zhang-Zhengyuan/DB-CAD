// Lightweight PostgreSQL adapter for the DBCAD demo.
// Uses libpq directly (no libpqxx) so we keep the dep footprint tiny and
// avoid cross-DLL CRT/runtime mismatch issues on Windows.
//
// API mirrors the small surface area we previously expressed with pqxx.

#include "pg_store.hxx"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef _WIN32
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <winsock2.h>
#endif

#include <libpq-fe.h>

#include <fstream>
#include <iostream>
#include <sstream>

namespace dbcad::pg_demo {

struct PgStore::Impl {
    PgConfig cfg;
    PGconn* conn = nullptr;

    std::string conninfo() const {
        std::ostringstream oss;
        oss << "host=" << cfg.host
            << " port=" << cfg.port
            << " user=" << cfg.user
            << " password=" << cfg.password
            << " dbname=" << cfg.dbname
            << " connect_timeout=5";
        return oss.str();
    }

    bool reconnect(std::string& error) {
        if (conn) { PQfinish(conn); conn = nullptr; }
        conn = PQconnectdb(conninfo().c_str());
        if (!conn) {
            error = "PQconnectdb returned null";
            return false;
        }
        if (PQstatus(conn) != CONNECTION_OK) {
            error = PQerrorMessage(conn);
            PQfinish(conn);
            conn = nullptr;
            return false;
        }
        return true;
    }

    void close() {
        if (conn) { PQfinish(conn); conn = nullptr; }
    }

    // Each SQL exec should be inside this guard: PGresult is always cleared.
    struct Result {
        PGresult* r = nullptr;
        ~Result() { if (r) PQclear(r); }
    };

    bool exec(const char* sql, std::string& error, int want = 1 /* PGRES_COMMAND_OK */) {
        Result guard;
        guard.r = PQexec(conn, sql);
        if (!guard.r) { error = "PQexec returned null"; return false; }
        const int st = PQresultStatus(guard.r);
        if (st != want) {
            error = PQerrorMessage(conn);
            return false;
        }
        return true;
    }
};

PgStore::PgStore(const PgConfig& cfg) : m_impl(new Impl()) {
    m_impl->cfg = cfg;
}

PgStore::~PgStore() {
    if (m_impl) {
        m_impl->close();
        delete m_impl;
    }
}

bool PgStore::ensureSchema(std::string& error) {
    if (!m_impl->reconnect(error)) return false;

    const char* sql =
        "CREATE TABLE IF NOT EXISTS dbcad_pg_demo ("
        " id BIGSERIAL PRIMARY KEY,"
        " name TEXT NOT NULL UNIQUE,"
        " sat_text TEXT NOT NULL,"
        " byte_size INTEGER NOT NULL,"
        " source_label TEXT NOT NULL DEFAULT 'unknown',"
        " created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),"
        " updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW())";

    return m_impl->exec(sql, error);
}

std::optional<StoredPart> PgStore::saveSat(const std::string& name,
                                           const std::string& sat_text,
                                           std::string& error) {
    if (!m_impl->conn) {
        if (!m_impl->reconnect(error)) return std::nullopt;
    }

    const int byte_size = static_cast<int>(sat_text.size());
    const std::string byte_size_str = std::to_string(byte_size);
    const char* params[4] = {
        name.c_str(),
        sat_text.c_str(),
        "demo_cpp_pg_mini",
        byte_size_str.c_str()
    };
    const int lengths[4] = {
        0,
        static_cast<int>(sat_text.size()),
        0,
        0  // text-format param: libpq reads strlen(paramValue)
    };
    const int formats[4] = {0, 0, 0, 0};  // all text — keeps it portable

    Impl::Result guard;
    guard.r = PQexecParams(m_impl->conn,
        "INSERT INTO dbcad_pg_demo (name, sat_text, source_label, byte_size) "
        "VALUES ($1, $2, $3, $4) "
        "ON CONFLICT (name) DO UPDATE SET "
        "  sat_text = EXCLUDED.sat_text, "
        "  byte_size = EXCLUDED.byte_size, "
        "  updated_at = NOW() "
        "RETURNING id, name, byte_size",
        4, nullptr, params, lengths, formats, 0);

    if (!guard.r) { error = "PQexecParams returned null"; return std::nullopt; }
    if (PQresultStatus(guard.r) != PGRES_TUPLES_OK) {
        error = PQerrorMessage(m_impl->conn);
        return std::nullopt;
    }

    StoredPart sp;
    sp.id = std::strtoll(PQgetvalue(guard.r, 0, 0), nullptr, 10);
    sp.name = PQgetvalue(guard.r, 0, 1);
    sp.size_bytes = std::strtoll(PQgetvalue(guard.r, 0, 2), nullptr, 10);
    return sp;
}

std::optional<std::string> PgStore::loadSat(const std::string& name,
                                             std::string& error) {
    if (!m_impl->conn) {
        if (!m_impl->reconnect(error)) return std::nullopt;
    }

    const char* params[1] = { name.c_str() };

    Impl::Result guard;
    guard.r = PQexecParams(m_impl->conn,
        "SELECT sat_text FROM dbcad_pg_demo WHERE name = $1",
        1, nullptr, params, nullptr, nullptr, 0);

    if (!guard.r) { error = "PQexecParams returned null"; return std::nullopt; }
    if (PQresultStatus(guard.r) != PGRES_TUPLES_OK) {
        error = PQerrorMessage(m_impl->conn);
        return std::nullopt;
    }
    if (PQntuples(guard.r) == 0) {
        return std::nullopt;  // not found
    }
    return std::string(PQgetvalue(guard.r, 0, 0),
                       PQgetlength(guard.r, 0, 0));
}

std::optional<long long> PgStore::deleteByName(const std::string& name,
                                                std::string& error) {
    if (!m_impl->conn) {
        if (!m_impl->reconnect(error)) return std::nullopt;
    }

    const char* params[1] = { name.c_str() };

    Impl::Result guard;
    guard.r = PQexecParams(m_impl->conn,
        "DELETE FROM dbcad_pg_demo WHERE name = $1 RETURNING id",
        1, nullptr, params, nullptr, nullptr, 0);

    if (!guard.r) { error = "PQexecParams returned null"; return std::nullopt; }
    if (PQresultStatus(guard.r) != PGRES_TUPLES_OK) {
        error = PQerrorMessage(m_impl->conn);
        return std::nullopt;
    }
    if (PQntuples(guard.r) == 0) return std::nullopt;
    return std::strtoll(PQgetvalue(guard.r, 0, 0), nullptr, 10);
}

long long PgStore::countParts(std::string& error) {
    if (!m_impl->conn) {
        if (!m_impl->reconnect(error)) return -1;
    }
    Impl::Result guard;
    guard.r = PQexec(m_impl->conn, "SELECT COUNT(*) FROM dbcad_pg_demo");
    if (!guard.r) { error = "PQexec returned null"; return -1; }
    if (PQresultStatus(guard.r) != PGRES_TUPLES_OK) {
        error = PQerrorMessage(m_impl->conn);
        return -1;
    }
    return std::strtoll(PQgetvalue(guard.r, 0, 0), nullptr, 10);
}

std::optional<std::vector<dbcad::pg_demo::PgStore::PartInfo>>
dbcad::pg_demo::PgStore::listParts(int limit, std::string& error) {
    if (!m_impl->conn) {
        if (!m_impl->reconnect(error)) return std::nullopt;
    }

    const std::string sql =
        "SELECT id, name, byte_size, "
        "       to_char(updated_at, 'YYYY-MM-DD HH24:MI:SS') "
        "FROM dbcad_pg_demo "
        "ORDER BY updated_at DESC "
        "LIMIT " + std::to_string(limit);

    Impl::Result guard;
    guard.r = PQexec(m_impl->conn, sql.c_str());
    if (!guard.r) { error = "PQexec returned null"; return std::nullopt; }
    if (PQresultStatus(guard.r) != PGRES_TUPLES_OK) {
        error = PQerrorMessage(m_impl->conn);
        return std::nullopt;
    }

    std::vector<PartInfo> out;
    const int rows = PQntuples(guard.r);
    out.reserve(rows);
    for (int i = 0; i < rows; ++i) {
        PartInfo p;
        p.id = std::strtoll(PQgetvalue(guard.r, i, 0), nullptr, 10);
        p.name = PQgetvalue(guard.r, i, 1);
        p.size_bytes = std::strtoll(PQgetvalue(guard.r, i, 2), nullptr, 10);
        p.updated_at = PQgetvalue(guard.r, i, 3);
        out.push_back(std::move(p));
    }
    return out;
}

std::string readFileText(const std::string& path, std::string& error) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        error = "cannot open file: " + path;
        return {};
    }
    std::ostringstream oss;
    oss << in.rdbuf();
    return oss.str();
}

bool writeFileText(const std::string& path,
                   const std::string& content,
                   std::string& error) {
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    if (!out) {
        error = "cannot open file for write: " + path;
        return false;
    }
    out.write(content.data(), static_cast<std::streamsize>(content.size()));
    if (!out) {
        error = "write failed: " + path;
        return false;
    }
    return true;
}

}  // namespace dbcad::pg_demo