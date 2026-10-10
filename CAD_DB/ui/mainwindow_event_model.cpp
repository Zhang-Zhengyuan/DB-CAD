#include "mainwindow_internal.h"

// Preserve state transitions and version alignment for this message family.
void MainWindow::handleModelSyncEvent(const QJsonObject &root, const QString &messageType) {
    if (messageType != "model_saved") {
        return;
    }

    const QString projectId = root.value("project_id").toString();
    if (projectId != fastapi_project_id) {
        return;
    }

    const int remoteVersion = root.value("version").toInt(0);
    if (remoteVersion <= 0) {
        return;
    }

    auto &session = CollabSession::instance();
    const QString requestId = root.value("request_id").toString();
    const bool acceptedOwnSubmit = !requestId.isEmpty() && requestId == session.submitRequestId();
    // SAT fallback 路径：entity_graph ack 先到清空 submitRequestId，
    // 用 satSubmitInFlight 标记 SAT 提交在飞行中，用 fastapiLastAcceptedRequestId
    // 匹配 submit_accepted 留下的 requestId。
    // 注意：SAT 的 model_saved 广播不含 requestId，此时 submitRequestId 仍未被清空，
    // 所以不能用 requestId 匹配，必须用 satSubmitInFlight 来判断。
    const bool isOwnSubmitBroadcast = acceptedOwnSubmit || (session.isSatSubmitInFlight() && requestId.isEmpty()) ||
                                      (!requestId.isEmpty() && requestId == fastapiLastAcceptedRequestId);

    if (isOwnSubmitBroadcast) {
        // 本端提交的广播（被 exclude_client_id 排除后发给自己的）。
        // 状态转换（submitOk）和版本号在 submit_accepted 时已经处理完毕。
        // 这里只需要同步 UI 状态，不要再次调用 onSubmitAccepted——remoteVersion
        // 是旧版本号（0），会错误地覆盖已接受的版本。
        if (!session.isLocalDirtyDuringSubmit() && curWindow != nullptr) {
            curWindow->setIsModified(false);
        }
        if (!fastapi_project_name.isEmpty()) {
            setCurrentPartName(fastapi_project_name);
        }
        updateCollabPanelUi();
        statusBar()->showMessage(tr("Collaborative snapshot accepted as version %1").arg(fastapi_model_version), 2500);
        if (session.isLocalDirtyDuringSubmit()) {
            scheduleFastAPIAutoPublish(tr("local-change-after-ack"));
        }
        return;
    }

    if (remoteVersion <= session.modelVersion()) {
        return;
    }

    // model_saved 消息可能是：
    //   1. 来自 submitFastAPIModelOverSocket（SAT fallback，neo4j 存储失败时降级路径）
    //   2. 来自 submit_entity_graph 的存储路径（content 包含 entity_graph）
    // 只要带了 SAT 内容，就应该立即 apply，跟 entity_graph_saved 的 SAT fallback 完全一致。
    const QJsonObject contentJson = root.value("content").toObject();
    const QString satContent = contentJson.value("sat").toString();
    const QString reason = root.value("trigger").toString("model_saved");
    qDebug().noquote() << "[Collab] model_saved branch: chosen mode=" << static_cast<int>(collabPullMode)
                       << "hasSat=" << !satContent.isEmpty() << "sat.size=" << satContent.size() << "reason=" << reason
                       << "satSubmitInFlight=" << session.isSatSubmitInFlight();

    const QJsonObject entityGraphJson = contentJson.value("entity_graph").toObject();

    if (curWindow == nullptr) {
        return;
    }

    // === 按用户选择的模式分发（与 sync_now 应答一致的 dispatch 策略）===
    switch (collabPullMode) {
    case CollabPullMode::IncrementalDelta:
    default: {
        const QJsonArray deltaBodies = contentJson.value("delta_bodies").toArray();
        const QJsonArray deletedUuids = contentJson.value("deleted_uuids").toArray();
        if (deltaBodies.isEmpty() && deletedUuids.isEmpty()) {
            qWarning().noquote() << "[Collab] model_saved IncrementalDelta: no delta in content, fallback to FullSat";
            goto do_full_sat_model_saved;
        }
        CollabSession::ApplyDecision applyDecision = session.tryBeginApplyRemote(remoteVersion, reason);
        if (applyDecision.kind != CollabSession::ApplyDecision::Allow) {
            qWarning() << "[Collab] model_saved [IncrementalDelta] apply rejected:" << applyDecision.reasonTr;
            session.onRemotePending(remoteVersion);
            fastapi_pending_remote_version = session.pendingRemoteVersion();
            updateCollabPanelUi();
            return;
        }
        QString deltaErr;
        if (!applyRemoteIncrementalDelta(contentJson, &deltaErr)) {
            qWarning().noquote() << "[Collab] model_saved [IncrementalDelta] failed:" << deltaErr
                                 << "→ fallback to FullSat";
            goto do_full_sat_model_saved;
        }
        session.onRemoteApplied(remoteVersion);
        fastapi_model_version = session.modelVersion();
        fastapi_pending_remote_version = session.pendingRemoteVersion();
        pendingEntityChanges.clear();
        if (!fastapi_project_name.isEmpty()) {
            setCurrentPartName(fastapi_project_name);
        }
        updateCollabPanelUi();
        statusBar()->showMessage(tr("已通过增量合并拉取远端版本 %1（+%2 -%3）")
                                     .arg(fastapi_model_version)
                                     .arg(deltaBodies.size())
                                     .arg(deletedUuids.size()),
                                 3000);
        return;
    }

    case CollabPullMode::FullSat:
    do_full_sat_model_saved:
        if (satContent.isEmpty()) {
            session.onRemotePending(remoteVersion);
            fastapi_pending_remote_version = session.pendingRemoteVersion();
            statusBar()->showMessage(tr("远端版本%1不包含SAT文本，无法全量替换，请刷新后重试").arg(remoteVersion),
                                     5000);
            updateCollabPanelUi();
            return;
        }
        {
            CollabSession::ApplyDecision applyDecision = session.tryBeginApplyRemote(remoteVersion, reason);
            if (applyDecision.kind != CollabSession::ApplyDecision::Allow) {
                qWarning() << "[Collab] model_saved [FullSat] apply rejected:" << applyDecision.reasonTr;
                session.onRemotePending(remoteVersion);
                fastapi_pending_remote_version = session.pendingRemoteVersion();
                updateCollabPanelUi();
                return;
            }
            if (applyRemoteSatSnapshot(satContent, reason)) {
                session.onRemoteApplied(remoteVersion);
                fastapi_model_version = session.modelVersion();
                fastapi_pending_remote_version = session.pendingRemoteVersion();
                pendingEntityChanges.clear();
                if (!fastapi_project_name.isEmpty()) {
                    setCurrentPartName(fastapi_project_name);
                }
                updateCollabPanelUi();
                statusBar()->showMessage(tr("已通过全量替换拉取远端版本 %1").arg(fastapi_model_version), 3000);
            } else {
                session.rollbackApply();
                session.onRemotePending(remoteVersion);
                fastapi_pending_remote_version = session.pendingRemoteVersion();
                updateCollabPanelUi();
                statusBar()->showMessage(tr("全量替换失败，已重新排队"), 5000);
            }
        }
        return;

    case CollabPullMode::EntityGraph:
        if (entityGraphJson.isEmpty() || entityGraphJson.value("nodes").toArray().isEmpty()) {
            qWarning().noquote()
                << "[Collab] model_saved [EntityGraph] no entity_graph in content, fallback to FullSat";
            goto do_full_sat_model_saved;
        }
        {
            CollabSession::ApplyDecision applyDecision = session.tryBeginApplyRemote(remoteVersion, reason);
            if (applyDecision.kind != CollabSession::ApplyDecision::Allow) {
                qWarning() << "[Collab] model_saved [EntityGraph] apply rejected:" << applyDecision.reasonTr;
                session.onRemotePending(remoteVersion);
                fastapi_pending_remote_version = session.pendingRemoteVersion();
                updateCollabPanelUi();
                return;
            }
            if (pullACISEntityGraph(remoteVersion, entityGraphJson, satContent)) {
                session.onRemoteApplied(remoteVersion);
                fastapi_model_version = session.modelVersion();
                fastapi_pending_remote_version = session.pendingRemoteVersion();
                pendingEntityChanges.clear();
                if (!fastapi_project_name.isEmpty()) {
                    setCurrentPartName(fastapi_project_name);
                }
                updateCollabPanelUi();
                statusBar()->showMessage(tr("已通过 JSON 反序列化拉取远端版本 %1").arg(fastapi_model_version), 3000);
            } else {
                session.rollbackApply();
                session.onRemotePending(remoteVersion);
                fastapi_pending_remote_version = session.pendingRemoteVersion();
                updateCollabPanelUi();
                statusBar()->showMessage(tr("JSON 反序列化失败，已重新排队"), 5000);
            }
        }
        return;
    }
}
