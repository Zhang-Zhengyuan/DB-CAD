#include "storage_bridge_internal.h"

using namespace dbcad::bridge;

QByteArray StorageBridgeService::handleHealth() const {
    QJsonObject obj;
    obj.insert("status", "ok");
    obj.insert("service", "cpp-storage-bridge");
    return QJsonDocument(obj).toJson(QJsonDocument::Compact);
}

QByteArray StorageBridgeService::handleCreateProject(const QByteArray &body, int &statusCode, QString &error) {
    statusCode = 201;

    const QJsonDocument doc = QJsonDocument::fromJson(body);
    if (!doc.isObject()) {
        statusCode = 400;
        error = QString::fromUtf8("Invalid JSON payload");
        return {};
    }

    const QString name = doc.object().value("name").toString().trimmed();
    if (name.isEmpty()) {
        statusCode = 400;
        error = QString::fromUtf8("Project name is required");
        return {};
    }

    mg_map *params = mg_map_make_empty(1);
    mg_map_append(params, mg_string_make("name"), mg_value_make_string(name.toUtf8().constData()));

    QString dbError;
    const auto existingRows = runQuery(neo4jHost, neo4jPort, neo4jUser, neo4jPassword,
                                       "MATCH (p:BridgeProject {name:$name}) RETURN p.id LIMIT 1", params, dbError);
    mg_map_destroy(params);

    if (!dbError.isEmpty()) {
        statusCode = 500;
        error = dbError;
        return {};
    }
    if (!existingRows.empty()) {
        statusCode = 409;
        error = QString::fromUtf8("Project name already exists");
        return {};
    }

    const QString projectId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    const QString now = nowIsoUtc();

    params = mg_map_make_empty(3);
    mg_map_append(params, mg_string_make("id"), mg_value_make_string(projectId.toUtf8().constData()));
    mg_map_append(params, mg_string_make("name"), mg_value_make_string(name.toUtf8().constData()));
    mg_map_append(params, mg_string_make("now"), mg_value_make_string(now.toUtf8().constData()));

    const auto rows = runQuery(neo4jHost, neo4jPort, neo4jUser, neo4jPassword,
                               "CREATE (p:BridgeProject {id:$id,name:$name,created_at:$now,updated_at:$now}) RETURN "
                               "p.id,p.name,p.created_at,p.updated_at",
                               params, dbError);
    mg_map_destroy(params);

    if (!dbError.isEmpty()) {
        statusCode = 500;
        error = dbError;
        return {};
    }
    if (rows.empty() || rows.front().size() < 4) {
        statusCode = 500;
        error = QString::fromUtf8("Failed to create project");
        return {};
    }

    QJsonObject response;
    response.insert("id", toQString(rows.front()[0]));
    response.insert("name", toQString(rows.front()[1]));
    response.insert("created_at", toQString(rows.front()[2]));
    response.insert("updated_at", toQString(rows.front()[3]));
    return QJsonDocument(response).toJson(QJsonDocument::Compact);
}

QByteArray StorageBridgeService::handleGetProject(const QString &projectId, int &statusCode, QString &error) {
    statusCode = 200;

    mg_map *params = mg_map_make_empty(1);
    mg_map_append(params, mg_string_make("id"), mg_value_make_string(projectId.toUtf8().constData()));

    QString dbError;
    const auto rows = runQuery(neo4jHost, neo4jPort, neo4jUser, neo4jPassword,
                               "MATCH (p:BridgeProject {id:$id}) RETURN p.id,p.name,p.created_at,p.updated_at LIMIT 1",
                               params, dbError);
    mg_map_destroy(params);

    if (!dbError.isEmpty()) {
        statusCode = 500;
        error = dbError;
        return {};
    }
    if (rows.empty() || rows.front().size() < 4) {
        statusCode = 404;
        error = QString::fromUtf8("Project not found");
        return {};
    }

    QJsonObject response;
    response.insert("id", toQString(rows.front()[0]));
    response.insert("name", toQString(rows.front()[1]));
    response.insert("created_at", toQString(rows.front()[2]));
    response.insert("updated_at", toQString(rows.front()[3]));
    return QJsonDocument(response).toJson(QJsonDocument::Compact);
}

QByteArray StorageBridgeService::handleGetProjectByName(const QString &projectName, int &statusCode, QString &error) {
    statusCode = 200;

    mg_map *params = mg_map_make_empty(1);
    mg_map_append(params, mg_string_make("name"), mg_value_make_string(projectName.toUtf8().constData()));

    QString dbError;
    const auto rows = runQuery(
        neo4jHost, neo4jPort, neo4jUser, neo4jPassword,
        "MATCH (p:BridgeProject {name:$name}) RETURN p.id,p.name,p.created_at,p.updated_at LIMIT 1", params, dbError);
    mg_map_destroy(params);

    if (!dbError.isEmpty()) {
        statusCode = 500;
        error = dbError;
        return {};
    }
    if (rows.empty() || rows.front().size() < 4) {
        statusCode = 404;
        error = QString::fromUtf8("Project not found");
        return {};
    }

    QJsonObject response;
    response.insert("id", toQString(rows.front()[0]));
    response.insert("name", toQString(rows.front()[1]));
    response.insert("created_at", toQString(rows.front()[2]));
    response.insert("updated_at", toQString(rows.front()[3]));
    return QJsonDocument(response).toJson(QJsonDocument::Compact);
}
