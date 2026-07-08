#include "mainwindow.h"
#include "pg_service.h"
#include "collab_session.h"


#include <QActionGroup>
#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFileDialog>
#include <QFile>
#include <QGuiApplication>
#include <QInputDialog>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QProcess>
#include <QStringConverter>
#include <QSettings>
#include <QScopedValueRollback>
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

    // 绑定协作状态机到 8 个旧字段（零行为变更：本阶段状态机只观测，不替代任何 if）
    CollabSession::instance().bindLegacyFields(
        fastapi_model_version,
        fastapi_pending_remote_version,
        fastapiLastPublishReason,
        fastapiPendingSubmitRequestId,
        fastapiSubmitInFlight,
        fastapiLocalDirtyDuringSubmit,
        fastapiApplyingRemoteSnapshot,
        fastapiPublishingSnapshot
    );
    qDebug().noquote() << "[CollabSession] bound:" << CollabSession::instance().dump(CollabSession::Event::Bound);

    CollabSession::instance().setDebugEnabled(true);
    CollabSession::instance().setMinDumpIntervalMs(0);
    CollabSession::instance().setEventMinInterval(CollabSession::Event::WsMessage, 200);
    qDebug().noquote() << "[CollabSession] ready: state machine active; ws_msg<=200ms.";
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
    fastapiSubmitInFlight = false;
    fastapiLocalDirtyDuringSubmit = false;
    fastapiPendingSubmitRequestId.clear();
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

void MainWindow::requestFastAPISyncNow() {
    if (fastapiSyncSocket != nullptr && fastapiSyncSocket->isValid()) {
        fastapiSyncSocket->sendTextMessage("sync_now");
        statusBar()->showMessage(tr("已请求服务器返回最新版本"), 1500);
    }
}

void MainWindow::notifyModelChangedForCollaboration() {
    if (fastapiApplyingRemoteSnapshot || fastapiPublishingSnapshot) {
        return;
    }

    QAction* checkedAct = setModeActGroup ? setModeActGroup->checkedAction() : nullptr;
    if (checkedAct != setFASTAPIModeAct) {
        return;
    }

    if (fastapi_project_id.isEmpty() || fastapi_project_name.isEmpty()) {
        return;
    }

    if (fastapiSubmitInFlight) {
        fastapiLocalDirtyDuringSubmit = true;
        fastapiLastPublishReason = tr("local-change");
        CollabSession::instance().onUserEditDuringInFlight();
        updateCollabPanelUi();
        return;
    }

    if (fastapi_pending_remote_version > fastapi_model_version) {
        statusBar()->showMessage(tr("Remote version pending; resolve it before submitting local changes."), 4000);
        updateCollabPanelUi();
        CollabSession::instance().onRemotePending(fastapi_pending_remote_version);
        return;
    }

    CollabSession::instance().onUserEdit();
    scheduleFastAPIAutoPublish(tr("local-change"));
}

void MainWindow::scheduleFastAPIAutoPublish(const QString& reason) {
    if (curWindow == nullptr || fastapi_project_id.isEmpty()) {
        return;
    }
    if (fastapiApplyingRemoteSnapshot || fastapiPublishingSnapshot) {
        return;
    }
    if (fastapiPublishTimer == nullptr) {
        return;
    }
    if (fastapiSubmitInFlight) {
        fastapiLocalDirtyDuringSubmit = true;
        fastapiLastPublishReason = reason;
        updateCollabPanelUi();
        return;
    }
    if (fastapi_pending_remote_version > fastapi_model_version) {
        statusBar()->showMessage(tr("Remote version pending; resolve it before submitting local changes."), 4000);
        updateCollabPanelUi();
        return;
    }

    fastapiLastPublishReason = reason;
    fastapiPublishTimer->start();
}

void MainWindow::publishFastAPIAutoSnapshot() {
    if (curWindow == nullptr || fastapi_project_id.isEmpty()) {
        return;
    }

    QAction* checkedAct = setModeActGroup ? setModeActGroup->checkedAction() : nullptr;
    if (checkedAct != setFASTAPIModeAct) {
        return;
    }

    publishFastAPIModelSnapshot(false);
}

bool MainWindow::exportCurrentModelToSat(QString* satContent, QString* errorMessage) {
    if (satContent == nullptr) {
        return false;
    }
    satContent->clear();

    ENTITY_LIST el;
    acis_get_noattrib_toplevel_active_entities(el);
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
    CollabSession::instance().onSubmitStarted();
    if (fastapiSyncSocket == nullptr || !fastapiSyncSocket->isValid()) {
        if (interactiveConflict) {
            QMessageBox::warning(this, tr("DBCAD"), tr("FastAPI WebSocket collaboration channel is not connected."));
        } else {
            statusBar()->showMessage(tr("Collaboration channel is not connected; local changes were not submitted."), 5000);
        }
        return false;
    }
    if (fastapiSubmitInFlight) {
        fastapiLocalDirtyDuringSubmit = true;
        return false;
    }
    if (fastapi_pending_remote_version > fastapi_model_version) {
        if (interactiveConflict) {
            QMessageBox::warning(this, tr("DBCAD"), tr("Remote version pending; resolve it before submitting local changes."));
        } else {
            statusBar()->showMessage(tr("Remote version pending; local changes were not submitted."), 5000);
        }
        updateCollabPanelUi();
        return false;
    }

    const QString author = QString::fromStdString(fastapi_author).trimmed();
    fastapiPendingSubmitRequestId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    fastapiSubmitInFlight = true;
    fastapiLocalDirtyDuringSubmit = false;

    QJsonObject content;
    content.insert("sat", satContent);

    QJsonObject payload;
    payload.insert("type", "submit_model");
    payload.insert("project_id", fastapi_project_id);
    payload.insert("request_id", fastapiPendingSubmitRequestId);
    payload.insert("author", author.isEmpty() ? QString::fromUtf8("dbcad-exe") : author);
    payload.insert("content", content);
    payload.insert("reason", reason.isEmpty() ? QString::fromUtf8("local-change") : reason);
    if (fastapi_model_version > 0) {
        payload.insert("base_version", fastapi_model_version);
    } else {
        payload.insert("base_version", QJsonValue::Null);
    }

    fastapiSyncSocket->sendTextMessage(QString::fromUtf8(QJsonDocument(payload).toJson(QJsonDocument::Compact)));
    statusBar()->showMessage(tr("Submitting collaborative snapshot..."), 1500);
    updateCollabPanelUi();
    return true;
}

bool MainWindow::publishFastAPIModelSnapshot(bool interactiveConflict) {
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

    CollabSession::instance().onHttpPublishStart();
    fastapiPublishingSnapshot = true;
    std::optional<int> baseVersion;
    if (fastapi_model_version > 0) {
        baseVersion = fastapi_model_version;
    }

    auto newVersion = client.saveModel(fastapi_project_id, satContent, baseVersion);
    fastapiPublishingSnapshot = false;
    CollabSession::instance().onHttpPublishEnd();

    if (!newVersion.has_value()) {
        if (client.lastStatusCode() == 409) {
            auto latest = client.getLatestModel(fastapi_project_id);
            if (latest.has_value()) {
                fastapi_pending_remote_version =
                    fastapi_pending_remote_version > latest->version ? fastapi_pending_remote_version : latest->version;
            }
        }
        if (interactiveConflict) {
            QMessageBox::warning(this, tr("DBCAD"), client.lastError());
        } else {
            statusBar()->showMessage(tr("自动发布协作版本失败：%1").arg(client.lastError()), 5000);
        }
        updateCollabPanelUi();
        return false;
    }

    fastapi_model_version = *newVersion;
    fastapi_pending_remote_version = 0;
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
    if (fastapi_pending_remote_version <= fastapi_model_version) {
        statusBar()->showMessage(tr("当前没有待同步版本"), 2000);
        return;
    }
    if (curWindow != nullptr && curWindow->getIsModified()) {
        QMessageBox::warning(this, tr("协作同步"), tr("当前有未保存修改，请先保存或放弃本地修改后再应用待同步版本。"));
        return;
    }
    syncFastAPIRemoteVersion(fastapi_pending_remote_version, tr("manual-apply"));
    updateCollabPanelUi();
}

bool MainWindow::syncFastAPIRemoteVersion(int remoteVersion, const QString& reason) {
    if (curWindow == nullptr || remoteVersion <= 0 || fastapi_project_id.isEmpty()) {
        return false;
    }

    BackendApiClient client(
        QString::fromStdString(fastapi_base_url),
        QString::fromStdString(fastapi_author),
        QString::fromStdString(fastapi_password));
    auto model = client.getModelVersion(fastapi_project_id, remoteVersion);
    if (!model.has_value()) {
        statusBar()->showMessage(tr("远程同步失败：%1").arg(client.lastError()), 5000);
        return false;
    }

    const GLWidget::ViewState viewState = curWindow->getViewState();
    curWindow->clear();
    if (!restoreFastAPIModelFromSat(model->sat)) {
        CollabSession::instance().onApplyEnd();
        return false;
    }
    curWindow->setViewState(viewState);
    CollabSession::instance().onApplyEnd();

    fastapi_model_version = model->version;
    fastapi_pending_remote_version = 0;
    setCurrentPartName(fastapi_project_name);
    curWindow->setIsModified(false);
    if (fastapiPublishTimer != nullptr) {
        fastapiPublishTimer->stop();
    }
    statusBar()->showMessage(tr("已同步远程版本%1（%2）").arg(fastapi_model_version).arg(reason), 4000);
    updateCollabPanelUi();
    return true;
}

bool MainWindow::applyFastAPIRemoteSat(int remoteVersion, const QString& satContent, const QString& reason) {
    if (curWindow == nullptr || remoteVersion <= 0 || fastapi_project_id.isEmpty() || satContent.isEmpty()) {
        return false;
    }

    if (fastapiPublishTimer != nullptr) {
        fastapiPublishTimer->stop();
    }

    const GLWidget::ViewState viewState = curWindow->getViewState();
    curWindow->clear();
    if (!restoreFastAPIModelFromSat(satContent)) {
        CollabSession::instance().onApplyEnd();
        return false;
    }
    curWindow->setViewState(viewState);
    CollabSession::instance().onApplyEnd();

    fastapi_model_version = remoteVersion;
    fastapi_pending_remote_version = 0;
    setCurrentPartName(fastapi_project_name);
    curWindow->setIsModified(false);
    statusBar()->showMessage(tr("已同步远程版本%1（%2）").arg(fastapi_model_version).arg(reason), 4000);
    updateCollabPanelUi();
    return true;
}

void MainWindow::reconnectFastAPISync() {
    CollabSession::instance().onReconnect(!fastapi_project_id.isEmpty());
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
        statusBar()->showMessage(tr("FastAPI实时同步已连接"), 2000);
        if (fastapiReconnectTimer != nullptr) {
            fastapiReconnectTimer->stop();
        }
        setCollabConnectionState(tr("已连接"));
        requestFastAPISyncNow();
        if (fastapiSyncTimer != nullptr) {
            fastapiSyncTimer->start();
        }
        if (fastapiHeartbeatTimer != nullptr) {
            fastapiHeartbeatTimer->start();
        }
        updateCollabPanelUi();
    });
    connect(fastapiSyncSocket, &QWebSocket::disconnected, this, [this]() {
        if (fastapiSyncTimer != nullptr) {
            fastapiSyncTimer->stop();
        }
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
    if (fastapi_project_id.isEmpty()) {
        return;
    }

    QAction* checkedAct = setModeActGroup ? setModeActGroup->checkedAction() : nullptr;
    if (checkedAct != setFASTAPIModeAct) {
        return;
    }

    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(message.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
        return;
    }

    const QJsonObject root = document.object();
    const QString messageType = root.value("type").toString();
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

    if (messageType == "submit_rejected") {
        const QString requestId = root.value("request_id").toString();
        if (!requestId.isEmpty() && requestId == fastapiPendingSubmitRequestId) {
            fastapiSubmitInFlight = false;
            fastapiLocalDirtyDuringSubmit = false;
            fastapiPendingSubmitRequestId.clear();
        }

        const int latestVersion = root.value("latest_version").toInt(0);
        if (latestVersion > fastapi_model_version) {
            fastapi_pending_remote_version =
                fastapi_pending_remote_version > latestVersion ? fastapi_pending_remote_version : latestVersion;
        }
        CollabSession::instance().onSubmitRejected();
        statusBar()->showMessage(tr("Collaborative submit was rejected: %1").arg(root.value("detail").toString()), 6000);
        updateCollabPanelUi();
        return;
    }

    if (messageType == "error") {
        statusBar()->showMessage(tr("Collaboration channel error: %1").arg(root.value("detail").toString()), 5000);
        return;
    }

    if (messageType != "model_saved") {
        return;
    }

    const QString projectId = root.value("project_id").toString();
    if (projectId != fastapi_project_id) {
        return;
    }

    const int remoteVersion = root.value("version").toInt(0);
    if (remoteVersion <= 0) {
        return;
    }
    const QString requestId = root.value("request_id").toString();
    const bool acceptedOwnSubmit = !requestId.isEmpty() && requestId == fastapiPendingSubmitRequestId;

    if (acceptedOwnSubmit) {
        fastapiSubmitInFlight = false;
        fastapiPendingSubmitRequestId.clear();
        fastapi_model_version = remoteVersion;
        fastapi_pending_remote_version = 0;
        if (!fastapiLocalDirtyDuringSubmit && curWindow != nullptr) {
            curWindow->setIsModified(false);
        }
        if (!fastapi_project_name.isEmpty()) {
            setCurrentPartName(fastapi_project_name);
        }
        const bool publishAgain = fastapiLocalDirtyDuringSubmit;
        fastapiLocalDirtyDuringSubmit = false;
        CollabSession::instance().onSubmitAccepted(remoteVersion);
        updateCollabPanelUi();
        statusBar()->showMessage(tr("Collaborative snapshot accepted as version %1").arg(fastapi_model_version), 2500);
        if (publishAgain) {
            scheduleFastAPIAutoPublish(tr("local-change-after-ack"));
        }
        return;
    }

    if (remoteVersion <= fastapi_model_version) {
        return;
    }

    const QJsonObject content = root.value("content").toObject();
    const QString satContent = content.value("sat").toString();

    if (curWindow == nullptr) {
        return;
    }

    if (fastapiSubmitInFlight || curWindow->getIsModified()) {
        fastapi_pending_remote_version =
            fastapi_pending_remote_version > remoteVersion ? fastapi_pending_remote_version : remoteVersion;
        CollabSession::instance().onRemotePending(remoteVersion);
        statusBar()->showMessage(
            tr("检测到远程新版本%1，当前有未保存修改，已进入待同步队列").arg(fastapi_pending_remote_version),
            5000);
        updateCollabPanelUi();
        return;
    }

    if (!fastapiAutoFollowRemote) {
        fastapi_pending_remote_version =
            fastapi_pending_remote_version > remoteVersion ? fastapi_pending_remote_version : remoteVersion;
        CollabSession::instance().onRemotePending(remoteVersion);
        statusBar()->showMessage(tr("检测到远程新版本%1，已等待你手动应用").arg(fastapi_pending_remote_version), 5000);
        updateCollabPanelUi();
        return;
    }

    const QString reason = root.value("trigger").toString("broadcast");
    if (!satContent.isEmpty()) {
        applyFastAPIRemoteSat(remoteVersion, satContent, reason);
    } else {
        syncFastAPIRemoteVersion(remoteVersion, reason);
    }
    CollabSession::instance().onRemoteApplied(remoteVersion);
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
    QAction* collabSyncNowAct = collabMenu->addAction(tr("立即同步最新版本"), this, &MainWindow::requestFastAPISyncNow);
    collabSyncNowAct->setStatusTip(tr("主动请求服务器返回最新版本并同步。"));
    QAction* collabApplyPendingAct = collabMenu->addAction(tr("应用待同步版本"), this, &MainWindow::applyPendingRemoteVersion);
    collabApplyPendingAct->setStatusTip(tr("当本地有未保存修改导致挂起时，手动应用待同步版本。"));
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
    collabSyncNowButton = new QPushButton(tr("立即同步"), collabBody);
    collabReconnectButton = new QPushButton(tr("重连"), collabBody);

    connect(collabAutoFollowCheckBox, &QCheckBox::toggled, this, [this](bool checked) {
        fastapiAutoFollowRemote = checked;
        if (checked && fastapi_pending_remote_version > fastapi_model_version && curWindow != nullptr && !curWindow->getIsModified()) {
            applyPendingRemoteVersion();
        }
        updateCollabPanelUi();
    });
    connect(collabSyncNowButton, &QPushButton::clicked, this, &MainWindow::requestFastAPISyncNow);
    connect(collabReconnectButton, &QPushButton::clicked, this, &MainWindow::reconnectFastAPISync);

    collabLayout->addWidget(new QLabel(tr("连接状态"), collabBody));
    collabLayout->addWidget(collabConnectionLabel);
    collabLayout->addWidget(collabProjectLabel);
    collabLayout->addWidget(collabVersionLabel);
    collabLayout->addWidget(collabPendingLabel);
    collabLayout->addWidget(new QLabel(tr("在线协作者"), collabBody));
    collabLayout->addWidget(collabMembersList);
    collabLayout->addWidget(collabAutoFollowCheckBox);
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
                        fastapi_model_version = 0;
                        reconnectFastAPISync();
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
                        fastapi_model_version = 0;
                        reconnectFastAPISync();
                        setCurrentPartName(fileName);
                        statusBar()->showMessage(tr("已加入协作项目（当前无版本，请先保存模型）"), 4000);
                        isRead = true;
                    } else {
                        QMessageBox::warning(this, tr("DBCAD"), client.lastError());
                    }
                } else if (restoreFastAPIModelFromSat(model->sat)) {
                    fastapi_project_id = project->id;
                    fastapi_project_name = fileName;
                    fastapi_model_version = model->version;
                    reconnectFastAPISync();
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
            fastapi_model_version = 0;
            setCurrentFile(fileName);
            statusBar()->showMessage(tr("文件已导入"), 2000);
        } else if (checkedAct == setNEO4JModeAct) {
            disconnectFastAPISync();
            fastapi_project_id.clear();
            fastapi_project_name.clear();
            fastapi_model_version = 0;
            fastapi_pending_remote_version = 0;
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
                    fastapi_model_version = model->version;
                    reconnectFastAPISync();
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
            statusBar()->showMessage(tr("零件已导入(FastAPI)，版本%1").arg(fastapi_model_version), 3000);
        } else {
            disconnectFastAPISync();
            fastapi_project_id.clear();
            fastapi_project_name.clear();
            fastapi_model_version = 0;
            fastapi_pending_remote_version = 0;
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

                            std::optional<int> baseVersion;
                            if (fastapi_project_id == project->id && fastapi_model_version > 0) {
                                baseVersion = fastapi_model_version;
                            }

                            auto newVersion = client.saveModel(project->id, satContent, baseVersion);
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
                                                fastapi_model_version = latest->version;
                                                fastapi_pending_remote_version = 0;
                                                reconnectFastAPISync();
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
                                                fastapi_model_version = *retriedVersion;
                                                fastapi_pending_remote_version = 0;
                                                reconnectFastAPISync();
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
                                                    fastapi_model_version = *forkVersion;
                                                    fastapi_pending_remote_version = 0;
                                                    reconnectFastAPISync();
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
                                fastapi_model_version = *newVersion;
                                fastapi_pending_remote_version = 0;
                                reconnectFastAPISync();
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
        fastapi_model_version = 0;
        setCurrentFile(fileName);
        statusBar()->showMessage(tr("文件已保存"), 2000);
    } else if (checkedAct == setFASTAPIModeAct) {
        setCurrentPartName(fileName);
        statusBar()->showMessage(tr("零件已保存(FastAPI)，版本%1").arg(fastapi_model_version), 3000);
        if (fastapiAutoFollowRemote && fastapi_pending_remote_version > fastapi_model_version && !curWindow->getIsModified()) {
            applyPendingRemoteVersion();
        }
    } else if (checkedAct == setNEO4JModeAct || checkedAct == setNEO4JIncrementalModeAct) {
        disconnectFastAPISync();
        fastapi_project_id.clear();
        fastapi_project_name.clear();
        fastapi_model_version = 0;
        fastapi_pending_remote_version = 0;
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
