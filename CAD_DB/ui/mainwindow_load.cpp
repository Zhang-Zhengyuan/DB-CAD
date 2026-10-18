#include "mainwindow_internal.h"

void MainWindow::loadFile(const QString &fileName) {
    if (curWindow == nullptr)
        addWindow();

    bool isRead = false;

#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    if (!usingCollabStorage()) {
        // SAT 文件（本地）：经 SatFileBackend 打开。
        QString error;
        const auto document = satBackend_->open(fileName, &error);
        if (!document.has_value()) {
            showMessage(error, 5000);
        } else {
            curWindow->clear();
            documentSession_->discardPendingChanges();
            documentSession_->clearIndexToUuid();
            for (size_t i = 0; i < document->shapes.size(); ++i)
                curWindow->addEntity(document->shapes[i], tr("导入实体%1").arg(i).toStdString(), -1);
            isRead = true;
        }
    } else {
        // FastAPI 协作项目：打开/创建由 CollabController 编排。
        const bool wasEnabled = isEnabled();
        setEnabled(false);
        const auto outcome = collabController_->openProject(fileName);
        setEnabled(wasEnabled);
        curWindow->refreshAfterDocumentChange(outcome.retiredDisplayData);
        if (outcome.ok) {
            setCurrentPartName(fileName);
            isRead = true;
        } else {
            displayOutcome(outcome);
        }
    }

    if (isRead) {
        curWindow->resetDocumentHistory();
        curWindow->setIsModified(false);
        if (!usingCollabStorage()) {
            collabController_->leaveProject();
            setCurrentFile(fileName);
            statusBar()->showMessage(tr("文件已导入"), 2000);
        } else {
            statusBar()->showMessage(tr("零件已导入(FastAPI)，版本%1").arg(collabController_->modelVersion()), 3000);
            updateCollabPanelUi();
        }
    }
#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
}

void MainWindow::loadFile(const QString &partName, const int generation) {
    if (curWindow == nullptr)
        addWindow();

#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    const bool wasEnabled = isEnabled();
    setEnabled(false);
    const auto outcome = collabController_->openProjectAt(partName, generation);
    setEnabled(wasEnabled);
    curWindow->refreshAfterDocumentChange(outcome.retiredDisplayData);

#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
    if (outcome.ok) {
        curWindow->resetDocumentHistory();
        curWindow->setIsModified(false);
        setCurrentPartName(partName);
        statusBar()->showMessage(tr("零件已导入(FastAPI)，版本%1").arg(collabController_->modelVersion()), 3000);
        updateCollabPanelUi();
    } else {
        displayOutcome(outcome);
    }
}
