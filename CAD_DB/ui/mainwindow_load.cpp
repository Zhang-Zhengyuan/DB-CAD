#include "mainwindow_internal.h"

void MainWindow::loadFile(const QString &fileName) {
    if (curWindow == nullptr)
        addWindow();

    curWindow->clear();
    pendingEntityChanges.clear();
    entityIndexToUuid.clear();

    bool isRead = false;

#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    QAction *checkedAct = setModeActGroup->checkedAction();

    if (checkedAct == setACISModeAct) {
        FILE *f = nullptr;
        f = fopen(fileName.toStdString().c_str(), "r");
        if (f) {
            ENTITY_LIST el;

            API_BEGIN;
            api_save_version(2, 0);
            result = api_restore_entity_list(f, true, el);
            API_END;

            for (int i = 0; i < el.count(); i++)
                curWindow->addEntity(el[i], tr("导入(ACIS)实体%1").arg(i).toStdString(), -1);
            fclose(f);
            isRead = true;
        } else {
            QMessageBox::warning(this, tr("DBCAD"), tr("无法读取文件%1。").arg(QDir::toNativeSeparators(fileName)));
        }
    } else if (checkedAct == setNEO4JModeAct) {
        try {
            Neo4jPart f(neo4jdb_host.c_str(), neo4jdb_port_bolt, neo4jdb_username.c_str(), neo4jdb_password.c_str(),
                        fileName.toStdString());
            int64_t countn = count_partnode(f);
            if (countn == 1) {
                ENTITY_LIST el;
                API_BEGIN;
                api_restore_entity_list_neo4j_part(f, el);
                API_END;
                for (int i = 0; i < el.count(); i++)
                    curWindow->addEntity(el[i], tr("导入(neo4j)实体%1").arg(i).toStdString(), -1);
                isRead = true;
            } else if (countn == 0) {
                QMessageBox::warning(this, tr("DBCAD"), tr("名为%1的零件(neo4j)不存在。").arg(QString(fileName)));
            } else {
                assert(countn > 1);
                QMessageBox::warning(this, tr("DBCAD"), tr("名为%1的零件(neo4j)不唯一。").arg(QString(fileName)));
            }
        } catch (const std::exception &ex) {
            QMessageBox::warning(this, tr("DBCAD"), tr("连接neo4j失败：%1").arg(QString::fromUtf8(ex.what())));
        }
    } else if (checkedAct == setFASTAPIModeAct) {
        std::fprintf(stderr, "[DEBUG] loadFile: setFASTAPIModeAct, fileName =%s\n", fileName.toUtf8().constData());
        BackendApiClient client(QString::fromStdString(fastapi_base_url), QString::fromStdString(fastapi_author),
                                QString::fromStdString(fastapi_password));
        std::fprintf(stderr, "[DEBUG] loadFile: client.isConfigured() =%d baseUrl =%s\n", client.isConfigured() ? 1 : 0,
                     fastapi_base_url.c_str());
        if (!client.isConfigured()) {
            QMessageBox::warning(this, tr("DBCAD"), tr("FastAPI地址未配置，请先设置fastapi_connect_info.conf"));
        } else {
            auto project = client.getProjectByName(fileName);
            std::fprintf(stderr, "[DEBUG] loadFile: getProjectByName returned has_value=%d project_id='%s'\n",
                         project.has_value() ? 1 : 0, project.has_value() ? project->id.toUtf8().constData() : "null");
            if (!project.has_value()) {
                if (client.lastStatusCode() == 404) {
                    auto created = client.createProject(fileName);
                    if (!created.has_value()) {
                        QMessageBox::warning(this, tr("DBCAD"), tr("创建协作项目失败：%1").arg(client.lastError()));
                    } else {
                        fastapi_project_id = created->id;
                        fastapi_project_name = fileName;
                        CollabSession::instance().setProjectId(created->id);
                        reconnectFastAPISync();
                        CollabSession::instance().setModelVersion(0);
                        setCurrentPartName(fileName);
                        statusBar()->showMessage(tr("已创建并加入协作项目（当前无版本，请先保存模型）"), 4000);
                        isRead = true;
                    }
                } else {
                    QString err = client.lastError();
                    if (err.isEmpty()) {
                        QMessageBox::warning(this, tr("DBCAD"), tr("查询协作项目失败"));
                    } else {
                        QMessageBox::warning(this, tr("DBCAD"), err);
                    }
                }
            } else {
                const auto jsonModel = client.getJsonDelta(project->id, 0);
                if (!jsonModel.has_value()) {
                    QMessageBox::warning(this, tr("DBCAD"), tr("读取 Mode1 JSON 项目失败：%1").arg(client.lastError()));
                } else {
                    fastapi_project_id = project->id;
                    fastapi_project_name = fileName;
                    auto &session = CollabSession::instance();
                    session.setProjectId(project->id);
                    session.setModelVersion(0);
                    bool restored = true;
                    if (jsonModel->version > 0) {
                        QJsonParseError graphError;
                        const QJsonDocument graphDoc =
                            QJsonDocument::fromJson(jsonModel->graphJson.toUtf8(), &graphError);
                        restored = graphError.error == QJsonParseError::NoError && graphDoc.isObject();
                        if (restored && !graphDoc.object().value("nodes").toArray().isEmpty()) {
                            restored = pullACISEntityGraph(jsonModel->version, graphDoc.object(),
                                                           jsonModel->delta, jsonModel->removedIds);
                        } else if (restored) {
                            // An intentionally empty latest snapshot is a valid project.
                            session.setModelVersion(jsonModel->version);
                        }
                    }
                    if (restored) {
                        session.setModelVersion(jsonModel->version);
                        reconnectFastAPISync();
                        isRead = true;
                    } else {
                        disconnectFastAPISync();
                        fastapi_project_id.clear();
                        fastapi_project_name.clear();
                        session.setProjectId(QString());
                        session.setModelVersion(0);
                        updateCollabPanelUi();
                        QMessageBox::warning(this, tr("DBCAD"), tr("Mode1 JSON 图无法重建，请检查序列化器兼容性"));
                    }
                }

            }
        }
    } else {
        throw std::runtime_error("不支持的加载模式");
    }

#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
    if (isRead) {
        curWindow->setIsModified(false);
        if (checkedAct == setACISModeAct) {
            disconnectFastAPISync();
            CollabSession::instance().setProjectId(QString());
            fastapi_project_name.clear();
            CollabSession::instance().setModelVersion(0);
            setCurrentFile(fileName);
            statusBar()->showMessage(tr("文件已导入"), 2000);
        } else if (checkedAct == setNEO4JModeAct) {
            disconnectFastAPISync();
            CollabSession::instance().setProjectId(QString());
            fastapi_project_name.clear();
            CollabSession::instance().setModelVersion(0);
            CollabSession::instance().setPendingRemoteVersion(0);
            setCurrentPartName(fileName);
            statusBar()->showMessage(tr("零件已导入(neo4j)"), 2000);
        } else if (checkedAct == setFASTAPIModeAct) {
            setCurrentPartName(fileName);
            statusBar()->showMessage(tr("零件已导入(FastAPI)，版本%1").arg(fastapi_model_version), 3000);
        } else {
            throw std::runtime_error("不支持的加载模式");
        }
    }
}

void MainWindow::loadFile(const QString &partName, const int generation) {
    if (curWindow == nullptr)
        addWindow();

    curWindow->clear();
    pendingEntityChanges.clear();
    entityIndexToUuid.clear();

    bool isRead = false;

#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    QAction *checkedAct = setModeActGroup->checkedAction();

    if (checkedAct == setNEO4JIncrementalModeAct) {
        try {
            Neo4jPart f(neo4jdb_host.c_str(), neo4jdb_port_bolt, neo4jdb_username.c_str(), neo4jdb_password.c_str(),
                        partName.toStdString());
            int64_t countn = count_partnode(f);
            if (countn == 1) {
                IncrementalContext incCtx_incrementalRestore;
                api_restore_neo4j(f, generation, incCtx_incrementalRestore);
                ENTITY_LIST el;
                acis_get_noattrib_toplevel_active_entities(el);
                for (int i = 0; i < el.count(); i++)
                    curWindow->addEntity(el[i], tr("导入(neo4j)实体%1").arg(i).toStdString(), -1);
                isRead = true;
            } else if (countn == 0) {
                QMessageBox::warning(this, tr("DBCAD"), tr("名为%1的零件(neo4j)不存在。").arg(QString(partName)));
            } else {
                assert(countn > 1);
                QMessageBox::warning(this, tr("DBCAD"), tr("名为%1的零件(neo4j)不唯一。").arg(QString(partName)));
            }
        } catch (const std::exception &ex) {
            QMessageBox::warning(this, tr("DBCAD"), tr("连接neo4j失败：%1").arg(QString::fromUtf8(ex.what())));
        }
    } else if (checkedAct == setFASTAPIModeAct) {
        BackendApiClient client(QString::fromStdString(fastapi_base_url), QString::fromStdString(fastapi_author),
                                QString::fromStdString(fastapi_password));
        if (!client.isConfigured()) {
            QMessageBox::warning(this, tr("DBCAD"), tr("FastAPI地址未配置，请先设置fastapi_connect_info.conf"));
        } else {
            auto project = client.getProjectByName(partName);
            if (!project.has_value()) {
                QString err = client.lastError();
                if (err.isEmpty()) {
                    QMessageBox::warning(this, tr("DBCAD"), tr("名为%1的项目(FastAPI)不存在。").arg(QString(partName)));
                } else {
                    QMessageBox::warning(this, tr("DBCAD"), err);
                }
            } else {
                auto model = client.getJsonVersion(project->id, generation);
                if (!model.has_value()) {
                    if (client.lastStatusCode() == 404) {
                        QMessageBox::warning(this, tr("DBCAD"), tr("协作项目存在，但指定版本不存在。"));
                    } else {
                        QMessageBox::warning(this, tr("DBCAD"), client.lastError());
                    }
                } else {
                    QJsonParseError parseError;
                    const auto doc = QJsonDocument::fromJson(model->graphJson.toUtf8(), &parseError);
                    fastapi_project_id = project->id;
                    if (parseError.error != QJsonParseError::NoError || !doc.isObject() ||
                        !pullACISEntityGraph(model->version, doc.object(), false)) {
                        QMessageBox::warning(this, tr("DBCAD"), tr("历史版本无法重建"));
                        fastapi_project_id.clear();
#ifndef QT_NO_CURSOR
                        QGuiApplication::restoreOverrideCursor();
#endif
                        return;
                    }
                    fastapi_project_id = project->id;
                    fastapi_project_name = partName;
                    CollabSession::instance().setProjectId(project->id);
                    reconnectFastAPISync();
                    CollabSession::instance().setModelVersion(model->version);
                    isRead = true;
                }
            }
        }
    } else {
        throw std::runtime_error("不支持的指定版本加载模式");
    }

#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
    if (isRead) {
        curWindow->setIsModified(false);
        setCurrentPartName(partName);
        if (checkedAct == setFASTAPIModeAct) {
            statusBar()->showMessage(tr("零件已导入(FastAPI)，版本%1").arg(CollabSession::instance().modelVersion()),
                                     3000);
        } else {
            disconnectFastAPISync();
            CollabSession::instance().setProjectId(QString());
            fastapi_project_name.clear();
            CollabSession::instance().setModelVersion(0);
            CollabSession::instance().setPendingRemoteVersion(0);
            statusBar()->showMessage(tr("零件已导入(neo4j)"), 2000);
        }
        updateCollabPanelUi();
    }
}
