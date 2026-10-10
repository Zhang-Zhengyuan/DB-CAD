#include "mainwindow_internal.h"

bool MainWindow::saveFile(const QString &fileName) {
    QString errorMessage;

    if (setModeActGroup->checkedAction() == setFASTAPIModeAct && collabMode == CollabMode::Mode2) {
        statusBar()->showMessage(tr("Mode2 PostgreSQL 增量协作尚未实现"), 5000);
        return false;
    }
    if (setModeActGroup->checkedAction() == setFASTAPIModeAct && collabMode == CollabMode::Mode1 &&
        !CollabSession::instance().isConnected()) {
        statusBar()->showMessage(tr("Mode1 JSON 保存需要先连接协作通道"), 5000);
        return false;
    }
    // In a live Mode1 session Ctrl+S must use the same JSON version stream as
    // Push. A SAT save would create an unrelated model version and mislead the
    // collaboration UI about what peers can Pull.
    if (setModeActGroup->checkedAction() == setFASTAPIModeAct && collabMode == CollabMode::Mode1 &&
        CollabSession::instance().isConnected()) {
        if (pendingEntityChanges.isEmpty()) {
            statusBar()->showMessage(tr("Mode1 没有待保存的本地修改"), 2500);
            return true;
        }
        return submitMode1JsonDelta(tr("mode1-json-save"));
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
    } else if (checkedAct == setFASTAPIModeAct) {
        BackendApiClient client(QString::fromStdString(fastapi_base_url), QString::fromStdString(fastapi_author),
                                QString::fromStdString(fastapi_password));
        if (!client.isConfigured()) {
            errorMessage = tr("FastAPI地址未配置，请先设置fastapi_connect_info.conf");
        } else {
            auto project = client.getOrCreateProject(fileName);
            if (!project.has_value()) {
                errorMessage = client.lastError();
            } else {
                QTemporaryFile tempFile(QDir::tempPath() + "/dbcad_save_XXXXXX.sat");
                tempFile.setAutoRemove(true);
                if (!tempFile.open()) {
                    errorMessage = tr("无法创建临时文件用于导出SAT");
                } else {
                    const QString tempPath = tempFile.fileName();
                    tempFile.close();

                    FILE *f = fopen(tempPath.toStdString().c_str(), "wb");
                    if (!f) {
                        errorMessage = tr("无法打开临时SAT文件用于写入");
                    } else {
                        API_NOP_BEGIN;
                        api_save_version(2, 0);
                        FileInfo fileinfo;
                        fileinfo.set_units(1.0);
                        fileinfo.set_product_id("dbcad_fastapi");
                        result = api_set_file_info((FileIdent | FileUnits), fileinfo);
                        result = api_set_int_option("sequence_save_files", 1);
                        ENTITY_LIST el;
                        acis_get_noattrib_toplevel_active_entities(el);
                        result = api_save_entity_list(f, true, el);
                        API_NOP_END;
                        fclose(f);

                        QFile satFile(tempPath);
                        if (!satFile.open(QIODevice::ReadOnly)) {
                            errorMessage = tr("读取临时SAT文件失败");
                        } else {
                            const QString satContent = QString::fromUtf8(satFile.readAll());
                            satFile.close();

                            auto &session = CollabSession::instance();
                            std::optional<int> baseVersion;
                            if (fastapi_project_id == project->id && session.modelVersion() > 0) {
                                baseVersion = session.modelVersion();
                            }

                            // Mode0 always persists a complete SAT snapshot.
                            std::optional<int> newVersion = client.saveModel(project->id, satContent, baseVersion);
                            if (!newVersion.has_value()) {
                                if (client.lastStatusCode() == 409) {
                                    QMessageBox msg(this);
                                    msg.setIcon(QMessageBox::Warning);
                                    msg.setWindowTitle(tr("协作冲突"));
                                    msg.setText(tr("检测到版本冲突：远程已有更新。请选择处理方式。"));
                                    QPushButton *syncRemoteBtn =
                                        msg.addButton(tr("同步远程最新"), QMessageBox::AcceptRole);
                                    QPushButton *forceSubmitBtn =
                                        msg.addButton(tr("提交我的版本（基于最新重试）"), QMessageBox::ActionRole);
                                    QPushButton *forkBtn =
                                        msg.addButton(tr("另存为新协作项目"), QMessageBox::DestructiveRole);
                                    QPushButton *cancelBtn = msg.addButton(QMessageBox::Cancel);
                                    Q_UNUSED(syncRemoteBtn);
                                    Q_UNUSED(forceSubmitBtn);
                                    Q_UNUSED(forkBtn);
                                    msg.exec();

                                    if (msg.clickedButton() == cancelBtn) {
                                        errorMessage = tr("已取消保存");
                                    } else if (msg.clickedButton() == syncRemoteBtn) {
                                        auto latest = client.getLatestModel(project->id);
                                        if (!latest.has_value()) {
                                            errorMessage = tr("获取远程最新版本失败：%1").arg(client.lastError());
                                        } else {
                                            const GLWidget::ViewState viewState = curWindow->getViewState();
                                            curWindow->clear();
                                            if (!restoreFastAPIModelFromSat(latest->sat)) {
                                                errorMessage = tr("同步远程最新版本失败");
                                            } else {
                                                curWindow->setViewState(viewState);
                                                fastapi_project_id = project->id;
                                                fastapi_project_name = fileName;
                                                session.setProjectId(project->id);
                                                reconnectFastAPISync();
                                                session.setModelVersion(latest->version);
                                                session.setPendingRemoteVersion(0);
                                                errorMessage.clear();
                                            }
                                        }
                                    } else if (msg.clickedButton() == forceSubmitBtn) {
                                        auto latest = client.getLatestModel(project->id);
                                        if (!latest.has_value()) {
                                            errorMessage = tr("获取远程最新版本失败：%1").arg(client.lastError());
                                        } else {
                                            auto retriedVersion =
                                                client.saveModel(project->id, satContent, latest->version);
                                            if (!retriedVersion.has_value()) {
                                                errorMessage =
                                                    tr("基于最新版本重试提交失败：%1").arg(client.lastError());
                                            } else {
                                                fastapi_project_id = project->id;
                                                fastapi_project_name = fileName;
                                                session.setProjectId(project->id);
                                                reconnectFastAPISync();
                                                session.setModelVersion(*retriedVersion);
                                                session.setPendingRemoteVersion(0);
                                                errorMessage.clear();
                                            }
                                        }
                                    } else {
                                        bool ok = false;
                                        QString newProjectName = QInputDialog::getText(
                                            this, tr("另存为新协作项目"), tr("请输入新项目名:"), QLineEdit::Normal,
                                            fileName + tr("-fork"), &ok,
                                            this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
                                        if (!ok || newProjectName.trimmed().isEmpty()) {
                                            errorMessage = tr("已取消另存为");
                                        } else {
                                            auto forkProject = client.getOrCreateProject(newProjectName.trimmed());
                                            if (!forkProject.has_value()) {
                                                errorMessage = tr("创建新协作项目失败：%1").arg(client.lastError());
                                            } else {
                                                auto forkVersion =
                                                    client.saveModel(forkProject->id, satContent, std::nullopt);
                                                if (!forkVersion.has_value()) {
                                                    errorMessage =
                                                        tr("保存到新协作项目失败：%1").arg(client.lastError());
                                                } else {
                                                    fastapi_project_id = forkProject->id;
                                                    fastapi_project_name = newProjectName.trimmed();
                                                    session.setProjectId(forkProject->id);
                                                    reconnectFastAPISync();
                                                    session.setModelVersion(*forkVersion);
                                                    session.setPendingRemoteVersion(0);
                                                    errorMessage.clear();
                                                }
                                            }
                                        }
                                    }
                                } else {
                                    errorMessage = client.lastError();
                                }
                            } else {
                                fastapi_project_id = project->id;
                                fastapi_project_name = fileName;
                                session.setProjectId(project->id);
                                reconnectFastAPISync();
                                session.setModelVersion(*newVersion);
                                session.setPendingRemoteVersion(0);
                            }
                        }
                    }
                }
            }
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
        fastapi_project_id.clear();
        fastapi_project_name.clear();
        // session 状态已在 disconnectFastAPISync() → onDisconnected() 中清零
        fastapi_model_version = CollabSession::instance().modelVersion();
        setCurrentFile(fileName);
        statusBar()->showMessage(tr("文件已保存"), 2000);
    } else if (checkedAct == setFASTAPIModeAct) {
        auto &session = CollabSession::instance();
        setCurrentPartName(fileName);
        statusBar()->showMessage(tr("零件已保存(FastAPI)，版本%1").arg(session.modelVersion()), 3000);
        if (fastapiAutoFollowRemote && session.pendingRemoteVersion() > session.modelVersion() &&
            !curWindow->getIsModified()) {
            applyPendingRemoteVersion();
        }
    } else if (checkedAct == setNEO4JModeAct || checkedAct == setNEO4JIncrementalModeAct) {
        disconnectFastAPISync();
        fastapi_project_id.clear();
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
