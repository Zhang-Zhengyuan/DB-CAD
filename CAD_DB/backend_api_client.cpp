#include "backend_api_client.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

#include <utility>

BackendApiClient::BackendApiClient(QString baseUrl, QString author) : baseUrl(std::move(baseUrl)), author(std::move(author)) {
    this->baseUrl = this->baseUrl.trimmed();
    this->author = this->author.trimmed();
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

BackendApiClient::HttpResult BackendApiClient::sendJsonRequest(const QString& method, const QString& path, const QByteArray& payload) {
    HttpResult result;

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
    loop.exec();

    result.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    result.body = reply->readAll();

    if (reply->error() != QNetworkReply::NoError) {
        result.error = reply->errorString();
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
        errorMessage = QString::fromUtf8("查询项目失败，HTTP状态码：%1").arg(response.statusCode);
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
        errorMessage = QString::fromUtf8("创建项目失败，HTTP状态码：%1").arg(response.statusCode);
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
        errorMessage = QString::fromUtf8("保存模型失败，HTTP状态码：%1").arg(response.statusCode);
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
        errorMessage = QString::fromUtf8("获取最新模型失败，HTTP状态码：%1").arg(response.statusCode);
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
        errorMessage = QString::fromUtf8("获取指定版本模型失败，HTTP状态码：%1").arg(response.statusCode);
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

    if (payload.projectId.isEmpty() || payload.version <= 0 || payload.sat.isEmpty()) {
        errorMessage = QString::fromUtf8("获取指定版本模型返回数据缺少必要字段");
        return std::nullopt;
    }

    return payload;
}
