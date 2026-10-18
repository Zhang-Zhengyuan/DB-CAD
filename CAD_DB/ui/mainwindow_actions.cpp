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

    // ---------- 插入/操作菜单：由命令注册表生成（M1.6） ----------
    registerDefaultCommands();

    QMenu *insertMenu = menuBar()->addMenu(tr("插入(&I)"));
    QMenu *insertSubMenu = nullptr;
    QToolBar *insertSubToolBar = nullptr;
    QString insertGroupId;
    const auto insertGroups = commandBus_->groups(QStringLiteral("insert"));
    for (const auto *command : commandBus_->commands(QStringLiteral("insert"))) {
        if (command->groupId != insertGroupId) {
            const dbcad::CommandBus::Group *group = nullptr;
            for (const auto& candidate : insertGroups)
                if (candidate.id == command->groupId)
                    group = &candidate;
            if (group != nullptr) {
                insertSubMenu = insertMenu->addMenu(group->title);
                insertSubToolBar = addToolBar(tr("插入%1").arg(group->title));
                insertGroupId = command->groupId;
            }
        }
        if (command->toolbarBreakBefore && insertSubToolBar != nullptr) {
            insertToolBarBreak(insertSubToolBar);
        }
        const QIcon icon = QIcon::fromTheme("application", QIcon(":/images/" + command->icon + ".png"));
        const QString id = command->id;
        QAction *act =
            insertSubMenu->addAction(icon, command->label, this, [this, id]() { commandBus_->dispatch(id); });
        act->setCheckable(command->checkable);
        act->setStatusTip(QString::fromStdString("插入" + command->label.toStdString()));
        insertSubToolBar->addAction(act);
    }

    QMenu *operatorMenu = menuBar()->addMenu(tr("操作(&O)"));
    QToolBar *operatorToolBar = addToolBar(tr("操作"));
    QString operateGroupId;
    for (const auto *command : commandBus_->commands(QStringLiteral("operate"))) {
        if (command->groupId != operateGroupId) {
            if (!operateGroupId.isEmpty())
                operatorMenu->addSeparator();
            operatorToolBar = addToolBar(tr("操作"));
            operateGroupId = command->groupId;
        }
        const QIcon icon = QIcon::fromTheme("application", QIcon(":/images/" + command->icon + ".png"));
        const QString id = command->id;
        QAction *act =
            operatorMenu->addAction(icon, command->label, this, [this, id]() { commandBus_->dispatch(id); });
        act->setCheckable(command->checkable);
        act->setStatusTip(command->label);
        operatorToolBar->addAction(act);
    }

    // 使用帮助
    QMenu *helpMenu = menuBar()->addMenu(tr("帮助(&H)"));
    QAction *aboutAct = helpMenu->addAction(tr("关于(&A)"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("显示系统说明"));

    // 设置存取模式：模式切换 = 后端选择
    QMenu *settingsMenu = menuBar()->addMenu(tr("设置存取模式(&S)"));

    setACISModeAct = new QAction(tr("SAT文件(ACIS)(&A)"), this);
    setACISModeAct->setStatusTip(tr("使用当前几何内核读写 SAT 文件"));
    setACISModeAct->setCheckable(true);
    settingsMenu->addAction(setACISModeAct);

    setFASTAPIModeAct = new QAction(tr("FastAPI远程版本存取(&F)"), this);
    setFASTAPIModeAct->setStatusTip(tr("通过HTTP连接FastAPI后端，进行模型版本化远程存取"));
    setFASTAPIModeAct->setCheckable(true);
    settingsMenu->addAction(setFASTAPIModeAct);

    setModeActGroup = new QActionGroup(this);
    setModeActGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::Exclusive);
    setModeActGroup->addAction(setACISModeAct);
    setModeActGroup->addAction(setFASTAPIModeAct);
    setACISModeAct->setChecked(true);

    connect(setACISModeAct, &QAction::triggered, this, [this]() { activeStorage_ = satBackend_.get(); });
    connect(setFASTAPIModeAct, &QAction::triggered, this, [this]() { activeStorage_ = collabBackend_.get(); });

    settingsMenu->addSeparator();
    QAction *setFastAPIConnectInfoAct =
        settingsMenu->addAction(tr("设置FastAPI连接信息"), this, &MainWindow::setFastAPIConnectInfo);
    setFastAPIConnectInfoAct->setStatusTip(
        tr("配置FastAPI地址、作者名与连接密码，并保存到fastapi_connect_info.conf（前三行）。"));
    settingsMenu->addAction(setFastAPIConnectInfoAct);

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
    QAction *collabTagAct = collabMenu->addAction(tr("定版(Tag)…"), this, &MainWindow::onCollabTagVersionClicked);
    collabTagAct->setStatusTip(tr("给当前版本打命名标签（微版本=自动同步，标签=显式定版）。"));

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
    collabSyncNowButton = new QPushButton(tr("刷新"), collabBody);
    collabPushButton = new QPushButton(tr("推送(Push)"), collabBody);
    collabPullButton = new QPushButton(tr("拉取(Pull)"), collabBody);
    collabReconnectButton = new QPushButton(tr("重连"), collabBody);

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
    collabLayout->addWidget(collabPushButton);
    collabLayout->addWidget(collabPullButton);

    // Unified collaboration format
    collabLayout->addWidget(new QLabel(tr("Mode1 · JSON + SAT 组件回退"), collabBody));

    collabLayout->addWidget(collabSyncNowButton);
    collabLayout->addWidget(collabReconnectButton);
    collabLayout->addStretch();

    collabDock->setWidget(collabBody);
    addDockWidget(Qt::RightDockWidgetArea, collabDock);
    collabDock->show();
    updateCollabPanelUi();
}
