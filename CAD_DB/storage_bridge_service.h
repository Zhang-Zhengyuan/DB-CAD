#pragma once

#include <QObject>
#include <QString>

class QTcpServer;
class QTcpSocket;

class StorageBridgeService : public QObject {
public:
    StorageBridgeService(
        QString bindHost,
        int bindPort,
        QString neo4jHost,
        int neo4jPort,
        QString neo4jUser,
        QString neo4jPassword,
        QObject* parent = nullptr);

    bool start(QString& errorMessage);
    void stop();

private:
    void onNewConnection();
    struct HttpRequest {
        QString method;
        QString path;
        QByteArray body;
    };

    bool parseHttpRequest(QByteArray& buffer, HttpRequest& request);
    void processRequest(QTcpSocket* socket, const HttpRequest& request);

    void sendJson(QTcpSocket* socket, int statusCode, const QByteArray& payload);
    void sendError(QTcpSocket* socket, int statusCode, const QString& detail);

    QByteArray handleHealth() const;
    QByteArray handleCreateProject(const QByteArray& body, int& statusCode, QString& error);
    QByteArray handleGetProject(const QString& projectId, int& statusCode, QString& error);
    QByteArray handleGetProjectByName(const QString& projectName, int& statusCode, QString& error);
    QByteArray handleCreateModel(const QString& projectId, const QByteArray& body, int& statusCode, QString& error);
    QByteArray handleGetLatestModel(const QString& projectId, int& statusCode, QString& error);
    QByteArray handleGetModelVersion(const QString& projectId, int version, int& statusCode, QString& error);
    QByteArray handleListVersions(const QString& projectId, int limit, int offset, int& statusCode, QString& error);

    QString bindHost;
    int bindPort = 8100;
    QString neo4jHost;
    int neo4jPort = 7687;
    QString neo4jUser;
    QString neo4jPassword;
    QTcpServer* server = nullptr;
    int acisLevel = 0;
};
