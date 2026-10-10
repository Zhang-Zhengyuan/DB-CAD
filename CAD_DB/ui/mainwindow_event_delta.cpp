#include "mainwindow_internal.h"

// Preserve state transitions and version alignment for this message family.
void MainWindow::handleDeltaSyncEvent(const QJsonObject &root, const QString &messageType) {
    if (messageType == "submit_delta") {
        const QJsonObject content = root.value("content").toObject();
        const int remoteVersion = root.value("version").toInt(0);
        const QString requestId = root.value("request_id").toString();
        const QString reason = root.value("trigger").toString("broadcast");
        const bool isPullResponse = (reason == QStringLiteral("sync_now"));

        qDebug().noquote() << "[Collab] submit_delta received: v=" << remoteVersion << "reason=" << reason
                           << "isPull=" << isPullResponse << "delta_bodies=" << content["delta_bodies"].toArray().size()
                           << "deleted=" << content["deleted_uuids"].toArray().size();

        // 自己发出的 submit_delta 的 submit_accepted 走 submitInFlight 路径，不在此处理
        auto &session = CollabSession::instance();
        if (!requestId.isEmpty() && requestId == session.submitRequestId()) {
            // 推进 delta 基准线，防止下次增量包含本次已提交的变更
            api_advance_delta_since(collabCtx);
            return;
        }

        // 仅在 Pull 应答时立即 apply；远端广播则 parking（git-like）
        if (!isPullResponse) {
            if (remoteVersion > session.modelVersion()) {
                session.onRemotePending(remoteVersion);
                fastapi_pending_remote_version = session.pendingRemoteVersion();
                statusBar()->showMessage(tr("远端有增量变更v%1，请点「拉取(Pull)」合并").arg(remoteVersion), 4000);
                updateCollabPanelUi();
            }
            return;
        }

        if (curWindow == nullptr || remoteVersion <= session.modelVersion()) {
            return;
        }

        // apply 增量 delta
        QString errorMsg;
        if (!applyRemoteIncrementalDelta(content, &errorMsg)) {
            qWarning().noquote() << "[Collab] submit_delta apply failed:" << errorMsg;
            statusBar()->showMessage(tr("增量合并失败：%1，fallback到SAT").arg(errorMsg), 5000);
            const QString satContent = content.value("sat").toString();
            if (!satContent.isEmpty()) {
                applyRemoteSatSnapshot(satContent, QString::fromUtf8("submit_delta-fallback"));
            }
        } else {
            session.onRemoteApplied(remoteVersion);
            fastapi_model_version = session.modelVersion();
            updateCollabPanelUi();
            statusBar()->showMessage(tr("增量合并完成，版本 v%1").arg(remoteVersion), 2500);
        }
        return;
    }

    // Bridge-free Mode1 JSON graph broadcast.  Keep the git-like behavior:
    // announce a remote commit, but never overwrite a dirty local canvas.
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
