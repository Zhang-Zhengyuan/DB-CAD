#include "storage/storage_bridge_internal.h"

// HTTP framing, connection lifecycle and dispatch. Domain handlers live in storage/.
using namespace dbcad::bridge;

StorageBridgeService::StorageBridgeService(QString bindHost, int bindPort, QString neo4jHost, int neo4jPort,
                                           QString neo4jUser, QString neo4jPassword, QObject *parent)
    : QObject(parent), bindHost(std::move(bindHost)), bindPort(bindPort), neo4jHost(std::move(neo4jHost)),
      neo4jPort(neo4jPort), neo4jUser(std::move(neo4jUser)), neo4jPassword(std::move(neo4jPassword)) {}

bool StorageBridgeService::start(QString &errorMessage) {
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
        QTcpSocket *socket = server->nextPendingConnection();
        auto *buffer = new QByteArray();

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

bool StorageBridgeService::parseHttpRequest(QByteArray &buffer, HttpRequest &request) {
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
    QString sourceClientId;
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
        } else if (key == "x-source-client-id") {
            // 【Phase B】保存提交方 client_id 供后续 broadcast 排除
            sourceClientId = QString::fromUtf8(value);
        }
    }

    const int fullLength = headerEnd + 4 + contentLength;
    if (buffer.size() < fullLength) {
        return false;
    }

    request.method = QString::fromUtf8(firstLine[0]);
    request.path = QString::fromUtf8(firstLine[1]);
    request.body = buffer.mid(headerEnd + 4, contentLength);
    request.sourceClientId = sourceClientId; // 【Phase B】从 HTTP header 透传
    return true;
}

void StorageBridgeService::processRequest(QTcpSocket *socket, const HttpRequest &request) {
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
        } else if (request.method == "POST" && parts.size() == 2 && parts[1] == "delta") {
            // 【Phase B】先把 HTTP header 里的 X-Source-Client-Id 暂存到成员，
            // handleSaveDelta 内部会把它透传给 handle_save_delta_to_neo4j。
            lastDeltaSourceClientId = request.sourceClientId;
            payload = handleSaveDelta(QUrl::fromPercentEncoding(parts[0].toUtf8()), request.body, statusCode, error);
        } else if (request.method == "GET" && parts.size() == 2 && parts[1] == "delta") {
            // GET /projects/{id}/delta?base_version=X
            const QUrlQuery query(url);
            int baseVersion = parsePositiveInt(query.queryItemValue("base_version"), 0);
            payload = handleGetDelta(QUrl::fromPercentEncoding(parts[0].toUtf8()), baseVersion, statusCode, error);
        } else if (request.method == "GET" && parts.size() == 3 && parts[1] == "models" && parts[2] == "latest") {
            payload = handleGetLatestModel(QUrl::fromPercentEncoding(parts[0].toUtf8()), statusCode, error);
        } else if (request.method == "GET" && parts.size() == 3 && parts[1] == "models" && parts[2] == "versions") {
            const QUrlQuery query(url);
            const int limit = parsePositiveInt(query.queryItemValue("limit"), 50);
            const int offset = (std::max)(0, query.queryItemValue("offset").toInt());
            payload =
                handleListVersions(QUrl::fromPercentEncoding(parts[0].toUtf8()), limit, offset, statusCode, error);
        } else if (request.method == "GET" && parts.size() == 3 && parts[1] == "models") {
            bool ok = false;
            const int version = parts[2].toInt(&ok);
            if (!ok || version <= 0) {
                error = QString::fromUtf8("Invalid version number");
                statusCode = 400;
            } else {
                payload =
                    handleGetModelVersion(QUrl::fromPercentEncoding(parts[0].toUtf8()), version, statusCode, error);
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

void StorageBridgeService::sendJson(QTcpSocket *socket, int statusCode, const QByteArray &payload) {
    const QByteArray reason = statusCode == 200   ? "OK"
                              : statusCode == 201 ? "Created"
                              : statusCode == 400 ? "Bad Request"
                              : statusCode == 401 ? "Unauthorized"
                              : statusCode == 404 ? "Not Found"
                              : statusCode == 409 ? "Conflict"
                              : statusCode == 500 ? "Internal Server Error"
                                                  : "Error";

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

void StorageBridgeService::sendError(QTcpSocket *socket, int statusCode, const QString &detail) {
    sendJson(socket, statusCode, jsonError(detail));
}
