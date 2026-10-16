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
            const auto projectCount=dbcad::legacyPartCount({host.toStdString(),portEdit.value(),userEdit.text().trimmed().toStdString(),passwordEdit.text().toStdString()},"");
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

void MainWindow::insertElements(OPERATOR_TYPES type,int subtype) {
    if(!curWindow) return;
    dbcad::ShapeHandle shape; dbcad::HandleGrid handles;
    curWindow->createEntity(subtype,shape,handles);
    if(!shape) return;
    std::string name="实体";
    for(auto& menu:menus_entities) if(menu.operatorType==type && menu.subOperatorType==subtype) name=menu.name_cn;
    curWindow->addEntity(shape,name,subtype,handles);
    curWindow->captureHistory();
}
void MainWindow::operation(OPERATOR_TYPES type,int subtype) {
    if(!curWindow) return;
    auto ids=curWindow->getSelectedEntities();
    if(ids.size()<(type==OPERATOR_DEFEATURE?1u:2u)) { showMessage(tr("未选择足够实体"),4000); return; }
    auto first=curWindow->getEntityItemByIndex(ids[0]);
    if(!first) return;
    std::string name=first->name+"的运算结果";
    dbcad::ShapeList outputs;
    if(!curWindow->operation(type,subtype,ids,outputs,true)) return;
    if(type==OPERATOR_BOOLEAN) ids.clear();
    for(auto shape:outputs) curWindow->addEntity(shape,name,subtype,ids,type);
    curWindow->captureHistory();
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
    if (curWindow->getEntityList().empty()) {
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
