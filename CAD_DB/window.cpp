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
#include <QSignalBlocker>
#include <QJsonArray>
#include <cstdio>
#include <set>
#include <functional>
#include <unordered_map>

#include "collab_session.h"

using namespace std;
using namespace dbcad;

Window::Window(MainWindow* mw) : mainWindow(mw) {
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
    for (auto& eti : entity_tree) {
        if (eti.index == index) {
            currentEntityIndex = eti.index;
            updateTabWidget();
        }
    }
}

void Window::entityChanged(QTreeWidgetItem* item, int column) {
    if (item == nullptr) return;
    int index = item->text(1).toInt();
    for (auto& eti : entity_tree) {
        if (eti.index == index) {
            if (item->checkState(0) == Qt::Checked && std::find(selected_entities.begin(), selected_entities.end(), index) == selected_entities.end())
                selected_entities.push_back(eti.index);
            else if (item->checkState(0) == Qt::Unchecked)
                selected_entities.erase(std::remove(selected_entities.begin(), selected_entities.end(), index), selected_entities.end());

            std::string name = item->text(0).toStdString();
            if(!name.empty() && name!=eti.name) {
                eti.name=name;
                mainWindow->recordEntityModified(eti.index);
                isModified=true;
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
    ShapeList el;
    for (auto eti : entity_tree) {
        el.push_back(eti.shape);
    }
    return el;
}

ShapeList Window::getSelectedEntityList() {
    ShapeList el;
    for (auto selected_id : selected_entities) {
        auto entity = getEntityItemByIndex(selected_id);
        if(entity) el.push_back(entity->shape);
    }
    return el;
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
        for (auto eti : entity_tree) {
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
    for (auto eti : entity_tree) {
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
    kernel().erase(getEntityList());
    for(auto& item:entity_tree) { mainWindow->recordEntityRemoved(item.index); delete item.ptrDisplayData; }
    entity_tree.clear();
    input_handles.clear(); selected_entities.clear(); currentEntityIndex=-1;
    imode=SELECTION;
    updateTreeWidget(); glWidget->clear();
    captureHistory();
    isModified=true;
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
    for (auto& e : entity_tree) e.visible = !e.visible;
    this->updateTreeWidget();
    this->updateMeshData();
}

void Window::displayInfo() {
    if (currentEntityItem()) {
        kernel().statistics(currentEntityItem()->shape);
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
        shape=kernel().draft(); glWidget->setPickedEntity(shape); return;
    }
    try { auto candidate=handles; shape=kernel().create(id,candidate); handles=std::move(candidate); }
    catch(const std::exception& e) { showMessage(QString::fromUtf8(e.what()),5000); }
}

bool Window::operation(OPERATOR_TYPES type,int subtype,std::vector<int>& ids,ShapeList& outputs,bool updateDocument) {
    if(type!=OPERATOR_DEFEATURE && ids.size()!=2) { showMessage(tr("此运算需要恰好两个实体"),4000); return false; }
    if(std::set<int>(ids.begin(),ids.end()).size()!=ids.size()) { showMessage(tr("不能重复选择同一实体"),4000); return false; }
    if(updateDocument && type==OPERATOR_BOOLEAN) {
        for(auto id:ids) if(auto item=getEntityItemByIndex(id); item && !item->index_support.empty()) {
            showMessage(tr("实体仍有依赖结果，请先删除依赖结果再进行布尔运算"),5000); return false;
        }
    }
    ShapeList inputs;
    for(auto id:ids) { auto item=getEntityItemByIndex(id); if(item && item->shape) inputs.push_back(item->shape); }
    try { outputs=kernel().operate(type,subtype,inputs); }
    catch(const std::exception& e) { showMessage(QString::fromUtf8(e.what()),5000); return false; }
    if(outputs.empty()) { showMessage(tr("运算未产生结果"),4000); return false; }
    if(updateDocument) {
        if(type==OPERATOR_BOOLEAN) {
            try { kernel().erase({inputs[0],inputs[1]}); }
            catch(const std::exception& e) { kernel().erase(outputs); outputs.clear(); showMessage(QString::fromUtf8(e.what()),5000); return false; }
            for(auto it=entity_tree.begin();it!=entity_tree.end();) {
                if(std::find(ids.begin(),ids.begin()+2,it->index)!=ids.begin()+2) {
                    mainWindow->recordEntityRemoved(it->index);
                    delete it->ptrDisplayData; it=entity_tree.erase(it);
                } else ++it;
            }
            // Remove dependency references to consumed input rows.
            for(auto& item:entity_tree) {
                auto consumed=[&](int id) { return id==ids[0] || id==ids[1]; };
                std::erase_if(item.index_base,consumed);
                std::erase_if(item.index_support,consumed);
            }
        } else {
            for(auto id:ids) if(auto item=getEntityItemByIndex(id)) item->visible=false;
        }
        currentEntityIndex=-1;
        clearSelectedEntities();
        setCurrentEntity({});
    }
    return true;
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
    std::fprintf(stderr, "[Collab][DEBUG] >>> Window::addEntity ENTER name=%s shape=%llu sot=%d index_base.size=%zu ot=%d\n",
                 name.c_str(), static_cast<unsigned long long>(shape.value), sot, index_base.size(), (int)ot);
    if (!shape) {
        std::fprintf(stderr, "[Collab][DEBUG] <<< Window::addEntity EXIT (null shape)\n");
        return nullptr;
    }
    ENTITY_TREE_ITEM eti;
    eti.uuid = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    eti.name = name;
    // Index is an identity used by dependency lists and collaboration
    // bookkeeping.  entity_tree.size() is not stable after a deletion (it
    // would reuse an existing index and make two rows address the same
    // entity), so allocate from the monotonic counter instead.
    eti.index = latest_index++;
    eti.shape = shape;
    eti.handles = handles;
    eti.visible = true;
    eti.operatorType = ot;
    eti.subOperatorType = sot;
    eti.index_base = index_base;
    entity_tree.push_back(eti);
    std::fprintf(stderr, "[Collab][DEBUG] Window::addEntity after entity_tree.push_back, size=%zu\n", entity_tree.size());
    for (auto index : index_base) {
        for (auto& e : entity_tree) {
            if (e.index == index) e.index_support.push_back(eti.index);
        }
    }
    std::fprintf(stderr, "[Collab][DEBUG] Window::addEntity before updateTreeWidget\n");
    this->updateTreeWidget();
    std::fprintf(stderr, "[Collab][DEBUG] Window::addEntity after updateTreeWidget, before updateMeshData\n");
    this->updateMeshData();
    std::fprintf(stderr, "[Collab][DEBUG] Window::addEntity after updateMeshData\n");
    mainWindow->notifyModelChangedForCollaboration();
    std::fprintf(stderr, "[Collab][DEBUG] Window::addEntity after notifyModelChangedForCollaboration\n");

    // 只有在"非 apply remote snapshot / 非 publishing snapshot"时才记录增量变更，
    // 否则会把远端实体的"添加"动作记录成本地 ADD change，下次提交造成回环上传。
    auto& session = CollabSession::instance();
    std::fprintf(stderr, "[Collab][DEBUG] Window::addEntity isApplyingRemote=%d\n",
                 session.isApplyingRemoteSnapshot() ? 1 : 0);
    if (!session.isApplyingRemoteSnapshot()) {
        mainWindow->recordEntityAdded(
            QString::fromStdString(eti.uuid),
            QString::fromStdString(eti.name),
            QString::fromStdString(eti.name),
            eti.index);
    }

    std::fprintf(stderr, "[Collab][DEBUG] <<< Window::addEntity EXIT OK logical_index=%d vector_index=%zu\n",
                 eti.index, entity_tree.size() - 1);
    // `ENTITY_TREE_ITEM::index` is a monotonic logical identity.  It is not
    // the current std::vector position after an entity has been deleted.
    // Returning entity_tree[eti.index] therefore asserts as soon as a new
    // entity receives an index larger than the compacted vector size.
    return &entity_tree.back();
}

void Window::updateMeshData() {
    std::fprintf(stderr, "[Collab][DEBUG] >>> Window::updateMeshData ENTER entity_tree.size=%zu\n", entity_tree.size());
    std::vector<GmeMesh::DisplayData*>& md = glWidget->getMeshData();
    md.clear();
    size_t entitiesWithNoMesh = 0;
    for (size_t idx = 0; idx < entity_tree.size(); ++idx) {
        auto& e = entity_tree[idx];
        std::fprintf(stderr, "[Window::updateMeshData] processing idx=%zu shape=%llu visible=%d\n",
                     idx, static_cast<unsigned long long>(e.shape.value), (int)e.visible);
        if (e.visible && kernel().kind(e.shape)!=ShapeKind::Unknown) {
            if (e.ptrDisplayData == nullptr) {
                std::fprintf(stderr, "[Window::updateMeshData] calling CreateMeshFromEntity for idx=%zu\n", idx);
                GmeMesh::DisplayData* dd = new GmeMesh::DisplayData;
                // 生成面片数据
                if (kernel().mesh(e.shape, *dd)) {
                    std::fprintf(stderr, "[Window::updateMeshData] CreateMeshFromEntity SUCCESS for idx=%zu\n", idx);
                    dd->displayType = e.displayType;
                    e.ptrDisplayData = dd;
                    md.push_back(dd);
                } else {
                    std::fprintf(stderr, "[Window::updateMeshData] CreateMeshFromEntity FAILED for idx=%zu\n", idx);
                    ++entitiesWithNoMesh;
                    delete dd;
                }
                isModified = true;
            } else {
                std::fprintf(stderr, "[Window::updateMeshData] using existing ptrDisplayData for idx=%zu\n", idx);
                md.push_back(e.ptrDisplayData);
            }
        }
    }
    if (entitiesWithNoMesh > 0) {
        std::fprintf(stderr, "[Window::updateMeshData] entities without mesh: %zu / total entities=%zu\n",
                     entitiesWithNoMesh, entity_tree.size());
    }
    size_t totalFaces = 0, totalEdges = 0;
    for (auto dd : md) {
        totalFaces += dd->faceMesh.size();
        totalEdges += dd->edgeMesh.size();
    }
    std::fprintf(stderr, "[Window::updateMeshData] display_data=%zu totalFaces=%zu totalEdges=%zu no-mesh entities=%zu\n",
                 md.size(), totalFaces, totalEdges, entitiesWithNoMesh);

    std::fprintf(stderr, "[Window::updateMeshData] calling glWidget->updateMeshData()\n");
    glWidget->updateMeshData();
    std::fprintf(stderr, "[Collab][DEBUG] <<< Window::updateMeshData EXIT\n");
}

void Window::addHandle(ShapeHandle shape, double* p) {
    for (auto& eti : entity_tree) {
        if (eti.shape == shape) {
            auto pointsBefore=input_handles;
            input_handles.push_back(Point3(p));
            auto previous=eti.shape;
            HandleGrid splineHandles{input_handles};
            ShapeHandle curve;
            bool update=input_handles.size()>1;
            try { curve=update?kernel().create(E_BSPLINE,splineHandles):kernel().draft(); }
            catch(const std::exception& e) { input_handles=std::move(pointsBefore); showMessage(QString::fromUtf8(e.what()),5000); return; }
            eti.shape=curve;
            kernel().erase({previous});
            std::vector<std::vector<Point3>> points;
            points.push_back(input_handles);
            eti.handles = points;
            glWidget->setPickedEntity(curve);
            if (eti.ptrDisplayData) delete eti.ptrDisplayData;
            eti.ptrDisplayData = nullptr;
            this->updateMeshData();
            mainWindow->recordEntityModified(eti.index);
            captureHistory();
            mainWindow->notifyModelChangedForCollaboration();
            return;
        }
    }
}

bool Window::getHandles(ShapeHandle shape, std::vector<std::vector<Point3>>& handles) {
    for (auto eti : entity_tree) {
        if (eti.shape == shape) {
            handles = eti.handles;
            return true;
        }
    }
    return false;
}

bool Window::getVisibility(ShapeHandle shape) {
    for (auto eti : entity_tree) {
        if (eti.shape == shape) {
            return eti.visible;
        }
    }
    return false;
}

void Window::changeHandle(ShapeHandle shape, Point3 p, QVector3D v) {
    for (auto& eti : entity_tree) {
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
            kernel().copyColor(eti.shape,shape);
            kernel().erase({eti.shape});
            eti.shape=shape;
            mainWindow->recordEntityModified(eti.index);
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
    try { kernel().translate(shape,t); }
    catch(const std::exception& e) { showMessage(QString::fromUtf8(e.what()),5000); return; }
    for (auto& eti : entity_tree) {
        if (eti.shape == shape) {
            if (eti.operatorType != OPERATOR_CONSTRUCTOR || eti.subOperatorType < E_BSPLINE_CTRLPTS) eti.trans *= t;
            for (auto& h_u : eti.handles)
                for (auto& h : h_u) {
                    h += t.translation();
                }
            // 记录实体被修改（用于增量同步）
            mainWindow->recordEntityModified(eti.index);
        }
    }
    std::vector<int> downstream;
    for (auto& eti : entity_tree) if(eti.shape==shape) {
        delete eti.ptrDisplayData; eti.ptrDisplayData=nullptr;
        downstream=eti.index_support; break;
    }
    updateEntites(downstream);
    this->updateMeshData();
    captureHistory();
    mainWindow->notifyModelChangedForCollaboration();
}

void Window::updateEntites(std::vector<int>& indices) {
    std::set<int> affected,visited,active;
    bool cycle=false;
    std::function<void(int)> collect=[&](int index) {
        if(!affected.insert(index).second) return;
        if(auto row=getEntityItemByIndex(index)) for(auto child:row->index_support) collect(child);
    };
    for(auto index:indices) collect(index);
    std::function<void(int)> rebuild=[&](int index) {
        if(visited.contains(index)) return;
        if(!active.insert(index).second) { cycle=true; return; }
        auto item=getEntityItemByIndex(index);
        const auto bases=item?item->index_base:std::vector<int>{};
        for(auto base:bases) if(affected.contains(base)) rebuild(base);
        active.erase(index);
        if(cycle) return;
        if(!visited.insert(index).second) return;
        item=getEntityItemByIndex(index);
        if(!item || !item->shape || !item->isAutoUpdate) return;
        auto metadata=*item;
        std::vector<int> siblings{index};
        if(metadata.rebuildGroup>=0) {
            siblings.clear();
            for(const auto& row:entity_tree) if(row.rebuildGroup==metadata.rebuildGroup) {
                siblings.push_back(row.index); visited.insert(row.index);
            }
        }
        ShapeList results;
        if(metadata.operatorType==OPERATOR_CONSTRUCTOR) {
            ShapeHandle shape; createEntity(metadata.subOperatorType,shape,metadata.handles);
            if(shape) results.push_back(shape);
        } else {
            auto bases=metadata.index_base;
            if(!operation(metadata.operatorType,metadata.subOperatorType,bases,results,false)) return;
        }
        if(results.empty()) return;
        for(size_t i=0;i<results.size();++i) {
            auto row=i<siblings.size()?getEntityItemByIndex(siblings[i]):nullptr;
            kernel().copyColor(metadata.shape,results[i]);
            kernel().translate(results[i],row?row->trans:metadata.trans,true);
        }
        for(size_t i=0;i<results.size();++i) {
            if(i<siblings.size()) {
                auto row=getEntityItemByIndex(siblings[i]);
                auto old=row->shape;
                row->shape=results[i];
                kernel().erase({old});
                delete row->ptrDisplayData; row->ptrDisplayData=nullptr;
                mainWindow->recordEntityModified(row->index);
            } else {
                auto row=addEntity(results[i],metadata.name,metadata.subOperatorType,metadata.index_base,metadata.operatorType);
                row->rebuildGroup=metadata.rebuildGroup;
                row->trans=metadata.trans;
            }
        }
        for(size_t i=results.size();i<siblings.size();++i)
            mainWindow->deleteEntityByIndexForCollaboration(siblings[i]);
    };
    for(auto index:affected) { rebuild(index); if(cycle) break; }
    if(cycle) showMessage(tr("检测到循环依赖，已停止重建"),5000);
}

ENTITY_TREE_ITEM* Window::getEntityItemByIndex(int index) {
    for (auto& e : entity_tree)
        if (e.index == index) {
            return &e;
        }
    return nullptr;
}

void Window::showMessage(QString s, int duration) {
    mainWindow->showMessage(s, duration);
}

void Window::resetDocumentHistory() {
    history.clear(); historyPosition=-1;
    captureHistory();
}
void Window::captureHistory() {
    if(restoringHistory) return;
    Snapshot snapshot;
    try {
        kernel().noteState();
        ShapeList shapes; QHash<int,QString> uuids;
        for(size_t i=0;i<entity_tree.size();++i) {
            auto& item=entity_tree[i];
            if(kernel().kind(item.shape)==ShapeKind::Unknown) { snapshot.drafts.push_back(static_cast<int>(i)); continue; }
            uuids.insert(static_cast<int>(shapes.size()),QString::fromStdString(item.uuid));
            shapes.push_back(item.shape);
        }
        if(!shapes.empty()) {
            snapshot.geometry=kernel().encodeGraph(uuids,shapes);
            auto nodes=snapshot.geometry.value("nodes").toArray();
            for(int i=0;i<nodes.size();++i) {
                auto node=nodes[i].toObject(); auto props=node.value("props").toObject();
                if(props.value("top_level").toBool()) { props.insert("fallback","sat_fragment"); node.insert("props",props); nodes[i]=node; }
            }
            snapshot.geometry.insert("nodes",nodes);
        }
    } catch(const std::exception& e) { showMessage(QString::fromUtf8(e.what()),5000); return; }
    snapshot.items=entity_tree; snapshot.nextIndex=latest_index;
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
    if(!snapshot.geometry.isEmpty() && !kernel().decodeGraph(snapshot.geometry,decoded,error)) {
        showMessage(error,5000); return;
    }
    for(size_t i=0;i<snapshot.items.size();++i) {
        if(std::find(snapshot.drafts.begin(),snapshot.drafts.end(),static_cast<int>(i))!=snapshot.drafts.end()) restored.push_back(kernel().draft());
        else restored.push_back(decoded.shapes.value(QString::fromStdString(snapshot.items[i].uuid)));
    }
    if(std::find(restored.begin(),restored.end(),ShapeHandle{})!=restored.end()) {
        ShapeList cleanup; for(auto h:restored) if(h) cleanup.push_back(h);
        kernel().erase(cleanup); showMessage(tr("历史几何与文档标识不匹配"),5000); return;
    }
    kernel().erase(getEntityList());
    for(auto& item:entity_tree) { mainWindow->recordEntityRemoved(item.index); delete item.ptrDisplayData; }
    entity_tree=snapshot.items;
    latest_index=std::max(latest_index,snapshot.nextIndex);
    for(size_t i=0;i<entity_tree.size();++i) {
        auto& item=entity_tree[i]; item.shape=restored[i];
        mainWindow->recordEntityAdded(QString::fromStdString(item.uuid),QString::fromStdString(item.name),"body",item.index);
    }
    currentEntityIndex=-1; selected_entities.clear(); input_handles.clear(); imode=SELECTION;
    setCurrentEntity({});
    historyPosition=target;
    updateTreeWidget(); updateMeshData(); isModified=true;
    mainWindow->notifyModelChangedForCollaboration();
}
