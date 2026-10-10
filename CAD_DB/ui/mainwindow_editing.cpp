#include "mainwindow_internal.h"

void MainWindow::setNEO4JConnectInfo() {
    auto values = read_config_lines("neo4j_connect_info.conf", 4);
    if (!values.empty()) {
        if (!values[0].empty()) {
            neo4jdb_host = values[0];
        }
        if (!values[1].empty()) {
            try {
                neo4jdb_port_bolt = std::stoi(values[1]);
            } catch (...) {
                neo4jdb_port_bolt = 7687;
            }
        }
        neo4jdb_username = values[2];
        neo4jdb_password = values[3];
    } else if (neo4jdb_host.empty()) {
        neo4jdb_host = "127.0.0.1";
        neo4jdb_port_bolt = 7687;
        neo4jdb_username = "neo4j";
    }

    QDialog dialog(this);
    dialog.setWindowTitle(tr("neo4j连接配置"));
    dialog.setMinimumSize(420, 240);
    QFormLayout form(&dialog);

    QLineEdit hostEdit(&dialog);
    hostEdit.setText(QString::fromStdString(neo4jdb_host));
    form.addRow(tr("主机地址:"), &hostEdit);

    QSpinBox portEdit(&dialog);
    portEdit.setMinimum(1);
    portEdit.setMaximum(65535);
    portEdit.setValue(neo4jdb_port_bolt <= 0 ? 7687 : neo4jdb_port_bolt);
    form.addRow(tr("Bolt端口:"), &portEdit);

    QLineEdit userEdit(&dialog);
    userEdit.setText(QString::fromStdString(neo4jdb_username));
    form.addRow(tr("用户名:"), &userEdit);

    QLineEdit passwordEdit(&dialog);
    passwordEdit.setEchoMode(QLineEdit::Password);
    passwordEdit.setText(QString::fromStdString(neo4jdb_password));
    form.addRow(tr("密码:"), &passwordEdit);

    QPushButton testButton(tr("测试连接"), &dialog);
    form.addRow(&testButton);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    QObject::connect(&testButton, &QPushButton::clicked, &dialog, [&]() {
        const QString host = hostEdit.text().trimmed();
        if (host.isEmpty()) {
            QMessageBox::warning(this, tr("neo4j连接测试"), tr("主机地址不能为空"));
            return;
        }

        try {
            Neo4jPart conn(host.toStdString().c_str(), portEdit.value(),
                           userEdit.text().trimmed().toStdString().c_str(), passwordEdit.text().toStdString().c_str(),
                           "");
            const int64_t projectCount = count_partnode(conn);
            Q_UNUSED(projectCount);
            QMessageBox::information(this, tr("neo4j连接测试"), tr("连接成功。"));
        } catch (const std::exception &ex) {
            QMessageBox::warning(this, tr("neo4j连接测试"), tr("连接失败：%1").arg(QString::fromUtf8(ex.what())));
        }
    });

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    const QString host = hostEdit.text().trimmed();
    if (host.isEmpty()) {
        QMessageBox::warning(this, tr("neo4j连接配置"), tr("主机地址不能为空"));
        return;
    }

    neo4jdb_host = host.toStdString();
    neo4jdb_port_bolt = portEdit.value();
    neo4jdb_username = userEdit.text().trimmed().toStdString();
    neo4jdb_password = passwordEdit.text().toStdString();

    QString writeError;
    if (!write_config_lines("neo4j_connect_info.conf",
                            {neo4jdb_host, std::to_string(neo4jdb_port_bolt), neo4jdb_username, neo4jdb_password},
                            &writeError)) {
        QMessageBox::warning(this, tr("neo4j连接配置"), writeError);
        return;
    }

    statusBar()->showMessage(tr("neo4j连接信息已更新"), 2000);
}

void MainWindow::clear() {
    curWindow->clear();
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage(tr("已就绪"));
}

void MainWindow::insertElements(const OPERATOR_TYPES ot, const int subOperatorType) {
    if (!curWindow)
        return;

    ENTITY *ptrEntity = nullptr;
    std::vector<std::vector<SPAposition>> handles;

    // 1. 开始一个新的事务（开启 bulletin board）
    api_bb_begin(TRUE); // TRUE = 线性历史流

    // 2. 执行建模操作
    curWindow->createEntity(subOperatorType, ptrEntity, handles);

    // 3. 决定事务结果
    outcome result; // 临时的 outcome，用于 api_bb_end
    if (ptrEntity != nullptr) {
        // 成功：关闭 bulletin board 并标注为成功
        api_bb_end(result, TRUE, FALSE); // 第三个参数 FALSE 表示不删除 stacked BB
        if (result.ok()) {
            // 4. 成功：记录当前状态为 DELTA_STATE
            DELTA_STATE *ds = nullptr;
            outcome note_result = api_note_state(ds);

            if (ds && ds->id() != 0) {
                // 附加自定义数据
                std::string name_cn = "实体";
                for (auto mi : menus_entities) {
                    if (mi.operatorType == ot && mi.subOperatorType == subOperatorType)
                        name_cn = mi.name_cn;
                }
                GME_DELTA_STATE_user_data *userData = ACIS_NEW GME_DELTA_STATE_user_data();
                userData->add_tree_item(curWindow->addEntity(ptrEntity, name_cn, subOperatorType, handles));
                ds->set_user_data(userData);
            }
        }
    } else {
        // 失败：关闭 bulletin board 并标注为失败（这会自动回滚模型）
        api_bb_end(result, FALSE, FALSE); // 第二个参数 FALSE 表示线性历史流（失败也需标记）
        // 注意：result 此时会包含错误信息，但不需额外处理
    }

    // curWindow->addEntity(ptrEntity, name_cn, subOperatorType, handles);
    //  @todo：以下版本管理导致demo_qt创建任意实体后再交互式插入B样条时程序崩溃。
    /*GME_DELTA_STATE_user_data* delta_state_user_data = ACIS_NEW GME_DELTA_STATE_user_data();
    delta_state_user_data->add_tree_item(curWindow->addEntity(ptrEntity, name_cn, subOperatorType, handles));

    DELTA_STATE* ds = nullptr;
    api_note_state(ds);
    ds->set_user_data(delta_state_user_data);*/

    // debug_history_stream();
}

void MainWindow::operation(const OPERATOR_TYPES ot, const int subOperatorType) {
    if (!curWindow)
        return;

    std::vector<int> ids = curWindow->getSelectedEntities();
    ENTITY_TREE_ITEM *eti0 = curWindow->getEntityItemByIndex(ids[0]);
    std::string eti0_name = eti0->name;
    ENTITY_TREE_ITEM *eti1 = curWindow->getEntityItemByIndex(ids[1]);
    std::string eti1_name = eti1->name;

    ENTITY_LIST *el = ACIS_NEW ENTITY_LIST();
    if (!curWindow->operation(ot, subOperatorType, ids, el, true))
        return;
    std::string name_cn = "运算结果";
    for (auto mi : menus_operators) {
        if (mi.operatorType == ot && mi.subOperatorType == subOperatorType)
            name_cn = mi.name_cn;
    }
    std::string name_cn2 = std::format("[{}]和[{}]的{}", eti0_name, eti1_name, name_cn);

    ids.clear(); // 两实体做布尔运算后，只保留运算结果，删除参与运算的两实体

    GME_DELTA_STATE_user_data *delta_state_user_data = ACIS_NEW GME_DELTA_STATE_user_data();
    for (ENTITY *ent = el->first(); ent; ent = el->next()) {
        delta_state_user_data->add_tree_item(curWindow->addEntity(ent, name_cn2, subOperatorType, ids, ot));
    }

    DELTA_STATE *ds = nullptr;
    api_note_state(ds);
    ds->set_user_data(delta_state_user_data);
    // debug_history_stream();
}

void MainWindow::saveImage() {
    if (curWindow == nullptr) {
        const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("DBCAD"),
                                                                     tr("当前窗口未初始化\n"
                                                                        "是否初始化？"),
                                                                     QMessageBox::Yes | QMessageBox::No);
        switch (ret) {
        case QMessageBox::Yes:
            addWindow();
        }
        return;
    }
    if (curWindow->getEntityList().count() == 0) {
        QMessageBox::warning(this, tr("DBCAD"), tr("当前窗口为空。"));
        return;
    }
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix(QString("png"));
    if (dialog.exec() != QDialog::Accepted)
        return;
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QImage im = curWindow->getScreenshot();
    bool ss = im.save(dialog.selectedFiles().first(), "PNG");
    QGuiApplication::restoreOverrideCursor();
    if (ss)
        statusBar()->showMessage(tr("截图已保存"), 2000);
    else
        QMessageBox::warning(this, tr("DBCAD"),
                             tr("保存文件%1失败。").arg(QDir::toNativeSeparators(dialog.selectedFiles().first())));
}

bool MainWindow::maybeSave() {
    if (curWindow->getIsModified()) {
        const QMessageBox::StandardButton ret =
            QMessageBox::warning(this, tr("DBCAD"),
                                 tr("模型已修改。\n"
                                    "是否保存？"),
                                 QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        switch (ret) {
        case QMessageBox::Save:
            return save();
        case QMessageBox::Cancel:
            return false;
        default:
            break;
        }
    }
    return true;
}
