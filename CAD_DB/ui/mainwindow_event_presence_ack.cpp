#include "mainwindow_internal.h"

// Preserve state transitions and version alignment for this message family.
void MainWindow::handlePresenceAndAckEvent(const QJsonObject &root, const QString &messageType) {
    if (messageType == "presence_snapshot") {
        fastapi_collaborators.clear();
        const QJsonArray members = root.value("members").toArray();
        for (const auto &memberValue : members) {
            if (!memberValue.isObject()) {
                continue;
            }
            const QJsonObject member = memberValue.toObject();
            const QString memberClientId = member.value("client_id").toString();
            if (memberClientId.isEmpty()) {
                continue;
            }
            fastapi_collaborators.insert(memberClientId, member.value("author").toString());
        }
        updateFastAPICollaboratorsStatus();
        return;
    }

    if (messageType == "collaborator_joined") {
        const QString memberClientId = root.value("client_id").toString();
        if (!memberClientId.isEmpty()) {
            fastapi_collaborators.insert(memberClientId, root.value("author").toString());
            updateFastAPICollaboratorsStatus();
        }
        return;
    }

    if (messageType == "collaborator_left") {
        const QString memberClientId = root.value("client_id").toString();
        if (!memberClientId.isEmpty()) {
            fastapi_collaborators.remove(memberClientId);
            updateFastAPICollaboratorsStatus();
        }
        return;
    }

    if (messageType == "pong") {
        return;
    }

    if (messageType == "error") {
        statusBar()->showMessage(tr("协作通道错误：%1").arg(root.value("detail").toString()), 5000);
    }
}
