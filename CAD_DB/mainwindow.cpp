#include "mainwindow.h"
#include "pg_service.h"
#include "collab_session.h"
#include "backend_api_client.h"
#include "entity_graph_serializer.h"


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
#include <QLabel>
#include <QDialogButtonBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QPushButton>
#include <QSpinBox>
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


std::string process(outcome& result);
int initialize_acis();
void terminate_acis(int level);

struct menu_struct {
    std::string name_cn = "菜单";                                        // 菜单项中文名/状态栏提示
    std::string name_en = "menu";                                        // 菜单项英文名/icon文件名.png
    bool checkable = false;                                              // 该菜单是否可以check
    OPERATOR_TYPES operatorType = OPERATOR_TYPES::OPERATOR_CONSTRUCTOR;  // 该项对应的操作类型
    // 说明：-1表示读入实体；OPERATOR_TYPES::OPERATOR_SPERATOR，表示插入分隔符
    int subOperatorType = 0;                                             // 该项对应的子操作类型
};

std::vector<menu_struct> menus_entities = {
  {"体",                      "Body",             false, OPERATOR_TYPES::OPERATOR_SPERATOR,    0                                                },
  {"立方体",                "cube",             false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_CUBE                           },
  {"球体",                   "sphere",           false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_SPHERE                         },
  {"圆柱",                   "cylinder",         false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_CYLINDER                       },
  {"圆锥",                   "cone",             false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_CONE                           },
  {"圆环",                   "torus",            false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_TORUS                          },
  {"棱柱",                   "prism",            false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_PRISM                          },
  {"金字塔",                "pyramid",          false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_PYRAMID                        },
  {"摆动面",                "wiggle",           false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_WIGGLE                         },
  {"面",                      "Face",             false, OPERATOR_TYPES::OPERATOR_SPERATOR,    0                                                },
  {"矩形面",                "plane",            false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_PLANE                          },
  {"平行四边形面",       "pplane",           false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_PPLANE                         },
  {"圆形面",                "cplane",           false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_CPLANE                         },
  {"球面",                   "spheres",          false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_SPHERES                        },
  {"部分球面",             "pspheres",         false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_PSPHERES                       },
  {"圆锥面",                "conic",            false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_CONIC_SIDE                     },
  {"圆柱面",                "cylinders",        false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_CYLINDER_SIDE                  },
  {"部分圆锥面",          "pconic",           false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_PCONIC                         },
  {"圆环面",                "toruss",           false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_TORUSS                         },
  {"部分圆环面",          "ptoruss",          false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_PTORUSS                        },
  {"公式面",                "laws",             false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_LAWS                           },
  {"B样条面-控制点",     "bsplines_ctrlpts", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_BSPLINE_CTRLPTS                },
  {"B样条面-拟合",        "bsplines_fit",     false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_BSPLINE_FIT                    },
  {"B样条面-插值",        "bsplines_intrp",   false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_BSPLINE_INTRP                  },
  {"线",                      "Edge",             false, OPERATOR_TYPES::OPERATOR_SPERATOR,    0                                                },
  {"换行符",                "line_break",       false, OPERATOR_TYPES::OPERATOR_SPERATOR,    1                                                },
  {"直线",                   "line",             false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_LINE                           },
  {"圆弧",                   "arc",              false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_ARC                            },
  {"椭圆",                   "ellipse",          false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_ELLIPSE                        },
  {"二次线",                "conicc",           false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_CONIC                          },
  {"螺旋线",                "helix",            false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_HELIX                          },
  {"阿基米德螺旋线",    "spiral",           false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_SPIRAL                         },
  {"弹簧线",                "spring",           false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_SPRING                         },
  {"方程线",                "lawc",             false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_LAWC                           },
  {"贝塞尔曲线",          "bezier",           false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_BEZIER                         },
  {"B样条线",               "bsplinec",         true,  OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_BSPLINE                        },
};

std::vector<menu_struct> menus_operators = {
  {"求交",          "intersector",  false, OPERATOR_TYPES::OPERATOR_INTERSECTOR, 0                     },
  {"分割线",       "separate",     false, OPERATOR_TYPES::OPERATOR_SPERATOR,    0                     },
  {"布尔并",       "union",        false, OPERATOR_TYPES::OPERATOR_BOOLEAN,     UNION                 },
  {"布尔交",       "intersection", false, OPERATOR_TYPES::OPERATOR_BOOLEAN,     INTERSECTION          },
  {"布尔差",       "subtraction",  false, OPERATOR_TYPES::OPERATOR_BOOLEAN,     SUBTRACTION           },
  {"分割线",       "separate",     false, OPERATOR_TYPES::OPERATOR_SPERATOR,    0                     },
  {"去重",          "unrepeat",     false, OPERATOR_TYPES::OPERATOR_DEFEATURE,   DEFEATURE_UNREPEAT    },
  //{"去除小特征", "remove_small", false, OPERATOR_TYPES::OPERATOR_DEFEATURE,   DEFEATURE_REMOVE_SMALL},
  //{"抽壳",          "shell",        false, OPERATOR_TYPES::OPERATOR_DEFEATURE,   DEFEATURE_SHELL       },
  //{"拆分",          "split",        false, OPERATOR_TYPES::OPERATOR_DEFEATURE,   DEFEATURE_SPLIT       },
  //{"包络",          "envelope",     false, OPERATOR_TYPES::OPERATOR_DEFEATURE,   DEFEATURE_ENVELOPE    },
  //{"Brep转CSG",      "brep2csg",     false, OPERATOR_TYPES::OPERATOR_DEFEATURE,   DEFEATURE_BREP2CSG    },
};

namespace {
std::string trim_copy(const std::string& input) {
    const size_t first = input.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    const size_t last = input.find_last_not_of(" \t\r\n");
    return input.substr(first, last - first + 1);
}

// Read up to maxLines from a config file. Tries, in order:
//   1. filePath as-is (absolute, or relative to current working dir)
//   2. <exe dir>/filePath
//   3. <exe dir>/../filePath
//   4. <exe dir>/../../filePath
std::vector<std::string> read_config_lines(const std::string& filePath, int maxLines) {
    std::vector<std::string> values;
    auto tryOpen = [&](const std::string& p) -> bool {
        std::ifstream fs(p, std::ios_base::in);
        if (!fs.is_open()) return false;
        std::string line;
        while (static_cast<int>(values.size()) < maxLines && std::getline(fs, line)) {
            values.push_back(trim_copy(line));
        }
        return true;
    };

    QStringList candidates;
    candidates << QString::fromStdString(filePath);
    const QString appDir = QApplication::applicationDirPath();
    candidates << QDir(appDir).filePath(QString::fromStdString(filePath));
    candidates << QDir(appDir).filePath(QString("..") + QDir::separator() + QString::fromStdString(filePath));
    candidates << QDir(appDir).filePath(QString("../..") + QDir::separator() + QString::fromStdString(filePath));

    for (const QString& c : candidates) {
        values.clear();
        if (tryOpen(c.toStdString())) break;
    }

    // pad to maxLines if the file existed but had fewer entries
    while (static_cast<int>(values.size()) < maxLines) {
        values.emplace_back("");
    }
    return values;
}

bool write_config_lines(const std::string& filePath, const std::vector<std::string>& lines, QString* error) {
    QStringList candidates;
    candidates << QString::fromStdString(filePath);
    const QString appDir = QApplication::applicationDirPath();
    candidates << QDir(appDir).filePath(QString::fromStdString(filePath));
    candidates << QDir(appDir).filePath(QString("..") + QDir::separator() + QString::fromStdString(filePath));
    candidates << QDir(appDir).filePath(QString("../..") + QDir::separator() + QString::fromStdString(filePath));

    for (const QString& c : candidates) {
        std::ofstream fs(c.toStdString(), std::ios_base::out | std::ios_base::trunc);
        if (!fs.is_open()) continue;
        for (const auto& line : lines) {
            fs << line << '\n';
        }
        return true;
    }
    if (error != nullptr) {
        *error = QObject::tr("无法写入配置文件：%1").arg(QString::fromStdString(filePath));
    }
    return false;
}
}

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags flags) : QMainWindow(parent, flags) {
    setObjectName("MainWindow");
    setWindowTitle("DBCAD");
    curWindow = nullptr;
    fastapi_client_id = QUuid::createUuid().toString(QUuid::WithoutBraces);

    fastapiSyncTimer = new QTimer(this);
    fastapiSyncTimer->setInterval(15000);
    connect(fastapiSyncTimer, &QTimer::timeout, this, &MainWindow::requestFastAPISyncNow);

    fastapiHeartbeatTimer = new QTimer(this);
    fastapiHeartbeatTimer->setInterval(10000);
    connect(fastapiHeartbeatTimer, &QTimer::timeout, this, [this]() {
        if (fastapiSyncSocket != nullptr && fastapiSyncSocket->isValid()) {
            fastapiSyncSocket->sendTextMessage("ping");
        }
    });

    fastapiReconnectTimer = new QTimer(this);
    fastapiReconnectTimer->setInterval(3000);
    connect(fastapiReconnectTimer, &QTimer::timeout, this, [this]() {
        QAction* checkedAct = setModeActGroup ? setModeActGroup->checkedAction() : nullptr;
        if (checkedAct == setFASTAPIModeAct && !fastapi_project_id.isEmpty()) {
            reconnectFastAPISync();
        } else {
            fastapiReconnectTimer->stop();
        }
    });

    fastapiPublishTimer = new QTimer(this);
    fastapiPublishTimer->setSingleShot(true);
    fastapiPublishTimer->setInterval(900);
    connect(fastapiPublishTimer, &QTimer::timeout, this, &MainWindow::publishFastAPIAutoSnapshot);

    // PostgreSQL service — initialized once, reuses connection across all operations.
    // Note: PgService::loaded is intentionally NOT connected here.
    // `pgLoadFromDatabaseToCurrent()` uses a SingleShotConnection lambda to route the
    // SAT text back into the calling flow; connecting a second handler here would
    // double-render. `loadFailed` IS connected permanently as a safety net.
    m_pgService = std::make_unique<PgService>();
    connect(m_pgService.get(), &PgService::initFailed, this, [](const QString& err) {
        QMessageBox::warning(nullptr, tr("PostgreSQL 初始化失败"), err);
    });
    connect(m_pgService.get(), &PgService::saved, this, &MainWindow::onPgSaved);
    connect(m_pgService.get(), &PgService::saveFailed, this, &MainWindow::onPgSaveFailed);
    connect(m_pgService.get(), &PgService::loadFailed, this, &MainWindow::onPgLoadFailed);
    connect(m_pgService.get(), &PgService::listed, this, &MainWindow::onPgListed);
    connect(m_pgService.get(), &PgService::listFailed, this, &MainWindow::onPgListFailed);
    connect(m_pgService.get(), &PgService::counted, this, &MainWindow::onPgCounted);
    connect(m_pgService.get(), &PgService::countFailed, this, &MainWindow::onPgCountFailed);
    connect(m_pgService.get(), &PgService::deleted, this, &MainWindow::onPgDeleted);
    connect(m_pgService.get(), &PgService::deleteFailed, this, &MainWindow::onPgDeleteFailed);
    // Initial menu state is refreshed after createActions() runs and the menu
    // actions exist; see updatePgMenuState() invocation at end of createActions().

    if (0 == initialize_acis()) {
        QMessageBox::warning(this, tr("异常"), tr("ACIS初始化失败"));
    }

    createActions();
    createStatusBar();

#ifndef QT_NO_SESSIONMANAGER
    connect(qApp, &QGuiApplication::commitDataRequest, this, &MainWindow::commitData);
#endif

    addWindow();

    // option_header* delete_forward_states_option = find_option("delete_forward_states");
    // delete_forward_states_option->push(FALSE);

    HISTORY_STREAM* new_hs = new HISTORY_STREAM();
    set_default_stream(new_hs);

    {
        auto neo4jValues = read_config_lines("neo4j_connect_info.conf", 4);
        if (!neo4jValues.empty()) {
            if (!neo4jValues[0].empty()) {
                neo4jdb_host = neo4jValues[0];
            }
            if (!neo4jValues[1].empty()) {
                try {
                    neo4jdb_port_bolt = std::stoi(neo4jValues[1]);
                } catch (...) {
                    neo4jdb_port_bolt = 7687;
                }
            }
            neo4jdb_username = neo4jValues[2];
            neo4jdb_password = neo4jValues[3];
        } else {
            neo4jdb_host = "127.0.0.1";
            neo4jdb_port_bolt = 7687;
            neo4jdb_username = "neo4j";
        }

        auto fastapiValues = read_config_lines("fastapi_connect_info.conf", 3);
        if (!fastapiValues.empty()) {
            fastapi_base_url = fastapiValues[0];
            fastapi_author = fastapiValues[1];
            fastapi_password = fastapiValues[2];
        }

        auto bridgeValues = read_config_lines("storage_bridge_connect_info.conf", 2);
        if (!bridgeValues.empty()) {
            if (!bridgeValues[0].empty()) {
                bridge_bind_host = bridgeValues[0];
            }
            if (!bridgeValues[1].empty()) {
                try {
                    bridge_bind_port = std::stoi(bridgeValues[1]);
                } catch (...) {
                    bridge_bind_port = 8100;
                }
            }
        }

        if (bridge_bind_host.empty()) {
            bridge_bind_host = "127.0.0.1";
        }
        if (bridge_bind_port <= 0) {
            bridge_bind_port = 8100;
        }
    }

    mg_init();

    // 彻底重置历史系统
    HISTORY_STREAM *hs = get_default_stream(false);
    if (hs) {
        // 删除所有 delta states，但保留流对象
        delete_all_delta_states(hs, true);   // true 表示保留流
        hs->clear();                          // 清空流内容
    }
    else {
        hs = get_default_stream(true);        // 创建新流
    }
    // 重置全局历史管理器状态
    initialize_delta_states();
    // 确保根状态为空（便于后续记录）
    DELTA_STATE *root_state = nullptr;
    api_ensure_empty_root_state(hs, root_state);

    CollabSession::instance().bindLegacyFields(
        fastapi_model_version,
        fastapi_pending_remote_version,
        fastapiLastPublishReason,
        fastapiPendingSubmitRequestId,
        fastapiSubmitInFlight,
        fastapiLocalDirtyDuringSubmit,
        fastapiApplyingRemoteSnapshot,
        fastapiPublishingSnapshot,
        fastapiLocalDirty
    );
    qDebug().noquote() << "[CollabSession] bound:" << CollabSession::instance().dump(CollabSession::Event::Bound);
    CollabSession::instance().setDebugEnabled(true);
    CollabSession::instance().setMinDumpIntervalMs(0);
    CollabSession::instance().setEventMinInterval(CollabSession::Event::WsMessage, 200);

    // 永久启用实体变更追踪：每次 addEntity/removeEntity/modifyEntity 都会记录到 pendingEntityChanges，
    // 直到 publishFastAPIAutoSnapshot / submitEntityGraphIncremental 把变更推到服务器并清空列表。
    // 这是 entity_graph 增量提交路径生效的前提。
    beginEntityChangeTracking();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (maybeSave()) {
        event->accept();
        if (bridgeProcess != nullptr) {
            bridgeStopRequested = true;
            if (bridgeProcess->state() != QProcess::NotRunning) {
                bridgeProcess->terminate();
                if (!bridgeProcess->waitForFinished(3000)) {
                    bridgeProcess->kill();
                    bridgeProcess->waitForFinished(3000);
                }
            }
        }
        disconnectFastAPISync();
        if (curWindow) {
            curWindow->close();
        }
        mg_finalize();
        terminate_acis(2);
    } else {
        event->ignore();
    }
}

void MainWindow::setFastAPIConnectInfo() {
    auto values = read_config_lines("fastapi_connect_info.conf", 3);
    if (!values.empty()) {
        if (!values[0].empty()) {
            fastapi_base_url = values[0];
        }
        if (!values[1].empty()) {
            fastapi_author = values[1];
        }
        fastapi_password = values[2];
    }

    QDialog dialog(this);
    dialog.setWindowTitle(tr("FastAPI连接配置"));
    dialog.setMinimumSize(420, 220);
    QFormLayout form(&dialog);

    QLineEdit baseUrlEdit(&dialog);
    baseUrlEdit.setPlaceholderText("http://127.0.0.1:8000");
    baseUrlEdit.setText(QString::fromStdString(fastapi_base_url));
    form.addRow(tr("FastAPI地址:"), &baseUrlEdit);

    QLineEdit authorEdit(&dialog);
    authorEdit.setPlaceholderText("dbcad-exe");
    authorEdit.setText(QString::fromStdString(fastapi_author));
    form.addRow(tr("作者名:"), &authorEdit);

    QLineEdit passwordEdit(&dialog);
    passwordEdit.setEchoMode(QLineEdit::Password);
    passwordEdit.setPlaceholderText("可留空（后端未启用密码时）");
    passwordEdit.setText(QString::fromStdString(fastapi_password));
    form.addRow(tr("连接密码:"), &passwordEdit);

    QPushButton testButton(tr("测试连接"), &dialog);
    form.addRow(&testButton);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    QObject::connect(&testButton, &QPushButton::clicked, &dialog, [&]() {
        const QString url = baseUrlEdit.text().trimmed();
        if (url.isEmpty()) {
            QMessageBox::warning(this, tr("FastAPI连接测试"), tr("FastAPI地址不能为空"));
            return;
        }

        BackendApiClient client(url, authorEdit.text().trimmed(), passwordEdit.text());
        auto project = client.getProjectByName("__dbcad_connection_test__");
        Q_UNUSED(project);
        const QString err = client.lastError();
        if (!err.isEmpty()) {
            QMessageBox::warning(this, tr("FastAPI连接测试"), tr("连接失败：%1").arg(err));
        } else {
            QMessageBox::information(this, tr("FastAPI连接测试"), tr("连接成功。"));
        }
    });

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    const QString baseUrl = baseUrlEdit.text().trimmed();
    if (baseUrl.isEmpty()) {
        QMessageBox::warning(this, tr("FastAPI连接配置"), tr("FastAPI地址不能为空"));
        return;
    }

    fastapi_base_url = baseUrl.toStdString();
    fastapi_author = authorEdit.text().trimmed().toStdString();
    fastapi_password = passwordEdit.text().trimmed().toStdString();

    QString writeError;
    if (!write_config_lines(
            "fastapi_connect_info.conf",
            { fastapi_base_url, fastapi_author, fastapi_password },
            &writeError)) {
        QMessageBox::warning(this, tr("FastAPI连接配置"), writeError);
        return;
    }

    statusBar()->showMessage(tr("FastAPI连接信息已更新"), 2000);
}

void MainWindow::setBridgeConnectInfo() {
    auto values = read_config_lines("storage_bridge_connect_info.conf", 2);
    if (!values.empty()) {
        if (!values[0].empty()) {
            bridge_bind_host = values[0];
        }
        if (!values[1].empty()) {
            try {
                bridge_bind_port = std::stoi(values[1]);
            } catch (...) {
                bridge_bind_port = 8100;
            }
        }
    } else {
        if (bridge_bind_host.empty()) {
            bridge_bind_host = "127.0.0.1";
        }
        if (bridge_bind_port <= 0) {
            bridge_bind_port = 8100;
        }
    }

    QDialog dialog(this);
    dialog.setWindowTitle(tr("Bridge模式配置"));
    dialog.setMinimumSize(420, 180);
    QFormLayout form(&dialog);

    QLineEdit hostEdit(&dialog);
    hostEdit.setText(QString::fromStdString(bridge_bind_host));
    form.addRow(tr("Bridge监听地址:"), &hostEdit);

    QSpinBox portEdit(&dialog);
    portEdit.setMinimum(1);
    portEdit.setMaximum(65535);
    portEdit.setValue(bridge_bind_port <= 0 ? 8100 : bridge_bind_port);
    form.addRow(tr("Bridge监听端口:"), &portEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    const QString host = hostEdit.text().trimmed();
    if (host.isEmpty()) {
        QMessageBox::warning(this, tr("Bridge模式配置"), tr("Bridge监听地址不能为空"));
        return;
    }

    bridge_bind_host = host.toStdString();
    bridge_bind_port = portEdit.value();

    QString writeError;
    if (!write_config_lines(
            "storage_bridge_connect_info.conf",
            { bridge_bind_host, std::to_string(bridge_bind_port) },
            &writeError)) {
        QMessageBox::warning(this, tr("Bridge模式配置"), writeError);
        return;
    }

    statusBar()->showMessage(tr("Bridge模式配置已更新"), 2000);
}

void MainWindow::toggleBridgeMode(bool checked) {
    if (checked) {
        if (bridgeProcess != nullptr && bridgeProcess->state() != QProcess::NotRunning) {
            statusBar()->showMessage(tr("Bridge模式已在运行"), 2000);
            return;
        }

        if (neo4jdb_host.empty()) {
            QMessageBox::warning(this, tr("Bridge模式"), tr("请先配置neo4j连接信息。"));
            if (toggleBridgeModeAct != nullptr) {
                QSignalBlocker blocker(toggleBridgeModeAct);
                toggleBridgeModeAct->setChecked(false);
            }
            return;
        }

        QString program = QCoreApplication::applicationFilePath();
        QStringList args;
        args << "--storage-bridge"
             << "--bridge-host" << QString::fromStdString(bridge_bind_host)
             << "--bridge-port" << QString::number(bridge_bind_port)
             << "--neo4j-host" << QString::fromStdString(neo4jdb_host)
             << "--neo4j-port" << QString::number(neo4jdb_port_bolt)
             << "--neo4j-user" << QString::fromStdString(neo4jdb_username)
             << "--neo4j-password" << QString::fromStdString(neo4jdb_password);

        bridgeProcess = new QProcess(this);
        bridgeProcess->setWorkingDirectory(QCoreApplication::applicationDirPath());
        bridgeProcess->setProcessChannelMode(QProcess::SeparateChannels);

        connect(bridgeProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            [this](int exitCode, QProcess::ExitStatus exitStatus) {
                const bool expectedStop = bridgeStopRequested;
                bridgeStopRequested = false;

                QString stderrText;
                if (bridgeProcess != nullptr) {
                    stderrText = QString::fromLocal8Bit(bridgeProcess->readAllStandardError()).trimmed();
                    bridgeProcess->deleteLater();
                    bridgeProcess = nullptr;
                }

                if (toggleBridgeModeAct != nullptr) {
                    QSignalBlocker blocker(toggleBridgeModeAct);
                    toggleBridgeModeAct->setChecked(false);
                }

                if (!expectedStop) {
                    QString msg = tr("Bridge进程已退出（exit=%1）。").arg(exitCode);
                    if (exitStatus == QProcess::CrashExit) {
                        msg += tr(" 进程发生崩溃。");
                    }
                    if (!stderrText.isEmpty()) {
                        msg += tr("\n错误输出：%1").arg(stderrText);
                    }
                    QMessageBox::warning(this, tr("Bridge模式"), msg);
                } else {
                    statusBar()->showMessage(tr("Bridge模式已停止"), 2000);
                }
            });

        bridgeStopRequested = false;
        bridgeProcess->start(program, args);
        if (!bridgeProcess->waitForStarted(5000)) {
            const QString error = bridgeProcess->errorString();
            bridgeProcess->deleteLater();
            bridgeProcess = nullptr;
            if (toggleBridgeModeAct != nullptr) {
                QSignalBlocker blocker(toggleBridgeModeAct);
                toggleBridgeModeAct->setChecked(false);
            }
            QMessageBox::warning(this, tr("Bridge模式"), tr("启动Bridge进程失败：%1").arg(error));
            return;
        }

        statusBar()->showMessage(tr("Bridge模式已启动，PID=%1").arg(bridgeProcess->processId()), 3000);
    } else {
        if (bridgeProcess == nullptr) {
            return;
        }
        bridgeStopRequested = true;
        if (bridgeProcess->state() != QProcess::NotRunning) {
            bridgeProcess->terminate();
            if (!bridgeProcess->waitForFinished(3000)) {
                bridgeProcess->kill();
            }
        }
    }
}

// All PostgreSQL environment loading moved to PgService::loadConfig().

void MainWindow::setPGConnectInfo() {
    auto values = read_config_lines("pg_connect_info.conf", 5);
    QDialog dlg(this);
    dlg.setWindowTitle(tr("设置 PostgreSQL 连接信息"));
    dlg.setWindowFlags(dlg.windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    QFormLayout form(&dlg);
    QLineEdit hostEdit;  hostEdit.setText(QString::fromStdString(values[0]));   hostEdit.setPlaceholderText("127.0.0.1");
    QLineEdit portEdit;  portEdit.setText(QString::fromStdString(values[1]));   portEdit.setPlaceholderText("5432");
    QLineEdit userEdit;  userEdit.setText(QString::fromStdString(values[2]));   userEdit.setPlaceholderText("postgres");
    QLineEdit passEdit;  passEdit.setText(QString::fromStdString(values[3]));
                         passEdit.setEchoMode(QLineEdit::Password);              passEdit.setPlaceholderText("(password)");
    QLineEdit dbEdit;    dbEdit.setText(QString::fromStdString(values[4]));     dbEdit.setPlaceholderText("dbcad_demo");
    form.addRow(tr("Host:"),     &hostEdit);
    form.addRow(tr("Port:"),     &portEdit);
    form.addRow(tr("User:"),     &userEdit);
    form.addRow(tr("Password:"), &passEdit);
    form.addRow(tr("Database:"), &dbEdit);

    QDialogButtonBox bb(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    form.addRow(&bb);
    QObject::connect(&bb, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&bb, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) return;

    QString err;
    if (!write_config_lines("pg_connect_info.conf",
            { hostEdit.text().toStdString(), portEdit.text().toStdString(),
              userEdit.text().toStdString(), passEdit.text().toStdString(),
              dbEdit.text().toStdString() }, &err)) {
        QMessageBox::warning(this, tr("DBCAD"), err);
        return;
    }
    m_pgService = std::make_unique<PgService>();
    connect(m_pgService.get(), &PgService::initFailed, this, [](const QString& err) {
        QMessageBox::warning(nullptr, tr("PostgreSQL 初始化失败"), err);
    });
    connect(m_pgService.get(), &PgService::saved, this, &MainWindow::onPgSaved);
    connect(m_pgService.get(), &PgService::saveFailed, this, &MainWindow::onPgSaveFailed);
    connect(m_pgService.get(), &PgService::loadFailed, this, &MainWindow::onPgLoadFailed);
    connect(m_pgService.get(), &PgService::listed, this, &MainWindow::onPgListed);
    connect(m_pgService.get(), &PgService::listFailed, this, &MainWindow::onPgListFailed);
    connect(m_pgService.get(), &PgService::counted, this, &MainWindow::onPgCounted);
    connect(m_pgService.get(), &PgService::countFailed, this, &MainWindow::onPgCountFailed);
    connect(m_pgService.get(), &PgService::deleted, this, &MainWindow::onPgDeleted);
    connect(m_pgService.get(), &PgService::deleteFailed, this, &MainWindow::onPgDeleteFailed);

    if (m_pgService->isInitialized()) {
        statusBar()->showMessage(tr("PostgreSQL 连接信息已更新"), 3000);
    } else {
        statusBar()->showMessage(tr("PostgreSQL 配置已保存，但初始化失败"), 4000);
    }
    updatePgMenuState();
}

void MainWindow::pgSaveCurrentToDatabase() {
    if (curWindow == nullptr) {
        QMessageBox::information(this, tr("DBCAD"), tr("请先打开或绘制一个实体，再保存。"));
        return;
    }
    if (!m_pgService->isInitialized()) {
        QMessageBox::warning(this, tr("DBCAD"), tr("PostgreSQL 未初始化，请检查连接配置。"));
        return;
    }

    bool ok = false;
    const QString defaultName = QString("dbcad_pg_%1")
        .arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));
    const QString name = QInputDialog::getText(this,
        tr("保存到 PostgreSQL"),
        tr("请输入零件名(将作为 dbcad_pg_demo.name，唯一):"),
        QLineEdit::Normal, defaultName, &ok,
        this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    if (!ok || name.trimmed().isEmpty()) return;

    // Export current ACIS model to a SAT string via temp file.
    QTemporaryFile tempFile(QDir::tempPath() + "/dbcad_pg_save_XXXXXX.sat");
    tempFile.setAutoRemove(true);
    if (!tempFile.open()) {
        QMessageBox::warning(this, tr("DBCAD"), tr("无法创建临时 SAT 文件。"));
        return;
    }
    const QString tempPath = tempFile.fileName();
    FILE* f = nullptr;
#ifdef _MSC_VER
    f = _wfopen(reinterpret_cast<const wchar_t*>(tempPath.utf16()), L"wb");
#else
    f = fopen(tempPath.toStdString().c_str(), "wb");
#endif
    if (!f) {
        tempFile.close();
        QMessageBox::warning(this, tr("DBCAD"), tr("无法写入临时 SAT 文件。"));
        return;
    }
    {
        API_NOP_BEGIN;
        api_save_version(2, 0);
        FileInfo fileinfo;
        fileinfo.set_units(1.0);
        fileinfo.set_product_id("dbcad_pg_save");
        outcome result = api_set_file_info((FileIdent | FileUnits), fileinfo);
        result = api_set_int_option("sequence_save_files", 1);
        ENTITY_LIST el;
        acis_get_noattrib_toplevel_active_entities(el);
        result = api_save_entity_list(f, true, el);
        API_NOP_END;
        fclose(f);
        if (!result.ok()) {
            tempFile.close();
            QMessageBox::warning(this, tr("DBCAD"), tr("导出当前模型为 SAT 失败。"));
            return;
        }
    }
    tempFile.close();

    // Read SAT text into memory (avoid a second temp-file round-trip).
    QFile satFile(tempPath);
    if (!satFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("DBCAD"), tr("无法读取导出的 SAT 文件。"));
        return;
    }
    const QString satText = QString::fromUtf8(satFile.readAll());
    satFile.close();

    m_pgService->saveSat(name, satText);
    statusBar()->showMessage(tr("正在保存到 PostgreSQL..."), 3000);
}

void MainWindow::pgLoadFromDatabaseToCurrent() {
    if (!m_pgService->isInitialized()) {
        QMessageBox::warning(this, tr("DBCAD"), tr("PostgreSQL 未初始化，请检查连接配置。"));
        return;
    }

    bool ok = false;
    const QString name = QInputDialog::getText(this,
        tr("从 PostgreSQL 加载"),
        tr("请输入要加载的零件名:"),
        QLineEdit::Normal, QString(), &ok,
        this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    if (!ok || name.trimmed().isEmpty()) return;

    // Connect the loaded signal once; disconnect after first emission.
    connect(m_pgService.get(), &PgService::loaded, this,
        [this, name](const QString& satText) {
            disconnect(m_pgService.get(), &PgService::loaded, this, nullptr);
            disconnect(m_pgService.get(), &PgService::loadFailed, this, nullptr);

            if (curWindow == nullptr) addWindow();
            curWindow->clear();

            QTemporaryFile tempFile(QDir::tempPath() + "/dbcad_pg_load_XXXXXX.sat");
            tempFile.setAutoRemove(true);
            if (!tempFile.open()) {
                QMessageBox::warning(this, tr("DBCAD"), tr("无法创建临时 SAT 文件。"));
                return;
            }
            const QString tempPath = tempFile.fileName();
            tempFile.write(satText.toUtf8());
            tempFile.close();

            FILE* f = nullptr;
#ifdef _MSC_VER
            f = _wfopen(reinterpret_cast<const wchar_t*>(tempPath.utf16()), L"r");
#else
            f = fopen(tempPath.toStdString().c_str(), "r");
#endif
            if (!f) {
                QMessageBox::warning(this, tr("DBCAD"), tr("无法读取已下载的临时 SAT 文件。"));
                return;
            }
            {
                ENTITY_LIST el;
                API_BEGIN;
                api_save_version(2, 0);
                outcome result = api_restore_entity_list(f, true, el);
                API_END;
                fclose(f);
                if (!result.ok()) {
                    QMessageBox::warning(this, tr("DBCAD"), tr("ACIS 解析 SAT 失败。"));
                    return;
                }
                for (int i = 0; i < el.count(); i++) {
                    curWindow->addEntity(el[i], tr("PG 导入实体%1").arg(i).toStdString(), -1);
                }
            }
            QMessageBox::information(this, tr("DBCAD"),
                tr("已从 PostgreSQL 加载:\n%1").arg(name));
            statusBar()->showMessage(tr("已从 PostgreSQL 加载: %1").arg(name), 3000);
        }, Qt::SingleShotConnection);

    connect(m_pgService.get(), &PgService::loadFailed, this,
        [this](const QString& err) {
            disconnect(m_pgService.get(), &PgService::loaded, this, nullptr);
            disconnect(m_pgService.get(), &PgService::loadFailed, this, nullptr);
            QMessageBox::warning(this, tr("DBCAD"),
                tr("从 PostgreSQL 加载失败:\n%1").arg(err));
            statusBar()->showMessage(tr("从 PostgreSQL 加载失败"), 4000);
        }, Qt::SingleShotConnection);

    m_pgService->loadSat(name);
}

void MainWindow::pgListPartsInDatabase() {
    if (!m_pgService->isInitialized()) {
        QMessageBox::warning(this, tr("DBCAD"), tr("PostgreSQL 未初始化，请检查连接配置。"));
        return;
    }
    m_pgService->listParts(200);
}

void MainWindow::onPgSaved(const QString& name, qint64 sizeBytes, qint64) {
    QMessageBox::information(this, tr("DBCAD"),
        tr("已保存到 PostgreSQL:\n%1\n大小: %2 bytes").arg(name).arg(sizeBytes));
    statusBar()->showMessage(tr("已保存到 PostgreSQL: %1").arg(name), 3000);
}

void MainWindow::onPgSaveFailed(const QString& error) {
    QMessageBox::warning(this, tr("DBCAD"),
        tr("保存到 PostgreSQL 失败:\n%1").arg(error));
    statusBar()->showMessage(tr("保存到 PostgreSQL 失败"), 4000);
}

void MainWindow::onPgLoadFailed(const QString& error) {
    QMessageBox::warning(this, tr("DBCAD"),
        tr("从 PostgreSQL 加载失败:\n%1").arg(error));
    statusBar()->showMessage(tr("从 PostgreSQL 加载失败"), 4000);
}

void MainWindow::onPgCounted(qint64 count) {
    statusBar()->showMessage(tr("PostgreSQL 已存零件数: %1").arg(count), 3000);
}

void MainWindow::onPgCountFailed(const QString& error) {
    QMessageBox::warning(this, tr("DBCAD"),
        tr("统计 PostgreSQL 零件数失败:\n%1").arg(error));
    statusBar()->showMessage(tr("统计 PostgreSQL 零件数失败"), 4000);
}

void MainWindow::onPgDeleted(const QString& name) {
    statusBar()->showMessage(tr("已从 PostgreSQL 删除: %1").arg(name), 3000);
}

void MainWindow::onPgDeleteFailed(const QString& error) {
    QMessageBox::warning(this, tr("DBCAD"),
        tr("从 PostgreSQL 删除失败:\n%1").arg(error));
    statusBar()->showMessage(tr("从 PostgreSQL 删除失败"), 4000);
}

void MainWindow::onPgListed(const QVector<QString>& names,
                            const QVector<qint64>&,
                            const QVector<qint64>& sizes,
                            const QVector<QString>& updatedAts) {
    if (names.isEmpty()) {
        QMessageBox::information(this, tr("PostgreSQL 已存零件"), tr("数据库中暂无零件。"));
        return;
    }
    QString msg;
    for (int i = 0; i < names.size(); ++i) {
        msg += tr("%1 | %2 bytes | %3\n")
                   .arg(names[i], -30)
                   .arg(sizes[i])
                   .arg(updatedAts[i]);
    }
    QMessageBox::information(this, tr("PostgreSQL 已存零件 (%1 个)").arg(names.size()), msg);
    statusBar()->showMessage(tr("PostgreSQL 已存零件列表已显示"), 2000);
}

void MainWindow::onPgListFailed(const QString& error) {
    QMessageBox::warning(this, tr("DBCAD"),
        tr("列出 PostgreSQL 零件失败:\n%1").arg(error));
}

void MainWindow::updatePgMenuState() {
    const bool ready = (m_pgService != nullptr && m_pgService->isInitialized());
    if (pgSaveAct != nullptr)   pgSaveAct->setEnabled(ready);
    if (pgLoadAct != nullptr)   pgLoadAct->setEnabled(ready);
    if (pgListAct != nullptr)   pgListAct->setEnabled(ready);
    if (pgCountAct != nullptr)  pgCountAct->setEnabled(ready);
    if (pgDeleteAct != nullptr) pgDeleteAct->setEnabled(ready);
}

void MainWindow::pgCountPartsInDatabase() {
    if (m_pgService == nullptr || !m_pgService->isInitialized()) {
        QMessageBox::warning(this, tr("DBCAD"), tr("PostgreSQL 未初始化，请检查连接配置。"));
        return;
    }
    m_pgService->countParts();
}

void MainWindow::pgDeleteByNameFromDatabase() {
    if (m_pgService == nullptr || !m_pgService->isInitialized()) {
        QMessageBox::warning(this, tr("DBCAD"), tr("PostgreSQL 未初始化，请检查连接配置。"));
        return;
    }

    bool ok = false;
    const QString name = QInputDialog::getText(this,
        tr("从 PostgreSQL 删除"),
        tr("请输入要删除的零件名:"),
        QLineEdit::Normal, QString(), &ok,
        this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    if (!ok || name.trimmed().isEmpty()) return;

    if (QMessageBox::question(this, tr("从 PostgreSQL 删除"),
            tr("确定删除 PostgreSQL 中的零件 “%1” 吗？此操作不可撤销。").arg(name),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    m_pgService->deleteByName(name);
}

bool MainWindow::restoreFastAPIModelFromSat(const QString& satContent) {
    if (curWindow == nullptr) {
        addWindow();
    }

    QScopedValueRollback<bool> remoteSnapshotGuard(fastapiApplyingRemoteSnapshot, true);
    CollabSession::instance().onApplyStart();

    QTemporaryFile tempFile(QDir::tempPath() + "/dbcad_load_XXXXXX.sat");
    tempFile.setAutoRemove(true);
    if (!tempFile.open()) {
        QMessageBox::warning(this, tr("DBCAD"), tr("无法创建临时文件用于恢复FastAPI模型"));
        return false;
    }

    QByteArray satBytes = satContent.toUtf8();
    if (tempFile.write(satBytes) != satBytes.size()) {
        QMessageBox::warning(this, tr("DBCAD"), tr("写入临时SAT文件失败"));
        return false;
    }

    tempFile.flush();
    std::string tempPath = tempFile.fileName().toStdString();
    FILE* f = fopen(tempPath.c_str(), "r");
    if (!f) {
        QMessageBox::warning(this, tr("DBCAD"), tr("无法读取临时SAT文件"));
        return false;
    }

    ENTITY_LIST el;
    API_BEGIN;
    api_save_version(2, 0);
    result = api_restore_entity_list(f, true, el);
    API_END;
    fclose(f);

    qDebug().noquote() << "[Collab] restoreFastAPIModelFromSat: after api_restore_entity_list el.count()=" << el.count();
    for (int i = 0; i < el.count(); i++) {
        ENTITY* e = el[i];
        qDebug().noquote() << "[Collab] restoreFastAPIModelFromSat: entity[" << i << "] ptr=" << e
                           << "isBODY=" << (e ? is_BODY(e) : 0);
    }

    if (el.count() == 0) {
        acis_get_noattrib_toplevel_active_entities(el);
    }

    if (el.count() == 0) {
        QMessageBox::warning(this, tr("DBCAD"), tr("远程模型已恢复，但未检测到可渲染实体。请检查SAT内容或存储桥输出。"));
        return false;
    }

    for (int i = 0; i < el.count(); i++) {
        curWindow->addEntity(el[i], tr("导入(FastAPI)实体%1").arg(i).toStdString(), -1);
    }
    curWindow->updateMeshData();
    return true;
}

// Pull apply 入口：clear 本地画布 + 整个 SAT 文本替换。
// 跟 master 分支 applyFastAPIRemoteSat 完全等价（除了我们额外把版本号更新交给 caller）。
// 冲突合并：submit_rejected 时会备份本地 SAT 到 fastapiConflictLocalSatBackup，
// Pull 应答到达后，如果备份存在，在 restore 完成后弹出对话框让用户选择如何处理。
bool MainWindow::applyRemoteSatSnapshot(const QString& satContent, const QString& reason) {
    Q_UNUSED(reason);
    if (curWindow == nullptr) {
        return false;
    }
    // 保存视角，避免 apply 后镜头跳回原点。
    const GLWidget::ViewState viewState = curWindow->getViewState();
    curWindow->clear();

    // 在 restore 之前把 fastapiApplyingRemoteSnapshot 设为 true，这样 restore 过程中
    // 的一切 addEntity/removeEntity 都不会被记录到 pendingEntityChanges。
    // 注意：restoreFastAPIModelFromSat 内部也会设一次（QScopedValueRollback），双重保险。
    CollabSession::instance().onApplyStart();

    if (!restoreFastAPIModelFromSat(satContent)) {
        CollabSession::instance().onApplyEnd();
        return false;
    }
    CollabSession::instance().onApplyEnd();

    curWindow->setViewState(viewState);

    // 冲突合并：submit_rejected 时备份了本地 SAT，Pull 到达后 offer 给用户。
    if (!fastapiConflictLocalSatBackup.isEmpty() && !isShowingConflictMergeDialog) {
        isShowingConflictMergeDialog = true;
        QString backup = fastapiConflictLocalSatBackup;
        fastapiConflictLocalSatBackup.clear();

        int choice = QMessageBox::question(
            this,
            tr("冲突合并"),
            tr("您的本地修改与远端版本冲突（远端已被其他客户端更新）。\n\n"
               "是否将您本地的修改合并到最新版本？\n"
               "  • 选择「Yes」：将本地修改追加到远端最新版本\n"
               "  • 选择「No」：丢弃本地修改，只保留远端最新版本"),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);

        isShowingConflictMergeDialog = false;

        if (choice == QMessageBox::Yes) {
            // 把本地备份的 body 追加到 restore 后的画布中。
            // 此时 isTrackingEntityChanges=true 但 CollabSession::isApplyingRemoteSnapshot()=false
            // (apply 已结束)，所以 recordEntityAdded 会正常记录追加的 body 为新 ADD 变更。
            // 但不要在这里触发 scheduleFastAPIAutoPublish，留给用户下一步操作。
            if (!backup.isEmpty()) {
                // 用和 restoreFastAPIModelFromSat 一样的模式：保持 QTemporaryFile 在作用域内，
                // 避免 autoRemove 在 fopen 之前就把文件删了。
                QScopedPointer<QTemporaryFile> tf(new QTemporaryFile(QDir::tempPath() + "/dbcad_merge_backup_XXXXXX.sat"));
                tf->setAutoRemove(true);
                if (tf->open()) {
                    QByteArray backupBytes = backup.toUtf8();
                    if (tf->write(backupBytes) == backupBytes.size()) {
                        tf->flush();
                        FILE* f = fopen(tf->fileName().toStdString().c_str(), "r");
                        if (f) {
                            ENTITY_LIST backupEl;
                            API_BEGIN;
                            api_save_version(2, 0);
                            result = api_restore_entity_list(f, true, backupEl);
                            API_END;
                            fclose(f);
                            if (backupEl.count() > 0) {
                                for (int i = 0; i < backupEl.count(); i++) {
                                    curWindow->addEntity(backupEl[i],
                                        tr("冲突合并(本地)实体%1").arg(i).toStdString(), -1);
                                }
                                curWindow->updateMeshData();
                                qDebug().noquote() << "[Collab] Conflict merge: added" << backupEl.count()
                                                   << "backup bodies, total now:" << curWindow->getEntityTree().size();
                            }
                        }
                    }
                }
            }
            // 合并后的画布与远端版本不再一致，标记为 modified，这样用户下一步 Ctrl+S
            // 或 Push 时会把合并结果推上去。
            curWindow->setIsModified(true);
        } else {
            // 用户选择丢弃本地修改，保持 clear+restore 后的状态。
            curWindow->setIsModified(false);
        }
    }

    return true;
}

void MainWindow::disconnectFastAPISync() {
    CollabSession::instance().onDisconnected();
    if (fastapiSyncTimer != nullptr) {
        fastapiSyncTimer->stop();
    }
    if (fastapiHeartbeatTimer != nullptr) {
        fastapiHeartbeatTimer->stop();
    }
    if (fastapiPublishTimer != nullptr) {
        fastapiPublishTimer->stop();
    }
    fastapi_collaborators.clear();
    if (fastapiReconnectTimer != nullptr) {
        fastapiReconnectTimer->stop();
    }

    if (fastapiSyncSocket != nullptr) {
        fastapiSyncSocket->close();
        fastapiSyncSocket->deleteLater();
        fastapiSyncSocket = nullptr;
    }
    setCollabConnectionState(tr("未连接"));
    updateCollabPanelUi();
}

// ========== 增量协作支持实现 ==========

void MainWindow::beginEntityChangeTracking() {
    isTrackingEntityChanges = true;
    entityChangeTrackingStartTime = QDateTime::currentMSecsSinceEpoch();
    pendingEntityChanges.clear();
}

void MainWindow::recordEntityAdded(const QString& uuid, const QString& name, const QString& entityType, int index, const QString& sat) {
    if (!isTrackingEntityChanges) return;
    MainWindow::EntityChange change;
    change.uuid = uuid;
    change.name = name;
    change.entityType = entityType;
    change.changeType = MainWindow::EntityChangeType::ADD;
    change.entityIndex = index;
    change.timestamp = QDateTime::currentMSecsSinceEpoch();
    change.sat = sat;
    pendingEntityChanges.append(change);
    entityIndexToUuid[index] = uuid;
}

void MainWindow::recordEntityRemoved(int index) {
    if (!isTrackingEntityChanges) return;
    // apply remote snapshot 期间 Window::clear() 会触发本函数，需要过滤掉，避免
    // 把"远端实体在 apply 时被 clear"错误地记成本地 REMOVE 变更。
    auto& session = CollabSession::instance();
    if (session.isApplyingRemoteSnapshot() || session.isPublishingSnapshot()) {
        return;
    }
    QString uuid = entityIndexToUuid.value(index, "");
    MainWindow::EntityChange change;
    change.uuid = uuid;
    change.name = "";
    change.entityType = "";
    change.changeType = MainWindow::EntityChangeType::REMOVE;
    change.entityIndex = index;
    change.timestamp = QDateTime::currentMSecsSinceEpoch();
    pendingEntityChanges.append(change);
    entityIndexToUuid.remove(index);
}

void MainWindow::recordEntityModified(int index) {
    if (!isTrackingEntityChanges) return;
    auto& session = CollabSession::instance();
    if (session.isApplyingRemoteSnapshot() || session.isPublishingSnapshot()) {
        return;
    }
    QString uuid = entityIndexToUuid.value(index, "");
    if (uuid.isEmpty()) return;
    MainWindow::EntityChange change;
    change.uuid = uuid;
    change.name = "";
    change.entityType = "";
    change.changeType = MainWindow::EntityChangeType::MODIFY;
    change.entityIndex = index;
    change.timestamp = QDateTime::currentMSecsSinceEpoch();
    pendingEntityChanges.append(change);
}

QList<MainWindow::EntityChange> MainWindow::endEntityChangeTracking() {
    isTrackingEntityChanges = false;
    return pendingEntityChanges;
}

void MainWindow::clearEntityChanges() {
    pendingEntityChanges.clear();
    isTrackingEntityChanges = false;
}

QString MainWindow::exportEntityGraphToJson() {
    if (curWindow == nullptr) return "{}";

    QJsonObject root;
    QJsonArray nodesArray;
    QJsonArray relsArray;

    const auto& entityTree = curWindow->getEntityTree();
    for (const auto& eti : entityTree) {
        QJsonObject node;
        node["id"] = QString::fromStdString(eti.uuid);
        QString entityType = QString::fromStdString(eti.name);
        QJsonArray labels;
        labels.append(entityType);
        node["labels"] = labels;

        // 实体属性
        QJsonObject props;
        props["index"] = eti.index;
        props["name"] = QString::fromStdString(eti.name);
        props["operatorType"] = static_cast<int>(eti.operatorType);
        props["subOperatorType"] = eti.subOperatorType;

        // 变换信息：SPAtransf 序列化。
        // 这里只导出 translation 分量（3 个标量），接收端用 entity_graph 重建几何时不需要完整 4x4 矩阵。
        QJsonArray transArr;
        SPAvector tVec = eti.trans.translation();
        transArr.append(tVec.x());
        transArr.append(tVec.y());
        transArr.append(tVec.z());
        props["transform"] = transArr;

        // 依赖信息
        QJsonArray depsArr;
        for (int dep : eti.index_base) {
            depsArr.append(dep);
        }
        props["index_base"] = depsArr;

        // 支持该实体的其他实体
        QJsonArray supportArr;
        for (int sup : eti.index_support) {
            supportArr.append(sup);
        }
        props["index_support"] = supportArr;

        props["visible"] = eti.visible;
        props["displayType"] = static_cast<int>(eti.displayType);

        node["props"] = props;
        nodesArray.append(node);

        // 记录索引到UUID的映射
        entityIndexToUuid[eti.index] = QString::fromStdString(eti.uuid);
    }

    // 生成关系（基于依赖）
    for (const auto& eti : entityTree) {
        for (int depIdx : eti.index_base) {
            QString depUuid = entityIndexToUuid.value(depIdx, "");
            if (!depUuid.isEmpty() && !eti.uuid.empty()) {
                QJsonObject rel;
                rel["type"] = "DEPENDS_ON";
                rel["start"] = QString::fromStdString(eti.uuid);
                rel["end"] = depUuid;
                relsArray.append(rel);
            }
        }
    }

    root["nodes"] = nodesArray;
    root["rels"] = relsArray;

    return QString::fromUtf8(QJsonDocument(root).toJson(QJsonDocument::Compact));
}

QString MainWindow::exportEntityChangesToJson(const QList<MainWindow::EntityChange>& changes) {
    QJsonArray changesArray;
    for (const auto& change : changes) {
        QJsonObject obj;
        obj["uuid"] = change.uuid;
        obj["name"] = change.name;
        obj["entityType"] = change.entityType;
        switch (change.changeType) {
            case MainWindow::EntityChangeType::ADD: obj["changeType"] = "ADD"; break;
            case MainWindow::EntityChangeType::REMOVE: obj["changeType"] = "REMOVE"; break;
            case MainWindow::EntityChangeType::MODIFY: obj["changeType"] = "MODIFY"; break;
        }
        obj["entityIndex"] = change.entityIndex;
        obj["timestamp"] = change.timestamp;
        // ADD 变更附带该 body 的 SAT 文本；接收端用 acis_restore_entity_list 重建后 addEntity。
        // REMOVE/MODIFY 没有 sat 段。
        if (!change.sat.isEmpty()) {
            obj["sat"] = change.sat;
        }
        changesArray.append(obj);
    }
    QJsonObject root;
    root["changes"] = changesArray;
    root["trackingStartTime"] = entityChangeTrackingStartTime;
    root["exportTime"] = QDateTime::currentMSecsSinceEpoch();
    return QString::fromUtf8(QJsonDocument(root).toJson(QJsonDocument::Compact));
}

bool MainWindow::submitEntityGraphIncremental(const QString& entityGraphJson, const QString& changesJson, const QString& reason) {
    auto& session = CollabSession::instance();
    CollabSession::SubmitDecision decision = session.tryBeginSubmit(reason);

    if (decision.kind != CollabSession::SubmitDecision::Allow) {
        statusBar()->showMessage(decision.reason, 5000);
        return false;
    }

    if (fastapiSyncSocket == nullptr || !fastapiSyncSocket->isValid()) {
        session.rollbackSubmit();
        statusBar()->showMessage(tr("协作通道未连接"), 5000);
        return false;
    }

    const QString author = QString::fromStdString(fastapi_author).trimmed();
    const QString requestId = decision.requestId;

    // 后端持久化走 storage_bridge，bridge 端校验 content.sat 必须存在
    // （与 submit_model 共享同一持久化路径）。所以即便走 entity_graph 增量提交，
    // 也必须带一份完整的 SAT 全量文本作为 content.sat；接收端用 entity_graph 增量合并。
    QString fullSat;
    if (!exportCurrentModelToSat(&fullSat, nullptr) || fullSat.isEmpty()) {
        session.rollbackSubmit();
        statusBar()->showMessage(tr("导出本地模型 SAT 失败，无法推送"), 5000);
        return false;
    }
    qDebug().noquote() << "[Collab] submitEntityGraphIncremental: fullSat.size=" << fullSat.size()
                       << "entityGraphJson.size=" << entityGraphJson.size()
                       << "changesJson.size=" << changesJson.size();

    QJsonObject content;
    content.insert("sat", fullSat);
    const QJsonDocument entityGraphDoc = QJsonDocument::fromJson(entityGraphJson.toUtf8());
    const QJsonDocument changesDoc = QJsonDocument::fromJson(changesJson.toUtf8());
    if (entityGraphDoc.isObject()) {
        content.insert("entity_graph", entityGraphDoc.object());
    }
    if (changesDoc.isObject()) {
        content.insert("changes", changesDoc.object());
    }

    // 旧 entity_graph（ETI 级别：只有 index_base 依赖关系）的 Neo4j 持久化是非必须的，
    // 因为它本身只用于 git-like 增量 diff，不携带 ACIS 拓扑几何。所以这里不调
    // saveEntityGraph()（neo4j_entity_store 期望的是完整 ACIS 拓扑）。
    // ACIS 拓扑走 submitACISEntityGraph() 的路径，那里有正确的 egVersion 注入。

    QJsonObject payload;
    payload.insert("type", "submit_entity_graph");
    payload.insert("project_id", fastapi_project_id);
    payload.insert("request_id", requestId);
    payload.insert("author", author.isEmpty() ? QString::fromUtf8("dbcad-exe") : author);
    payload.insert("content", content);
    payload.insert("reason", reason.isEmpty() ? QString::fromUtf8("local-change") : reason);
    if (session.modelVersion() > 0) {
        payload.insert("base_version", session.modelVersion());
    } else {
        payload.insert("base_version", QJsonValue::Null);
    }

    fastapiSyncSocket->sendTextMessage(QString::fromUtf8(QJsonDocument(payload).toJson(QJsonDocument::Compact)));
    statusBar()->showMessage(tr("正在提交增量协作变更..."), 1500);
    updateCollabPanelUi();
    return true;
}

bool MainWindow::applyRemoteEntityGraphIncremental(const QString& remoteEntityGraphJson, const QString& remoteChangesJson, const QString& reason) {
    qDebug().noquote() << "[Collab][DEBUG] >>> applyRemoteEntityGraphIncremental ENTER reason=" << reason
                       << "entityGraphJson.size=" << remoteEntityGraphJson.size()
                       << "changesJson.size=" << remoteChangesJson.size();
    if (curWindow == nullptr || fastapi_project_id.isEmpty()) {
        qDebug().noquote() << "[Collab][DEBUG] <<< applyRemoteEntityGraphIncremental EXIT (no curWindow/project)";
        return false;
    }

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(remoteEntityGraphJson.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        statusBar()->showMessage(tr("远端实体图解析失败"), 5000);
        qDebug().noquote() << "[Collab][DEBUG] <<< applyRemoteEntityGraphIncremental EXIT (json parse fail)";
        return false;
    }

    // 增量合并关键策略：
    // - 收集本地所有 body uuid
    // - 对每个远端 ADD 变更：如果 uuid 不在本地 → 用变更携带的 sat 文本 acis_restore 后 addEntity
    // - 对每个远端 REMOVE 变更：保留本地新增（不删除本地未上传的实体）
    // - 不 clear 本地画布
    const auto& localTree = curWindow->getEntityTree();
    QSet<QString> localUuids;
    for (const auto& eti : localTree) {
        localUuids.insert(QString::fromStdString(eti.uuid));
    }
    qDebug().noquote() << "[Collab][DEBUG] apply: localEntityCount=" << localTree.size();

    int appliedAdd = 0;
    int skippedAdd = 0;
    int skippedRemoteRemove = 0;
    int failedAdd = 0;

    // 解析 changes（如果有）
    if (!remoteChangesJson.isEmpty()) {
        QJsonDocument cdoc = QJsonDocument::fromJson(remoteChangesJson.toUtf8());
        if (cdoc.isObject()) {
            const QJsonArray changes = cdoc.object().value("changes").toArray();
            qDebug().noquote() << "[Collab][DEBUG] apply: changes.size=" << changes.size();
            int changeIdx = 0;
            for (const QJsonValue& v : changes) {
                const QJsonObject ch = v.toObject();
                const QString uuid = ch.value("uuid").toString();
                const QString changeType = ch.value("changeType").toString();
                qDebug().noquote() << "[Collab][DEBUG] apply: change[" << changeIdx << "] type=" << changeType << "uuid=" << uuid;
                if (changeType == "ADD") {
                    if (uuid.isEmpty() || localUuids.contains(uuid)) {
                        // 本地已有该 uuid（可能是之前 apply 时加入的），跳过。
                        qDebug().noquote() << "[Collab][DEBUG] apply: change[" << changeIdx << "] skip (uuid empty or local has it)";
                        ++skippedAdd;
                        ++changeIdx;
                        continue;
                    }
                    const QString sat = ch.value("sat").toString();
                    if (sat.isEmpty()) {
                        // ADD 但没有 SAT 文本（提交方忘记塞），无法重建。
                        qWarning() << "[Collab] ADD change missing 'sat' for uuid=" << uuid;
                        ++failedAdd;
                        ++changeIdx;
                        continue;
                    }
                    // 把 SAT 文本写到临时文件并恢复为 ENTITY_LIST，然后从 BODY 提取 top-level body addEntity。
                    // 注意：之前用 QTemporaryFile + QFile out + acis_restore_entity_list(filename, ...) 三步，
                    // 在 Windows 临时目录里 fopen_s 偶发失败 —— 怀疑是 OneDrive/防病毒扫描临时文件时短暂占用。
                    // 现在改用一次性 fopen("wb") 写入 + fflush + fclose + fopen("rb") 再传给 acis_restore_entity_list(FILE*, ...)，
                    // 减少文件状态被外部干扰的概率。
                    QTemporaryFile satTmp(QDir::tempPath() + "/dbcad_apply_XXXXXX.sat");
                    satTmp.setAutoRemove(true);
                    if (!satTmp.open()) {
                        ++failedAdd;
                        ++changeIdx;
                        continue;
                    }
                    const QString satPath = satTmp.fileName();
                    satTmp.close();
                    // satTmp 在本 frame 结束前不会析构 → 文件不会被删除，但为了绕开 satTmp 的句柄关联
                    // 直接用 C stdio 自己开。
                    std::string satPathStd = satPath.toStdString();
                    FILE* writeFile = nullptr;
                    if (fopen_s(&writeFile, satPathStd.c_str(), "wb") != 0 || writeFile == nullptr) {
                        qWarning().noquote() << "[Collab] fopen_s(wb) failed for path=" << satPath;
                        ++failedAdd;
                        ++changeIdx;
                        continue;
                    }
                    const QByteArray satBytes = sat.toUtf8();
                    const size_t wroteBytes = std::fwrite(satBytes.constData(), 1, satBytes.size(), writeFile);
                    std::fflush(writeFile);
                    std::fclose(writeFile);
                    writeFile = nullptr;
                    if (wroteBytes != static_cast<size_t>(satBytes.size())) {
                        qWarning().noquote() << "[Collab] sat fwrite short for uuid=" << uuid
                                             << "wrote=" << wroteBytes << "expected=" << satBytes.size();
                        ++failedAdd;
                        ++changeIdx;
                        continue;
                    }
                    // 写完后立即显式校验文件确实存在且大小正确；line 35 log 提示可能存在 OneDrive
                    // / 防病毒等外部进程短暂占用，给文件 SIZE 校验加一个短暂重试来容忍这种瞬态。
                    bool fileReady = false;
                    for (int attempt = 0; attempt < 20; ++attempt) {
                        QFileInfo fi(satPath);
                        if (fi.exists() && static_cast<size_t>(fi.size()) == static_cast<size_t>(satBytes.size())) {
                            fileReady = true;
                            break;
                        }
                        // Sleep 50ms 后重试（最多 1 秒容忍临时文件被防病毒扫描占用）。
                        QThread::msleep(50);
                    }
                    if (!fileReady) {
                        QFileInfo fi(satPath);
                        qWarning().noquote() << "[Collab] sat temp file not ready for uuid=" << uuid
                                             << "exists=" << fi.exists() << "size=" << fi.size()
                                             << "expected=" << satBytes.size() << "path=" << satPath;
                        ++failedAdd;
                        ++changeIdx;
                        continue;
                    }
                    qDebug().noquote() << "[Collab][DEBUG] apply: change[" << changeIdx << "] before acis_restore, sat.size=" << sat.size();
                    ENTITY_LIST restored;
                    bool restoreOk = false;
                    // 包裹 try/catch：acis_restore_entity_list 内部用 myerror() 抛 std::runtime_error
                    // (例如打开临时 SAT 文件失败)，API_BEGIN/END 在 DBCAD 当前实现下不会捕获
                    // std::exception，异常会一路穿透到 Qt WS 回调并触发 std::terminate → abort()。
                    // 这里吞掉异常并计入 failedAdd，避免一个失败的 ADD 变更把整个客户端拖崩。
                    FILE* readFile = nullptr;
                    // 同样给"rb 打开"加短暂重试，Windows 上偶尔第一发 fopen_s 失败但第二发就能成功。
                    bool opened = false;
                    for (int attempt = 0; attempt < 20; ++attempt) {
                        errno_t err = fopen_s(&readFile, satPathStd.c_str(), "rb");
                        if (err == 0 && readFile != nullptr) { opened = true; break; }
                        QThread::msleep(50);
                    }
                    if (!opened) {
                        qWarning().noquote() << "[Collab] fopen_s(rb) failed after retries for path=" << satPath;
                        ++failedAdd;
                        ++changeIdx;
                        continue;
                    }
                    try {
                        acis_restore_entity_list(restored, readFile, 2, 0, true);
                        restoreOk = true;
                    } catch (const std::exception& e) {
                        qWarning().noquote() << "[Collab] acis_restore_entity_list threw for uuid=" << uuid
                                             << "what=" << e.what();
                        restored.clear();
                    } catch (...) {
                        qWarning().noquote() << "[Collab] acis_restore_entity_list threw unknown exception for uuid=" << uuid;
                        restored.clear();
                    }
                    if (readFile != nullptr) {
                        std::fclose(readFile);
                    }
                    qDebug().noquote() << "[Collab][DEBUG] apply: change[" << changeIdx << "] after acis_restore, count=" << restored.count();
                    if (!restoreOk) {
                        ++failedAdd;
                        ++changeIdx;
                        continue;
                    }
                    if (restored.count() == 0) {
                        qWarning() << "[Collab] acis_restore_entity_list produced 0 entities for uuid=" << uuid;
                        ++failedAdd;
                        ++changeIdx;
                        continue;
                    }
                    // 取第一个 top-level body 调 addEntity，让 ETI 注册到本地 tree。
                    ENTITY* restoredEntity = restored[0];
                    const QString name = ch.value("name").toString();
                    qDebug().noquote() << "[Collab][DEBUG] apply: change[" << changeIdx << "] before addEntity, name=" << name;
                    // 同样保护 addEntity：内部会调 updateMeshData → CreateMeshFromEntity → 任何
                    // ACIS API 失败抛 std::exception 都可能穿透 WS 回调触发 abort。
                    try {
                        curWindow->addEntity(restoredEntity, name.toStdString(), 0);
                    } catch (const std::exception& e) {
                        qWarning().noquote() << "[Collab] addEntity threw for uuid=" << uuid
                                             << "what=" << e.what();
                        ++failedAdd;
                        ++changeIdx;
                        continue;
                    } catch (...) {
                        qWarning().noquote() << "[Collab] addEntity threw unknown exception for uuid=" << uuid;
                        ++failedAdd;
                        ++changeIdx;
                        continue;
                    }
                    qDebug().noquote() << "[Collab][DEBUG] apply: change[" << changeIdx << "] after addEntity OK";
                    localUuids.insert(uuid);
                    ++appliedAdd;
                } else if (changeType == "REMOVE") {
                    // 永远保留本地新增（用户在本地加的、还没 push 的实体不能被远端强制删除）。
                    ++skippedRemoteRemove;
                }
                ++changeIdx;
                // MODIFY 当前不处理：接收端不需要实时同步属性变更（同步后会变 stale 但不影响显示）。
            }
        }
    }

    QString msg = tr("已应用远端增量：ADD=%1 跳过=%2 远端REMOVE保留本地=%3 失败=%4")
        .arg(appliedAdd).arg(skippedAdd).arg(skippedRemoteRemove).arg(failedAdd);
    statusBar()->showMessage(msg, 4000);
    qDebug().noquote() << "[Collab][DEBUG] <<< applyRemoteEntityGraphIncremental EXIT:" << msg;
    // 关键修复：只有真正至少成功 addEntity 了一个 ADD 变更才返回 true。返回 true 会让
    // 上层 tryBeginApplyRemote 把 modelVersion 推到 remoteVersion；返回 false 则让上层
    // 走 rollbackApply + onRemotePending，这样后续相同 remoteVersion 的 sync_now 应答
    // 仍能被再次尝试（之前 failedAdd>0 但返回 true 导致 v 直接被推到 1，Pull 永远进不来）。
    return appliedAdd > 0;
}

void MainWindow::requestFastAPISyncNow() {
    qDebug().noquote() << "[Collab][DEBUG] >>> requestFastAPISyncNow socket=" << (fastapiSyncSocket ? "exists" : "null")
                       << "valid=" << (fastapiSyncSocket && fastapiSyncSocket->isValid() ? "yes" : "no");
    if (fastapiSyncSocket != nullptr && fastapiSyncSocket->isValid()) {
        fastapiSyncSocket->sendTextMessage("sync_now");
        statusBar()->showMessage(tr("已请求服务器返回最新版本"), 1500);
    } else {
        qDebug().noquote() << "[Collab][DEBUG] <<< requestFastAPISyncNow EXIT (socket not valid, cannot send sync_now)";
    }
}

void MainWindow::notifyModelChangedForCollaboration() {
    auto& session = CollabSession::instance();

    if (session.isApplyingRemoteSnapshot() || session.isPublishingSnapshot()) {
        return;
    }

    QAction* checkedAct = setModeActGroup ? setModeActGroup->checkedAction() : nullptr;
    if (checkedAct != setFASTAPIModeAct) {
        return;
    }

    if (fastapi_project_id.isEmpty() || fastapi_project_name.isEmpty()) {
        return;
    }

    // 不再自动 publish / 自动 schedule：用户改动时只标记 LocalDirty，
    // 由协作面板的「Push」按钮手动触发。详见 COLLABORATION_TECHNICAL_ROADMAP。
    if (session.pendingRemoteVersion() > session.modelVersion()) {
        statusBar()->showMessage(tr("远端有未拉取版本，请先点击「拉取(Pull)」再「推送(Push)」本地修改"), 4000);
        session.onRemotePending(session.pendingRemoteVersion());
    }
    if (session.isSubmitInFlight()) {
        session.setLastPublishReason(tr("local-change"));
        session.onUserEditDuringInFlight();
    } else {
        session.onUserEdit();
    }
    updateCollabPanelUi();
}

void MainWindow::scheduleFastAPIAutoPublish(const QString& reason) {
    // 已废弃：不再使用 900ms 防抖定时器自动发布，保留此函数仅为兼容既有调用点，
    // 内部直接转交给 publishFastAPIAutoSnapshot 即时执行（仅由 Push 按钮调用）。
    auto& session = CollabSession::instance();

    if (curWindow == nullptr || fastapi_project_id.isEmpty()) {
        return;
    }
    if (session.isApplyingRemoteSnapshot() || session.isPublishingSnapshot()) {
        return;
    }
    if (session.isSubmitInFlight()) {
        // 提交中：标记 dirty，不重复提交
        session.setLastPublishReason(reason);
        updateCollabPanelUi();
        return;
    }
    if (session.pendingRemoteVersion() > session.modelVersion()) {
        statusBar()->showMessage(tr("远端有未拉取版本，请先点击「拉取(Pull)」再「推送(Push)」"), 4000);
        updateCollabPanelUi();
        return;
    }

    session.setLastPublishReason(reason);
    publishFastAPIAutoSnapshot();
}

void MainWindow::onCollabPushButtonClicked() {
    if (curWindow == nullptr || fastapi_project_id.isEmpty()) {
        statusBar()->showMessage(tr("未连接或未打开项目，无法推送"), 3000);
        return;
    }
    auto& session = CollabSession::instance();
    if (session.pendingRemoteVersion() > session.modelVersion()) {
        QMessageBox::warning(this, tr("协作推送"), tr("远端有未拉取的版本（v%1 → v%2），请先点击「拉取(Pull)」再「推送(Push)」")
            .arg(session.modelVersion()).arg(session.pendingRemoteVersion()));
        return;
    }
    if (pendingEntityChanges.isEmpty()) {
        statusBar()->showMessage(tr("没有未推送的本地修改"), 2000);
        return;
    }
    // 直接调 publishFastAPIAutoSnapshot：它会优先用 entity_graph 路径，
    // 把每个 ADD body 的独立 SAT 文本一并发出。
    scheduleFastAPIAutoPublish(tr("manual-push"));
}

void MainWindow::onCollabPullButtonClicked() {
    qDebug().noquote() << "[Collab][DEBUG] >>> onCollabPullButtonClicked ENTER curWindow=" << (curWindow ? "ok" : "null")
                       << "fastapi_project_id=" << fastapi_project_id
                       << "socket=" << (fastapiSyncSocket ? "exists" : "null")
                       << "socketValid=" << (fastapiSyncSocket && fastapiSyncSocket->isValid() ? "yes" : "no");
    if (curWindow == nullptr || fastapi_project_id.isEmpty()) {
        statusBar()->showMessage(tr("未连接或未打开项目，无法拉取"), 3000);
        return;
    }
    auto& session = CollabSession::instance();
    // 注意：不要在这里用 pendingRemoteVersion 判断"已是最新"。
    // 用户主动点 Pull = "把 server 上当前最新的 entity_graph 拉下来"，即便本地
    // pending == model == 0（首次进来没收到任何推送），也允许 Pull 去 server 取一次最新版本。
    // 收到 entity_graph_saved 后若 remoteVersion <= modelVersion 自然会被忽略。
    if (session.isSubmitInFlight()) {
        statusBar()->showMessage(tr("本地正在提交，无法拉取，请稍后再试"), 3000);
        return;
    }
    requestFastAPISyncNow();
    statusBar()->showMessage(tr("已请求远端最新版本，请等待 entity_graph_saved 到达"), 3000);
    qDebug().noquote() << "[Collab][DEBUG] <<< onCollabPullButtonClicked EXIT (sync_now sent)";
}

void MainWindow::publishFastAPIAutoSnapshot() {
    if (curWindow == nullptr || fastapi_project_id.isEmpty()) {
        return;
    }

    QAction* checkedAct = setModeActGroup ? setModeActGroup->checkedAction() : nullptr;
    if (checkedAct != setFASTAPIModeAct) {
        return;
    }

    QString reason = fastapiLastPublishReason.isEmpty() ? QString::fromUtf8("local-change") : fastapiLastPublishReason;

    // 优先：尝试 ACIS entity graph 路径（序列化完整 ACIS 拓扑 → POST 到 neo4j_entity_store）
    if (submitACISEntityGraph(reason)) {
        pendingEntityChanges.clear();
        return;
    }
    // submitACISEntityGraph 失败时会自动降级到 WebSocket SAT 路径

    // 回退到旧路径：pendingChanges 有就用 submitEntityGraphIncremental，没有就用 SAT 全量
    if (!pendingEntityChanges.isEmpty()) {
        QString entityGraphJson = exportEntityGraphToJson();
        QString changesJson = exportEntityChangesToJson(pendingEntityChanges);
        if (submitEntityGraphIncremental(entityGraphJson, changesJson, reason)) {
            pendingEntityChanges.clear();
            return;
        }
    }

    // 最后兜底：SAT 全量
    publishFastAPIModelSnapshot(false);
}

// ============================================================================
// Neo4j Entity Graph 协作方法
// ============================================================================

bool MainWindow::submitACISEntityGraph(const QString& reason) {
    if (curWindow == nullptr || fastapi_project_id.isEmpty()) {
        return false;
    }

    auto& session = CollabSession::instance();
    const QString author = QString::fromStdString(fastapi_author).trimmed();
    CollabSession::SubmitDecision decision = session.tryBeginSubmit(reason);
    if (decision.kind != CollabSession::SubmitDecision::Allow) {
        statusBar()->showMessage(decision.reason, 5000);
        return false;
    }

    if (fastapiSyncSocket == nullptr || !fastapiSyncSocket->isValid()) {
        session.rollbackSubmit();
        statusBar()->showMessage(tr("协作通道未连接"), 5000);
        return false;
    }

    // 1. 序列化完整 ACIS entity graph
    const ENTITY_LIST& entities = curWindow->getEntityList();
    QJsonObject acisGraph = serializeACISEntityGraph(entityIndexToUuid, entities);
    qDebug().noquote() << "[Collab] submitACISEntityGraph: entityGraph nodes="
                       << acisGraph.value("nodes").toArray().size()
                       << "rels=" << acisGraph.value("rels").toArray().size();

    // 2. 同时导出 SAT（用于广播 content）
    QString fullSat;
    if (!exportCurrentModelToSat(&fullSat, nullptr) || fullSat.isEmpty()) {
        session.rollbackSubmit();
        statusBar()->showMessage(tr("导出本地模型 SAT 失败，无法推送"), 5000);
        return false;
    }

    // 3. 通过 WebSocket 广播 entity_graph_saved
    //    entity_graph 和 SAT 一起存在 storage_bridge 的 content_text 里，
    //    storage_bridge 返回时会完整返回 content_text 中的 JSON（包含 entity_graph），
    //    拉取端直接解析 entity_graph 做反序列化，无需走 Python neo4j 路径。
    const QString requestId = decision.requestId;

    QJsonObject content;
    content.insert("sat", fullSat);
    content.insert("entity_graph", acisGraph); // 完整 entity_graph JSON，存 content_text

    QJsonObject payload;
    payload.insert("type", "submit_entity_graph");
    payload.insert("project_id", fastapi_project_id);
    payload.insert("request_id", requestId);
    payload.insert("author", author.isEmpty() ? QString::fromUtf8("dbcad-exe") : author);
    payload.insert("content", content);
    payload.insert("reason", reason.isEmpty() ? QString::fromUtf8("local-change") : reason);
    if (session.modelVersion() > 0) {
        payload.insert("base_version", session.modelVersion());
    } else {
        payload.insert("base_version", QJsonValue::Null);
    }

    fastapiSyncSocket->sendTextMessage(QString::fromUtf8(QJsonDocument(payload).toJson(QJsonDocument::Compact)));
    statusBar()->showMessage(tr("正在提交 ACIS Entity Graph 变更..."), 1500);
    updateCollabPanelUi();
    return true;
}

bool MainWindow::pullACISEntityGraph(int version, const QJsonObject& entityGraphJson) {
    if (curWindow == nullptr || fastapi_project_id.isEmpty()) {
        return false;
    }

    qDebug().noquote() << "[Collab] pullACISEntityGraph: version=" << version
                       << "entityGraph nodes=" << entityGraphJson.value("nodes").toArray().size()
                       << "rels=" << entityGraphJson.value("rels").toArray().size();

    // 反序列化 entity_graph JSON → ACIS BODY 列表
    DeserializedEntityGraph result;
    QString deserializeError;
    if (!deserializeACISEntityGraph(entityGraphJson, &result, &deserializeError)) {
        qWarning().noquote() << "[Collab] pullACISEntityGraph: deserialize failed:" << deserializeError;
        return false;
    }

    if (result.entities->count() == 0) {
        qWarning().noquote() << "[Collab] pullACISEntityGraph: deserialize returned 0 entities";
        delete result.entities;
        return false;
    }

    qDebug().noquote() << "[Collab] pullACISEntityGraph: deserialize OK, bodies=" << result.entities->count();

    // ====== 临时调试：直接返回 false，强制走 SAT 路径，确认 entity_graph 路径是崩溃源 ======
    qDebug().noquote() << "[Collab] pullACISEntityGraph: DIAGNOSTIC MODE - 返回 false，让 SAT 路径接管";
    delete result.entities;
    return false;
}

/*
 * 以下代码临时禁用（诊断目的），启用请打开下一段宏。
 */
#if 0

    // 验证 BODY 拓扑完整性（先 api_facet_entity 一下，能成功说明拓扑完整）
    for (int t = 0; t < result.entities->count(); ++t) {
        ENTITY* tb = (*result.entities)[t];
        qDebug().noquote() << "[Collab] pullACISEntityGraph: body[" << t << "] ptr=" << (void*)tb
                           << "isBODY=" << (tb && is_BODY(tb) ? 1 : 0)
                           << "lump=" << (tb ? (void*)((BODY*)tb)->lump() : (void*)nullptr);
        if (tb && is_BODY(tb)) {
            BODY* bbody = (BODY*)tb;
            LUMP* ll = bbody->lump();
            qDebug().noquote() << "[Collab] pullACISEntityGraph: body[" << t << "] lump=" << (void*)ll
                               << "shell=" << (ll ? (void*)ll->shell() : (void*)nullptr);
            SHELL* ss = ll ? ll->shell() : nullptr;
            FACE* ff = ss ? ss->face() : nullptr;
            qDebug().noquote() << "[Collab] pullACISEntityGraph: body[" << t << "] shell=" << (void*)ss
                               << "face=" << (void*)ff;
        }
    }

    // 清空本地画布
    qDebug().noquote() << "[Collab] pullACISEntityGraph: about to call curWindow->clear()";
    curWindow->clear();
    qDebug().noquote() << "[Collab] pullACISEntityGraph: after clear, entity_tree.size=" << curWindow->getEntityList().count();

    // 将反序列化出的 BODY 加入 entity_tree
    int addedCount = 0;
    for (int i = 0; i < result.entities->count(); ++i) {
        ENTITY* body = (*result.entities)[i];
        qDebug().noquote() << "[Collab] pullACISEntityGraph: loop iter" << i
                           << "body ptr=" << (void*)body
                           << "count left=" << result.entities->count() - i;
        if (!body) {
            qWarning() << "[Collab] pullACISEntityGraph: body is null at index" << i;
            continue;
        }
        qDebug().noquote() << "[Collab] pullACISEntityGraph: body[" << i << "] is_BODY check";
        if (is_BODY(body)) {
            // 生成新的 ETI index（从当前 entityIndexToUuid.size() 开始）
            int newIndex = entityIndexToUuid.size();
            QString uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
            entityIndexToUuid[newIndex] = uuid;
            // 通过 ACIS api_restore_entity_list 内部机制注册到 ETI（如果需要）
            // 直接 addEntity，由 Window 负责注册到 ETI
            const QString name = QString::fromUtf8("导入实体%1").arg(curWindow->getEntityList().count());
            qDebug().noquote() << "[Collab] pullACISEntityGraph: about to addEntity, body=" << (void*)body
                               << "lump=" << (void*)((BODY*)body)->lump();
            curWindow->addEntity(body, name.toStdString(), -1);
            qDebug().noquote() << "[Collab] pullACISEntityGraph: added BODY ptr=" << (void*)body << "name=" << name;
            addedCount++;
        } else {
            qWarning() << "[Collab] pullACISEntityGraph: body[" << i << "] is not a BODY";
        }
    }

    qDebug().noquote() << "[Collab] pullACISEntityGraph: ADDED" << addedCount << "bodies";
    delete result.entities;
    qDebug().noquote() << "[Collab] pullACISEntityGraph: entities list deleted, returning true";
    return true;
}
#endif // #if 0 诊断模式结束

bool MainWindow::exportCurrentModelToSat(QString* satContent, QString* errorMessage) {
    if (satContent == nullptr) {
        return false;
    }
    satContent->clear();

    if (curWindow == nullptr) {
        if (errorMessage != nullptr) {
            *errorMessage = tr("当前窗口为空。");
        }
        return false;
    }

    // 直接从 Window 的 entity_tree 获取实体列表，不再依赖 acis_get_noattrib_toplevel_active_entities()。
    // 原因：api_restore_entity_list() 读取 SAT 后实体进入了 ACIS，但 acis_get_noattrib_toplevel_active_entities
    // 仍然找不到已 restore 的实体（ACIS 内部状态不一致），导致协作 pull 后的实体在 export 时丢失。
    // entity_tree 是 Qt 层维护的实体列表，始终准确。
    ENTITY_LIST el = curWindow->getEntityList();
    qDebug().noquote() << "[Collab] exportCurrentModelToSat: el.count()=" << el.count()
                       << "(from entity_tree, not ACIS API)";
    for (int i = 0; i < el.count(); i++) {
        ENTITY* e = el[i];
        qDebug().noquote() << "[Collab] exportCurrentModelToSat: entity[" << i << "] ptr=" << e
                           << "isBODY=" << (e ? is_BODY(e) : 0);
    }
    if (el.count() == 0) {
        if (errorMessage != nullptr) {
            *errorMessage = tr("当前无顶级活跃实体。");
        }
        return false;
    }

    QTemporaryFile tempFile(QDir::tempPath() + "/dbcad_save_XXXXXX.sat");
    tempFile.setAutoRemove(true);
    if (!tempFile.open()) {
        if (errorMessage != nullptr) {
            *errorMessage = tr("无法创建临时文件用于导出SAT");
        }
        return false;
    }

    const QString tempPath = tempFile.fileName();
    tempFile.close();

    FILE* f = fopen(tempPath.toStdString().c_str(), "wb");
    if (!f) {
        if (errorMessage != nullptr) {
            *errorMessage = tr("无法打开临时SAT文件用于写入");
        }
        return false;
    }

    API_NOP_BEGIN;
    api_save_version(2, 0);
    FileInfo fileinfo;
    fileinfo.set_units(1.0);
    fileinfo.set_product_id("dbcad_fastapi");
    result = api_set_file_info((FileIdent | FileUnits), fileinfo);
    result = api_set_int_option("sequence_save_files", 1);
    result = api_save_entity_list(f, true, el);
    API_NOP_END;
    fclose(f);

    QFile satFile(tempPath);
    if (!satFile.open(QIODevice::ReadOnly)) {
        if (errorMessage != nullptr) {
            *errorMessage = tr("读取临时SAT文件失败");
        }
        return false;
    }

    *satContent = QString::fromUtf8(satFile.readAll());
    satFile.close();
    if (satContent->isEmpty()) {
        if (errorMessage != nullptr) {
            *errorMessage = tr("导出的SAT内容为空");
        }
        return false;
    }

    return true;
}

bool MainWindow::submitFastAPIModelOverSocket(const QString& satContent, const QString& reason, bool interactiveConflict) {
    auto& session = CollabSession::instance();
    CollabSession::SubmitDecision decision = session.tryBeginSubmit(reason);

    if (decision.kind != CollabSession::SubmitDecision::Allow) {
        if (interactiveConflict) {
            QMessageBox::warning(this, tr("DBCAD"), decision.reason);
        } else {
            statusBar()->showMessage(decision.reason, 5000);
        }
        updateCollabPanelUi();
        return false;
    }

    // SAT fallback 路径与 entity_graph 路径共用同一个 submitInFlight 状态，
    // 但 entity_graph 的 submit_accepted 会先到达并清空它。用 satSubmitInFlight
    // 作为独立标记，确保 SAT 的 model_saved 广播能正确识别本端提交。
    session.setSatSubmitInFlight(true);

    if (fastapiSyncSocket == nullptr || !fastapiSyncSocket->isValid()) {
        session.rollbackSubmit();
        session.setSatSubmitInFlight(false);
        if (interactiveConflict) {
            QMessageBox::warning(this, tr("DBCAD"), tr("FastAPI WebSocket collaboration channel is not connected."));
        } else {
            statusBar()->showMessage(tr("Collaboration channel is not connected; local changes were not submitted."), 5000);
        }
        return false;
    }

    const QString author = QString::fromStdString(fastapi_author).trimmed();
    const QString requestId = decision.requestId;

    QJsonObject content;
    content.insert("sat", satContent);

    QJsonObject payload;
    payload.insert("type", "submit_model");
    payload.insert("project_id", fastapi_project_id);
    payload.insert("request_id", requestId);
    payload.insert("author", author.isEmpty() ? QString::fromUtf8("dbcad-exe") : author);
    payload.insert("content", content);
    payload.insert("reason", reason.isEmpty() ? QString::fromUtf8("local-change") : reason);
    if (session.modelVersion() > 0) {
        payload.insert("base_version", session.modelVersion());
    } else {
        payload.insert("base_version", QJsonValue::Null);
    }

    fastapiSyncSocket->sendTextMessage(QString::fromUtf8(QJsonDocument(payload).toJson(QJsonDocument::Compact)));
    statusBar()->showMessage(tr("Submitting collaborative snapshot..."), 1500);
    updateCollabPanelUi();
    return true;
}

bool MainWindow::publishFastAPIModelSnapshot(bool interactiveConflict) {
    auto& session = CollabSession::instance();

    if (curWindow == nullptr || fastapi_project_id.isEmpty()) {
        return false;
    }

    QString satContent;
    QString errorMessage;
    if (!exportCurrentModelToSat(&satContent, &errorMessage)) {
        if (interactiveConflict && !errorMessage.isEmpty()) {
            QMessageBox::warning(this, tr("DBCAD"), errorMessage);
        }
        return false;
    }

    BackendApiClient client(
        QString::fromStdString(fastapi_base_url),
        QString::fromStdString(fastapi_author),
        QString::fromStdString(fastapi_password));
    if (!client.isConfigured()) {
        if (interactiveConflict) {
            QMessageBox::warning(this, tr("DBCAD"), tr("FastAPI地址未配置，请先设置fastapi_connect_info.conf"));
        }
        return false;
    }

    if (!interactiveConflict) {
        return submitFastAPIModelOverSocket(satContent, fastapiLastPublishReason, false);
    }

    CollabSession::PublishDecision pubDecision = session.tryBeginHttpPublish();
    if (pubDecision.kind != CollabSession::PublishDecision::Allow) {
        QMessageBox::warning(this, tr("DBCAD"), pubDecision.reason);
        return false;
    }

    std::optional<int> baseVersion;
    if (session.modelVersion() > 0) {
        baseVersion = session.modelVersion();
    }

    auto newVersion = client.saveModel(fastapi_project_id, satContent, baseVersion);

    if (!newVersion.has_value()) {
        session.rollbackHttpPublish();
        if (client.lastStatusCode() == 409) {
            auto latest = client.getLatestModel(fastapi_project_id);
            if (latest.has_value()) {
                session.onSubmitRejected(latest->version);
                QMessageBox::warning(this, tr("DBCAD"),
                    tr("HTTP 发布冲突：远端版本 %1 已更新，请处理冲突后重试。").arg(latest->version));
            }
        } else {
            QMessageBox::warning(this, tr("DBCAD"), client.lastError());
        }
        updateCollabPanelUi();
        return false;
    }

    session.onHttpPublishEnd();
    fastapi_model_version = session.modelVersion();
    fastapi_pending_remote_version = session.pendingRemoteVersion();
    curWindow->setIsModified(false);
    if (!fastapi_project_name.isEmpty()) {
        setCurrentPartName(fastapi_project_name);
    }
    updateCollabPanelUi();
    statusBar()->showMessage(tr("已自动发布协作版本%1").arg(fastapi_model_version), 2000);
    return true;
}

void MainWindow::updateFastAPICollaboratorsStatus() {
    if (!fastapi_project_id.isEmpty()) {
        statusBar()->showMessage(tr("当前在线协作者：%1").arg(fastapi_collaborators.size()), 2500);
    }
    updateCollabPanelUi();
}

void MainWindow::setCollabConnectionState(const QString& stateText) {
    if (collabConnectionLabel != nullptr) {
        collabConnectionLabel->setText(stateText);
    }
}

void MainWindow::updateCollabPanelUi() {
    if (collabProjectLabel != nullptr) {
        collabProjectLabel->setText(
            fastapi_project_name.isEmpty() ? tr("项目：未打开") : tr("项目：%1").arg(fastapi_project_name));
    }
    if (collabVersionLabel != nullptr) {
        collabVersionLabel->setText(tr("本地版本：%1").arg(fastapi_model_version));
    }
    if (collabPendingLabel != nullptr) {
        if (fastapiSubmitInFlight) {
            collabPendingLabel->setText(tr("Submit: waiting for server"));
        } else if (fastapi_pending_remote_version > fastapi_model_version) {
            collabPendingLabel->setText(tr("待同步版本：%1").arg(fastapi_pending_remote_version));
        } else {
            collabPendingLabel->setText(tr("待同步版本：无"));
        }
    }
    if (collabAutoFollowCheckBox != nullptr) {
        collabAutoFollowCheckBox->setChecked(fastapiAutoFollowRemote);
    }
    if (collabMembersList != nullptr) {
        collabMembersList->clear();
        for (auto it = fastapi_collaborators.constBegin(); it != fastapi_collaborators.constEnd(); ++it) {
            const QString author = it.value().trimmed().isEmpty() ? tr("unknown") : it.value();
            collabMembersList->addItem(author + " (" + it.key() + ")");
        }
    }
}

void MainWindow::applyPendingRemoteVersion() {
    // 已废弃：旧的 "待同步版本" 走的是 clear+restore 全量同步路径，与新的增量同步语义冲突。
    // 保留此函数仅为兼容既有菜单项 / 信号连接，实际调用将引导用户改用 Pull 按钮。
    statusBar()->showMessage(tr("请改用协作面板的「拉取(Pull)」按钮获取远端增量变更"), 4000);
}

bool MainWindow::syncFastAPIRemoteVersion(int remoteVersion, const QString& reason) {
    // 已废弃：clear+restore 全量同步会清空本地所有未 push 的修改，与增量语义冲突。
    // 同步被替换为 Pull 按钮驱动的 entity_graph 增量合并。
    Q_UNUSED(remoteVersion);
    Q_UNUSED(reason);
    statusBar()->showMessage(tr("全量同步已禁用，请改用「拉取(Pull)」按钮"), 4000);
    return false;
}

bool MainWindow::applyFastAPIRemoteSat(int remoteVersion, const QString& satContent, const QString& reason) {
    // 已废弃：见 syncFastAPIRemoteVersion 注释。
    Q_UNUSED(remoteVersion);
    Q_UNUSED(satContent);
    Q_UNUSED(reason);
    statusBar()->showMessage(tr("全量同步已禁用，请改用「拉取(Pull)」按钮"), 4000);
    return false;
}

void MainWindow::reconnectFastAPISync() {
    // 注意：不要在这里调 onReconnect —— 紧接着的 disconnectFastAPISync() 会通过
    // onDisconnected() 把 state 又打回 Disconnected。state 转 Connected_Idle 的真正时机
    // 是下面 connected lambda（socket 真连上时）。
    disconnectFastAPISync();

    if (fastapi_project_id.isEmpty() || fastapi_base_url.empty()) {
        setCollabConnectionState(tr("未连接"));
        updateCollabPanelUi();
        return;
    }

    setCollabConnectionState(tr("连接中"));
    updateCollabPanelUi();

    QString wsBaseUrl = QString::fromStdString(fastapi_base_url).trimmed();
    while (wsBaseUrl.endsWith('/')) {
        wsBaseUrl.chop(1);
    }
    if (wsBaseUrl.startsWith("https://")) {
        wsBaseUrl.replace(0, 5, "wss");
    } else if (wsBaseUrl.startsWith("http://")) {
        wsBaseUrl.replace(0, 4, "ws");
    } else if (!wsBaseUrl.startsWith("ws://") && !wsBaseUrl.startsWith("wss://")) {
        wsBaseUrl = "ws://" + wsBaseUrl;
    }

    QString wsPath = "/ws/projects/" + fastapi_project_id;
    QStringList queryItems;
    queryItems << "client_id=" + QString::fromUtf8(QUrl::toPercentEncoding(fastapi_client_id));
    queryItems << "author=" + QString::fromUtf8(QUrl::toPercentEncoding(QString::fromStdString(fastapi_author)));
    if (!fastapi_password.empty()) {
        queryItems << "password=" + QString::fromUtf8(QUrl::toPercentEncoding(QString::fromStdString(fastapi_password)));
    }
    wsPath += "?" + queryItems.join("&");
    const QUrl wsUrl(wsBaseUrl + wsPath);
    fastapiSyncSocket = new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this);

    connect(fastapiSyncSocket, &QWebSocket::textMessageReceived, this, &MainWindow::handleFastAPISyncMessage);
    connect(fastapiSyncSocket, &QWebSocket::connected, this, [this]() {
        qDebug().noquote() << "[Collab][DEBUG] >>> WS connected lambda FIRED";
        statusBar()->showMessage(tr("FastAPI实时同步已连接"), 2000);
        if (fastapiReconnectTimer != nullptr) {
            fastapiReconnectTimer->stop();
        }
        // onReconnect(true) 把 CollabSession state 转 Connected_Idle，确保 Pull 触发 sync_now
        // 应答后 tryBeginApplyRemote 不会因 state_ == Disconnected 被拒。
        // disconnectFastAPISync() 在 reconnectFastAPISync 入口处把它打回了 Disconnected，
        // 这里必须重新转 Connected_Idle。
        CollabSession::instance().onReconnect(!fastapi_project_id.isEmpty());
        qDebug().noquote() << "[Collab][DEBUG] WS connected: onReconnect done, state=" << CollabSession::instance().dump(CollabSession::Event::WsMessage);
        setCollabConnectionState(tr("已连接"));
        // Git 语义：重连后不主动 sync_now，避免一连接就把远端的 entity_graph_saved 自动 apply 下来。
        // 远端历史版本应等待用户主动点 Pull 才合并到本地画布。
        // （详见 COLLABORATION_TECHNICAL_ROADMAP 的 Git-like 同步策略。）
        if (fastapiHeartbeatTimer != nullptr) {
            fastapiHeartbeatTimer->start();
        }
        updateCollabPanelUi();
    });
    connect(fastapiSyncSocket, &QWebSocket::disconnected, this, [this]() {
        if (fastapiHeartbeatTimer != nullptr) {
            fastapiHeartbeatTimer->stop();
        }
        if (!fastapi_project_id.isEmpty()) {
            statusBar()->showMessage(tr("FastAPI实时同步已断开"), 2000);
            setCollabConnectionState(tr("已断开，重连中"));
            if (fastapiReconnectTimer != nullptr && !fastapiReconnectTimer->isActive()) {
                fastapiReconnectTimer->start();
            }
        } else {
            setCollabConnectionState(tr("未连接"));
        }
        updateCollabPanelUi();
    });

    fastapiSyncSocket->open(wsUrl);
}

void MainWindow::handleFastAPISyncMessage(const QString& message) {
    qDebug().noquote() << CollabSession::instance().dump(CollabSession::Event::WsMessage);
    qDebug().noquote() << "[Collab][DEBUG] >>> handleFastAPISyncMessage message.size=" << message.size();
    // 最外层 try/catch 守卫：WS 回调里任何 C++ 异常如果穿透到 Qt event loop，
    // 都会触发 std::terminate → abort()（参见 log_B.txt 中"打开文件失败"导致 B.exe crash 的根因）。
    try {
        handleFastAPISyncMessageImpl(message);
    } catch (const std::exception& e) {
        qWarning().noquote() << "[Collab] handleFastAPISyncMessage threw:" << e.what();
        statusBar()->showMessage(tr("协作消息处理失败：%1").arg(QString::fromUtf8(e.what())), 5000);
    } catch (...) {
        qWarning().noquote() << "[Collab] handleFastAPISyncMessage threw unknown exception";
        statusBar()->showMessage(tr("协作消息处理失败（未知异常）"), 5000);
    }
}

void MainWindow::handleFastAPISyncMessageImpl(const QString& message) {
    if (fastapi_project_id.isEmpty()) {
        qDebug().noquote() << "[Collab][DEBUG] <<< handleFastAPISyncMessage EXIT (empty fastapi_project_id)";
        return;
    }

    QAction* checkedAct = setModeActGroup ? setModeActGroup->checkedAction() : nullptr;
    if (checkedAct != setFASTAPIModeAct) {
        qDebug().noquote() << "[Collab][DEBUG] <<< handleFastAPISyncMessage EXIT (not FASTAPI mode)";
        return;
    }

    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(message.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
        return;
    }

    const QJsonObject root = document.object();
    const QString messageType = root.value("type").toString();
    qDebug().noquote() << "[Collab][DEBUG] handleFastAPISyncMessage type=" << messageType
                       << "hasSat=" << root.value("content").toObject().contains("sat")
                       << "hasTrigger=" << root.value("trigger").toString();
    if (messageType == "presence_snapshot") {
        fastapi_collaborators.clear();
        const QJsonArray members = root.value("members").toArray();
        for (const auto& memberValue : members) {
            if (!memberValue.isObject()) {
                continue;
            }
            const QJsonObject member = memberValue.toObject();
            const QString memberClientId = member.value("client_id").toString();
            if (memberClientId.isEmpty()) {
                continue;
            }
            fastapi_collaborators.insert(memberClientId, member.value("author").toString());
        }
        updateFastAPICollaboratorsStatus();
        return;
    }

    if (messageType == "collaborator_joined") {
        const QString memberClientId = root.value("client_id").toString();
        if (!memberClientId.isEmpty()) {
            fastapi_collaborators.insert(memberClientId, root.value("author").toString());
            updateFastAPICollaboratorsStatus();
        }
        return;
    }

    if (messageType == "collaborator_left") {
        const QString memberClientId = root.value("client_id").toString();
        if (!memberClientId.isEmpty()) {
            fastapi_collaborators.remove(memberClientId);
            updateFastAPICollaboratorsStatus();
        }
        return;
    }

    if (messageType == "pong") {
        return;
    }

    if (messageType == "submit_accepted") {
        // Server 在 broadcast entity_graph_saved 时会 exclude_client_id=client_id，
        // 然后单独给提交者发 submit_accepted { request_id, version, version_model }。
        // client 必须处理这条消息，否则 SubmitInFlight 状态永远清不掉。
        auto& session = CollabSession::instance();
        const QString requestId = root.value("request_id").toString();
        const int acceptedVersion = root.value("version").toInt(0);

        if (requestId.isEmpty() || requestId == session.submitRequestId()) {
            // SAT fallback 路径（version_model="model"）：服务端 version 字段已经包含正确版本，
            // 直接用 acceptedVersion 更新版本，model_saved 广播走 isOwnSubmitBroadcast 路径。
            // entity_graph 路径（version_model="entity_graph"）：同理的，版本在 acceptedVersion 里。
            session.onSubmitAccepted(acceptedVersion);
            fastapi_model_version = session.modelVersion();
            fastapi_pending_remote_version = session.pendingRemoteVersion();
            pendingEntityChanges.clear();
            if (curWindow != nullptr) {
                curWindow->setIsModified(false);
            }
            if (!fastapi_project_name.isEmpty()) {
                setCurrentPartName(fastapi_project_name);
            }
            updateCollabPanelUi();
            statusBar()->showMessage(tr("协作快照已提交，版本 %1").arg(fastapi_model_version), 2500);
        }
        return;
    }

    if (messageType == "submit_rejected") {
        auto& session = CollabSession::instance();
        const QString requestId = root.value("request_id").toString();
        const int latestVersion = root.value("latest_version").toInt(0);

        if (!requestId.isEmpty() && requestId == session.submitRequestId()) {
            session.rollbackSubmit();
        }

        if (latestVersion > session.modelVersion()) {
            session.onSubmitRejected(latestVersion);
        } else {
            session.onSubmitRejected(session.modelVersion());
        }

        // 冲突自动处理：备份本地 SAT → Pull 最新版本 → 用户选择是否 merge 回来。
        // 这样 A 的修改不会在 Pull 时被 clear() 永久丢失。
        QString localSatBackup;
        if (curWindow != nullptr) {
            // 无论本地是否 dirty，都把当前 ACIS 模型内容备份出来。
            // isLocalDirtyDuringSubmit=true 说明提交之后用户又做了改动（不在 pendingEntityChanges 里），
            // 也应该一并备份，否则 Pull 后那些改动会丢失。
            ENTITY_LIST el;
            acis_get_noattrib_toplevel_active_entities(el);
            if (el.count() > 0) {
                exportCurrentModelToSat(&localSatBackup, nullptr);
            }
        }
        fastapiConflictLocalSatBackup = localSatBackup;
        const QString conflictDetail = root.value("detail").toString();
        const QString conflictSat = root.value("content").toObject().value("sat").toString();

        if (!conflictSat.isEmpty()) {
            statusBar()->showMessage(
                tr("协作提交被拒绝：%1（冲突版本 %2），正在拉取最新版本...").arg(conflictDetail).arg(latestVersion),
                6000);
        } else {
            statusBar()->showMessage(
                tr("协作提交被拒绝：%1（冲突版本 %2），正在拉取最新版本...").arg(conflictDetail).arg(latestVersion),
                6000);
        }
        updateCollabPanelUi();

        // 自动 Pull 最新版本。注意：Pull 应答到达后 applyRemoteSatSnapshot 会检查
        // fastapiConflictLocalSatBackup，如果有备份会在 clear() 后弹出合并对话框。
        if (fastapiSyncSocket != nullptr && fastapiSyncSocket->isValid()) {
            fastapiSyncSocket->sendTextMessage("sync_now");
        }
        return;
    }

    if (messageType == "error") {
        statusBar()->showMessage(tr("Collaboration channel error: %1").arg(root.value("detail").toString()), 5000);
        return;
    }

    // 支持增量实体图消息
    if (messageType == "entity_graph_saved") {
        qDebug().noquote() << "[Collab][DEBUG] handleFastAPISyncMessage: entity_graph_saved RECEIVED, projectId=" << root.value("project_id").toString()
                            << "requestId=" << root.value("request_id").toString();
        const QString projectId = root.value("project_id").toString();
        if (projectId != fastapi_project_id) {
            return;
        }

        const QJsonObject content = root.value("content").toObject();
        const int remoteVersion = root.value("version").toInt(0);
        if (remoteVersion <= 0) {
            return;
        }

        auto& session = CollabSession::instance();

        // Pull 应答 / 远端广播 entity_graph_saved：直接用 content.sat 整个替换本地画布。
        // 后端 entity_graph_saved 消息的 content 同时携带了 entity_graph/changes（git-like 增量元数据）
        // 以及 sat（整个 ACIS 顶级 body 的 SAT 文本，由 submitEntityGraphIncremental 在 push 时随 payload
        // 一起发出；详见 mainwindow.cpp:1223 `fullSat = exportCurrentModelToSat(...)`）。
        // 跟 master 分支的 applyFastAPIRemoteSat 完全一致：clear() → restore → addEntity，比对
        // entity_graph 增量合并简单无数倍，而且不出错（增量合并需要对每个 ADD 做 acis_restore_entity_list
        // → addEntity，里面很容易在临时 SAT 文件 / ACIS API 异常时炸）。
        const QString satContent = content.value("sat").toString();
        const QString reason = root.value("trigger").toString("broadcast");
        const bool isPullResponse = (reason == QStringLiteral("sync_now"));
        qDebug().noquote() << "[Collab] entity_graph_saved v=" << remoteVersion
                           << "trigger=" << reason
                           << "isPull=" << isPullResponse
                           << "state=" << session.dump(CollabSession::Event::WsMessage)
                           << "content.hasSat=" << (!satContent.isEmpty())
                           << "sat.size=" << satContent.size();

        // 仅在自己提交的请求应答时走 submit-accept 路径（保留原语义）。
        const QString requestId = root.value("request_id").toString();
        const bool acceptedOwnSubmit = !requestId.isEmpty() && requestId == session.submitRequestId();
        if (acceptedOwnSubmit) {
            session.onSubmitAccepted(remoteVersion);
            fastapi_model_version = session.modelVersion();
            fastapi_pending_remote_version = session.pendingRemoteVersion();
            if (!session.isLocalDirtyDuringSubmit() && curWindow != nullptr) {
                curWindow->setIsModified(false);
            }
            pendingEntityChanges.clear();
            if (!fastapi_project_name.isEmpty()) {
                setCurrentPartName(fastapi_project_name);
            }
            updateCollabPanelUi();
            statusBar()->showMessage(tr("增量实体图已提交，版本 %1").arg(fastapi_model_version), 2500);
            if (session.isLocalDirtyDuringSubmit()) {
                scheduleFastAPIAutoPublish(tr("local-change-after-ack"));
            }
            return;
        }

        if (remoteVersion <= session.modelVersion()) {
            return;
        }

        if (curWindow == nullptr) {
            return;
        }

        if (session.isSubmitInFlight()) {
            // 本地正在提交，不要在这条响应里 apply；放到 pending，等下一轮 Pull 再合并。
            session.onRemotePending(remoteVersion);
            fastapi_pending_remote_version = session.pendingRemoteVersion();
            statusBar()->showMessage(
                tr("检测到远程增量版本%1，当前正在提交中，已加入待同步队列").arg(session.pendingRemoteVersion()),
                5000);
            updateCollabPanelUi();
            return;
        }

        // Git-like 语义：
        //   - 只有用户主动 Pull（trigger == "sync_now"）的应答才立即合并本地画布
        //   - 远端 broadcast（其他客户端提交触发）默认只更新 pendingRemoteVersion，
        //     提示用户"远端有新版本，请在协作面板点 Pull 合并"，保留本地编辑不被破坏。
        // 这样：A push 后，B 不会立刻同步，必须点 Pull 才合并，符合 git 协作习惯。
        const bool shouldApply = isPullResponse;
        qDebug().noquote() << "[Collab] shouldApply decision:"
                           << "isPullResponse=" << isPullResponse
                           << "trigger=" << reason
                           << "final=" << shouldApply;
        if (!shouldApply) {
            qDebug().noquote() << "[Collab][DEBUG] handleFastAPISyncMessage: shouldApply=false, parking as pending, msg_type=entity_graph_saved";
            session.onRemotePending(remoteVersion);
            fastapi_pending_remote_version = session.pendingRemoteVersion();
            const QString tip = tr("远端版本%1已推送（git-like 语义），请在协作面板点「拉取(Pull)」合并").arg(remoteVersion);
            statusBar()->showMessage(tip, 5000);
            updateCollabPanelUi();
            return;
        }

        if (satContent.isEmpty()) {
            // sync_now 应答居然没 SAT，理论不应发生，回退到 pending 让用户重试
            session.onRemotePending(remoteVersion);
            fastapi_pending_remote_version = session.pendingRemoteVersion();
            statusBar()->showMessage(tr("远端版本%1的 Pull 应答缺少SAT文本，请刷新后重试").arg(remoteVersion), 5000);
            updateCollabPanelUi();
            return;
        }

        CollabSession::ApplyDecision applyDecision = session.tryBeginApplyRemote(remoteVersion, reason);
        qDebug().noquote() << "[Collab] Pull tryBeginApplyRemote kind=" << (int)applyDecision.kind
                           << "reason=" << applyDecision.reasonTr;
        if (applyDecision.kind != CollabSession::ApplyDecision::Allow) {
            qWarning() << "[Collab] Pull apply rejected:" << applyDecision.reasonTr
                       << "— version parked as pending, retry Pull later.";
            session.onRemotePending(remoteVersion);
            fastapi_pending_remote_version = session.pendingRemoteVersion();
            updateCollabPanelUi();
            return;
        }

        // 从 content.entity_graph 直接获取 entity_graph JSON，
        // 不再依赖 Python neo4j API（_entity_graph_version）。
        // 新路径：submitACISEntityGraph 把 entity_graph 存在 storage_bridge 的 content_text 里，
        // storage_bridge 返回完整 content 时会携带 entity_graph。
        const QJsonObject entityGraphJson = content.value("entity_graph").toObject();
        if (!entityGraphJson.isEmpty() && !entityGraphJson.value("nodes").toArray().isEmpty()) {
            qDebug().noquote() << "[Collab] Pull try entity_graph path, nodes="
                               << entityGraphJson.value("nodes").toArray().size()
                               << "rels=" << entityGraphJson.value("rels").toArray().size();
            if (pullACISEntityGraph(remoteVersion, entityGraphJson)) {
                session.onRemoteApplied(remoteVersion);
                fastapi_model_version = session.modelVersion();
                fastapi_pending_remote_version = session.pendingRemoteVersion();
                pendingEntityChanges.clear();
                if (!fastapi_project_name.isEmpty()) {
                    setCurrentPartName(fastapi_project_name);
                }
                updateCollabPanelUi();
                statusBar()->showMessage(tr("已通过 ACIS Entity Graph 拉取远端版本 %1").arg(fastapi_model_version), 3000);
                return;
            }
            qDebug().noquote() << "[Collab] Pull entity_graph path returned false, falling back to SAT";
            session.rollbackApply();
        }

        if (satContent.isEmpty()) {
            session.rollbackApply();
            session.onRemotePending(remoteVersion);
            fastapi_pending_remote_version = session.pendingRemoteVersion();
            updateCollabPanelUi();
            statusBar()->showMessage(tr("远端版本%1无SAT文本且Entity Graph重建失败，请刷新后重试").arg(remoteVersion), 5000);
            return;
        }

        qDebug().noquote() << "[Collab] Pull applyRemoteSat start, sat.size=" << satContent.size();
        // SAT fallback：调用 master 同款的 clear() + restoreFastAPIModelFromSat() 全量替换路径。
        const bool ok = applyRemoteSatSnapshot(satContent, reason);
        qDebug().noquote() << "[Collab] Pull applyRemoteSat done, ok=" << ok;
        if (ok) {
            session.onRemoteApplied(remoteVersion);
            fastapi_model_version = session.modelVersion();
            fastapi_pending_remote_version = session.pendingRemoteVersion();
            // 远端 apply 后清空本地未 push 的变更（它们已经在远端版本里）。
            pendingEntityChanges.clear();
            if (!fastapi_project_name.isEmpty()) {
                setCurrentPartName(fastapi_project_name);
            }
            updateCollabPanelUi();
            statusBar()->showMessage(tr("已拉取远端版本 %1").arg(fastapi_model_version), 3000);
        } else {
            // apply 失败时回滚 applying 标志，让用户再点 Pull 还能重试。
            session.rollbackApply();
            session.onRemotePending(remoteVersion);
            fastapi_model_version = session.modelVersion();
            fastapi_pending_remote_version = session.pendingRemoteVersion();
            updateCollabPanelUi();
            qWarning() << "[Collab] Pull applyRemoteSatSnapshot failed, parked as pending for retry. v=" << remoteVersion;
        }
        return;
    }

    if (messageType != "model_saved") {
        return;
    }

    qDebug().noquote() << "[Collab][DEBUG] handleFastAPISyncMessage: entering model_saved branch";

    const QString projectId = root.value("project_id").toString();
    if (projectId != fastapi_project_id) {
        return;
    }

    const int remoteVersion = root.value("version").toInt(0);
    if (remoteVersion <= 0) {
        return;
    }

    auto& session = CollabSession::instance();
    const QString requestId = root.value("request_id").toString();
    const bool acceptedOwnSubmit = !requestId.isEmpty() && requestId == session.submitRequestId();
    // SAT fallback 路径：entity_graph ack 先到清空 submitRequestId，
    // 用 satSubmitInFlight 标记 SAT 提交在飞行中，用 fastapiLastAcceptedRequestId
    // 匹配 submit_accepted 留下的 requestId。
    // 注意：SAT 的 model_saved 广播不含 requestId，此时 submitRequestId 仍未被清空，
    // 所以不能用 requestId 匹配，必须用 satSubmitInFlight 来判断。
    const bool isOwnSubmitBroadcast = acceptedOwnSubmit
        || (session.isSatSubmitInFlight() && requestId.isEmpty())
        || (!requestId.isEmpty() && requestId == fastapiLastAcceptedRequestId);

    if (isOwnSubmitBroadcast) {
        // 本端提交的广播（被 exclude_client_id 排除后发给自己的）。
        // 状态转换（submitOk）和版本号在 submit_accepted 时已经处理完毕。
        // 这里只需要同步 UI 状态，不要再次调用 onSubmitAccepted——remoteVersion
        // 是旧版本号（0），会错误地覆盖已接受的版本。
        if (!session.isLocalDirtyDuringSubmit() && curWindow != nullptr) {
            curWindow->setIsModified(false);
        }
        if (!fastapi_project_name.isEmpty()) {
            setCurrentPartName(fastapi_project_name);
        }
        updateCollabPanelUi();
        statusBar()->showMessage(tr("Collaborative snapshot accepted as version %1").arg(fastapi_model_version), 2500);
        if (session.isLocalDirtyDuringSubmit()) {
            scheduleFastAPIAutoPublish(tr("local-change-after-ack"));
        }
        return;
    }

    if (remoteVersion <= session.modelVersion()) {
        return;
    }

    // model_saved 消息可能是：
    //   1. 来自 submitFastAPIModelOverSocket（SAT fallback，neo4j 存储失败时降级路径）
    //   2. 来自 submit_entity_graph 的存储路径（content 包含 entity_graph）
    // 只要带了 SAT 内容，就应该立即 apply，跟 entity_graph_saved 的 SAT fallback 完全一致。
    const QJsonObject contentJson = root.value("content").toObject();
    const QString satContent = contentJson.value("sat").toString();
    const QString reason = root.value("trigger").toString("model_saved");
    qDebug().noquote() << "[Collab][DEBUG] model_saved branch: hasSat=" << !satContent.isEmpty()
                       << "sat.size=" << satContent.size() << "reason=" << reason
                       << "requestId=" << requestId
                       << "lastAcceptedId=" << fastapiLastAcceptedRequestId
                       << "satSubmitInFlight=" << session.isSatSubmitInFlight();

    // 优先尝试 entity_graph 反序列化路径（如果 content 包含 entity_graph）
    const QJsonObject entityGraphJson = contentJson.value("entity_graph").toObject();
    if (!entityGraphJson.isEmpty() && !entityGraphJson.value("nodes").toArray().isEmpty()) {
        qDebug().noquote() << "[Collab] model_saved try entity_graph path, nodes="
                           << entityGraphJson.value("nodes").toArray().size();
        if (pullACISEntityGraph(remoteVersion, entityGraphJson)) {
            session.onRemoteApplied(remoteVersion);
            fastapi_model_version = session.modelVersion();
            fastapi_pending_remote_version = session.pendingRemoteVersion();
            pendingEntityChanges.clear();
            if (!fastapi_project_name.isEmpty()) {
                setCurrentPartName(fastapi_project_name);
            }
            updateCollabPanelUi();
            statusBar()->showMessage(tr("已通过 ACIS Entity Graph 拉取远端版本 %1").arg(fastapi_model_version), 3000);
            return;
        }
        qDebug().noquote() << "[Collab] model_saved entity_graph path returned false, falling back to SAT";
    }

    if (curWindow == nullptr) {
        return;
    }

    if (satContent.isEmpty()) {
        session.onRemotePending(remoteVersion);
        fastapi_pending_remote_version = session.pendingRemoteVersion();
        statusBar()->showMessage(
            tr("远端版本%1不包含SAT文本，无法自动拉取，请刷新后重试").arg(remoteVersion),
            5000);
        updateCollabPanelUi();
        return;
    }

    CollabSession::ApplyDecision applyDecision = session.tryBeginApplyRemote(remoteVersion, reason);
    qDebug().noquote() << "[Collab] model_saved tryBeginApplyRemote kind=" << (int)applyDecision.kind
                       << "reason=" << applyDecision.reasonTr;
    if (applyDecision.kind != CollabSession::ApplyDecision::Allow) {
        qWarning() << "[Collab] model_saved apply rejected:" << applyDecision.reasonTr;
        session.onRemotePending(remoteVersion);
        fastapi_pending_remote_version = session.pendingRemoteVersion();
        updateCollabPanelUi();
        return;
    }

    qDebug().noquote() << "[Collab] model_saved: applying remote SAT snapshot, sat.size=" << satContent.size();
    const bool ok = applyRemoteSatSnapshot(satContent, reason);
    qDebug().noquote() << "[Collab] model_saved applyRemoteSat done, ok=" << ok;
    if (ok) {
        session.onRemoteApplied(remoteVersion);
        fastapi_model_version = session.modelVersion();
        fastapi_pending_remote_version = session.pendingRemoteVersion();
        pendingEntityChanges.clear();
        if (!fastapi_project_name.isEmpty()) {
            setCurrentPartName(fastapi_project_name);
        }
        updateCollabPanelUi();
        statusBar()->showMessage(tr("已拉取远端版本 %1（SAT 格式）").arg(fastapi_model_version), 3000);
    } else {
        session.rollbackApply();
        session.onRemotePending(remoteVersion);
        fastapi_model_version = session.modelVersion();
        fastapi_pending_remote_version = session.pendingRemoteVersion();
        updateCollabPanelUi();
        qWarning() << "[Collab] model_saved applyRemoteSatSnapshot failed, parked as pending for retry. v=" << remoteVersion;
    }
}

void MainWindow::addWindow() {
    if (!centralWidget()) {
        curWindow = new Window(this);
        setCentralWidget(curWindow);
    } else
        QMessageBox::information(this, tr("无法创建新窗口"), tr("您最多只能创建一个窗口。"));
}

void MainWindow::open() {
    if (maybeSave()) {
        QAction* checkedAct = setModeActGroup->checkedAction();
        if (checkedAct == setACISModeAct) {
            QString fileName = QFileDialog::getOpenFileName(this, QObject::tr("打开文件"), QString::fromStdString(std::string(".")));
            if (!fileName.isEmpty()) loadFile(fileName);
        } else if (checkedAct == setNEO4JModeAct) {
            bool ok;
            QDateTime curDateTime = QDateTime::currentDateTime();
            QString curDateTime_qstr = QString("part") + curDateTime.toString("yyyyMMddhhmmss");  // 保存零件对话框的默认零件名是part+当前年月日时分秒
            QString partnametext = QInputDialog::getText(this, tr("打开零件(neo4j)"), tr("请输入打开的零件名:"), QLineEdit::Normal, curDateTime_qstr, &ok, this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
            if (ok && !partnametext.isEmpty()) {
                loadFile(partnametext);
            }
        } else if (checkedAct == setFASTAPIModeAct) {
            QDateTime curDateTime = QDateTime::currentDateTime();
            QString curDateTime_qstr = QString("part") + curDateTime.toString("yyyyMMddhhmmss");

            QDialog dialog(this);
            dialog.setMinimumSize(220, 220);
            dialog.setWindowTitle(tr("打开零件(FastAPI)"));
            QFormLayout form(&dialog);

            QLabel lineEdit_partname_label(tr("请输入打开的项目名:"), &dialog);
            form.addRow(&lineEdit_partname_label);
            QLineEdit lineEdit_partname(&dialog);
            lineEdit_partname.setText(curDateTime_qstr);
            form.addRow(&lineEdit_partname);

            QLabel spinBox_version_label(tr("请输入版本号（0表示最新）:"), &dialog);
            form.addRow(&spinBox_version_label);
            QSpinBox spinBox_version(&dialog);
            spinBox_version.setMinimum(0);
            spinBox_version.setMaximum(2000000000);
            form.addRow(&spinBox_version);

            QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
            form.addRow(&buttonBox);
            QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
            QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

            if (dialog.exec() == QDialog::Accepted && !lineEdit_partname.text().isEmpty()) {
                if (spinBox_version.value() <= 0) {
                    loadFile(lineEdit_partname.text());
                } else {
                    loadFile(lineEdit_partname.text(), spinBox_version.value());
                }
            }
        } else {
            assert(checkedAct == setNEO4JIncrementalModeAct);
            QDateTime curDateTime = QDateTime::currentDateTime();
            QString curDateTime_qstr = QString("part") + curDateTime.toString("yyyyMMddhhmmss");  // 保存零件对话框的默认零件名是part+当前年月日时分秒

            QDialog dialog(this);
            dialog.setMinimumSize(200, 200);
            dialog.setWindowTitle(tr("打开零件(neo4j)"));
            QFormLayout form(&dialog);

            QLabel lineEdit_partname_label(tr("请输入打开的零件名:"), &dialog);
            form.addRow(&lineEdit_partname_label);
            QLineEdit lineEdit_partname(&dialog);
            lineEdit_partname.setText(curDateTime_qstr);
            form.addRow(&lineEdit_partname);

            QLabel spinBox_generation_label(tr("请输入打开的零件版次（正整数）:"), &dialog);
            form.addRow(&spinBox_generation_label);
            QSpinBox spinBox_generation(&dialog);
            spinBox_generation.setMinimum(1);
            spinBox_generation.setMaximum(2000000000);
            form.addRow(&spinBox_generation);

            QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
            form.addRow(&buttonBox);
            QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
            QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

            if (dialog.exec() == QDialog::Accepted) {
                loadFile(lineEdit_partname.text(), spinBox_generation.value());
            }

        }
    }
}

void MainWindow::buildTreeFromHistroy(HISTORY_STREAM* hs) {
    if (hs == nullptr) {
        hs = get_default_stream();
    }
    
    set_logging(false);
    curWindow->clear();
    DELTA_STATE *this_ds = hs->get_root_ds();
    while (this_ds) {
        GME_DELTA_STATE_user_data* delta_state_user_data = (GME_DELTA_STATE_user_data*)(this_ds->get_user_data());
        if (delta_state_user_data) {
            auto tree_items = delta_state_user_data->get_tree_items();
            for (auto item : *tree_items) {
                item.ptrDisplayData = nullptr;
                curWindow->addTreeItem(item);
                curWindow->entity_tree[item.index].index_support.clear();
                for (auto base : item.index_base) {
                    curWindow->entity_tree[base].visible = false;
                    curWindow->entity_tree[base].index_support.push_back(item.index);
                }
            }
        }
        this_ds = this_ds->prev();
    }
    curWindow->updateTreeWidget();
    curWindow->updateMeshData();

    if (hs->current_ds) {
        delete hs->current_ds;
        hs->current_ds = nullptr;
    }
    set_logging(true);
}

void MainWindow::undo() {
    HISTORY_STREAM* hs = get_default_stream();
    if (!hs) return;
    abort_bb(hs);

    

    
    
    int request_n = -1;
    int actual_n = 0;

    if (hs) {
        api_roll_n_states(hs, request_n, actual_n);
    }
    if (actual_n != request_n) {
        return;
    }

    buildTreeFromHistroy(hs);
    return;
}

void MainWindow::redo() {
    HISTORY_STREAM* hs = get_default_stream();
    int request_n = 1;
    int actual_n = 0;

    if (hs) {
        api_roll_n_states(hs, request_n, actual_n);
    }
    if (actual_n != request_n) {
        return;
    }

    buildTreeFromHistroy(hs);
    return;
}

bool MainWindow::save() {
    if (curWindow == nullptr) {
        const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("DBCAD"),
            tr("当前窗口未初始化\n"
                "是否初始化？"),
            QMessageBox::Yes | QMessageBox::No);
        switch (ret) {
            case QMessageBox::Yes:
                addWindow();
        }
        return false;
    }

    ENTITY_LIST el;
    acis_get_noattrib_toplevel_active_entities(el);
    if (el.count() == 0) {
        QMessageBox::warning(this, tr("DBCAD"), tr("当前无顶级活跃实体。"));
        return false;
    }

    QAction* checkedAct = setModeActGroup->checkedAction();

    if (checkedAct == setACISModeAct) {
        if (curWindow->getCurFile().isEmpty()) {
            return saveAs();
        } else {
            return saveFile(curWindow->getCurFile());
        }
    } else {
        assert(checkedAct == setNEO4JModeAct || checkedAct == setNEO4JIncrementalModeAct || checkedAct == setFASTAPIModeAct);
        if (curWindow->getCurPartName().isEmpty()) {
            return saveAs();
        } else {
            return saveFile(curWindow->getCurPartName());
        }
    }
}

void MainWindow::saveEntity() {
    if (curWindow == nullptr) {
        const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("DBCAD"),
            tr("当前窗口未初始化\n"
                "是否初始化？"),
            QMessageBox::Yes | QMessageBox::No);
        switch (ret) {
            case QMessageBox::Yes:
                addWindow();
        }
        return;
    }

    if (curWindow->getSelectedEntities().size() == 0) {
        QMessageBox::warning(this, tr("DBCAD"), tr("请在实体树中选择导出对象"), QMessageBox::Ok);
        return;
    }

    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix(QString("txt"));
    if (dialog.exec() != QDialog::Accepted) return;
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    bool ss = false;
    FILE* file = fopen(dialog.selectedFiles().first().toStdString().c_str(), "a+");
    if (file) {
        for (auto ei : curWindow->getSelectedEntities()) {
            ENTITY_TREE_ITEM* ptrEti = curWindow->getEntityItemByIndex(ei);
            dump_object(file, ptrEti->name.c_str(), 0, ptrEti->ptrEntity);
        }
        fclose(file);
        ss = true;
    }
    QGuiApplication::restoreOverrideCursor();
    if (ss)
        statusBar()->showMessage(tr("已保存"), 2000);
    else
        QMessageBox::warning(this, tr("DBCAD"), tr("保存文件%1失败。").arg(QDir::toNativeSeparators(dialog.selectedFiles().first())));
}

void MainWindow::saveDebugFile() {
    if (curWindow == nullptr) {
        const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("DBCAD"),
            tr("当前窗口未初始化\n"
                "是否初始化？"),
            QMessageBox::Yes | QMessageBox::No);
        switch (ret) {
            case QMessageBox::Yes:
                addWindow();
        }
        return;
    }

    if (curWindow->getSelectedEntities().size() == 0) {
        QMessageBox::warning(this, tr("DBCAD"), tr("请在实体树中选择导出对象"), QMessageBox::Ok);
        return;
    }

    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix(QString("dbg"));
    if (dialog.exec() != QDialog::Accepted) return;
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    bool ss = false;
    FILE* file = fopen(dialog.selectedFiles().first().toStdString().c_str(), "a+");
    if (file) {
        for (auto ei : curWindow->getSelectedEntities()) {
            ENTITY_TREE_ITEM* ptrEti = curWindow->getEntityItemByIndex(ei);
            api_set_int_option("brief_surface_debug", false);
            api_set_int_option("brief_curve_debug", false);
            api_set_int_option("brief_pcurve_debug", false);
            debug_entity(ptrEti->ptrEntity, file);
        }
        fclose(file);
        ss = true;
    }
    QGuiApplication::restoreOverrideCursor();
    if (ss)
        statusBar()->showMessage(tr("已保存"), 2000);
    else
        QMessageBox::warning(this, tr("DBCAD"), tr("保存文件%1失败。").arg(QDir::toNativeSeparators(dialog.selectedFiles().first())));
}

bool MainWindow::saveAs() {
    QAction* checkedAct = setModeActGroup->checkedAction();
    if (checkedAct == setACISModeAct) {
        QFileDialog dialog(this);
        dialog.setWindowModality(Qt::WindowModal);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setDefaultSuffix(QString("sat"));
        if (dialog.exec() != QDialog::Accepted) return false;
        return saveFile(dialog.selectedFiles().first());
    } else {
        assert(checkedAct == setNEO4JModeAct || checkedAct == setNEO4JIncrementalModeAct || checkedAct == setFASTAPIModeAct);
        bool ok;
        QDateTime curDateTime = QDateTime::currentDateTime();
        QString curDateTime_qstr = QString("part") + curDateTime.toString("yyyyMMddhhmmss");  // 保存零件对话框的默认零件名是part+当前年月日时分秒
        QString title = checkedAct == setFASTAPIModeAct ? tr("保存零件(FastAPI)") : tr("保存零件(neo4j)");
        QString text = QInputDialog::getText(this, title, tr("请输入保存的零件名:"), QLineEdit::Normal, curDateTime_qstr, &ok, this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
        if (ok && !text.isEmpty()) {
            return saveFile(text);
        } else {
            return false;
        }
    }
}

void MainWindow::visiablility() {
    if (curWindow) curWindow->visiablility();
}

void MainWindow::displayInfo() {
    if (curWindow) curWindow->displayInfo();
}

void MainWindow::about() {

    QMessageBox::about(this, tr("关于DBCAD"), tr("用于测试和演示DBCAD功能与用法。"));
}

void MainWindow::runTest() {
    Neo4jPart conn(neo4jdb_host.c_str(), neo4jdb_port_bolt, neo4jdb_username.c_str(), neo4jdb_password.c_str(), "");

    for (const auto& entry : std::filesystem::directory_iterator(".\\testcases")) {
        if (entry.is_regular_file()) {
            std::string filepath = entry.path().string();
            std::string extname = std::filesystem::path(filepath).extension().string();
            std::transform(extname.begin(), extname.end(), extname.begin(), tolower); //忽略大小写
            if (extname == ".sat") {
                std::string TestCaseName = std::filesystem::path(filepath).stem().string();
                ENTITY_LIST el;
                acis_restore_entity_list(el, filepath.c_str(), 2, 0, true);

                auto [testresult, neo4j_save_duration, acis_save_duration, neo4j_restore_duration, acis_restore_duration] = AccessTest::CheckTestCase(conn, TestCaseName, el);
                api_del_entity_list(el);

                std::ofstream logfs("testlog.txt", std::ios::app);
                logfs << std::format("=======Test: {}=======", filepath) << std::endl;
                logfs << (testresult ? "PASS" : "FAIL") << "\t数据库存:" << neo4j_save_duration << "ms\t文件存:" << acis_save_duration << "ms\t数据库存/文件存:" << neo4j_save_duration / acis_save_duration << "\t数据库取:" << neo4j_restore_duration << "ms\t文件取:" << acis_restore_duration << "ms\t数据库取/文件取:" << neo4j_restore_duration / acis_restore_duration << std::endl;
                logfs.close();
            }
        }
    }

    QMessageBox::information(this, tr("DBCAD"), tr("neo4j测试运行结束，请打开./testlog.txt文件查看测试报告。"));
}

void MainWindow::incrementalTest() {
    TMDF;

    {

        std::ofstream logfs("incremental_dighole_testlog.txt", std::ios::app);
        BODY* block;
        check_outcome(api_solid_block(SPAposition(0.0, 0.0, 0.0), SPAposition(1000.0, 2000.0, 300.0), block));
        bool is_through = false;
        int holecnt = 0;
        for (int i = 10; i <= 990; i += 20) { //50个
            for (int j = 10; j <= 1990; j += 20) { //100个
                BODY* hole;
                check_outcome(api_solid_cylinder_cone(SPAposition(i, j, is_through ? -10.0 : 100.0), SPAposition(i, j, 310.0), 5.0, 5.0, 5.0, nullptr, hole));
                check_outcome(api_subtract(hole, block));
                holecnt++;

                std::string acis_save_filename_str = std::format("acis_partTest_save_{}.sat", holecnt);
                const char* acis_save_filename = acis_save_filename_str.c_str();
                std::string acis_save_check_filename_str = std::format("check_acis_partTest_save_{}.sat", holecnt);
                const char* acis_save_check_filename = acis_save_check_filename_str.c_str();
                std::string conn_check_filename_str = std::format("check_incrementalTest_{}.sat", holecnt);
                const char* conn_check_filename = conn_check_filename_str.c_str();

                Neo4jPart conn_incremental(neo4jdb_host.c_str(), neo4jdb_port_bolt, neo4jdb_username.c_str(), neo4jdb_password.c_str(), "incrementalTest");

                //增量存
                TMST;
                IncrementalContext incCtx_incrementalSave;
                api_save_neo4j(conn_incremental, incCtx_incrementalSave);
                TMED;
                double neo4j_incremental_save_duration = TMDR;

                //ACIS存
                TMST;
                acis_save_noattrib_toplevel_active_entities(acis_save_filename, 2, 0, true);
                TMED;
                double acis_save_duration = TMDR;

                is_through = !is_through;
            }
        }
    }

    QMessageBox::information(this, tr("DBCAD"), tr("增量存取测试结束。"));
}

void MainWindow::createActions() {
    QMenu* fileMenu = menuBar()->addMenu(tr("文件(&F)"));
    QToolBar* fileToolBar = addToolBar(tr("文件"));

    // 清空当前Window
    const QIcon clearIcon = QIcon::fromTheme("document-clear", QIcon(":/images/clear.png"));
    QAction* clearAct = new QAction(clearIcon, tr("清空(&L)"), this);
    clearAct->setStatusTip(tr("清空"));
    connect(clearAct, &QAction::triggered, this, &MainWindow::clear);
    fileMenu->addAction(clearAct);
    fileToolBar->addAction(clearAct);

    // 创建Window
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction* newAct = new QAction(newIcon, tr("新建(&N)"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("新建文件"));
    connect(newAct, &QAction::triggered, this, &MainWindow::addWindow);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction* openAct = new QAction(openIcon, tr("打开(&O)"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("打开已有文件"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

    const QIcon undoIcon = QIcon::fromTheme("document-undo", QIcon(":/images/undo.png"));
    QAction* undoAct = new QAction(undoIcon, tr("撤销(&U)"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("撤销"));
    connect(undoAct, &QAction::triggered, this, &MainWindow::undo);
    fileMenu->addAction(undoAct);
    fileToolBar->addAction(undoAct);

    const QIcon redoIcon = QIcon::fromTheme("document-redo", QIcon(":/images/redo.png"));
    QAction* redoAct = new QAction(redoIcon, tr("重做(&Y)"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("重做"));
    connect(redoAct, &QAction::triggered, this, &MainWindow::redo);
    fileMenu->addAction(redoAct);
    fileToolBar->addAction(redoAct);

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction* saveAct = new QAction(saveIcon, tr("保存模型(&S)"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("保存模型"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);

    const QIcon saveEntityIcon = QIcon::fromTheme("document-save", QIcon(":/images/save_entity.png"));
    QAction* saveEntityAct = new QAction(saveEntityIcon, tr("保存模型树(&D)"), this);
    saveEntityAct->setShortcut(tr("ctrl+D"));
    saveEntityAct->setStatusTip(tr("导出模型结构"));
    connect(saveEntityAct, &QAction::triggered, this, &MainWindow::saveEntity);
    fileMenu->addAction(saveEntityAct);
    fileToolBar->addAction(saveEntityAct);

    const QIcon saveDebugFileIcon = QIcon::fromTheme("document-save", QIcon(":/images/save_entity.png"));
    QAction* saveDebugFileAct = new QAction(saveDebugFileIcon, tr("保存调试文件(&D)"), this);
    saveDebugFileAct->setStatusTip(tr("保存调试文件"));
    connect(saveDebugFileAct, &QAction::triggered, this, &MainWindow::saveDebugFile);
    fileMenu->addAction(saveDebugFileAct);
    fileToolBar->addAction(saveDebugFileAct);

    const QIcon saveImageIcon = QIcon::fromTheme("document-save", QIcon(":/images/save_image.png"));
    QAction* saveImageAct = new QAction(saveImageIcon, tr("保存截图(&T)"), this);
    saveImageAct->setStatusTip(tr("保存截图"));
    connect(saveImageAct, &QAction::triggered, this, &MainWindow::saveImage);
    fileMenu->addAction(saveImageAct);
    fileToolBar->addAction(saveImageAct);

    const QIcon saveAsIcon = QIcon::fromTheme("document-save-as", QIcon(":/images/saveas.png"));
    QAction* saveAsAct = new QAction(saveAsIcon, tr("另存为(&A)"), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("另存为新模型"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);
    fileMenu->addAction(saveAsAct);
    fileToolBar->addAction(saveAsAct);

    const QIcon visiablilityIcon = QIcon::fromTheme("document-visiablility", QIcon(":/images/visiablility.png"));
    QAction* visiablilityAct = new QAction(visiablilityIcon, tr("显示(&V)"), this);
    visiablilityAct->setStatusTip(tr("是否显示"));
    connect(visiablilityAct, &QAction::triggered, this, &MainWindow::visiablility);
    fileMenu->addAction(visiablilityAct);
    fileToolBar->addAction(visiablilityAct);

    const QIcon infoIcon = QIcon::fromTheme("document-info", QIcon(":/images/info.png"));
    QAction* infoAct = new QAction(infoIcon, tr("模型信息(&I)"), this);
    infoAct->setStatusTip(tr("模型信息"));
    connect(infoAct, &QAction::triggered, this, &MainWindow::displayInfo);
    fileMenu->addAction(infoAct);
    fileToolBar->addAction(infoAct);

    fileMenu->addSeparator();

    // 退出系统
    const QIcon exitIcon = QIcon::fromTheme("application-exit", QIcon(":/images/exit.png"));
    QAction* exitAct = fileMenu->addAction(exitIcon, tr("退出(&X)"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("退出系统"));

    // 插入基本体的菜单项和工具栏
    QMenu* insertMenu = menuBar()->addMenu(tr("插入(&I)"));
    QMenu* insertSubMenu = nullptr;
    QToolBar* insertSubToolBar = nullptr;
    for (auto it = menus_entities.begin(); it != menus_entities.end(); ++it) {
        if (it->operatorType == OPERATOR_TYPES::OPERATOR_SPERATOR) {
            if (it->subOperatorType == 0) {
                insertSubMenu = insertMenu->addMenu(it->name_cn.c_str());
                insertSubToolBar = addToolBar(tr("插入%1").arg(it->name_cn.c_str()));
            } else if (it->subOperatorType == 1)
                insertToolBarBreak(insertSubToolBar);

            continue;
        }
        const QIcon icon = QIcon::fromTheme("application", QIcon(QString::fromStdString(":/images/" + it->name_en + ".png")));
        QAction* act = insertSubMenu->addAction(icon, tr(it->name_cn.c_str()), this, [=]() { this->insertElements(it->operatorType, it->subOperatorType); });
        act->setCheckable(it->checkable);
        act->setStatusTip(QString::fromStdString("插入" + it->name_cn));
        insertSubToolBar->addAction(act);
    }

    // 插入布尔运算菜单项和工具栏
    QMenu* operatorMenu = menuBar()->addMenu(tr("操作(&O)"));
    QToolBar* operatorToolBar = addToolBar(tr("操作"));
    for (auto it = menus_operators.begin(); it != menus_operators.end(); ++it) {
        if (it->operatorType == OPERATOR_TYPES::OPERATOR_SPERATOR) {
            operatorMenu->addSeparator();
            operatorToolBar = addToolBar(tr("操作"));
            continue;
        }
        const QIcon icon = QIcon::fromTheme("application", QIcon(QString::fromStdString(":/images/" + it->name_en + ".png")));
        QAction* act = operatorMenu->addAction(icon, tr(it->name_cn.c_str()), this, [=]() { this->operation(it->operatorType, it->subOperatorType); });
        act->setCheckable(it->checkable);
        act->setStatusTip(QString::fromStdString(it->name_cn));
        operatorToolBar->addAction(act);
    }

    // 使用帮助
    QMenu* helpMenu = menuBar()->addMenu(tr("帮助(&H)"));
    QAction* aboutAct = helpMenu->addAction(tr("关于(&A)"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("显示系统说明"));
    QAction* runTestAct = helpMenu->addAction(tr("运行测试neo4j(&T)"), this, &MainWindow::runTest);
    runTestAct->setStatusTip(tr("使用./testcases目录下的SAT文件（2.0版本，不含attrib类实体）作为测试用例，对基于neo4j数据库的几何模型存取接口进行功能和性能测试，测试报告以追加方式输出至./testlog.txt文件"));
    QAction* incrementalTestAct = helpMenu->addAction(tr("增量存取测试"), this, &MainWindow::incrementalTest);
    incrementalTestAct->setStatusTip(tr("增量存取测试"));


    // 设置存取模式
    QMenu* settingsMenu = menuBar()->addMenu(tr("设置存取模式(&S)"));

    setACISModeAct = new QAction(tr("SAT文件(ACIS)(&A)"), this);
    setACISModeAct->setStatusTip(tr("存取为SAT文件，存取函数使用ACIS提供的API"));
    setACISModeAct->setCheckable(true);
    settingsMenu->addAction(setACISModeAct);

    setNEO4JModeAct = new QAction(tr("neo4j图数据全量存取(&N)"), this);
    setNEO4JModeAct->setStatusTip(tr("全量存取为neo4j图数据，通过Bolt协议连接到neo4j图数据库"));
    setNEO4JModeAct->setCheckable(true);
    settingsMenu->addAction(setNEO4JModeAct);

    setNEO4JIncrementalModeAct = new QAction(tr("neo4j图数据增量存取(&I)"), this);
    setNEO4JIncrementalModeAct->setStatusTip(tr("增量存取为neo4j图数据，通过Bolt协议连接到neo4j图数据库"));
    setNEO4JIncrementalModeAct->setCheckable(true);
    settingsMenu->addAction(setNEO4JIncrementalModeAct);

    setFASTAPIModeAct = new QAction(tr("FastAPI远程版本存取(&F)"), this);
    setFASTAPIModeAct->setStatusTip(tr("通过HTTP连接FastAPI后端，进行模型版本化远程存取"));
    setFASTAPIModeAct->setCheckable(true);
    settingsMenu->addAction(setFASTAPIModeAct);

    setModeActGroup = new QActionGroup(this);
    setModeActGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::Exclusive);
    setModeActGroup->addAction(setACISModeAct);
    setModeActGroup->addAction(setNEO4JModeAct);
    setModeActGroup->addAction(setNEO4JIncrementalModeAct);
    setModeActGroup->addAction(setFASTAPIModeAct);
    setACISModeAct->setChecked(true);

    settingsMenu->addSeparator();
    QAction* setNEO4JConnectInfoAct = settingsMenu->addAction(tr("设置neo4j连接信息"), this, &MainWindow::setNEO4JConnectInfo);
    setNEO4JConnectInfoAct->setStatusTip(tr("读取与程序相同目录下的neo4j_connect_info.conf文件，设置neo4j图数据库连接信息。该文件第1行是IP地址，第2行是Bolt协议端口号，第3行是用户名，第4行是密码，不能包含空格和多余的换行符，编码须为UTF-8（无BOM）。"));
    settingsMenu->addAction(setNEO4JConnectInfoAct);
    QAction* setFastAPIConnectInfoAct = settingsMenu->addAction(tr("设置FastAPI连接信息"), this, &MainWindow::setFastAPIConnectInfo);
    setFastAPIConnectInfoAct->setStatusTip(tr("配置FastAPI地址、作者名与连接密码，并保存到fastapi_connect_info.conf（前三行）。"));
    settingsMenu->addAction(setFastAPIConnectInfoAct);

    QAction* setBridgeConnectInfoAct = settingsMenu->addAction(tr("设置Bridge模式信息"), this, &MainWindow::setBridgeConnectInfo);
    setBridgeConnectInfoAct->setStatusTip(tr("配置Bridge监听地址与端口，并保存到storage_bridge_connect_info.conf（前两行）。"));
    settingsMenu->addAction(setBridgeConnectInfoAct);

    toggleBridgeModeAct = settingsMenu->addAction(tr("启动Bridge模式"), this, &MainWindow::toggleBridgeMode);
    toggleBridgeModeAct->setCheckable(true);
    toggleBridgeModeAct->setStatusTip(tr("以独立CAD进程启动/停止C++ Bridge服务，便于手动联调FastAPI与客户端。"));
    settingsMenu->addAction(toggleBridgeModeAct);

    // ----------------------------------------------------------------
    // PostgreSQL 直存 SAT 文本（m_pgService 入口）。
    // 不会改动 storage_bridge_service 或 saveFile() —— 走 Qt signal/slot 调
    // m_pgService (PgService wrapping PgStore/libpq)。连接配置在 pg_connect_info.conf
    // (host/port/user/password/dbname 五行)，运行时可通过"设置 PostgreSQL 连接信息"覆盖。
    // ----------------------------------------------------------------
    QMenu* pgMenu = menuBar()->addMenu(tr("PostgreSQL(&P)"));
    pgSaveAct = pgMenu->addAction(tr("保存当前模型到 PostgreSQL"), this, &MainWindow::pgSaveCurrentToDatabase);
    pgSaveAct->setStatusTip(tr("将当前 ACIS 实体导出为 SAT，调 PgService::saveSat 写入 dbcad_pg_demo 表。"));
    pgLoadAct = pgMenu->addAction(tr("从 PostgreSQL 加载到当前窗口..."), this, &MainWindow::pgLoadFromDatabaseToCurrent);
    pgLoadAct->setStatusTip(tr("弹框输入零件名，调 PgService::loadSat 拉到本地，再用 api_restore_entity_list 重建实体。"));
    pgListAct = pgMenu->addAction(tr("列出已存零件"), this, &MainWindow::pgListPartsInDatabase);
    pgListAct->setStatusTip(tr("调 PgService::listParts，在弹窗里看 id/name/bytes/updated_at。"));
    pgCountAct = pgMenu->addAction(tr("统计已存零件数"), this, &MainWindow::pgCountPartsInDatabase);
    pgCountAct->setStatusTip(tr("调 PgService::countParts，结果通过 statusBar 显示。"));
    pgDeleteAct = pgMenu->addAction(tr("删除指定零件..."), this, &MainWindow::pgDeleteByNameFromDatabase);
    pgDeleteAct->setStatusTip(tr("弹框输入零件名，二次确认后调 PgService::deleteByName 删除该零件（不可撤销）。"));
    pgMenu->addSeparator();
    QAction* setPGConnectInfoAct = pgMenu->addAction(tr("设置 PostgreSQL 连接信息"), this, &MainWindow::setPGConnectInfo);
    setPGConnectInfoAct->setStatusTip(tr("读取/写入 pg_connect_info.conf（前 5 行：host/port/user/password/dbname）；保存后会自动重建 PgService 并重连信号。"));
    updatePgMenuState();

    QMenu* collabMenu = menuBar()->addMenu(tr("协作(&C)"));
    QAction* collabPushAct = collabMenu->addAction(tr("推送(Push)"), this, &MainWindow::onCollabPushButtonClicked);
    collabPushAct->setStatusTip(tr("把本地未推送的实体图增量推送到服务器。"));
    QAction* collabPullAct = collabMenu->addAction(tr("拉取(Pull)"), this, &MainWindow::onCollabPullButtonClicked);
    collabPullAct->setStatusTip(tr("从服务器拉取最新增量：远端新增的 body 会加到本地画布，远端删除的 body 会被跳过（保留本地未推送的新增）。"));
    QAction* collabSyncNowAct = collabMenu->addAction(tr("刷新连接"), this, &MainWindow::requestFastAPISyncNow);
    collabSyncNowAct->setStatusTip(tr("主动请求服务器返回最新版本（仅刷新版本号，不修改画布）。"));
    QAction* collabReconnectAct = collabMenu->addAction(tr("重连协作通道"), this, &MainWindow::reconnectFastAPISync);
    collabReconnectAct->setStatusTip(tr("重建WebSocket协作连接。"));

    collabDock = new QDockWidget(tr("多人协作控制台"), this);
    collabDock->setObjectName("CollabDock");
    QWidget* collabBody = new QWidget(collabDock);
    QVBoxLayout* collabLayout = new QVBoxLayout(collabBody);

    collabConnectionLabel = new QLabel(tr("未连接"), collabBody);
    collabProjectLabel = new QLabel(tr("项目：未打开"), collabBody);
    collabVersionLabel = new QLabel(tr("本地版本：0"), collabBody);
    collabPendingLabel = new QLabel(tr("待同步版本：无"), collabBody);
    collabMembersList = new QListWidget(collabBody);
    collabMembersList->setMinimumHeight(120);
    collabMembersList->setToolTip(tr("当前在线协作者列表"));
    collabAutoFollowCheckBox = new QCheckBox(tr("自动跟随远程最新版本"), collabBody);
    collabAutoFollowCheckBox->setChecked(true);
    collabSyncNowButton = new QPushButton(tr("刷新"), collabBody);
    collabPushButton = new QPushButton(tr("推送(Push)"), collabBody);
    collabPullButton = new QPushButton(tr("拉取(Pull)"), collabBody);
    collabReconnectButton = new QPushButton(tr("重连"), collabBody);

    connect(collabAutoFollowCheckBox, &QCheckBox::toggled, this, [this](bool checked) {
        fastapiAutoFollowRemote = checked;
        updateCollabPanelUi();
    });
    connect(collabSyncNowButton, &QPushButton::clicked, this, &MainWindow::requestFastAPISyncNow);
    connect(collabPushButton, &QPushButton::clicked, this, &MainWindow::onCollabPushButtonClicked);
    connect(collabPullButton, &QPushButton::clicked, this, &MainWindow::onCollabPullButtonClicked);
    connect(collabReconnectButton, &QPushButton::clicked, this, &MainWindow::reconnectFastAPISync);

    collabLayout->addWidget(new QLabel(tr("连接状态"), collabBody));
    collabLayout->addWidget(collabConnectionLabel);
    collabLayout->addWidget(collabProjectLabel);
    collabLayout->addWidget(collabVersionLabel);
    collabLayout->addWidget(collabPendingLabel);
    collabLayout->addWidget(new QLabel(tr("在线协作者"), collabBody));
    collabLayout->addWidget(collabMembersList);
    collabLayout->addWidget(collabAutoFollowCheckBox);
    collabLayout->addWidget(collabPushButton);
    collabLayout->addWidget(collabPullButton);
    collabLayout->addWidget(collabSyncNowButton);
    collabLayout->addWidget(collabReconnectButton);
    collabLayout->addStretch();

    collabDock->setWidget(collabBody);
    addDockWidget(Qt::RightDockWidgetArea, collabDock);
    collabDock->show();
    updateCollabPanelUi();
}

void MainWindow::setNEO4JConnectInfo() {
    auto values = read_config_lines("neo4j_connect_info.conf", 4);
    if (!values.empty()) {
        if (!values[0].empty()) {
            neo4jdb_host = values[0];
        }
        if (!values[1].empty()) {
            try {
                neo4jdb_port_bolt = std::stoi(values[1]);
            } catch (...) {
                neo4jdb_port_bolt = 7687;
            }
        }
        neo4jdb_username = values[2];
        neo4jdb_password = values[3];
    } else if (neo4jdb_host.empty()) {
        neo4jdb_host = "127.0.0.1";
        neo4jdb_port_bolt = 7687;
        neo4jdb_username = "neo4j";
    }

    QDialog dialog(this);
    dialog.setWindowTitle(tr("neo4j连接配置"));
    dialog.setMinimumSize(420, 240);
    QFormLayout form(&dialog);

    QLineEdit hostEdit(&dialog);
    hostEdit.setText(QString::fromStdString(neo4jdb_host));
    form.addRow(tr("主机地址:"), &hostEdit);

    QSpinBox portEdit(&dialog);
    portEdit.setMinimum(1);
    portEdit.setMaximum(65535);
    portEdit.setValue(neo4jdb_port_bolt <= 0 ? 7687 : neo4jdb_port_bolt);
    form.addRow(tr("Bolt端口:"), &portEdit);

    QLineEdit userEdit(&dialog);
    userEdit.setText(QString::fromStdString(neo4jdb_username));
    form.addRow(tr("用户名:"), &userEdit);

    QLineEdit passwordEdit(&dialog);
    passwordEdit.setEchoMode(QLineEdit::Password);
    passwordEdit.setText(QString::fromStdString(neo4jdb_password));
    form.addRow(tr("密码:"), &passwordEdit);

    QPushButton testButton(tr("测试连接"), &dialog);
    form.addRow(&testButton);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    QObject::connect(&testButton, &QPushButton::clicked, &dialog, [&]() {
        const QString host = hostEdit.text().trimmed();
        if (host.isEmpty()) {
            QMessageBox::warning(this, tr("neo4j连接测试"), tr("主机地址不能为空"));
            return;
        }

        try {
            Neo4jPart conn(
                host.toStdString().c_str(),
                portEdit.value(),
                userEdit.text().trimmed().toStdString().c_str(),
                passwordEdit.text().toStdString().c_str(),
                "");
            const int64_t projectCount = count_partnode(conn);
            Q_UNUSED(projectCount);
            QMessageBox::information(this, tr("neo4j连接测试"), tr("连接成功。"));
        } catch (const std::exception& ex) {
            QMessageBox::warning(this, tr("neo4j连接测试"), tr("连接失败：%1").arg(QString::fromUtf8(ex.what())));
        }
    });

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    const QString host = hostEdit.text().trimmed();
    if (host.isEmpty()) {
        QMessageBox::warning(this, tr("neo4j连接配置"), tr("主机地址不能为空"));
        return;
    }

    neo4jdb_host = host.toStdString();
    neo4jdb_port_bolt = portEdit.value();
    neo4jdb_username = userEdit.text().trimmed().toStdString();
    neo4jdb_password = passwordEdit.text().toStdString();

    QString writeError;
    if (!write_config_lines(
            "neo4j_connect_info.conf",
            { neo4jdb_host, std::to_string(neo4jdb_port_bolt), neo4jdb_username, neo4jdb_password },
            &writeError)) {
        QMessageBox::warning(this, tr("neo4j连接配置"), writeError);
        return;
    }

    statusBar()->showMessage(tr("neo4j连接信息已更新"), 2000);
}

void MainWindow::clear() {
    curWindow->clear();
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage(tr("已就绪"));
}


void MainWindow::insertElements(const OPERATOR_TYPES ot, const int subOperatorType) {
    if (!curWindow) return;

    ENTITY *ptrEntity = nullptr;
    std::vector<std::vector<SPAposition>> handles;

    // 1. 开始一个新的事务（开启 bulletin board）
    api_bb_begin(TRUE);          // TRUE = 线性历史流

    // 2. 执行建模操作
    curWindow->createEntity(subOperatorType, ptrEntity, handles);

    // 3. 决定事务结果
    outcome result;              // 临时的 outcome，用于 api_bb_end
    if (ptrEntity != nullptr) {
        // 成功：关闭 bulletin board 并标注为成功
        api_bb_end(result, TRUE, FALSE);   // 第三个参数 FALSE 表示不删除 stacked BB
        if (result.ok()) {
            // 4. 成功：记录当前状态为 DELTA_STATE
            DELTA_STATE *ds = nullptr;
            outcome note_result = api_note_state(ds);
            
            if (ds && ds->id() != 0) {
                // 附加自定义数据
                std::string name_cn = "实体";
                for (auto mi : menus_entities) {
                    if (mi.operatorType == ot && mi.subOperatorType == subOperatorType)
                        name_cn = mi.name_cn;
                }
                GME_DELTA_STATE_user_data *userData = ACIS_NEW GME_DELTA_STATE_user_data();
                userData->add_tree_item(curWindow->addEntity(ptrEntity, name_cn, subOperatorType, handles));
                ds->set_user_data(userData);
            }
        }
    }
    else {
        // 失败：关闭 bulletin board 并标注为失败（这会自动回滚模型）
        api_bb_end(result, FALSE, FALSE);  // 第二个参数 FALSE 表示线性历史流（失败也需标记）
        // 注意：result 此时会包含错误信息，但不需额外处理
    }

    
    

    // curWindow->addEntity(ptrEntity, name_cn, subOperatorType, handles);
    //  @todo：以下版本管理导致demo_qt创建任意实体后再交互式插入B样条时程序崩溃。
    /*GME_DELTA_STATE_user_data* delta_state_user_data = ACIS_NEW GME_DELTA_STATE_user_data();
    delta_state_user_data->add_tree_item(curWindow->addEntity(ptrEntity, name_cn, subOperatorType, handles));

    DELTA_STATE* ds = nullptr;
    api_note_state(ds);
    ds->set_user_data(delta_state_user_data);*/

    // debug_history_stream();
}

void MainWindow::operation(const OPERATOR_TYPES ot, const int subOperatorType) {
    if (!curWindow) return;

    std::vector<int> ids = curWindow->getSelectedEntities();
    ENTITY_TREE_ITEM* eti0 = curWindow->getEntityItemByIndex(ids[0]);
    std::string eti0_name = eti0->name;
    ENTITY_TREE_ITEM* eti1 = curWindow->getEntityItemByIndex(ids[1]);
    std::string eti1_name = eti1->name;

    ENTITY_LIST* el = ACIS_NEW ENTITY_LIST();
    if (!curWindow->operation(ot, subOperatorType, ids, el, true)) return;
    std::string name_cn = "运算结果";
    for (auto mi : menus_operators) {
        if (mi.operatorType == ot && mi.subOperatorType == subOperatorType) name_cn = mi.name_cn;
    }
    std::string name_cn2 = std::format("[{}]和[{}]的{}", eti0_name, eti1_name, name_cn);

    ids.clear(); //两实体做布尔运算后，只保留运算结果，删除参与运算的两实体

    GME_DELTA_STATE_user_data* delta_state_user_data = ACIS_NEW GME_DELTA_STATE_user_data();
    for (ENTITY* ent = el->first(); ent; ent = el->next()) {
        delta_state_user_data->add_tree_item(curWindow->addEntity(ent, name_cn2, subOperatorType, ids, ot));
    }

    DELTA_STATE* ds = nullptr;
    api_note_state(ds);
    ds->set_user_data(delta_state_user_data);
    // debug_history_stream();
}

void MainWindow::saveImage() {
    if (curWindow == nullptr) {
        const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("DBCAD"),
            tr("当前窗口未初始化\n"
                "是否初始化？"),
            QMessageBox::Yes | QMessageBox::No);
        switch (ret) {
            case QMessageBox::Yes:
                addWindow();
        }
        return;
    }
    if (curWindow->getEntityList().count() == 0) {
        QMessageBox::warning(this, tr("DBCAD"), tr("当前窗口为空。"));
        return;
    }
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix(QString("png"));
    if (dialog.exec() != QDialog::Accepted) return;
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QImage im = curWindow->getScreenshot();
    bool ss = im.save(dialog.selectedFiles().first(), "PNG");
    QGuiApplication::restoreOverrideCursor();
    if (ss)
        statusBar()->showMessage(tr("截图已保存"), 2000);
    else
        QMessageBox::warning(this, tr("DBCAD"), tr("保存文件%1失败。").arg(QDir::toNativeSeparators(dialog.selectedFiles().first())));
}

bool MainWindow::maybeSave() {
    if (curWindow->getIsModified()) {
        const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("DBCAD"),
            tr("模型已修改。\n"
                "是否保存？"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        switch (ret) {
            case QMessageBox::Save:
                return save();
            case QMessageBox::Cancel:
                return false;
            default:
                break;
        }
    }
    return true;
}

void MainWindow::loadFile(const QString& fileName) {
    if (curWindow == nullptr) addWindow();

    curWindow->clear();

    bool isRead = false;

#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    QAction* checkedAct = setModeActGroup->checkedAction();

    if (checkedAct == setACISModeAct) {
        FILE* f = nullptr;
        f = fopen(fileName.toStdString().c_str(), "r");
        if (f) {
            ENTITY_LIST el;

            API_BEGIN;
            api_save_version(2, 0);
            result = api_restore_entity_list(f, true, el);
            API_END;

            for (int i = 0; i < el.count(); i++) curWindow->addEntity(el[i], tr("导入(ACIS)实体%1").arg(i).toStdString(), -1);
            fclose(f);
            isRead = true;
        } else {
            QMessageBox::warning(this, tr("DBCAD"), tr("无法读取文件%1。").arg(QDir::toNativeSeparators(fileName)));
        }
    } else if (checkedAct == setNEO4JModeAct) {
        try {
            Neo4jPart f(neo4jdb_host.c_str(), neo4jdb_port_bolt, neo4jdb_username.c_str(), neo4jdb_password.c_str(), fileName.toStdString());
            int64_t countn = count_partnode(f);
            if (countn == 1) {
                ENTITY_LIST el;
                API_BEGIN;
                api_restore_entity_list_neo4j_part(f, el);
                API_END;
                for (int i = 0; i < el.count(); i++) curWindow->addEntity(el[i], tr("导入(neo4j)实体%1").arg(i).toStdString(), -1);
                isRead = true;
            } else if (countn == 0) {
                QMessageBox::warning(this, tr("DBCAD"), tr("名为%1的零件(neo4j)不存在。").arg(QString(fileName)));
            } else {
                assert(countn > 1);
                QMessageBox::warning(this, tr("DBCAD"), tr("名为%1的零件(neo4j)不唯一。").arg(QString(fileName)));
            }
        } catch (const std::exception& ex) {
            QMessageBox::warning(this, tr("DBCAD"), tr("连接neo4j失败：%1").arg(QString::fromUtf8(ex.what())));
        }
    } else if (checkedAct == setFASTAPIModeAct) {
        BackendApiClient client(
            QString::fromStdString(fastapi_base_url),
            QString::fromStdString(fastapi_author),
            QString::fromStdString(fastapi_password));
        if (!client.isConfigured()) {
            QMessageBox::warning(this, tr("DBCAD"), tr("FastAPI地址未配置，请先设置fastapi_connect_info.conf"));
        } else {
            auto project = client.getProjectByName(fileName);
            if (!project.has_value()) {
                if (client.lastStatusCode() == 404) {
                    auto created = client.createProject(fileName);
                    if (!created.has_value()) {
                        QMessageBox::warning(this, tr("DBCAD"), tr("创建协作项目失败：%1").arg(client.lastError()));
                    } else {
                        fastapi_project_id = created->id;
                        fastapi_project_name = fileName;
                        reconnectFastAPISync();
                        CollabSession::instance().setModelVersion(0);
                        setCurrentPartName(fileName);
                        statusBar()->showMessage(tr("已创建并加入协作项目（当前无版本，请先保存模型）"), 4000);
                        isRead = true;
                    }
                } else {
                    QString err = client.lastError();
                    if (err.isEmpty()) {
                        QMessageBox::warning(this, tr("DBCAD"), tr("查询协作项目失败"));
                    } else {
                        QMessageBox::warning(this, tr("DBCAD"), err);
                    }
                }
            } else {
                auto model = client.getLatestModel(project->id);
                if (!model.has_value()) {
                    if (client.lastStatusCode() == 404) {
                        fastapi_project_id = project->id;
                        fastapi_project_name = fileName;
                        reconnectFastAPISync();
                        CollabSession::instance().setModelVersion(0);
                        setCurrentPartName(fileName);
                        statusBar()->showMessage(tr("已加入协作项目（当前无版本，请先保存模型）"), 4000);
                        isRead = true;
                    } else {
                        QMessageBox::warning(this, tr("DBCAD"), client.lastError());
                    }
                } else if (restoreFastAPIModelFromSat(model->sat)) {
                    fastapi_project_id = project->id;
                    fastapi_project_name = fileName;
                    reconnectFastAPISync();
                    CollabSession::instance().setModelVersion(model->version);
                    isRead = true;
                }
            }
        }
    } else {
        throw std::runtime_error("不支持的加载模式");
    }

#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
        if (isRead) {
            curWindow->setIsModified(false);
            if (checkedAct == setACISModeAct) {
                disconnectFastAPISync();
                fastapi_project_id.clear();
                fastapi_project_name.clear();
                CollabSession::instance().setModelVersion(0);
                setCurrentFile(fileName);
                statusBar()->showMessage(tr("文件已导入"), 2000);
            } else if (checkedAct == setNEO4JModeAct) {
                disconnectFastAPISync();
                fastapi_project_id.clear();
                fastapi_project_name.clear();
                CollabSession::instance().setModelVersion(0);
                CollabSession::instance().setPendingRemoteVersion(0);
                setCurrentPartName(fileName);
                statusBar()->showMessage(tr("零件已导入(neo4j)"), 2000);
        } else if (checkedAct == setFASTAPIModeAct) {
            setCurrentPartName(fileName);
            statusBar()->showMessage(tr("零件已导入(FastAPI)，版本%1").arg(fastapi_model_version), 3000);
        } else {
            throw std::runtime_error("不支持的加载模式");
        }
    }
}

void MainWindow::loadFile(const QString& partName, const int generation) {
    if (curWindow == nullptr) addWindow();

    curWindow->clear();

    bool isRead = false;

#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    QAction* checkedAct = setModeActGroup->checkedAction();

    if (checkedAct == setNEO4JIncrementalModeAct) {
        try {
            Neo4jPart f(neo4jdb_host.c_str(), neo4jdb_port_bolt, neo4jdb_username.c_str(), neo4jdb_password.c_str(), partName.toStdString());
            int64_t countn = count_partnode(f);
            if (countn == 1) {
                IncrementalContext incCtx_incrementalRestore;
                api_restore_neo4j(f, generation, incCtx_incrementalRestore);
                ENTITY_LIST el;
                acis_get_noattrib_toplevel_active_entities(el);
                for (int i = 0; i < el.count(); i++) curWindow->addEntity(el[i], tr("导入(neo4j)实体%1").arg(i).toStdString(), -1);
                isRead = true;
            } else if (countn == 0) {
                QMessageBox::warning(this, tr("DBCAD"), tr("名为%1的零件(neo4j)不存在。").arg(QString(partName)));
            } else {
                assert(countn > 1);
                QMessageBox::warning(this, tr("DBCAD"), tr("名为%1的零件(neo4j)不唯一。").arg(QString(partName)));
            }
        } catch (const std::exception& ex) {
            QMessageBox::warning(this, tr("DBCAD"), tr("连接neo4j失败：%1").arg(QString::fromUtf8(ex.what())));
        }
    } else if (checkedAct == setFASTAPIModeAct) {
        BackendApiClient client(
            QString::fromStdString(fastapi_base_url),
            QString::fromStdString(fastapi_author),
            QString::fromStdString(fastapi_password));
        if (!client.isConfigured()) {
            QMessageBox::warning(this, tr("DBCAD"), tr("FastAPI地址未配置，请先设置fastapi_connect_info.conf"));
        } else {
            auto project = client.getProjectByName(partName);
            if (!project.has_value()) {
                QString err = client.lastError();
                if (err.isEmpty()) {
                    QMessageBox::warning(this, tr("DBCAD"), tr("名为%1的项目(FastAPI)不存在。").arg(QString(partName)));
                } else {
                    QMessageBox::warning(this, tr("DBCAD"), err);
                }
            } else {
                auto model = client.getModelVersion(project->id, generation);
                if (!model.has_value()) {
                    if (client.lastStatusCode() == 404) {
                        QMessageBox::warning(this, tr("DBCAD"), tr("协作项目存在，但指定版本不存在。"));
                    } else {
                        QMessageBox::warning(this, tr("DBCAD"), client.lastError());
                    }
                } else if (restoreFastAPIModelFromSat(model->sat)) {
                    fastapi_project_id = project->id;
                    fastapi_project_name = partName;
                    reconnectFastAPISync();
                    CollabSession::instance().setModelVersion(model->version);
                    isRead = true;
                }
            }
        }
    } else {
        throw std::runtime_error("不支持的指定版本加载模式");
    }

#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
    if (isRead) {
        curWindow->setIsModified(false);
        setCurrentPartName(partName);
        if (checkedAct == setFASTAPIModeAct) {
            statusBar()->showMessage(tr("零件已导入(FastAPI)，版本%1").arg(CollabSession::instance().modelVersion()), 3000);
        } else {
            disconnectFastAPISync();
            fastapi_project_id.clear();
            fastapi_project_name.clear();
            CollabSession::instance().setModelVersion(0);
            CollabSession::instance().setPendingRemoteVersion(0);
            statusBar()->showMessage(tr("零件已导入(neo4j)"), 2000);
        }
        updateCollabPanelUi();
    }

}

bool MainWindow::saveFile(const QString& fileName) {
    QString errorMessage;

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);

    QAction* checkedAct = setModeActGroup->checkedAction();

    if (checkedAct == setACISModeAct) {
        FILE* f = nullptr;
        f = fopen(fileName.toStdString().c_str(), "wb");
        if (f) {
            API_NOP_BEGIN;
            api_save_version(2, 0);
            FileInfo fileinfo;
            fileinfo.set_units(1.0);
            fileinfo.set_product_id("demo_qt");
            result = api_set_file_info((FileIdent | FileUnits), fileinfo);
            result = api_set_int_option("sequence_save_files", 1);
            ENTITY_LIST el;
            acis_get_noattrib_toplevel_active_entities(el);
            result = api_save_entity_list(f, true, el);
            API_NOP_END;
            fclose(f);
        } else {
            errorMessage = tr("无法以写入方式打开文件%1。").arg(QDir::toNativeSeparators(fileName));
        }

    } else if (checkedAct == setNEO4JModeAct) {
        try {
            Neo4jPart f(neo4jdb_host.c_str(), neo4jdb_port_bolt, neo4jdb_username.c_str(), neo4jdb_password.c_str(), fileName.toStdString());
            ENTITY_LIST el;
            acis_get_noattrib_toplevel_active_entities(el);
            api_save_entity_list_neo4j_part(f, el);
        } catch (const std::exception& ex) {
            errorMessage = tr("保存到neo4j失败：%1").arg(QString::fromUtf8(ex.what()));
        }
    } else if (checkedAct == setNEO4JIncrementalModeAct) {
        try {
            Neo4jPart f(neo4jdb_host.c_str(), neo4jdb_port_bolt, neo4jdb_username.c_str(), neo4jdb_password.c_str(), fileName.toStdString());
            IncrementalContext incCtx_incrementalSave2;
            api_save_neo4j(f, incCtx_incrementalSave2);
        } catch (const std::exception& ex) {
            errorMessage = tr("保存到neo4j失败：%1").arg(QString::fromUtf8(ex.what()));
        }
    } else if (checkedAct == setFASTAPIModeAct) {
        BackendApiClient client(
            QString::fromStdString(fastapi_base_url),
            QString::fromStdString(fastapi_author),
            QString::fromStdString(fastapi_password));
        if (!client.isConfigured()) {
            errorMessage = tr("FastAPI地址未配置，请先设置fastapi_connect_info.conf");
        } else {
            auto project = client.getOrCreateProject(fileName);
            if (!project.has_value()) {
                errorMessage = client.lastError();
            } else {
                QTemporaryFile tempFile(QDir::tempPath() + "/dbcad_save_XXXXXX.sat");
                tempFile.setAutoRemove(true);
                if (!tempFile.open()) {
                    errorMessage = tr("无法创建临时文件用于导出SAT");
                } else {
                    const QString tempPath = tempFile.fileName();
                    tempFile.close();

                    FILE* f = fopen(tempPath.toStdString().c_str(), "wb");
                    if (!f) {
                        errorMessage = tr("无法打开临时SAT文件用于写入");
                    } else {
                        API_NOP_BEGIN;
                        api_save_version(2, 0);
                        FileInfo fileinfo;
                        fileinfo.set_units(1.0);
                        fileinfo.set_product_id("dbcad_fastapi");
                        result = api_set_file_info((FileIdent | FileUnits), fileinfo);
                        result = api_set_int_option("sequence_save_files", 1);
                        ENTITY_LIST el;
                        acis_get_noattrib_toplevel_active_entities(el);
                        result = api_save_entity_list(f, true, el);
                        API_NOP_END;
                        fclose(f);

                        QFile satFile(tempPath);
                        if (!satFile.open(QIODevice::ReadOnly)) {
                            errorMessage = tr("读取临时SAT文件失败");
                        } else {
                            const QString satContent = QString::fromUtf8(satFile.readAll());
                            satFile.close();

                            auto& session = CollabSession::instance();
                            std::optional<int> baseVersion;
                            if (fastapi_project_id == project->id && session.modelVersion() > 0) {
                                baseVersion = session.modelVersion();
                            }

                            // 协作模式下：如果本地有待发的 entity_graph 改动，Ctrl+S 必须也走 entity_graph 增量提交，
                            // 而不是 HTTP POST 全量 SAT 路径。否则 server 的 _create_model_version_serialized
                            // 会创建一个没有 entity_graph 字段的版本，broadcast model_saved 给所有 client，
                            // 接收端会走 clear+restore 把画布清空，丢失对齐信息。
                            // 只有在 pendingEntityChanges 为空时（例如 fork / 首次保存）才退化走 HTTP POST。
                            std::optional<int> newVersion;
                            if (!pendingEntityChanges.isEmpty() && fastapiSyncSocket != nullptr && fastapiSyncSocket->isValid()) {
                                std::fprintf(stderr, "[saveFile FASTAPI] Ctrl+S intercept: %d pending entity changes, routing to submitEntityGraphIncremental instead of HTTP POST\n",
                                             (int)pendingEntityChanges.size());
                                QString entityGraphJson = exportEntityGraphToJson();
                                QString changesJson = exportEntityChangesToJson(pendingEntityChanges);
                                const QString egReason = fastapiLastPublishReason.isEmpty() ? QString::fromUtf8("ctrl-s") : fastapiLastPublishReason;
                                if (submitEntityGraphIncremental(entityGraphJson, changesJson, egReason)) {
                                    pendingEntityChanges.clear();
                                    // 用 session.modelVersion()+1 作为占位版本号，让下面那段"成功"分支正常执行；
                                    // 真正的最新版本号会在收到 entity_graph_saved 事件时由 CollabSession 更新。
                                    newVersion = session.modelVersion() + 1;
                                    errorMessage.clear();
                                } else {
                                    errorMessage = tr("协作增量提交失败");
                                }
                            } else {
                                newVersion = client.saveModel(project->id, satContent, baseVersion);
                            }
                            if (!newVersion.has_value()) {
                                if (client.lastStatusCode() == 409) {
                                    QMessageBox msg(this);
                                    msg.setIcon(QMessageBox::Warning);
                                    msg.setWindowTitle(tr("协作冲突"));
                                    msg.setText(tr("检测到版本冲突：远程已有更新。请选择处理方式。"));
                                    QPushButton* syncRemoteBtn = msg.addButton(tr("同步远程最新"), QMessageBox::AcceptRole);
                                    QPushButton* forceSubmitBtn = msg.addButton(tr("提交我的版本（基于最新重试）"), QMessageBox::ActionRole);
                                    QPushButton* forkBtn = msg.addButton(tr("另存为新协作项目"), QMessageBox::DestructiveRole);
                                    QPushButton* cancelBtn = msg.addButton(QMessageBox::Cancel);
                                    Q_UNUSED(syncRemoteBtn);
                                    Q_UNUSED(forceSubmitBtn);
                                    Q_UNUSED(forkBtn);
                                    msg.exec();

                                    if (msg.clickedButton() == cancelBtn) {
                                        errorMessage = tr("已取消保存");
                                    } else if (msg.clickedButton() == syncRemoteBtn) {
                                        auto latest = client.getLatestModel(project->id);
                                        if (!latest.has_value()) {
                                            errorMessage = tr("获取远程最新版本失败：%1").arg(client.lastError());
                                        } else {
                                            const GLWidget::ViewState viewState = curWindow->getViewState();
                                            curWindow->clear();
                                            if (!restoreFastAPIModelFromSat(latest->sat)) {
                                                errorMessage = tr("同步远程最新版本失败");
                                            } else {
                                                curWindow->setViewState(viewState);
                                                fastapi_project_id = project->id;
                                                fastapi_project_name = fileName;
                                                reconnectFastAPISync();
                                                session.setModelVersion(latest->version);
                                                session.setPendingRemoteVersion(0);
                                                errorMessage.clear();
                                            }
                                        }
                                    } else if (msg.clickedButton() == forceSubmitBtn) {
                                        auto latest = client.getLatestModel(project->id);
                                        if (!latest.has_value()) {
                                            errorMessage = tr("获取远程最新版本失败：%1").arg(client.lastError());
                                        } else {
                                            auto retriedVersion = client.saveModel(project->id, satContent, latest->version);
                                            if (!retriedVersion.has_value()) {
                                                errorMessage = tr("基于最新版本重试提交失败：%1").arg(client.lastError());
                                            } else {
                                                fastapi_project_id = project->id;
                                                fastapi_project_name = fileName;
                                                reconnectFastAPISync();
                                                session.setModelVersion(*retriedVersion);
                                                session.setPendingRemoteVersion(0);
                                                errorMessage.clear();
                                            }
                                        }
                                    } else {
                                        bool ok = false;
                                        QString newProjectName = QInputDialog::getText(
                                            this,
                                            tr("另存为新协作项目"),
                                            tr("请输入新项目名:"),
                                            QLineEdit::Normal,
                                            fileName + tr("-fork"),
                                            &ok,
                                            this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
                                        if (!ok || newProjectName.trimmed().isEmpty()) {
                                            errorMessage = tr("已取消另存为");
                                        } else {
                                            auto forkProject = client.getOrCreateProject(newProjectName.trimmed());
                                            if (!forkProject.has_value()) {
                                                errorMessage = tr("创建新协作项目失败：%1").arg(client.lastError());
                                            } else {
                                                auto forkVersion = client.saveModel(forkProject->id, satContent, std::nullopt);
                                                if (!forkVersion.has_value()) {
                                                    errorMessage = tr("保存到新协作项目失败：%1").arg(client.lastError());
                                                } else {
                                                    fastapi_project_id = forkProject->id;
                                                    fastapi_project_name = newProjectName.trimmed();
                                                    reconnectFastAPISync();
                                                    session.setModelVersion(*forkVersion);
                                                    session.setPendingRemoteVersion(0);
                                                    errorMessage.clear();
                                                }
                                            }
                                        }
                                    }
                                } else {
                                    errorMessage = client.lastError();
                                }
                            } else {
                                fastapi_project_id = project->id;
                                fastapi_project_name = fileName;
                                reconnectFastAPISync();
                                session.setModelVersion(*newVersion);
                                session.setPendingRemoteVersion(0);
                            }
                        }
                    }
                }
            }
        }
    } else {
        throw std::runtime_error("Cannot find the enum");
    }

    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, tr("DBCAD"), errorMessage);
        return false;
    } else
        curWindow->setIsModified(false);

    if (checkedAct == setACISModeAct) {
        disconnectFastAPISync();
        fastapi_project_id.clear();
        fastapi_project_name.clear();
        // session 状态已在 disconnectFastAPISync() → onDisconnected() 中清零
        fastapi_model_version = CollabSession::instance().modelVersion();
        setCurrentFile(fileName);
        statusBar()->showMessage(tr("文件已保存"), 2000);
    } else if (checkedAct == setFASTAPIModeAct) {
        auto& session = CollabSession::instance();
        setCurrentPartName(fileName);
        statusBar()->showMessage(tr("零件已保存(FastAPI)，版本%1").arg(session.modelVersion()), 3000);
        if (fastapiAutoFollowRemote && session.pendingRemoteVersion() > session.modelVersion() && !curWindow->getIsModified()) {
            applyPendingRemoteVersion();
        }
    } else if (checkedAct == setNEO4JModeAct || checkedAct == setNEO4JIncrementalModeAct) {
        disconnectFastAPISync();
        fastapi_project_id.clear();
        fastapi_project_name.clear();
        fastapi_model_version = CollabSession::instance().modelVersion();
        fastapi_pending_remote_version = CollabSession::instance().pendingRemoteVersion();
        setCurrentPartName(fileName);
        statusBar()->showMessage(tr("零件已保存(neo4j)"), 2000);
    } else {
        throw std::runtime_error("不支持的保存模式");
    }
    updateCollabPanelUi();
    return true;
}

void MainWindow::setCurrentFile(const QString& fileName) {
    curWindow->setCurFile(fileName);
    curWindow->setWindowModified(false);
}

void MainWindow::setCurrentPartName(const QString& fileName) {
    curWindow->setCurPartName(fileName);
    curWindow->setWindowModified(false);
}

void MainWindow::showMessage(const QString s, int duration) {
    if (duration == -1)
        statusBar()->showMessage(s);
    else
        statusBar()->showMessage(s, duration);
}

#ifndef QT_NO_SESSIONMANAGER
void MainWindow::commitData(QSessionManager& manager) {
    if (manager.allowsInteraction()) {
        if (!maybeSave()) manager.cancel();
    } else {
        // 自动保存
        if (curWindow->getIsModified()) save();
    }
}
#endif
