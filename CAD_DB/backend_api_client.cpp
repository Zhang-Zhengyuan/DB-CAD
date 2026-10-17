#include "backend_api_client.h"

#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDateTime>
#include <QTextStream>
#include <QTimer>
#include <QUrl>

#include <utility>

namespace {
QString extractErrorDetail(const QByteArray& body) {
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(body, &parseError);
    if (parseError.error == QJsonParseError::NoError && doc.isObject()) {
        const QJsonValue raw = doc.object().value("detail");
        QString detail;
        if (raw.isObject()) detail = QString::fromUtf8(QJsonDocument(raw.toObject()).toJson(QJsonDocument::Compact));
        else if (raw.isArray()) detail = QString::fromUtf8(QJsonDocument(raw.toArray()).toJson(QJsonDocument::Compact));
        else detail = raw.toString().trimmed();
        if (!detail.isEmpty()) {
            return detail;
        }
    }
    return {};
}

void appendFastApiErrorLog(
    const QString& method,
    const QString& url,
    int statusCode,
    const QString& networkError,
    const QByteArray& body) {
    QFile file("fastapi_error.log");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        return;
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    stream << "[" << QDateTime::currentDateTime().toString(Qt::ISODateWithMs) << "] "
           << method << " " << url << " status=" << statusCode;
    if (!networkError.trimmed().isEmpty()) {
        stream << " network_error=" << networkError;
    }

    const QString detail = extractErrorDetail(body);
    if (!detail.isEmpty()) {
        stream << " detail=" << detail;
    } else {
        QString raw = QString::fromUtf8(body).trimmed();
        if (raw.size() > 800) {
            raw = raw.left(800) + "...";
        }
        if (!raw.isEmpty()) {
            stream << " body=" << raw;
        }
    }
    stream << "\n";
}

QString formatHttpErrorMessage(const QString& prefix, int statusCode, const QByteArray& body) {
    const QString detail = extractErrorDetail(body);
    if (!detail.isEmpty()) {
        return QString::fromUtf8("%1，HTTP状态码：%2，详情：%3").arg(prefix).arg(statusCode).arg(detail);
    }
    return QString::fromUtf8("%1，HTTP状态码：%2").arg(prefix).arg(statusCode);
}
}

BackendApiClient::BackendApiClient(QString baseUrl, QString author, QString apiPassword)
    : baseUrl(std::move(baseUrl)), author(std::move(author)), apiPassword(std::move(apiPassword)) {
    this->baseUrl = this->baseUrl.trimmed();
    this->author = this->author.trimmed();
    this->apiPassword = this->apiPassword.trimmed();
    while (this->baseUrl.endsWith('/')) {
        this->baseUrl.chop(1);
    }
}

bool BackendApiClient::isConfigured() const {
    return !baseUrl.isEmpty();
}

QString BackendApiClient::lastError() const {
    return errorMessage;
}

int BackendApiClient::lastStatusCode() const {
    return lastHttpStatusCode;
}

BackendApiClient::HttpResult BackendApiClient::sendJsonRequest(const QString& method, const QString& path, const QByteArray& payload) {
    HttpResult result;
    lastHttpStatusCode = -1;

    if (baseUrl.isEmpty()) {
        result.error = QString::fromUtf8("FastAPI地址未配置，请先设置fastapi_connect_info.conf");
        return result;
    }

    QUrl url(baseUrl + path);
    if (!url.isValid()) {
        result.error = QString::fromUtf8("FastAPI地址无效");
        return result;
    }

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    if (!apiPassword.isEmpty()) {
        request.setRawHeader("X-API-Password", apiPassword.toUtf8());
    }

    QNetworkAccessManager manager;
    QNetworkReply* reply = nullptr;
    if (method == "GET") {
        reply = manager.get(request);
    } else if (method == "POST") {
        reply = manager.post(request, payload);
    } else {
        result.error = QString::fromUtf8("不支持的HTTP方法");
        return result;
    }

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QTimer timer;
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, &loop, [&]() {
        if (reply->isRunning()) {
            reply->abort();
            result.error = QString::fromUtf8("连接FastAPI超时（10秒）");
        }
    });
    timer.start(10000);
    loop.exec();
    timer.stop();

    result.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    lastHttpStatusCode = result.statusCode;
    result.body = reply->readAll();

    if (reply->error() != QNetworkReply::NoError) {
        if (result.error.isEmpty()) {
            result.error = reply->errorString();
        }
    }

    if (result.statusCode >= 400 || !result.error.isEmpty()) {
        appendFastApiErrorLog(method, url.toString(), result.statusCode, result.error, result.body);
    }

    reply->deleteLater();
    return result;
}

std::optional<BackendApiClient::ProjectInfo> BackendApiClient::getProjectByName(const QString& projectName) {
    errorMessage.clear();
    const QString encodedName = QString::fromUtf8(QUrl::toPercentEncoding(projectName));
    HttpResult response = sendJsonRequest("GET", QString("/projects/by-name/%1").arg(encodedName));

    if (!response.error.isEmpty() && response.statusCode <= 0) {
        errorMessage = response.error;
        return std::nullopt;
    }

    if (response.statusCode == 404) {
        return std::nullopt;
    }

    if (response.statusCode != 200) {
        errorMessage = formatHttpErrorMessage(QString::fromUtf8("查询项目失败"), response.statusCode, response.body);
        return std::nullopt;
    }

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(response.body, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        errorMessage = QString::fromUtf8("查询项目返回数据格式错误");
        return std::nullopt;
    }

    QJsonObject obj = doc.object();
    ProjectInfo info;
    info.id = obj.value("id").toString();
    info.name = obj.value("name").toString();
    if (info.id.isEmpty() || info.name.isEmpty()) {
        errorMessage = QString::fromUtf8("查询项目返回数据缺少必要字段");
        return std::nullopt;
    }
    return info;
}

std::optional<BackendApiClient::ProjectInfo> BackendApiClient::createProject(const QString& projectName) {
    errorMessage.clear();

    QJsonObject payload;
    payload.insert("name", projectName);

    HttpResult response = sendJsonRequest("POST", "/projects", QJsonDocument(payload).toJson(QJsonDocument::Compact));

    if (!response.error.isEmpty() && response.statusCode <= 0) {
        errorMessage = response.error;
        return std::nullopt;
    }

    if (response.statusCode == 409) {
        return std::nullopt;
    }

    if (response.statusCode != 201) {
        errorMessage = formatHttpErrorMessage(QString::fromUtf8("创建项目失败"), response.statusCode, response.body);
        return std::nullopt;
    }

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(response.body, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        errorMessage = QString::fromUtf8("创建项目返回数据格式错误");
        return std::nullopt;
    }

    QJsonObject obj = doc.object();
    ProjectInfo info;
    info.id = obj.value("id").toString();
    info.name = obj.value("name").toString();
    if (info.id.isEmpty() || info.name.isEmpty()) {
        errorMessage = QString::fromUtf8("创建项目返回数据缺少必要字段");
        return std::nullopt;
    }
    return info;
}

std::optional<BackendApiClient::ProjectInfo> BackendApiClient::getOrCreateProject(const QString& projectName) {
    errorMessage.clear();

    auto project = getProjectByName(projectName);
    if (project.has_value()) {
        return project;
    }

    if (!errorMessage.isEmpty()) {
        return std::nullopt;
    }

    project = createProject(projectName);
    if (project.has_value()) {
        return project;
    }

    if (errorMessage.isEmpty()) {
        errorMessage = QString::fromUtf8("项目已存在但无法查询，请检查后端接口状态");
    }
    return std::nullopt;
}

std::optional<int> BackendApiClient::saveJsonDelta(
    const QString& projectId,
    const QString& author,
    const QString& entityGraphJson,
    const QString& changesJson,
    const QStringList& removedIds,
    std::optional<int> baseVersion,
    const QString& sourceClientId
) {
    errorMessage.clear();
    QJsonParseError parseError;
    const QJsonDocument graphDoc = QJsonDocument::fromJson(entityGraphJson.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !graphDoc.isObject()) {
        errorMessage = QString::fromUtf8("Entity Graph JSON 格式错误");
        return std::nullopt;
    }
    const QJsonDocument changesDoc = QJsonDocument::fromJson(changesJson.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || (!changesDoc.isArray() && !changesDoc.isObject())) {
        errorMessage = QString::fromUtf8("changes JSON 格式错误");
        return std::nullopt;
    }
    QJsonArray changes;
    if (changesDoc.isArray()) changes = changesDoc.array();
    else changes = changesDoc.object().value("changes").toArray();

    QJsonArray removed;
    for (const QString& id : removedIds) removed.append(id);
    QJsonObject payload;
    payload.insert("author", author);
    payload.insert("entity_graph", graphDoc.object());
    payload.insert("changes", changes);
    payload.insert("removed_ids", removed);
    payload.insert("base_version", baseVersion.has_value() ? QJsonValue(*baseVersion) : QJsonValue(QJsonValue::Null));
    if (!sourceClientId.isEmpty()) payload.insert("source_client_id", sourceClientId);

    const HttpResult response = sendJsonRequest(
        "POST", QString("/projects/%1/json-delta").arg(projectId),
        QJsonDocument(payload).toJson(QJsonDocument::Compact));
    if (!response.error.isEmpty() && response.statusCode <= 0) { errorMessage = response.error; return std::nullopt; }
    if (response.statusCode != 201) {
        errorMessage = formatHttpErrorMessage(QString::fromUtf8("保存 JSON delta 失败"), response.statusCode, response.body);
        return std::nullopt;
    }
    const QJsonDocument resultDoc = QJsonDocument::fromJson(response.body, &parseError);
    const int version = resultDoc.object().value("version").toInt(0);
    if (parseError.error != QJsonParseError::NoError || version <= 0) {
        errorMessage = QString::fromUtf8("保存 JSON delta 返回版本无效");
        return std::nullopt;
    }
    return version;
}

std::optional<BackendApiClient::JsonGraphPayload> BackendApiClient::getJsonDelta(
    const QString& projectId, int baseVersion) {
    errorMessage.clear();
    const HttpResult response = sendJsonRequest(
        "GET", QString("/projects/%1/json-delta?base_version=%2").arg(projectId).arg(baseVersion));
    if (!response.error.isEmpty() && response.statusCode <= 0) { errorMessage = response.error; return std::nullopt; }
    if (response.statusCode != 200) {
        errorMessage = formatHttpErrorMessage(QString::fromUtf8("获取 JSON delta 失败"), response.statusCode, response.body);
        return std::nullopt;
    }
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(response.body, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        errorMessage = QString::fromUtf8("获取 JSON delta 返回格式错误");
        return std::nullopt;
    }
    const QJsonObject root = doc.object();
    JsonGraphPayload result;
    result.version = root.value("version").toInt(0);
    if (!root.value("entity_graph").isObject()) {
        errorMessage = QString::fromUtf8("获取 JSON delta 缺少实体图");
        return std::nullopt;
    }
    result.graphJson = QString::fromUtf8(QJsonDocument(root.value("entity_graph").toObject()).toJson(QJsonDocument::Compact));
    result.changesJson = QString::fromUtf8(QJsonDocument(root.value("changes").toObject()).toJson(QJsonDocument::Compact));
    if (root.value("changes").isArray()) result.changesJson = QString::fromUtf8(QJsonDocument(root.value("changes").toArray()).toJson(QJsonDocument::Compact));
    for (const QJsonValue& value : root.value("removed_ids").toArray()) {
        const QString id = value.toString().trimmed();
        if (!id.isEmpty()) result.removedIds.append(id);
    }
    result.delta = root.value("delta").toBool(false);
    result.graphHash = root.value("graph_hash").toString();
    if (result.version < 0 || result.graphJson.isEmpty()) { errorMessage = QString::fromUtf8("获取 JSON delta 版本无效"); return std::nullopt; }
    return result;
}

std::optional<BackendApiClient::JsonGraphPayload> BackendApiClient::getJsonVersion(const QString &projectId, int version) {
    const auto response = sendJsonRequest("GET", QString("/projects/%1/models/%2").arg(projectId).arg(version));
    if (response.statusCode != 200) { errorMessage = formatHttpErrorMessage("读取历史版本失败", response.statusCode, response.body); return std::nullopt; }
    QJsonParseError error;
    const auto doc = QJsonDocument::fromJson(response.body, &error);
    const auto content = doc.object().value("content").toObject();
    if (error.error != QJsonParseError::NoError || !content.value("entity_graph").isObject()) {
        errorMessage = "历史版本缺少实体图"; return std::nullopt;
    }
    JsonGraphPayload result;
    result.version = doc.object().value("version").toInt();
    result.graphJson = QString::fromUtf8(QJsonDocument(content.value("entity_graph").toObject()).toJson(QJsonDocument::Compact));
    result.changesJson = QString::fromUtf8(QJsonDocument(content.value("changes").toArray()).toJson(QJsonDocument::Compact));
    return result;
}
