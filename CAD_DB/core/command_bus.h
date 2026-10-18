#pragma once

// CommandBus —— 命令总线骨架（架构 v2 核心库，REFACTOR_PLAN M1.6）。
//
// 命令注册表：命令名 → 参数 schema → 执行器。insertElements/operation 等
// 入口改为注册命令，菜单由注册表生成。UI 与未来的 AI（M6）都是命令的
// 调用方，与"UI 与 AI 都是核心的客户端"的设计不变量对齐。
//
// 本阶段是骨架：注册/查找/dispatch（含必填参数校验），不引入队列与撤销
// 路由（那是 M2 命令级 undo 的事）。

#include <functional>
#include <map>
#include <vector>

#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <QStringList>

#include "kernel/model_types.h"

namespace dbcad {

class CommandBus {
public:
    struct ParamSpec {
        QString name;
        QString type;    // "integer" | "number" | "string" | "boolean" | "array" | "object"
        bool required = true;
        QJsonValue defaultValue;
    };

    // 命令分组：同组命令进同一子菜单/工具栏（保持既有菜单结构）。
    struct Group {
        QString id;
        QString title;          // 子菜单标题 / 工具栏名
        bool toolbarBreak = false; // 组前插入工具栏换行（原"换行符"分隔条）
    };

    struct Command {
        QString id;      // 唯一命令名，如 "insert.cube" / "operate.boolean.unite"
        QString category; // "insert" | "operate" | 自定义
        QString groupId; // 所属分组 id（空 = 顶层）
        QString label;   // 显示名（中文）
        QString icon;    // 图标资源名（name_en，不带扩展名）
        bool checkable = false;
        bool toolbarBreakBefore = false; // 该命令前插入工具栏换行（保持既有菜单布局）
        OPERATOR_TYPES operatorType = OPERATOR_TYPES::OPERATOR_CONSTRUCTOR;
        int subOperatorType = 0;
        QJsonArray paramSchema; // ParamSpec 数组（机器可读，供 AI/无头调用方）
        std::function<bool(const QJsonObject& params)> execute;
    };

    // 注册分组（按注册顺序生成菜单）；重复 id 返回 false。
    bool registerGroup(const QString& category, const Group& group);
    // 注册命令；id 重复或找不到 execute 返回 false。
    bool registerCommand(Command command);
    const Command* command(const QString& id) const;
    QStringList commandIds() const;
    // 按注册顺序返回某类别（或全部，category 为空）的命令。
    std::vector<const Command*> commands(const QString& category = QString()) const;
    const std::vector<Group>& groups(const QString& category) const;

    // 校验必填参数并执行；未知命令或参数缺失返回 false 并写 *error。
    bool dispatch(const QString& id, const QJsonObject& params = {}, QString* error = nullptr) const;

    static QJsonArray toJsonSchema(const std::vector<ParamSpec>& params);

private:
    std::map<QString, Command> commands_;          // id → command（查找）
    std::vector<QString> registrationOrder_;        // 菜单顺序
    std::map<QString, std::vector<Group>> groups_;  // category → groups
};

} // namespace dbcad
