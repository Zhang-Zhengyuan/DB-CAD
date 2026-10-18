#include "mainwindow_internal.h"

void MainWindow::loadFile(const QString &fileName) {
    if (curWindow == nullptr)
        addWindow();

    if (!usingCollabStorage()) {
        // SAT 文件（本地）：快 IO，同步经 SatFileBackend 打开（回调内联完成）。
        bool isRead = false;
#ifndef QT_NO_CURSOR
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        satBackend_->open(fileName, [this, &fileName, &isRead](const dbcad::StorageOpenResult& result) {
            if (!result.ok) {
                showMessage(result.error, 5000);
                return;
            }
            curWindow->clear();
            documentSession_->discardPendingChanges();
            documentSession_->clearIndexToUuid();
            for (size_t i = 0; i < result.document.shapes.size(); ++i)
                curWindow->addEntity(result.document.shapes[i], tr("导入实体%1").arg(i).toStdString(), -1);
            isRead = true;
        });
#ifndef QT_NO_CURSOR
        QGuiApplication::restoreOverrideCursor();
#endif

        if (isRead) {
            curWindow->resetDocumentHistory();
            curWindow->setIsModified(false);
            collabController_->leaveProject();
            setCurrentFile(fileName);
            statusBar()->showMessage(tr("文件已导入"), 2000);
        }
        return;
    }

    // FastAPI 协作项目：异步打开（M2.1），完成回调在 handleOpenProjectFinished。
    if (collabController_->isBusy()) {
        showMessage(collabController_->busyMessage(), 3000);
        return;
    }
    pendingOpenName_ = fileName;
#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    collabController_->openProjectAsync(fileName);
}

void MainWindow::loadFile(const QString &partName, const int generation) {
    if (curWindow == nullptr)
        addWindow();

    if (collabController_->isBusy()) {
        showMessage(collabController_->busyMessage(), 3000);
        return;
    }
    pendingOpenName_ = partName;
#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    collabController_->openProjectAtAsync(partName, generation);
}
