#include "window.h"

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QCloseEvent>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTemporaryFile>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QUuid>
#include <QSet>
#include <QSignalBlocker>
#include <QJsonArray>
#include <cstdio>
#include <set>
#include <functional>
#include <unordered_map>

using namespace std;
using namespace dbcad;

Window::Window(MainWindow* mw) : mainWindow(mw), session_(&mw->documentSession()) {
    // 核心库的消息（验证失败/内核异常）直达状态栏。
    session_->setMessageSink([this](const QString& message) { showMessage(message, 5000); });

    glWidget = new GLWidget(this);
    currentEntityIndex = -1;
    selected_entities.clear();

    treeWidget = new QTreeWidget(this);
    const QStringList headers({ tr("实体") });
    treeWidget->setHeaderLabels(headers);
    treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(treeWidget, &QTreeWidget::currentItemChanged, this, &Window::currentEntityChanged);
    connect(treeWidget, &QTreeWidget::itemChanged, this, &Window::entityChanged);
    connect(treeWidget, &QTreeWidget::customContextMenuRequested,
            this, &Window::onTreeContextMenu);
    updateTreeWidget();

    tabWidget = new QTabWidget(this);
    addTabWidget();

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(1, 1);
    mainLayout->addWidget(glWidget, 0, 0, 2, 2);
    mainLayout->addWidget(treeWidget, 0, 2);
    mainLayout->addWidget(tabWidget, 1, 2);
    setLayout(mainLayout);

    setWindowTitle(tr("DBCAD实例"));
    resetDocumentHistory();
}

void Window::currentEntityChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous) {
    if (current == nullptr) { currentEntityIndex=-1; return; }
    int index = current->text(1).toInt();
    for (auto& eti : session_->entitiesMutable()) {
        if (eti.index == index) {
            currentEntityIndex = eti.index;
            updateTabWidget();
        }
    }
}

void Window::entityChanged(QTreeWidgetItem* item, int column) {
    if (item == nullptr) return;
    int index = item->text(1).toInt();
    for (auto& eti : session_->entitiesMutable()) {
        if (eti.index == index) {
            if (item->checkState(0) == Qt::Checked && std::find(selected_entities.begin(), selected_entities.end(), index) == selected_entities.end())
                selected_entities.push_back(eti.index);
            else if (item->checkState(0) == Qt::Unchecked)
                selected_entities.erase(std::remove(selected_entities.begin(), selected_entities.end(), index), selected_entities.end());

            std::string name = item->text(0).toStdString();
            if(!name.empty() && name!=eti.name) {
                eti.name=name;
                session_->recordEntityModified(eti.index);
                session_->setDirty(true);
                captureHistory();
                mainWindow->notifyModelChangedForCollaboration();
            }
        }
    }
}

// 右键菜单：实体树条目 → 删除选中实体（协作增量同步友好的入口）
void Window::onTreeContextMenu(const QPoint& pos) {
    if (treeWidget == nullptr) return;
    QTreeWidgetItem* item = treeWidget->itemAt(pos);
    if (item == nullptr) return;

    QMenu menu(this);
    QAction* delAct = menu.addAction(tr("删除选中实体"));
    QAction* chosen = menu.exec(treeWidget->viewport()->mapToGlobal(pos));
    if (chosen != delAct) return;

    // 从 tree 当前项反查 index，再交回 MainWindow 处理
    int idx = item->text(1).toInt();
    if (mainWindow != nullptr) {
        mainWindow->deleteEntityByIndexForCollaboration(idx);
    }
}

ShapeList Window::getEntityList() {
    return session_->shapeList();
}

ShapeList Window::getSelectedEntityList() {
    return session_->selectedShapeList(selected_entities);
}

QImage Window::getScreenshot() {
    if (glWidget) return glWidget->grabFramebuffer();
    return QImage();
}

void Window::addTabWidget() {
    // 显示
    QWidget* displayWidget = new QWidget(this);

    displayCheckBox = new QCheckBox(tr("显示"), this);
    displayCheckBox->setChecked(true);
    connect(displayCheckBox, &QCheckBox::stateChanged, this, &Window::isDisplayChanged);

    displayComboBox = new QComboBox(this);
    displayComboBox->setFixedWidth(100);
    connect(displayComboBox, &QComboBox::activated, this, &Window::displayTypeChanged);
    displayComboBox->addItem(tr("面"), DISPLAY_TYPES::DISPLAY_FACE);
    displayComboBox->addItem(tr("边"), DISPLAY_TYPES::DISPLAY_EDGE);
    displayComboBox->addItem(tr("全部"), DISPLAY_TYPES::DISPLAY_ALL);
    displayComboBox->setCurrentIndex(DISPLAY_TYPES::DISPLAY_ALL);
    QLabel* displayLabel = new QLabel(tr("类型"));
    displayLabel->setBuddy(displayComboBox);

    QDoubleSpinBox* alphaSpinBox = new QDoubleSpinBox(this);
    alphaSpinBox->setFixedWidth(100);
    alphaSpinBox->setRange(0.0, 1.0);
    alphaSpinBox->setSingleStep(0.1);
    alphaSpinBox->setValue(1.0);
    connect(alphaSpinBox, &QDoubleSpinBox::valueChanged, this, &Window::setSurfaceAlpha);
    QLabel* alphaLabel = new QLabel(tr("面透明度"));
    alphaLabel->setBuddy(alphaSpinBox);

    QSpinBox* widthSpinBox = new QSpinBox(this);
    widthSpinBox->setFixedWidth(100);
    widthSpinBox->setRange(1, 20);
    widthSpinBox->setSingleStep(2);
    widthSpinBox->setValue(2);
    connect(widthSpinBox, &QSpinBox::valueChanged, this, &Window::setLineWidth);
    QLabel* widthLabel = new QLabel(tr("边宽度"));
    widthLabel->setBuddy(widthSpinBox);

    QSpinBox* sizeSpinBox = new QSpinBox(this);
    sizeSpinBox->setFixedWidth(100);
    sizeSpinBox->setRange(1, 100);
    sizeSpinBox->setSingleStep(5);
    sizeSpinBox->setValue(10);
    connect(sizeSpinBox, &QSpinBox::valueChanged, this, &Window::setPointSize);
    QLabel* sizeLabel = new QLabel(tr("点大小"));
    sizeLabel->setBuddy(sizeSpinBox);

    QGridLayout* displayLayout = new QGridLayout;
    displayLayout->addWidget(displayCheckBox, 0, 0, Qt::AlignLeft);
    displayLayout->addWidget(displayLabel, 1, 0, Qt::AlignLeft);
    displayLayout->addWidget(displayComboBox, 1, 1, Qt::AlignLeft);
    displayLayout->addWidget(alphaLabel, 3, 0, Qt::AlignLeft);
    displayLayout->addWidget(alphaSpinBox, 3, 1, Qt::AlignLeft);
    displayLayout->addWidget(widthLabel, 4, 0, Qt::AlignLeft);
    displayLayout->addWidget(widthSpinBox, 4, 1, Qt::AlignLeft);
    displayLayout->addWidget(sizeLabel, 5, 0, Qt::AlignLeft);
    displayLayout->addWidget(sizeSpinBox, 5, 1, Qt::AlignLeft);
    displayLayout->setRowStretch(6, 1);

    displayWidget->setLayout(displayLayout);

    tabWidget->addTab(displayWidget, tr("显示"));
}

void Window::isDisplayChanged() {
    if (currentEntityItem() == nullptr) {
        QMessageBox::information(this, tr("未选择实体"), tr("请先在实体树窗口选择一个实体。"));
        return;
    }
    currentEntityItem()->visible = displayCheckBox->isChecked();
    this->updateMeshData();
}

void Window::updateTabWidget() {
    // 显示
    if (nullptr == currentEntityItem()) return;

    QSignalBlocker blockVisibility(displayCheckBox);
    QSignalBlocker blockDisplay(displayComboBox);
    displayCheckBox->setChecked(currentEntityItem()->visible);
    displayComboBox->setCurrentIndex(currentEntityItem()->displayType);
}


void Window::setSurfaceAlpha(double sa) {
    glWidget->setSurfaceAlpha(sa);
}

void Window::setLineWidth(int lw) {
    glWidget->setLineWidth(lw);
}

void Window::setPointSize(int ps) {
    glWidget->setPointSize(ps);
}

void Window::addTreeItem(QTreeWidgetItem* item, std::vector<int> index_base) {
    std::set<int> ancestors;
    for(auto parent=item;parent;parent=parent->parent()) ancestors.insert(parent->text(1).toInt());
    for (auto index : index_base) {
        if(ancestors.contains(index)) continue;
        for (auto& eti : session_->entitiesMutable()) {
            if (eti.index == index) {
                QTreeWidgetItem* i = new QTreeWidgetItem(item);
                i->setFlags(i->flags() | Qt::ItemIsEditable);
                i->setCheckState(0, std::find(selected_entities.begin(), selected_entities.end(), index) == selected_entities.end() ? Qt::Unchecked : Qt::Checked);
                i->setText(0, QString::fromStdString(eti.name));
                i->setText(1, QString::number(eti.index));
                this->addTreeItem(i, eti.index_base);
            }
        }
    }
}

void Window::updateTreeWidget() {
    if (nullptr == treeWidget) return;
    QSignalBlocker blockTreeSignals(treeWidget);
    treeWidget->clear();
    QList<QTreeWidgetItem*> items;
    for (auto& eti : session_->entitiesMutable()) {
        if (eti.index_support.size() == 0) {
            QTreeWidgetItem* item = new QTreeWidgetItem();
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            item->setCheckState(0, std::find(selected_entities.begin(), selected_entities.end(), eti.index) == selected_entities.end() ? Qt::Unchecked : Qt::Checked);
            item->setText(0, QString::fromStdString(eti.name));
            item->setText(1, QString::number(eti.index));
            this->addTreeItem(item, eti.index_base);
            items.append(item);
        }
    }
    treeWidget->insertTopLevelItems(0, items);
    treeWidget->expandAll();
}

void Window::closeEvent(QCloseEvent* event) {
    try { clear(); event->accept(); }
    catch(const std::exception& e) { showMessage(QString::fromUtf8(e.what()),5000); event->ignore(); }
}

void Window::clear() {
    auto retired = session_->clearAll();
    for (auto* display : retired) delete display;
    input_handles.clear(); selected_entities.clear(); currentEntityIndex=-1;
    imode=SELECTION;
    updateTreeWidget(); glWidget->clear();
    captureHistory();
}

void Window::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Escape) {
        imode=SELECTION; input_handles.clear(); setCurrentEntity({});
        glWidget->update();
    }
    else
        QWidget::keyPressEvent(e);
}

void Window::visiablility() {
    for (auto& e : session_->entitiesMutable()) e.visible = !e.visible;
    this->updateTreeWidget();
    this->updateMeshData();
}

void Window::displayInfo() {
    if (currentEntityItem()) {
        session_->kernel().statistics(currentEntityItem()->shape);
    }
}

void Window::displayTypeChanged() {
    if (currentEntityItem()) {
        currentEntityItem()->displayType = (DISPLAY_TYPES)displayComboBox->currentIndex();
        if (currentEntityItem()->ptrDisplayData) currentEntityItem()->ptrDisplayData->displayType = currentEntityItem()->displayType;
    } else {
        QMessageBox::information(this, tr("未选择实体"), tr("请先在实体树窗口选择一个实体。"));
        return;
    }
    glWidget->update();
}

void Window::createEntity(const int id, ShapeHandle& shape, HandleGrid& handles) {
    if(id==E_BSPLINE && handles.empty()) {
        if(imode==INPUT_SPLINE) { imode=SELECTION; return; }
        imode=INPUT_SPLINE; input_handles.clear();
        shape=session_->kernel().draft(); glWidget->setPickedEntity(shape); return;
    }
    try { auto candidate=handles; shape=session_->kernel().create(id,candidate); handles=std::move(candidate); }
    catch(const std::exception& e) { showMessage(QString::fromUtf8(e.what()),5000); }
}

bool Window::operation(OPERATOR_TYPES type,int subtype,std::vector<int>& ids,ShapeList& outputs,bool updateDocument) {
    std::vector<GmeMesh::DisplayData*> retired;
    const bool ok = session_->operate(type, subtype, ids, outputs, updateDocument, retired);
    for (auto* display : retired) delete display;
    if (ok && updateDocument) {
        currentEntityIndex=-1;
        clearSelectedEntities();
        setCurrentEntity({});
    }
    return ok;
}

ENTITY_TREE_ITEM* Window::addEntity(ShapeHandle shape, const std::string name, int subOperatorType) {
    std::vector<std::vector<Point3>> handles;
    std::vector<int> index;
    return addEntity(shape, name, subOperatorType, handles, index);
}

ENTITY_TREE_ITEM* Window::addEntity(ShapeHandle shape, const std::string name, int subOperatorType, std::vector<int> index_base, OPERATOR_TYPES ot) {
    std::vector<std::vector<Point3>> handles;
    return addEntity(shape, name, subOperatorType, handles, index_base, ot);
}

ENTITY_TREE_ITEM* Window::addEntity(ShapeHandle shape, const std::string name, int subOperatorType, std::vector<std::vector<Point3>>& handles) {
    std::vector<int> index_base;
    return addEntity(shape, name, subOperatorType, handles, index_base);
}

ENTITY_TREE_ITEM* Window::addEntity(ShapeHandle shape, const std::string name, int sot, std::vector<std::vector<Point3>>& handles, std::vector<int> index_base, OPERATOR_TYPES ot) {
    auto* added = session_->addEntity(shape, name, sot, handles, index_base, ot);
    if (added == nullptr) return nullptr;
    this->updateTreeWidget();
    this->updateMeshData();
    mainWindow->notifyModelChangedForCollaboration();
    return added;
}

void Window::updateMeshData() {
    std::vector<GmeMesh::DisplayData*>& md = glWidget->getMeshData();
    md.clear();
    size_t entitiesWithNoMesh = 0;
    for (size_t idx = 0; idx < session_->entities().size(); ++idx) {
        auto& e = session_->entitiesMutable()[idx];
        if (e.visible && session_->kernel().kind(e.shape)!=ShapeKind::Unknown) {
            if (e.ptrDisplayData == nullptr) {
                GmeMesh::DisplayData* dd = new GmeMesh::DisplayData;
                // 生成面片数据
                if (session_->kernel().mesh(e.shape, *dd)) {
                    dd->displayType = e.displayType;
                    e.ptrDisplayData = dd;
                    md.push_back(dd);
                } else {
                    ++entitiesWithNoMesh;
                    delete dd;
                }
                session_->setDirty(true);
            } else {
                md.push_back(e.ptrDisplayData);
            }
        }
    }
    glWidget->updateMeshData();
}

void Window::addHandle(ShapeHandle shape, double* p) {
    for (auto& eti : session_->entitiesMutable()) {
        if (eti.shape == shape) {
            auto pointsBefore=input_handles;
            input_handles.push_back(Point3(p));
            auto previous=eti.shape;
            HandleGrid splineHandles{input_handles};
            ShapeHandle curve;
            bool update=input_handles.size()>1;
            try { curve=update?session_->kernel().create(E_BSPLINE,splineHandles):session_->kernel().draft(); }
            catch(const std::exception& e) { input_handles=std::move(pointsBefore); showMessage(QString::fromUtf8(e.what()),5000); return; }
            eti.shape=curve;
            session_->kernel().erase({previous});
            std::vector<std::vector<Point3>> points;
            points.push_back(input_handles);
            eti.handles = points;
            glWidget->setPickedEntity(curve);
            if (eti.ptrDisplayData) delete eti.ptrDisplayData;
            eti.ptrDisplayData = nullptr;
            this->updateMeshData();
            session_->recordEntityModified(eti.index);
            captureHistory();
            mainWindow->notifyModelChangedForCollaboration();
            return;
        }
    }
}

bool Window::getHandles(ShapeHandle shape, std::vector<std::vector<Point3>>& handles) {
    for (const auto& eti : session_->entities()) {
        if (eti.shape == shape) {
            handles = eti.handles;
            return true;
        }
    }
    return false;
}

bool Window::getVisibility(ShapeHandle shape) {
    for (const auto& eti : session_->entities()) {
        if (eti.shape == shape) {
            return eti.visible;
        }
    }
    return false;
}

void Window::changeHandle(ShapeHandle shape, Point3 p, QVector3D v) {
    for (auto& eti : session_->entitiesMutable()) {
        if (eti.shape == shape) {
            auto previousHandles=eti.handles;
            for (auto& hps : eti.handles) {
                for (auto& hp : hps) {
                    if (hp == p) {
                        hp += Point3(v.x(), v.y(), v.z());
                        glWidget->setPickedHandle(hp);
                    }
                }
            }
            ShapeHandle shape{};
            this->createEntity(eti.subOperatorType, shape, eti.handles);
            if (!shape) { eti.handles=std::move(previousHandles); return; }
            session_->kernel().copyColor(eti.shape,shape);
            session_->kernel().erase({eti.shape});
            eti.shape=shape;
            session_->recordEntityModified(eti.index);
            glWidget->setPickedEntity(eti.shape);

            if (eti.ptrDisplayData) delete eti.ptrDisplayData;
            eti.ptrDisplayData = nullptr;
            auto downstream=eti.index_support;
            updateEntites(downstream);
            this->updateMeshData();
            captureHistory();
            mainWindow->notifyModelChangedForCollaboration();
            return;
        }
    }
}

void Window::changeEntity(ShapeList e_list, QVector3D v) {
    for (auto e : e_list) {
        this->changeEntity(e, v);
    }
}

void Window::changeEntity(ShapeHandle shape, QVector3D v) {
    Translation t{Point3(v.x(), v.y(), v.z())};
    changeEntity(shape, t);
}

void Window::changeEntity(ShapeHandle shape, Translation t) {
    if (!shape) return;
    try { session_->kernel().translate(shape,t); }
    catch(const std::exception& e) { showMessage(QString::fromUtf8(e.what()),5000); return; }
    for (auto& eti : session_->entitiesMutable()) {
        if (eti.shape == shape) {
            if (eti.operatorType != OPERATOR_CONSTRUCTOR || eti.subOperatorType < E_BSPLINE_CTRLPTS) eti.trans *= t;
            for (auto& h_u : eti.handles)
                for (auto& h : h_u) {
                    h += t.translation();
                }
            // 记录实体被修改（用于增量同步）
            session_->recordEntityModified(eti.index);
        }
    }
    std::vector<int> downstream;
    for (auto& eti : session_->entitiesMutable()) if(eti.shape==shape) {
        delete eti.ptrDisplayData; eti.ptrDisplayData=nullptr;
        downstream=eti.index_support; break;
    }
    updateEntites(downstream);
    this->updateMeshData();
    captureHistory();
    mainWindow->notifyModelChangedForCollaboration();
}

void Window::updateEntites(std::vector<int>& indices) {
    std::vector<GmeMesh::DisplayData*> retired;
    session_->rebuildDownstream(indices, retired);
    for (auto* display : retired) delete display;
}

ENTITY_TREE_ITEM* Window::getEntityItemByIndex(int index) {
    return session_->itemByIndex(index);
}

void Window::showMessage(QString s, int duration) {
    mainWindow->showMessage(s, duration);
}

void Window::refreshAfterDocumentChange(std::vector<GmeMesh::DisplayData*> retiredDisplayData) {
    for (auto* display : retiredDisplayData) delete display;
    // 远端编辑使本地撤销快照失效——不能让 undo 撤掉同事的修改。
    resetDocumentHistory();
    currentEntityIndex = -1;
    clearSelectedEntities();
    setCurrentEntity({});
    updateTreeWidget();
    updateMeshData();
}

void Window::resetDocumentHistory() {
    history.clear(); historyPosition=-1;
    captureHistory();
}
void Window::captureHistory() {
    if(restoringHistory) return;
    Snapshot snapshot;
    try {
        session_->kernel().noteState();
        ShapeList shapes; QHash<int,QString> uuids;
        const auto& entities = session_->entities();
        for(size_t i=0;i<entities.size();++i) {
            const auto& item=entities[i];
            if(session_->kernel().kind(item.shape)==ShapeKind::Unknown) { snapshot.drafts.push_back(static_cast<int>(i)); continue; }
            uuids.insert(static_cast<int>(shapes.size()),QString::fromStdString(item.uuid));
            shapes.push_back(item.shape);
        }
        if(!shapes.empty()) {
            snapshot.geometry=session_->kernel().encodeGraph(uuids,shapes);
            auto nodes=snapshot.geometry.value("nodes").toArray();
            for(int i=0;i<nodes.size();++i) {
                auto node=nodes[i].toObject(); auto props=node.value("props").toObject();
                if(props.value("top_level").toBool()) { props.insert("fallback","sat_fragment"); node.insert("props",props); nodes[i]=node; }
            }
            snapshot.geometry.insert("nodes",nodes);
        }
    } catch(const std::exception& e) { showMessage(QString::fromUtf8(e.what()),5000); return; }
    snapshot.items=session_->entities(); snapshot.nextIndex=session_->nextIndex();
    for(auto& item:snapshot.items) { item.ptrDisplayData=nullptr; item.shape={}; }
    history.resize(static_cast<size_t>(historyPosition+1));
    history.push_back(std::move(snapshot));
    // Bound session memory: each entry includes a lossless coarse backup.
    if(history.size()>50) history.erase(history.begin());
    historyPosition=static_cast<int>(history.size())-1;
}
void Window::restoreHistory(int direction) {
    int target=historyPosition+direction;
    if(target<0 || target>=static_cast<int>(history.size())) return;
    auto& snapshot=history[static_cast<size_t>(target)];
    ShapeList restored;
    DecodedGraph decoded; QString error;
    if(!snapshot.geometry.isEmpty() && !session_->kernel().decodeGraph(snapshot.geometry,decoded,error)) {
        showMessage(error,5000); return;
    }
    for(size_t i=0;i<snapshot.items.size();++i) {
        if(std::find(snapshot.drafts.begin(),snapshot.drafts.end(),static_cast<int>(i))!=snapshot.drafts.end()) restored.push_back(session_->kernel().draft());
        else restored.push_back(decoded.shapes.value(QString::fromStdString(snapshot.items[i].uuid)));
    }
    if(std::find(restored.begin(),restored.end(),ShapeHandle{})!=restored.end()) {
        ShapeList cleanup; for(auto h:restored) if(h) cleanup.push_back(h);
        session_->kernel().erase(cleanup); showMessage(tr("历史几何与文档标识不匹配"),5000); return;
    }
    session_->kernel().erase(session_->shapeList());
    // M2.4 记账对齐：撤销恢复按 uuid 差量化记账——消失的记 REMOVE、新增/复活的
    // 记 ADD、存续的记 MODIFY（不再对全树 REMOVE+ADD 全量记账；存续组件经 MODIFY
    // 走组件替换，复活组件按 ADD 覆盖此前的 REMOVE，推送语义与基线一致）。
    QSet<QString> restoredUuids;
    for (const auto& item : snapshot.items) restoredUuids.insert(QString::fromStdString(item.uuid));
    QSet<QString> previousUuids;
    for (const auto& item : session_->entitiesMutable()) previousUuids.insert(QString::fromStdString(item.uuid));
    for (auto& item : session_->entitiesMutable()) {
        const QString uuid = QString::fromStdString(item.uuid);
        if (!restoredUuids.contains(uuid))
            session_->recordEntityRemoved(item.index);
        delete item.ptrDisplayData;
        item.ptrDisplayData = nullptr;
    }
    session_->entitiesMutable()=snapshot.items;
    for(size_t i=0;i<session_->entities().size();++i) {
        auto& item=session_->entitiesMutable()[i]; item.shape=restored[i];
        const QString uuid = QString::fromStdString(item.uuid);
        if (previousUuids.contains(uuid))
            session_->recordEntityModified(item.index);
        else
            session_->recordEntityAdded(uuid,QString::fromStdString(item.name),"body",item.index);
    }
    currentEntityIndex=-1; selected_entities.clear(); input_handles.clear(); imode=SELECTION;
    setCurrentEntity({});
    historyPosition=target;
    updateTreeWidget(); updateMeshData(); session_->setDirty(true);
    mainWindow->notifyModelChangedForCollaboration();
}
