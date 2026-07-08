#pragma once

#include <QString>

// CollabSession — 协作状态机
// 
// 转移入口:
//   onUserEdit()              notifyModelChangedForCollaboration 入口
//   onSubmitStarted()         submitFastAPIModelOverSocket 入口
//   onSubmitAccepted()        ws_msg model_saved (自己的 request_id 匹配)
//   onSubmitRejected()        ws_msg submit_rejected
//   onRemotePending()         ws_msg model_saved (别人的,或 pv 落库)
//   onRemoteApplied()         syncFastAPIRemoteVersion / applyFastAPIRemoteSat 完成
//   onApplyStart()            restoreFastAPIModelFromSat 入口
//   onApplyEnd()              restoreFastAPIModelFromSat RAII 退出
//   onHttpPublishStart()      publishFastAPIModelSnapshot(true) 入口
//   onHttpPublishEnd()        publishFastAPIModelSnapshot(true) 出口
//   onReconnect()             reconnectFastAPISync 入口
//   onProjectOpened()         fastapi_project_id 由空变非空
//   onProjectCleared()        fastapi_project_id 由非空变空
//   onDisconnected()          disconnectFastAPISync 入口
//   onUserEditDuringInFlight() notifyChanged 在 in_flight=true 期间触发
//
// 状态:
//   Disconnected                  初始; WS 未连; 或项目未开
//   Connected_NoProject           WS 连, project_id 为空
//   Connected_Idle                project_id 非空, v=server v, 无 pv, 无 in_flight
//   Connected_LocalDirty          用户改了, publishTimer 启动中, 即将 submit
//   Connected_SubmitInFlight      submit 已发出, 等 server ack
//   Connected_SubmitInFlight_Dirty submit 期间用户又改了
//   Connected_RemotePending       server 有更高 v, 本地没脏
//   Connected_RemotePending_Dirty server 有更高 v, 本地有脏
//   Connected_ApplyingRemote      SAT 反序列化中
//   Connected_PublishingDirect    HTTP PUT 直发中

class CollabSession {
public:
    enum class State {
        Disconnected,
        Connected_NoProject,
        Connected_Idle,
        Connected_LocalDirty,
        Connected_SubmitInFlight,
        Connected_SubmitInFlight_Dirty,
        Connected_RemotePending,
        Connected_RemotePending_Dirty,
        Connected_ApplyingRemote,
        Connected_PublishingDirect
    };

    enum class Event {
        WsMessage,                 // ws_msg 入口 (限频 dump 用, 转移由 type 分支决定)
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

    State state() const { return state_; }
    const char* stateName() const;
    const char* eventName(Event e) const;

    void bindLegacyFields(
        int&    fastapi_model_version,
        int&    fastapi_pending_remote_version,
        QString& fastapiLastPublishReason,
        QString& fastapiPendingSubmitRequestId,
        bool&   fastapiSubmitInFlight,
        bool&   fastapiLocalDirtyDuringSubmit,
        bool&   fastapiApplyingRemoteSnapshot,
        bool&   fastapiPublishingSnapshot
    );

    // dump() 强制输出当前快照
    QString dump(Event event) const;

    void setDebugEnabled(bool enabled);
    bool isDebugEnabled() const;
    void setMinDumpIntervalMs(int intervalMs);
    int  minDumpIntervalMs() const;
    void setEventMinInterval(Event e, int intervalMs);
    int  eventMinInterval(Event e) const;

    // 转移函数 — mainwindow.cpp 在每个事件边界调用一次
    void onUserEdit();
    void onUserEditDuringInFlight();
    void onSubmitStarted();
    void onSubmitAccepted(int newRemoteVersion);
    void onSubmitRejected();
    void onRemotePending(int remoteVersion);
    void onRemoteApplied(int appliedVersion);
    void onApplyStart();
    void onApplyEnd();
    void onHttpPublishStart();
    void onHttpPublishEnd();
    void onReconnect(bool projectIdValid);
    void onProjectOpened();
    void onProjectCleared();
    void onDisconnected();

    // 校验 state_ 和旧字段是否一致; 不一致 qWarning 但不崩
    void assertConsistent() const;

    static CollabSession& instance();

private:
    CollabSession() = default;
    State state_ = State::Disconnected;
    bool  debug_enabled_ = true;
    int   min_dump_interval_ms_ = 0;
    int   event_min_interval_ms_[17] = {0};

    int*     fastapi_model_version_ref_              = nullptr;
    int*     fastapi_pending_remote_version_ref_     = nullptr;
    QString* fastapiLastPublishReason_ref_           = nullptr;
    QString* fastapiPendingSubmitRequestId_ref_      = nullptr;
    bool*    fastapiSubmitInFlight_ref_              = nullptr;
    bool*    fastapiLocalDirtyDuringSubmit_ref_      = nullptr;
    bool*    fastapiApplyingRemoteSnapshot_ref_      = nullptr;
    bool*    fastapiPublishingSnapshot_ref_          = nullptr;

    mutable qint64 last_dump_ms_[17] = {0};

    // 内部: 计算 "如果 mainwindow 旧字段一致, 当前应该是哪个 state"
    CollabSession::State inferStateFromLegacy() const;
    void  emitDump(Event event, bool force);
    bool  isBound() const;
    static bool isLocalDirtyFromUI();  // 由 mainwindow 通过 setIsLocalDirtyFromUI 设置
};


// RAII 守卫: 进入反序列化区间时设 apply 状态, 退出时回归
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