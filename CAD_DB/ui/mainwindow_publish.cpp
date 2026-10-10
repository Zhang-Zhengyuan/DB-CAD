#include "mainwindow_internal.h"

bool MainWindow::submitFastAPIModelOverSocket(const QString &satContent, const QString &reason,
                                              bool interactiveConflict) {
    auto &session = CollabSession::instance();
    CollabSession::SubmitDecision decision = session.tryBeginSubmit(reason);

    if (decision.kind != CollabSession::SubmitDecision::Allow) {
        if (interactiveConflict) {
            QMessageBox::warning(this, tr("DBCAD"), decision.reason);
        } else {
            statusBar()->showMessage(decision.reason, 5000);
        }
        updateCollabPanelUi();
        return false;
    }

    // SAT fallback 路径与 entity_graph 路径共用同一个 submitInFlight 状态，
    // 但 entity_graph 的 submit_accepted 会先到达并清空它。用 satSubmitInFlight
    // 作为独立标记，确保 SAT 的 model_saved 广播能正确识别本端提交。
    session.setSatSubmitInFlight(true);

    if (fastapiSyncSocket == nullptr || !fastapiSyncSocket->isValid()) {
        session.rollbackSubmit();
        session.setSatSubmitInFlight(false);
        if (interactiveConflict) {
            QMessageBox::warning(this, tr("DBCAD"), tr("FastAPI WebSocket collaboration channel is not connected."));
        } else {
            statusBar()->showMessage(tr("Collaboration channel is not connected; local changes were not submitted."),
                                     5000);
        }
        return false;
    }

    const QString author = QString::fromStdString(fastapi_author).trimmed();
    const QString requestId = decision.requestId;

    QJsonObject content;
    content.insert("sat", satContent);

    QJsonObject payload;
    payload.insert("type", "submit_model");
    payload.insert("project_id", fastapi_project_id);
    payload.insert("request_id", requestId);
    payload.insert("author", author.isEmpty() ? QString::fromUtf8("dbcad-exe") : author);
    payload.insert("content", content);
    payload.insert("reason", reason.isEmpty() ? QString::fromUtf8("local-change") : reason);
    if (session.modelVersion() > 0) {
        payload.insert("base_version", session.modelVersion());
    } else {
        payload.insert("base_version", QJsonValue::Null);
    }

    fastapiSyncSocket->sendTextMessage(QString::fromUtf8(QJsonDocument(payload).toJson(QJsonDocument::Compact)));
    statusBar()->showMessage(tr("Submitting collaborative snapshot..."), 1500);
    updateCollabPanelUi();
    return true;
}

bool MainWindow::publishFastAPIModelSnapshot(bool interactiveConflict) {
    auto &session = CollabSession::instance();

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

    BackendApiClient client(QString::fromStdString(fastapi_base_url), QString::fromStdString(fastapi_author),
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

    CollabSession::PublishDecision pubDecision = session.tryBeginHttpPublish();
    if (pubDecision.kind != CollabSession::PublishDecision::Allow) {
        QMessageBox::warning(this, tr("DBCAD"), pubDecision.reason);
        return false;
    }

    std::optional<int> baseVersion;
    if (session.modelVersion() > 0) {
        baseVersion = session.modelVersion();
    }

    auto newVersion = client.saveModel(fastapi_project_id, satContent, baseVersion);

    if (!newVersion.has_value()) {
        session.rollbackHttpPublish();
        if (client.lastStatusCode() == 409) {
            auto latest = client.getLatestModel(fastapi_project_id);
            if (latest.has_value()) {
                session.onSubmitRejected(latest->version);
                QMessageBox::warning(this, tr("DBCAD"),
                                     tr("HTTP 发布冲突：远端版本 %1 已更新，请处理冲突后重试。").arg(latest->version));
            }
        } else {
            QMessageBox::warning(this, tr("DBCAD"), client.lastError());
        }
        updateCollabPanelUi();
        return false;
    }

    session.onHttpPublishEnd();
    fastapi_model_version = session.modelVersion();
    fastapi_pending_remote_version = session.pendingRemoteVersion();
    curWindow->setIsModified(false);
    if (!fastapi_project_name.isEmpty()) {
        setCurrentPartName(fastapi_project_name);
    }
    updateCollabPanelUi();
    statusBar()->showMessage(tr("已自动发布协作版本%1").arg(fastapi_model_version), 2000);
    return true;
}
