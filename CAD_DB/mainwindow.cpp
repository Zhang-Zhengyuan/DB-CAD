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


    try { dbcad::kernel(); } catch (const std::exception&) {
        QMessageBox::warning(this, tr("异常"), tr("几何内核初始化失败"));
        throw;
    }

    createActions();
    createStatusBar();

#ifndef QT_NO_SESSIONMANAGER
    connect(qApp, &QGuiApplication::commitDataRequest, this, &MainWindow::commitData);
#endif

    addWindow();

    // option_header* delete_forward_states_option = find_option("delete_forward_states");
    // delete_forward_states_option->push(FALSE);



    {
        auto fastapiValues = read_config_lines("fastapi_connect_info.conf", 3);
        if (!fastapiValues.empty()) {
            fastapi_base_url = fastapiValues[0];
            fastapi_author = fastapiValues[1];
            fastapi_password = fastapiValues[2];
        }

    }

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
        if(curWindow && !curWindow->close()) { event->ignore(); return; }
        event->accept();
        disconnectFastAPISync();
        dbcad::shutdownKernel();
    } else {
        event->ignore();
    }
}
