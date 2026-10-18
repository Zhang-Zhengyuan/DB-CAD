#include "mainwindow_internal.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags) {
    setObjectName("MainWindow");
    setWindowTitle("DBCAD");
    curWindow = nullptr;

    // 核心库装配：内核 → 文档会话 → 协作后端/控制器 → 命令总线。
    try { dbcad::kernel(); } catch (const std::exception&) {
        QMessageBox::warning(this, tr("异常"), tr("几何内核初始化失败"));
        throw;
    }
    documentSession_ = std::make_unique<dbcad::DocumentSession>(dbcad::kernel());
    collabBackend_ = std::make_unique<dbcad::CollabBackend>(this);
    collabController_ = std::make_unique<dbcad::CollabController>(*documentSession_, *collabBackend_, this);
    satBackend_ = std::make_unique<dbcad::SatFileBackend>(dbcad::kernel());
    commandBus_ = std::make_unique<dbcad::CommandBus>();
    activeStorage_ = satBackend_.get();

    // 后端信号 → 视图反应（状态机转移在控制器，文案在这里）。
    connect(collabBackend_.get(), &dbcad::CollabBackend::textMessageReceived, this,
            &MainWindow::handleSyncTextMessage);
    connect(collabBackend_.get(), &dbcad::CollabBackend::channelStateChanged, this,
            &MainWindow::handleBackendChannelState);
    connect(collabBackend_.get(), &dbcad::CollabBackend::channelConnected, this,
            &MainWindow::handleBackendChannelConnected);
    connect(collabBackend_.get(), &dbcad::CollabBackend::channelDisconnected, this,
            &MainWindow::handleBackendChannelDisconnected);
    connect(collabBackend_.get(), &dbcad::CollabBackend::channelError, this,
            &MainWindow::handleBackendChannelError);
    connect(collabBackend_.get(), &dbcad::CollabBackend::collaboratorsChanged, this,
            &MainWindow::handleBackendCollaboratorsChanged);
    connect(collabController_.get(), &dbcad::CollabController::infoMessage, this,
            [this](const QString& message, int duration) { showMessage(message, duration); });
    connect(collabController_.get(), &dbcad::CollabController::stateChanged, this,
            &MainWindow::updateCollabPanelUi);
    // 重连策略 = 存取后端是协作后端且项目仍打开（原"模式为 FastAPI 且项目非空"）。
    collabBackend_->setReconnectPolicy([this]() { return usingCollabStorage() && !collabController_->projectId().isEmpty(); });

    createActions();
    createStatusBar();

#ifndef QT_NO_SESSIONMANAGER
    connect(qApp, &QGuiApplication::commitDataRequest, this, &MainWindow::commitData);
#endif

    addWindow();

    {
        auto fastapiValues = read_config_lines("fastapi_connect_info.conf", 3);
        if (!fastapiValues.empty()) {
            collabBackend_->configure(QString::fromStdString(fastapiValues[0]),
                                      QString::fromStdString(fastapiValues[1]),
                                      QString::fromStdString(fastapiValues[2]));
        }
    }

    qDebug().noquote() << "[CollabState] bound:" << collabController_->state().dump(dbcad::CollabState::Event::Bound);

    // 永久启用实体变更追踪：每次 addEntity/removeEntity/modifyEntity 都会记录，
    // 直到当前协作模式提交成功后才清空列表。这是 entity_graph 增量提交路径生效的前提。
    documentSession_->beginChangeTracking();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (maybeSave()) {
        if(curWindow && !curWindow->close()) { event->ignore(); return; }
        event->accept();
        collabController_->disconnectChannel();
        dbcad::shutdownKernel();
    } else {
        event->ignore();
    }
}
