#include "mainwindow_internal.h"
#include <QScopeGuard>

bool MainWindow::saveFile(const QString& fileName) {
    if (usingCollabStorage()) {
        // FastAPI 项目保存：编排（换名建项/收录全部实体/等待通道/推送）在控制器。
        const bool wasEnabled = isEnabled();
        setEnabled(false);
        const auto outcome = collabController_->saveToProject(fileName);
        setEnabled(wasEnabled);
        updateCollabPanelUi();
        displayOutcome(outcome);
        if (outcome.ok)
            setCurrentPartName(fileName);
        return outcome.ok;
    }

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);

    // SAT 文件（本地）：经 SatFileBackend 全量写。
    QString errorMessage;
    try {
        dbcad::SaveRequest request;
        request.satPayload = dbcad::kernel().writeNative(curWindow->getEntityList());
        QString error;
        if (!satBackend_->save(fileName, request, &error))
            errorMessage = error;
    } catch (const std::exception& e) {
        errorMessage = QString::fromUtf8(e.what());
    }

    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, tr("DBCAD"), errorMessage);
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
