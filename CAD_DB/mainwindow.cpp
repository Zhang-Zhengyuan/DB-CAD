#include "mainwindow_internal.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags) {
    setObjectName("MainWindow");
    setWindowTitle("DBCAD");
    curWindow = nullptr;
    fastapi_client_id = QUuid::createUuid().toString(QUuid::WithoutBraces);


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
        QAction *checkedAct = setModeActGroup ? setModeActGroup->checkedAction() : nullptr;
        if (checkedAct == setFASTAPIModeAct && !fastapi_project_id.isEmpty()) {
            reconnectFastAPISync();
        } else {
            fastapiReconnectTimer->stop();
        }
    });


    // PostgreSQL service — initialized once, reuses connection across all operations.
    // Note: PgService::loaded is intentionally NOT connected here.
    // `pgLoadFromDatabaseToCurrent()` uses a SingleShotConnection lambda to route the
    // SAT text back into the calling flow; connecting a second handler here would
    // double-render. `loadFailed` IS connected permanently as a safety net.
    m_pgService = std::make_unique<PgService>();
    connect(m_pgService.get(), &PgService::initFailed, this,
            [](const QString &err) { QMessageBox::warning(nullptr, tr("PostgreSQL 初始化失败"), err); });
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

    HISTORY_STREAM *new_hs = new HISTORY_STREAM();
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

    }

    mg_init();

    // 彻底重置历史系统
    HISTORY_STREAM *hs = get_default_stream(false);
    if (hs) {
        // 删除所有 delta states，但保留流对象
        delete_all_delta_states(hs, true); // true 表示保留流
        hs->clear();                       // 清空流内容
    } else {
        hs = get_default_stream(true); // 创建新流
    }
    // 重置全局历史管理器状态
    initialize_delta_states();
    // 确保根状态为空（便于后续记录）
    DELTA_STATE *root_state = nullptr;
    api_ensure_empty_root_state(hs, root_state);

    CollabSession::instance().bindLegacyFields(
        fastapi_project_id, fastapi_model_version, fastapi_pending_remote_version, fastapiLastPublishReason,
        fastapiPendingSubmitRequestId, fastapiSubmitInFlight, fastapiLocalDirtyDuringSubmit,
        fastapiApplyingRemoteSnapshot, fastapiLocalDirty);
    qDebug().noquote() << "[CollabSession] bound:" << CollabSession::instance().dump(CollabSession::Event::Bound);
    CollabSession::instance().setDebugEnabled(true);
    CollabSession::instance().setMinDumpIntervalMs(0);
    CollabSession::instance().setEventMinInterval(CollabSession::Event::WsMessage, 200);

    // 永久启用实体变更追踪：每次 addEntity/removeEntity/modifyEntity 都会记录到 pendingEntityChanges，
    // 直到当前协作模式提交成功后才清空列表。
    // 这是 entity_graph 增量提交路径生效的前提。
    beginEntityChangeTracking();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (maybeSave()) {
        event->accept();
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
