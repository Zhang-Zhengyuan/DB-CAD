#pragma once

#include <QWidget>
#include <algorithm>
#include <vector>

#include "kernel/geometry_kernel.h"
#include "kernel/document_model.h"
#include "core/document_session.h"
#include "glwidget.h"
#include "mainwindow.h"

QT_BEGIN_NAMESPACE class QCheckBox;
class QLineEdit;
class QComboBox;
class QLabel;
class QPushButton;
class QTabWidget;
class QTreeWidget;
class QTreeWidgetItem;
QT_END_NAMESPACE

class MainWindow;

// Window —— 文档视图（架构 v2）：业务实体树、变更记账、远端合并都住在
// DocumentSession（core）里；本类只负责呈现（树控件/网格/属性页）与
// 交互式输入（样条草绘、句柄拖动），并把编辑后果通知 MainWindow。
class Window : public QWidget {
    Q_OBJECT

        friend class MainWindow;

public:
    Window(MainWindow* mw);
    void closeEvent(QCloseEvent* event) override;

    dbcad::DocumentSession& session() { return *session_; }
    const dbcad::DocumentSession& session() const { return *session_; }
    // 旧调用面（回归测试在用）：转发到会话。
    std::vector<ENTITY_TREE_ITEM>& getEntityTree() { return session_->entitiesMutable(); }

    void createEntity(const int id, dbcad::ShapeHandle& shape, dbcad::HandleGrid& handles);
    bool operation(const OPERATOR_TYPES ot, const int subOT, std::vector<int>& ids, dbcad::ShapeList& el, bool isHide = true);
    ENTITY_TREE_ITEM* addEntity(dbcad::ShapeHandle shape, const std::string name, int subOperatorType);
    ENTITY_TREE_ITEM* addEntity(dbcad::ShapeHandle shape, const std::string name, int subOperatorType, std::vector<int> index_base, OPERATOR_TYPES ot);
    ENTITY_TREE_ITEM* addEntity(dbcad::ShapeHandle shape, const std::string name, int subOperatorType, std::vector<std::vector<dbcad::Point3>>& handles);
    ENTITY_TREE_ITEM* addEntity(dbcad::ShapeHandle shape, const std::string name, int subOperatorType, std::vector<std::vector<dbcad::Point3>>& handles, std::vector<int> index_base, OPERATOR_TYPES ot = OPERATOR_TYPES::OPERATOR_CONSTRUCTOR);
    void changeEntity(dbcad::ShapeList e_list, QVector3D v);
    void changeEntity(dbcad::ShapeHandle shape, QVector3D v);
    void changeEntity(dbcad::ShapeHandle shape, dbcad::Translation t);
    void setCurrentEntity(dbcad::ShapeHandle shape) { glWidget->setPickedEntity(shape); }
    GLWidget::ViewState getViewState() const { return glWidget->viewState(); }
    void setViewState(const GLWidget::ViewState& state) { glWidget->setViewState(state); }
    ENTITY_TREE_ITEM* getEntityItemByIndex(int index);
    void updateMeshData();
    void visiablility();
    void displayInfo();
    void clear();
    // 远端合并/项目打开后的整体刷新：接管淘汰的显示缓存并复位撤销历史。
    void refreshAfterDocumentChange(std::vector<GmeMesh::DisplayData*> retiredDisplayData);
    void captureHistory();
    void resetDocumentHistory();
    void restoreHistory(int direction);
    std::vector<int> getSelectedEntities() { return selected_entities; }
    void setIsInputHandle(bool ih) { isInputHandle = ih; };
    bool getIsInputHandle() { return isInputHandle; }
    void addHandle(dbcad::ShapeHandle shape, double* p);
    bool getHandles(dbcad::ShapeHandle shape, std::vector<std::vector<dbcad::Point3>>& handles);
    bool getVisibility(dbcad::ShapeHandle shape);
    void changeHandle(dbcad::ShapeHandle shape, dbcad::Point3 p, QVector3D v);
    void setInputMode(INPUT_MODES im) { imode = im; }
    INPUT_MODES getInputMode() { return imode; }
    void setCurFile(QString s) { curFile = s; }
    QString getCurFile() { return curFile; }
    void setCurPartName(QString s) { curPartName = s; }
    QString getCurPartName() { return curPartName; }
    dbcad::ShapeList getEntityList();
    dbcad::ShapeList getSelectedEntityList();
    void setIsModified(bool im) { session_->setDirty(im); }
    bool getIsModified() { return session_->isDirty(); }
    QImage getScreenshot();
    void clearSelectedEntities() { selected_entities.clear(); }
    void removeFromSelectedEntities(int index) {
        selected_entities.erase(std::remove(selected_entities.begin(), selected_entities.end(), index), selected_entities.end());
    }
    void showMessage(QString s, int duration = -1);
    // 依赖驱动的下游重建（视图包装：会话重建 + 刷新）。
    void updateEntites(std::vector<int>& indices);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void displayTypeChanged();
    void updateTreeWidget();
    void addTreeItem(QTreeWidgetItem* item, std::vector<int> index_base);
    void addTabWidget();
    void updateTabWidget();
    void currentEntityChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
    void entityChanged(QTreeWidgetItem* item, int column);
    void onTreeContextMenu(const QPoint& pos);
    void isDisplayChanged();
    void setSurfaceAlpha(double sa);
    void setLineWidth(int lw);
    void setPointSize(int ps);

private:
    struct Snapshot {
        QJsonObject geometry;
        std::vector<int> drafts;
        std::vector<ENTITY_TREE_ITEM> items;
        int nextIndex=0;
    };
    std::vector<Snapshot> history;
    int historyPosition=-1;
    bool restoringHistory=false;
    MainWindow* mainWindow;
    dbcad::DocumentSession* session_; // 由 MainWindow 持有
    GLWidget* glWidget;
    QTabWidget* tabWidget;
    QString curFile;
    QString curPartName;

    // 交互相关
    INPUT_MODES imode = INPUT_MODES::SELECTION;
    bool isInputHandle = false;
    std::vector<dbcad::Point3> input_handles;

    // 实体树视图相关（业务数据在 DocumentSession）
    QTreeWidget* treeWidget;
    std::vector<int> selected_entities;
    int currentEntityIndex = -1;
    ENTITY_TREE_ITEM* currentEntityItem() { return getEntityItemByIndex(currentEntityIndex); }

    // 显示相关
    QCheckBox* displayCheckBox;
    QComboBox* displayComboBox;
};
