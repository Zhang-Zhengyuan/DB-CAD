#include "mainwindow_internal.h"

// Preserve state transitions and version alignment for this message family.
void MainWindow::handleEntityGraphSyncEvent(const QJsonObject &root, const QString &messageType) {
    if (messageType == "entity_graph_saved") {
        const QString projectId = root.value("project_id").toString();
        if (projectId != fastapi_project_id) {
            return;
        }

        const QJsonObject content = root.value("content").toObject();
        const int remoteVersion = root.value("version").toInt(0);
        if (remoteVersion <= 0) {
            return;
        }

        qDebug().noquote() << "[Collab] handleFastAPISyncMessage: entity_graph_saved RECEIVED, version="
                           << remoteVersion << "trigger=" << root.value("trigger").toString();

        auto &session = CollabSession::instance();

        // Pull 应答 / 远端广播 entity_graph_saved：直接用 content.sat 整个替换本地画布。
        // 后端 entity_graph_saved 消息的 content 同时携带了 entity_graph/changes（git-like 增量元数据）
        // 以及 sat（旧项目历史版本可能包含完整 ACIS SAT 文本）。
        // 跟 master 分支的 applyFastAPIRemoteSat 完全一致：clear() → restore → addEntity，比对
        // entity_graph 增量合并简单无数倍，而且不出错。
        const QString satContent = content.value("sat").toString();
        const QString reason = root.value("trigger").toString("broadcast");
        const bool isPullResponse = (reason == QStringLiteral("sync_now"));
        qDebug().noquote() << "[Collab] entity_graph_saved v=" << remoteVersion << "trigger=" << reason
                           << "isPull=" << isPullResponse << "content.hasSat=" << (!satContent.isEmpty())
                           << "sat.size=" << satContent.size();

        // 仅在自己提交的请求应答时走 submit-accept 路径（保留原语义）。
        const QString requestId = root.value("request_id").toString();
        const bool acceptedOwnSubmit = !requestId.isEmpty() && requestId == session.submitRequestId();
        if (acceptedOwnSubmit) {
            session.onSubmitAccepted(remoteVersion);
            // 推进 delta 基准线，防止增量路径下次包含已提交的全量内容
            api_advance_delta_since(collabCtx);
            fastapi_model_version = session.modelVersion();
            fastapi_pending_remote_version = session.pendingRemoteVersion();
            if (!session.isLocalDirtyDuringSubmit() && curWindow != nullptr) {
                curWindow->setIsModified(false);
            }
            pendingEntityChanges.clear();
            if (!fastapi_project_name.isEmpty()) {
                setCurrentPartName(fastapi_project_name);
            }
            updateCollabPanelUi();
            statusBar()->showMessage(tr("增量实体图已提交，版本 %1").arg(fastapi_model_version), 2500);
            if (session.isLocalDirtyDuringSubmit()) {
                scheduleFastAPIAutoPublish(tr("local-change-after-ack"));
            }
            return;
        }

        if (remoteVersion <= session.modelVersion()) {
            return;
        }

        if (curWindow == nullptr) {
            return;
        }

        if (session.isSubmitInFlight()) {
            // 本地正在提交，不要在这条响应里 apply；放到 pending，等下一轮 Pull 再合并。
            session.onRemotePending(remoteVersion);
            fastapi_pending_remote_version = session.pendingRemoteVersion();
            statusBar()->showMessage(
                tr("检测到远程增量版本%1，当前正在提交中，已加入待同步队列").arg(session.pendingRemoteVersion()), 5000);
            updateCollabPanelUi();
            return;
        }

        // Git-like 语义：
        //   - 用户主动 Pull（trigger == "sync_now"）的应答才立即合并本地画布
        //   - 远端 broadcast（其他客户端提交触发）默认只更新 pendingRemoteVersion，
        //     提示用户"远端有新版本，请在协作面板点 Pull 合并"，保留本地编辑不被破坏。
        const bool shouldApply = isPullResponse;
        qDebug().noquote() << "[Collab] shouldApply decision: isPullResponse=" << isPullResponse << "trigger=" << reason
                           << "final=" << shouldApply;
        if (!shouldApply) {
            session.onRemotePending(remoteVersion);
            fastapi_pending_remote_version = session.pendingRemoteVersion();
            const QString tip =
                tr("远端版本%1已推送（git-like 语义），请在协作面板点「拉取(Pull)」合并").arg(remoteVersion);
            statusBar()->showMessage(tip, 5000);
            updateCollabPanelUi();
            return;
        }

        if (satContent.isEmpty()) {
            // sync_now 应答居然没 SAT，理论不应发生，回退到 pending 让用户重试
            session.onRemotePending(remoteVersion);
            fastapi_pending_remote_version = session.pendingRemoteVersion();
            statusBar()->showMessage(tr("远端版本%1的 Pull 应答缺少SAT文本，请刷新后重试").arg(remoteVersion), 5000);
            updateCollabPanelUi();
            return;
        }

        CollabSession::ApplyDecision applyDecision = session.tryBeginApplyRemote(remoteVersion, reason);
        qDebug().noquote() << "[Collab] Pull tryBeginApplyRemote kind=" << (int)applyDecision.kind
                           << "reason=" << applyDecision.reasonTr;
        if (applyDecision.kind != CollabSession::ApplyDecision::Allow) {
            qWarning() << "[Collab] Pull apply rejected:" << applyDecision.reasonTr
                       << "— version parked as pending, retry Pull later.";
            session.onRemotePending(remoteVersion);
            fastapi_pending_remote_version = session.pendingRemoteVersion();
            updateCollabPanelUi();
            return;
        }

        // 预取 delta 数据（供 IncrementalDelta 路径使用）
        const QJsonArray deltaBodiesPull = content.value("delta_bodies").toArray();
        const QJsonArray deletedUuidsPull = content.value("deleted_uuids").toArray();
        const QJsonObject entityGraphJson = content.value("entity_graph").toObject();
        qDebug().noquote() << "[Collab] Pull: chosen mode=" << static_cast<int>(collabPullMode)
                           << "sat.size=" << satContent.size() << "delta=" << (int)deltaBodiesPull.size()
                           << "deleted=" << (int)deletedUuidsPull.size()
                           << "entity_graph nodes=" << entityGraphJson.value("nodes").toArray().size();

        // === 按用户选择的模式分发 Pull 路径（不再有优先级链，互相独立）===
        // 模式说明：
        //   • IncrementalDelta（默认）：按 UUID 去重 add / 真正 delete，不破坏本地未推送变更，已验证稳定
        //   • FullSat：clear + restore 整段 SAT，全量替换本地画布（远端为准），丢弃本地未推送
        //   • EntityGraph：用 content.sat 单独 restore（与 delta 路径一样的 restore 方式，不调 clear），
        //                 用 entity_graph JSON 对齐 UUID，删除本地多余，已修复 clear 崩溃问题

        // IncrementalDelta 失败时需要 rollback tryBeginApplyRemote，再 fallback 到 FullSat
        auto rollbackToPending = [&](const QString &why) {
            session.rollbackApply();
            session.onRemotePending(remoteVersion);
            fastapi_pending_remote_version = session.pendingRemoteVersion();
            updateCollabPanelUi();
        };

        if (collabPullMode == CollabPullMode::IncrementalDelta) {
            if (deltaBodiesPull.isEmpty() && deletedUuidsPull.isEmpty()) {
                qWarning().noquote() << "[Collab] Pull IncrementalDelta: no delta in content, fallback to FullSat";
                // 不 rollback apply 标志，继续用 FullSat
                qDebug().noquote() << "[Collab] Pull [FullSat] sat.size=" << satContent.size();
                if (applyRemoteSatSnapshot(satContent, reason)) {
                    session.onRemoteApplied(remoteVersion);
                    fastapi_model_version = session.modelVersion();
                    fastapi_pending_remote_version = session.pendingRemoteVersion();
                    pendingEntityChanges.clear();
                    if (!fastapi_project_name.isEmpty()) {
                        setCurrentPartName(fastapi_project_name);
                    }
                    updateCollabPanelUi();
                    statusBar()->showMessage(tr("Pull 全量替换完成 v%1（SAT）").arg(fastapi_model_version), 3000);
                } else {
                    rollbackToPending(QStringLiteral("FullSat after IncrementalDelta fallback"));
                    statusBar()->showMessage(tr("Pull 全量替换失败，已重新排队"), 5000);
                }
                return;
            }
            qDebug().noquote() << "[Collab] Pull [IncrementalDelta] +" << deltaBodiesPull.size() << " -"
                               << deletedUuidsPull.size();
            QString deltaErr;
            if (applyRemoteIncrementalDelta(content, &deltaErr)) {
                session.onRemoteApplied(remoteVersion);
                fastapi_model_version = session.modelVersion();
                fastapi_pending_remote_version = session.pendingRemoteVersion();
                pendingEntityChanges.clear();
                if (!fastapi_project_name.isEmpty()) {
                    setCurrentPartName(fastapi_project_name);
                }
                updateCollabPanelUi();
                statusBar()->showMessage(tr("Pull 增量合并完成 v%1（+%2 -%3）")
                                             .arg(fastapi_model_version)
                                             .arg(deltaBodiesPull.size())
                                             .arg(deletedUuidsPull.size()),
                                         3000);
                return;
            }
            qWarning().noquote() << "[Collab] Pull [IncrementalDelta] failed:" << deltaErr << "→ fallback to FullSat";
            // IncrementalDelta 失败：rollback apply 标志（apply 内可能已部分修改 entity_tree，需要清理）
            rollbackToPending(deltaErr);
            statusBar()->showMessage(tr("Pull 增量合并失败，已重新排队"), 5000);
            return;
        }

        if (collabPullMode == CollabPullMode::FullSat) {
            qDebug().noquote() << "[Collab] Pull [FullSat] sat.size=" << satContent.size();
            if (applyRemoteSatSnapshot(satContent, reason)) {
                session.onRemoteApplied(remoteVersion);
                fastapi_model_version = session.modelVersion();
                fastapi_pending_remote_version = session.pendingRemoteVersion();
                pendingEntityChanges.clear();
                if (!fastapi_project_name.isEmpty()) {
                    setCurrentPartName(fastapi_project_name);
                }
                updateCollabPanelUi();
                statusBar()->showMessage(tr("Pull 全量替换完成 v%1（SAT）").arg(fastapi_model_version), 3000);
            } else {
                rollbackToPending(QStringLiteral("FullSat failed"));
                statusBar()->showMessage(tr("Pull 全量替换失败，已重新排队"), 5000);
                qWarning() << "[Collab] Pull [FullSat] applyRemoteSatSnapshot failed, parked as pending. v="
                           << remoteVersion;
            }
            return;
        }

        if (collabPullMode == CollabPullMode::EntityGraph) {
            qDebug().noquote() << "[Collab] Pull [EntityGraph] nodes="
                               << entityGraphJson.value("nodes").toArray().size();
            if (entityGraphJson.isEmpty() || entityGraphJson.value("nodes").toArray().isEmpty()) {
                rollbackToPending(QStringLiteral("EntityGraph empty in content"));
                statusBar()->showMessage(tr("Pull JSON 反序列化失败（无 Entity Graph 数据），已重新排队"), 5000);
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
                statusBar()->showMessage(tr("Pull JSON 反序列化完成 v%1").arg(fastapi_model_version), 3000);
            } else {
                rollbackToPending(QStringLiteral("EntityGraph path failed"));
                statusBar()->showMessage(tr("Pull JSON 反序列化失败，已重新排队"), 5000);
                qWarning() << "[Collab] Pull [EntityGraph] pullACISEntityGraph failed, parked as pending. v="
                           << remoteVersion;
            }
            return;
        }

        // 未识别的模式（不应该到这里，因为 enum 只有 3 个值）
        qWarning().noquote() << "[Collab] Pull: unknown collabPullMode=" << static_cast<int>(collabPullMode);
        rollbackToPending(QStringLiteral("unknown pull mode"));
    }
}
