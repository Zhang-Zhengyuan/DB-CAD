#pragma once

#include <QString>

#include <QtGlobal>
#include <QUuid>

// CollabSession — 协作状态机
//
// 状态机持有所有协作状态（版本号、提交中标记等），是协作状态的唯一真相源。
// MainWindow 通过决策接口（tryBegin*）询问能否操作，通过转移函数（on*）上报事件。
// 旧字段 fastapi_* 保留为 mirror，由 mirrorToLegacy() 同步，禁止直接写入。

class CollabSession {
public:
    enum class State {
        Disconnected,
        Connected_NoProject,
        Connected_Idle,
        Connected_LocalDirty,
        Connected_SubmitInFlight,
        Connected_SubmitInFlight_Dirty,
        Connected_RemotePending_Dirty,
        Connected_ApplyingRemote,
        Connected_PublishingDirect
    };

    enum class Event {
        WsMessage,
        UserEdit,
        UserEditDuringInFlight,
        SubmitStarted,
        SubmitAccepted,
        SubmitRejected,
        RemotePending,
        RemoteApplied,
        ApplyStart,
        ApplyEnd,
        HttpPublishStart,
        HttpPublishEnd,
        Reconnect,
        ProjectOpened,
        ProjectCleared,
        Disconnected,
        Bound
    };

    struct SubmitDecision {
        enum Kind {
            Allow,
            RejectNoProject,
            RejectAlreadyInFlight,
            RejectRemotePending,
            RejectApplyingRemote,
            RejectPublishingDirect
        };
        Kind kind = RejectNoProject;
        QString reason;
        int remoteVersion = 0;
        QString requestId;
    };

    struct ApplyDecision {
        enum Kind {
            Allow,
            RejectNoProject,
            RejectAlreadyApplying,
            RejectNoContent
        };
        Kind kind = RejectNoProject;
        QString reason;
        QString reasonTr;
    };

    struct PublishDecision {
        enum Kind {
            Allow,
            RejectNoProject,
            RejectAlreadyPublishing,
            RejectSubmitInFlight
        };
        Kind kind = RejectNoProject;
        QString reason;
    };

    State state() const { return state_; }
    const char* stateName() const;
    const char* eventName(Event e) const;

    // 决策接口
    SubmitDecision  tryBeginSubmit(const QString& reason);
    void            rollbackSubmit();
    ApplyDecision   tryBeginApplyRemote(int remoteVersion, const QString& /*reason*/);
    void            rollbackApply();
    PublishDecision tryBeginHttpPublish();
    void            rollbackHttpPublish();

    // 转移函数
    void onUserEdit();
    void onUserEditDuringInFlight();
    void onSubmitAccepted(int newRemoteVersion);
    void onSubmitRejected(int latestVersion);
    void onRemotePending(int remoteVersion);
    void onRemoteApplied(int appliedVersion);
    void onApplyStart();
    void onApplyEnd();
    void onHttpPublishStart();
    void onHttpPublishEnd();
    void onReconnect(bool /*projectIdValid*/);
    void onProjectOpened();
    void onProjectCleared();
    void onDisconnected();

    // 重置状态机到初始状态（用于测试）
    void reset();

    // 只读访问器
    int  modelVersion() const              { return snapshot_.modelVersion; }
    int  pendingRemoteVersion() const      { return snapshot_.pendingRemoteVersion; }
    bool isSubmitInFlight() const          { return snapshot_.submitInFlight; }
    bool isSatSubmitInFlight() const      { return snapshot_.satSubmitInFlight; }
    void setSatSubmitInFlight(bool v)     { snapshot_.satSubmitInFlight = v; }
    bool isLocalDirtyDuringSubmit() const { return snapshot_.localDirtyDuringSubmit; }
    bool isLocalDirty() const { return state_ == State::Connected_LocalDirty; }
    void clearLocalDirtyDuringSubmit() { snapshot_.localDirtyDuringSubmit = false; mirrorToLegacy(); }
    bool isApplyingRemoteSnapshot() const  { return snapshot_.applyingRemoteSnapshot; }
    void setApplyingRemoteSnapshot(bool v) { snapshot_.applyingRemoteSnapshot = v; }
    bool isPublishingSnapshot() const      { return snapshot_.publishingSnapshot; }
    const QString& submitRequestId() const       { return snapshot_.submitRequestId; }
    const QString& lastPublishReason() const    { return snapshot_.lastPublishReason; }

    // Mode1 delta 版本追踪
    int pushedVersion() const { return snapshot_.pushedVersion; }
    void setPushedVersion(int v) { snapshot_.pushedVersion = v; }

    // 项目 ID（由 legacy fastapi_project_id mirror）
    const QString& projectId() const { return snapshot_.projectId; }
    void setProjectId(const QString& id) { snapshot_.projectId = id; mirrorToLegacy(); }

    // 连接状态
    bool isConnected() const { return state_ != State::Disconnected; }

    // 状态写入接口
    void setModelVersion(int v);
    void setPendingRemoteVersion(int pv);
    void setLastPublishReason(const QString& reason);

    // 旧字段绑定（兼容层）
    void bindLegacyFields(
        QString& fastapi_project_id,
        int&    fastapi_model_version,
        int&    fastapi_pending_remote_version,
        QString& fastapiLastPublishReason,
        QString& fastapiPendingSubmitRequestId,
        bool&   fastapiSubmitInFlight,
        bool&   fastapiLocalDirtyDuringSubmit,
        bool&   fastapiApplyingRemoteSnapshot,
        bool&   fastapiPublishingSnapshot,
        bool&   fastapiLocalDirty
    );

    // 调试接口
    QString dump(Event event) const;
    void setDebugEnabled(bool enabled);
    bool isDebugEnabled() const;
    void setMinDumpIntervalMs(int intervalMs);
    int  minDumpIntervalMs() const;
    void setEventMinInterval(Event e, int intervalMs);
    int  eventMinInterval(Event e) const;

    void assertConsistent() const;
    static CollabSession& instance();

private:
    CollabSession() = default;

    struct Snapshot {
        int  modelVersion = 0;
        int  pendingRemoteVersion = 0;
        int  pushedVersion = 0;  // Mode1: 上次成功 push 的版本号
        QString projectId;       // 当前连接的 project_id
        QString submitRequestId;
        QString lastPublishReason;
        bool submitInFlight = false;
        bool satSubmitInFlight = false;
        bool localDirtyDuringSubmit = false;
        bool applyingRemoteSnapshot = false;
        bool publishingSnapshot = false;
    };

    void transition(State newState, Event event);
    void mirrorToLegacy() const;

    State state_ = State::Disconnected;
    Snapshot snapshot_;
    bool debug_enabled_ = true;
    int  min_dump_interval_ms_ = 0;
    int  event_min_interval_ms_[17] = {0};

    int*     fastapi_model_version_ref_              = nullptr;
    int*     fastapi_pending_remote_version_ref_     = nullptr;
    QString* fastapiLastPublishReason_ref_           = nullptr;
    QString* fastapiPendingSubmitRequestId_ref_      = nullptr;
    bool*    fastapiSubmitInFlight_ref_              = nullptr;
    bool*    fastapiLocalDirtyDuringSubmit_ref_      = nullptr;
    bool*    fastapiApplyingRemoteSnapshot_ref_      = nullptr;
    bool*    fastapiPublishingSnapshot_ref_          = nullptr;
    bool*    fastapiLocalDirty_ref_                  = nullptr;
    QString* fastapi_project_id_ref_                 = nullptr;

    mutable qint64 last_dump_ms_[17] = {0};

    CollabSession::State inferStateFromLegacy() const;
    void  emitDump(Event event, bool force);
    bool  isBound() const;
    static const char* stateNameFor(State s);
};


class CollabSessionApplyRemoteGuard {
public:
    CollabSessionApplyRemoteGuard();
    ~CollabSessionApplyRemoteGuard();
    CollabSessionApplyRemoteGuard(const CollabSessionApplyRemoteGuard&) = delete;
    CollabSessionApplyRemoteGuard& operator=(const CollabSessionApplyRemoteGuard&) = delete;
};

class CollabSessionPublishGuard {
public:
    CollabSessionPublishGuard();
    ~CollabSessionPublishGuard();
    CollabSessionPublishGuard(const CollabSessionPublishGuard&) = delete;
    CollabSessionPublishGuard& operator=(const CollabSessionPublishGuard&) = delete;
};