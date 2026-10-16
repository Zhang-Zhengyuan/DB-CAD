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
        FILE *f = nullptr;
        f = fopen(fileName.toStdString().c_str(), "wb");
        if (f) {
            API_NOP_BEGIN;
            api_save_version(2, 0);
            FileInfo fileinfo;
            fileinfo.set_units(1.0);
            fileinfo.set_product_id("demo_qt");
            result = api_set_file_info((FileIdent | FileUnits), fileinfo);
            result = api_set_int_option("sequence_save_files", 1);
            ENTITY_LIST el;
            acis_get_noattrib_toplevel_active_entities(el);
            result = api_save_entity_list(f, true, el);
            API_NOP_END;
            fclose(f);
        } else {
            errorMessage = tr("无法以写入方式打开文件%1。").arg(QDir::toNativeSeparators(fileName));
        }

    } else if (checkedAct == setNEO4JModeAct) {
        try {
            Neo4jPart f(neo4jdb_host.c_str(), neo4jdb_port_bolt, neo4jdb_username.c_str(), neo4jdb_password.c_str(),
                        fileName.toStdString());
            ENTITY_LIST el;
            acis_get_noattrib_toplevel_active_entities(el);
            api_save_entity_list_neo4j_part(f, el);
        } catch (const std::exception &ex) {
            errorMessage = tr("保存到neo4j失败：%1").arg(QString::fromUtf8(ex.what()));
        }
    } else if (checkedAct == setNEO4JIncrementalModeAct) {
        try {
            Neo4jPart f(neo4jdb_host.c_str(), neo4jdb_port_bolt, neo4jdb_username.c_str(), neo4jdb_password.c_str(),
                        fileName.toStdString());
            IncrementalContext incCtx_incrementalSave2;
            api_save_neo4j(f, incCtx_incrementalSave2);
        } catch (const std::exception &ex) {
            errorMessage = tr("保存到neo4j失败：%1").arg(QString::fromUtf8(ex.what()));
        }
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
    } else if (checkedAct == setNEO4JModeAct || checkedAct == setNEO4JIncrementalModeAct) {
        disconnectFastAPISync();
        CollabSession::instance().setProjectId(QString());
        fastapi_project_name.clear();
        fastapi_model_version = CollabSession::instance().modelVersion();
        fastapi_pending_remote_version = CollabSession::instance().pendingRemoteVersion();
        setCurrentPartName(fileName);
        statusBar()->showMessage(tr("零件已保存(neo4j)"), 2000);
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
