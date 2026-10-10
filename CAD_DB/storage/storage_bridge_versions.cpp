#include "storage_bridge_internal.h"

using namespace dbcad::bridge;

QByteArray StorageBridgeService::handleGetLatestModel(const QString &projectId, int &statusCode, QString &error) {
    statusCode = 200;

    mg_map *params = mg_map_make_empty(1);
    mg_map_append(params, mg_string_make("project_id"), mg_value_make_string(projectId.toUtf8().constData()));

    QString dbError;
    const auto rows = runQuery(neo4jHost, neo4jPort, neo4jUser, neo4jPassword,
                               "MATCH (p:BridgeProject {id:$project_id})-[:HAS_VERSION]->(v:BridgeVersion) "
                               "RETURN id(v),v.project_id,v.version,v.author,v.created_at,v.part_name,v.content_text "
                               "ORDER BY v.version DESC LIMIT 1",
                               params, dbError);
    mg_map_destroy(params);

    if (!dbError.isEmpty()) {
        statusCode = 500;
        error = dbError;
        return {};
    }
    if (rows.empty() || rows.front().size() < 6) {
        statusCode = 404;
        error = QString::fromUtf8("No model version found");
        return {};
    }

    QString sat;
    const QString partName = toQString(rows.front()[5]);
    if (!loadSatFromPart(neo4jHost, neo4jPort, neo4jUser, neo4jPassword, partName, sat, dbError)) {
        statusCode = 500;
        error = dbError;
        return {};
    }

    QJsonObject response;
    response.insert("id", static_cast<double>(toInt64(rows.front()[0])));
    response.insert("project_id", toQString(rows.front()[1]));
    response.insert("version", static_cast<int>(toInt64(rows.front()[2])));
    response.insert("author", toQString(rows.front()[3]));
    response.insert("created_at", toQString(rows.front()[4]));
    // 从 BridgeVersion.content_text 反序列化回完整 content（包含 entity_graph / changes / sat_format）。
    // 旧数据可能没有 content_text 字段，回退到只塞 sat（兼容老版本）。
    QJsonObject content;
    if (rows.front().size() >= 7) {
        const QString contentText = toQString(rows.front()[6]);
        if (!contentText.isEmpty()) {
            const QJsonDocument contentDoc = QJsonDocument::fromJson(contentText.toUtf8());
            if (contentDoc.isObject()) {
                content = contentDoc.object();
            }
        }
    }
    if (content.isEmpty()) {
        content.insert("sat", sat);
    } else if (!content.contains("sat") || content.value("sat").toString().isEmpty()) {
        // 兜底：如果存的内容里没有 sat，从 part 节点补上
        content.insert("sat", sat);
    }
    response.insert("content", content);
    return QJsonDocument(response).toJson(QJsonDocument::Compact);
}

QByteArray StorageBridgeService::handleGetModelVersion(const QString &projectId, int version, int &statusCode,
                                                       QString &error) {
    statusCode = 200;

    mg_map *params = mg_map_make_empty(2);
    mg_map_append(params, mg_string_make("project_id"), mg_value_make_string(projectId.toUtf8().constData()));
    mg_map_append(params, mg_string_make("version"), mg_value_make_integer(version));

    QString dbError;
    const auto rows =
        runQuery(neo4jHost, neo4jPort, neo4jUser, neo4jPassword,
                 "MATCH (p:BridgeProject {id:$project_id})-[:HAS_VERSION]->(v:BridgeVersion {version:$version}) "
                 "RETURN id(v),v.project_id,v.version,v.author,v.created_at,v.part_name,v.content_text LIMIT 1",
                 params, dbError);
    mg_map_destroy(params);

    if (!dbError.isEmpty()) {
        statusCode = 500;
        error = dbError;
        return {};
    }
    if (rows.empty() || rows.front().size() < 6) {
        statusCode = 404;
        error = QString::fromUtf8("Model version not found");
        return {};
    }

    QString sat;
    if (!loadSatFromPart(neo4jHost, neo4jPort, neo4jUser, neo4jPassword, toQString(rows.front()[5]), sat, dbError)) {
        statusCode = 500;
        error = dbError;
        return {};
    }

    QJsonObject response;
    response.insert("id", static_cast<double>(toInt64(rows.front()[0])));
    response.insert("project_id", toQString(rows.front()[1]));
    response.insert("version", static_cast<int>(toInt64(rows.front()[2])));
    response.insert("author", toQString(rows.front()[3]));
    response.insert("created_at", toQString(rows.front()[4]));
    // 从 BridgeVersion.content_text 反序列化回完整 content（包含 entity_graph / changes / sat_format）。
    // 旧数据可能没有 content_text 字段，回退到只塞 sat（兼容老版本）。
    QJsonObject content;
    if (rows.front().size() >= 7) {
        const QString contentText = toQString(rows.front()[6]);
        if (!contentText.isEmpty()) {
            const QJsonDocument contentDoc = QJsonDocument::fromJson(contentText.toUtf8());
            if (contentDoc.isObject()) {
                content = contentDoc.object();
            }
        }
    }
    if (content.isEmpty()) {
        content.insert("sat", sat);
    } else if (!content.contains("sat") || content.value("sat").toString().isEmpty()) {
        content.insert("sat", sat);
    }
    response.insert("content", content);
    return QJsonDocument(response).toJson(QJsonDocument::Compact);
}

QByteArray StorageBridgeService::handleListVersions(const QString &projectId, int limit, int offset, int &statusCode,
                                                    QString &error) {
    statusCode = 200;

    mg_map *params = mg_map_make_empty(3);
    mg_map_append(params, mg_string_make("project_id"), mg_value_make_string(projectId.toUtf8().constData()));
    mg_map_append(params, mg_string_make("offset"), mg_value_make_integer(offset));
    mg_map_append(params, mg_string_make("limit"), mg_value_make_integer(limit));

    QString dbError;
    const auto rows = runQuery(neo4jHost, neo4jPort, neo4jUser, neo4jPassword,
                               "MATCH (p:BridgeProject {id:$project_id})-[:HAS_VERSION]->(v:BridgeVersion) "
                               "RETURN id(v),v.project_id,v.version,v.author,v.created_at,v.part_name,v.content_text "
                               "ORDER BY v.version DESC SKIP $offset LIMIT $limit",
                               params, dbError);
    mg_map_destroy(params);

    if (!dbError.isEmpty()) {
        statusCode = 500;
        error = dbError;
        return {};
    }

    QJsonArray response;
    for (const auto &row : rows) {
        if (row.size() < 6) {
            continue;
        }

        QString sat;
        if (!loadSatFromPart(neo4jHost, neo4jPort, neo4jUser, neo4jPassword, toQString(row[5]), sat, dbError)) {
            statusCode = 500;
            error = dbError;
            return {};
        }

        QJsonObject item;
        item.insert("id", static_cast<double>(toInt64(row[0])));
        item.insert("project_id", toQString(row[1]));
        item.insert("version", static_cast<int>(toInt64(row[2])));
        item.insert("author", toQString(row[3]));
        item.insert("created_at", toQString(row[4]));
        // 从 content_text 反序列化回完整 content（包含 entity_graph / changes / sat_format）。
        // 旧数据可能没有 content_text，回退到只塞 sat。
        QJsonObject content;
        if (row.size() >= 7) {
            const QString contentText = toQString(row[6]);
            if (!contentText.isEmpty()) {
                const QJsonDocument contentDoc = QJsonDocument::fromJson(contentText.toUtf8());
                if (contentDoc.isObject()) {
                    content = contentDoc.object();
                }
            }
        }
        if (content.isEmpty()) {
            content.insert("sat", sat);
        } else if (!content.contains("sat") || content.value("sat").toString().isEmpty()) {
            content.insert("sat", sat);
        }
        item.insert("content", content);
        response.append(item);
    }

    return QJsonDocument(response).toJson(QJsonDocument::Compact);
}
