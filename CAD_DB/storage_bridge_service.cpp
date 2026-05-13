#include "storage_bridge_service.h"

#include <QDateTime>
#include <QFile>
#include <QHostAddress>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUrl>
#include <QUrlQuery>

#include <algorithm>
#include <cstdio>
#include <stdexcept>

#include <acis/include/alltop.hxx>

#include "access.hxx"
#include "acis.hxx"
#include "neo4j.hxx"

namespace {
QString nowIsoUtc() {
    return QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs);
}

QByteArray jsonError(const QString& detail) {
    QJsonObject body;
    body.insert("detail", detail);
    return QJsonDocument(body).toJson(QJsonDocument::Compact);
}

QByteArray asUtf8(const mg_string* value) {
    if (value == nullptr) {
        return {};
    }
    return QByteArray(value->data, static_cast<int>(value->size));
}

struct QueryValue {
    mg_value_type type = MG_VALUE_TYPE_NULL;
    QString stringValue;
    int64_t intValue = 0;
};

QString toQString(const QueryValue& value) {
    if (value.type != MG_VALUE_TYPE_STRING) {
        return {};
    }
    return value.stringValue;
}

int64_t toInt64(const QueryValue& value, bool* ok = nullptr) {
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

std::vector<std::vector<QueryValue>> runQuery(
    const QString& host,
    int port,
    const QString& user,
    const QString& password,
    const QString& statement,
    mg_map* params,
    QString& error) {
    std::vector<std::vector<QueryValue>> rows;

    try {
        Neo4jPart conn(
            host.toStdString().c_str(),
            port,
            user.toStdString().c_str(),
            password.toStdString().c_str(),
            "");

        conn.execute_bolt(statement.toUtf8().constData(), params);

        mg_result* result = nullptr;
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

            const mg_list* row = mg_result_row(result);
            if (row == nullptr) {
                continue;
            }

            const uint32_t rowSize = mg_list_size(row);
            std::vector<QueryValue> values;
            values.reserve(rowSize);
            for (uint32_t i = 0; i < rowSize; ++i) {
                const mg_value* raw = mg_list_at(row, i);
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
    } catch (const std::exception& ex) {
        error = QString::fromUtf8(ex.what());
    }

    return rows;
}

bool saveSatToPart(
    const QString& host,
    int port,
    const QString& user,
    const QString& password,
    const QString& partName,
    const QString& sat,
    QString& error) {
    FILE* satStream = nullptr;
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
    if (satBytes.isEmpty() || fwrite(satBytes.constData(), 1, static_cast<size_t>(satBytes.size()), satStream) != static_cast<size_t>(satBytes.size())) {
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
        Neo4jPart conn(
            host.toStdString().c_str(),
            port,
            user.toStdString().c_str(),
            password.toStdString().c_str(),
            partName.toStdString());
        api_save_entity_list_neo4j_part(conn, entityList);
    } catch (const std::exception& ex) {
        error = QString::fromUtf8(ex.what());
        closeSatStream();
        return false;
    }

    closeSatStream();
    return true;
}

bool loadSatFromPart(
    const QString& host,
    int port,
    const QString& user,
    const QString& password,
    const QString& partName,
    QString& sat,
    QString& error) {
    FILE* satStream = nullptr;
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
        Neo4jPart conn(
            host.toStdString().c_str(),
            port,
            user.toStdString().c_str(),
            password.toStdString().c_str(),
            partName.toStdString());

        if (count_partnode(conn) <= 0) {
            error = QString::fromUtf8("Model version not found");
            return false;
        }

        ENTITY_LIST entityList;
        api_restore_entity_list_neo4j_part(conn, entityList);
        acis_save_entity_list(satStream, entityList, 2, 0, true);
    } catch (const std::exception& ex) {
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

int parsePositiveInt(const QString& value, int fallback) {
    bool ok = false;
    const int parsed = value.toInt(&ok);
    if (!ok || parsed <= 0) {
        return fallback;
    }
    return parsed;
}
}

StorageBridgeService::StorageBridgeService(
    QString bindHost,
    int bindPort,
    QString neo4jHost,
    int neo4jPort,
    QString neo4jUser,
    QString neo4jPassword,
    QObject* parent)
    : QObject(parent),
      bindHost(std::move(bindHost)),
      bindPort(bindPort),
      neo4jHost(std::move(neo4jHost)),
      neo4jPort(neo4jPort),
      neo4jUser(std::move(neo4jUser)),
      neo4jPassword(std::move(neo4jPassword)) {}

bool StorageBridgeService::start(QString& errorMessage) {
    if (server != nullptr) {
        return true;
    }

    acisLevel = initialize_acis();
    if (acisLevel == 0) {
        errorMessage = QString::fromUtf8("Failed to initialize ACIS");
        return false;
    }

    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &StorageBridgeService::onNewConnection);

    const QHostAddress hostAddress(bindHost);
    if (!server->listen(hostAddress, static_cast<quint16>(bindPort))) {
        errorMessage = server->errorString();
        stop();
        return false;
    }

    return true;
}

void StorageBridgeService::stop() {
    if (server != nullptr) {
        server->close();
    }
    if (acisLevel > 0) {
        terminate_acis(acisLevel);
        acisLevel = 0;
    }
}

void StorageBridgeService::onNewConnection() {
    while (server != nullptr && server->hasPendingConnections()) {
        QTcpSocket* socket = server->nextPendingConnection();
        auto* buffer = new QByteArray();

        connect(socket, &QTcpSocket::readyRead, this, [this, socket, buffer]() {
            buffer->append(socket->readAll());

            HttpRequest request;
            if (!parseHttpRequest(*buffer, request)) {
                return;
            }

            processRequest(socket, request);
            if (socket->bytesToWrite() > 0) {
                socket->waitForBytesWritten(30000);
            }
            socket->disconnectFromHost();
        });

        connect(socket, &QTcpSocket::disconnected, socket, [buffer, socket]() {
            delete buffer;
            socket->deleteLater();
        });
    }
}

bool StorageBridgeService::parseHttpRequest(QByteArray& buffer, HttpRequest& request) {
    const int headerEnd = buffer.indexOf("\r\n\r\n");
    if (headerEnd < 0) {
        return false;
    }

    const QByteArray headerBytes = buffer.left(headerEnd);
    const QList<QByteArray> lines = headerBytes.split('\n');
    if (lines.isEmpty()) {
        return false;
    }

    const QList<QByteArray> firstLine = lines.first().trimmed().split(' ');
    if (firstLine.size() < 2) {
        return false;
    }

    int contentLength = 0;
    for (int i = 1; i < lines.size(); ++i) {
        const QByteArray line = lines[i].trimmed();
        const int colon = line.indexOf(':');
        if (colon <= 0) {
            continue;
        }

        const QByteArray key = line.left(colon).trimmed().toLower();
        const QByteArray value = line.mid(colon + 1).trimmed();
        if (key == "content-length") {
            bool ok = false;
            const int parsed = value.toInt(&ok);
            if (ok && parsed > 0) {
                contentLength = parsed;
            }
        }
    }

    const int fullLength = headerEnd + 4 + contentLength;
    if (buffer.size() < fullLength) {
        return false;
    }

    request.method = QString::fromUtf8(firstLine[0]);
    request.path = QString::fromUtf8(firstLine[1]);
    request.body = buffer.mid(headerEnd + 4, contentLength);
    return true;
}

void StorageBridgeService::processRequest(QTcpSocket* socket, const HttpRequest& request) {
    QString error;
    int statusCode = 200;
    QByteArray payload;

    const QUrl url(request.path);
    const QString path = url.path();

    if (request.method == "GET" && path == "/health") {
        payload = handleHealth();
        sendJson(socket, 200, payload);
        return;
    }

    if (request.method == "POST" && path == "/projects") {
        payload = handleCreateProject(request.body, statusCode, error);
        if (!error.isEmpty()) {
            sendError(socket, statusCode, error);
        } else {
            sendJson(socket, statusCode, payload);
        }
        return;
    }

    if (request.method == "GET" && path.startsWith("/projects/by-name/")) {
        const QString raw = path.mid(QString("/projects/by-name/").size());
        payload = handleGetProjectByName(QUrl::fromPercentEncoding(raw.toUtf8()), statusCode, error);
        if (!error.isEmpty()) {
            sendError(socket, statusCode, error);
        } else {
            sendJson(socket, statusCode, payload);
        }
        return;
    }

    if (path.startsWith("/projects/")) {
        const QString remain = path.mid(QString("/projects/").size());
        const QStringList parts = remain.split('/', Qt::SkipEmptyParts);

        if (request.method == "GET" && parts.size() == 1) {
            payload = handleGetProject(QUrl::fromPercentEncoding(parts[0].toUtf8()), statusCode, error);
        } else if (request.method == "POST" && parts.size() == 2 && parts[1] == "models") {
            payload = handleCreateModel(QUrl::fromPercentEncoding(parts[0].toUtf8()), request.body, statusCode, error);
        } else if (request.method == "GET" && parts.size() == 3 && parts[1] == "models" && parts[2] == "latest") {
            payload = handleGetLatestModel(QUrl::fromPercentEncoding(parts[0].toUtf8()), statusCode, error);
        } else if (request.method == "GET" && parts.size() == 3 && parts[1] == "models" && parts[2] == "versions") {
            const QUrlQuery query(url);
            const int limit = parsePositiveInt(query.queryItemValue("limit"), 50);
            const int offset = (std::max)(0, query.queryItemValue("offset").toInt());
            payload = handleListVersions(QUrl::fromPercentEncoding(parts[0].toUtf8()), limit, offset, statusCode, error);
        } else if (request.method == "GET" && parts.size() == 3 && parts[1] == "models") {
            bool ok = false;
            const int version = parts[2].toInt(&ok);
            if (!ok || version <= 0) {
                error = QString::fromUtf8("Invalid version number");
                statusCode = 400;
            } else {
                payload = handleGetModelVersion(QUrl::fromPercentEncoding(parts[0].toUtf8()), version, statusCode, error);
            }
        } else {
            error = QString::fromUtf8("Not found");
            statusCode = 404;
        }

        if (!error.isEmpty()) {
            sendError(socket, statusCode, error);
        } else {
            sendJson(socket, statusCode, payload);
        }
        return;
    }

    sendError(socket, 404, QString::fromUtf8("Not found"));
}

void StorageBridgeService::sendJson(QTcpSocket* socket, int statusCode, const QByteArray& payload) {
    const QByteArray reason =
        statusCode == 200 ? "OK" :
        statusCode == 201 ? "Created" :
        statusCode == 400 ? "Bad Request" :
        statusCode == 401 ? "Unauthorized" :
        statusCode == 404 ? "Not Found" :
        statusCode == 409 ? "Conflict" :
        statusCode == 500 ? "Internal Server Error" : "Error";

    QByteArray response;
    response += "HTTP/1.1 " + QByteArray::number(statusCode) + " " + reason + "\r\n";
    response += "Content-Type: application/json; charset=utf-8\r\n";
    response += "Content-Length: " + QByteArray::number(payload.size()) + "\r\n";
    response += "Connection: close\r\n\r\n";
    response += payload;

    socket->write(response);
    socket->flush();
    if (socket->bytesToWrite() > 0) {
        socket->waitForBytesWritten(30000);
    }
}

void StorageBridgeService::sendError(QTcpSocket* socket, int statusCode, const QString& detail) {
    sendJson(socket, statusCode, jsonError(detail));
}

QByteArray StorageBridgeService::handleHealth() const {
    QJsonObject obj;
    obj.insert("status", "ok");
    obj.insert("service", "cpp-storage-bridge");
    return QJsonDocument(obj).toJson(QJsonDocument::Compact);
}

QByteArray StorageBridgeService::handleCreateProject(const QByteArray& body, int& statusCode, QString& error) {
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

    mg_map* params = mg_map_make_empty(1);
    mg_map_append(params, mg_string_make("name"), mg_value_make_string(name.toUtf8().constData()));

    QString dbError;
    const auto existingRows = runQuery(
        neo4jHost,
        neo4jPort,
        neo4jUser,
        neo4jPassword,
        "MATCH (p:BridgeProject {name:$name}) RETURN p.id LIMIT 1",
        params,
        dbError);
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

    const auto rows = runQuery(
        neo4jHost,
        neo4jPort,
        neo4jUser,
        neo4jPassword,
        "CREATE (p:BridgeProject {id:$id,name:$name,created_at:$now,updated_at:$now}) RETURN p.id,p.name,p.created_at,p.updated_at",
        params,
        dbError);
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

QByteArray StorageBridgeService::handleGetProject(const QString& projectId, int& statusCode, QString& error) {
    statusCode = 200;

    mg_map* params = mg_map_make_empty(1);
    mg_map_append(params, mg_string_make("id"), mg_value_make_string(projectId.toUtf8().constData()));

    QString dbError;
    const auto rows = runQuery(
        neo4jHost,
        neo4jPort,
        neo4jUser,
        neo4jPassword,
        "MATCH (p:BridgeProject {id:$id}) RETURN p.id,p.name,p.created_at,p.updated_at LIMIT 1",
        params,
        dbError);
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

QByteArray StorageBridgeService::handleGetProjectByName(const QString& projectName, int& statusCode, QString& error) {
    statusCode = 200;

    mg_map* params = mg_map_make_empty(1);
    mg_map_append(params, mg_string_make("name"), mg_value_make_string(projectName.toUtf8().constData()));

    QString dbError;
    const auto rows = runQuery(
        neo4jHost,
        neo4jPort,
        neo4jUser,
        neo4jPassword,
        "MATCH (p:BridgeProject {name:$name}) RETURN p.id,p.name,p.created_at,p.updated_at LIMIT 1",
        params,
        dbError);
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

QByteArray StorageBridgeService::handleCreateModel(const QString& projectId, const QByteArray& body, int& statusCode, QString& error) {
    statusCode = 201;

    const QJsonDocument doc = QJsonDocument::fromJson(body);
    if (!doc.isObject()) {
        statusCode = 400;
        error = QString::fromUtf8("Invalid JSON payload");
        return {};
    }

    const QJsonObject root = doc.object();
    const QString author = root.value("author").toString().trimmed();
    const QJsonObject content = root.value("content").toObject();
    const QString sat = content.value("sat").toString();
    const bool hasBaseVersion = !root.value("base_version").isNull();
    const int baseVersion = root.value("base_version").toInt(0);

    if (author.isEmpty() || sat.isEmpty()) {
        statusCode = 400;
        error = QString::fromUtf8("author and content.sat are required");
        return {};
    }

    mg_map* params = mg_map_make_empty(1);
    mg_map_append(params, mg_string_make("id"), mg_value_make_string(projectId.toUtf8().constData()));

    QString dbError;
    const auto lookup = runQuery(
        neo4jHost,
        neo4jPort,
        neo4jUser,
        neo4jPassword,
        "MATCH (p:BridgeProject {id:$id}) OPTIONAL MATCH (p)-[:HAS_VERSION]->(v:BridgeVersion) RETURN p.id,max(v.version)",
        params,
        dbError);
    mg_map_destroy(params);

    if (!dbError.isEmpty()) {
        statusCode = 500;
        error = dbError;
        return {};
    }
    if (lookup.empty() || lookup.front().size() < 2) {
        statusCode = 404;
        error = QString::fromUtf8("Project not found");
        return {};
    }

    bool latestOk = false;
    const int64_t latestVersionValue = toInt64(lookup.front()[1], &latestOk);
    const int latestVersion = latestOk ? static_cast<int>(latestVersionValue) : 0;

    if (!hasBaseVersion && latestVersion > 0) {
        statusCode = 409;
        error = QString::fromUtf8("base_version is required: latest version is %1").arg(latestVersion);
        return {};
    }

    if (hasBaseVersion && baseVersion != latestVersion) {
        statusCode = 409;
        error = QString::fromUtf8("Version conflict: latest version is %1").arg(latestVersion);
        return {};
    }

    const int nextVersion = latestVersion + 1;
    const QString partName = QString::fromUtf8("bridge__%1__v%2").arg(projectId).arg(nextVersion);

    if (!saveSatToPart(neo4jHost, neo4jPort, neo4jUser, neo4jPassword, partName, sat, dbError)) {
        statusCode = 500;
        error = dbError;
        return {};
    }

    const QString now = nowIsoUtc();
    params = mg_map_make_empty(5);
    mg_map_append(params, mg_string_make("project_id"), mg_value_make_string(projectId.toUtf8().constData()));
    mg_map_append(params, mg_string_make("version"), mg_value_make_integer(nextVersion));
    mg_map_append(params, mg_string_make("author"), mg_value_make_string(author.toUtf8().constData()));
    mg_map_append(params, mg_string_make("created_at"), mg_value_make_string(now.toUtf8().constData()));
    mg_map_append(params, mg_string_make("part_name"), mg_value_make_string(partName.toUtf8().constData()));

    const auto rows = runQuery(
        neo4jHost,
        neo4jPort,
        neo4jUser,
        neo4jPassword,
        "MATCH (p:BridgeProject {id:$project_id}) "
        "SET p.updated_at=$created_at "
        "CREATE (v:BridgeVersion {project_id:$project_id,version:$version,author:$author,created_at:$created_at,part_name:$part_name}) "
        "CREATE (p)-[:HAS_VERSION]->(v) "
        "RETURN id(v),v.project_id,v.version,v.author,v.created_at",
        params,
        dbError);
    mg_map_destroy(params);

    if (!dbError.isEmpty()) {
        statusCode = 500;
        error = dbError;
        return {};
    }
    if (rows.empty() || rows.front().size() < 5) {
        statusCode = 500;
        error = QString::fromUtf8("Failed to create model version metadata");
        return {};
    }

    bool idOk = false;
    const int64_t dbId = toInt64(rows.front()[0], &idOk);
    if (!idOk) {
        statusCode = 500;
        error = QString::fromUtf8("Invalid version id");
        return {};
    }

    QJsonObject response;
    response.insert("id", static_cast<double>(dbId));
    response.insert("project_id", toQString(rows.front()[1]));
    response.insert("version", static_cast<int>(toInt64(rows.front()[2])));
    response.insert("author", toQString(rows.front()[3]));
    response.insert("created_at", toQString(rows.front()[4]));
    QJsonObject outContent;
    outContent.insert("sat", sat);
    response.insert("content", outContent);
    return QJsonDocument(response).toJson(QJsonDocument::Compact);
}

QByteArray StorageBridgeService::handleGetLatestModel(const QString& projectId, int& statusCode, QString& error) {
    statusCode = 200;

    mg_map* params = mg_map_make_empty(1);
    mg_map_append(params, mg_string_make("project_id"), mg_value_make_string(projectId.toUtf8().constData()));

    QString dbError;
    const auto rows = runQuery(
        neo4jHost,
        neo4jPort,
        neo4jUser,
        neo4jPassword,
        "MATCH (p:BridgeProject {id:$project_id})-[:HAS_VERSION]->(v:BridgeVersion) "
        "RETURN id(v),v.project_id,v.version,v.author,v.created_at,v.part_name "
        "ORDER BY v.version DESC LIMIT 1",
        params,
        dbError);
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
    QJsonObject content;
    content.insert("sat", sat);
    response.insert("content", content);
    return QJsonDocument(response).toJson(QJsonDocument::Compact);
}

QByteArray StorageBridgeService::handleGetModelVersion(const QString& projectId, int version, int& statusCode, QString& error) {
    statusCode = 200;

    mg_map* params = mg_map_make_empty(2);
    mg_map_append(params, mg_string_make("project_id"), mg_value_make_string(projectId.toUtf8().constData()));
    mg_map_append(params, mg_string_make("version"), mg_value_make_integer(version));

    QString dbError;
    const auto rows = runQuery(
        neo4jHost,
        neo4jPort,
        neo4jUser,
        neo4jPassword,
        "MATCH (p:BridgeProject {id:$project_id})-[:HAS_VERSION]->(v:BridgeVersion {version:$version}) "
        "RETURN id(v),v.project_id,v.version,v.author,v.created_at,v.part_name LIMIT 1",
        params,
        dbError);
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
    QJsonObject content;
    content.insert("sat", sat);
    response.insert("content", content);
    return QJsonDocument(response).toJson(QJsonDocument::Compact);
}

QByteArray StorageBridgeService::handleListVersions(const QString& projectId, int limit, int offset, int& statusCode, QString& error) {
    statusCode = 200;

    mg_map* params = mg_map_make_empty(3);
    mg_map_append(params, mg_string_make("project_id"), mg_value_make_string(projectId.toUtf8().constData()));
    mg_map_append(params, mg_string_make("offset"), mg_value_make_integer(offset));
    mg_map_append(params, mg_string_make("limit"), mg_value_make_integer(limit));

    QString dbError;
    const auto rows = runQuery(
        neo4jHost,
        neo4jPort,
        neo4jUser,
        neo4jPassword,
        "MATCH (p:BridgeProject {id:$project_id})-[:HAS_VERSION]->(v:BridgeVersion) "
        "RETURN id(v),v.project_id,v.version,v.author,v.created_at,v.part_name "
        "ORDER BY v.version DESC SKIP $offset LIMIT $limit",
        params,
        dbError);
    mg_map_destroy(params);

    if (!dbError.isEmpty()) {
        statusCode = 500;
        error = dbError;
        return {};
    }

    QJsonArray response;
    for (const auto& row : rows) {
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
        QJsonObject content;
        content.insert("sat", sat);
        item.insert("content", content);
        response.append(item);
    }

    return QJsonDocument(response).toJson(QJsonDocument::Compact);
}
