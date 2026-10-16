#pragma once

#include <QHash>
#include <QList>
#include <QMainWindow>
#include <QSessionManager>
#include <QString>
#include <QtGlobal>
#include <memory>
#include <QColor>
#include "kernel/model_types.h"
#include "pg_service.h"

class Window;
class QWebSocket;
class QProcess;
class QTimer;
class QLabel;
class QListWidget;
class QCheckBox;
class QPushButton;
class QDockWidget;
class QComboBox;

const QStringList colorNames = QColor::colorNames();

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr, Qt::WindowFlags flags = {});

    // ========== 增量协作支持 ==========
    // 变更类型枚举
    enum class EntityChangeType { ADD, REMOVE, MODIFY };

    // 实体变更结构体
    struct EntityChange {
        QString uuid;
        QString name;
        QString entityType;
        EntityChangeType changeType;
        int entityIndex;
        qint64 timestamp;
        // Geometry belongs to the component graph; this record carries metadata only.

    };

    // 增量协作方法
    void beginEntityChangeTracking();
    void recordEntityAdded(const QString& uuid, const QString& name, const QString& entityType, int index);
    void recordEntityRemoved(int index);
    void recordEntityModified(int index);
    QList<EntityChange> endEntityChangeTracking();
    void clearEntityChanges();
    // 协作友好的本地删除：UI 右键删除按钮调用，内核删除、变更记账
    void deleteEntityByIndexForCollaboration(int index);
    QString exportEntityChangesToJson(const QList<EntityChange>& changes);

    // Legacy WebSocket delta apply, retained for old project history.

    // Component encoding stays on the client; the server merges opaque JSON.
    bool submitMode1JsonDelta(const QString& reason);
    void pullMode1JsonDelta();

    // Reconstruct through the kernel codec, then merge by stable document UUID.
    bool pullEntityGraph(int version, const QJsonObject& graph, bool deltaGraph = false,
                         const QStringList& removedIds = {}, const QHash<QString, QString>& remoteNames = {});

    // ========== 其他 public 方法 ==========
    void setCurWindow(Window* w) { curWindow = w; }
    void showMessage(const QString s, int duration = -1);
    void notifyModelChangedForCollaboration();


protected:
    void loadFile(const QString& fileName);
    void loadFile(const QString& partName, const int generation);
    void closeEvent(QCloseEvent* event) override;

private slots:
    void addWindow();
    void open();
    void undo();
    void redo();
    bool save();
    void saveEntity();
    void saveDebugFile();
    void saveImage();
    bool saveAs();
    void about();
    void runTest();
    void incrementalTest();
    void setNEO4JConnectInfo();
    void setFastAPIConnectInfo();
    void pgSaveCurrentToDatabase();
    void pgLoadFromDatabaseToCurrent();
    void pgListPartsInDatabase();
    void pgCountPartsInDatabase();
    void pgDeleteByNameFromDatabase();
    void setPGConnectInfo();
    void onPgSaved(const QString& name, qint64 sizeBytes, qint64 id);
    void onPgSaveFailed(const QString& error);
    void onPgLoadFailed(const QString& error);
    void onPgListed(const QVector<QString>& names,
                    const QVector<qint64>& ids,
                    const QVector<qint64>& sizes,
                    const QVector<QString>& updatedAts);
    void onPgListFailed(const QString& error);
    void onPgCounted(qint64 count);
    void onPgCountFailed(const QString& error);
    void onPgDeleted(const QString& name);
    void onPgDeleteFailed(const QString& error);
    void visiablility();
    void displayInfo();
#ifndef QT_NO_SESSIONMANAGER
    void commitData(QSessionManager&);
#endif
    void insertElements(const OPERATOR_TYPES ot, const int subOperatorType);
    void operation(const OPERATOR_TYPES ot, const int subOperatorType);
    void clear();

private:
    void createActions();
    void createStatusBar();
    bool maybeSave();
    bool saveFile(const QString& fileName);
    void updateCollabPanelUi();
    void setCollabConnectionState(const QString& stateText);
    void reconnectFastAPISync();
    void disconnectFastAPISync();
    void handleFastAPISyncMessage(const QString& message);
    void handlePresenceAndAckEvent(const QJsonObject& root, const QString& messageType);
    void handleDeltaSyncEvent(const QJsonObject& root, const QString& messageType);
    void handleFastAPISyncMessageImpl(const QString& message);
    void requestFastAPISyncNow();
    void updateFastAPICollaboratorsStatus();
    void setCurrentFile(const QString& fileName);
    void setCurrentPartName(const QString& partName);
    void onCollabPushButtonClicked();
    void onCollabPullButtonClicked();

    Window* curWindow;
    QAction* setACISModeAct;
    QAction* setNEO4JModeAct;
    QAction* setNEO4JIncrementalModeAct;
    QAction* setFASTAPIModeAct;
    QActionGroup* setModeActGroup;
    std::string neo4jdb_host;
    int neo4jdb_port_bolt;
    std::string neo4jdb_username;
    std::string neo4jdb_password;
    std::string fastapi_base_url;
    std::string fastapi_author;
    std::string fastapi_password;
    QString fastapi_project_id;
    QString fastapi_project_name;
    QString fastapi_client_id;
    // Mirror fields — synced from CollabSession via mirrorToLegacy(). Do not write directly.
    int fastapi_model_version = 0;
    int fastapi_pending_remote_version = 0;
    bool fastapiApplyingRemoteSnapshot = false;
    bool fastapiSubmitInFlight = false;
    bool fastapiLocalDirtyDuringSubmit = false;
    bool fastapiLocalDirty = false;
    QString fastapiLastPublishReason;
    QString fastapiPendingSubmitRequestId;
    // Presence and WebSocket infrastructure.
    QHash<QString, QString> fastapi_collaborators;
    QWebSocket* fastapiSyncSocket = nullptr;
    QTimer* fastapiHeartbeatTimer = nullptr;
    QTimer* fastapiReconnectTimer = nullptr;

    QDockWidget* collabDock = nullptr;
    QLabel* collabConnectionLabel = nullptr;
    QLabel* collabProjectLabel = nullptr;
    QLabel* collabVersionLabel = nullptr;
    QLabel* collabPendingLabel = nullptr;
    QListWidget* collabMembersList = nullptr;
    QPushButton* collabSyncNowButton = nullptr;
    QPushButton* collabPushButton = nullptr;
    QPushButton* collabPullButton = nullptr;
    QPushButton* collabReconnectButton = nullptr;


    std::unique_ptr<PgService> m_pgService;
    // PG menu actions — kept as members so they can be enabled/disabled
    // based on m_pgService->isInitialized().
    QAction* pgSaveAct = nullptr;
    QAction* pgLoadAct = nullptr;
    QAction* pgListAct = nullptr;
    QAction* pgCountAct = nullptr;
    QAction* pgDeleteAct = nullptr;
    void updatePgMenuState();

    // 增量协作成员变量
    QHash<int, QString> entityIndexToUuid;
    QList<EntityChange> pendingEntityChanges;
    bool isTrackingEntityChanges = false;
    qint64 entityChangeTrackingStartTime = 0;
};
