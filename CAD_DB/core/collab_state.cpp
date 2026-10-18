#include "core/collab_state.h"

#include <QDebug>
#include <QStringList>

namespace dbcad {

namespace {
QString tr(const char* s) { return QString::fromUtf8(s); }
}

const char* CollabState::stateName() const {
    return stateNameFor(state_);
}

const char* CollabState::stateNameFor(State s) {
    switch (s) {
    case State::Disconnected:
        return "Disconnected";
    case State::Connected_NoProject:
        return "Connected_NoProject";
    case State::Connected_Idle:
        return "Connected_Idle";
    case State::Connected_LocalDirty:
        return "Connected_LocalDirty";
    case State::Connected_SubmitInFlight:
        return "Connected_SubmitInFlight";
    case State::Connected_SubmitInFlight_Dirty:
        return "Connected_SubmitInFlight_Dirty";
    case State::Connected_RemotePending_Dirty:
        return "Connected_RemotePending_Dirty";
    case State::Connected_ApplyingRemote:
        return "Connected_ApplyingRemote";
    }
    return "?";
}

const char* CollabState::eventName(Event e) const {
    switch (e) {
    case Event::WsMessage:
        return "ws_msg";
    case Event::UserEdit:
        return "userEdit";
    case Event::UserEditDuringInFlight:
        return "userEdit_inFlight";
    case Event::SubmitStarted:
        return "submitStart";
    case Event::SubmitAccepted:
        return "submitOk";
    case Event::SubmitRejected:
        return "submitReject";
    case Event::RemotePending:
        return "remotePending";
    case Event::RemoteApplied:
        return "remoteApply";
    case Event::ApplyStart:
        return "applyStart";
    case Event::ApplyEnd:
        return "applyEnd";
    case Event::Reconnect:
        return "reconnect";
    case Event::ProjectOpened:
        return "projectOpen";
    case Event::ProjectCleared:
        return "projectClear";
    case Event::Disconnected:
        return "disconnect";
    case Event::Bound:
        return "bound";
    }
    return "other";
}

void CollabState::setEventMinInterval(Event e, int intervalMs) {
    const int idx = static_cast<int>(e);
    if (idx >= 0 && idx < 15)
        eventMinIntervalMs_[idx] = intervalMs;
}

int CollabState::eventMinInterval(Event e) const {
    const int idx = static_cast<int>(e);
    if (idx >= 0 && idx < 15)
        return eventMinIntervalMs_[idx];
    return 0;
}

void CollabState::transition(State newState, Event event) {
    if (state_ == newState) {
        emitDump(event, true);
        return;
    }
    state_ = newState;
    emitDump(event, true);
}

QString CollabState::dump(Event event) const {
    if (!debugEnabled_)
        return QString();

    const qint64 nowMs = QDateTime::currentMSecsSinceEpoch();
    const int eventIdx = static_cast<int>(event);
    const qint64 lastMs = lastDumpMs_[eventIdx];

    const int perEventMs = eventMinIntervalMs_[eventIdx];
    const int effMs = (perEventMs > 0) ? perEventMs : minDumpIntervalMs_;
    if (effMs > 0 && lastMs != 0 && (nowMs - lastMs) < effMs) {
        return QString();
    }
    lastDumpMs_[eventIdx] = nowMs;

    QStringList parts;
    const qint64 rel = (lastMs == 0) ? 0 : (nowMs - lastMs);
    parts << QString("[+%1ms]").arg(rel);
    parts << QString("[CollabState]");
    parts << QString::fromUtf8(eventName(event));
    parts << QString::fromUtf8(stateName());
    parts << QString("v=%1").arg(snapshot_.modelVersion);
    parts << QString("pv=%1").arg(snapshot_.pendingRemoteVersion);
    parts << QString("in_flight=%1").arg(snapshot_.submitInFlight ? "T" : "F");
    parts << QString("dirty_during=%1").arg(snapshot_.localDirtyDuringSubmit ? "T" : "F");
    parts << QString("apply=%1").arg(snapshot_.applyingRemoteSnapshot ? "T" : "F");
    if (!snapshot_.submitRequestId.isEmpty()) {
        parts << QString("req=%1").arg(snapshot_.submitRequestId.left(8));
    }
    return parts.join(" | ");
}

void CollabState::emitDump(Event event, bool force) {
    if (!debugEnabled_)
        return;
    const int idx = static_cast<int>(event);
    const qint64 nowMs = QDateTime::currentMSecsSinceEpoch();
    const qint64 lastMs = lastDumpMs_[idx];
    if (!force) {
        const int perEventMs = eventMinIntervalMs_[idx];
        const int effMs = (perEventMs > 0) ? perEventMs : minDumpIntervalMs_;
        if (effMs > 0 && lastMs != 0 && (nowMs - lastMs) < effMs) {
            return;
        }
    }
    lastDumpMs_[idx] = nowMs;
    qDebug().noquote() << dump(event);
}

CollabState::SubmitDecision CollabState::tryBeginSubmit(const QString& reason) {
    SubmitDecision d;

    if (snapshot_.projectId.isEmpty() || state_ == State::Disconnected || state_ == State::Connected_NoProject) {
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
    if (snapshot_.submitInFlight) {
        snapshot_.localDirtyDuringSubmit = true;
        transition(State::Connected_SubmitInFlight_Dirty, Event::UserEditDuringInFlight);
        d.kind = SubmitDecision::RejectAlreadyInFlight;
        d.reason = tr("上一笔提交还在进行中，已标记为 dirty，请等待当前提交完成");
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

    if (state_ == State::Connected_Idle || state_ == State::Connected_LocalDirty) {
        transition(State::Connected_SubmitInFlight, Event::SubmitStarted);
    } else {
        qWarning() << "[CollabState] tryBeginSubmit from unexpected state" << QString::fromUtf8(stateName());
        transition(State::Connected_SubmitInFlight, Event::SubmitStarted);
    }

    d.kind = SubmitDecision::Allow;
    d.requestId = snapshot_.submitRequestId;
    return d;
}

void CollabState::rollbackSubmit() {
    if (!snapshot_.submitInFlight)
        return;
    snapshot_.submitInFlight = false;
    snapshot_.submitRequestId.clear();
    if (snapshot_.localDirtyDuringSubmit) {
        transition(State::Connected_LocalDirty, Event::SubmitStarted);
    } else {
        transition(State::Connected_Idle, Event::SubmitStarted);
    }
}

void CollabState::onUserEdit() {
    if (snapshot_.applyingRemoteSnapshot) {
        emitDump(Event::UserEdit, false);
        return;
    }
    if (snapshot_.submitInFlight) {
        snapshot_.localDirtyDuringSubmit = true;
        transition(State::Connected_SubmitInFlight_Dirty, Event::UserEditDuringInFlight);
        return;
    }
    transition(State::Connected_LocalDirty, Event::UserEdit);
}

void CollabState::onUserEditDuringInFlight() {
    if (!snapshot_.submitInFlight) {
        qWarning() << "[CollabState] onUserEditDuringInFlight while not in_flight";
        return;
    }
    snapshot_.localDirtyDuringSubmit = true;
    transition(State::Connected_SubmitInFlight_Dirty, Event::UserEditDuringInFlight);
}

void CollabState::onSubmitAccepted(int newRemoteVersion) {
    const bool hadDirtyDuringSubmit = snapshot_.localDirtyDuringSubmit;
    if (snapshot_.submitInFlight) {
        snapshot_.submitInFlight = false;
        snapshot_.submitRequestId.clear();
        snapshot_.modelVersion = qMax(snapshot_.modelVersion, newRemoteVersion);
        if (snapshot_.pendingRemoteVersion <= snapshot_.modelVersion)
            snapshot_.pendingRemoteVersion = 0;
        snapshot_.localDirtyDuringSubmit = false;
    }
    if (snapshot_.pendingRemoteVersion > snapshot_.modelVersion) {
        transition(State::Connected_RemotePending_Dirty, Event::SubmitAccepted);
    } else if (hadDirtyDuringSubmit) {
        transition(State::Connected_LocalDirty, Event::SubmitAccepted);
    } else {
        transition(State::Connected_Idle, Event::SubmitAccepted);
    }
}

void CollabState::onRemotePending(int remoteVersion) {
    // Keep the newest notification, including messages received during HTTP.
    if (remoteVersion > snapshot_.modelVersion) {
        snapshot_.pendingRemoteVersion = qMax(snapshot_.pendingRemoteVersion, remoteVersion);
    }
    // During reconstruction, retain notifications until the apply completes.
    if (snapshot_.applyingRemoteSnapshot) {
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

void CollabState::onRemoteApplied(int appliedVersion) {
    snapshot_.applyingRemoteSnapshot = false;
    snapshot_.modelVersion = appliedVersion;
    if (snapshot_.pendingRemoteVersion <= snapshot_.modelVersion)
        snapshot_.pendingRemoteVersion = 0;
    if (snapshot_.submitInFlight) {
        transition(State::Connected_SubmitInFlight, Event::RemoteApplied);
    } else if (snapshot_.pendingRemoteVersion > snapshot_.modelVersion) {
        transition(State::Connected_RemotePending_Dirty, Event::RemoteApplied);
    } else if (snapshot_.localDirtyDuringSubmit) {
        transition(State::Connected_LocalDirty, Event::RemoteApplied);
    } else {
        transition(State::Connected_Idle, Event::RemoteApplied);
    }
}

void CollabState::onReconnect(bool /*projectIdValid*/) {
    if (snapshot_.submitInFlight) {
        transition(State::Connected_SubmitInFlight, Event::Reconnect);
    } else {
        transition(State::Connected_Idle, Event::Reconnect);
    }
}

void CollabState::onDisconnected() {
    snapshot_.submitInFlight = false;
    snapshot_.localDirtyDuringSubmit = false;
    snapshot_.submitRequestId.clear();
    snapshot_.applyingRemoteSnapshot = false;
    transition(State::Disconnected, Event::Disconnected);
}

void CollabState::reset() {
    snapshot_ = Snapshot();
    state_ = State::Disconnected;
}

} // namespace dbcad
