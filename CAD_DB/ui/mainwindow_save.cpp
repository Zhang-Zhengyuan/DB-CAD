#include "mainwindow_internal.h"

bool MainWindow::saveFile(const QString& fileName) {
    if (usingCollabStorage()) {
        // FastAPI 项目保存：异步编排（换名建项/收录全部实体/等待通道/推送）。
        // 结果经 saveToProjectFinished 信号返回；closeEvent 在操作在飞时拒绝退出。
        if (collabController_->isBusy()) {
            showMessage(collabController_->busyMessage(), 3000);
            return false;
        }
        pendingSaveName_ = fileName;
        collabController_->saveToProjectAsync(fileName);
        return true; // 已启动；失败经状态栏/对话框反馈
    }

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);

    // SAT 文件（本地）：经 SatFileBackend 全量写（回调内联完成）。
    QString errorMessage;
    dbcad::SaveRequest request;
    try {
        request.satPayload = dbcad::kernel().writeNative(curWindow->getEntityList());
        satBackend_->save(fileName, request, [&errorMessage](const dbcad::StorageSaveResult& result) {
            if (!result.ok)
                errorMessage = result.error;
        });
    } catch (const std::exception& e) {
        errorMessage = QString::fromUtf8(e.what());
    }

    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty()) {
        showErrorDialog(errorMessage);
        return false;
    }
    curWindow->setIsModified(false);

    collabController_->leaveProject();
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("文件已保存"), 2000);
    updateCollabPanelUi();
    return true;
}

void MainWindow::setCurrentFile(const QString& fileName) {
    curWindow->setCurFile(fileName);
    curWindow->setWindowModified(false);
}

void MainWindow::setCurrentPartName(const QString& fileName) {
    curWindow->setCurPartName(fileName);
    curWindow->setWindowModified(false);
}

void MainWindow::showMessage(const QString s, int duration) {
    if (duration == -1)
        statusBar()->showMessage(s);
    else
        statusBar()->showMessage(s, duration);
}

#ifndef QT_NO_SESSIONMANAGER
void MainWindow::commitData(QSessionManager& manager) {
    if (manager.allowsInteraction()) {
        if (!maybeSave())
            manager.cancel();
    } else {
        // 自动保存
        if (curWindow != nullptr && curWindow->getIsModified())
            save();
    }
}
#endif
