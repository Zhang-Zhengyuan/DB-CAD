#include "storage_bridge_internal.h"

using namespace dbcad::bridge;

QByteArray StorageBridgeService::handleCreateModel(const QString &projectId, const QByteArray &body, int &statusCode,
                                                   QString &error) {
    statusCode = 201;

    std::fprintf(stderr, "[storage_bridge handleCreateModel] ENTER projectId=%s bodyBytes=%d\n",
                 projectId.toStdString().c_str(), (int)body.size());

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

    std::fprintf(
        stderr,
        "[storage_bridge handleCreateModel] projectId=%s author=%s satLen=%d hasBaseVersion=%d baseVersion=%d\n",
        projectId.toStdString().c_str(), author.toStdString().c_str(), (int)sat.size(), (int)hasBaseVersion,
        baseVersion);

    if (author.isEmpty() || sat.isEmpty()) {
        statusCode = 400;
        error = QString::fromUtf8("author and content.sat are required");
        return {};
    }

    mg_map *params = mg_map_make_empty(1);
    mg_map_append(params, mg_string_make("id"), mg_value_make_string(projectId.toUtf8().constData()));

    QString dbError;
    const auto lookup = runQuery(neo4jHost, neo4jPort, neo4jUser, neo4jPassword,
                                 "MATCH (p:BridgeProject {id:$id}) OPTIONAL MATCH "
                                 "(p)-[:HAS_VERSION]->(v:BridgeVersion) RETURN p.id,max(v.version)",
                                 params, dbError);
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

    std::fprintf(stderr,
                 "[storage_bridge handleCreateModel] projectId=%s latestVersion=%d hasBaseVersion=%d baseVersion=%d -> "
                 "branch=%s\n",
                 projectId.toStdString().c_str(), latestVersion, (int)hasBaseVersion, baseVersion,
                 (!hasBaseVersion && latestVersion > 0)             ? "409-need-base"
                 : (hasBaseVersion && baseVersion != latestVersion) ? "409-conflict"
                                                                    : "create");

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
    // 把客户端传入的完整 content 序列化为 JSON 字符串存到 BridgeVersion.content_text 属性，
    // 否则后续 GET /projects/.../models/{version} 时拿不到 entity_graph / changes / sat_format
    // 等字段，server 端就无法把这个版本识别为 entity_graph 类型，
    // 所有接收端都会走 model_saved 路径触发清空+restore，丢失节点对齐信息。
    const QByteArray contentJsonBytes = QJsonDocument(content).toJson(QJsonDocument::Compact);
    const QString contentText = QString::fromUtf8(contentJsonBytes);

    params = mg_map_make_empty(6);
    mg_map_append(params, mg_string_make("project_id"), mg_value_make_string(projectId.toUtf8().constData()));
    mg_map_append(params, mg_string_make("version"), mg_value_make_integer(nextVersion));
    mg_map_append(params, mg_string_make("author"), mg_value_make_string(author.toUtf8().constData()));
    mg_map_append(params, mg_string_make("created_at"), mg_value_make_string(now.toUtf8().constData()));
    mg_map_append(params, mg_string_make("part_name"), mg_value_make_string(partName.toUtf8().constData()));
    mg_map_append(params, mg_string_make("content_text"), mg_value_make_string(contentText.toUtf8().constData()));

    const auto rows = runQuery(neo4jHost, neo4jPort, neo4jUser, neo4jPassword,
                               // 用 OPTIONAL MATCH 先检查 version 是否已存在：如果存在则 v 字段非空，
                               // 此时不要 CREATE，直接返回空结果让上层走 409 冲突路径。
                               // 这样可以避免两个并发请求都"看到"latest=v=1、都计算 nextVersion=v=2、
                               // 然后都执行 CREATE 写入两个 version=2 的节点。
                               "MATCH (p:BridgeProject {id:$project_id}) "
                               "OPTIONAL MATCH (p)-[:HAS_VERSION]->(existing:BridgeVersion {version:$version}) "
                               "WITH p, existing "
                               "WHERE existing IS NULL "
                               "SET p.updated_at=$created_at "
                               "CREATE (v:BridgeVersion "
                               "{project_id:$project_id,version:$version,author:$author,created_at:$created_at,part_"
                               "name:$part_name,content_text:$content_text}) "
                               "CREATE (p)-[:HAS_VERSION]->(v) "
                               "RETURN id(v),v.project_id,v.version,v.author,v.created_at",
                               params, dbError);
    mg_map_destroy(params);

    if (!dbError.isEmpty()) {
        statusCode = 500;
        error = dbError;
        return {};
    }
    if (rows.empty()) {
        // version 冲突：另一个并发请求已经写入了相同 version 号。
        // 返回 409 让上层 fastapi 重试逻辑拉最新 latest 后重新提交。
        std::fprintf(stderr,
                     "[storage_bridge handleCreateModel] projectId=%s nextVersion=%d CREATE returned empty (concurrent "
                     "conflict)\n",
                     projectId.toStdString().c_str(), nextVersion);
        statusCode = 409;
        error = QString::fromUtf8("Version %1 already exists (concurrent write conflict)").arg(nextVersion);
        return {};
    }
    if (rows.front().size() < 5) {
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

    std::fprintf(stderr, "[storage_bridge handleCreateModel] projectId=%s CREATED v=%d dbId=%lld author=%s\n",
                 projectId.toStdString().c_str(), nextVersion, (long long)dbId, author.toStdString().c_str());

    QJsonObject response;
    response.insert("id", static_cast<double>(dbId));
    response.insert("project_id", toQString(rows.front()[1]));
    response.insert("version", static_cast<int>(toInt64(rows.front()[2])));
    response.insert("author", toQString(rows.front()[3]));
    response.insert("created_at", toQString(rows.front()[4]));
    // 返回客户端传入的完整 content（包括 entity_graph / changes / sat / sat_format 等），
    // 不能只把 sat 字段透传——因为 entity_graph 是协作接收端用来做增量合并的关键数据，
    // 丢了 entity_graph 后服务端检测不到这个版本是 entity_graph 类型，
    // 后续 broadcast 会按 model_saved 路径走清空+restore，丢失所有节点对齐信息。
    response.insert("content", content);
    return QJsonDocument(response).toJson(QJsonDocument::Compact);
}
