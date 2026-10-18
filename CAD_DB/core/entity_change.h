#pragma once

#include <QList>
#include <QString>
#include <QtGlobal>

namespace dbcad {

// 实体变更类型：组件级增量记账的枚举（线协议字符串 ADD/REMOVE/MODIFY 保持冻结）。
enum class EntityChangeType { ADD, REMOVE, MODIFY };

// 单条实体变更记录。几何属于组件图（entity graph），本记录只携带元数据。
struct EntityChange {
    QString uuid;
    QString name;
    QString entityType;
    EntityChangeType changeType = EntityChangeType::ADD;
    int entityIndex = 0;
    qint64 timestamp = 0;
};

} // namespace dbcad
