#include "mainwindow_internal.h"

// 协作动作：按钮/菜单 → 控制器；WS 消息分发（模式门控在此，语义在 core）。

void MainWindow::displayOutcome(const dbcad::CollabController::Outcome& outcome) {
    if (outcome.message.isEmpty())
        return;
    if (!outcome.ok && outcome.showDialog) {
        QMessageBox::warning(this, tr("DBCAD"), outcome.message);
    } else {
        showMessage(outcome.message, outcome.messageDuration);
    }
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
    // push 不改画布（只清脏标记与待提交账），无需视图刷新。
    const bool wasEnabled = isEnabled();
    setEnabled(false);
    const auto outcome = collabController_->push(tr("manual-push"));
    setEnabled(wasEnabled);
    updateCollabPanelUi();
    displayOutcome(outcome);
}

void MainWindow::onCollabPullButtonClicked() {
    QString reason;
    if (!collabController_->canPullNow(&reason)) {
        showMessage(reason, 3000);
        return;
    }
    if (curWindow == nullptr)
        return;
    const bool wasEnabled = isEnabled();
    setEnabled(false);
    const auto outcome = collabController_->pull();
    setEnabled(wasEnabled);
    curWindow->refreshAfterDocumentChange(outcome.retiredDisplayData);
    displayOutcome(outcome);
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

// 协作友好的本地删除：会话级联删除（内核删除 + 依赖清理 + REMOVE 记账）+
// 视图刷新。仅记待提交账，不自动 Push；与添加实体同一行为契约（待 Push 按钮）。
void MainWindow::deleteEntityByIndexForCollaboration(int index) {
    qDebug().noquote() << "[Collab][Delta] deleteEntityByIndexForCollaboration ENTER index=" << index;
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
