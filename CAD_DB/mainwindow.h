#pragma once

#include <QHash>
#include <QList>
#include <QMainWindow>
#include <QSessionManager>
#include <QString>
#include <QtGlobal>
#include <memory>
#include <QColor>

#include "core/collab_backend.h"
#include "core/collab_controller.h"
#include "core/command_bus.h"
#include "core/document_session.h"
#include "core/sat_file_backend.h"
#include "kernel/model_types.h"

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

// 对话框式交互的可注入面（REFACTOR_PLAN M2.2）：无头化/换 UI 时替换实现，
// Qt 实现是默认值。核心库不感知。
class InteractionSurface {
public:
    virtual ~InteractionSurface() = default;
    enum class SaveDecision { Save, Discard, Cancel };
    virtual SaveDecision askSaveModified() { return SaveDecision::Cancel; } // 无头默认：取消
    virtual void showError(const QString& message) { Q_UNUSED(message); }
};

// MainWindow —— 视图接线（架构 v2 M1/M2）：协作编排住在 core（DocumentSession /
// CollabController / CollabBackend），本类只做菜单/面板/状态栏与对话框，
// 不持有版本号或 HTTP/WS 细节。M2.1 起全部协作操作异步：按钮启动操作，
// 结果经 *Finished 信号回到本类显示；界面不再整窗冻结。
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr, Qt::WindowFlags flags = {});

    // ---------- 供视图与回归测试的入口 ----------
    void setCurWindow(Window* w) { curWindow = w; }
    void showMessage(const QString s, int duration = -1);
    // 本地编辑后的协作状态通知（模式门控在此，策略在 CollabController）。
    void notifyModelChangedForCollaboration();
    // 协作友好的本地删除：会话删除 + 视图刷新（右键菜单/依赖重建共用）。
    void deleteEntityByIndexForCollaboration(int index);

    dbcad::DocumentSession& documentSession() { return *documentSession_; }
    dbcad::CollabController& collabController() { return *collabController_; }
    dbcad::CollabBackend& collabBackend() { return *collabBackend_; }
    dbcad::CommandBus& commandBus() { return *commandBus_; }
    // 当前存取后端（模式切换 = 后端选择，取代 if/else 分支）。
    dbcad::StorageBackend* activeStorage() const { return activeStorage_; }
    bool usingCollabStorage() const { return activeStorage_ == collabBackend_.get(); }

    // 对话框交互面注入（所有权归调用方；nullptr 恢复 Qt 实现）。
    void setInteractionSurface(InteractionSurface* surface);

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
    void setFastAPIConnectInfo();
    void visiablility();
    void displayInfo();
#ifndef QT_NO_SESSIONMANAGER
    void commitData(QSessionManager&);
#endif
    void insertElements(const OPERATOR_TYPES ot, const int subOperatorType);
    void operation(const OPERATOR_TYPES ot, const int subOperatorType);
    void clear();

    // 协作面板动作（启动异步操作，转发到 controller/backend）
    void onCollabPushButtonClicked();
    void onCollabPullButtonClicked();
    void requestFastAPISyncNow();
    void reconnectFastAPISync();
    void onCollabTagVersionClicked();

private:
    void createActions();
    void createStatusBar();
    void registerDefaultCommands();
    bool maybeSave();
    bool saveFile(const QString& fileName);
    void updateCollabPanelUi();
    void setCollabConnectionState(const QString& stateText);
    void setCurrentFile(const QString& fileName);
    void setCurrentPartName(const QString& partName);
    void handleSyncTextMessage(const QString& message);
    void handleBackendChannelState(const QString& stateKey);
    void handleBackendChannelConnected();
    void handleBackendChannelDisconnected();
    void handleBackendChannelError(const QString& message);
    void handleBackendCollaboratorsChanged();
    void displayOutcome(const dbcad::CollabController::Outcome& outcome);
    void showErrorDialog(const QString& message); // 经 InteractionSurface

    // 异步操作完成处理（M2.1）
    void handlePushFinished(const dbcad::CollabController::Outcome& outcome);
    void handlePullFinished(const dbcad::CollabController::Outcome& outcome);
    void handleOpenProjectFinished(const dbcad::CollabController::Outcome& outcome);
    void handleOpenProjectAtFinished(const dbcad::CollabController::Outcome& outcome);
    void handleSaveToProjectFinished(const dbcad::CollabController::Outcome& outcome);

    Window* curWindow;
    QAction* setACISModeAct;
    QAction* setFASTAPIModeAct;
    QActionGroup* setModeActGroup;

    // ---------- 核心库（组合，视图无权直写） ----------
    std::unique_ptr<dbcad::DocumentSession> documentSession_;
    std::unique_ptr<dbcad::CollabBackend> collabBackend_;
    std::unique_ptr<dbcad::CollabController> collabController_;
    std::unique_ptr<dbcad::SatFileBackend> satBackend_;
    std::unique_ptr<dbcad::CommandBus> commandBus_;
    dbcad::StorageBackend* activeStorage_ = nullptr;

    // ---------- 对话框交互面 ----------
    std::unique_ptr<InteractionSurface> defaultInteraction_; // Qt 实现（默认）
    InteractionSurface* interaction_ = nullptr;

    // ---------- 异步操作上下文（完成后使用） ----------
    QString pendingOpenName_;
    QString pendingSaveName_;

    // ---------- M4.2 在场感知 ----------
    QSet<QString> announcedIntents_;               // 本端已广播 begin 的组件
    QHash<QString, QString> remoteEditIntents_;    // uuid → 远端作者
    void announceEditIntentEnds();
    void handleEditIntentMessage(const QJsonObject& root);

    // ---------- 协作面板（纯视图） ----------
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
};
