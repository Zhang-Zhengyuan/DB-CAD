#include "mainwindow_internal.h"

void MainWindow::setPGConnectInfo() {
    auto values = read_config_lines("pg_connect_info.conf", 5);
    QDialog dlg(this);
    dlg.setWindowTitle(tr("设置 PostgreSQL 连接信息"));
    dlg.setWindowFlags(dlg.windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    QFormLayout form(&dlg);
    QLineEdit hostEdit;
    hostEdit.setText(QString::fromStdString(values[0]));
    hostEdit.setPlaceholderText("127.0.0.1");
    QLineEdit portEdit;
    portEdit.setText(QString::fromStdString(values[1]));
    portEdit.setPlaceholderText("5432");
    QLineEdit userEdit;
    userEdit.setText(QString::fromStdString(values[2]));
    userEdit.setPlaceholderText("postgres");
    QLineEdit passEdit;
    passEdit.setText(QString::fromStdString(values[3]));
    passEdit.setEchoMode(QLineEdit::Password);
    passEdit.setPlaceholderText("(password)");
    QLineEdit dbEdit;
    dbEdit.setText(QString::fromStdString(values[4]));
    dbEdit.setPlaceholderText("dbcad_demo");
    form.addRow(tr("Host:"), &hostEdit);
    form.addRow(tr("Port:"), &portEdit);
    form.addRow(tr("User:"), &userEdit);
    form.addRow(tr("Password:"), &passEdit);
    form.addRow(tr("Database:"), &dbEdit);

    QDialogButtonBox bb(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    form.addRow(&bb);
    QObject::connect(&bb, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&bb, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted)
        return;

    QString err;
    if (!write_config_lines("pg_connect_info.conf",
                            {hostEdit.text().toStdString(), portEdit.text().toStdString(),
                             userEdit.text().toStdString(), passEdit.text().toStdString(), dbEdit.text().toStdString()},
                            &err)) {
        QMessageBox::warning(this, tr("DBCAD"), err);
        return;
    }
    m_pgService = std::make_unique<PgService>();
    connect(m_pgService.get(), &PgService::initFailed, this,
            [](const QString &err) { QMessageBox::warning(nullptr, tr("PostgreSQL 初始化失败"), err); });
    connect(m_pgService.get(), &PgService::saved, this, &MainWindow::onPgSaved);
    connect(m_pgService.get(), &PgService::saveFailed, this, &MainWindow::onPgSaveFailed);
    connect(m_pgService.get(), &PgService::loadFailed, this, &MainWindow::onPgLoadFailed);
    connect(m_pgService.get(), &PgService::listed, this, &MainWindow::onPgListed);
    connect(m_pgService.get(), &PgService::listFailed, this, &MainWindow::onPgListFailed);
    connect(m_pgService.get(), &PgService::counted, this, &MainWindow::onPgCounted);
    connect(m_pgService.get(), &PgService::countFailed, this, &MainWindow::onPgCountFailed);
    connect(m_pgService.get(), &PgService::deleted, this, &MainWindow::onPgDeleted);
    connect(m_pgService.get(), &PgService::deleteFailed, this, &MainWindow::onPgDeleteFailed);

    if (m_pgService->isInitialized()) {
        statusBar()->showMessage(tr("PostgreSQL 连接信息已更新"), 3000);
    } else {
        statusBar()->showMessage(tr("PostgreSQL 配置已保存，但初始化失败"), 4000);
    }
    updatePgMenuState();
}

void MainWindow::pgSaveCurrentToDatabase() {
    if (curWindow == nullptr) {
        QMessageBox::information(this, tr("DBCAD"), tr("请先打开或绘制一个实体，再保存。"));
        return;
    }
    if (!m_pgService->isInitialized()) {
        QMessageBox::warning(this, tr("DBCAD"), tr("PostgreSQL 未初始化，请检查连接配置。"));
        return;
    }

    bool ok = false;
    const QString defaultName = QString("dbcad_pg_%1").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));
    const QString name = QInputDialog::getText(
        this, tr("保存到 PostgreSQL"), tr("请输入零件名(将作为 dbcad_pg_demo.name，唯一):"), QLineEdit::Normal,
        defaultName, &ok, this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    if (!ok || name.trimmed().isEmpty())
        return;

    QString satText;
    try { satText=QString::fromUtf8(dbcad::kernel().writeNative(curWindow->getEntityList())); }
    catch(const std::exception& e) { showMessage(QString::fromUtf8(e.what()),5000); return; }

    m_pgService->saveSat(name, satText);
    statusBar()->showMessage(tr("正在保存到 PostgreSQL..."), 3000);
}

void MainWindow::pgLoadFromDatabaseToCurrent() {
    if (!m_pgService->isInitialized()) {
        QMessageBox::warning(this, tr("DBCAD"), tr("PostgreSQL 未初始化，请检查连接配置。"));
        return;
    }

    bool ok = false;
    const QString name =
        QInputDialog::getText(this, tr("从 PostgreSQL 加载"), tr("请输入要加载的零件名:"), QLineEdit::Normal, QString(),
                              &ok, this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    if (!ok || name.trimmed().isEmpty())
        return;

    // Scope only the per-request callbacks; retain the permanent error handler.
    auto requestContext=new QObject(this);
    connect(
        m_pgService.get(), &PgService::loaded, requestContext,
        [this, name, requestContext](const QString &satText) {
            requestContext->deleteLater();

            if (curWindow == nullptr)
                addWindow();
            dbcad::ShapeList shapes;
            try { shapes=dbcad::kernel().readNative(satText.toUtf8()); }
            catch(const std::exception& e) { showMessage(QString::fromUtf8(e.what()),5000); return; }
            curWindow->clear();
            for(size_t i=0;i<shapes.size();++i) curWindow->addEntity(shapes[i],tr("PG 导入实体%1").arg(i).toStdString(),-1);
            curWindow->resetDocumentHistory();
            QMessageBox::information(this, tr("DBCAD"), tr("已从 PostgreSQL 加载:\n%1").arg(name));
            statusBar()->showMessage(tr("已从 PostgreSQL 加载: %1").arg(name), 3000);
        },
        Qt::SingleShotConnection);

    connect(m_pgService.get(), &PgService::loadFailed, requestContext,
            [requestContext](const QString&) { requestContext->deleteLater(); }, Qt::SingleShotConnection);

    m_pgService->loadSat(name);
}

void MainWindow::pgListPartsInDatabase() {
    if (!m_pgService->isInitialized()) {
        QMessageBox::warning(this, tr("DBCAD"), tr("PostgreSQL 未初始化，请检查连接配置。"));
        return;
    }
    m_pgService->listParts(200);
}

void MainWindow::onPgSaved(const QString &name, qint64 sizeBytes, qint64) {
    QMessageBox::information(this, tr("DBCAD"),
                             tr("已保存到 PostgreSQL:\n%1\n大小: %2 bytes").arg(name).arg(sizeBytes));
    statusBar()->showMessage(tr("已保存到 PostgreSQL: %1").arg(name), 3000);
}

void MainWindow::onPgSaveFailed(const QString &error) {
    QMessageBox::warning(this, tr("DBCAD"), tr("保存到 PostgreSQL 失败:\n%1").arg(error));
    statusBar()->showMessage(tr("保存到 PostgreSQL 失败"), 4000);
}

void MainWindow::onPgLoadFailed(const QString &error) {
    QMessageBox::warning(this, tr("DBCAD"), tr("从 PostgreSQL 加载失败:\n%1").arg(error));
    statusBar()->showMessage(tr("从 PostgreSQL 加载失败"), 4000);
}

void MainWindow::onPgCounted(qint64 count) {
    statusBar()->showMessage(tr("PostgreSQL 已存零件数: %1").arg(count), 3000);
}

void MainWindow::onPgCountFailed(const QString &error) {
    QMessageBox::warning(this, tr("DBCAD"), tr("统计 PostgreSQL 零件数失败:\n%1").arg(error));
    statusBar()->showMessage(tr("统计 PostgreSQL 零件数失败"), 4000);
}

void MainWindow::onPgDeleted(const QString &name) {
    statusBar()->showMessage(tr("已从 PostgreSQL 删除: %1").arg(name), 3000);
}

void MainWindow::onPgDeleteFailed(const QString &error) {
    QMessageBox::warning(this, tr("DBCAD"), tr("从 PostgreSQL 删除失败:\n%1").arg(error));
    statusBar()->showMessage(tr("从 PostgreSQL 删除失败"), 4000);
}

void MainWindow::onPgListed(const QVector<QString> &names, const QVector<qint64> &, const QVector<qint64> &sizes,
                            const QVector<QString> &updatedAts) {
    if (names.isEmpty()) {
        QMessageBox::information(this, tr("PostgreSQL 已存零件"), tr("数据库中暂无零件。"));
        return;
    }
    QString msg;
    for (int i = 0; i < names.size(); ++i) {
        msg += tr("%1 | %2 bytes | %3\n").arg(names[i], -30).arg(sizes[i]).arg(updatedAts[i]);
    }
    QMessageBox::information(this, tr("PostgreSQL 已存零件 (%1 个)").arg(names.size()), msg);
    statusBar()->showMessage(tr("PostgreSQL 已存零件列表已显示"), 2000);
}

void MainWindow::onPgListFailed(const QString &error) {
    QMessageBox::warning(this, tr("DBCAD"), tr("列出 PostgreSQL 零件失败:\n%1").arg(error));
}

void MainWindow::updatePgMenuState() {
    const bool ready = (m_pgService != nullptr && m_pgService->isInitialized());
    if (pgSaveAct != nullptr)
        pgSaveAct->setEnabled(ready);
    if (pgLoadAct != nullptr)
        pgLoadAct->setEnabled(ready);
    if (pgListAct != nullptr)
        pgListAct->setEnabled(ready);
    if (pgCountAct != nullptr)
        pgCountAct->setEnabled(ready);
    if (pgDeleteAct != nullptr)
        pgDeleteAct->setEnabled(ready);
}

void MainWindow::pgCountPartsInDatabase() {
    if (m_pgService == nullptr || !m_pgService->isInitialized()) {
        QMessageBox::warning(this, tr("DBCAD"), tr("PostgreSQL 未初始化，请检查连接配置。"));
        return;
    }
    m_pgService->countParts();
}

void MainWindow::pgDeleteByNameFromDatabase() {
    if (m_pgService == nullptr || !m_pgService->isInitialized()) {
        QMessageBox::warning(this, tr("DBCAD"), tr("PostgreSQL 未初始化，请检查连接配置。"));
        return;
    }

    bool ok = false;
    const QString name =
        QInputDialog::getText(this, tr("从 PostgreSQL 删除"), tr("请输入要删除的零件名:"), QLineEdit::Normal, QString(),
                              &ok, this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    if (!ok || name.trimmed().isEmpty())
        return;

    if (QMessageBox::question(this, tr("从 PostgreSQL 删除"),
                              tr("确定删除 PostgreSQL 中的零件 “%1” 吗？此操作不可撤销。").arg(name),
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    m_pgService->deleteByName(name);
}
