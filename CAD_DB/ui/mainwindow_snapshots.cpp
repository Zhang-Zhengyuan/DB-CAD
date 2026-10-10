#include "mainwindow_internal.h"

bool MainWindow::restoreFastAPIModelFromSat(const QString &satContent) {
    if (curWindow == nullptr) {
        addWindow();
    }

    QScopedValueRollback<bool> remoteSnapshotGuard(fastapiApplyingRemoteSnapshot, true);
    CollabSession::instance().onApplyStart();

    QTemporaryFile tempFile(QDir::tempPath() + "/dbcad_load_XXXXXX.sat");
    tempFile.setAutoRemove(true);
    if (!tempFile.open()) {
        QMessageBox::warning(this, tr("DBCAD"), tr("无法创建临时文件用于恢复FastAPI模型"));
        return false;
    }

    QByteArray satBytes = satContent.toUtf8();
    if (tempFile.write(satBytes) != satBytes.size()) {
        QMessageBox::warning(this, tr("DBCAD"), tr("写入临时SAT文件失败"));
        return false;
    }

    tempFile.flush();
    std::string tempPath = tempFile.fileName().toStdString();
    FILE *f = fopen(tempPath.c_str(), "r");
    if (!f) {
        QMessageBox::warning(this, tr("DBCAD"), tr("无法读取临时SAT文件"));
        return false;
    }

    ENTITY_LIST el;
    API_BEGIN;
    api_save_version(2, 0);
    result = api_restore_entity_list(f, true, el);
    API_END;
    fclose(f);

    qDebug().noquote() << "[Collab] restoreFastAPIModelFromSat: after api_restore_entity_list el.count()="
                       << el.count();

    if (el.count() == 0) {
        acis_get_noattrib_toplevel_active_entities(el);
    }

    if (el.count() == 0) {
        QMessageBox::warning(this, tr("DBCAD"),
                             tr("远程模型已恢复，但未检测到可渲染实体。请检查SAT内容或存储桥输出。"));
        return false;
    }

    for (int i = 0; i < el.count(); i++) {
        curWindow->addEntity(el[i], tr("导入(FastAPI)实体%1").arg(i).toStdString(), -1);
    }
    curWindow->updateMeshData();
    return true;
}

// Pull apply 入口：clear 本地画布 + 整个 SAT 文本替换。
// 跟 master 分支 applyFastAPIRemoteSat 完全等价（除了我们额外把版本号更新交给 caller）。
// 冲突合并：submit_rejected 时会备份本地 SAT 到 fastapiConflictLocalSatBackup，
// Pull 应答到达后，如果备份存在，在 restore 完成后弹出对话框让用户选择如何处理。
bool MainWindow::applyRemoteSatSnapshot(const QString &satContent, const QString &reason) {
    Q_UNUSED(reason);
    if (curWindow == nullptr) {
        return false;
    }
    // 保存视角，避免 apply 后镜头跳回原点。
    const GLWidget::ViewState viewState = curWindow->getViewState();
    curWindow->clear();

    // 在 restore 之前把 fastapiApplyingRemoteSnapshot 设为 true，这样 restore 过程中
    // 的一切 addEntity/removeEntity 都不会被记录到 pendingEntityChanges。
    // 注意：restoreFastAPIModelFromSat 内部也会设一次（QScopedValueRollback），双重保险。
    CollabSession::instance().onApplyStart();

    if (!restoreFastAPIModelFromSat(satContent)) {
        CollabSession::instance().onApplyEnd();
        return false;
    }
    CollabSession::instance().onApplyEnd();

    curWindow->setViewState(viewState);

    // 冲突合并：submit_rejected 时备份了本地 SAT，Pull 到达后 offer 给用户。
    if (!fastapiConflictLocalSatBackup.isEmpty() && !isShowingConflictMergeDialog) {
        isShowingConflictMergeDialog = true;
        QString backup = fastapiConflictLocalSatBackup;
        fastapiConflictLocalSatBackup.clear();

        int choice = QMessageBox::question(this, tr("冲突合并"),
                                           tr("您的本地修改与远端版本冲突（远端已被其他客户端更新）。\n\n"
                                              "是否将您本地的修改合并到最新版本？\n"
                                              "  • 选择「Yes」：将本地修改追加到远端最新版本\n"
                                              "  • 选择「No」：丢弃本地修改，只保留远端最新版本"),
                                           QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

        isShowingConflictMergeDialog = false;

        if (choice == QMessageBox::Yes) {
            // 把本地备份的 body 追加到 restore 后的画布中。
            // 此时 isTrackingEntityChanges=true 但 CollabSession::isApplyingRemoteSnapshot()=false
            // (apply 已结束)，所以 recordEntityAdded 会正常记录追加的 body 为新 ADD 变更。
            // 但不要在这里触发 scheduleFastAPIAutoPublish，留给用户下一步操作。
            if (!backup.isEmpty()) {
                // 用和 restoreFastAPIModelFromSat 一样的模式：保持 QTemporaryFile 在作用域内，
                // 避免 autoRemove 在 fopen 之前就把文件删了。
                QScopedPointer<QTemporaryFile> tf(
                    new QTemporaryFile(QDir::tempPath() + "/dbcad_merge_backup_XXXXXX.sat"));
                tf->setAutoRemove(true);
                if (tf->open()) {
                    QByteArray backupBytes = backup.toUtf8();
                    if (tf->write(backupBytes) == backupBytes.size()) {
                        tf->flush();
                        FILE *f = fopen(tf->fileName().toStdString().c_str(), "r");
                        if (f) {
                            ENTITY_LIST backupEl;
                            API_BEGIN;
                            api_save_version(2, 0);
                            result = api_restore_entity_list(f, true, backupEl);
                            API_END;
                            fclose(f);
                            if (backupEl.count() > 0) {
                                for (int i = 0; i < backupEl.count(); i++) {
                                    curWindow->addEntity(backupEl[i], tr("冲突合并(本地)实体%1").arg(i).toStdString(),
                                                         -1);
                                }
                                curWindow->updateMeshData();
                                qDebug().noquote() << "[Collab] Conflict merge: added" << backupEl.count()
                                                   << "backup bodies, total now:" << curWindow->getEntityTree().size();
                            }
                        }
                    }
                }
            }
            // 合并后的画布与远端版本不再一致，标记为 modified，这样用户下一步 Ctrl+S
            // 或 Push 时会把合并结果推上去。
            curWindow->setIsModified(true);
        } else {
            // 用户选择丢弃本地修改，保持 clear+restore 后的状态。
            curWindow->setIsModified(false);
        }
    }

    return true;
}

void MainWindow::disconnectFastAPISync() {
    std::fprintf(stderr, "[DEBUG] disconnectFastAPISync ENTER pid=%llu has_socket=%d base_url_len=%zu\n",
                 (unsigned long long)QCoreApplication::applicationPid(), fastapiSyncSocket != nullptr ? 1 : 0,
                 fastapi_base_url.size());
    CollabSession::instance().onDisconnected();
    if (fastapiSyncTimer != nullptr) {
        fastapiSyncTimer->stop();
    }
    if (fastapiHeartbeatTimer != nullptr) {
        fastapiHeartbeatTimer->stop();
    }
    if (fastapiPublishTimer != nullptr) {
        fastapiPublishTimer->stop();
    }
    fastapi_collaborators.clear();
    if (fastapiReconnectTimer != nullptr) {
        fastapiReconnectTimer->stop();
    }

    if (fastapiSyncSocket != nullptr) {
        std::fprintf(stderr, "[DEBUG] disconnectFastAPISync before socket.close base_url_len=%zu\n",
                     fastapi_base_url.size());
        fastapiSyncSocket->close();
        std::fprintf(stderr, "[DEBUG] disconnectFastAPISync after  socket.close base_url_len=%zu\n",
                     fastapi_base_url.size());
        fastapiSyncSocket->deleteLater();
        fastapiSyncSocket = nullptr;
    }
    std::fprintf(stderr, "[DEBUG] disconnectFastAPISync after  setCollabConnectionState base_url_len=%zu\n",
                 fastapi_base_url.size());
    updateCollabPanelUi();
    std::fprintf(stderr, "[DEBUG] disconnectFastAPISync after  updateCollabPanelUi base_url_len=%zu\n",
                 fastapi_base_url.size());
    std::fprintf(stderr, "[DEBUG] disconnectFastAPISync EXIT\n");
}
