#pragma once

#include <optional>

#include <QByteArray>
#include <QString>

class BackendApiClient {
public:
    struct ProjectInfo {
        QString id;
        QString name;
    };

    struct ModelPayload {
        QString projectId;
        int version = 0;
        QString sat;
        QString contentJson;  // 完整 content JSON（含 entity_graph/changes/sat），便于走 entity_graph 合并路径
    };

    BackendApiClient(QString baseUrl, QString author, QString apiPassword = {});

    bool isConfigured() const;
    QString lastError() const;
    int lastStatusCode() const;

    std::optional<ProjectInfo> getProjectByName(const QString& projectName);
    std::optional<ProjectInfo> createProject(const QString& projectName);
    std::optional<ProjectInfo> getOrCreateProject(const QString& projectName);

    std::optional<int> saveModel(const QString& projectId, const QString& sat, std::optional<int> baseVersion);
    std::optional<ModelPayload> getLatestModel(const QString& projectId);
    std::optional<ModelPayload> getModelVersion(const QString& projectId, int version);

private:
    struct HttpResult {
        int statusCode = -1;
        QByteArray body;
        QString error;
    };

    HttpResult sendJsonRequest(const QString& method, const QString& path, const QByteArray& payload = {});

    QString baseUrl;
    QString author;
    QString apiPassword;
    mutable QString errorMessage;
    mutable int lastHttpStatusCode = -1;
};
