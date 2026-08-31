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
        const QString detail = doc.object().value("detail").toString().trimmed();
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

std::optional<int> BackendApiClient::saveModel(const QString& projectId, const QString& sat, std::optional<int> baseVersion) {
    errorMessage.clear();

    QJsonObject content;
    content.insert("sat", sat);

    QJsonObject payload;
    payload.insert("author", author.isEmpty() ? QString::fromUtf8("dbcad-exe") : author);
    payload.insert("content", content);
    if (baseVersion.has_value()) {
        payload.insert("base_version", *baseVersion);
    } else {
        payload.insert("base_version", QJsonValue::Null);
    }

    HttpResult response = sendJsonRequest(
        "POST",
        QString("/projects/%1/models").arg(projectId),
        QJsonDocument(payload).toJson(QJsonDocument::Compact));

    if (!response.error.isEmpty() && response.statusCode <= 0) {
        errorMessage = response.error;
        return std::nullopt;
    }

    if (response.statusCode == 409) {
        errorMessage = QString::fromUtf8("远程版本冲突，请先重新加载最新模型后再保存");
        return std::nullopt;
    }

    if (response.statusCode != 201) {
        errorMessage = formatHttpErrorMessage(QString::fromUtf8("保存模型失败"), response.statusCode, response.body);
        return std::nullopt;
    }

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(response.body, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        errorMessage = QString::fromUtf8("保存模型返回数据格式错误");
        return std::nullopt;
    }

    const int version = doc.object().value("version").toInt(0);
    if (version <= 0) {
        errorMessage = QString::fromUtf8("保存模型返回版本号无效");
        return std::nullopt;
    }
    return version;
}

std::optional<BackendApiClient::ModelPayload> BackendApiClient::getLatestModel(const QString& projectId) {
    errorMessage.clear();
    HttpResult response = sendJsonRequest("GET", QString("/projects/%1/models/latest").arg(projectId));

    if (!response.error.isEmpty() && response.statusCode <= 0) {
        errorMessage = response.error;
        return std::nullopt;
    }

    if (response.statusCode != 200) {
        errorMessage = formatHttpErrorMessage(QString::fromUtf8("获取最新模型失败"), response.statusCode, response.body);
        return std::nullopt;
    }

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(response.body, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        errorMessage = QString::fromUtf8("获取最新模型返回数据格式错误");
        return std::nullopt;
    }

    const QJsonObject root = doc.object();
    const QJsonObject content = root.value("content").toObject();

    ModelPayload payload;
    payload.projectId = root.value("project_id").toString();
    payload.version = root.value("version").toInt(0);
    payload.sat = content.value("sat").toString();

    if (payload.projectId.isEmpty() || payload.version <= 0 || payload.sat.isEmpty()) {
        errorMessage = QString::fromUtf8("获取最新模型返回数据缺少必要字段");
        return std::nullopt;
    }

    return payload;
}

std::optional<BackendApiClient::ModelPayload> BackendApiClient::getModelVersion(const QString& projectId, int version) {
    errorMessage.clear();
    HttpResult response = sendJsonRequest("GET", QString("/projects/%1/models/%2").arg(projectId).arg(version));

    if (!response.error.isEmpty() && response.statusCode <= 0) {
        errorMessage = response.error;
        return std::nullopt;
    }

    if (response.statusCode != 200) {
        errorMessage = formatHttpErrorMessage(QString::fromUtf8("获取指定版本模型失败"), response.statusCode, response.body);
        return std::nullopt;
    }

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(response.body, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        errorMessage = QString::fromUtf8("获取指定版本模型返回数据格式错误");
        return std::nullopt;
    }

    const QJsonObject root = doc.object();
    const QJsonObject content = root.value("content").toObject();

    ModelPayload payload;
    payload.projectId = root.value("project_id").toString();
    payload.version = root.value("version").toInt(0);
    payload.sat = content.value("sat").toString();
    // 把整个 content 序列化为 JSON 字符串，便于上游调用方走 entity_graph 增量合并路径
    // 而不是 clear+restore 全量替换。
    payload.contentJson = QString::fromUtf8(QJsonDocument(content).toJson(QJsonDocument::Compact));

    if (payload.projectId.isEmpty() || payload.version <= 0 || payload.sat.isEmpty()) {
        errorMessage = QString::fromUtf8("获取指定版本模型返回数据缺少必要字段");
        return std::nullopt;
    }

    return payload;
}

// ---------------------------------------------------------------------------
// Neo4j Entity Graph Storage
// ---------------------------------------------------------------------------

std::optional<int> BackendApiClient::saveEntityGraph(
    const QString& projectId,
    const QString& author,
    const QString& entityGraphJson,
    std::optional<int> baseVersion
) {
    errorMessage.clear();

    // 构造 EntityVersionCreate 兼容的 payload
    // { "author": "...", "entity_graph": {...}, "base_version": ... }
    QJsonParseError parseError;
    const QJsonDocument egDoc = QJsonDocument::fromJson(entityGraphJson.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        errorMessage = QString::fromUtf8("entity_graph JSON 格式错误: %1").arg(parseError.errorString());
        return std::nullopt;
    }
    if (!egDoc.isObject()) {
        errorMessage = QString::fromUtf8("entity_graph 根必须是对象");
        return std::nullopt;
    }

    QJsonObject payload;
    payload.insert("author", author.isEmpty() ? QString::fromUtf8("dbcad-exe") : author);
    payload.insert("entity_graph", egDoc.object());
    if (baseVersion.has_value()) {
        payload.insert("base_version", *baseVersion);
    } else {
        payload.insert("base_version", QJsonValue::Null);
    }

    HttpResult response = sendJsonRequest(
        "POST",
        QString("/projects/%1/entities").arg(projectId),
        QJsonDocument(payload).toJson(QJsonDocument::Compact)
    );

    if (!response.error.isEmpty() && response.statusCode <= 0) {
        errorMessage = response.error;
        return std::nullopt;
    }

    if (response.statusCode == 409) {
        errorMessage = QString::fromUtf8("Entity graph 版本冲突（base_version 与服务端最新版本不匹配）");
        return std::nullopt;
    }

    if (response.statusCode != 201) {
        errorMessage = formatHttpErrorMessage(QString::fromUtf8("保存 entity graph 失败"), response.statusCode, response.body);
        return std::nullopt;
    }

    const QJsonDocument respDoc = QJsonDocument::fromJson(response.body, &parseError);
    if (parseError.error != QJsonParseError::NoError || !respDoc.isObject()) {
        errorMessage = QString::fromUtf8("保存 entity graph 返回数据格式错误");
        return std::nullopt;
    }

    const int version = respDoc.object().value("version").toInt(0);
    if (version <= 0) {
        errorMessage = QString::fromUtf8("保存 entity graph 返回版本号无效");
        return std::nullopt;
    }
    return version;
}

std::optional<BackendApiClient::EntityGraphPayload> BackendApiClient::getEntityVersion(
    const QString& projectId,
    int version
) {
    errorMessage.clear();

    HttpResult response = sendJsonRequest(
        "GET",
        QString("/projects/%1/entities/%2").arg(projectId).arg(version)
    );

    if (!response.error.isEmpty() && response.statusCode <= 0) {
        errorMessage = response.error;
        return std::nullopt;
    }

    if (response.statusCode == 404) {
        return std::nullopt;
    }

    if (response.statusCode != 200) {
        errorMessage = formatHttpErrorMessage(QString::fromUtf8("获取 entity graph 版本失败"), response.statusCode, response.body);
        return std::nullopt;
    }

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(response.body, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        errorMessage = QString::fromUtf8("获取 entity graph 返回数据格式错误");
        return std::nullopt;
    }

    const QJsonObject root = doc.object();

    EntityGraphPayload result;
    result.version = root.value("version").toInt(0);
    result.author = root.value("author").toString();
    const QString createdAtStr = root.value("created_at").toString();
    if (!createdAtStr.isEmpty()) {
        QDateTime dt = QDateTime::fromString(createdAtStr, Qt::ISODate);
        result.createdAt = dt.isValid() ? dt.toMSecsSinceEpoch() : 0;
    }

    // 直接保留 nodes 数组（原序），调用方需要时按 id 自行索引
    result.nodes = root.value("nodes").toArray();
    result.rels = root.value("rels").toArray();

    if (result.version <= 0) {
        errorMessage = QString::fromUtf8("获取 entity graph 版本号无效");
        return std::nullopt;
    }
    return result;
}

// ---------------------------------------------------------------------------
// Mode1 Delta Push / Pull
// ---------------------------------------------------------------------------

std::optional<BackendApiClient::DeltaSavePayload> BackendApiClient::saveDelta(
    const QString& projectId,
    const QString& author,
    std::optional<int> baseVersion,
    const QStringList& deltaUuids,
    const QStringList& deltaSatSegments,
    const QStringList& removedUuids,
    const QString& sourceClientId
) {
    errorMessage.clear();

    QJsonArray deltaUuidsJson;
    for (const QString& uuid : deltaUuids) {
        deltaUuidsJson.append(uuid);
    }
    QJsonArray deltaSatSegmentsJson;
    for (const QString& sat : deltaSatSegments) {
        deltaSatSegmentsJson.append(sat);
    }
    QJsonArray removedUuidsJson;
    for (const QString& uuid : removedUuids) {
        removedUuidsJson.append(uuid);
    }

    QJsonObject payload;
    payload.insert("author", author);
    if (baseVersion.has_value()) {
        payload.insert("base_version", *baseVersion);
    } else {
        payload.insert("base_version", QJsonValue::Null);
    }
    payload.insert("delta_uuids", deltaUuidsJson);
    payload.insert("delta_sat_segments", deltaSatSegmentsJson);
    payload.insert("removed_uuids", removedUuidsJson);
    // 【Phase B】把本端 client_id 带给 server，server 在 broadcast 时用
    // exclude_client_id 排除本端，避免 mode1_delta_saved 事件回声触发二次 pull。
    if (!sourceClientId.isEmpty()) {
        payload.insert("source_client_id", sourceClientId);
    }

    fprintf(stderr, "[backend_api_client saveDelta] POST /projects/%s/delta delta_uuids=%d delta_sat_segments=%d removed_uuids=%d source_client_id=%s\n",
            projectId.toUtf8().constData(), deltaUuids.size(), deltaSatSegments.size(), removedUuids.size(),
            sourceClientId.isEmpty() ? "(empty)" : sourceClientId.toUtf8().constData());

    HttpResult response = sendJsonRequest(
        "POST",
        QString("/projects/%1/delta").arg(projectId),
        QJsonDocument(payload).toJson(QJsonDocument::Compact)
    );

    if (!response.error.isEmpty() && response.statusCode <= 0) {
        errorMessage = response.error;
        return std::nullopt;
    }

    if (response.statusCode == 409) {
        errorMessage = QString::fromUtf8("Delta 版本冲突（base_version 与服务端最新版本不匹配）");
        return std::nullopt;
    }

    if (response.statusCode != 201) {
        errorMessage = formatHttpErrorMessage(QString::fromUtf8("保存 delta 失败"), response.statusCode, response.body);
        return std::nullopt;
    }

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(response.body, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        errorMessage = QString::fromUtf8("保存 delta 返回数据格式错误");
        return std::nullopt;
    }

    const QJsonObject root = doc.object();
    DeltaSavePayload result;
    result.version = root.value("version").toInt(0);
    result.projectId = root.value("project_id").toString();
    result.author = root.value("author").toString();
    result.createdAt = root.value("created_at").toString();

    if (result.version <= 0) {
        errorMessage = QString::fromUtf8("保存 delta 返回版本号无效");
        return std::nullopt;
    }

    fprintf(stderr, "[backend_api_client saveDelta] SUCCESS v=%d projectId=%s\n",
            result.version, result.projectId.toUtf8().constData());
    return result;
}

std::optional<BackendApiClient::DeltaPullPayload> BackendApiClient::getDelta(
    const QString& projectId,
    int baseVersion
) {
    errorMessage.clear();

    fprintf(stderr, "[backend_api_client getDelta] GET /projects/%s/delta base_version=%d\n",
            projectId.toUtf8().constData(), baseVersion);

    HttpResult response = sendJsonRequest(
        "GET",
        QString("/projects/%1/delta?base_version=%2").arg(projectId).arg(baseVersion)
    );

    if (!response.error.isEmpty() && response.statusCode <= 0) {
        errorMessage = response.error;
        return std::nullopt;
    }

    if (response.statusCode != 200) {
        errorMessage = formatHttpErrorMessage(QString::fromUtf8("获取 delta 失败"), response.statusCode, response.body);
        return std::nullopt;
    }

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(response.body, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        errorMessage = QString::fromUtf8("获取 delta 返回数据格式错误");
        return std::nullopt;
    }

    const QJsonObject root = doc.object();
    DeltaPullPayload result;
    result.version = root.value("version").toInt(0);

    const QJsonArray deltaBodiesArr = root.value("delta_bodies").toArray();
    for (const QJsonValue& v : deltaBodiesArr) {
        const QJsonObject item = v.toObject();
        DeltaBodyItem body;
        body.uuid = item.value("uuid").toString();
        body.sat = item.value("sat").toString();
        result.deltaBodies.append(body);
    }

    const QJsonArray deletedArr = root.value("deleted_uuids").toArray();
    for (const QJsonValue& v : deletedArr) {
        result.deletedUuids.append(v.toString());
    }

    fprintf(stderr, "[backend_api_client getDelta] SUCCESS v=%d delta_bodies=%d deleted_uuids=%d\n",
            result.version, result.deltaBodies.size(), result.deletedUuids.size());
    return result;
}
