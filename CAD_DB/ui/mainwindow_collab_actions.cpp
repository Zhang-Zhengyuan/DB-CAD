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

// ---------- 异步完成处理 ----------

void MainWindow::handlePushFinished(const dbcad::CollabController::Outcome& outcome) {
    displayOutcome(outcome);
    updateCollabPanelUi();
}

void MainWindow::handlePullFinished(const dbcad::CollabController::Outcome& outcome) {
    if (curWindow != nullptr)
        curWindow->refreshAfterDocumentChange(outcome.retiredDisplayData);
    displayOutcome(outcome);
    updateCollabPanelUi();
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
