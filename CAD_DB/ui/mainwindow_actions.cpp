#include "mainwindow_internal.h"

void MainWindow::createActions() {
    QMenu *fileMenu = menuBar()->addMenu(tr("文件(&F)"));
    QToolBar *fileToolBar = addToolBar(tr("文件"));

    // 清空当前Window
    const QIcon clearIcon = QIcon::fromTheme("document-clear", QIcon(":/images/clear.png"));
    QAction *clearAct = new QAction(clearIcon, tr("清空(&L)"), this);
    clearAct->setStatusTip(tr("清空"));
    connect(clearAct, &QAction::triggered, this, &MainWindow::clear);
    fileMenu->addAction(clearAct);
    fileToolBar->addAction(clearAct);

    // 创建Window
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("新建(&N)"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("新建文件"));
    connect(newAct, &QAction::triggered, this, &MainWindow::addWindow);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("打开(&O)"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("打开已有文件"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

    const QIcon undoIcon = QIcon::fromTheme("document-undo", QIcon(":/images/undo.png"));
    QAction *undoAct = new QAction(undoIcon, tr("撤销(&U)"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("撤销"));
    connect(undoAct, &QAction::triggered, this, &MainWindow::undo);
    fileMenu->addAction(undoAct);
    fileToolBar->addAction(undoAct);

    const QIcon redoIcon = QIcon::fromTheme("document-redo", QIcon(":/images/redo.png"));
    QAction *redoAct = new QAction(redoIcon, tr("重做(&Y)"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("重做"));
    connect(redoAct, &QAction::triggered, this, &MainWindow::redo);
    fileMenu->addAction(redoAct);
    fileToolBar->addAction(redoAct);

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("保存模型(&S)"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("保存模型"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);

    const QIcon saveEntityIcon = QIcon::fromTheme("document-save", QIcon(":/images/save_entity.png"));
    QAction *saveEntityAct = new QAction(saveEntityIcon, tr("保存模型树(&D)"), this);
    saveEntityAct->setShortcut(tr("ctrl+D"));
    saveEntityAct->setStatusTip(tr("导出模型结构"));
    connect(saveEntityAct, &QAction::triggered, this, &MainWindow::saveEntity);
    fileMenu->addAction(saveEntityAct);
    fileToolBar->addAction(saveEntityAct);

    const QIcon saveDebugFileIcon = QIcon::fromTheme("document-save", QIcon(":/images/save_entity.png"));
    QAction *saveDebugFileAct = new QAction(saveDebugFileIcon, tr("保存调试文件(&D)"), this);
    saveDebugFileAct->setStatusTip(tr("保存调试文件"));
    connect(saveDebugFileAct, &QAction::triggered, this, &MainWindow::saveDebugFile);
    fileMenu->addAction(saveDebugFileAct);
    fileToolBar->addAction(saveDebugFileAct);

    const QIcon saveImageIcon = QIcon::fromTheme("document-save", QIcon(":/images/save_image.png"));
    QAction *saveImageAct = new QAction(saveImageIcon, tr("保存截图(&T)"), this);
    saveImageAct->setStatusTip(tr("保存截图"));
    connect(saveImageAct, &QAction::triggered, this, &MainWindow::saveImage);
    fileMenu->addAction(saveImageAct);
    fileToolBar->addAction(saveImageAct);

    const QIcon saveAsIcon = QIcon::fromTheme("document-save-as", QIcon(":/images/saveas.png"));
    QAction *saveAsAct = new QAction(saveAsIcon, tr("另存为(&A)"), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("另存为新模型"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);
    fileMenu->addAction(saveAsAct);
    fileToolBar->addAction(saveAsAct);

    const QIcon visiablilityIcon = QIcon::fromTheme("document-visiablility", QIcon(":/images/visiablility.png"));
    QAction *visiablilityAct = new QAction(visiablilityIcon, tr("显示(&V)"), this);
    visiablilityAct->setStatusTip(tr("是否显示"));
    connect(visiablilityAct, &QAction::triggered, this, &MainWindow::visiablility);
    fileMenu->addAction(visiablilityAct);
    fileToolBar->addAction(visiablilityAct);

    const QIcon infoIcon = QIcon::fromTheme("document-info", QIcon(":/images/info.png"));
    QAction *infoAct = new QAction(infoIcon, tr("模型信息(&I)"), this);
    infoAct->setStatusTip(tr("模型信息"));
    connect(infoAct, &QAction::triggered, this, &MainWindow::displayInfo);
    fileMenu->addAction(infoAct);
    fileToolBar->addAction(infoAct);

    fileMenu->addSeparator();

    // 退出系统
    const QIcon exitIcon = QIcon::fromTheme("application-exit", QIcon(":/images/exit.png"));
    QAction *exitAct = fileMenu->addAction(exitIcon, tr("退出(&X)"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("退出系统"));

    // 插入基本体的菜单项和工具栏
    QMenu *insertMenu = menuBar()->addMenu(tr("插入(&I)"));
    QMenu *insertSubMenu = nullptr;
    QToolBar *insertSubToolBar = nullptr;
    for (auto it = menus_entities.begin(); it != menus_entities.end(); ++it) {
        if (it->operatorType == OPERATOR_TYPES::OPERATOR_SPERATOR) {
            if (it->subOperatorType == 0) {
                insertSubMenu = insertMenu->addMenu(it->name_cn.c_str());
                insertSubToolBar = addToolBar(tr("插入%1").arg(it->name_cn.c_str()));
            } else if (it->subOperatorType == 1)
                insertToolBarBreak(insertSubToolBar);

            continue;
        }
        const QIcon icon =
            QIcon::fromTheme("application", QIcon(QString::fromStdString(":/images/" + it->name_en + ".png")));
        QAction *act = insertSubMenu->addAction(icon, tr(it->name_cn.c_str()), this,
                                                [=]() { this->insertElements(it->operatorType, it->subOperatorType); });
        act->setCheckable(it->checkable);
        act->setStatusTip(QString::fromStdString("插入" + it->name_cn));
        insertSubToolBar->addAction(act);
    }

    // 插入布尔运算菜单项和工具栏
    QMenu *operatorMenu = menuBar()->addMenu(tr("操作(&O)"));
    QToolBar *operatorToolBar = addToolBar(tr("操作"));
    for (auto it = menus_operators.begin(); it != menus_operators.end(); ++it) {
        if (it->operatorType == OPERATOR_TYPES::OPERATOR_SPERATOR) {
            operatorMenu->addSeparator();
            operatorToolBar = addToolBar(tr("操作"));
            continue;
        }
        const QIcon icon =
            QIcon::fromTheme("application", QIcon(QString::fromStdString(":/images/" + it->name_en + ".png")));
        QAction *act = operatorMenu->addAction(icon, tr(it->name_cn.c_str()), this,
                                               [=]() { this->operation(it->operatorType, it->subOperatorType); });
        act->setCheckable(it->checkable);
        act->setStatusTip(QString::fromStdString(it->name_cn));
        operatorToolBar->addAction(act);
    }

    // 使用帮助
    QMenu *helpMenu = menuBar()->addMenu(tr("帮助(&H)"));
    QAction *aboutAct = helpMenu->addAction(tr("关于(&A)"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("显示系统说明"));
    QAction *runTestAct = helpMenu->addAction(tr("运行测试neo4j(&T)"), this, &MainWindow::runTest);
    runTestAct->setStatusTip(tr("使用./"
                                "testcases目录下的SAT文件（2."
                                "0版本，不含attrib类实体）作为测试用例，对基于neo4j数据库的几何模型存取接口进行功能和性"
                                "能测试，测试报告以追加方式输出至./testlog.txt文件"));
    QAction *incrementalTestAct = helpMenu->addAction(tr("增量存取测试"), this, &MainWindow::incrementalTest);
    incrementalTestAct->setStatusTip(tr("增量存取测试"));

    // 设置存取模式
    QMenu *settingsMenu = menuBar()->addMenu(tr("设置存取模式(&S)"));

    setACISModeAct = new QAction(tr("SAT文件(ACIS)(&A)"), this);
    setACISModeAct->setStatusTip(tr("存取为SAT文件，存取函数使用ACIS提供的API"));
    setACISModeAct->setCheckable(true);
    settingsMenu->addAction(setACISModeAct);

    setNEO4JModeAct = new QAction(tr("neo4j图数据全量存取(&N)"), this);
    setNEO4JModeAct->setStatusTip(tr("全量存取为neo4j图数据，通过Bolt协议连接到neo4j图数据库"));
    setNEO4JModeAct->setCheckable(true);
    settingsMenu->addAction(setNEO4JModeAct);

    setNEO4JIncrementalModeAct = new QAction(tr("neo4j图数据增量存取(&I)"), this);
    setNEO4JIncrementalModeAct->setStatusTip(tr("增量存取为neo4j图数据，通过Bolt协议连接到neo4j图数据库"));
    setNEO4JIncrementalModeAct->setCheckable(true);
    settingsMenu->addAction(setNEO4JIncrementalModeAct);

    setFASTAPIModeAct = new QAction(tr("FastAPI远程版本存取(&F)"), this);
    setFASTAPIModeAct->setStatusTip(tr("通过HTTP连接FastAPI后端，进行模型版本化远程存取"));
    setFASTAPIModeAct->setCheckable(true);
    settingsMenu->addAction(setFASTAPIModeAct);

    setModeActGroup = new QActionGroup(this);
    setModeActGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::Exclusive);
    setModeActGroup->addAction(setACISModeAct);
    setModeActGroup->addAction(setNEO4JModeAct);
    setModeActGroup->addAction(setNEO4JIncrementalModeAct);
    setModeActGroup->addAction(setFASTAPIModeAct);
    setACISModeAct->setChecked(true);

    settingsMenu->addSeparator();
    QAction *setNEO4JConnectInfoAct =
        settingsMenu->addAction(tr("设置neo4j连接信息"), this, &MainWindow::setNEO4JConnectInfo);
    setNEO4JConnectInfoAct->setStatusTip(
        tr("读取与程序相同目录下的neo4j_connect_info."
           "conf文件，设置neo4j图数据库连接信息。该文件第1行是IP地址，第2行是Bolt协议端口号，第3行是用户名，第4行是密码"
           "，不能包含空格和多余的换行符，编码须为UTF-8（无BOM）。"));
    settingsMenu->addAction(setNEO4JConnectInfoAct);
    QAction *setFastAPIConnectInfoAct =
        settingsMenu->addAction(tr("设置FastAPI连接信息"), this, &MainWindow::setFastAPIConnectInfo);
    setFastAPIConnectInfoAct->setStatusTip(
        tr("配置FastAPI地址、作者名与连接密码，并保存到fastapi_connect_info.conf（前三行）。"));
    settingsMenu->addAction(setFastAPIConnectInfoAct);

    QAction *setBridgeConnectInfoAct =
        settingsMenu->addAction(tr("设置Bridge模式信息"), this, &MainWindow::setBridgeConnectInfo);
    setBridgeConnectInfoAct->setStatusTip(
        tr("配置Bridge监听地址与端口，并保存到storage_bridge_connect_info.conf（前两行）。"));
    settingsMenu->addAction(setBridgeConnectInfoAct);

    toggleBridgeModeAct = settingsMenu->addAction(tr("启动Bridge模式"), this, &MainWindow::toggleBridgeMode);
    toggleBridgeModeAct->setCheckable(true);
    toggleBridgeModeAct->setStatusTip(tr("以独立CAD进程启动/停止C++ Bridge服务，便于手动联调FastAPI与客户端。"));
    settingsMenu->addAction(toggleBridgeModeAct);

    // ----------------------------------------------------------------
    // PostgreSQL 直存 SAT 文本（m_pgService 入口）。
    // 不会改动 storage_bridge_service 或 saveFile() —— 走 Qt signal/slot 调
    // m_pgService (PgService wrapping PgStore/libpq)。连接配置在 pg_connect_info.conf
    // (host/port/user/password/dbname 五行)，运行时可通过"设置 PostgreSQL 连接信息"覆盖。
    // ----------------------------------------------------------------
    QMenu *pgMenu = menuBar()->addMenu(tr("PostgreSQL(&P)"));
    pgSaveAct = pgMenu->addAction(tr("保存当前模型到 PostgreSQL"), this, &MainWindow::pgSaveCurrentToDatabase);
    pgSaveAct->setStatusTip(tr("将当前 ACIS 实体导出为 SAT，调 PgService::saveSat 写入 dbcad_pg_demo 表。"));
    pgLoadAct =
        pgMenu->addAction(tr("从 PostgreSQL 加载到当前窗口..."), this, &MainWindow::pgLoadFromDatabaseToCurrent);
    pgLoadAct->setStatusTip(
        tr("弹框输入零件名，调 PgService::loadSat 拉到本地，再用 api_restore_entity_list 重建实体。"));
    pgListAct = pgMenu->addAction(tr("列出已存零件"), this, &MainWindow::pgListPartsInDatabase);
    pgListAct->setStatusTip(tr("调 PgService::listParts，在弹窗里看 id/name/bytes/updated_at。"));
    pgCountAct = pgMenu->addAction(tr("统计已存零件数"), this, &MainWindow::pgCountPartsInDatabase);
    pgCountAct->setStatusTip(tr("调 PgService::countParts，结果通过 statusBar 显示。"));
    pgDeleteAct = pgMenu->addAction(tr("删除指定零件..."), this, &MainWindow::pgDeleteByNameFromDatabase);
    pgDeleteAct->setStatusTip(tr("弹框输入零件名，二次确认后调 PgService::deleteByName 删除该零件（不可撤销）。"));
    pgMenu->addSeparator();
    QAction *setPGConnectInfoAct =
        pgMenu->addAction(tr("设置 PostgreSQL 连接信息"), this, &MainWindow::setPGConnectInfo);
    setPGConnectInfoAct->setStatusTip(
        tr("读取/写入 pg_connect_info.conf（前 5 行：host/port/user/password/dbname）；保存后会自动重建 PgService "
           "并重连信号。"));
    updatePgMenuState();

    QMenu *collabMenu = menuBar()->addMenu(tr("协作(&C)"));
    QAction *collabPushAct = collabMenu->addAction(tr("推送(Push)"), this, &MainWindow::onCollabPushButtonClicked);
    collabPushAct->setStatusTip(tr("把本地未推送的实体图增量推送到服务器。"));
    QAction *collabPullAct = collabMenu->addAction(tr("拉取(Pull)"), this, &MainWindow::onCollabPullButtonClicked);
    collabPullAct->setStatusTip(
        tr("从服务器拉取最新增量：远端新增的 body 会加到本地画布，远端删除的 body 会被跳过（保留本地未推送的新增）。"));
    QAction *collabSyncNowAct = collabMenu->addAction(tr("刷新连接"), this, &MainWindow::requestFastAPISyncNow);
    collabSyncNowAct->setStatusTip(tr("主动请求服务器返回最新版本（仅刷新版本号，不修改画布）。"));
    QAction *collabReconnectAct = collabMenu->addAction(tr("重连协作通道"), this, &MainWindow::reconnectFastAPISync);
    collabReconnectAct->setStatusTip(tr("重建WebSocket协作连接。"));

    collabDock = new QDockWidget(tr("多人协作控制台"), this);
    collabDock->setObjectName("CollabDock");
    QWidget *collabBody = new QWidget(collabDock);
    QVBoxLayout *collabLayout = new QVBoxLayout(collabBody);

    collabConnectionLabel = new QLabel(tr("未连接"), collabBody);
    collabProjectLabel = new QLabel(tr("项目：未打开"), collabBody);
    collabVersionLabel = new QLabel(tr("本地版本：0"), collabBody);
    collabPendingLabel = new QLabel(tr("待同步版本：无"), collabBody);
    collabMembersList = new QListWidget(collabBody);
    collabMembersList->setMinimumHeight(120);
    collabMembersList->setToolTip(tr("当前在线协作者列表"));
    collabAutoFollowCheckBox = new QCheckBox(tr("自动跟随远程最新版本"), collabBody);
    collabAutoFollowCheckBox->setChecked(true);
    collabSyncNowButton = new QPushButton(tr("刷新"), collabBody);
    collabPushButton = new QPushButton(tr("推送(Push)"), collabBody);
    collabPullButton = new QPushButton(tr("拉取(Pull)"), collabBody);
    collabReconnectButton = new QPushButton(tr("重连"), collabBody);

    collabModeCombo = new QComboBox(collabBody);
    collabModeCombo->addItem(tr("Mode0 — SAT全量"), static_cast<int>(CollabMode::Mode0));
    collabModeCombo->addItem(tr("Mode1 — 纯JSON + Neo4j增量"), static_cast<int>(CollabMode::Mode1));
    collabModeCombo->addItem(tr("Mode2 — PostgreSQL增量（开发中）"), static_cast<int>(CollabMode::Mode2));
    collabModeCombo->setItemData(2, 0, Qt::UserRole - 1);
    collabModeCombo->setCurrentIndex(static_cast<int>(collabMode));
    collabModeCombo->setToolTip(tr("协作模式:\n"
                                   "  • Mode0 — SAT全量：推送/拉取完整SAT文本，简单但效率低\n"
                                   "  • Mode1 — 纯JSON + Neo4j增量：推送仅传变化Body的拓扑组件，拉取按UUID合并\n"
                                   "  • Mode2 — PostgreSQL增量：待实现"));
    collabModeCombo->setEnabled(true); // 可在项目打开后切换版本流，保留本地画布

    connect(
        collabModeCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
        [this](int index) {
            const CollabMode requested = static_cast<CollabMode>(collabModeCombo->itemData(index).toInt());
            if (requested == collabMode) {
                return;
            }

            const bool projectOpen = !fastapi_project_id.isEmpty();
            if (!projectOpen) {
                collabMode = requested;
                qDebug().noquote() << "[Collab] Mode changed to:" << static_cast<int>(collabMode);
                return;
            }

            if (requested == CollabMode::Mode2) {
                QMessageBox::information(
                    this, tr("协作模式"),
                    tr("Mode2 PostgreSQL 增量协作尚未实现，当前项目保持在 Mode%1。").arg(static_cast<int>(collabMode)));
                QSignalBlocker blocker(collabModeCombo);
                collabModeCombo->setCurrentIndex(static_cast<int>(collabMode));
                return;
            }

            // A collaboration mode selects the wire/version stream. It
            // must not discard a canvas that the user has already drawn.
            // Resolve the target stream's head first so a Mode0 version is
            // never reused as a Mode1 base (and vice versa).
            const QString projectId = fastapi_project_id;
            int targetVersion = 0;
            BackendApiClient client(QString::fromStdString(fastapi_base_url), QString::fromStdString(fastapi_author),
                                    QString::fromStdString(fastapi_password));
            if (requested == CollabMode::Mode1) {
                const auto latest = client.getJsonDelta(projectId, 0);
                if (latest.has_value()) {
                    targetVersion = latest->version;
                } else if (client.lastStatusCode() != 404) {
                    QMessageBox::warning(this, tr("协作模式"),
                                         tr("无法读取 Mode1 版本流，暂不切换：%1").arg(client.lastError()));
                    QSignalBlocker blocker(collabModeCombo);
                    collabModeCombo->setCurrentIndex(static_cast<int>(collabMode));
                    return;
                }
            } else if (requested == CollabMode::Mode0) {
                const auto latest = client.getLatestModel(projectId);
                if (latest.has_value()) {
                    targetVersion = latest->version;
                } else if (client.lastStatusCode() != 404) {
                    QMessageBox::warning(this, tr("协作模式"),
                                         tr("无法读取 Mode0 版本流，暂不切换：%1").arg(client.lastError()));
                    QSignalBlocker blocker(collabModeCombo);
                    collabModeCombo->setCurrentIndex(static_cast<int>(collabMode));
                    return;
                }
            }

            const bool hasLocalChanges =
                !pendingEntityChanges.isEmpty() || (curWindow != nullptr && curWindow->getIsModified());
            collabMode = requested;
            disconnectFastAPISync();
            auto &session = CollabSession::instance();
            // Keep the project identity, canvas and pendingEntityChanges.
            // Only the version number belongs to the selected stream.
            session.setProjectId(projectId);
            session.setModelVersion(targetVersion);
            session.setPendingRemoteVersion(0);
            fastapi_model_version = targetVersion;
            fastapi_pending_remote_version = 0;
            reconnectFastAPISync();
            updateCollabPanelUi();
            if (hasLocalChanges) {
                statusBar()->showMessage(
                    tr("已切换到 Mode%1，保留当前画布和本地修改，可直接 Push").arg(static_cast<int>(collabMode)), 4000);
            } else {
                statusBar()->showMessage(tr("已切换到 Mode%1 版本流 v%2；当前画布未自动重载，请按需 Pull")
                                             .arg(static_cast<int>(collabMode))
                                             .arg(targetVersion),
                                         4000);
            }
            qDebug().noquote() << "[Collab] Mode changed to:" << static_cast<int>(collabMode)
                               << "targetVersion=" << targetVersion << "preservedLocalChanges=" << hasLocalChanges;
        });

    connect(collabAutoFollowCheckBox, &QCheckBox::toggled, this, [this](bool checked) {
        fastapiAutoFollowRemote = checked;
        updateCollabPanelUi();
    });
    connect(collabSyncNowButton, &QPushButton::clicked, this, &MainWindow::requestFastAPISyncNow);
    connect(collabPushButton, &QPushButton::clicked, this, &MainWindow::onCollabPushButtonClicked);
    connect(collabPullButton, &QPushButton::clicked, this, &MainWindow::onCollabPullButtonClicked);
    connect(collabReconnectButton, &QPushButton::clicked, this, &MainWindow::reconnectFastAPISync);

    collabLayout->addWidget(new QLabel(tr("连接状态"), collabBody));
    collabLayout->addWidget(collabConnectionLabel);
    collabLayout->addWidget(collabProjectLabel);
    collabLayout->addWidget(collabVersionLabel);
    collabLayout->addWidget(collabPendingLabel);
    collabLayout->addWidget(new QLabel(tr("在线协作者"), collabBody));
    collabLayout->addWidget(collabMembersList);
    collabLayout->addWidget(collabAutoFollowCheckBox);
    collabLayout->addWidget(collabPushButton);
    collabLayout->addWidget(collabPullButton);

    // 协作模式选择
    collabLayout->addWidget(new QLabel(tr("协作模式"), collabBody));
    collabLayout->addWidget(collabModeCombo);

    collabLayout->addWidget(collabSyncNowButton);
    collabLayout->addWidget(collabReconnectButton);
    collabLayout->addStretch();

    collabDock->setWidget(collabBody);
    addDockWidget(Qt::RightDockWidgetArea, collabDock);
    collabDock->show();
    updateCollabPanelUi();
}
