#include "mainwindow_internal.h"

bool MainWindow::pullACISEntityGraph(int version, const QJsonObject &entityGraphJson, const QString &satContent,
                                     bool deltaGraph, const QStringList &removedIds,
                                     const QHash<QString, QString> &remoteNames) {
    if (curWindow == nullptr || fastapi_project_id.isEmpty()) {
        qWarning() << "[Collab] pullACISEntityGraph: curWindow or fastapi_project_id is null";
        return false;
    }

    const QJsonArray nodes = entityGraphJson.value("nodes").toArray();
    qDebug().noquote() << "[Collab] pullACISEntityGraph: version=" << version << "nodes=" << nodes.size()
                       << "sat.size=" << satContent.size()
                       << "serializer_build=" << entityGraphJson.value("serializer_build").toString();

    // =========================================================================
    // 策略：优先尝试纯 JSON 反序列化（deserializeACISEntityGraph），
    //       完全不用 SAT，完整利用 entity_graph 的拓扑/几何数据。
    //       如果失败（不支持的几何类型等），fallback 到 SAT restore。
    // =========================================================================

    // 1. 收集本地现有 UUID（merge 基准线）
    fprintf(stderr, "[Collab] pullACISEntityGraph: stage 1 collecting local uuids, curWindow=%p\n", (void *)curWindow);
    fflush(stderr);
    QSet<QString> localUuids;
    QHash<QString, void *> localUuidToPtr;
    QHash<QString, QString> localUuidToName;
    const auto &localTree = curWindow->getEntityTree();
    fprintf(stderr, "[Collab] pullACISEntityGraph: stage 1 localTree.size()=%d\n", (int)localTree.size());
    fflush(stderr);
    for (const auto &eti : localTree) {
        if (!eti.uuid.empty()) {
            const QString u = QString::fromStdString(eti.uuid);
            localUuids.insert(u);
            localUuidToPtr.insert(u, eti.ptrEntity);
            localUuidToName.insert(u, QString::fromStdString(eti.name));
        }
    }
    fprintf(stderr, "[Collab] pullACISEntityGraph: stage 1 done, localUuids=%d\n", (int)localUuids.size());

    // 2. 标记正在应用远端快照（过滤 recordEntityRemoved）
    CollabSession::instance().setApplyingRemoteSnapshot(true);

    // 3. 优先：纯 JSON 反序列化（严格三阶段，见 entity_graph_serializer.cpp）
    DeserializedEntityGraph deserialized;
    deserialized.entities = nullptr;
    deserialized.uuidToEntity.clear();
    QString jsonError;
    // A delete-only commit has no incoming nodes. It is still a valid JSON
    // delta: skip ACIS reconstruction and let the explicit removed UUID list
    // drive the local deletion phase below.
    const bool emptyDelta = deltaGraph && nodes.isEmpty() && removedIds.isEmpty();
    const bool deleteOnlyDelta = deltaGraph && nodes.isEmpty() && !removedIds.isEmpty();
    bool fromJson =
        emptyDelta || deleteOnlyDelta || deserializeACISEntityGraph(entityGraphJson, &deserialized, &jsonError);

    ENTITY_LIST remoteBodies;
    QHash<QString, void *> remoteUuidToBody; // UUID → BODY*

    if (emptyDelta) {
        qDebug() << "[Collab] pullACISEntityGraph [JSON]: empty delta";
    } else if (deleteOnlyDelta) {
        qDebug() << "[Collab] pullACISEntityGraph [JSON]: delete-only delta" << removedIds;
    } else if (fromJson && deserialized.entities && deserialized.entities->count() > 0) {
        // deserializeACISEntityGraph 成功：body 节点 id = UUID（serialize 时直接设 node.id = uuid）
        fprintf(stderr, "[Collab] pullACISEntityGraph: STAGE A about to copy remoteBodies/remoteUuidToBody\n");
        remoteBodies = *deserialized.entities;
        fprintf(stderr, "[Collab] pullACISEntityGraph: STAGE A1 remoteBodies copied, count=%d\n",
                (int)remoteBodies.count());
        remoteUuidToBody = deserialized.uuidToEntity;
        fprintf(stderr, "[Collab] pullACISEntityGraph: STAGE A2 remoteUuidToBody copied, size=%d\n",
                (int)remoteUuidToBody.size());
        int bodyCount = (int)remoteBodies.count();
        int uuidCount = (int)remoteUuidToBody.size();
        fprintf(stderr, "[Collab] pullACISEntityGraph [JSON]: deserialized %d bodies, UUID map size=%d\n", bodyCount,
                uuidCount);
        fprintf(stderr, "[Collab] pullACISEntityGraph: about to enter merge loop, bodies=%d\n", bodyCount);
    } else {
        // JSON 反序列化失败，用 SAT restore
        qDebug().noquote() << "[Collab] pullACISEntityGraph [JSON] failed:" << jsonError
                           << "falling back to SAT restore";
        delete deserialized.entities;

        if (satContent.isEmpty()) {
            qWarning() << "[Collab] pullACISEntityGraph: both JSON failed and satContent empty";
            CollabSession::instance().setApplyingRemoteSnapshot(false);
            return false;
        }

        // SAT restore 并用 body 节点 id 对齐 UUID
        if (!restoreSatWithUuidAlignment(satContent, entityGraphJson, remoteBodies, remoteUuidToBody)) {
            qWarning() << "[Collab] pullACISEntityGraph: SAT restore failed";
            CollabSession::instance().setApplyingRemoteSnapshot(false);
            return false;
        }
    }

    fprintf(stderr, "[Collab] pullACISEntityGraph: about to construct remoteUuids QSet\n");
    fprintf(stderr, "[Collab] pullACISEntityGraph: remoteUuidToBody.size()=%d\n", (int)remoteUuidToBody.size());
    for (auto it = remoteUuidToBody.constBegin(); it != remoteUuidToBody.constEnd(); ++it) {
        fprintf(stderr, "[Collab] pullACISEntityGraph: remoteUuidToBody key=%s value=%p\n",
                it.key().toUtf8().constData(), it.value());
    }
    fprintf(stderr, "[Collab] pullACISEntityGraph: about to call keys()\n");
    QList<QString> keysList = remoteUuidToBody.keys();
    fprintf(stderr, "[Collab] pullACISEntityGraph: keys() returned size=%d\n", (int)keysList.size());
    QSet<QString> remoteUuids = QSet<QString>(keysList.begin(), keysList.end());
    fprintf(stderr, "[Collab] pullACISEntityGraph: remoteUuids QSet constructed ok, size=%d\n",
            (int)remoteUuids.size());
    int willAdd = 0, willUpdate = 0;
    for (auto it = remoteUuids.constBegin(); it != remoteUuids.constEnd(); ++it) {
        if (localUuids.contains(*it))
            willUpdate++;
        else
            willAdd++;
    }
    int addedCount = 0, updatedCount = 0, deletedCount = 0;
    QStringList bodiesToDeleteLocal;
    // A delta graph intentionally omits unchanged bodies and local dirty
    // bodies. Only explicit REMOVE UUIDs are eligible for deletion.
    if (deltaGraph) {
        bodiesToDeleteLocal = removedIds;
    } else {
        for (auto it = localUuids.constBegin(); it != localUuids.constEnd(); ++it) {
            if (!remoteUuids.contains(*it))
                bodiesToDeleteLocal.append(*it);
        }
    }
    fprintf(stderr,
            "[Collab] pullACISEntityGraph: remoteUuids=%d localUuids=%d willAdd=%d willUpdate=%d willDelete=%d\n",
            (int)remoteUuids.size(), (int)localUuids.size(), willAdd, willUpdate, (int)bodiesToDeleteLocal.size());
    qDebug().noquote() << "[Collab] pullACISEntityGraph: remote bodies=" << remoteBodies.count()
                       << "local=" << (int)localUuids.size() << "will add=" << willAdd << "will update=" << willUpdate
                       << "will delete=" << bodiesToDeleteLocal.size();

    // 5a. 按 body 在 remoteBodies 中的顺序处理（对应 entity_graph 节点顺序）
    for (int i = 0; i < remoteBodies.count(); ++i) {
        ENTITY *body = remoteBodies[i];
        if (!body || !is_BODY(body))
            continue;

        // 找这个 body 的 UUID（deserialize 路径用 uuidToEntity，SAT 路径用 index 对齐）
        QString uuid;
        BODY *remoteBodyPtr = (BODY *)body;
        if (!remoteUuidToBody.isEmpty()) {
            // 从 uuidToEntity 找（JSON 路径）或 index 对齐（SAT 路径）
            // remoteUuidToBody 的 key = node.id (UUID)
            for (auto it = remoteUuidToBody.constBegin(); it != remoteUuidToBody.constEnd(); ++it) {
                if (it.value() == (void *)body) {
                    uuid = it.key();
                    break;
                }
            }
            if (uuid.isEmpty() && i < remoteUuidToBody.size()) {
                // SAT 路径：按顺序对应（bodyUuidOrder）
                const QJsonArray bodyNodes = entityGraphJson.value("nodes").toArray();
                int bodyIdx = 0;
                for (const QJsonValue &nv : bodyNodes) {
                    const QJsonObject n = nv.toObject();
                    const QJsonArray labels = n.value("labels").toArray();
                    bool isBody = false;
                    for (const QJsonValue &lv : labels) {
                        if (lv.toString() == QStringLiteral("body")) {
                            isBody = true;
                            break;
                        }
                    }
                    if (!isBody)
                        continue;
                    if (bodyIdx == i) {
                        uuid = n.value("id").toString();
                        break;
                    }
                    bodyIdx++;
                }
            }
        }

        bool wasLocal = !uuid.isEmpty() && localUuids.contains(uuid);

        // 如果本地已有，替换旧 body
        if (wasLocal) {
            ENTITY *oldBody = (ENTITY *)localUuidToPtr.value(uuid, nullptr);
            if (oldBody && oldBody != body) {
                // 删除旧 body（从 entity_tree 摘除，不调 delete_all_delta_states）
                auto &etiList = curWindow->getEntityTree();
                for (int k = (int)etiList.size() - 1; k >= 0; --k) {
                    if (QString::fromStdString(etiList[k].uuid) == uuid) {
                        int removedIdx = etiList[k].index;
                        etiList.erase(etiList.begin() + k);
                        entityIndexToUuid.remove(removedIdx);
                        break;
                    }
                }
                API_NOP_BEGIN;
                api_del_entity(oldBody);
                API_NOP_END;
            }
            updatedCount++;
        }

        // addEntity 新/替换后的 body
        fprintf(stderr, "[Collab] pullACISEntityGraph: i=%d body=%p uuid=%s about to call addEntity\n", i, (void *)body,
                qPrintable(uuid));
        const QString remoteName = remoteNames.value(uuid).trimmed();
        const QString stableName = !remoteName.isEmpty() ? remoteName : localUuidToName.value(uuid).trimmed();
        const QString name = !stableName.isEmpty() ? QString::fromUtf8("远程 JSON · %1").arg(stableName)
                                                   : (fromJson ? QString::fromUtf8("远端JSON%1").arg(i)
                                                               : QString::fromUtf8("远端EntityGraph%1").arg(i));
        try {
            curWindow->addEntity(body, name.toStdString(), -1);
            fprintf(stderr, "[Collab] pullACISEntityGraph: addEntity returned OK for i=%d\n", i);
        } catch (const std::exception &e) {
            qWarning().noquote() << "[Collab] pullACISEntityGraph: addEntity threw:" << e.what();
            continue;
        } catch (...) {
            qWarning() << "[Collab] pullACISEntityGraph: addEntity threw unknown";
            continue;
        }

        // 修正 UUID（addEntity 会生成新 UUID）
        if (!uuid.isEmpty()) {
            int last = (int)curWindow->getEntityTree().size() - 1;
            if (last >= 0) {
                curWindow->getEntityTree()[last].uuid = uuid.toStdString();
                if (!stableName.isEmpty())
                    curWindow->getEntityTree()[last].name = name.toStdString();
                entityIndexToUuid[curWindow->getEntityTree()[last].index] = uuid;
            }
        }
        addedCount++;
    }

    // 5b. 删除本地多余 body
    qDebug() << "[Collab] pullACISEntityGraph: merge loop done, entering delete phase";
    if (!bodiesToDeleteLocal.isEmpty()) {
        auto &etiList = curWindow->getEntityTree();
        for (int i = (int)etiList.size() - 1; i >= 0; --i) {
            const QString u = QString::fromStdString(etiList[i].uuid);
            if (bodiesToDeleteLocal.contains(u)) {
                ENTITY *oldBody = etiList[i].ptrEntity;
                int removedIdx = etiList[i].index;
                if (oldBody) {
                    API_NOP_BEGIN;
                    api_del_entity(oldBody);
                    API_NOP_END;
                }
                etiList.erase(etiList.begin() + i);
                entityIndexToUuid.remove(removedIdx);
                deletedCount++;
            }
        }
    }

    // 6. 清理
    delete deserialized.entities;
    CollabSession::instance().setApplyingRemoteSnapshot(false);
    CollabSession::instance().onRemoteApplied(version);

    // 7. 刷新显示
    curWindow->updateTreeWidget();
    curWindow->updateMeshData();

    qDebug().noquote() << "[Collab] pullACISEntityGraph: [" << (fromJson ? "JSON" : "SAT") << "] ADDED=" << addedCount
                       << "UPDATED=" << updatedCount << "DELETED=" << deletedCount;
    return true;
}

// =========================================================================
// 辅助函数：SAT restore + UUID 对齐（EntityGraph fallback 路径）
// SAT 中的 body 顺序与 entity_graph 中 body 节点顺序一致（serialize 时
// 按 entity_tree 顺序遍历，对应 entity_graph 节点顺序）
// =========================================================================
bool MainWindow::restoreSatWithUuidAlignment(const QString &satContent, const QJsonObject &entityGraphJson,
                                             ENTITY_LIST &outBodies, QHash<QString, void *> &outUuidToBody) {
    // 1. 收集 body UUID 顺序（按 entity_graph 节点顺序）
    QStringList bodyUuidOrder;
    {
        const QJsonArray nodes = entityGraphJson.value("nodes").toArray();
        for (const QJsonValue &nv : nodes) {
            const QJsonObject nodeObj = nv.toObject();
            const QJsonArray labels = nodeObj.value("labels").toArray();
            bool isBody = false;
            for (const QJsonValue &lv : labels) {
                if (lv.toString() == QStringLiteral("body")) {
                    isBody = true;
                    break;
                }
            }
            if (!isBody)
                continue;
            const QString uuid = nodeObj.value("id").toString();
            if (!uuid.isEmpty())
                bodyUuidOrder.append(uuid);
        }
    }
    qDebug().noquote() << "[Collab] restoreSatWithUuidAlignment: collected" << bodyUuidOrder.size()
                       << "body UUIDs from entity_graph";

    // 2. SAT restore
    QTemporaryFile satTmp(QDir::tempPath() + "/dbcad_pull_eg_XXXXXX.sat");
    satTmp.setAutoRemove(false);
    if (!satTmp.open()) {
        qWarning() << "[Collab] restoreSatWithUuidAlignment: cannot open temp sat";
        return false;
    }
    satTmp.write(satContent.toUtf8());
    satTmp.flush();
    satTmp.close();
    const std::string tmpPath = satTmp.fileName().toStdString();

    bool restoreOk = false;
    QString restoreErr;
    {
        FILE *f = std::fopen(tmpPath.c_str(), "rb");
        if (f == nullptr) {
            restoreErr = QStringLiteral("open temp sat failed");
        } else {
            try {
                acis_restore_entity_list(outBodies, f, 2, 0, 1);
                restoreOk = true;
            } catch (const std::exception &e) {
                restoreErr = QString::fromUtf8(e.what());
            } catch (...) {
                restoreErr = QStringLiteral("unknown ACIS exception");
            }
            std::fclose(f);
        }
    }
    QFile::remove(satTmp.fileName());

    if (!restoreOk) {
        qWarning().noquote() << "[Collab] restoreSatWithUuidAlignment: acis_restore_entity_list failed:" << restoreErr;
        return false;
    }
    qDebug().noquote() << "[Collab] restoreSatWithUuidAlignment: restored" << outBodies.count() << "bodies";

    // 3. 按顺序对齐 UUID（outBodies[i] → bodyUuidOrder[i]）
    for (int i = 0; i < outBodies.count() && i < bodyUuidOrder.size(); ++i) {
        ENTITY *body = outBodies[i];
        if (!body || !is_BODY(body))
            continue;
        const QString &uuid = bodyUuidOrder[i];
        outUuidToBody.insert(uuid, (void *)body);
    }

    return true;
}

bool MainWindow::exportCurrentModelToSat(QString *satContent, QString *errorMessage) {
    if (satContent == nullptr) {
        return false;
    }
    satContent->clear();

    if (curWindow == nullptr) {
        if (errorMessage != nullptr) {
            *errorMessage = tr("当前窗口为空。");
        }
        return false;
    }

    // 直接从 Window 的 entity_tree 获取实体列表，不再依赖 acis_get_noattrib_toplevel_active_entities()。
    // 原因：api_restore_entity_list() 读取 SAT 后实体进入了 ACIS，但
    // acis_get_noattrib_toplevel_active_entities 仍找不到已 restore 的实体（ACIS 内部状态不一致），
    // 导致协作 pull 后的实体在 export 时丢失。entity_tree 是 Qt 层维护的实体列表，始终准确。
    ENTITY_LIST el = curWindow->getEntityList();
    qDebug().noquote() << "[Collab] exportCurrentModelToSat: el.count()=" << el.count()
                       << "(from entity_tree, not ACIS API)";
    if (el.count() == 0) {
        if (errorMessage != nullptr) {
            *errorMessage = tr("当前无顶级活跃实体。");
        }
        return false;
    }

    QTemporaryFile tempFile(QDir::tempPath() + "/dbcad_save_XXXXXX.sat");
    tempFile.setAutoRemove(true);
    if (!tempFile.open()) {
        if (errorMessage != nullptr) {
            *errorMessage = tr("无法创建临时文件用于导出SAT");
        }
        return false;
    }

    const QString tempPath = tempFile.fileName();
    tempFile.close();

    FILE *f = fopen(tempPath.toStdString().c_str(), "wb");
    if (!f) {
        if (errorMessage != nullptr) {
            *errorMessage = tr("无法打开临时SAT文件用于写入");
        }
        return false;
    }

    try {
        API_NOP_BEGIN;
        api_save_version(2, 0);
        FileInfo fileinfo;
        fileinfo.set_units(1.0);
        fileinfo.set_product_id("dbcad_fastapi");
        result = api_set_file_info((FileIdent | FileUnits), fileinfo);
        result = api_set_int_option("sequence_save_files", 1);
        result = api_save_entity_list(f, true, el);
        API_NOP_END;
    } catch (const std::exception &e) {
        fclose(f);
        if (errorMessage != nullptr) {
            *errorMessage = tr("ACIS 异常：%1").arg(QString::fromUtf8(e.what()));
        }
        return false;
    } catch (...) {
        fclose(f);
        if (errorMessage != nullptr) {
            *errorMessage = tr("ACIS 未知异常");
        }
        return false;
    }
    fclose(f);

    QFile satFile(tempPath);
    if (!satFile.open(QIODevice::ReadOnly)) {
        if (errorMessage != nullptr) {
            *errorMessage = tr("读取临时SAT文件失败");
        }
        return false;
    }

    *satContent = QString::fromUtf8(satFile.readAll());
    satFile.close();
    if (satContent->isEmpty()) {
        if (errorMessage != nullptr) {
            *errorMessage = tr("导出的SAT内容为空");
        }
        return false;
    }

    return true;
}
