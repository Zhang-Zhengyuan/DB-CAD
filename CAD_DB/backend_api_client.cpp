#include "backend_api_client.h"

#include <QEventLoop>
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

    if (payload.projectId.isEmpty() || payload.version <= 0 || payload.sat.isEmpty()) {
        errorMessage = QString::fromUtf8("获取指定版本模型返回数据缺少必要字段");
        return std::nullopt;
    }

    return payload;
}
