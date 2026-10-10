#include "mainwindow_internal.h"

void MainWindow::setFastAPIConnectInfo() {
    auto values = read_config_lines("fastapi_connect_info.conf", 3);
    if (!values.empty()) {
        if (!values[0].empty()) {
            fastapi_base_url = values[0];
        }
        if (!values[1].empty()) {
            fastapi_author = values[1];
        }
        fastapi_password = values[2];
    }

    QDialog dialog(this);
    dialog.setWindowTitle(tr("FastAPI连接配置"));
    dialog.setMinimumSize(420, 220);
    QFormLayout form(&dialog);

    QLineEdit baseUrlEdit(&dialog);
    baseUrlEdit.setPlaceholderText("http://127.0.0.1:8000");
    baseUrlEdit.setText(QString::fromStdString(fastapi_base_url));
    form.addRow(tr("FastAPI地址:"), &baseUrlEdit);

    QLineEdit authorEdit(&dialog);
    authorEdit.setPlaceholderText("dbcad-exe");
    authorEdit.setText(QString::fromStdString(fastapi_author));
    form.addRow(tr("作者名:"), &authorEdit);

    QLineEdit passwordEdit(&dialog);
    passwordEdit.setEchoMode(QLineEdit::Password);
    passwordEdit.setPlaceholderText("可留空（后端未启用密码时）");
    passwordEdit.setText(QString::fromStdString(fastapi_password));
    form.addRow(tr("连接密码:"), &passwordEdit);

    QPushButton testButton(tr("测试连接"), &dialog);
    form.addRow(&testButton);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    QObject::connect(&testButton, &QPushButton::clicked, &dialog, [&]() {
        const QString url = baseUrlEdit.text().trimmed();
        if (url.isEmpty()) {
            QMessageBox::warning(this, tr("FastAPI连接测试"), tr("FastAPI地址不能为空"));
            return;
        }

        BackendApiClient client(url, authorEdit.text().trimmed(), passwordEdit.text());
        auto project = client.getProjectByName("__dbcad_connection_test__");
        Q_UNUSED(project);
        const QString err = client.lastError();
        if (!err.isEmpty()) {
            QMessageBox::warning(this, tr("FastAPI连接测试"), tr("连接失败：%1").arg(err));
        } else {
            QMessageBox::information(this, tr("FastAPI连接测试"), tr("连接成功。"));
        }
    });

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    const QString baseUrl = baseUrlEdit.text().trimmed();
    if (baseUrl.isEmpty()) {
        QMessageBox::warning(this, tr("FastAPI连接配置"), tr("FastAPI地址不能为空"));
        return;
    }

    fastapi_base_url = baseUrl.toStdString();
    fastapi_author = authorEdit.text().trimmed().toStdString();
    fastapi_password = passwordEdit.text().trimmed().toStdString();

    QString writeError;
    if (!write_config_lines("fastapi_connect_info.conf", {fastapi_base_url, fastapi_author, fastapi_password},
                            &writeError)) {
        QMessageBox::warning(this, tr("FastAPI连接配置"), writeError);
        return;
    }

    statusBar()->showMessage(tr("FastAPI连接信息已更新"), 2000);

    // 保存连接信息后，如果有项目ID则重新建立WebSocket连接
    if (!fastapi_project_id.isEmpty() && !fastapi_base_url.empty()) {
        reconnectFastAPISync();
    }
}

void MainWindow::setBridgeConnectInfo() {
    auto values = read_config_lines("storage_bridge_connect_info.conf", 2);
    if (!values.empty()) {
        if (!values[0].empty()) {
            bridge_bind_host = values[0];
        }
        if (!values[1].empty()) {
            try {
                bridge_bind_port = std::stoi(values[1]);
            } catch (...) {
                bridge_bind_port = 8100;
            }
        }
    } else {
        if (bridge_bind_host.empty()) {
            bridge_bind_host = "127.0.0.1";
        }
        if (bridge_bind_port <= 0) {
            bridge_bind_port = 8100;
        }
    }

    QDialog dialog(this);
    dialog.setWindowTitle(tr("Bridge模式配置"));
    dialog.setMinimumSize(420, 180);
    QFormLayout form(&dialog);

    QLineEdit hostEdit(&dialog);
    hostEdit.setText(QString::fromStdString(bridge_bind_host));
    form.addRow(tr("Bridge监听地址:"), &hostEdit);

    QSpinBox portEdit(&dialog);
    portEdit.setMinimum(1);
    portEdit.setMaximum(65535);
    portEdit.setValue(bridge_bind_port <= 0 ? 8100 : bridge_bind_port);
    form.addRow(tr("Bridge监听端口:"), &portEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    const QString host = hostEdit.text().trimmed();
    if (host.isEmpty()) {
        QMessageBox::warning(this, tr("Bridge模式配置"), tr("Bridge监听地址不能为空"));
        return;
    }

    bridge_bind_host = host.toStdString();
    bridge_bind_port = portEdit.value();

    QString writeError;
    if (!write_config_lines("storage_bridge_connect_info.conf", {bridge_bind_host, std::to_string(bridge_bind_port)},
                            &writeError)) {
        QMessageBox::warning(this, tr("Bridge模式配置"), writeError);
        return;
    }

    statusBar()->showMessage(tr("Bridge模式配置已更新"), 2000);
}

void MainWindow::toggleBridgeMode(bool checked) {
    if (checked) {
        if (bridgeProcess != nullptr && bridgeProcess->state() != QProcess::NotRunning) {
            statusBar()->showMessage(tr("Bridge模式已在运行"), 2000);
            return;
        }

        if (neo4jdb_host.empty()) {
            QMessageBox::warning(this, tr("Bridge模式"), tr("请先配置neo4j连接信息。"));
            if (toggleBridgeModeAct != nullptr) {
                QSignalBlocker blocker(toggleBridgeModeAct);
                toggleBridgeModeAct->setChecked(false);
            }
            return;
        }

        QString program = QCoreApplication::applicationFilePath();
        QStringList args;
        args << "--storage-bridge"
             << "--bridge-host" << QString::fromStdString(bridge_bind_host) << "--bridge-port"
             << QString::number(bridge_bind_port) << "--neo4j-host" << QString::fromStdString(neo4jdb_host)
             << "--neo4j-port" << QString::number(neo4jdb_port_bolt) << "--neo4j-user"
             << QString::fromStdString(neo4jdb_username) << "--neo4j-password"
             << QString::fromStdString(neo4jdb_password);

        bridgeProcess = new QProcess(this);
        bridgeProcess->setWorkingDirectory(QCoreApplication::applicationDirPath());
        bridgeProcess->setProcessChannelMode(QProcess::SeparateChannels);

        connect(bridgeProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
                [this](int exitCode, QProcess::ExitStatus exitStatus) {
                    const bool expectedStop = bridgeStopRequested;
                    bridgeStopRequested = false;

                    QString stderrText;
                    if (bridgeProcess != nullptr) {
                        stderrText = QString::fromLocal8Bit(bridgeProcess->readAllStandardError()).trimmed();
                        bridgeProcess->deleteLater();
                        bridgeProcess = nullptr;
                    }

                    if (toggleBridgeModeAct != nullptr) {
                        QSignalBlocker blocker(toggleBridgeModeAct);
                        toggleBridgeModeAct->setChecked(false);
                    }

                    if (!expectedStop) {
                        QString msg = tr("Bridge进程已退出（exit=%1）。").arg(exitCode);
                        if (exitStatus == QProcess::CrashExit) {
                            msg += tr(" 进程发生崩溃。");
                        }
                        if (!stderrText.isEmpty()) {
                            msg += tr("\n错误输出：%1").arg(stderrText);
                        }
                        QMessageBox::warning(this, tr("Bridge模式"), msg);
                    } else {
                        statusBar()->showMessage(tr("Bridge模式已停止"), 2000);
                    }
                });

        bridgeStopRequested = false;
        bridgeProcess->start(program, args);
        if (!bridgeProcess->waitForStarted(5000)) {
            const QString error = bridgeProcess->errorString();
            bridgeProcess->deleteLater();
            bridgeProcess = nullptr;
            if (toggleBridgeModeAct != nullptr) {
                QSignalBlocker blocker(toggleBridgeModeAct);
                toggleBridgeModeAct->setChecked(false);
            }
            QMessageBox::warning(this, tr("Bridge模式"), tr("启动Bridge进程失败：%1").arg(error));
            return;
        }

        statusBar()->showMessage(tr("Bridge模式已启动，PID=%1").arg(bridgeProcess->processId()), 3000);
    } else {
        if (bridgeProcess == nullptr) {
            return;
        }
        bridgeStopRequested = true;
        if (bridgeProcess->state() != QProcess::NotRunning) {
            bridgeProcess->terminate();
            if (!bridgeProcess->waitForFinished(3000)) {
                bridgeProcess->kill();
            }
        }
    }
}
