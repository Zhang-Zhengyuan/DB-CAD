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
    bool&    fastapiPublishingSnapshot,
    bool&    fastapiLocalDirty
) {
    fastapi_model_version_ref_              = &fastapi_model_version;
    fastapi_pending_remote_version_ref_     = &fastapi_pending_remote_version;
    fastapiLastPublishReason_ref_           = &fastapiLastPublishReason;
    fastapiPendingSubmitRequestId_ref_      = &fastapiPendingSubmitRequestId;
    fastapiSubmitInFlight_ref_              = &fastapiSubmitInFlight;
    fastapiLocalDirtyDuringSubmit_ref_      = &fastapiLocalDirtyDuringSubmit;
    fastapiApplyingRemoteSnapshot_ref_      = &fastapiApplyingRemoteSnapshot;
    fastapiPublishingSnapshot_ref_          = &fastapiPublishingSnapshot;
    fastapiLocalDirty_ref_                  = &fastapiLocalDirty;

    snapshot_.modelVersion           = fastapi_model_version;
    snapshot_.pendingRemoteVersion   = fastapi_pending_remote_version;
    snapshot_.lastPublishReason     = fastapiLastPublishReason;
    snapshot_.submitRequestId       = fastapiPendingSubmitRequestId;
    snapshot_.submitInFlight        = fastapiSubmitInFlight;
    snapshot_.localDirtyDuringSubmit= fastapiLocalDirtyDuringSubmit;
    snapshot_.applyingRemoteSnapshot= fastapiApplyingRemoteSnapshot;
    snapshot_.publishingSnapshot    = fastapiPublishingSnapshot;

    emitDump(Event::Bound, true);
}

const char* CollabSession::stateName() const {
    return stateNameFor(state_);
}

const char* CollabSession::stateNameFor(State s) {
    switch (s) {
        case State::Disconnected:                   return "Disconnected";
        case State::Connected_NoProject:            return "Connected_NoProject";
        case State::Connected_Idle:                 return "Connected_Idle";
        case State::Connected_LocalDirty:           return "Connected_LocalDirty";
        case State::Connected_SubmitInFlight:        return "Connected_SubmitInFlight";
        case State::Connected_SubmitInFlight_Dirty:  return "Connected_SubmitInFlight_Dirty";
        case State::Connected_RemotePending_Dirty:  return "Connected_RemotePending_Dirty";
        case State::Connected_ApplyingRemote:       return "Connected_ApplyingRemote";
        case State::Connected_PublishingDirect:      return "Connected_PublishingDirect";
    }
    return "?";
}

const char* CollabSession::eventName(Event e) const {
    switch (e) {
        case Event::WsMessage:              return "ws_msg";
        case Event::UserEdit:               return "userEdit";
        case Event::UserEditDuringInFlight: return "userEdit_inFlight";
        case Event::SubmitStarted:          return "submitStart";
        case Event::SubmitAccepted:         return "submitOk";
        case Event::SubmitRejected:         return "submitReject";
        case Event::RemotePending:          return "remotePending";
        case Event::RemoteApplied:          return "remoteApply";
        case Event::ApplyStart:            return "applyStart";
        case Event::ApplyEnd:              return "applyEnd";
        case Event::HttpPublishStart:      return "httpStart";
        case Event::HttpPublishEnd:        return "httpEnd";
        case Event::Reconnect:             return "reconnect";
        case Event::ProjectOpened:         return "projectOpen";
        case Event::ProjectCleared:        return "projectClear";
        case Event::Disconnected:          return "disconnect";
        case Event::Bound:                 return "bound";
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

void CollabSession::mirrorToLegacy() const {
    if (!isBound()) return;
    *fastapi_model_version_ref_              = snapshot_.modelVersion;
    *fastapi_pending_remote_version_ref_    = snapshot_.pendingRemoteVersion;
    *fastapiLastPublishReason_ref_          = snapshot_.lastPublishReason;
    *fastapiPendingSubmitRequestId_ref_      = snapshot_.submitRequestId;
    *fastapiSubmitInFlight_ref_             = snapshot_.submitInFlight;
    *fastapiLocalDirtyDuringSubmit_ref_     = snapshot_.localDirtyDuringSubmit;
    *fastapiApplyingRemoteSnapshot_ref_     = snapshot_.applyingRemoteSnapshot;
    *fastapiPublishingSnapshot_ref_         = snapshot_.publishingSnapshot;
    *fastapiLocalDirty_ref_ = (state_ == State::Connected_LocalDirty);
}

void CollabSession::transition(State newState, Event event) {
    if (state_ == newState) {
        emitDump(event, true);
        assertConsistent();
        return;
    }
    state_ = newState;
    mirrorToLegacy();
    emitDump(event, true);
    assertConsistent();
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
    parts << QString("v=%1").arg(snapshot_.modelVersion);
    parts << QString("pv=%1").arg(snapshot_.pendingRemoteVersion);
    parts << QString("in_flight=%1").arg(snapshot_.submitInFlight ? "T" : "F");
    parts << QString("dirty_during=%1").arg(snapshot_.localDirtyDuringSubmit ? "T" : "F");
    parts << QString("apply=%1").arg(snapshot_.applyingRemoteSnapshot ? "T" : "F");
    parts << QString("publish=%1").arg(snapshot_.publishingSnapshot ? "T" : "F");
    if (!snapshot_.submitRequestId.isEmpty()) {
        parts << QString("req=%1").arg(snapshot_.submitRequestId.left(8));
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
    if (!isBound()) return state_;
    if (*fastapiPublishingSnapshot_ref_)              return State::Connected_PublishingDirect;
    if (*fastapiApplyingRemoteSnapshot_ref_)          return State::Connected_ApplyingRemote;
    if (*fastapiSubmitInFlight_ref_) {
        if (*fastapiLocalDirtyDuringSubmit_ref_)      return State::Connected_SubmitInFlight_Dirty;
        return State::Connected_SubmitInFlight;
    }
    if (*fastapi_pending_remote_version_ref_ > *fastapi_model_version_ref_) {
        return State::Connected_RemotePending_Dirty;
    }
    if (*fastapiLocalDirty_ref_)                      return State::Connected_LocalDirty;
    return State::Connected_Idle;
}

void CollabSession::assertConsistent() const {
    if (!isBound()) return;
    State inferred = inferStateFromLegacy();
    if (state_ == State::Disconnected
     || state_ == State::Connected_NoProject
     || inferred == State::Disconnected) {
        return;
    }
    if (inferred != state_) {
        qWarning().noquote() << "[CollabSession] INCONSISTENT:"
                             << "state=" << QString::fromUtf8(stateNameFor(state_))
                             << "inferred=" << QString::fromUtf8(stateNameFor(inferred));
    }
}

CollabSession::SubmitDecision CollabSession::tryBeginSubmit(const QString& reason) {
    SubmitDecision d;

    if (state_ == State::Disconnected || state_ == State::Connected_NoProject) {
        d.kind = SubmitDecision::RejectNoProject;
        d.reason = tr("协作通道未连接或项目未打开");
        emitDump(Event::SubmitStarted, true);
        return d;
    }
    if (snapshot_.applyingRemoteSnapshot) {
        d.kind = SubmitDecision::RejectApplyingRemote;
        d.reason = tr("正在应用远程快照，无法提交");
        emitDump(Event::SubmitStarted, true);
        return d;
    }
    if (snapshot_.publishingSnapshot) {
        d.kind = SubmitDecision::RejectPublishingDirect;
        d.reason = tr("正在通过 HTTP 发布，无法提交");
        emitDump(Event::SubmitStarted, true);
        return d;
    }
    if (snapshot_.submitInFlight) {
        snapshot_.localDirtyDuringSubmit = true;
        mirrorToLegacy();
        transition(State::Connected_SubmitInFlight_Dirty, Event::UserEditDuringInFlight);
        d.kind = SubmitDecision::RejectAlreadyInFlight;
        d.reason = tr("上一笔提交还在进行中，已标记为 dirty，待当前 ack 后重新发布");
        return d;
    }
    if (snapshot_.pendingRemoteVersion > snapshot_.modelVersion) {
        d.kind = SubmitDecision::RejectRemotePending;
        d.reason = tr("有远程版本 %1 待同步，请先处理冲突").arg(snapshot_.pendingRemoteVersion);
        d.remoteVersion = snapshot_.pendingRemoteVersion;
        emitDump(Event::SubmitStarted, true);
        return d;
    }

    snapshot_.submitRequestId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    snapshot_.submitInFlight = true;
    snapshot_.localDirtyDuringSubmit = false;
    snapshot_.lastPublishReason = reason.isEmpty() ? QString::fromUtf8("local-change") : reason;
    mirrorToLegacy();

    if (state_ == State::Connected_Idle || state_ == State::Connected_LocalDirty) {
        transition(State::Connected_SubmitInFlight, Event::SubmitStarted);
    } else {
        qWarning() << "[CollabSession] tryBeginSubmit from unexpected state"
                   << QString::fromUtf8(stateName());
        transition(State::Connected_SubmitInFlight, Event::SubmitStarted);
    }

    d.kind = SubmitDecision::Allow;
    d.requestId = snapshot_.submitRequestId;
    return d;
}

void CollabSession::rollbackSubmit() {
    if (!snapshot_.submitInFlight) return;
    snapshot_.submitInFlight = false;
    snapshot_.satSubmitInFlight = false;
    snapshot_.submitRequestId.clear();
    mirrorToLegacy();
    if (snapshot_.localDirtyDuringSubmit) {
        transition(State::Connected_LocalDirty, Event::SubmitStarted);
    } else {
        transition(State::Connected_Idle, Event::SubmitStarted);
    }
}

CollabSession::ApplyDecision CollabSession::tryBeginApplyRemote(int remoteVersion, const QString& /*reason*/) {
    ApplyDecision d;

    if (state_ == State::Disconnected || state_ == State::Connected_NoProject) {
        d.kind = ApplyDecision::RejectNoProject;
        d.reason = tr("协作通道未连接或项目未打开");
        d.reasonTr = QString::fromUtf8("协作通道未连接或项目未打开");
        return d;
    }
    if (snapshot_.applyingRemoteSnapshot) {
        d.kind = ApplyDecision::RejectAlreadyApplying;
        d.reason = tr("已经在应用远程快照");
        d.reasonTr = QString::fromUtf8("已经在应用远程快照");
        return d;
    }
    if (remoteVersion <= 0) {
        d.kind = ApplyDecision::RejectNoContent;
        d.reason = tr("远端版本号非法");
        d.reasonTr = QString::fromUtf8("远端版本号非法");
        return d;
    }

    snapshot_.applyingRemoteSnapshot = true;
    snapshot_.pendingRemoteVersion = qMax(snapshot_.pendingRemoteVersion, remoteVersion);
    mirrorToLegacy();

    d.kind = ApplyDecision::Allow;
    return d;
}

void CollabSession::rollbackApply() {
    if (!snapshot_.applyingRemoteSnapshot) return;
    snapshot_.applyingRemoteSnapshot = false;
    mirrorToLegacy();
    if (snapshot_.pendingRemoteVersion > snapshot_.modelVersion) {
        transition(State::Connected_RemotePending_Dirty, Event::ApplyEnd);
    } else {
        transition(State::Connected_Idle, Event::ApplyEnd);
    }
}

CollabSession::PublishDecision CollabSession::tryBeginHttpPublish() {
    PublishDecision d;
    if (state_ == State::Disconnected || state_ == State::Connected_NoProject) {
        d.kind = PublishDecision::RejectNoProject;
        d.reason = tr("协作通道未连接或项目未打开");
        emitDump(Event::HttpPublishStart, true);
        return d;
    }
    if (snapshot_.publishingSnapshot) {
        d.kind = PublishDecision::RejectAlreadyPublishing;
        d.reason = tr("已经在 HTTP 发布中");
        emitDump(Event::HttpPublishStart, true);
        return d;
    }
    if (snapshot_.submitInFlight) {
        d.kind = PublishDecision::RejectSubmitInFlight;
        d.reason = tr("WS submit 还在进行中，无法 HTTP 直发");
        emitDump(Event::HttpPublishStart, true);
        return d;
    }

    snapshot_.publishingSnapshot = true;
    mirrorToLegacy();
    transition(State::Connected_PublishingDirect, Event::HttpPublishStart);

    d.kind = PublishDecision::Allow;
    return d;
}

void CollabSession::rollbackHttpPublish() {
    if (!snapshot_.publishingSnapshot) return;
    snapshot_.publishingSnapshot = false;
    mirrorToLegacy();
    if (snapshot_.pendingRemoteVersion > snapshot_.modelVersion) {
        transition(State::Connected_RemotePending_Dirty, Event::HttpPublishEnd);
    } else {
        transition(State::Connected_Idle, Event::HttpPublishEnd);
    }
}

void CollabSession::setLastPublishReason(const QString& reason) {
    snapshot_.lastPublishReason = reason;
    mirrorToLegacy();
}

void CollabSession::setModelVersion(int v) {
    snapshot_.modelVersion = v;
    mirrorToLegacy();
}

void CollabSession::setPendingRemoteVersion(int pv) {
    snapshot_.pendingRemoteVersion = pv;
    mirrorToLegacy();
}

void CollabSession::onUserEdit() {
    if (!isBound()) { emitDump(Event::UserEdit, true); return; }

    if (snapshot_.applyingRemoteSnapshot || snapshot_.publishingSnapshot) {
        emitDump(Event::UserEdit, false);
        return;
    }
    if (snapshot_.submitInFlight) {
        snapshot_.localDirtyDuringSubmit = true;
        mirrorToLegacy();
        transition(State::Connected_SubmitInFlight_Dirty, Event::UserEditDuringInFlight);
        return;
    }
    transition(State::Connected_LocalDirty, Event::UserEdit);
}

void CollabSession::onUserEditDuringInFlight() {
    if (!isBound()) { emitDump(Event::UserEditDuringInFlight, true); return; }
    if (!snapshot_.submitInFlight) {
        qWarning() << "[CollabSession] onUserEditDuringInFlight while not in_flight";
        return;
    }
    snapshot_.localDirtyDuringSubmit = true;
    mirrorToLegacy();
    transition(State::Connected_SubmitInFlight_Dirty, Event::UserEditDuringInFlight);
}

void CollabSession::onSubmitAccepted(int newRemoteVersion) {
    if (!isBound()) { emitDump(Event::SubmitAccepted, true); return; }
    const bool hadDirtyDuringSubmit = snapshot_.localDirtyDuringSubmit;
    if (snapshot_.submitInFlight) {
        snapshot_.submitInFlight = false;
        snapshot_.satSubmitInFlight = false;
        snapshot_.submitRequestId.clear();
        snapshot_.modelVersion = qMax(snapshot_.modelVersion, newRemoteVersion);
        snapshot_.pendingRemoteVersion = 0;
        snapshot_.localDirtyDuringSubmit = false;
        mirrorToLegacy();
    }
    if (hadDirtyDuringSubmit) {
        transition(State::Connected_LocalDirty, Event::SubmitAccepted);
    } else {
        transition(State::Connected_Idle, Event::SubmitAccepted);
    }
}

void CollabSession::onSubmitRejected(int latestVersion) {
    if (!isBound()) { emitDump(Event::SubmitRejected, true); return; }
    if (snapshot_.submitInFlight) {
        snapshot_.submitInFlight = false;
        snapshot_.submitRequestId.clear();
    }
    if (latestVersion > snapshot_.modelVersion) {
        snapshot_.pendingRemoteVersion = qMax(snapshot_.pendingRemoteVersion, latestVersion);
        mirrorToLegacy();
        transition(State::Connected_RemotePending_Dirty, Event::SubmitRejected);
    } else {
        mirrorToLegacy();
        if (snapshot_.localDirtyDuringSubmit) {
            transition(State::Connected_LocalDirty, Event::SubmitRejected);
        } else {
            transition(State::Connected_Idle, Event::SubmitRejected);
        }
    }
}

void CollabSession::onRemotePending(int remoteVersion) {
    if (!isBound()) { emitDump(Event::RemotePending, true); return; }
    // 总是把 pendingRemoteVersion 累加上，以便 publish/apply 完成后能正确进入
    // Connected_RemotePending_Dirty 状态。
    if (remoteVersion > snapshot_.modelVersion) {
        snapshot_.pendingRemoteVersion = qMax(snapshot_.pendingRemoteVersion, remoteVersion);
        mirrorToLegacy();
    }
    // 在 applying/publishing 期间，publish 流程会自己处理 pendingRemoteVersion
    // （onHttpPublishEnd / onApplyEnd 会重新检查并切到 RemotePending_Dirty），
    // 这里不要打乱状态机；保留 current state 不变即可。
    if (snapshot_.applyingRemoteSnapshot || snapshot_.publishingSnapshot) {
        emitDump(Event::RemotePending, true);
        return;
    }
    if (snapshot_.submitInFlight) {
        transition(State::Connected_SubmitInFlight, Event::RemotePending);
    } else if (snapshot_.localDirtyDuringSubmit) {
        transition(State::Connected_SubmitInFlight_Dirty, Event::RemotePending);
    } else {
        transition(State::Connected_RemotePending_Dirty, Event::RemotePending);
    }
}

void CollabSession::onRemoteApplied(int appliedVersion) {
    if (!isBound()) { emitDump(Event::RemoteApplied, true); return; }
    // 如果当前正在 apply 流程（tryBeginApplyRemote 已把 applyingRemoteSnapshot=true），
    // 这里要把它清掉，否则 apply 标志会卡住，导致后续 addEntity 的 recordEntityAdded 被跳过。
    // 这种情况出现在 syncFastAPIRemoteVersion 走 entity_graph 路径时。
    snapshot_.applyingRemoteSnapshot = false;
    snapshot_.modelVersion = appliedVersion;
    snapshot_.pendingRemoteVersion = 0;
    mirrorToLegacy();
    if (snapshot_.submitInFlight) {
        transition(State::Connected_SubmitInFlight, Event::RemoteApplied);
    } else if (snapshot_.localDirtyDuringSubmit) {
        transition(State::Connected_LocalDirty, Event::RemoteApplied);
    } else {
        transition(State::Connected_Idle, Event::RemoteApplied);
    }
}

void CollabSession::onApplyStart() {
    if (!isBound()) { emitDump(Event::ApplyStart, true); return; }
    snapshot_.applyingRemoteSnapshot = true;
    mirrorToLegacy();
    transition(State::Connected_ApplyingRemote, Event::ApplyStart);
}

void CollabSession::onApplyEnd() {
    if (!isBound()) { emitDump(Event::ApplyEnd, true); return; }
    snapshot_.applyingRemoteSnapshot = false;
    mirrorToLegacy();
    // Only clear pendingRemoteVersion if no newer remote version arrived during apply.
    // onRemotePending() may have bumped pendingRemoteVersion while we were applying;
    // in that case we must stay in (or transition to) RemotePending_Dirty.
    if (snapshot_.pendingRemoteVersion <= snapshot_.modelVersion) {
        snapshot_.pendingRemoteVersion = 0;
        mirrorToLegacy();
    }
    if (snapshot_.submitInFlight) {
        transition(State::Connected_SubmitInFlight, Event::ApplyEnd);
    } else if (snapshot_.localDirtyDuringSubmit) {
        transition(State::Connected_SubmitInFlight_Dirty, Event::ApplyEnd);
    } else if (snapshot_.pendingRemoteVersion > snapshot_.modelVersion) {
        transition(State::Connected_RemotePending_Dirty, Event::ApplyEnd);
    } else {
        transition(State::Connected_Idle, Event::ApplyEnd);
    }
}

void CollabSession::onHttpPublishStart() {
    if (!isBound()) { emitDump(Event::HttpPublishStart, true); return; }
    snapshot_.publishingSnapshot = true;
    mirrorToLegacy();
    transition(State::Connected_PublishingDirect, Event::HttpPublishStart);
}

void CollabSession::onHttpPublishEnd() {
    if (!isBound()) { emitDump(Event::HttpPublishEnd, true); return; }
    snapshot_.publishingSnapshot = false;
    mirrorToLegacy();
    if (snapshot_.pendingRemoteVersion > snapshot_.modelVersion) {
        transition(State::Connected_RemotePending_Dirty, Event::HttpPublishEnd);
    } else {
        transition(State::Connected_Idle, Event::HttpPublishEnd);
    }
}

void CollabSession::onReconnect(bool /*projectIdValid*/) {
    if (!isBound()) { emitDump(Event::Reconnect, true); return; }
    if (snapshot_.submitInFlight) {
        transition(State::Connected_SubmitInFlight, Event::Reconnect);
    } else {
        transition(State::Connected_Idle, Event::Reconnect);
    }
}

void CollabSession::onProjectOpened() {
    if (!isBound()) { emitDump(Event::ProjectOpened, true); return; }
    snapshot_.submitInFlight = false;
    snapshot_.localDirtyDuringSubmit = false;
    snapshot_.submitRequestId.clear();
    snapshot_.publishingSnapshot = false;
    snapshot_.applyingRemoteSnapshot = false;
    mirrorToLegacy();
    transition(State::Connected_Idle, Event::ProjectOpened);
}

void CollabSession::onProjectCleared() {
    if (!isBound()) { emitDump(Event::ProjectCleared, true); return; }
    transition(State::Disconnected, Event::ProjectCleared);
}

void CollabSession::onDisconnected() {
    if (!isBound()) { emitDump(Event::Disconnected, true); return; }
    snapshot_.submitInFlight = false;
    snapshot_.localDirtyDuringSubmit = false;
    snapshot_.submitRequestId.clear();
    snapshot_.publishingSnapshot = false;
    snapshot_.applyingRemoteSnapshot = false;
    mirrorToLegacy();
    transition(State::Disconnected, Event::Disconnected);
}

void CollabSession::reset() {
    snapshot_ = Snapshot();
    state_ = State::Disconnected;
    mirrorToLegacy();
}

CollabSessionApplyRemoteGuard::CollabSessionApplyRemoteGuard()  = default;
CollabSessionApplyRemoteGuard::~CollabSessionApplyRemoteGuard() {}
CollabSessionPublishGuard::CollabSessionPublishGuard()  = default;
CollabSessionPublishGuard::~CollabSessionPublishGuard() {}