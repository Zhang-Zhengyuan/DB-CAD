#pragma once

#include <QHash>
#include <QList>
#include <QMainWindow>
#include <QSessionManager>
#include <QString>
#include <QtGlobal>
#include <memory>
#include "access.hxx"
#include "pg_service.h"

enum OPERATOR_TYPES { OPERATOR_IMPORT = 0, OPERATOR_CONSTRUCTOR, OPERATOR_INTERSECTOR, OPERATOR_BOOLEAN, OPERATOR_DEFEATURE, OPERATOR_UNKNOWN, OPERATOR_SPERATOR = 999 };
enum DEFEATURE_TYPES { DEFEATURE_UNREPEAT = 0, DEFEATURE_REMOVE_SMALL, DEFEATURE_SHELL, DEFEATURE_SPLIT, DEFEATURE_ENVELOPE, DEFEATURE_BREP2CSG };
enum INPUT_MODES { SELECTION, INPUT_SPLINE };
// 注意：BASIC_ENTITIES中非样条基本体需放在BSPLINE_CTRLPTS之前
// 否则将导致进行变换时未修改ENTITY_TREE_ITEM的trans。
enum BASIC_ENTITIES {
    E_CUBE = 0,         // 立方体
    E_SPHERE,           // 球体
    E_CYLINDER,         // 圆柱体
    E_TORUS,            // 圆环体
    E_CONE,             // 圆锥体
    E_PRISM,            // 多棱锥
    E_PYRAMID,          // 金字塔
    E_WIGGLE,           // 摆动体
    E_PLANE,            // 矩形面
    E_PPLANE,           // 平行四边形面
    E_CPLANE,           // 圆形面
    E_SPHERES,          // 球面
    E_PSPHERES,         // 部分球面
    E_CONIC_SIDE,       // 圆锥面
    E_CYLINDER_SIDE,    // 圆柱面
    E_PCONIC,           // 部分圆锥面
    E_TORUSS,           // 完整圆环面
    E_PTORUSS,          // 部分圆环面
    E_LAWS,             // 公式面
    E_LINE,             // 直线
    E_ARC,              // 圆弧
    E_ELLIPSE,          // 椭圆
    E_CONIC,            // 二次线
    E_HELIX,            // 螺旋线
    E_SPIRAL,           // 阿基米德螺旋线
    E_SPRING,           // 弹簧线
    E_LAWC,             // 方程线
    E_BSPLINE_CTRLPTS,  // 控制点b样条
    E_BSPLINE_FIT,      // 拟合b样条
    E_BSPLINE_INTRP,    // 插值b样条
    E_BEZIER,           // 贝塞尔
    E_BSPLINE           // b样条
};

class ENTITY_LIST;
class AcisOptions;
class outcome;
class Window;
class HISTORY_STREAM;
class DELTA_STATE;
class BULLETIN_BOARD;
class QWebSocket;
class QProcess;
class QTimer;
class QLabel;
class QListWidget;
class QCheckBox;
class QPushButton;
class QDockWidget;

typedef outcome(*GME_fp)(ENTITY_LIST&, AcisOptions*);

struct menu_map {
    std::string name;
    int value;
};

extern std::unordered_map<std::string, std::vector<menu_map>> menus_example;

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
        // 仅 ADD 变更有效：把该 body 单独序列化的 SAT 文本，供接收端 acis_restore_entity_list 后 addEntity。
        QString sat;
    };

    // 增量协作方法
    void beginEntityChangeTracking();
    void recordEntityAdded(const QString& uuid, const QString& name, const QString& entityType, int index, const QString& sat = QString());
    void recordEntityRemoved(int index);
    void recordEntityModified(int index);
    QList<EntityChange> endEntityChangeTracking();
    void clearEntityChanges();
    // 协作友好的本地删除：UI 右键删除按钮调用，ACIS 真删 + 记账 + 调度 Push
    void deleteEntityByIndexForCollaboration(int index);
    QString exportEntityGraphToJson();
    QString exportEntityChangesToJson(const QList<EntityChange>& changes);
    bool submitEntityGraphIncremental(const QString& entityGraphJson, const QString& changesJson, const QString& reason);
    bool applyRemoteEntityGraphIncremental(const QString& remoteEntityGraphJson, const QString& remoteChangesJson, const QString& reason);

    // ========== Neo4j Entity Graph 协作方法 ==========
    // 推送：序列化完整 ACIS entity graph → POST 到 Python neo4j_entity_store
    // 成功后通过 WebSocket "submit_entity_graph" 广播 SAT fallback
    bool submitACISEntityGraph(const QString& reason);

    // ========== 增量 delta Push / Pull（接入 access 模块） ==========
    // 增量 Push：基于 ACIS delta_state 计算 body 变更，只上传真正变化的部分
    bool submitIncrementalDelta(const QString& reason);
    // 增量 Pull：基于 UUID 去重，不调 clear()，只 addEntity 远端独有的 bodies
    bool applyRemoteIncrementalDelta(const QJsonObject& remoteContent, QString* errorMessage);
    // 把单个 body 序列化为 SAT 文本（供增量 Push 使用）
    QString serializeBodyToSat(ENTITY* body);
    // 把远端 SAT 文本增量 restore 到画布（不 clear，UUID 去重）
    int restoreRemoteDeltaSat(const QString& remoteSat, const QJsonObject& collabSnapshot, QString* errorMessage);

    // 拉取：从 Python neo4j_entity_store 拉取 entity graph（已改为走 storage_bridge content_text）
    // 优先走 entity graph 路径；失败则用 SAT fallback
    bool pullACISEntityGraph(int version, const QJsonObject& entityGraphJson);

    // ========== 其他 public 方法 ==========
    void setCurWindow(Window* w) { curWindow = w; }
    void showMessage(const QString s, int duration = -1);
    void notifyModelChangedForCollaboration();
    void buildTreeFromHistroy(HISTORY_STREAM* hs = nullptr);

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
    void setBridgeConnectInfo();
    void toggleBridgeMode(bool checked);
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
    bool restoreFastAPIModelFromSat(const QString& satContent);
    bool applyRemoteSatSnapshot(const QString& satContent, const QString& reason);
    bool syncFastAPIRemoteVersion(int remoteVersion, const QString& reason);
    bool applyFastAPIRemoteSat(int remoteVersion, const QString& satContent, const QString& reason);
    void updateCollabPanelUi();
    void setCollabConnectionState(const QString& stateText);
    void reconnectFastAPISync();
    void disconnectFastAPISync();
    void handleFastAPISyncMessage(const QString& message);
    void handleFastAPISyncMessageImpl(const QString& message);
    void requestFastAPISyncNow();
    void updateFastAPICollaboratorsStatus();
    void applyPendingRemoteVersion();
    void scheduleFastAPIAutoPublish(const QString& reason);
    void publishFastAPIAutoSnapshot();
    bool publishFastAPIModelSnapshot(bool interactiveConflict);
    bool submitFastAPIModelOverSocket(const QString& satContent, const QString& reason, bool interactiveConflict);
    bool exportCurrentModelToSat(QString* satContent, QString* errorMessage);
    void setCurrentFile(const QString& fileName);
    void setCurrentPartName(const QString& partName);
    void onCollabPushButtonClicked();
    void onCollabPullButtonClicked();

    Window* curWindow;
    QAction* setACISModeAct;
    QAction* setNEO4JModeAct;
    QAction* setNEO4JIncrementalModeAct;
    QAction* setFASTAPIModeAct;
    QAction* toggleBridgeModeAct = nullptr;
    QActionGroup* setModeActGroup;
    QProcess* bridgeProcess = nullptr;
    bool bridgeStopRequested = false;
    std::string bridge_bind_host;
    int bridge_bind_port = 8100;
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
    bool fastapiPublishingSnapshot = false;
    bool fastapiSubmitInFlight = false;
    bool fastapiLocalDirtyDuringSubmit = false;
    bool fastapiLocalDirty = false;
    QString fastapiLastPublishReason;
    QString fastapiPendingSubmitRequestId;
    // submit_accepted 里的 requestId，用于 model_saved 时识别本端推送。
    // 因为 SAT fallback 路径 entity_graph ack 先到清空 submitRequestId，所以独立保存。
    QString fastapiLastAcceptedRequestId;
    // 协作基础设施（非状态，不归 CollabSession 管）
    // Conflict resolution 备份：submit_rejected 时备份本地 SAT 内容，Pull 后 offer 给用户恢复。
    QString fastapiConflictLocalSatBackup;
    // 防止 Pull 期间重复弹出 merge 对话框。
    bool isShowingConflictMergeDialog = false;
    QHash<QString, QString> fastapi_collaborators;
    QWebSocket* fastapiSyncSocket = nullptr;
    QTimer* fastapiSyncTimer = nullptr;
    QTimer* fastapiHeartbeatTimer = nullptr;
    QTimer* fastapiReconnectTimer = nullptr;
    QTimer* fastapiPublishTimer = nullptr;
    bool fastapiAutoFollowRemote = true;

    QDockWidget* collabDock = nullptr;
    QLabel* collabConnectionLabel = nullptr;
    QLabel* collabProjectLabel = nullptr;
    QLabel* collabVersionLabel = nullptr;
    QLabel* collabPendingLabel = nullptr;
    QListWidget* collabMembersList = nullptr;
    QCheckBox* collabAutoFollowCheckBox = nullptr;
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
    // Access 模块增量 delta 追踪（Push 时计算 delta，Pull 时做 UUID 去重）
    IncrementalContext collabCtx;
};
