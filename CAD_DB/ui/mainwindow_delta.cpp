#include "mainwindow_internal.h"

bool MainWindow::applyRemoteIncrementalDelta(const QJsonObject &remoteContent, QString *errorMessage) {
    std::fprintf(stderr,
                 "[FINGERPRINT-2026-08-03] applyRemoteIncrementalDelta CALLED, this code includes the el-bypass fix\n");
    if (curWindow == nullptr) {
        if (errorMessage)
            *errorMessage = tr("当前窗口为空");
        return false;
    }

    qDebug().noquote() << "[Collab][Delta] >>> applyRemoteIncrementalDelta ENTER"
                       << " delta_bodies.size=" << (int)remoteContent.value("delta_bodies").toArray().size()
                       << " deleted_uuids.size=" << (int)remoteContent.value("deleted_uuids").toArray().size();
    CollabSession::instance().onApplyStart();

    // 1. 收集本地所有 body UUID
    const auto &localTree = curWindow->getEntityTree();
    QSet<QString> localUuids;
    for (const auto &eti : localTree) {
        if (!eti.uuid.empty()) {
            localUuids.insert(QString::fromStdString(eti.uuid));
        }
    }
    qDebug().noquote() << "[Collab][Delta]   localUuids count=" << localUuids.size()
                       << " entity_tree.size=" << localTree.size();
    for (int i = 0; i < localTree.size(); ++i) {
        qDebug().noquote() << "[Collab][Delta]   local[" << i << "] uuid="
                           << (localTree[i].uuid.empty() ? "(empty)"
                                                         : QString::fromStdString(localTree[i].uuid).left(8) + "...");
    }

    int appliedAdd = 0;
    int skippedAdd = 0;
    int appliedDelete = 0;
    int skippedDelete = 0;

    // 2. 处理增量 bodies（只 add 不在本地 UUID 集合中的）
    const QJsonArray deltaBodies = remoteContent["delta_bodies"].toArray();
    for (int bodyIdx = 0; bodyIdx < deltaBodies.size(); ++bodyIdx) {
        const QJsonValue &v = deltaBodies[bodyIdx];
        QJsonObject item = v.toObject();
        QString uuid = item["uuid"].toString();
        QString sat = item["sat"].toString();

        if (uuid.isEmpty() || sat.isEmpty()) {
            qDebug().noquote() << "[Collab][Delta]   body[" << bodyIdx << "] skip: uuid or sat empty";
            skippedAdd++;
            continue;
        }
        if (localUuids.contains(uuid)) {
            qDebug().noquote() << "[Collab][Delta]   body[" << bodyIdx << "] skip: already local";
            skippedAdd++;
            continue;
        }

        // acis_restore 该 body 并 addEntity。
        // 关键：Qt 的 QTemporaryFile 在 Windows 上是排他锁，所以必须 close() 释放锁
        // 再 fopen 同路径交给 ACIS 读；setAutoRemove(false) 是为了不在 close 时立刻删除。
        QTemporaryFile tempFile(QDir::tempPath() + "/dbcad_remote_delta_XXXXXX.sat");
        tempFile.setAutoRemove(false);
        if (!tempFile.open()) {
            qDebug().noquote() << "[Collab][Delta]   body[" << bodyIdx << "] FAIL: tempFile.open failed";
            skippedAdd++;
            continue;
        }
        QByteArray satBytes = sat.toUtf8();
        if (tempFile.write(satBytes) != satBytes.size()) {
            tempFile.close();
            QFile::remove(tempFile.fileName());
            qDebug().noquote() << "[Collab][Delta]   body[" << bodyIdx << "] FAIL: tempFile.write short";
            skippedAdd++;
            continue;
        }
        tempFile.flush();
        tempFile.close();

        std::string tempPath = tempFile.fileName().toStdString();
        FILE *f = std::fopen(tempPath.c_str(), "rb");
        if (f == nullptr) {
            QFile::remove(QString::fromStdString(tempPath));
            qDebug().noquote() << "[Collab][Delta]   body[" << bodyIdx << "] FAIL: fopen rb failed";
            skippedAdd++;
            continue;
        }
        qDebug().noquote() << "[Collab][Delta]   body[" << bodyIdx
                           << "] temp SAT ready, path=" << QString::fromStdString(tempPath)
                           << " bytes=" << satBytes.size();

        ENTITY_LIST el;
        bool restoreOk = false;
        std::string restoreErr;
        try {
            // 不要嵌套 API_NOP_BEGIN/END，wrapper 内部已 API_BEGIN/END；嵌套会让 ACIS
            // 状态机异常，导致 restore 完后 entity 不进 active list 也不进 el 内部数组。
            acis_restore_entity_list(el, f, 2, 0, 1); // text_mode=1 与 push 端 api_save_entity_list(f, true, el) 对称
            restoreOk = true;
        } catch (const std::exception &e) {
            restoreErr = e.what();
            restoreOk = false;
        } catch (...) {
            restoreErr = "unknown std exception";
            restoreOk = false;
        }
        std::fclose(f);
        QFile::remove(QString::fromStdString(tempPath));
        if (!restoreOk) {
            // 把 SAT 落到工作目录供排查，不删
            QString dumpPath = QDir::current().filePath("dbcad_failed_delta.sat");
            QFile::remove(dumpPath);
            QFile dump(dumpPath);
            if (dump.open(QIODevice::WriteOnly)) {
                dump.write(satBytes);
                dump.close();
                std::fprintf(stderr, "[Collab][Delta]   dumped failed SAT to: %s\n", dumpPath.toUtf8().constData());
            }
            std::fprintf(stderr, "[Collab][Delta]   body[%d] FAIL: restore threw, err=%s\n", bodyIdx,
                         restoreErr.c_str());
            skippedAdd++;
            continue;
        }

        ENTITY *foundBody = nullptr;
        if (el.count() > 0) {
            try {
                ENTITY *e = el[0];
                if (e != nullptr) {
                    foundBody = e;
                }
            } catch (...) {
                std::fprintf(stderr, "[Collab][Delta]   body[%d] el[0] access threw\n", bodyIdx);
            }
        }

        if (foundBody != nullptr) {
            int idxBeforeAdd = static_cast<int>(curWindow->getEntityTree().size());
            try {
                curWindow->addEntity(foundBody, tr("远端增量%1").arg(idxBeforeAdd).toStdString(), -1);
            } catch (const std::exception &e) {
                std::fprintf(stderr, "[Collab][Delta]   body[%d] addEntity threw std::exception: %s\n", bodyIdx,
                             e.what());
                skippedAdd++;
                continue;
            } catch (...) {
                std::fprintf(stderr, "[Collab][Delta]   body[%d] addEntity threw unknown\n", bodyIdx);
                skippedAdd++;
                continue;
            }
            // addEntity 会往 entity_tree 末尾添加一项（uuid 是新生成的），需要更正为远端 UUID
            int idxAfterAdd = static_cast<int>(curWindow->getEntityTree().size()) - 1;
            auto &etiList = curWindow->getEntityTree();
            if (idxAfterAdd >= 0 && idxAfterAdd < static_cast<int>(etiList.size())) {
                etiList[idxAfterAdd].uuid = uuid.toStdString();
                qDebug().noquote() << "[Collab][Delta]   body[" << bodyIdx << "] uuid updated:"
                                   << " local=" << QString::fromStdString(etiList[idxAfterAdd].uuid)
                                   << " remote=" << uuid;
            }
            entityIndexToUuid[etiList[idxAfterAdd].index] = uuid;
            appliedAdd++;
        } else {
            qDebug().noquote() << "[Collab][Delta]   body[" << bodyIdx << "] no entity found in el, skip";
            skippedAdd++;
        }
    }

    // 3. 处理删除列表（只删除本地有的）
    const QJsonArray deletedUuids = remoteContent["deleted_uuids"].toArray();
    qDebug().noquote() << "[Collab][Delta]   deleting uuids:" << deletedUuids;
    for (int delIdx = 0; delIdx < deletedUuids.size(); ++delIdx) {
        const QString uuid = deletedUuids[delIdx].toString();
        if (uuid.isEmpty()) {
            qDebug().noquote() << "[Collab][Delta]   del[" << delIdx << "] skip: uuid empty";
            skippedDelete++;
            continue;
        }
        if (!localUuids.contains(uuid)) {
            qDebug().noquote() << "[Collab][Delta]   del[" << delIdx << "] skip: uuid not found locally:" << uuid;
            skippedDelete++;
            continue;
        }

        // 在 entityTree 中找到该 uuid 对应的 entity 并真正删除（不仅是 visible=false）
        bool removed = false;
        auto &etiList = curWindow->getEntityTree();
        for (int i = static_cast<int>(etiList.size()) - 1; i >= 0; --i) {
            if (QString::fromStdString(etiList[i].uuid) == uuid) {
                ENTITY *entity = etiList[i].ptrEntity;
                int removedIdx = etiList[i].index;
                if (entity != nullptr) {
                    try {
                        API_NOP_BEGIN;
                        api_del_entity(entity);
                        API_NOP_END;
                    } catch (const std::exception &e) {
                        qWarning().noquote()
                            << "[Collab][Delta]   del[" << delIdx << "] api_del_entity threw:" << e.what();
                    } catch (...) {
                        qWarning().noquote() << "[Collab][Delta]   del[" << delIdx << "] api_del_entity threw unknown";
                    }
                }
                etiList.erase(etiList.begin() + i);
                entityIndexToUuid.remove(removedIdx);
                removed = true;
                break;
            }
        }
        if (removed) {
            appliedDelete++;
        } else {
            skippedDelete++;
        }
    }

    qDebug().noquote() << "[Collab][Delta] applyRemote: +" << appliedAdd << "/" << (appliedAdd + skippedAdd) << "  -"
                       << appliedDelete << "/" << (appliedDelete + skippedDelete)
                       << " (ENTER saw delta_bodies=" << deltaBodies.size() << "deleted=" << deletedUuids.size() << ")";
    CollabSession::instance().onApplyEnd();

    // 删除后刷新 mesh（新增的 addEntity 会自动触发 updateMeshData，无需单独处理）
    if (appliedDelete > 0) {
        curWindow->updateMeshData();
        curWindow->updateTreeWidget(); // 关键：删除后必须重绘 tree widget，否则画布删了但 tree 那行还在
        qDebug().noquote()
            << "[Collab][Delta]   afterDelete: updateMeshData + updateTreeWidget called, entityTree.size="
            << (int)curWindow->getEntityTree().size();
    } else if (appliedAdd > 0) {
        curWindow->updateTreeWidget(); // addEntity 路径同样需要 refresh tree widget
    }
    qDebug().noquote() << "[Collab][Delta] <<< applyRemoteIncrementalDelta EXIT appliedAdd=" << appliedAdd
                       << " appliedDelete=" << appliedDelete << " skippedAdd=" << skippedAdd
                       << " skippedDelete=" << skippedDelete;

    // 判定"应用结果"：
    //   - 真应用了任意 add/delete → true
    //   - payload 里声称有变化但全部因 UUID 已存在而被 skip → 也算 true（状态已一致）
    //   - payload 为空（caller 应已拦掉，正常不会到这里）→ true
    //   - curWindow 缺失（已在入口处返回 false） → 这里不可达
    // 调用方可以据此决定是否 fallback 到 SAT 全量。
    const int totalRequested = (int)deltaBodies.size() + (int)deletedUuids.size();
    if (totalRequested > 0 && appliedAdd == 0 && appliedDelete == 0 && skippedAdd == 0 && skippedDelete == 0) {
        // 罕见：声称有变更，但解析/反序列化全部失败（被 skipped 但仍是真错误）
        if (errorMessage) {
            *errorMessage = tr("远端增量反序列化全部失败（%1 项）").arg(totalRequested);
        }
        return false;
    }
    if (errorMessage && (appliedAdd == 0 && appliedDelete == 0 && (skippedAdd + skippedDelete) > 0)) {
        *errorMessage = tr("远端增量与本地无差异");
    }

    return true;
}
