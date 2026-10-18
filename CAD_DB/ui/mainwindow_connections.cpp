#include "mainwindow_internal.h"

void MainWindow::setFastAPIConnectInfo() {
    auto values = read_config_lines("fastapi_connect_info.conf", 3);
    QString baseUrl = collabBackend_->baseUrl();
    QString author = collabBackend_->author();
    QString password;
    if (!values.empty()) {
        if (!values[0].empty()) baseUrl = QString::fromStdString(values[0]);
        if (!values[1].empty()) author = QString::fromStdString(values[1]);
        password = QString::fromStdString(values[2]);
    }

    QDialog dialog(this);
    dialog.setWindowTitle(tr("FastAPI连接配置"));
    dialog.setMinimumSize(420, 220);
    QFormLayout form(&dialog);

    QLineEdit baseUrlEdit(&dialog);
    baseUrlEdit.setPlaceholderText("http://127.0.0.1:8000");
    baseUrlEdit.setText(baseUrl);
    form.addRow(tr("FastAPI地址:"), &baseUrlEdit);

    QLineEdit authorEdit(&dialog);
    authorEdit.setPlaceholderText("dbcad-exe");
    authorEdit.setText(author);
    form.addRow(tr("作者名:"), &authorEdit);

    QLineEdit passwordEdit(&dialog);
    passwordEdit.setEchoMode(QLineEdit::Password);
    passwordEdit.setPlaceholderText(tr("可留空（后端未启用密码时）"));
    passwordEdit.setText(password);
    form.addRow(tr("连接密码:"), &passwordEdit);

    QPushButton testButton(tr("测试连接"), &dialog);
    form.addRow(&testButton);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    QObject::connect(&testButton, &QPushButton::clicked, &dialog, [&]() {
        const QString url = baseUrlEdit.text().trimmed();
        if (url.isEmpty()) {
            QMessageBox::warning(this, tr("FastAPI连接测试"), tr("FastAPI地址不能为空"));
            return;
        }

        dbcad::CollabBackend probe;
        probe.configure(url, authorEdit.text().trimmed(), passwordEdit.text());
        probe.testConnection([this](bool ok, const QString& error) {
            if (!ok) {
                QMessageBox::warning(this, tr("FastAPI连接测试"), tr("连接失败：%1").arg(error));
            } else {
                QMessageBox::information(this, tr("FastAPI连接测试"), tr("连接成功。"));
            }
        });
    });

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    const QString newBaseUrl = baseUrlEdit.text().trimmed();
    if (newBaseUrl.isEmpty()) {
        QMessageBox::warning(this, tr("FastAPI连接配置"), tr("FastAPI地址不能为空"));
        return;
    }

    const std::string newAuthor = authorEdit.text().trimmed().toStdString();
    const std::string newPassword = passwordEdit.text().trimmed().toStdString();
    const std::string newBaseUrlStd = newBaseUrl.toStdString();
    collabBackend_->configure(newBaseUrl, authorEdit.text().trimmed(), passwordEdit.text().trimmed());

    QString writeError;
    if (!write_config_lines("fastapi_connect_info.conf", {newBaseUrlStd, newAuthor, newPassword}, &writeError)) {
        QMessageBox::warning(this, tr("FastAPI连接配置"), writeError);
        return;
    }

    statusBar()->showMessage(tr("FastAPI连接信息已更新"), 2000);

    // 保存连接信息后，如果有项目ID则重新建立WebSocket连接
    if (!collabController_->projectId().isEmpty() && collabBackend_->isConfigured()) {
        reconnectFastAPISync();
    }
}
