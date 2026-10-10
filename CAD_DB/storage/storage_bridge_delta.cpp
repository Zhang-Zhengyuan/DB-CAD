#include "storage_bridge_internal.h"

using namespace dbcad::bridge;

// ================================================================================================
// Mode1 Delta Push: 处理 POST /projects/{projectId}/delta
// payload = { author, base_version, delta_uuids, delta_sat_segments, removed_uuids }
// 【Phase B】sourceClientId：从 HTTP header X-Source-Client-Id 传入，供上层 broadcast 排除。
// ================================================================================================
QByteArray StorageBridgeService::handleSaveDelta(const QString &projectId, const QByteArray &body, int &statusCode,
                                                 QString &error) {
    statusCode = 201;

    std::fprintf(stderr, "[storage_bridge handleSaveDelta] ENTER projectId=%s bodyBytes=%d sourceClientId=%s\n",
                 projectId.toStdString().c_str(), (int)body.size(),
                 lastDeltaSourceClientId.isEmpty() ? "(empty)" : lastDeltaSourceClientId.toStdString().c_str());

    const QJsonDocument doc = QJsonDocument::fromJson(body);
    if (!doc.isObject()) {
        statusCode = 400;
        error = QString::fromUtf8("Invalid JSON payload");
        return {};
    }

    const QJsonObject root = doc.object();
    const QString author = root.value("author").toString().trimmed();
    const int baseVersion = root.value("base_version").toInt(0);
    const QJsonArray deltaUuidsArr = root.value("delta_uuids").toArray();
    const QJsonArray deltaSatSegmentsArr = root.value("delta_sat_segments").toArray();
    const QJsonArray removedUuidsArr = root.value("removed_uuids").toArray();

    std::fprintf(stderr,
                 "[storage_bridge handleSaveDelta] author=%s baseVersion=%d "
                 "delta_uuids=%d delta_sat_segments=%d removed_uuids=%d\n",
                 author.toStdString().c_str(), baseVersion, (int)deltaUuidsArr.size(), (int)deltaSatSegmentsArr.size(),
                 (int)removedUuidsArr.size());

    if (author.isEmpty()) {
        statusCode = 400;
        error = QString::fromUtf8("author is required");
        return {};
    }

    // 将 JSON 数组转为 std::vector<std::string>
    std::vector<std::string> deltaUuids;
    for (const QJsonValue &v : deltaUuidsArr) {
        deltaUuids.push_back(v.toString().toStdString());
    }
    std::vector<std::string> deltaSatSegments;
    for (const QJsonValue &v : deltaSatSegmentsArr) {
        deltaSatSegments.push_back(v.toString().toStdString());
    }
    std::vector<std::string> removedUuids;
    for (const QJsonValue &v : removedUuidsArr) {
        removedUuids.push_back(v.toString().toStdString());
    }

    // 构造 partname = bridge__{projectId}_v{baseVersion+1}
    // 注意：这里用 projectId 直接构造 partname，与 handleCreateModel 的模式一致
    // 但 mode1 的 partname 格式需要与 getDelta 一致。
    // 我们使用 "delta__{projectId}" 作为 partname 前缀，每次 push 覆盖写入。
    // 为了简化：partname = "delta__" + projectId
    const QString partName = QString::fromUtf8("delta__") + projectId;
    const QString timestamp = nowIsoUtc();

    std::fprintf(stderr, "[storage_bridge handleSaveDelta] STEP A: creating Neo4jPart connection partName=%s\n",
                 partName.toStdString().c_str());
    fflush(stderr);

    // 构造 Neo4jPart 连接
    Neo4jPart conn(neo4jHost.toStdString().c_str(), neo4jPort, neo4jUser.toStdString().c_str(),
                   neo4jPassword.toStdString().c_str(), partName.toStdString());

    std::fprintf(
        stderr,
        "[storage_bridge handleSaveDelta] STEP B: Neo4jPart connection created, calling handle_save_delta_to_neo4j\n");
    fflush(stderr);

    // 调用 C++ 层 delta 保存逻辑
    int newVersion = 0;
    std::string errorStr;
    bool ok = handle_save_delta_to_neo4j(conn, baseVersion, deltaUuids, deltaSatSegments, removedUuids,
                                         author.toStdString(), timestamp.toStdString(), newVersion, errorStr);

    std::fprintf(stderr, "[storage_bridge handleSaveDelta] STEP C: handle_save_delta_to_neo4j returned ok=%d\n",
                 (int)ok);
    fflush(stderr);

    if (!ok) {
        std::fprintf(stderr, "[storage_bridge handleSaveDelta] FAILED: %s\n", errorStr.c_str());
        // 如果是版本冲突，返回 409
        if (errorStr.find("Version conflict") != std::string::npos) {
            statusCode = 409;
            error = QString::fromUtf8(errorStr.c_str());
        } else {
            statusCode = 500;
            error = QString::fromUtf8(errorStr.c_str());
        }
        return {};
    }

    std::fprintf(stderr, "[storage_bridge handleSaveDelta] SUCCESS v=%d projectId=%s\n", newVersion,
                 projectId.toStdString().c_str());

    // 返回新版本信息
    QJsonObject response;
    response.insert("version", newVersion);
    response.insert("project_id", projectId);
    response.insert("author", author);
    response.insert("created_at", timestamp);
    return QJsonDocument(response).toJson(QJsonDocument::Compact);
}

// ================================================================================================
// Mode1 Delta Pull: 处理 GET /projects/{projectId}/delta?base_version=X
// 返回 { latest_version, body_uuids, sat_segments_by_uuid }
// ================================================================================================
QByteArray StorageBridgeService::handleGetDelta(const QString &projectId, int baseVersion, int &statusCode,
                                                QString &error) {
    statusCode = 200;

    std::fprintf(stderr, "[storage_bridge handleGetDelta] ENTER projectId=%s baseVersion=%d\n",
                 projectId.toStdString().c_str(), baseVersion);

    // partname 与 handleSaveDelta 一致
    const QString partName = QString::fromUtf8("delta__") + projectId;

    Neo4jPart conn(neo4jHost.toStdString().c_str(), neo4jPort, neo4jUser.toStdString().c_str(),
                   neo4jPassword.toStdString().c_str(), partName.toStdString());

    // 调用 C++ 层 delta 读取逻辑
    int latestVersion = 0;
    std::vector<std::string> bodyUuids;
    std::vector<std::string> satSegmentsByUuid;
    std::vector<std::string> deletedUuids;
    std::string errorStr;

    bool ok = handle_get_delta_from_neo4j(conn, baseVersion, latestVersion, bodyUuids, satSegmentsByUuid, deletedUuids,
                                          errorStr);

    if (!ok) {
        std::fprintf(stderr, "[storage_bridge handleGetDelta] FAILED: %s\n", errorStr.c_str());
        statusCode = 500;
        error = QString::fromUtf8(errorStr.c_str());
        return {};
    }

    std::fprintf(stderr, "[storage_bridge handleGetDelta] SUCCESS latestVersion=%d added=%zu deleted=%zu\n",
                 latestVersion, bodyUuids.size(), deletedUuids.size());

    // 如果 baseVersion >= latestVersion，返回空 delta
    if (baseVersion >= latestVersion) {
        std::fprintf(stderr, "[storage_bridge handleGetDelta] B端已是最新，无需 delta\n");
        QJsonObject response;
        response.insert("version", latestVersion);
        response.insert("delta_bodies", QJsonArray());
        response.insert("deleted_uuids", QJsonArray());
        return QJsonDocument(response).toJson(QJsonDocument::Compact);
    }

    // 构建 delta_bodies 数组：[{ "uuid": "...", "sat": "..." }, ...]
    QJsonArray deltaBodiesArr;
    for (size_t i = 0; i < bodyUuids.size() && i < satSegmentsByUuid.size(); ++i) {
        QJsonObject item;
        item.insert("uuid", QString::fromStdString(bodyUuids[i]));
        item.insert("sat", QString::fromStdString(satSegmentsByUuid[i]));
        deltaBodiesArr.append(item);
    }

    // deleted_uuids 直接来自 handle_get_delta_from_neo4j 的增量回放结果
    QJsonArray deletedUuidsArr;
    for (const std::string &u : deletedUuids) {
        deletedUuidsArr.append(QString::fromStdString(u));
    }

    QJsonObject response;
    response.insert("version", latestVersion);
    response.insert("delta_bodies", deltaBodiesArr);
    response.insert("deleted_uuids", deletedUuidsArr);
    return QJsonDocument(response).toJson(QJsonDocument::Compact);
}
