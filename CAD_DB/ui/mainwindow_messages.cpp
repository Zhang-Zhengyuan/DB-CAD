#include "mainwindow_internal.h"

void MainWindow::handleFastAPISyncMessage(const QString &message) {
    qDebug().noquote() << CollabSession::instance().dump(CollabSession::Event::WsMessage);
    // 最外层 try/catch 守卫：WS 回调里任何 C++ 异常如果穿透到 Qt event loop，
    // 都会触发 std::terminate → abort()（参见 log_B.txt 中"打开文件失败"导致 B.exe crash 的根因）。
    try {
        handleFastAPISyncMessageImpl(message);
    } catch (const std::exception &e) {
        qWarning().noquote() << "[Collab] handleFastAPISyncMessage threw:" << e.what();
        statusBar()->showMessage(tr("协作消息处理失败：%1").arg(QString::fromUtf8(e.what())), 5000);
    } catch (...) {
        qWarning().noquote() << "[Collab] handleFastAPISyncMessage threw unknown exception";
        statusBar()->showMessage(tr("协作消息处理失败（未知异常）"), 5000);
    }
}

void MainWindow::handleFastAPISyncMessageImpl(const QString &message) {
    if (fastapi_project_id.isEmpty()) {
        return;
    }

    QAction *checkedAct = setModeActGroup ? setModeActGroup->checkedAction() : nullptr;
    if (checkedAct != setFASTAPIModeAct) {
        return;
    }

    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(message.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
        return;
    }

    const QJsonObject root = document.object();
    const QString messageType = root.value("type").toString();
    if (collabMode == CollabMode::Mode1 &&
        (messageType == QStringLiteral("model_saved") || messageType == QStringLiteral("entity_graph_saved") ||
         messageType == QStringLiteral("submit_delta")))
        return;
    if (collabMode == CollabMode::Mode0 && messageType == QStringLiteral("json_delta_saved"))
        return;
    qDebug().noquote() << "[Collab] handleFastAPISyncMessage type=" << messageType
                       << "hasTrigger=" << root.value("trigger").toString();
    // Every message has one owner. Stream filtering above remains the common gate.
    if (messageType == "submit_delta" || messageType == "json_delta_saved") {
        handleDeltaSyncEvent(root, messageType);
    } else if (messageType == "entity_graph_saved") {
        handleEntityGraphSyncEvent(root, messageType);
    } else if (messageType == "model_saved") {
        handleModelSyncEvent(root, messageType);
    } else {
        handlePresenceAndAckEvent(root, messageType);
    }
}
