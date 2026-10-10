#include "storage_bridge_internal.h"

namespace dbcad::bridge {

QString nowIsoUtc() {
    return QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs);
}

QByteArray jsonError(const QString &detail) {
    QJsonObject body;
    body.insert("detail", detail);
    return QJsonDocument(body).toJson(QJsonDocument::Compact);
}

QByteArray asUtf8(const mg_string *value) {
    if (value == nullptr) {
        return {};
    }
    return QByteArray(value->data, static_cast<int>(value->size));
}

QString toQString(const QueryValue &value) {
    if (value.type != MG_VALUE_TYPE_STRING) {
        return {};
    }
    return value.stringValue;
}

int64_t toInt64(const QueryValue &value, bool *ok) {
    if (ok != nullptr) {
        *ok = false;
    }
    if (value.type != MG_VALUE_TYPE_INTEGER) {
        return 0;
    }
    if (ok != nullptr) {
        *ok = true;
    }
    return value.intValue;
}

std::vector<std::vector<QueryValue>> runQuery(const QString &host, int port, const QString &user,
                                              const QString &password, const QString &statement, mg_map *params,
                                              QString &error) {
    std::vector<std::vector<QueryValue>> rows;

    try {
        Neo4jPart conn(host.toStdString().c_str(), port, user.toStdString().c_str(), password.toStdString().c_str(),
                       "");

        conn.execute_bolt(statement.toUtf8().constData(), params);

        mg_result *result = nullptr;
        while (true) {
            const int status = mg_session_fetch(conn.session, &result);
            if (status == 0) {
                break;
            }
            if (status < 0) {
                error = QString::fromUtf8(mg_session_error(conn.session));
                rows.clear();
                return rows;
            }

            const mg_list *row = mg_result_row(result);
            if (row == nullptr) {
                continue;
            }

            const uint32_t rowSize = mg_list_size(row);
            std::vector<QueryValue> values;
            values.reserve(rowSize);
            for (uint32_t i = 0; i < rowSize; ++i) {
                const mg_value *raw = mg_list_at(row, i);
                QueryValue parsed;
                parsed.type = raw ? mg_value_get_type(raw) : MG_VALUE_TYPE_NULL;
                if (parsed.type == MG_VALUE_TYPE_STRING) {
                    parsed.stringValue = QString::fromUtf8(asUtf8(mg_value_string(raw)));
                } else if (parsed.type == MG_VALUE_TYPE_INTEGER) {
                    parsed.intValue = mg_value_integer(raw);
                }
                values.push_back(std::move(parsed));
            }
            rows.push_back(std::move(values));
        }
    } catch (const std::exception &ex) {
        error = QString::fromUtf8(ex.what());
    }

    return rows;
}

bool saveSatToPart(const QString &host, int port, const QString &user, const QString &password, const QString &partName,
                   const QString &sat, QString &error) {
    FILE *satStream = nullptr;
#ifdef _WIN32
    if (tmpfile_s(&satStream) != 0 || satStream == nullptr) {
        error = QString::fromUtf8("Failed to create temporary SAT stream");
        return false;
    }
#else
    satStream = tmpfile();
    if (satStream == nullptr) {
        error = QString::fromUtf8("Failed to create temporary SAT stream");
        return false;
    }
#endif

    const auto closeSatStream = [&]() {
        if (satStream != nullptr) {
            fclose(satStream);
            satStream = nullptr;
        }
    };

    const QByteArray satBytes = sat.toUtf8();
    if (satBytes.isEmpty() || fwrite(satBytes.constData(), 1, static_cast<size_t>(satBytes.size()), satStream) !=
                                  static_cast<size_t>(satBytes.size())) {
        error = QString::fromUtf8("Failed to write SAT content to temporary stream");
        closeSatStream();
        return false;
    }
    if (fflush(satStream) != 0 || fseek(satStream, 0, SEEK_SET) != 0) {
        error = QString::fromUtf8("Failed to rewind temporary SAT stream");
        closeSatStream();
        return false;
    }

    ENTITY_LIST entityList;

    try {
        acis_restore_entity_list(entityList, satStream, 2, 0, true);
        std::fprintf(stderr, "[storage_bridge saveSatToPart] partName=%s satBytes=%d entityListCount=%d\n",
                     partName.toStdString().c_str(), satBytes.size(), entityList.count());
        if (entityList.count() == 0) {
            fprintf(
                stderr,
                "[storage_bridge saveSatToPart] WARNING: empty entityList, dumping first 200 chars of SAT:\n%.200s\n",
                satBytes.constData());
        }
        Neo4jPart conn(host.toStdString().c_str(), port, user.toStdString().c_str(), password.toStdString().c_str(),
                       partName.toStdString());
        api_save_entity_list_neo4j_part(conn, entityList);

        // 兜底：把原始 SAT 文本也存到 Part 节点的 sat_text 属性上。
        // loadSatFromPart 会优先读这个字段，避免依赖 storage_bridge 子进程里的 ACIS round-trip。
        {
            mg_map *satParams = mg_map_make_empty(2);
            mg_map_append(satParams, mg_string_make("Y"), mg_value_make_string(partName.toUtf8().constData()));
            mg_map_append(satParams, mg_string_make("S"), mg_value_make_string(satBytes.constData()));
            conn.execute_bolt("MATCH (n:part {b:$Y}) SET n.sat_text = $S", satParams);
            mg_map_destroy(satParams);
            conn.discard_all_results();
        }
    } catch (const std::exception &ex) {
        error = QString::fromUtf8(ex.what());
        closeSatStream();
        return false;
    }

    closeSatStream();
    return true;
}

bool loadSatFromPart(const QString &host, int port, const QString &user, const QString &password,
                     const QString &partName, QString &sat, QString &error) {
    FILE *satStream = nullptr;
#ifdef _WIN32
    if (tmpfile_s(&satStream) != 0 || satStream == nullptr) {
        error = QString::fromUtf8("Failed to create temporary SAT stream");
        return false;
    }
#else
    satStream = tmpfile();
    if (satStream == nullptr) {
        error = QString::fromUtf8("Failed to create temporary SAT stream");
        return false;
    }
#endif

    const auto closeSatStream = [&]() {
        if (satStream != nullptr) {
            fclose(satStream);
            satStream = nullptr;
        }
    };

    try {
        Neo4jPart conn(host.toStdString().c_str(), port, user.toStdString().c_str(), password.toStdString().c_str(),
                       partName.toStdString());

        if (count_partnode(conn) <= 0) {
            error = QString::fromUtf8("Model version not found");
            std::fprintf(stderr, "[storage_bridge loadSatFromPart] partName=%s partNodeNotFound\n",
                         partName.toStdString().c_str());
            return false;
        }

        // 优先读 Part 节点上的 sat_text 兜底属性（saveSatToPart 写入）。
        // 如果存在就直接返回原始 SAT，避免 ACIS round-trip 在 storage_bridge 子进程里丢数据。
        {
            mg_map *satParams = mg_map_make_empty(1);
            mg_map_append(satParams, mg_string_make("Y"), mg_value_make_string(partName.toUtf8().constData()));
            conn.execute_bolt("MATCH (n:part {b:$Y}) RETURN n.sat_text", satParams);
            mg_map_destroy(satParams);

            mg_result *satResult = nullptr;
            bool satTextUsed = false;
            while (mg_session_fetch(conn.session, &satResult) == 1) {
                const mg_list *row = mg_result_row(satResult);
                if (row == nullptr || mg_list_size(row) < 1)
                    continue;
                const mg_value *v = mg_list_at(row, 0);
                if (v == nullptr || mg_value_get_type(v) != MG_VALUE_TYPE_STRING)
                    continue;
                const QByteArray raw = asUtf8(mg_value_string(v));
                if (raw.isEmpty())
                    continue;
                sat = QString::fromUtf8(raw);
                satTextUsed = true;
                std::fprintf(stderr, "[storage_bridge loadSatFromPart] partName=%s sat_text present, satLen=%d\n",
                             partName.toStdString().c_str(), raw.size());
                break;
            }
            if (satTextUsed) {
                closeSatStream();
                return true;
            }
            std::fprintf(
                stderr,
                "[storage_bridge loadSatFromPart] partName=%s sat_text missing, falling back to ACIS round-trip\n",
                partName.toStdString().c_str());
        }

        ENTITY_LIST entityList;
        api_restore_entity_list_neo4j_part(conn, entityList);
        std::fprintf(stderr, "[storage_bridge loadSatFromPart] partName=%s entityListCount=%d\n",
                     partName.toStdString().c_str(), entityList.count());
        acis_save_entity_list(satStream, entityList, 2, 0, true);
        std::fprintf(stderr, "[storage_bridge loadSatFromPart] partName=%s acis_save_entity_list done\n",
                     partName.toStdString().c_str());
    } catch (const std::exception &ex) {
        error = QString::fromUtf8(ex.what());
        closeSatStream();
        return false;
    }

    if (fflush(satStream) != 0 || fseek(satStream, 0, SEEK_SET) != 0) {
        error = QString::fromUtf8("Failed to rewind generated SAT stream");
        closeSatStream();
        return false;
    }

    QByteArray satBytes;
    char buffer[8192];
    while (true) {
        const size_t n = fread(buffer, 1, sizeof(buffer), satStream);
        if (n > 0) {
            satBytes.append(buffer, static_cast<int>(n));
        }
        if (n < sizeof(buffer)) {
            if (feof(satStream)) {
                break;
            }
            if (ferror(satStream)) {
                error = QString::fromUtf8("Failed to read generated SAT stream");
                closeSatStream();
                return false;
            }
        }
    }

    sat = QString::fromUtf8(satBytes);
    closeSatStream();
    return true;
}

int parsePositiveInt(const QString &value, int fallback) {
    bool ok = false;
    const int parsed = value.toInt(&ok);
    if (!ok || parsed <= 0) {
        return fallback;
    }
    return parsed;
}
} // namespace dbcad::bridge
