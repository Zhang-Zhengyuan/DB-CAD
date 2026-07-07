#pragma once

#include <optional>
#include <string>
#include <vector>

namespace dbcad::pg_demo {

struct PgConfig {
    std::string host = "127.0.0.1";
    int port = 5432;
    std::string user = "postgres";
    std::string password;
    std::string dbname = "dbcad_demo";
};

struct StoredPart {
    long long id = 0;
    std::string name;
    long long size_bytes = 0;
};

class PgStore {
public:
    explicit PgStore(const PgConfig& cfg);
    ~PgStore();

    PgStore(const PgStore&) = delete;
    PgStore& operator=(const PgStore&) = delete;

    bool ensureSchema(std::string& error);

    std::optional<StoredPart> saveSat(const std::string& name,
                                     const std::string& sat_text,
                                     std::string& error);

    std::optional<std::string> loadSat(const std::string& name,
                                       std::string& error);

    std::optional<long long> deleteByName(const std::string& name,
                                          std::string& error);

    long long countParts(std::string& error);

    struct PartInfo {
        long long id = 0;
        std::string name;
        long long size_bytes = 0;
        std::string updated_at;  // ISO-8601 from Postgres
    };

    // Returns up to `limit` most-recent parts ordered by updated_at desc.
    // Empty optional on error.
    std::optional<std::vector<PartInfo>> listParts(int limit,
                                                   std::string& error);

private:
    struct Impl;
    Impl* m_impl;
};

std::string readFileText(const std::string& path, std::string& error);
bool writeFileText(const std::string& path,
                   const std::string& content,
                   std::string& error);

}  // namespace dbcad::pg_demo
