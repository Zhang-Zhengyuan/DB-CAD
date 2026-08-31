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

    // ========== Neo4j Entity Graph Storage ==========
    // 存储格式：{nodes: [{id, labels, props}], rels: [{type, start, end, props}]}
    struct EntityGraphPayload {
        int version = 0;
        QString author;
        qint64 createdAt;
        QJsonArray nodes;  // [{id, labels, props}, ...]
        QJsonArray rels;   // [{type, start, end, props}, ...]
    };

    // POST /projects/{projectId}/entities — 存 ACIS entity graph JSON 到 Neo4j
    // 返回新版本号，失败返回 std::nullopt
    std::optional<int> saveEntityGraph(
        const QString& projectId,
        const QString& author,
        const QString& entityGraphJson,
        std::optional<int> baseVersion
    );

    // GET /projects/{projectId}/entities/{version} — 从 Neo4j 取 entity graph JSON
    // 返回完整 graph（含节点+关系），失败返回 std::nullopt
    std::optional<EntityGraphPayload> getEntityVersion(
        const QString& projectId,
        int version
    );

    // ========== Mode1 Delta Push / Pull ==========
    struct DeltaSavePayload {
        int version = 0;
        QString projectId;
        QString author;
        QString createdAt;
    };

    struct DeltaBodyItem {
        QString uuid;
        QString sat;
    };

    struct DeltaPullPayload {
        int version = 0;
        QList<DeltaBodyItem> deltaBodies;
        QStringList deletedUuids;
    };

    // POST /projects/{projectId}/delta — Mode1 Delta Push
    // 【Phase B】sourceClientId：可选，提交方 client_id；服务端会基于它在
    // broadcast 时排除自身，避免回声。
    std::optional<DeltaSavePayload> saveDelta(
        const QString& projectId,
        const QString& author,
        std::optional<int> baseVersion,
        const QStringList& deltaUuids,
        const QStringList& deltaSatSegments,
        const QStringList& removedUuids,
        const QString& sourceClientId = QString()
    );

    // GET /projects/{projectId}/delta?base_version=X — Mode1 Delta Pull
    std::optional<DeltaPullPayload> getDelta(
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
