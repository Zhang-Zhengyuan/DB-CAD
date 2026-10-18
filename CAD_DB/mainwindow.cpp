#include "mainwindow_internal.h"

namespace {
// Qt 默认交互面：真实对话框（无头/换 UI 时由调用方注入替代实现）。
class QtInteractionSurface : public InteractionSurface {
public:
    explicit QtInteractionSurface(MainWindow* window) : window(window) {}
    SaveDecision askSaveModified() override {
        const auto ret = QMessageBox::warning(window, QObject::tr("DBCAD"), QObject::tr("模型已修改。\n是否保存？"),
                                              QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        switch (ret) {
        case QMessageBox::Save:
            return SaveDecision::Save;
        case QMessageBox::Cancel:
            return SaveDecision::Cancel;
        default:
            return SaveDecision::Discard;
        }
    }
    void showError(const QString& message) override { QMessageBox::warning(window, QObject::tr("DBCAD"), message); }

private:
    MainWindow* window;
};
} // namespace

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
    defaultInteraction_ = std::make_unique<QtInteractionSurface>(this);
    interaction_ = defaultInteraction_.get();

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
    // M2.1 异步操作完成 → 视图收尾（显示结果、刷新画布/面板）。
    connect(collabController_.get(), &dbcad::CollabController::pushFinished, this,
            &MainWindow::handlePushFinished);
    connect(collabController_.get(), &dbcad::CollabController::pullFinished, this,
            &MainWindow::handlePullFinished);
    connect(collabController_.get(), &dbcad::CollabController::openProjectFinished, this,
            &MainWindow::handleOpenProjectFinished);
    connect(collabController_.get(), &dbcad::CollabController::openProjectAtFinished, this,
            &MainWindow::handleOpenProjectAtFinished);
    connect(collabController_.get(), &dbcad::CollabController::saveToProjectFinished, this,
            &MainWindow::handleSaveToProjectFinished);
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
    // M4.2 在场感知：本地首次编辑某组件时广播编辑意图（每 uuid 一次）。
    documentSession_->setEntityEditSink([this](const QString& uuid) {
        if (!usingCollabStorage() || announcedIntents_.contains(uuid))
            return;
        announcedIntents_.insert(uuid);
        collabBackend_->sendEditIntent(uuid, QStringLiteral("begin"));
    });
}

void MainWindow::setInteractionSurface(InteractionSurface* surface) {
    interaction_ = surface != nullptr ? surface : defaultInteraction_.get();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // 异步操作在飞时不退出：避免"保存已点但推送未完成"导致的数据丢失。
    if (collabController_->isBusy()) {
        showMessage(tr("协作操作进行中，请稍后再关闭"), 4000);
        event->ignore();
        return;
    }
    if (maybeSave()) {
        if(curWindow && !curWindow->close()) { event->ignore(); return; }
        event->accept();
        collabController_->disconnectChannel();
        dbcad::shutdownKernel();
    } else {
        event->ignore();
    }
}
