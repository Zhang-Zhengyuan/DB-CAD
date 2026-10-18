#include "mainwindow_internal.h"

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
    int group=-1;
    for(auto shape:outputs) {
        auto item=curWindow->addEntity(shape,name,subtype,ids,type);
        if(group<0) group=item->index;
        item->rebuildGroup=group;
    }
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
