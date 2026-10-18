#include "core/command_bus.h"

#include <QJsonObject>
#include <QJsonValue>

namespace dbcad {

bool CommandBus::registerGroup(const QString& category, const Group& group) {
    auto& list = groups_[category];
    for (const auto& existing : list) {
        if (existing.id == group.id)
            return false;
    }
    list.push_back(group);
    return true;
}

bool CommandBus::registerCommand(Command command) {
    if (command.id.isEmpty() || !command.execute)
        return false;
    if (commands_.find(command.id) != commands_.end())
        return false;
    const QString id = command.id;
    commands_.emplace(id, std::move(command));
    registrationOrder_.push_back(id);
    return true;
}

const CommandBus::Command* CommandBus::command(const QString& id) const {
    const auto it = commands_.find(id);
    return it == commands_.end() ? nullptr : &it->second;
}

QStringList CommandBus::commandIds() const {
    QStringList ids;
    for (const auto& id : registrationOrder_)
        ids << id;
    return ids;
}

std::vector<const CommandBus::Command*> CommandBus::commands(const QString& category) const {
    std::vector<const Command*> result;
    for (const auto& id : registrationOrder_) {
        const auto it = commands_.find(id);
        if (it == commands_.end())
            continue;
        if (category.isEmpty() || it->second.category == category)
            result.push_back(&it->second);
    }
    return result;
}

const std::vector<CommandBus::Group>& CommandBus::groups(const QString& category) const {
    static const std::vector<Group> empty;
    const auto it = groups_.find(category);
    return it == groups_.end() ? empty : it->second;
}

bool CommandBus::dispatch(const QString& id, const QJsonObject& params, QString* error) const {
    const Command* cmd = command(id);
    if (cmd == nullptr) {
        if (error != nullptr)
            *error = QString::fromUtf8("未知命令：%1").arg(id);
        return false;
    }
    for (const QJsonValue& value : cmd->paramSchema) {
        const QJsonObject spec = value.toObject();
        if (!spec.value("required").toBool())
            continue;
        if (!params.contains(spec.value("name").toString())) {
            if (error != nullptr)
                *error = QString::fromUtf8("命令 %1 缺少必填参数：%2").arg(id, spec.value("name").toString());
            return false;
        }
    }
    return cmd->execute(params);
}

QJsonArray CommandBus::toJsonSchema(const std::vector<ParamSpec>& params) {
    QJsonArray schema;
    for (const auto& spec : params) {
        QJsonObject entry;
        entry.insert("name", spec.name);
        entry.insert("type", spec.type);
        entry.insert("required", spec.required);
        if (!spec.defaultValue.isUndefined())
            entry.insert("default", spec.defaultValue);
        schema.append(entry);
    }
    return schema;
}

} // namespace dbcad
