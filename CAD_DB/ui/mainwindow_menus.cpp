#include "mainwindow_internal.h"

// 命令注册（REFACTOR_PLAN M1.6）：菜单数据表改为 CommandBus 注册项，
// insert/operate 入口成为注册命令；菜单由注册表生成（见 mainwindow_actions.cpp）。
//
// 注意"线"组的工具栏换行：master 基线中"换行符"分隔条只做工具栏换行，
// 后续线实体仍归入"面"子菜单——为保持"现有菜单行为不变"，此处按原样
// 映射（线命令 groupId="faces"，首个线命令标记 toolbarBreakBefore）。

namespace {

struct MenuRow {
    const char* labelCn;
    const char* iconEn;
    bool checkable;
    OPERATOR_TYPES operatorType;
    int subOperatorType;
    const char* groupId;
    bool toolbarBreakBefore;
};

// 原 menus_entities 数据表（顺序即菜单顺序）。
const std::vector<MenuRow>& insertRows() {
    static const std::vector<MenuRow> rows = {
        {"立方体", "cube", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_CUBE, "bodies", false},
        {"球体", "sphere", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_SPHERE, "bodies", false},
        {"圆柱", "cylinder", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_CYLINDER, "bodies", false},
        {"圆锥", "cone", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_CONE, "bodies", false},
        {"圆环", "torus", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_TORUS, "bodies", false},
        {"棱柱", "prism", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_PRISM, "bodies", false},
        {"金字塔", "pyramid", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_PYRAMID, "bodies", false},
        {"摆动面", "wiggle", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_WIGGLE, "bodies", false},
        {"矩形面", "plane", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_PLANE, "faces", false},
        {"平行四边形面", "pplane", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_PPLANE, "faces", false},
        {"圆形面", "cplane", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_CPLANE, "faces", false},
        {"球面", "spheres", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_SPHERES, "faces", false},
        {"部分球面", "pspheres", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_PSPHERES, "faces", false},
        {"圆锥面", "conic", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_CONIC_SIDE, "faces", false},
        {"圆柱面", "cylinders", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_CYLINDER_SIDE, "faces", false},
        {"部分圆锥面", "pconic", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_PCONIC, "faces", false},
        {"圆环面", "toruss", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_TORUSS, "faces", false},
        {"部分圆环面", "ptoruss", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_PTORUSS, "faces", false},
        {"公式面", "laws", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_LAWS, "faces", false},
        {"B样条面-控制点", "bsplines_ctrlpts", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR,
         BASIC_ENTITIES::E_BSPLINE_CTRLPTS, "faces", false},
        {"B样条面-拟合", "bsplines_fit", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_BSPLINE_FIT, "faces", false},
        {"B样条面-插值", "bsplines_intrp", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR,
         BASIC_ENTITIES::E_BSPLINE_INTRP, "faces", false},
        // "换行符"分隔条：仅工具栏换行，线实体仍归"面"子菜单（保持 master 行为）。
        {"直线", "line", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_LINE, "faces", true},
        {"圆弧", "arc", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_ARC, "faces", false},
        {"椭圆", "ellipse", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_ELLIPSE, "faces", false},
        {"二次线", "conicc", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_CONIC, "faces", false},
        {"螺旋线", "helix", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_HELIX, "faces", false},
        {"阿基米德螺旋线", "spiral", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_SPIRAL, "faces", false},
        {"弹簧线", "spring", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_SPRING, "faces", false},
        {"方程线", "lawc", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_LAWC, "faces", false},
        {"贝塞尔曲线", "bezier", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_BEZIER, "faces", false},
        {"B样条线", "bsplinec", true, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_BSPLINE, "faces", false},
    };
    return rows;
}

// 原 menus_operators 数据表。
const std::vector<MenuRow>& operateRows() {
    using BO = dbcad::BooleanOperation;
    static const std::vector<MenuRow> rows = {
        {"求交", "intersector", false, OPERATOR_TYPES::OPERATOR_INTERSECTOR, 0, "g0", false},
        {"布尔并", "union", false, OPERATOR_TYPES::OPERATOR_BOOLEAN, static_cast<int>(BO::Unite), "g1", false},
        {"布尔交", "intersection", false, OPERATOR_TYPES::OPERATOR_BOOLEAN, static_cast<int>(BO::Intersect), "g1", false},
        {"布尔差", "subtraction", false, OPERATOR_TYPES::OPERATOR_BOOLEAN, static_cast<int>(BO::Subtract), "g1", false},
        {"去重", "unrepeat", false, OPERATOR_TYPES::OPERATOR_DEFEATURE, DEFEATURE_UNREPEAT, "g2", false},
    };
    return rows;
}

QString commandId(const char* category, const MenuRow& row) {
    return QString::fromUtf8("%1.%2").arg(QLatin1String(category), QLatin1String(row.iconEn));
}
} // namespace

void MainWindow::registerDefaultCommands() {
    auto& bus = *commandBus_;

    bus.registerGroup(QStringLiteral("insert"), {QStringLiteral("bodies"), tr("体"), false});
    bus.registerGroup(QStringLiteral("insert"), {QStringLiteral("faces"), tr("面"), false});

    for (const auto& row : insertRows()) {
        const QString id = commandId("insert", row);
        dbcad::CommandBus::Command command;
        command.id = id;
        command.category = QStringLiteral("insert");
        command.groupId = QString::fromUtf8(row.groupId);
        command.label = QString::fromUtf8(row.labelCn);
        command.icon = QString::fromUtf8(row.iconEn);
        command.checkable = row.checkable;
        command.operatorType = row.operatorType;
        command.subOperatorType = row.subOperatorType;
        command.toolbarBreakBefore = row.toolbarBreakBefore;
        command.paramSchema = bus.toJsonSchema({});
        const OPERATOR_TYPES ot = row.operatorType;
        const int sot = row.subOperatorType;
        command.execute = [this, ot, sot](const QJsonObject&) {
            insertElements(ot, sot);
            return true;
        };
        bus.registerCommand(std::move(command));
    }

    for (const auto& row : operateRows()) {
        const QString id = commandId("operate", row);
        dbcad::CommandBus::Command command;
        command.id = id;
        command.category = QStringLiteral("operate");
        command.groupId = QString::fromUtf8(row.groupId);
        command.label = QString::fromUtf8(row.labelCn);
        command.icon = QString::fromUtf8(row.iconEn);
        command.checkable = row.checkable;
        command.operatorType = row.operatorType;
        command.subOperatorType = row.subOperatorType;
        command.toolbarBreakBefore = false;
        command.paramSchema = bus.toJsonSchema({});
        const OPERATOR_TYPES ot = row.operatorType;
        const int sot = row.subOperatorType;
        command.execute = [this, ot, sot](const QJsonObject&) {
            operation(ot, sot);
            return true;
        };
        bus.registerCommand(std::move(command));
    }
}
