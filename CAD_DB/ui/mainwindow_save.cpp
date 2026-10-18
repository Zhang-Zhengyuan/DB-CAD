#include "mainwindow_internal.h"
#include <QEventLoop>
#include <QScopeGuard>

bool MainWindow::saveFile(const QString &fileName) {
    QString errorMessage;

    if (setModeActGroup->checkedAction() == setFASTAPIModeAct) {
        const bool wasEnabled = isEnabled();
        setEnabled(false);
        const auto restoreUi = qScopeGuard([this, wasEnabled]() { setEnabled(wasEnabled); });
        auto &session = CollabSession::instance();
        if (fileName != fastapi_project_name || fastapi_project_id.isEmpty()) {
            BackendApiClient client(QString::fromStdString(fastapi_base_url), QString::fromStdString(fastapi_author),
                                    QString::fromStdString(fastapi_password));
            const auto project = client.getOrCreateProject(fileName);
            if (!project) {
                statusBar()->showMessage(client.lastError(), 6000);
                return false;
            }
            const auto head = client.getJsonDelta(project->id, 0);
            if (!head) {
                statusBar()->showMessage(client.lastError(), 6000);
                return false;
            }
            if (head->version > 0) {
                statusBar()->showMessage(tr("目标项目已有版本，请先打开并 Pull，或使用新的项目名"), 6000);
                return false;
            }
            disconnectFastAPISync();
            fastapi_project_id = project->id;
            fastapi_project_name = fileName;
            session.setProjectId(project->id);
            session.setModelVersion(0);
            // A new project receives every local Body, including previously saved ones.
            pendingEntityChanges.clear();
            for (const auto &item : curWindow->getEntityTree()) {
                const QString uuid = QString::fromStdString(item.uuid);
                recordEntityAdded(uuid, QString::fromStdString(item.name), "body", item.index);
            }
            reconnectFastAPISync();
        }
        if (!session.isConnected() && fastapiSyncSocket) {
            QEventLoop wait;
            QTimer timeout;
            timeout.setSingleShot(true);
            connect(&timeout, &QTimer::timeout, &wait, &QEventLoop::quit);
            connect(fastapiSyncSocket, &QWebSocket::connected, &wait, &QEventLoop::quit);
            connect(fastapiSyncSocket, &QWebSocket::errorOccurred, &wait, &QEventLoop::quit);
            timeout.start(5000);
            wait.exec();
        }
        if (!session.isConnected()) {
            statusBar()->showMessage(tr("协作通道正在连接，请连接成功后再次保存"), 5000);
            return false;
        }
        const bool saved = submitMode1JsonDelta(tr("save"));
        if (saved)
            setCurrentPartName(fileName);
        return saved;
    }

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);

    QAction *checkedAct = setModeActGroup->checkedAction();

    if (checkedAct == setACISModeAct) {
        try {
            QByteArray bytes=dbcad::kernel().writeNative(curWindow->getEntityList());
            QFile file(fileName);
            if(!file.open(QIODevice::WriteOnly) || file.write(bytes)!=bytes.size()) errorMessage=tr("无法写入文件：%1").arg(fileName);
        } catch(const std::exception& e) { errorMessage=QString::fromUtf8(e.what()); }
    } else {
        throw std::runtime_error("Cannot find the enum");
    }

    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, tr("DBCAD"), errorMessage);
        return false;
    } else
        curWindow->setIsModified(false);

    if (checkedAct == setACISModeAct) {
        disconnectFastAPISync();
        CollabSession::instance().setProjectId(QString());
        fastapi_project_name.clear();
        // session 状态已在 disconnectFastAPISync() → onDisconnected() 中清零
        fastapi_model_version = CollabSession::instance().modelVersion();
        setCurrentFile(fileName);
        statusBar()->showMessage(tr("文件已保存"), 2000);
    } else {
        throw std::runtime_error("不支持的保存模式");
    }
    updateCollabPanelUi();
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName) {
    curWindow->setCurFile(fileName);
    curWindow->setWindowModified(false);
}

void MainWindow::setCurrentPartName(const QString &fileName) {
    curWindow->setCurPartName(fileName);
    curWindow->setWindowModified(false);
}

void MainWindow::showMessage(const QString s, int duration) {
    if (duration == -1)
        statusBar()->showMessage(s);
    else
        statusBar()->showMessage(s, duration);
}

#ifndef QT_NO_SESSIONMANAGER
void MainWindow::commitData(QSessionManager &manager) {
    if (manager.allowsInteraction()) {
        if (!maybeSave())
            manager.cancel();
    } else {
        // 自动保存
        if (curWindow != nullptr && curWindow->getIsModified())
            save();
    }
}
#endif
