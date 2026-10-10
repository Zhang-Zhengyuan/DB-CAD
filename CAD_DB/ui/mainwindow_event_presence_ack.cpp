#include "mainwindow_internal.h"

// Preserve state transitions and version alignment for this message family.
void MainWindow::handlePresenceAndAckEvent(const QJsonObject &root, const QString &messageType) {
    if (messageType == "presence_snapshot") {
        fastapi_collaborators.clear();
        const QJsonArray members = root.value("members").toArray();
        for (const auto &memberValue : members) {
            if (!memberValue.isObject()) {
                continue;
            }
            const QJsonObject member = memberValue.toObject();
            const QString memberClientId = member.value("client_id").toString();
            if (memberClientId.isEmpty()) {
                continue;
            }
            fastapi_collaborators.insert(memberClientId, member.value("author").toString());
        }
        updateFastAPICollaboratorsStatus();
        return;
    }

    if (messageType == "collaborator_joined") {
        const QString memberClientId = root.value("client_id").toString();
        if (!memberClientId.isEmpty()) {
            fastapi_collaborators.insert(memberClientId, root.value("author").toString());
            updateFastAPICollaboratorsStatus();
        }
        return;
    }

    if (messageType == "collaborator_left") {
        const QString memberClientId = root.value("client_id").toString();
        if (!memberClientId.isEmpty()) {
            fastapi_collaborators.remove(memberClientId);
            updateFastAPICollaboratorsStatus();
        }
        return;
    }

    if (messageType == "pong") {
        return;
    }

    if (messageType == "submit_accepted") {
        // Server 在 broadcast entity_graph_saved 时会 exclude_client_id=client_id，
        // 然后单独给提交者发 submit_accepted { request_id, version, version_model }。
        // client 必须处理这条消息，否则 SubmitInFlight 状态永远清不掉。
        auto &session = CollabSession::instance();
        const QString requestId = root.value("request_id").toString();
        const int acceptedVersion = root.value("version").toInt(0);

        if (requestId.isEmpty() || requestId == session.submitRequestId()) {
            // SAT fallback 路径（version_model="model"）：服务端 version 字段已经包含正确版本，
            // 直接用 acceptedVersion 更新版本，model_saved 广播走 isOwnSubmitBroadcast 路径。
            // entity_graph 路径（version_model="entity_graph"）：同理的，版本在 acceptedVersion 里。
            session.onSubmitAccepted(acceptedVersion);
            // 推进 delta 基准线，防止增量路径下次包含已提交的全量内容
            api_advance_delta_since(collabCtx);
            fastapi_model_version = session.modelVersion();
            fastapi_pending_remote_version = session.pendingRemoteVersion();
            pendingEntityChanges.clear();
            if (curWindow != nullptr) {
                curWindow->setIsModified(false);
            }
            if (!fastapi_project_name.isEmpty()) {
                setCurrentPartName(fastapi_project_name);
            }
            updateCollabPanelUi();
            statusBar()->showMessage(tr("协作快照已提交，版本 %1").arg(fastapi_model_version), 2500);
        }
        return;
    }

    if (messageType == "submit_rejected") {
        auto &session = CollabSession::instance();
        const QString requestId = root.value("request_id").toString();
        const int latestVersion = root.value("latest_version").toInt(0);

        if (!requestId.isEmpty() && requestId == session.submitRequestId()) {
            session.rollbackSubmit();
        }

        if (latestVersion > session.modelVersion()) {
            session.onSubmitRejected(latestVersion);
        } else {
            session.onSubmitRejected(session.modelVersion());
        }

        // 冲突自动处理：备份本地 SAT → Pull 最新版本 → 用户选择是否 merge 回来。
        // 这样 A 的修改不会在 Pull 时被 clear() 永久丢失。
        QString localSatBackup;
        if (curWindow != nullptr) {
            // 无论本地是否 dirty，都把当前 ACIS 模型内容备份出来。
            // isLocalDirtyDuringSubmit=true 说明提交之后用户又做了改动（不在 pendingEntityChanges 里），
            // 也应该一并备份，否则 Pull 后那些改动会丢失。
            ENTITY_LIST el;
            acis_get_noattrib_toplevel_active_entities(el);
            if (el.count() > 0) {
                exportCurrentModelToSat(&localSatBackup, nullptr);
            }
        }
        fastapiConflictLocalSatBackup = localSatBackup;
        const QString conflictDetail = root.value("detail").toString();
        const QString conflictSat = root.value("content").toObject().value("sat").toString();

        if (!conflictSat.isEmpty()) {
            statusBar()->showMessage(
                tr("协作提交被拒绝：%1（冲突版本 %2），正在拉取最新版本...").arg(conflictDetail).arg(latestVersion),
                6000);
        } else {
            statusBar()->showMessage(
                tr("协作提交被拒绝：%1（冲突版本 %2），正在拉取最新版本...").arg(conflictDetail).arg(latestVersion),
                6000);
        }
        updateCollabPanelUi();

        // 自动 Pull 最新版本。注意：Pull 应答到达后 applyRemoteSatSnapshot 会检查
        // fastapiConflictLocalSatBackup，如果有备份会在 clear() 后弹出合并对话框。
        if (fastapiSyncSocket != nullptr && fastapiSyncSocket->isValid()) {
            fastapiSyncSocket->sendTextMessage("sync_now");
        }
        return;
    }

    if (messageType == "error") {
        auto &session = CollabSession::instance();
        const QString requestId = root.value("request_id").toString();
        const QString detail = root.value("detail").toString();
        qWarning().noquote() << "[Collab] error message from server: requestId=" << requestId << " detail=" << detail;

        // 检查是否为自己的提交被服务器拒绝
        if (!requestId.isEmpty() && requestId == session.submitRequestId()) {
            qWarning().noquote() << "[Collab] submit rejected by server, rolling back submit state";
            session.rollbackSubmit();
            fastapi_model_version = session.modelVersion();
            fastapi_pending_remote_version = session.pendingRemoteVersion();
            updateCollabPanelUi();
            statusBar()->showMessage(tr("提交被服务器拒绝：%1").arg(detail), 5000);
            return;
        }

        statusBar()->showMessage(tr("协作通道错误：%1").arg(detail), 5000);
        return;
    }
}
