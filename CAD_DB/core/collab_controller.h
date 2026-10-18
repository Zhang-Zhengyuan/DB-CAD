#pragma once

// CollabController —— 协作编排器（架构 v2 核心库，REFACTOR_PLAN M1.3）。
//
// 职责：
//   * 持有 CollabState（版本状态机，单一真相源——镜像字段模式已删除）；
//   * 推送/拉取编排（原 mainwindow_json.cpp 的策略部分）；
//   * 打开/另存项目编排（原 loadFile/saveFile 的 FastAPI 分支策略）；
//   * 本地编辑与远端通知的策略反应（原 notifyModelChangedForCollaboration /
//     handleDeltaSyncEvent 的策略部分）。
//
// 视图（MainWindow）只消费 Outcome 与信号；版本号/HTTP 细节不外泄。
// 本阶段网络调用保持同步（M2 异步化）。

#include <QObject>
#include <QString>
#include <vector>

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

    // ---------- 推/拉 ----------
    Outcome push(const QString& reason);
    Outcome pull();

    // ---------- 打开/另存 ----------
    // 打开项目最新版本；项目不存在则创建空项目（原 loadFile(name) FastAPI 分支）。
    Outcome openProject(const QString& projectName);
    // 打开项目指定历史版本（原 loadFile(name, generation)）。
    Outcome openProjectAt(const QString& projectName, int generation);
    // 保存到项目（新项目收录全部实体，原 saveFile FastAPI 分支的策略）。
    Outcome saveToProject(const QString& projectName);

    // ---------- 状态访问（单一真相源）----------
    const CollabState& state() const { return state_; }
    int modelVersion() const { return state_.modelVersion(); }
    int pendingRemoteVersion() const { return state_.pendingRemoteVersion(); }
    bool isSubmitInFlight() const { return state_.isSubmitInFlight(); }

    // 拉取前的闸门（原 onCollabPullButtonClicked 的"正在提交"判断）。
    bool canPullNow(QString* reason) const;

signals:
    void stateChanged();                                    // 协作面板刷新
    void infoMessage(const QString& message, int duration); // 状态栏提示

private:
    DocumentSession& session_;
    CollabBackend& backend_;
    CollabState state_;
    QString projectName_;

    void emitInfo(const QString& message, int duration);
};

} // namespace dbcad
