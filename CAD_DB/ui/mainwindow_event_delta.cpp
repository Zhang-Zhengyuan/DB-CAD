#include "mainwindow_internal.h"

// Preserve state transitions and version alignment for this message family.
void MainWindow::handleDeltaSyncEvent(const QJsonObject &root, const QString &messageType) {
    if (messageType == "json_delta_saved") {
        const int remoteVersion = root.value("version").toInt(0);
        auto &session = CollabSession::instance();
        // Never apply a WebSocket notification. Pull fetches only the
        // changed topology components through the HTTP delta endpoint.
        if (remoteVersion > session.modelVersion()) {
            session.onRemotePending(remoteVersion);
            fastapi_pending_remote_version = session.pendingRemoteVersion();
            statusBar()->showMessage(tr("远端有 JSON 协作版本 v%1，请点击 Pull 合并").arg(remoteVersion), 5000);
            updateCollabPanelUi();
        }
        return;
    }

    // 支持增量实体图消息
}
