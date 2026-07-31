#include "entity_merge_engine.h"

#include <QHash>
#include <QJsonArray>
#include <QJsonValue>
#include <QSet>

namespace {
QHash<QString, CollaborativeEntity> indexGraph(const QJsonObject& graph) {
    QHash<QString, CollaborativeEntity> result;
    const QJsonArray nodes = graph.value("nodes").toArray();
    for (const QJsonValue& value : nodes) {
        const QJsonObject node = value.toObject();
        const QJsonObject props = node.value("props").toObject();
        const QString uuid = node.value("id").toString(props.value("uuid").toString());
        if (uuid.isEmpty()) {
            continue;
        }
        CollaborativeEntity entity;
        entity.uuid = uuid;
        entity.node = node;
        entity.sat = props.value("sat").toString();
        result.insert(uuid, entity);
    }
    return result;
}

bool sameEntity(const CollaborativeEntity& lhs, const CollaborativeEntity& rhs) {
    return lhs.node == rhs.node && lhs.sat == rhs.sat;
}

bool containsEntity(const QHash<QString, CollaborativeEntity>& graph, const QString& uuid) {
    return graph.constFind(uuid) != graph.constEnd();
}

CollaborativeEntity chooseEntity(const QHash<QString, CollaborativeEntity>& base,
                                 const QHash<QString, CollaborativeEntity>& local,
                                 const QHash<QString, CollaborativeEntity>& remote,
                                 const QString& uuid,
                                 EntityMergeResult* result) {
    const bool inBase = containsEntity(base, uuid);
    const bool inLocal = containsEntity(local, uuid);
    const bool inRemote = containsEntity(remote, uuid);
    const CollaborativeEntity empty;
    const CollaborativeEntity& b = inBase ? base[uuid] : empty;
    const CollaborativeEntity& l = inLocal ? local[uuid] : empty;
    const CollaborativeEntity& r = inRemote ? remote[uuid] : empty;
    const bool localChanged = inBase ? !sameEntity(b, l) : inLocal;
    const bool remoteChanged = inBase ? !sameEntity(b, r) : inRemote;

    if (localChanged && remoteChanged && !sameEntity(l, r)) {
        EntityMergeConflict conflict;
        conflict.uuid = uuid;
        conflict.base = b;
        conflict.local = l;
        conflict.remote = r;
        conflict.kind = !inLocal || !inRemote ? QStringLiteral("delete_modify") : QStringLiteral("modify_modify");
        result->conflicts.append(conflict);
        return empty;
    }
    if (localChanged) {
        result->localChanged = true;
        return l;
    }
    if (remoteChanged) {
        result->localChanged = true;
        return r;
    }
    if (inLocal) {
        return l;
    }
    if (inRemote) {
        return r;
    }
    return empty;
}
}

QJsonObject collaborativeSnapshotGraph(const QList<CollaborativeEntity>& entities) {
    QJsonArray nodes;
    for (const CollaborativeEntity& entity : entities) {
        QJsonObject node = entity.node;
        QJsonObject props = node.value("props").toObject();
        if (!entity.sat.isEmpty()) {
            props.insert("sat", entity.sat);
        }
        node.insert("id", entity.uuid);
        node.insert("props", props);
        nodes.append(node);
    }
    QJsonObject graph;
    graph.insert("nodes", nodes);
    graph.insert("rels", QJsonArray());
    return graph;
}

EntityMergeResult mergeCollaborativeSnapshots(const QJsonObject& base,
                                               const QJsonObject& local,
                                               const QJsonObject& remote) {
    EntityMergeResult result;
    const QHash<QString, CollaborativeEntity> baseEntities = indexGraph(base);
    const QHash<QString, CollaborativeEntity> localEntities = indexGraph(local);
    const QHash<QString, CollaborativeEntity> remoteEntities = indexGraph(remote);

    QSet<QString> uuids;
    for (auto it = baseEntities.constBegin(); it != baseEntities.constEnd(); ++it) uuids.insert(it.key());
    for (auto it = localEntities.constBegin(); it != localEntities.constEnd(); ++it) uuids.insert(it.key());
    for (auto it = remoteEntities.constBegin(); it != remoteEntities.constEnd(); ++it) uuids.insert(it.key());

    for (const QString& uuid : uuids) {
        CollaborativeEntity entity = chooseEntity(baseEntities, localEntities, remoteEntities, uuid, &result);
        if (!entity.uuid.isEmpty()) {
            result.entities.append(entity);
        }
    }
    result.valid = true;
    return result;
}
