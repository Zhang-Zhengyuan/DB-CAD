#include "collab_session.h"

#include <QObject>
#include <QDateTime>
#include <QStringList>
#include <QDebug>

#undef tr
#define tr(s) QString::fromUtf8(s)

CollabSession& CollabSession::instance() {
    static CollabSession s;
    return s;
}

void CollabSession::bindLegacyFields(
    int&     fastapi_model_version,
    int&     fastapi_pending_remote_version,
    QString& fastapiLastPublishReason,
    QString& fastapiPendingSubmitRequestId,
    bool&    fastapiSubmitInFlight,
    bool&    fastapiLocalDirtyDuringSubmit,
    bool&    fastapiApplyingRemoteSnapshot,
    bool&    fastapiPublishingSnapshot
) {
    fastapi_model_version_ref_              = &fastapi_model_version;
    fastapi_pending_remote_version_ref_     = &fastapi_pending_remote_version;
    fastapiLastPublishReason_ref_           = &fastapiLastPublishReason;
    fastapiPendingSubmitRequestId_ref_      = &fastapiPendingSubmitRequestId;
    fastapiSubmitInFlight_ref_              = &fastapiSubmitInFlight;
    fastapiLocalDirtyDuringSubmit_ref_      = &fastapiLocalDirtyDuringSubmit;
    fastapiApplyingRemoteSnapshot_ref_      = &fastapiApplyingRemoteSnapshot;
    fastapiPublishingSnapshot_ref_          = &fastapiPublishingSnapshot;
}

const char* CollabSession::stateName() const {
    switch (state_) {
        case State::Disconnected:                   return "Disconnected";
        case State::Connected_NoProject:            return "Connected_NoProject";
        case State::Connected_Idle:                 return "Connected_Idle";
        case State::Connected_LocalDirty:           return "Connected_LocalDirty";
        case State::Connected_SubmitInFlight:       return "Connected_SubmitInFlight";
        case State::Connected_SubmitInFlight_Dirty: return "Connected_SubmitInFlight_Dirty";
        case State::Connected_RemotePending:        return "Connected_RemotePending";
        case State::Connected_RemotePending_Dirty:  return "Connected_RemotePending_Dirty";
        case State::Connected_ApplyingRemote:       return "Connected_ApplyingRemote";
        case State::Connected_PublishingDirect:     return "Connected_PublishingDirect";
    }
    return "?";
}

const char* CollabSession::eventName(Event e) const {
    switch (e) {
        case Event::WsMessage:                return "ws_msg";
        case Event::UserEdit:                return "userEdit";
        case Event::UserEditDuringInFlight:  return "userEdit_inFlight";
        case Event::SubmitStarted:           return "submitStart";
        case Event::SubmitAccepted:          return "submitOk";
        case Event::SubmitRejected:          return "submitReject";
        case Event::RemotePending:           return "remotePending";
        case Event::RemoteApplied:           return "remoteApply";
        case Event::ApplyStart:              return "applyStart";
        case Event::ApplyEnd:                return "applyEnd";
        case Event::HttpPublishStart:        return "httpStart";
        case Event::HttpPublishEnd:          return "httpEnd";
        case Event::Reconnect:               return "reconnect";
        case Event::ProjectOpened:           return "projectOpen";
        case Event::ProjectCleared:          return "projectClear";
        case Event::Disconnected:            return "disconnect";
        case Event::Bound:                   return "bound";
    }
    return "other";
}

void CollabSession::setDebugEnabled(bool enabled) { debug_enabled_ = enabled; }
bool CollabSession::isDebugEnabled() const { return debug_enabled_; }
void CollabSession::setMinDumpIntervalMs(int intervalMs) { min_dump_interval_ms_ = intervalMs; }
int  CollabSession::minDumpIntervalMs() const { return min_dump_interval_ms_; }
void CollabSession::setEventMinInterval(Event e, int intervalMs) {
    const int idx = static_cast<int>(e);
    if (idx >= 0 && idx < 17) event_min_interval_ms_[idx] = intervalMs;
}
int CollabSession::eventMinInterval(Event e) const {
    const int idx = static_cast<int>(e);
    if (idx >= 0 && idx < 17) return event_min_interval_ms_[idx];
    return 0;
}

bool CollabSession::isBound() const {
    return fastapi_model_version_ref_ != nullptr
        && fastapiSubmitInFlight_ref_ != nullptr
        && fastapiApplyingRemoteSnapshot_ref_ != nullptr
        && fastapiPublishingSnapshot_ref_ != nullptr;
}

bool CollabSession::isLocalDirtyFromUI() {
    // 由 mainwindow.cpp 的 updateCollabPanelUi 反映过来的"本地有未保存修改"指示
    // 但 CollabSession 不知道具体 UI. 这里返回 false 作为保守默认.
    // 真正的 dirty 检测在 mainwindow 里 (curWindow->getIsModified()),
    // 我们通过 fastapiLocalDirtyDuringSubmit 已经知道 "submit 期间又改了".
    return false;
}

QString CollabSession::dump(Event event) const {
    if (!debug_enabled_) return QString();

    const qint64 now_ms = QDateTime::currentMSecsSinceEpoch();
    const int event_idx = static_cast<int>(event);
    const qint64 last_ms = last_dump_ms_[event_idx];

    const int per_event_ms = event_min_interval_ms_[event_idx];
    const int eff_ms = (per_event_ms > 0) ? per_event_ms : min_dump_interval_ms_;
    if (eff_ms > 0 && last_ms != 0 && (now_ms - last_ms) < eff_ms) {
        return QString();
    }
    last_dump_ms_[event_idx] = now_ms;

    QStringList parts;
    const qint64 rel = (last_ms == 0) ? 0 : (now_ms - last_ms);
    parts << QString("[+%1ms]").arg(rel);
    parts << QString("[CollabSession]");
    parts << QString::fromUtf8(eventName(event));
    parts << QString::fromUtf8(stateName());
    if (fastapi_model_version_ref_) {
        parts << QString("v=%1").arg(*fastapi_model_version_ref_);
    }
    if (fastapi_pending_remote_version_ref_) {
        parts << QString("pv=%1").arg(*fastapi_pending_remote_version_ref_);
    }
    if (fastapiSubmitInFlight_ref_) {
        parts << QString("in_flight=%1").arg(*fastapiSubmitInFlight_ref_ ? "T" : "F");
    }
    if (fastapiLocalDirtyDuringSubmit_ref_) {
        parts << QString("dirty_during=%1").arg(*fastapiLocalDirtyDuringSubmit_ref_ ? "T" : "F");
    }
    if (fastapiApplyingRemoteSnapshot_ref_) {
        parts << QString("apply=%1").arg(*fastapiApplyingRemoteSnapshot_ref_ ? "T" : "F");
    }
    if (fastapiPublishingSnapshot_ref_) {
        parts << QString("publish=%1").arg(*fastapiPublishingSnapshot_ref_ ? "T" : "F");
    }
    return parts.join(" | ");
}

void CollabSession::emitDump(Event event, bool force) {
    if (!debug_enabled_) return;
    const int idx = static_cast<int>(event);
    const qint64 now_ms = QDateTime::currentMSecsSinceEpoch();
    const qint64 last_ms = last_dump_ms_[idx];
    if (!force) {
        const int per_event_ms = event_min_interval_ms_[idx];
        const int eff_ms = (per_event_ms > 0) ? per_event_ms : min_dump_interval_ms_;
        if (eff_ms > 0 && last_ms != 0 && (now_ms - last_ms) < eff_ms) {
            return;
        }
    }
    last_dump_ms_[idx] = now_ms;
    qDebug().noquote() << dump(event);
}

CollabSession::State CollabSession::inferStateFromLegacy() const {
    // 不依赖 external project_id 字段 (不在 8 字段里).
    // 这里只根据 6 个 bool/int 字段推算.
    if (fastapiPublishingSnapshot_ref_ && *fastapiPublishingSnapshot_ref_) {
        return State::Connected_PublishingDirect;
    }
    if (fastapiApplyingRemoteSnapshot_ref_ && *fastapiApplyingRemoteSnapshot_ref_) {
        return State::Connected_ApplyingRemote;
    }
    if (fastapiSubmitInFlight_ref_ && *fastapiSubmitInFlight_ref_) {
        if (fastapiLocalDirtyDuringSubmit_ref_ && *fastapiLocalDirtyDuringSubmit_ref_) {
            return State::Connected_SubmitInFlight_Dirty;
        }
        return State::Connected_SubmitInFlight;
    }
    if (fastapi_pending_remote_version_ref_ && *fastapi_pending_remote_version_ref_ > *fastapi_model_version_ref_) {
        // 本地是否有脏需要外部信号 — 这里保守返回 _Dirty 让 caller 决定
        return State::Connected_RemotePending_Dirty;
    }
    return State::Connected_Idle;
}

void CollabSession::assertConsistent() const {
    if (!isBound()) return;
    State inferred = inferStateFromLegacy();
    if (inferred != state_) {
        qWarning().noquote() << "[CollabSession] INCONSISTENT:"
                             << "state=" << QString::fromUtf8(stateName())
                             << "inferred=" << QString::fromUtf8([&]{
                                    switch (inferred) {
                                        case State::Disconnected:                   return "Disconnected";
                                        case State::Connected_NoProject:            return "Connected_NoProject";
                                        case State::Connected_Idle:                 return "Connected_Idle";
                                        case State::Connected_LocalDirty:           return "Connected_LocalDirty";
                                        case State::Connected_SubmitInFlight:       return "Connected_SubmitInFlight";
                                        case State::Connected_SubmitInFlight_Dirty: return "Connected_SubmitInFlight_Dirty";
                                        case State::Connected_RemotePending:        return "Connected_RemotePending";
                                        case State::Connected_RemotePending_Dirty:  return "Connected_RemotePending_Dirty";
                                        case State::Connected_ApplyingRemote:       return "Connected_ApplyingRemote";
                                        case State::Connected_PublishingDirect:     return "Connected_PublishingDirect";
                                    }
                                    return "?";
                                }());
    }
}

void CollabSession::onUserEdit() {
    if (!isBound()) { emitDump(Event::UserEdit, true); return; }

    if ((*fastapiApplyingRemoteSnapshot_ref_) || (*fastapiPublishingSnapshot_ref_)) {
        emitDump(Event::UserEdit, false);
        return;
    }
    if (fastapiSubmitInFlight_ref_ && *fastapiSubmitInFlight_ref_) {
        if (state_ != State::Connected_SubmitInFlight
         && state_ != State::Connected_SubmitInFlight_Dirty
         && state_ != State::Connected_RemotePending_Dirty) {
            qWarning() << "[CollabSession] onUserEdit while in_flight but state is"
                       << QString::fromUtf8(stateName());
        }
        state_ = State::Connected_SubmitInFlight_Dirty;
        emitDump(Event::UserEditDuringInFlight, true);
        assertConsistent();
        return;
    }
    state_ = State::Connected_LocalDirty;
    emitDump(Event::UserEdit, true);
    assertConsistent();
}

void CollabSession::onUserEditDuringInFlight() {
    if (!isBound()) { emitDump(Event::UserEditDuringInFlight, true); return; }
    state_ = State::Connected_SubmitInFlight_Dirty;
    emitDump(Event::UserEditDuringInFlight, true);
    assertConsistent();
}

void CollabSession::onSubmitStarted() {
    if (!isBound()) { emitDump(Event::SubmitStarted, true); return; }

    if (*fastapiSubmitInFlight_ref_) {
        state_ = (*fastapiLocalDirtyDuringSubmit_ref_)
               ? State::Connected_SubmitInFlight_Dirty
               : State::Connected_SubmitInFlight;
        emitDump(Event::SubmitStarted, true);
        assertConsistent();
        return;
    }
    if (state_ == State::Connected_LocalDirty
     || state_ == State::Connected_Idle
     || state_ == State::Connected_RemotePending) {
        state_ = State::Connected_SubmitInFlight;
    } else {
        qWarning() << "[CollabSession] onSubmitStarted from unexpected state"
                   << QString::fromUtf8(stateName());
        state_ = State::Connected_SubmitInFlight;
    }
    emitDump(Event::SubmitStarted, true);
    assertConsistent();
}

void CollabSession::onSubmitAccepted(int newRemoteVersion) {
    Q_UNUSED(newRemoteVersion);
    if (!isBound()) { emitDump(Event::SubmitAccepted, true); return; }
    if (*fastapiLocalDirtyDuringSubmit_ref_) {
        state_ = State::Connected_LocalDirty;
    } else {
        state_ = State::Connected_Idle;
    }
    emitDump(Event::SubmitAccepted, true);
    assertConsistent();
}

void CollabSession::onSubmitRejected() {
    if (!isBound()) { emitDump(Event::SubmitRejected, true); return; }
    if (*fastapi_pending_remote_version_ref_ > *fastapi_model_version_ref_) {
        state_ = State::Connected_RemotePending_Dirty;
    } else {
        state_ = State::Connected_Idle;
    }
    emitDump(Event::SubmitRejected, true);
    assertConsistent();
}

void CollabSession::onRemotePending(int remoteVersion) {
    Q_UNUSED(remoteVersion);
    if (!isBound()) { emitDump(Event::RemotePending, true); return; }
    if (*fastapiSubmitInFlight_ref_) {
        state_ = State::Connected_SubmitInFlight;
    } else {
        state_ = State::Connected_RemotePending_Dirty;
    }
    emitDump(Event::RemotePending, true);
    assertConsistent();
}

void CollabSession::onRemoteApplied(int appliedVersion) {
    Q_UNUSED(appliedVersion);
    if (!isBound()) { emitDump(Event::RemoteApplied, true); return; }
    if (*fastapiSubmitInFlight_ref_) {
        state_ = State::Connected_SubmitInFlight;
    } else if (*fastapiLocalDirtyDuringSubmit_ref_) {
        state_ = State::Connected_LocalDirty;
    } else {
        state_ = State::Connected_Idle;
    }
    emitDump(Event::RemoteApplied, true);
    assertConsistent();
}

void CollabSession::onApplyStart() {
    if (!isBound()) { emitDump(Event::ApplyStart, true); return; }
    state_ = State::Connected_ApplyingRemote;
    emitDump(Event::ApplyStart, true);
    assertConsistent();
}

void CollabSession::onApplyEnd() {
    if (!isBound()) { emitDump(Event::ApplyEnd, true); return; }
    if (*fastapiSubmitInFlight_ref_) {
        state_ = State::Connected_SubmitInFlight;
    } else if (*fastapiLocalDirtyDuringSubmit_ref_) {
        state_ = State::Connected_LocalDirty;
    } else {
        state_ = State::Connected_Idle;
    }
    emitDump(Event::ApplyEnd, true);
    assertConsistent();
}

void CollabSession::onHttpPublishStart() {
    if (!isBound()) { emitDump(Event::HttpPublishStart, true); return; }
    state_ = State::Connected_PublishingDirect;
    emitDump(Event::HttpPublishStart, true);
    assertConsistent();
}

void CollabSession::onHttpPublishEnd() {
    if (!isBound()) { emitDump(Event::HttpPublishEnd, true); return; }
    if (*fastapi_pending_remote_version_ref_ > *fastapi_model_version_ref_) {
        state_ = State::Connected_RemotePending_Dirty;
    } else {
        state_ = State::Connected_Idle;
    }
    emitDump(Event::HttpPublishEnd, true);
    assertConsistent();
}

void CollabSession::onReconnect(bool projectIdValid) {
    Q_UNUSED(projectIdValid);
    if (!isBound()) { emitDump(Event::Reconnect, true); return; }
    state_ = (*fastapiSubmitInFlight_ref_)
           ? State::Connected_SubmitInFlight
           : State::Connected_Idle;
    emitDump(Event::Reconnect, true);
    assertConsistent();
}

void CollabSession::onProjectOpened() {
    if (!isBound()) { emitDump(Event::ProjectOpened, true); return; }
    state_ = State::Connected_Idle;
    emitDump(Event::ProjectOpened, true);
    assertConsistent();
}

void CollabSession::onProjectCleared() {
    if (!isBound()) { emitDump(Event::ProjectCleared, true); return; }
    state_ = State::Disconnected;
    emitDump(Event::ProjectCleared, true);
    assertConsistent();
}

void CollabSession::onDisconnected() {
    if (!isBound()) { emitDump(Event::Disconnected, true); return; }
    state_ = State::Disconnected;
    emitDump(Event::Disconnected, true);
    assertConsistent();
}

// RAII 守卫 (未使用, 保留兼容)
CollabSessionApplyRemoteGuard::CollabSessionApplyRemoteGuard()  = default;
CollabSessionApplyRemoteGuard::~CollabSessionApplyRemoteGuard() {}
CollabSessionPublishGuard::CollabSessionPublishGuard()  = default;
CollabSessionPublishGuard::~CollabSessionPublishGuard() {}