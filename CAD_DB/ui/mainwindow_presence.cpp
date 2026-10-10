#include "mainwindow_internal.h"

void MainWindow::updateFastAPICollaboratorsStatus() {
    if (!fastapi_project_id.isEmpty()) {
        statusBar()->showMessage(tr("当前在线协作者：%1").arg(fastapi_collaborators.size()), 2500);
    }
    updateCollabPanelUi();
}

void MainWindow::setCollabConnectionState(const QString &stateText) {
    if (collabConnectionLabel != nullptr) {
        collabConnectionLabel->setText(stateText);
    }
}

void MainWindow::updateCollabPanelUi() {
    if (collabProjectLabel != nullptr) {
        collabProjectLabel->setText(fastapi_project_name.isEmpty() ? tr("项目：未打开")
                                                                   : tr("项目：%1").arg(fastapi_project_name));
    }
    if (collabVersionLabel != nullptr) {
        collabVersionLabel->setText(tr("本地版本：%1").arg(fastapi_model_version));
    }
    if (collabPendingLabel != nullptr) {
        if (fastapiSubmitInFlight) {
            collabPendingLabel->setText(tr("Submit: waiting for server"));
        } else if (fastapi_pending_remote_version > fastapi_model_version) {
            collabPendingLabel->setText(tr("待同步版本：%1").arg(fastapi_pending_remote_version));
        } else {
            collabPendingLabel->setText(tr("待同步版本：无"));
        }
    }
    if (collabAutoFollowCheckBox != nullptr) {
        collabAutoFollowCheckBox->setChecked(fastapiAutoFollowRemote);
    }
    if (collabMembersList != nullptr) {
        collabMembersList->clear();
        for (auto it = fastapi_collaborators.constBegin(); it != fastapi_collaborators.constEnd(); ++it) {
            const QString author = it.value().trimmed().isEmpty() ? tr("unknown") : it.value();
            collabMembersList->addItem(author + " (" + it.key() + ")");
        }
    }

    // 启用/禁用协作控件（需要连接到项目）
    bool connected = !fastapi_project_id.isEmpty() && !fastapi_base_url.empty();
    bool isSubmitting = fastapiSubmitInFlight;
    auto &session = CollabSession::instance();

    if (collabModeCombo != nullptr) {
        // Mode0 and Mode1 are independent version streams, so the selector
        // must remain available while a project is connected.  Mode2 is still
        // disabled at the item level until its PostgreSQL path is implemented.
        collabModeCombo->setEnabled(true);
    }
    if (collabPushButton != nullptr) {
        collabPushButton->setEnabled(connected && !isSubmitting && session.isConnected());
    }
    if (collabPullButton != nullptr) {
        collabPullButton->setEnabled(connected && !isSubmitting && session.isConnected());
    }
    if (collabSyncNowButton != nullptr) {
        collabSyncNowButton->setEnabled(connected && !isSubmitting);
    }
    if (collabReconnectButton != nullptr) {
        collabReconnectButton->setEnabled(connected);
    }
}

void MainWindow::applyPendingRemoteVersion() {
    // 已废弃：旧的 "待同步版本" 走的是 clear+restore 全量同步路径，与新的增量同步语义冲突。
    // 保留此函数仅为兼容既有菜单项 / 信号连接，实际调用将引导用户改用 Pull 按钮。
    statusBar()->showMessage(tr("请改用协作面板的「拉取(Pull)」按钮获取远端增量变更"), 4000);
}

void MainWindow::reconnectFastAPISync() {
    const auto enter_ts_ms = QDateTime::currentMSecsSinceEpoch();
    std::fprintf(stderr, "[DEBUG] reconnectFastAPISync ENTER pid=%llu ts=%lld project_id=%s base_url=%s\n",
                 (unsigned long long)QCoreApplication::applicationPid(), (long long)enter_ts_ms,
                 fastapi_project_id.toUtf8().constData(), fastapi_base_url.c_str());
    // 注意：不要在这里调 onReconnect —— 紧接着的 disconnectFastAPISync() 会通过
    // onDisconnected() 把 state 又打回 Disconnected。state 转 Connected_Idle 的真正时机
    // 是下面 connected lambda（socket 真连上时）。
    disconnectFastAPISync();
    std::fprintf(stderr, "[DEBUG] reconnectFastAPISync after disconnectFastAPISync\n");
    std::fprintf(stderr,
                 "[DEBUG] reconnectFastAPISync post-disconnect: project_id.isEmpty=%d base_url.empty=%d "
                 "base_url_len=%zu base_url='%s'\n",
                 fastapi_project_id.isEmpty() ? 1 : 0, fastapi_base_url.empty() ? 1 : 0, fastapi_base_url.size(),
                 fastapi_base_url.c_str());

    if (fastapi_project_id.isEmpty()) {
        std::fprintf(stderr, "[DEBUG] reconnectFastAPISync EARLY-RETURN: project_id IS EMPTY (isEmpty()=true)\n");
        setCollabConnectionState(tr("未连接"));
        updateCollabPanelUi();
        return;
    }
    if (fastapi_base_url.empty()) {
        std::fprintf(stderr, "[DEBUG] reconnectFastAPISync EARLY-RETURN: base_url IS EMPTY (was len=%zu)\n",
                     fastapi_base_url.size());
        setCollabConnectionState(tr("未连接"));
        updateCollabPanelUi();
        return;
    }

    setCollabConnectionState(tr("连接中"));
    updateCollabPanelUi();
    std::fprintf(stderr, "[DEBUG] reconnectFastAPISync label='连接中' set\n");

    QString wsBaseUrl = QString::fromStdString(fastapi_base_url).trimmed();
    while (wsBaseUrl.endsWith('/')) {
        wsBaseUrl.chop(1);
    }
    if (wsBaseUrl.startsWith("https://")) {
        wsBaseUrl.replace(0, 5, "wss");
    } else if (wsBaseUrl.startsWith("http://")) {
        wsBaseUrl.replace(0, 4, "ws");
    } else if (!wsBaseUrl.startsWith("ws://") && !wsBaseUrl.startsWith("wss://")) {
        wsBaseUrl = "ws://" + wsBaseUrl;
    }

    QString wsPath = "/ws/projects/" + fastapi_project_id;
    QStringList queryItems;
    queryItems << "client_id=" + QString::fromUtf8(QUrl::toPercentEncoding(fastapi_client_id));
    queryItems << "author=" + QString::fromUtf8(QUrl::toPercentEncoding(QString::fromStdString(fastapi_author)));
    queryItems << (collabMode == CollabMode::Mode1 ? QStringLiteral("stream=mode1") : QStringLiteral("stream=mode0"));
    if (!fastapi_password.empty()) {
        queryItems << "password=" +
                          QString::fromUtf8(QUrl::toPercentEncoding(QString::fromStdString(fastapi_password)));
    }
    wsPath += "?" + queryItems.join("&");
    const QUrl wsUrl(wsBaseUrl + wsPath);
    std::fprintf(stderr, "[DEBUG] reconnectFastAPISync ws_url=%s\n", wsUrl.toString().toUtf8().constData());

    fastapiSyncSocket = new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this);
    std::fprintf(stderr, "[DEBUG] reconnectFastAPISync new QWebSocket=%p\n", (void *)fastapiSyncSocket);

    connect(fastapiSyncSocket, &QWebSocket::textMessageReceived, this, &MainWindow::handleFastAPISyncMessage);
    connect(fastapiSyncSocket, &QWebSocket::connected, this, [this]() {
        std::fprintf(stderr, "[Collab] WS connected lambda FIRED pid=%llu\n",
                     (unsigned long long)QCoreApplication::applicationPid());
        statusBar()->showMessage(tr("FastAPI实时同步已连接"), 2000);
        if (fastapiReconnectTimer != nullptr) {
            fastapiReconnectTimer->stop();
        }
        // onReconnect(true) 把 CollabSession state 转 Connected_Idle，确保 Pull 触发 sync_now
        // 应答后 tryBeginApplyRemote 不会因 state_ == Disconnected 被拒。
        // disconnectFastAPISync() 在 reconnectFastAPISync 入口处把它打回了 Disconnected，
        // 这里必须重新转 Connected_Idle。
        CollabSession::instance().onReconnect(!fastapi_project_id.isEmpty());
        setCollabConnectionState(tr("已连接"));
        // Git 语义：重连后不主动 sync_now，避免一连接就把远端的 entity_graph_saved 自动 apply 下来。
        // 远端历史版本应等待用户主动点 Pull 才合并到本地画布。
        // （详见 COLLABORATION_TECHNICAL_ROADMAP 的 Git-like 同步策略。）
        if (fastapiHeartbeatTimer != nullptr) {
            fastapiHeartbeatTimer->start();
        }
        updateCollabPanelUi();
    });
    // 捕获"创建时那一把 socket"的指针，避免旧 socket 在 close 时被 deleteLater
    // 触发的 disconnected lambda 误启动 fastapiReconnectTimer / 把 label 改成"未连接"。
    // reconnectFastAPISync 入口处的 disconnectFastAPISync() 会显式关闭旧 socket，
    // 那一帧 disconnected 事件不应当被当作"运行中的连接掉了"处理。
    QWebSocket *const createdSocket = fastapiSyncSocket;
    connect(fastapiSyncSocket, &QWebSocket::disconnected, this, [this, createdSocket]() {
        std::fprintf(stderr, "[Collab] WS disconnected lambda FIRED pid=%llu socket=%p current_socket=%p\n",
                     (unsigned long long)QCoreApplication::applicationPid(), (void *)createdSocket,
                     (void *)fastapiSyncSocket);
        if (createdSocket != fastapiSyncSocket) {
            std::fprintf(stderr, "[Collab] WS disconnected lambda IGNORED (stale socket)\n");
            return;
        }
        if (fastapiHeartbeatTimer != nullptr) {
            fastapiHeartbeatTimer->stop();
        }
        if (!fastapi_project_id.isEmpty()) {
            statusBar()->showMessage(tr("FastAPI实时同步已断开"), 2000);
            setCollabConnectionState(tr("已断开，重连中"));
            if (fastapiReconnectTimer != nullptr && !fastapiReconnectTimer->isActive()) {
                fastapiReconnectTimer->start();
            }
        } else {
            setCollabConnectionState(tr("未连接"));
        }
        updateCollabPanelUi();
    });

    // 添加错误处理信号
    connect(fastapiSyncSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::errorOccurred), this,
            [this](QAbstractSocket::SocketError error) {
                std::fprintf(stderr, "[Collab] WS error: %d %s\n", (int)error,
                             fastapiSyncSocket ? fastapiSyncSocket->errorString().toUtf8().constData()
                                               : "(null socket)");
                statusBar()->showMessage(
                    tr("WebSocket连接失败: %1").arg(fastapiSyncSocket ? fastapiSyncSocket->errorString() : QString()),
                    5000);
            });

    std::fprintf(stderr, "[Collab] opening WebSocket...\n");
    fflush(stderr);
    fastapiSyncSocket->open(wsUrl);
    std::fprintf(stderr, "[Collab] after open() called socket_state=%d\n", (int)fastapiSyncSocket->state());
    fflush(stderr);
}
