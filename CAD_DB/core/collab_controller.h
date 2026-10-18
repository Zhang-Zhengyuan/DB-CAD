#pragma once

// CollabController —— 协作编排器（架构 v2 核心库，REFACTOR_PLAN M1.3 + M2.1）。
//
// 职责：
//   * 持有 CollabState（版本状态机，单一真相源——镜像字段模式已删除）；
//   * 推送/拉取/打开/另存编排（原 mainwindow_json.cpp 与 load/save 的策略）；
//   * 本地编辑与远端通知的策略反应；
//   * M2.1 起全部网络编排异步化：入口 *Async 启动操作，结果经
//     *Finished 信号交还视图；无嵌套事件循环，界面不冻结。
//     在飞操作由 busy 闸门串行化（提交中另有状态机闸门）。
//
// 视图（MainWindow）只消费 Outcome 与信号；版本号/HTTP 细节不外泄。

#include <functional>
#include <vector>

#include <QObject>
#include <QString>
#include <QTimer>

#include "core/collab_backend.h"
#include "core/collab_state.h"
#include "kernel/document_model.h"

namespace dbcad {

class DocumentSession;

class CollabController : public QObject {
    Q_OBJECT
public:
    // 操作结果：ok 为整体成败；message 交视图显示；showDialog 决定
    // 状态栏还是模态对话框（保持 master 基线的 UX）。
    struct Outcome {
        bool ok = false;
        bool conflict = false;     // push 409，或 pull 撞上本地未提交修改
        bool nothingToDo = false;  // 无未推送修改 / 已是最新版本
        bool showDialog = false;
        QString message;
        int messageDuration = 4000;
        int version = 0;
        // 会话中被淘汰的显示缓存，视图负责 delete。
        std::vector<GmeMesh::DisplayData*> retiredDisplayData;
    };

    CollabController(DocumentSession& session, CollabBackend& backend, QObject* parent = nullptr);

    // ---------- 项目绑定 ----------
    void bindProject(const QString& projectId, const QString& projectName);
    // 断开通道并清空项目绑定（切回本地文件模式时调用；状态整体归零）。
    void leaveProject();
    QString projectId() const { return state_.projectId(); }
    QString projectName() const { return projectName_; }

    // ---------- 连接编排 ----------
    // （重）建立当前项目的 WS 通道；内部先走一次断开以复位状态机。
    void connectProject();
    void disconnectChannel();
    bool isConnected() const { return state_.isConnected(); }
    // WS 真正连上后由视图调用：状态机转 Connected_*（原 onReconnect(true) 时机）。
    void onChannelConnected();

    // ---------- 本地编辑策略 ----------
    void onLocalEdit();

    // ---------- WS 通知策略 ----------
    void onRemoteVersionNotified(int remoteVersion);

    // ---------- 异步操作 ----------
    // 入口立即返回；前置条件不满足时同步发 *Finished（错误 Outcome）。
    // busy 闸门：任一异步操作在飞时，新操作被拒绝（message 说明）。
    bool isBusy() const { return operationInFlight_; }
    QString busyMessage() const { return QString::fromUtf8("上一协作操作进行中，请稍候"); }

    void pushAsync(const QString& reason);
    void pullAsync();
    void openProjectAsync(const QString& projectName);
    void openProjectAtAsync(const QString& projectName, int generation);
    void saveToProjectAsync(const QString& projectName);

    // 拉取前的闸门（"正在提交"判断）。
    bool canPullNow(QString* reason) const;

    // ---------- 状态访问（单一真相源）----------
    const CollabState& state() const { return state_; }
    int modelVersion() const { return state_.modelVersion(); }
    int pendingRemoteVersion() const { return state_.pendingRemoteVersion(); }
    bool isSubmitInFlight() const { return state_.isSubmitInFlight(); }

signals:
    void stateChanged();                                    // 协作面板刷新
    void infoMessage(const QString& message, int duration); // 状态栏提示
    void pushFinished(const dbcad::CollabController::Outcome& outcome);
    void pullFinished(const dbcad::CollabController::Outcome& outcome);
    void openProjectFinished(const dbcad::CollabController::Outcome& outcome);
    void openProjectAtFinished(const dbcad::CollabController::Outcome& outcome);
    void saveToProjectFinished(const dbcad::CollabController::Outcome& outcome);

private:
    using Done = std::function<void(const Outcome&)>;
    // 串行闸门：在飞则同步回调拒绝并返回 false；成功占位返回 true。
    bool beginOperation(const Done& done);
    void finishOperation();
    // 执行体：不带闸门（由公共入口或持有闸门的编排链调用）。
    void executePush(const QString& reason, const Done& done);
    void executePull(const Done& done);
    void runOpenProject(const QString& projectName, const Done& done);
    void runOpenProjectAt(const QString& projectName, int generation, const Done& done);
    void runSaveToProject(const QString& projectName, const Done& done);
    // 异步等待 WS 连接（connected/error/5s 超时），一次性。
    void waitForChannelThen(std::function<void()> proceed);

    DocumentSession& session_;
    CollabBackend& backend_;
    CollabState state_;
    QString projectName_;
    bool operationInFlight_ = false;
    // saveToProject 等待 WS 连接的一次性接线与定时器。
    QTimer* saveWaitTimer_ = nullptr;
    std::vector<QMetaObject::Connection> saveWaitConnections_;
    bool saveWaitCompleted_ = true;
    void clearSaveWait();
};

} // namespace dbcad
