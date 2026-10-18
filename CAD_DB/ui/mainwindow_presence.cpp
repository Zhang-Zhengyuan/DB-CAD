#include "mainwindow_internal.h"

// 协作面板与后端信号的视图反应：状态文案在这里，状态机/传输在 core。

void MainWindow::handleBackendCollaboratorsChanged() {
    if (!collabController_->projectId().isEmpty()) {
        statusBar()->showMessage(tr("当前在线协作者：%1").arg(collabBackend_->collaborators().size()), 2500);
    }
    updateCollabPanelUi();
}

void MainWindow::setCollabConnectionState(const QString& stateText) {
    if (collabConnectionLabel != nullptr) {
        collabConnectionLabel->setText(stateText);
    }
}

void MainWindow::handleBackendChannelState(const QString& stateKey) {
    if (stateKey == QStringLiteral("Connecting")) {
        setCollabConnectionState(tr("连接中"));
    } else if (stateKey == QStringLiteral("Connected")) {
        setCollabConnectionState(tr("已连接"));
    } else if (stateKey == QStringLiteral("Reconnecting")) {
        setCollabConnectionState(tr("已断开，重连中"));
    } else {
        setCollabConnectionState(tr("未连接"));
    }
    updateCollabPanelUi();
}

void MainWindow::handleBackendChannelConnected() {
    statusBar()->showMessage(tr("FastAPI实时同步已连接"), 2000);
    collabController_->onChannelConnected();
    updateCollabPanelUi();
}

void MainWindow::handleBackendChannelDisconnected() {
    if (!collabController_->projectId().isEmpty()) {
        statusBar()->showMessage(tr("FastAPI实时同步已断开"), 2000);
    }
    updateCollabPanelUi();
}

void MainWindow::handleBackendChannelError(const QString& message) {
    statusBar()->showMessage(tr("WebSocket连接失败: %1").arg(message), 5000);
}

void MainWindow::updateCollabPanelUi() {
    const auto& controller = *collabController_;
    if (collabProjectLabel != nullptr) {
        collabProjectLabel->setText(
            controller.projectName().isEmpty() ? tr("项目：未打开") : tr("项目：%1").arg(controller.projectName()));
    }
    if (collabVersionLabel != nullptr) {
        collabVersionLabel->setText(tr("本地版本：%1").arg(controller.modelVersion()));
    }
    if (collabPendingLabel != nullptr) {
        if (controller.isSubmitInFlight()) {
            collabPendingLabel->setText(tr("Submit: waiting for server"));
        } else if (controller.pendingRemoteVersion() > controller.modelVersion()) {
            collabPendingLabel->setText(tr("待同步版本：%1").arg(controller.pendingRemoteVersion()));
        } else {
            collabPendingLabel->setText(tr("待同步版本：无"));
        }
    }
    if (collabMembersList != nullptr) {
        collabMembersList->clear();
        const auto collaborators = collabBackend_->collaborators();
        for (auto it = collaborators.constBegin(); it != collaborators.constEnd(); ++it) {
            const QString author = it.value().trimmed().isEmpty() ? tr("unknown") : it.value();
            collabMembersList->addItem(author + " (" + it.key() + ")");
        }
    }

    // 启用/禁用协作控件：需要连接到项目；操作在飞或提交中时禁用。
    // （M2.1：取代整窗 setEnabled(false)，其余界面保持可用。）
    const bool connected = !controller.projectId().isEmpty() && collabBackend_->isConfigured();
    const bool busy = controller.isBusy() || controller.isSubmitInFlight();

    if (collabPushButton != nullptr) {
        collabPushButton->setEnabled(connected && !busy && controller.isConnected());
    }
    if (collabPullButton != nullptr) {
        collabPullButton->setEnabled(connected && !busy && controller.isConnected());
    }
    if (collabSyncNowButton != nullptr) {
        collabSyncNowButton->setEnabled(connected && !busy);
    }
    if (collabReconnectButton != nullptr) {
        collabReconnectButton->setEnabled(connected && !controller.isBusy());
    }
}
