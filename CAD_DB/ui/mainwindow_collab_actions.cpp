#include "mainwindow_internal.h"

// 协作动作（M2.1 异步）：按钮/菜单只启动操作并立即返回；
// 结果经 *Finished 信号回到 handle*Finished 显示。网络在飞期间界面保持可用，
// 由 busy 闸门（控制器）+ 面板按钮禁用（updateCollabPanelUi）防止并发操作。

void MainWindow::displayOutcome(const dbcad::CollabController::Outcome& outcome) {
    if (outcome.message.isEmpty())
        return;
    if (!outcome.ok && outcome.showDialog) {
        showErrorDialog(outcome.message);
    } else {
        showMessage(outcome.message, outcome.messageDuration);
    }
}

void MainWindow::showErrorDialog(const QString& message) {
    interaction_->showError(message);
}

void MainWindow::notifyModelChangedForCollaboration() {
    // 存取模式门控（后端选择）；应用远端/项目绑定等策略在 CollabController。
    if (!usingCollabStorage())
        return;
    collabController_->onLocalEdit();
}

void MainWindow::onCollabPushButtonClicked() {
    if (curWindow == nullptr)
        return;
    if (collabController_->isBusy()) {
        showMessage(collabController_->busyMessage(), 3000);
        return;
    }
    // push 不改画布（只清脏标记与待提交账）；完成回调里刷新面板即可。
    collabController_->pushAsync(tr("manual-push"));
}

void MainWindow::onCollabPullButtonClicked() {
    QString reason;
    if (!collabController_->canPullNow(&reason)) {
        showMessage(reason, 3000);
        return;
    }
    if (curWindow == nullptr)
        return;
    if (collabController_->isBusy()) {
        showMessage(collabController_->busyMessage(), 3000);
        return;
    }
    collabController_->pullAsync();
}

void MainWindow::requestFastAPISyncNow() {
    if (collabBackend_->channelReady()) {
        collabBackend_->sendSyncNow();
        statusBar()->showMessage(tr("已请求服务器返回最新版本"), 1500);
    }
}

void MainWindow::reconnectFastAPISync() {
    collabController_->connectProject();
}

// M4.3 定版：命名标签指向当前版本（对话框输入名称；结果经状态栏反馈）。
void MainWindow::onCollabTagVersionClicked() {
    if (!usingCollabStorage() || collabController_->projectId().isEmpty()) {
        showMessage(tr("协作通道未连接"), 4000);
        return;
    }
    bool ok = false;
    const QString name = QInputDialog::getText(this, tr("定版(Tag)"),
                                               tr("标签名（将指向当前版本 v%1）：").arg(collabController().modelVersion()),
                                               QLineEdit::Normal, QString(), &ok);
    if (!ok || name.trimmed().isEmpty())
        return;
    const QString projectId = collabController_->projectId();
    const int version = collabController_->modelVersion();
    collabBackend_->tagVersion(projectId, name.trimmed(), version, [this, name, version](bool ok, const QString& error) {
        if (ok)
            showMessage(tr("已定版：%1 → v%2").arg(name).arg(version), 4000);
        else
            showErrorDialog(error);
    });
}

// ---------- 异步完成处理 ----------

void MainWindow::handlePushFinished(const dbcad::CollabController::Outcome& outcome) {
    displayOutcome(outcome);
    if (outcome.ok)
        announceEditIntentEnds(); // 已提交的组件编辑结束
    updateCollabPanelUi();
}

void MainWindow::handlePullFinished(const dbcad::CollabController::Outcome& outcome) {
    if (curWindow != nullptr)
        curWindow->refreshAfterDocumentChange(outcome.retiredDisplayData);
    displayOutcome(outcome);
    if (outcome.ok)
        announceEditIntentEnds(); // 远端合并后重新声明意图
    updateCollabPanelUi();
}

void MainWindow::announceEditIntentEnds() {
    if (!usingCollabStorage()) {
        announcedIntents_.clear();
        documentSession_->forgetAllEntityEdits();
        return;
    }
    const QStringList ends = announcedIntents_.values();
    for (const QString& uuid : ends)
        collabBackend_->sendEditIntent(uuid, QStringLiteral("end"));
    announcedIntents_.clear();
    documentSession_->forgetAllEntityEdits();
}

// M4.2 组件级软锁提示：远端意图只用于 UI 提示；提交仍走 base_version 乐观锁。
void MainWindow::handleEditIntentMessage(const QJsonObject& root) {
    const QString componentUuid = root.value("component_uuid").toString();
    const QString author = root.value("author").toString();
    const QString action = root.value("action").toString();
    if (componentUuid.isEmpty())
        return;
    if (action == QStringLiteral("begin")) {
        remoteEditIntents_.insert(componentUuid, author);
        const QString name = [this, componentUuid]() {
            for (const auto& item : documentSession_->entities())
                if (QString::fromStdString(item.uuid) == componentUuid)
                    return QString::fromStdString(item.name);
            return QString::fromUtf8("新组件");
        }();
        statusBar()->showMessage(tr("%1 正在编辑「%2」").arg(author, name), 3000);
        for (const auto& change : documentSession_->pendingChanges())
            if (change.uuid == componentUuid) {
                statusBar()->showMessage(tr("注意：%1 也在编辑该组件，推送时可能冲突").arg(author), 5000);
                break;
            }
    } else if (action == QStringLiteral("end")) {
        remoteEditIntents_.remove(componentUuid);
    }
}

void MainWindow::handleOpenProjectFinished(const dbcad::CollabController::Outcome& outcome) {
#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
    if (curWindow != nullptr)
        curWindow->refreshAfterDocumentChange(outcome.retiredDisplayData);
    if (outcome.ok) {
        curWindow->resetDocumentHistory();
        curWindow->setIsModified(false);
        setCurrentPartName(pendingOpenName_);
        statusBar()->showMessage(tr("零件已导入(FastAPI)，版本%1").arg(collabController_->modelVersion()), 3000);
    } else {
        displayOutcome(outcome);
    }
    updateCollabPanelUi();
}

void MainWindow::handleOpenProjectAtFinished(const dbcad::CollabController::Outcome& outcome) {
#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
    if (curWindow != nullptr)
        curWindow->refreshAfterDocumentChange(outcome.retiredDisplayData);
    if (outcome.ok) {
        curWindow->resetDocumentHistory();
        curWindow->setIsModified(false);
        setCurrentPartName(pendingOpenName_);
        statusBar()->showMessage(tr("零件已导入(FastAPI)，版本%1").arg(collabController_->modelVersion()), 3000);
    } else {
        displayOutcome(outcome);
    }
    updateCollabPanelUi();
}

void MainWindow::handleSaveToProjectFinished(const dbcad::CollabController::Outcome& outcome) {
    displayOutcome(outcome);
    if (outcome.ok)
        setCurrentPartName(pendingSaveName_);
    updateCollabPanelUi();
}

// 协作友好的本地删除：会话级联删除（内核删除 + 依赖清理 + REMOVE 记账）+
// 视图刷新。仅记待提交账，不自动 Push；与添加实体同一行为契约（待 Push 按钮）。
void MainWindow::deleteEntityByIndexForCollaboration(int index) {
    if (curWindow == nullptr)
        return;
    if (documentSession_->itemByIndex(index) == nullptr) {
        statusBar()->showMessage(tr("未找到该实体"), 3000);
        return;
    }

    const auto cascade = documentSession_->removeEntityCascade(index);
    if (!cascade.ok)
        return; // 内核删除失败：错误消息已经由会话消息通道上抛
    for (auto removed : cascade.removedIndices)
        curWindow->removeFromSelectedEntities(removed);
    curWindow->currentEntityIndex = -1;
    curWindow->setCurrentEntity({});
    curWindow->captureHistory();

    // 刷新 UI（tree widget 必须重绘，仅 updateMeshData 不够）
    curWindow->updateTreeWidget();
    curWindow->updateMeshData();
    if (curWindow->getEntityTree().empty()) {
        curWindow->glWidget->clear();
    } else {
        curWindow->glWidget->update();
    }

    // 状态栏：仅记入待提交账，由用户点 Push 按钮统一提交（含 REMOVE）
    {
        int pendingRemoves = 0;
        for (const auto& change : documentSession_->pendingChanges()) {
            if (change.changeType == dbcad::EntityChangeType::REMOVE)
                ++pendingRemoves;
        }
        if (pendingRemoves > 0) {
            statusBar()->showMessage(
                tr("已删除 %1 个实体（待 Push，含 %2 个删除）").arg((int)cascade.removedIndices.size()).arg(pendingRemoves),
                3000);
        } else {
            statusBar()->showMessage(tr("已删除 %1 个实体（待 Push）").arg((int)cascade.removedIndices.size()), 3000);
        }
    }
    for (auto* display : cascade.retiredDisplayData) delete display;
    updateCollabPanelUi();
}

// WS 文本消息入口：异常守卫 + 模式门控 + 分发（原 handleFastAPISyncMessage）。
// 最外层 try/catch：WS 回调里任何 C++ 异常穿透 Qt event loop 会 std::terminate。
void MainWindow::handleSyncTextMessage(const QString& message) {
    try {
        if (collabController_->projectId().isEmpty()) {
            return;
        }
        if (!usingCollabStorage()) {
            return;
        }
        QJsonParseError parseError;
        const QJsonDocument document = QJsonDocument::fromJson(message.toUtf8(), &parseError);
        if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
            return;
        }
        const QJsonObject root = document.object();
        const QString messageType = root.value("type").toString();
        if (messageType == QStringLiteral("json_delta_saved")) {
            collabController_->onRemoteVersionNotified(root.value("version").toInt(0));
            return;
        }
        if (messageType == QStringLiteral("pong")) {
            return;
        }
        if (messageType == QStringLiteral("edit_intent")) {
            handleEditIntentMessage(root);
            return;
        }
        if (messageType == QStringLiteral("error")) {
            statusBar()->showMessage(tr("协作通道错误：%1").arg(root.value("detail").toString()), 5000);
            return;
        }
        // presence_* 家族：名单维护在后端。
        collabBackend_->processPresenceMessage(root, messageType);
    } catch (const std::exception& e) {
        qWarning().noquote() << "[Collab] handleSyncTextMessage threw:" << e.what();
        statusBar()->showMessage(tr("协作消息处理失败：%1").arg(QString::fromUtf8(e.what())), 5000);
    } catch (...) {
        qWarning().noquote() << "[Collab] handleSyncTextMessage threw unknown exception";
        statusBar()->showMessage(tr("协作消息处理失败（未知异常）"), 5000);
    }
}
