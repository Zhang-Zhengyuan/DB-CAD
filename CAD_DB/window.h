#pragma once

#include <QWidget>

#include "acis/include/lists.hxx"
#include "acis/include/position.hxx"
#include "acis/include/transf.hxx"
#include "glwidget.h"
#include "gme_mesh.hxx"
#include "gme_user_data.hxx"
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

struct ENTITY_TREE_ITEM {
    std::string name = "Unknown";                                        // ENTITY名称
    int index = 0;                                                       // 索引
    ENTITY* ptrEntity = nullptr;                                         // ENTITY本身
    OPERATOR_TYPES operatorType = OPERATOR_TYPES::OPERATOR_CONSTRUCTOR;  // 生成ENTITY的操作类型
    SPAtransf trans = SPAtransf();                                       // 记录基本体的变换操作
    int subOperatorType = 0;                                             // 生成ENTITY的子操作类型
    std::vector<std::vector<SPAposition>> handles;                       // 基本体手柄
    std::vector<int> index_base;                                         // 该ENTITY依赖的ENTITY
    bool isAutoUpdate = true;                                            // 依赖的ENTITY发生变化时是否自动更新
    std::vector<int> index_support;                                      // 依赖于该ENTITY的ENTITY
    GmeMesh::DisplayData* ptrDisplayData = nullptr;                      // 离散化数据
    bool visible = false;                                                // 是否显示
    DISPLAY_TYPES displayType = DISPLAY_TYPES::DISPLAY_ALL;              // 显示类型
    bool link_handles = true;                                            // 绘制时是否连接手柄
};

class Window : public QWidget {
    Q_OBJECT

        friend class MainWindow;

public:
    Window(MainWindow* mw);
    void closeEvent(QCloseEvent* event) override;
    std::vector<ENTITY_TREE_ITEM>& getEntityTree() { return entity_tree; }
    void createEntity(const int id, ENTITY*& ptrEntity, std::vector<std::vector<SPAposition>>& handles);
    bool operation(const OPERATOR_TYPES ot, const int subOT, std::vector<int>& ids, ENTITY_LIST*& el, bool isHide = true);
    void addTreeItem(ENTITY_TREE_ITEM item) { entity_tree.push_back(item); }
    ENTITY_TREE_ITEM* addEntity(ENTITY* ptrEntity, const std::string name, int subOperatorType);
    ENTITY_TREE_ITEM* addEntity(ENTITY* ptrEntity, const std::string name, int subOperatorType, std::vector<int> index_base, OPERATOR_TYPES ot);
    ENTITY_TREE_ITEM* addEntity(ENTITY* ptrEntity, const std::string name, int subOperatorType, std::vector<std::vector<SPAposition>>& handles);
    ENTITY_TREE_ITEM* addEntity(ENTITY* ptrEntity, const std::string name, int subOperatorType, std::vector<std::vector<SPAposition>>& handles, std::vector<int> index_base, OPERATOR_TYPES ot = OPERATOR_TYPES::OPERATOR_CONSTRUCTOR);
    void changeEntity(ENTITY_LIST e_list, QVector3D v);
    void changeEntity(ENTITY* ptrEntity, QVector3D v);
    void changeEntity(ENTITY* ptrEntity, SPAtransf t);
    void setCurrentEntity(ENTITY* ptrEntity) { glWidget->setPickedEntity(ptrEntity); }
    ENTITY_TREE_ITEM* getEntityItemByIndex(int index);
    void updateMeshData();
    void visiablility();
    void displayInfo();
    void clear();
    std::vector<int> getSelectedEntities() { return selected_entities; }
    void setIsInputHandle(bool ih) { isInputHandle = ih; };
    bool getIsInputHandle() { return isInputHandle; }
    void addHandle(ENTITY* ptrEntity, double* p);
    bool getHandles(ENTITY* ptrEntity, std::vector<std::vector<SPAposition>>& handles);
    bool getVisibility(ENTITY* ptrEntity);
    void changeHandle(ENTITY* ptrEntity, SPAposition p, QVector3D v);
    void setInputMode(INPUT_MODES im) { imode = im; }
    INPUT_MODES getInputMode() { return imode; }
    void setCurFile(QString s) { curFile = s; }
    QString getCurFile() { return curFile; }
    void setCurPartName(QString s) { curPartName = s; }
    QString getCurPartName() { return curPartName; }
    ENTITY_LIST getEntityList();
    ENTITY_LIST getSelectedEntityList();
    void setIsModified(bool im) { isModified = im; }
    bool getIsModified() { return isModified; }
    QImage getScreenshot();
    void clearSelectedEntities() { selected_entities.clear(); }
    void showMessage(QString s, int duration = -1);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void displayTypeChanged();
    void updateTreeWidget();
    void addTreeItem(QTreeWidgetItem* item, std::vector<int> index_base);
    void updateEntites(std::vector<int>& index);
    void addTabWidget();
    void updateTabWidget();
    void currentEntityChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
    void entityChanged(QTreeWidgetItem* item, int column);
    void isDisplayChanged();
    void setSurfaceAlpha(double sa);
    void setLineWidth(int lw);
    void setPointSize(int ps);

private:
    MainWindow* mainWindow;
    GLWidget* glWidget;
    QTabWidget* tabWidget;
    QString curFile;
    QString curPartName;
    bool isModified = false;

    // 交互相关
    INPUT_MODES imode = INPUT_MODES::SELECTION;
    bool isInputHandle = false;
    std::vector<SPAposition> input_handles;

    // ENTITY树相关
    QTreeWidget* treeWidget;
    std::vector<ENTITY_TREE_ITEM> entity_tree;
    int latest_index = 0;
    std::vector<int> selected_entities;
    ENTITY_TREE_ITEM* ptrCurrentTreeItem;

    // 显示相关
    QCheckBox* displayCheckBox;
    QComboBox* displayComboBox;
};
