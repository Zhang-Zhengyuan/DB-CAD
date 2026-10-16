#include "mainwindow_internal.h"

void MainWindow::requestFastAPISyncNow() {
    if (fastapiSyncSocket != nullptr && fastapiSyncSocket->isValid()) {
        fastapiSyncSocket->sendTextMessage("sync_now");
        statusBar()->showMessage(tr("已请求服务器返回最新版本"), 1500);
    }
}

void MainWindow::notifyModelChangedForCollaboration() {
    auto &session = CollabSession::instance();
    if (session.isApplyingRemoteSnapshot()) {
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

void MainWindow::onCollabPushButtonClicked() {
    submitMode1JsonDelta(tr("manual-push"));
}

void MainWindow::onCollabPullButtonClicked() {
    if (CollabSession::instance().isSubmitInFlight()) {
        statusBar()->showMessage(tr("正在提交，请稍后拉取"), 3000);
        return;
    }
    pullMode1JsonDelta();
}
