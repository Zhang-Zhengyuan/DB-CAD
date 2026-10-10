#include "mainwindow_internal.h"

void MainWindow::requestFastAPISyncNow() {
    if (fastapiSyncSocket != nullptr && fastapiSyncSocket->isValid()) {
        fastapiSyncSocket->sendTextMessage("sync_now");
        statusBar()->showMessage(tr("已请求服务器返回最新版本"), 1500);
    }
}

void MainWindow::notifyModelChangedForCollaboration() {
    auto &session = CollabSession::instance();
    if (session.isApplyingRemoteSnapshot() || session.isPublishingSnapshot()) {
        return;
    }

    QAction *checkedAct = setModeActGroup ? setModeActGroup->checkedAction() : nullptr;
    if (checkedAct != setFASTAPIModeAct) {
        return;
    }
    if (fastapi_project_id.isEmpty() || fastapi_project_name.isEmpty()) {
        return;
    }

    // Local edits only change collaboration state. The user explicitly pushes
    // through the collaboration panel, so this callback never starts a
    // network request from inside an ACIS edit operation.
    if (session.pendingRemoteVersion() > session.modelVersion()) {
        statusBar()->showMessage(tr("远端有未拉取版本，请先点击「拉取(Pull)」再推送本地修改"), 4000);
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

void MainWindow::scheduleFastAPIAutoPublish(const QString &reason) {
    // 已废弃：不再使用 900ms 防抖定时器自动发布，保留此函数仅为兼容既有调用点，
    // 内部直接转交给 publishFastAPIAutoSnapshot 即时执行（仅由 Push 按钮调用）。
    auto &session = CollabSession::instance();

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
    try {
        publishFastAPIAutoSnapshot();
    } catch (const std::exception &e) {
        qWarning().noquote() << "[Collab] publishFastAPIAutoSnapshot threw:" << e.what();
        statusBar()->showMessage(tr("发布快照失败：%1").arg(QString::fromUtf8(e.what())), 5000);
    } catch (...) {
        qWarning().noquote() << "[Collab] publishFastAPIAutoSnapshot threw unknown exception";
        statusBar()->showMessage(tr("发布快照失败（未知异常）"), 5000);
    }
}

void MainWindow::onCollabPushButtonClicked() {
    if (curWindow == nullptr || fastapi_project_id.isEmpty()) {
        statusBar()->showMessage(tr("未连接或未打开项目，无法推送"), 3000);
        return;
    }
    auto &session = CollabSession::instance();
    if (session.pendingRemoteVersion() > session.modelVersion()) {
        QMessageBox::warning(this, tr("协作推送"),
                             tr("远端有未拉取的版本（v%1 → v%2），请先点击「拉取(Pull)」再「推送(Push)」")
                                 .arg(session.modelVersion())
                                 .arg(session.pendingRemoteVersion()));
        return;
    }
    if (pendingEntityChanges.isEmpty()) {
        statusBar()->showMessage(tr("没有未推送的本地修改"), 2000);
        return;
    }

    // Mode1: 使用 HTTP delta API
    if (collabMode == CollabMode::Mode1) {
        if (submitMode1JsonDelta(tr("mode1-json-push"))) {
            pendingEntityChanges.clear();
            statusBar()->showMessage(tr("Mode1 Push 成功"), 3000);
        }
        return;
    }

    if (collabMode == CollabMode::Mode2) {
        statusBar()->showMessage(tr("Mode2 PostgreSQL 增量协作尚未实现"), 5000);
        return;
    }
    // Mode0: full SAT snapshot.
    scheduleFastAPIAutoPublish(tr("manual-push"));
}

void MainWindow::onCollabPullButtonClicked() {
    if (curWindow == nullptr || fastapi_project_id.isEmpty()) {
        statusBar()->showMessage(tr("未连接或未打开项目，无法拉取"), 3000);
        return;
    }
    auto &session = CollabSession::instance();
    if (session.isSubmitInFlight()) {
        statusBar()->showMessage(tr("本地正在提交，无法拉取，请稍后再试"), 3000);
        return;
    }

    // Mode1: 使用 HTTP delta API（独立 pull，不走 WebSocket）
    if (collabMode == CollabMode::Mode1) {
        pullMode1JsonDelta();
        return;
    }

    if (collabMode == CollabMode::Mode2) {
        statusBar()->showMessage(tr("Mode2 PostgreSQL 增量协作尚未实现"), 5000);
        return;
    }
    // Mode0: WebSocket sync_now 路径
    qDebug().noquote() << "[Collab] onCollabPullButtonClicked: requesting sync_now (Mode0)";
    requestFastAPISyncNow();
    statusBar()->showMessage(tr("已请求远端最新版本，请等待 entity_graph_saved 到达"), 3000);
}

void MainWindow::publishFastAPIAutoSnapshot() {
    if (curWindow == nullptr || fastapi_project_id.isEmpty()) {
        return;
    }

    QAction *checkedAct = setModeActGroup ? setModeActGroup->checkedAction() : nullptr;
    if (checkedAct != setFASTAPIModeAct) {
        return;
    }

    QString reason = fastapiLastPublishReason.isEmpty() ? QString::fromUtf8("local-change") : fastapiLastPublishReason;

    // Mode0 is the fixed full-SAT baseline, independent of Mode1 JSON.
    publishFastAPIModelSnapshot(false);
}
