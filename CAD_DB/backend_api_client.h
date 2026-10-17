#pragma once

#include <optional>

#include <QByteArray>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>

class BackendApiClient {
public:
    struct ProjectInfo {
        QString id;
        QString name;
    };


    BackendApiClient(QString baseUrl, QString author, QString apiPassword = {});

    bool isConfigured() const;
    QString lastError() const;
    int lastStatusCode() const;

    std::optional<ProjectInfo> getProjectByName(const QString& projectName);
    std::optional<ProjectInfo> createProject(const QString& projectName);
    std::optional<ProjectInfo> getOrCreateProject(const QString& projectName);

    // ========== Bridge-free JSON Mode1 ==========
    struct JsonGraphPayload {
        int version = 0;
        QString graphJson;
        QString changesJson;
        QStringList removedIds;
        bool delta = false;
        QString graphHash;
    };

    std::optional<int> saveJsonDelta(
        const QString& projectId,
        const QString& author,
        const QString& entityGraphJson,
        const QString& changesJson,
        const QStringList& removedIds,
        std::optional<int> baseVersion,
        const QString& sourceClientId = QString()
    );

    std::optional<JsonGraphPayload> getJsonVersion(const QString& projectId, int version);

    std::optional<JsonGraphPayload> getJsonDelta(
        const QString& projectId,
        int baseVersion
    );

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
