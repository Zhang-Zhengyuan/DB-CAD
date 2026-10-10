#pragma once
// Private dependencies shared by MainWindow implementation units.
#include "mainwindow.h"
#include "pg_service.h"
#include "collab_session.h"
#include "entity_graph_serializer.h"
#include "entity_merge_engine.h"

#include <QActionGroup>
#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QGuiApplication>
#include <QThread>

#include <cstdio>
#include <exception>
#include <stdexcept>
#include <QInputDialog>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QProcess>
#include <QStringConverter>
#include <QSettings>
#include <QScopedValueRollback>
#include <QSet>
#include <QSignalBlocker>
#include <QStatusBar>
#include <QTemporaryFile>
#include <QTextStream>
#include <QToolBar>
#include <QDebug>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QListWidget>
#include <QCheckBox>
#include <QDockWidget>
#include <QTimer>
#include <QUuid>
#include <QUrl>
#include <QtWebSockets/QWebSocket>
#include <fstream>
#include <algorithm>
#include <functional>
#include <sstream>

#include "gme_dump_object.hxx"
#include "acis/include/bulletin.hxx"
#include "acis/include/attrib.hxx"
#include "acis/include/kernapi.hxx"
#include "acis/include/boolapi.hxx"
#include "acis/include/cstrapi.hxx"
#include "acis/include/insanity_list.hxx"
#include "acis/include/rnd_api.hxx"
#include "gme_user_data.hxx"
#include "window.h"
#include "acis/include/debug.hxx"

#include "acis/include/part_api.hxx"

#include "neo4j.hxx"
#include "access.hxx"
#include "common.hxx"
#include "backend_api_client.h"
#include <mgclient-1.4.2/mgclient.h>
#include <acis/include/ckoutcom.hxx>

std::string process(outcome &result);
int initialize_acis();
void terminate_acis(int level);

struct menu_struct {
    std::string name_cn = "菜单";                                       // 菜单项中文名/状态栏提示
    std::string name_en = "menu";                                       // 菜单项英文名/icon文件名.png
    bool checkable = false;                                             // 该菜单是否可以check
    OPERATOR_TYPES operatorType = OPERATOR_TYPES::OPERATOR_CONSTRUCTOR; // 该项对应的操作类型
    // 说明：-1表示读入实体；OPERATOR_TYPES::OPERATOR_SPERATOR，表示插入分隔符
    int subOperatorType = 0; // 该项对应的子操作类型
};
extern std::vector<menu_struct> menus_entities;
extern std::vector<menu_struct> menus_operators;
std::vector<std::string> read_config_lines(const std::string &filePath, int maxLines);
bool write_config_lines(const std::string &filePath, const std::vector<std::string> &lines, QString *error);
