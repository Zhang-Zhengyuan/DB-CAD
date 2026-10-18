#pragma once

// CollabState —— 协作版本状态机（架构 v2 核心库）。
//
// 移植自 CollabSession（refactor/architecture-v2 M1.3），按重构计划删除了
// 镜像字段模式（bindLegacyFields/mirrorToLegacy/inferStateFromLegacy）：
// 本类自身就是唯一真相源，调用方（CollabController）只读访问。
//
// 状态机持有所有协作状态（版本号、提交中标记等）。
// 决策接口（tryBeginSubmit）询问能否提交；转移函数（on*）上报事件。

#include <QDateTime>
#include <QString>
#include <QUuid>

namespace dbcad {

class CollabState {
public:
    enum class State {
        Disconnected,
        Connected_NoProject,
        Connected_Idle,
        Connected_LocalDirty,
        Connected_SubmitInFlight,
        Connected_SubmitInFlight_Dirty,
        Connected_RemotePending_Dirty,
        Connected_ApplyingRemote
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
            RejectApplyingRemote
        };
        Kind kind = RejectNoProject;
        QString reason;
        int remoteVersion = 0;
        QString requestId;
    };

    State state() const { return state_; }
    const char* stateName() const;
    const char* eventName(Event e) const;

    // 决策接口
    SubmitDecision tryBeginSubmit(const QString& reason);
    void rollbackSubmit();

    // 转移函数
    void onUserEdit();
    void onUserEditDuringInFlight();
    void onSubmitAccepted(int newRemoteVersion);
    void onRemotePending(int remoteVersion);
    void onRemoteApplied(int appliedVersion);
    void onReconnect(bool projectIdValid);
    void onDisconnected();

    // 重置状态机到初始状态（用于测试）
    void reset();

    // 只读访问器
    int modelVersion() const { return snapshot_.modelVersion; }
    int pendingRemoteVersion() const { return snapshot_.pendingRemoteVersion; }
    bool isSubmitInFlight() const { return snapshot_.submitInFlight; }
    bool isLocalDirtyDuringSubmit() const { return snapshot_.localDirtyDuringSubmit; }
    bool isLocalDirty() const { return state_ == State::Connected_LocalDirty; }
    void clearLocalDirtyDuringSubmit() { snapshot_.localDirtyDuringSubmit = false; }
    bool isApplyingRemoteSnapshot() const { return snapshot_.applyingRemoteSnapshot; }
    void setApplyingRemoteSnapshot(bool v) { snapshot_.applyingRemoteSnapshot = v; }
    const QString& submitRequestId() const { return snapshot_.submitRequestId; }
    const QString& lastPublishReason() const { return snapshot_.lastPublishReason; }

    // Mode1 delta 版本追踪
    int pushedVersion() const { return snapshot_.pushedVersion; }
    void setPushedVersion(int v) { snapshot_.pushedVersion = v; }

    // 项目 ID（切换项目时状态整体归零）
    const QString& projectId() const { return snapshot_.projectId; }
    void setProjectId(const QString& id) {
        if (snapshot_.projectId != id)
            snapshot_ = Snapshot();
        snapshot_.projectId = id;
    }

    // 连接状态
    bool isConnected() const { return state_ != State::Disconnected; }

    // 状态写入接口
    void setModelVersion(int v) { snapshot_.modelVersion = v; }
    void setPendingRemoteVersion(int pv) { snapshot_.pendingRemoteVersion = pv; }
    void setLastPublishReason(const QString& reason) { snapshot_.lastPublishReason = reason; }

    // 调试接口
    QString dump(Event event) const;
    void setDebugEnabled(bool enabled) { debugEnabled_ = enabled; }
    bool isDebugEnabled() const { return debugEnabled_; }
    void setMinDumpIntervalMs(int intervalMs) { minDumpIntervalMs_ = intervalMs; }
    int minDumpIntervalMs() const { return minDumpIntervalMs_; }
    void setEventMinInterval(Event e, int intervalMs);
    int eventMinInterval(Event e) const;

private:
    void transition(State newState, Event event);
    void emitDump(Event event, bool force);
    static const char* stateNameFor(State s);

    struct Snapshot {
        int modelVersion = 0;
        int pendingRemoteVersion = 0;
        int pushedVersion = 0; // Mode1: 上次成功 push 的版本号
        QString projectId;     // 当前连接的 project_id
        QString submitRequestId;
        QString lastPublishReason;
        bool submitInFlight = false;
        bool localDirtyDuringSubmit = false;
        bool applyingRemoteSnapshot = false;
    };

    State state_ = State::Disconnected;
    Snapshot snapshot_;
    bool debugEnabled_ = true;
    int minDumpIntervalMs_ = 0;
    int eventMinIntervalMs_[15] = {0};
    mutable qint64 lastDumpMs_[15] = {0};
};

} // namespace dbcad
