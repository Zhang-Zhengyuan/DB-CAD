#include "mainwindow_internal.h"

void MainWindow::addWindow() {
    if (!centralWidget()) {
        curWindow = new Window(this);
        setCentralWidget(curWindow);
    } else
        QMessageBox::information(this, tr("无法创建新窗口"), tr("您最多只能创建一个窗口。"));
}

void MainWindow::open() {
    if (maybeSave()) {
        QAction *checkedAct = setModeActGroup->checkedAction();
        if (checkedAct == setACISModeAct) {
            QString fileName =
                QFileDialog::getOpenFileName(this, QObject::tr("打开文件"), QString::fromStdString(std::string(".")));
            if (!fileName.isEmpty())
                loadFile(fileName);
        } else if (checkedAct == setNEO4JModeAct) {
            bool ok;
            QDateTime curDateTime = QDateTime::currentDateTime();
            QString curDateTime_qstr =
                QString("part") +
                curDateTime.toString("yyyyMMddhhmmss"); // 保存零件对话框的默认零件名是part+当前年月日时分秒
            QString partnametext =
                QInputDialog::getText(this, tr("打开零件(neo4j)"), tr("请输入打开的零件名:"), QLineEdit::Normal,
                                      curDateTime_qstr, &ok, this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
            if (ok && !partnametext.isEmpty()) {
                loadFile(partnametext);
            }
        } else if (checkedAct == setFASTAPIModeAct) {
            QDateTime curDateTime = QDateTime::currentDateTime();
            QString curDateTime_qstr = QString("part") + curDateTime.toString("yyyyMMddhhmmss");

            QDialog dialog(this);
            dialog.setMinimumSize(220, 220);
            dialog.setWindowTitle(tr("打开零件(FastAPI)"));
            QFormLayout form(&dialog);

            QLabel lineEdit_partname_label(tr("请输入打开的项目名:"), &dialog);
            form.addRow(&lineEdit_partname_label);
            QLineEdit lineEdit_partname(&dialog);
            lineEdit_partname.setText(curDateTime_qstr);
            form.addRow(&lineEdit_partname);

            QLabel spinBox_version_label(tr("请输入版本号（0表示最新）:"), &dialog);
            form.addRow(&spinBox_version_label);
            QSpinBox spinBox_version(&dialog);
            spinBox_version.setMinimum(0);
            spinBox_version.setMaximum(2000000000);
            form.addRow(&spinBox_version);

            QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
            form.addRow(&buttonBox);
            QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
            QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

            if (dialog.exec() == QDialog::Accepted && !lineEdit_partname.text().isEmpty()) {
                if (spinBox_version.value() <= 0) {
                    loadFile(lineEdit_partname.text());
                } else {
                    loadFile(lineEdit_partname.text(), spinBox_version.value());
                }
            }
        } else {
            assert(checkedAct == setNEO4JIncrementalModeAct);
            QDateTime curDateTime = QDateTime::currentDateTime();
            QString curDateTime_qstr =
                QString("part") +
                curDateTime.toString("yyyyMMddhhmmss"); // 保存零件对话框的默认零件名是part+当前年月日时分秒

            QDialog dialog(this);
            dialog.setMinimumSize(200, 200);
            dialog.setWindowTitle(tr("打开零件(neo4j)"));
            QFormLayout form(&dialog);

            QLabel lineEdit_partname_label(tr("请输入打开的零件名:"), &dialog);
            form.addRow(&lineEdit_partname_label);
            QLineEdit lineEdit_partname(&dialog);
            lineEdit_partname.setText(curDateTime_qstr);
            form.addRow(&lineEdit_partname);

            QLabel spinBox_generation_label(tr("请输入打开的零件版次（正整数）:"), &dialog);
            form.addRow(&spinBox_generation_label);
            QSpinBox spinBox_generation(&dialog);
            spinBox_generation.setMinimum(1);
            spinBox_generation.setMaximum(2000000000);
            form.addRow(&spinBox_generation);

            QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
            form.addRow(&buttonBox);
            QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
            QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

            if (dialog.exec() == QDialog::Accepted) {
                loadFile(lineEdit_partname.text(), spinBox_generation.value());
            }
        }
    }
}

void MainWindow::buildTreeFromHistroy(HISTORY_STREAM *hs) {
    if (hs == nullptr) {
        hs = get_default_stream();
    }

    set_logging(false);
    curWindow->clear();
    DELTA_STATE *this_ds = hs->get_root_ds();
    while (this_ds) {
        GME_DELTA_STATE_user_data *delta_state_user_data = (GME_DELTA_STATE_user_data *)(this_ds->get_user_data());
        if (delta_state_user_data) {
            auto tree_items = delta_state_user_data->get_tree_items();
            for (auto item : *tree_items) {
                item.ptrDisplayData = nullptr;
                curWindow->addTreeItem(item);
                ENTITY_TREE_ITEM *restored = curWindow->getEntityItemByIndex(item.index);
                if (restored == nullptr) {
                    qWarning() << "[History] restored entity index not found:" << item.index;
                    continue;
                }
                restored->index_support.clear();
                for (auto base : item.index_base) {
                    ENTITY_TREE_ITEM *baseItem = curWindow->getEntityItemByIndex(base);
                    if (baseItem == nullptr) {
                        qWarning() << "[History] base entity index not found:" << base;
                        continue;
                    }
                    baseItem->visible = false;
                    baseItem->index_support.push_back(item.index);
                }
            }
        }
        this_ds = this_ds->prev();
    }
    curWindow->updateTreeWidget();
    curWindow->updateMeshData();

    if (hs->current_ds) {
        delete hs->current_ds;
        hs->current_ds = nullptr;
    }
    set_logging(true);
}

void MainWindow::undo() {
    HISTORY_STREAM *hs = get_default_stream();
    if (!hs)
        return;
    abort_bb(hs);

    int request_n = -1;
    int actual_n = 0;

    if (hs) {
        api_roll_n_states(hs, request_n, actual_n);
    }
    if (actual_n != request_n) {
        return;
    }

    buildTreeFromHistroy(hs);
    return;
}

void MainWindow::redo() {
    HISTORY_STREAM *hs = get_default_stream();
    int request_n = 1;
    int actual_n = 0;

    if (hs) {
        api_roll_n_states(hs, request_n, actual_n);
    }
    if (actual_n != request_n) {
        return;
    }

    buildTreeFromHistroy(hs);
    return;
}

bool MainWindow::save() {
    if (curWindow == nullptr) {
        const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("DBCAD"),
                                                                     tr("当前窗口未初始化\n"
                                                                        "是否初始化？"),
                                                                     QMessageBox::Yes | QMessageBox::No);
        switch (ret) {
        case QMessageBox::Yes:
            addWindow();
        }
        return false;
    }

    ENTITY_LIST el;
    acis_get_noattrib_toplevel_active_entities(el);
    if (el.count() == 0) {
        QMessageBox::warning(this, tr("DBCAD"), tr("当前无顶级活跃实体。"));
        return false;
    }

    QAction *checkedAct = setModeActGroup->checkedAction();

    if (checkedAct == setACISModeAct) {
        if (curWindow->getCurFile().isEmpty()) {
            return saveAs();
        } else {
            return saveFile(curWindow->getCurFile());
        }
    } else {
        assert(checkedAct == setNEO4JModeAct || checkedAct == setNEO4JIncrementalModeAct ||
               checkedAct == setFASTAPIModeAct);
        if (curWindow->getCurPartName().isEmpty()) {
            return saveAs();
        } else {
            return saveFile(curWindow->getCurPartName());
        }
    }
}

void MainWindow::saveEntity() {
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

    if (curWindow->getSelectedEntities().size() == 0) {
        QMessageBox::warning(this, tr("DBCAD"), tr("请在实体树中选择导出对象"), QMessageBox::Ok);
        return;
    }

    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix(QString("txt"));
    if (dialog.exec() != QDialog::Accepted)
        return;
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    bool ss = false;
    FILE *file = fopen(dialog.selectedFiles().first().toStdString().c_str(), "a+");
    if (file) {
        for (auto ei : curWindow->getSelectedEntities()) {
            ENTITY_TREE_ITEM *ptrEti = curWindow->getEntityItemByIndex(ei);
            dump_object(file, ptrEti->name.c_str(), 0, ptrEti->ptrEntity);
        }
        fclose(file);
        ss = true;
    }
    QGuiApplication::restoreOverrideCursor();
    if (ss)
        statusBar()->showMessage(tr("已保存"), 2000);
    else
        QMessageBox::warning(this, tr("DBCAD"),
                             tr("保存文件%1失败。").arg(QDir::toNativeSeparators(dialog.selectedFiles().first())));
}

void MainWindow::saveDebugFile() {
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

    if (curWindow->getSelectedEntities().size() == 0) {
        QMessageBox::warning(this, tr("DBCAD"), tr("请在实体树中选择导出对象"), QMessageBox::Ok);
        return;
    }

    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix(QString("dbg"));
    if (dialog.exec() != QDialog::Accepted)
        return;
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    bool ss = false;
    FILE *file = fopen(dialog.selectedFiles().first().toStdString().c_str(), "a+");
    if (file) {
        for (auto ei : curWindow->getSelectedEntities()) {
            ENTITY_TREE_ITEM *ptrEti = curWindow->getEntityItemByIndex(ei);
            api_set_int_option("brief_surface_debug", false);
            api_set_int_option("brief_curve_debug", false);
            api_set_int_option("brief_pcurve_debug", false);
            debug_entity(ptrEti->ptrEntity, file);
        }
        fclose(file);
        ss = true;
    }
    QGuiApplication::restoreOverrideCursor();
    if (ss)
        statusBar()->showMessage(tr("已保存"), 2000);
    else
        QMessageBox::warning(this, tr("DBCAD"),
                             tr("保存文件%1失败。").arg(QDir::toNativeSeparators(dialog.selectedFiles().first())));
}

bool MainWindow::saveAs() {
    QAction *checkedAct = setModeActGroup->checkedAction();
    if (checkedAct == setACISModeAct) {
        QFileDialog dialog(this);
        dialog.setWindowModality(Qt::WindowModal);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setDefaultSuffix(QString("sat"));
        if (dialog.exec() != QDialog::Accepted)
            return false;
        return saveFile(dialog.selectedFiles().first());
    } else {
        assert(checkedAct == setNEO4JModeAct || checkedAct == setNEO4JIncrementalModeAct ||
               checkedAct == setFASTAPIModeAct);
        bool ok;
        QDateTime curDateTime = QDateTime::currentDateTime();
        QString curDateTime_qstr =
            QString("part") +
            curDateTime.toString("yyyyMMddhhmmss"); // 保存零件对话框的默认零件名是part+当前年月日时分秒
        QString title = checkedAct == setFASTAPIModeAct ? tr("保存零件(FastAPI)") : tr("保存零件(neo4j)");
        QString text =
            QInputDialog::getText(this, title, tr("请输入保存的零件名:"), QLineEdit::Normal, curDateTime_qstr, &ok,
                                  this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
        if (ok && !text.isEmpty()) {
            return saveFile(text);
        } else {
            return false;
        }
    }
}

void MainWindow::visiablility() {
    if (curWindow)
        curWindow->visiablility();
}

void MainWindow::displayInfo() {
    if (curWindow)
        curWindow->displayInfo();
}

void MainWindow::about() {

    QMessageBox::about(this, tr("关于DBCAD"), tr("用于测试和演示DBCAD功能与用法。"));
}

void MainWindow::runTest() {
    Neo4jPart conn(neo4jdb_host.c_str(), neo4jdb_port_bolt, neo4jdb_username.c_str(), neo4jdb_password.c_str(), "");

    for (const auto &entry : std::filesystem::directory_iterator(".\\testcases")) {
        if (entry.is_regular_file()) {
            std::string filepath = entry.path().string();
            std::string extname = std::filesystem::path(filepath).extension().string();
            std::transform(extname.begin(), extname.end(), extname.begin(), tolower); // 忽略大小写
            if (extname == ".sat") {
                std::string TestCaseName = std::filesystem::path(filepath).stem().string();
                ENTITY_LIST el;
                acis_restore_entity_list(el, filepath.c_str(), 2, 0, true);

                auto [testresult, neo4j_save_duration, acis_save_duration, neo4j_restore_duration,
                      acis_restore_duration] = AccessTest::CheckTestCase(conn, TestCaseName, el);
                api_del_entity_list(el);

                std::ofstream logfs("testlog.txt", std::ios::app);
                logfs << std::format("=======Test: {}=======", filepath) << std::endl;
                logfs << (testresult ? "PASS" : "FAIL") << "\t数据库存:" << neo4j_save_duration
                      << "ms\t文件存:" << acis_save_duration
                      << "ms\t数据库存/文件存:" << neo4j_save_duration / acis_save_duration
                      << "\t数据库取:" << neo4j_restore_duration << "ms\t文件取:" << acis_restore_duration
                      << "ms\t数据库取/文件取:" << neo4j_restore_duration / acis_restore_duration << std::endl;
                logfs.close();
            }
        }
    }

    QMessageBox::information(this, tr("DBCAD"), tr("neo4j测试运行结束，请打开./testlog.txt文件查看测试报告。"));
}

void MainWindow::incrementalTest() {
    TMDF;

    {

        std::ofstream logfs("incremental_dighole_testlog.txt", std::ios::app);
        BODY *block;
        check_outcome(api_solid_block(SPAposition(0.0, 0.0, 0.0), SPAposition(1000.0, 2000.0, 300.0), block));
        bool is_through = false;
        int holecnt = 0;
        for (int i = 10; i <= 990; i += 20) {      // 50个
            for (int j = 10; j <= 1990; j += 20) { // 100个
                BODY *hole;
                check_outcome(api_solid_cylinder_cone(SPAposition(i, j, is_through ? -10.0 : 100.0),
                                                      SPAposition(i, j, 310.0), 5.0, 5.0, 5.0, nullptr, hole));
                check_outcome(api_subtract(hole, block));
                holecnt++;

                std::string acis_save_filename_str = std::format("acis_partTest_save_{}.sat", holecnt);
                const char *acis_save_filename = acis_save_filename_str.c_str();
                std::string acis_save_check_filename_str = std::format("check_acis_partTest_save_{}.sat", holecnt);
                const char *acis_save_check_filename = acis_save_check_filename_str.c_str();
                std::string conn_check_filename_str = std::format("check_incrementalTest_{}.sat", holecnt);
                const char *conn_check_filename = conn_check_filename_str.c_str();

                Neo4jPart conn_incremental(neo4jdb_host.c_str(), neo4jdb_port_bolt, neo4jdb_username.c_str(),
                                           neo4jdb_password.c_str(), "incrementalTest");

                // 增量存
                TMST;
                IncrementalContext incCtx_incrementalSave;
                api_save_neo4j(conn_incremental, incCtx_incrementalSave);
                TMED;
                double neo4j_incremental_save_duration = TMDR;

                // ACIS存
                TMST;
                acis_save_noattrib_toplevel_active_entities(acis_save_filename, 2, 0, true);
                TMED;
                double acis_save_duration = TMDR;

                is_through = !is_through;
            }
        }
    }

    QMessageBox::information(this, tr("DBCAD"), tr("增量存取测试结束。"));
}
