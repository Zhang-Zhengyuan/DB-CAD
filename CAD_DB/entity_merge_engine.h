#pragma once

#include <QJsonObject>
#include <QList>
#include <QString>

struct CollaborativeEntity {
    QString uuid;
    QJsonObject node;
    QString sat;
};

struct EntityMergeConflict {
    QString uuid;
    CollaborativeEntity base;
    CollaborativeEntity local;
    CollaborativeEntity remote;
    QString kind;
};

struct EntityMergeResult {
    bool valid = false;
    bool localChanged = false;
    QList<CollaborativeEntity> entities;
    QList<EntityMergeConflict> conflicts;
    QString error;
};

QJsonObject collaborativeSnapshotGraph(const QList<CollaborativeEntity>& entities);
EntityMergeResult mergeCollaborativeSnapshots(const QJsonObject& base,
                                               const QJsonObject& local,
                                               const QJsonObject& remote);
