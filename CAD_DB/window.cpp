#include "window.h"

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <set>
#include <unordered_map>

#include "gme_dump_object.hxx"
#include "acis/include/alltop.hxx"
#include "acis/include/boolapi.hxx"
#include "acis/include/cstrapi.hxx"
#include "acis/include/insanity_list.hxx"
#include "acis/include/intrapi.hxx"
#include "acis/include/kernapi.hxx"
#include "acis/include/rnd_api.hxx"
#include "acis/include/splgrid.hxx"
#include "acis/include/transfrm.hxx"
#include "gme_mesh.hxx"

using namespace std;
#include "acis/include/point.hxx"

std::string process(outcome& result);
bool CreateMeshFromEntity(ENTITY* e, GmeMesh::DisplayData& dd);

Window::Window(MainWindow* mw) : mainWindow(mw) {
    glWidget = new GLWidget(this);
    ptrCurrentTreeItem = nullptr;
    selected_entities.clear();

    treeWidget = new QTreeWidget(this);
    const QStringList headers({ tr("实体") });
    treeWidget->setHeaderLabels(headers);
    connect(treeWidget, &QTreeWidget::currentItemChanged, this, &Window::currentEntityChanged);
    connect(treeWidget, &QTreeWidget::itemChanged, this, &Window::entityChanged);
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
}

void Window::currentEntityChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous) {
    if (current == nullptr) return;
    int index = current->text(1).toInt();
    for (auto& eti : entity_tree) {
        if (eti.index == index) {
            ptrCurrentTreeItem = &eti;
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

            std::string s = item->text(0).toStdString();
            if (s.length()) eti.name = s;
        }
    }
}

ENTITY_LIST Window::getEntityList() {
    ENTITY_LIST el;
    for (auto eti : entity_tree) {
        el.add(eti.ptrEntity);
    }
    return el;
}

ENTITY_LIST Window::getSelectedEntityList() {
    ENTITY_LIST el;
    for (auto selected_id : selected_entities) {
        auto entity = getEntityItemByIndex(selected_id);
        el.add(entity->ptrEntity);
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
    if (ptrCurrentTreeItem == nullptr) {
        QMessageBox::information(this, tr("未选择实体"), tr("请先在实体树窗口选择一个实体。"));
        return;
    }
    ptrCurrentTreeItem->visible = displayCheckBox->isChecked();
    this->updateMeshData();
}

void Window::updateTabWidget() {
    // 显示
    if (nullptr == ptrCurrentTreeItem) return;

    displayCheckBox->setChecked(ptrCurrentTreeItem->visible);
    displayComboBox->setCurrentIndex(ptrCurrentTreeItem->displayType);
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
    for (auto index : index_base) {
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
    ENTITY_LIST el;
    for (auto& eti : entity_tree) {
        el.add(eti.ptrEntity);
    }
    if (el.count()) {
        outcome result = api_del_entity_list(el);
        QString r = QString::fromStdString(process(result));
        if (r.length()) QMessageBox::warning(nullptr, tr("错误"), r);
    }
}

DELTA_STATE* lastsave_ds = nullptr;
std::unordered_map<void*, int64_t> ptr2nodeid;

void Window::clear() {
    // 删除所有实体
    for (auto &e : entity_tree) {
        if (e.ptrEntity) api_del_entity(e.ptrEntity);
    }
    // 清空历史流（保留默认流）
    HISTORY_STREAM *hs = get_default_stream(false);
    if (hs) {
        delete_all_delta_states(hs, true);   // 删除所有 delta states
        hs->clear();                         // 清空流内容
        DELTA_STATE *root;
        api_ensure_empty_root_state(hs, root);
    }
    // 清空应用数据
    latest_index = 0;
    entity_tree.clear();
    input_handles.clear();
    selected_entities.clear();
    ptrCurrentTreeItem = nullptr;
    lastsave_ds = nullptr;
    ptr2nodeid.clear();
    updateTreeWidget();
    glWidget->clear();
}

void Window::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}

void Window::visiablility() {
    for (auto& e : entity_tree) e.visible = !e.visible;
    this->updateTreeWidget();
    this->updateMeshData();
}

void Window::displayInfo() {
    if (ptrCurrentTreeItem) {
        print_statistic(ptrCurrentTreeItem->ptrEntity);
    }
}

void Window::displayTypeChanged() {
    if (ptrCurrentTreeItem) {
        ptrCurrentTreeItem->displayType = (DISPLAY_TYPES)displayComboBox->currentIndex();
        if (ptrCurrentTreeItem->ptrDisplayData) ptrCurrentTreeItem->ptrDisplayData->displayType = ptrCurrentTreeItem->displayType;
    } else {
        QMessageBox::information(this, tr("未选择实体"), tr("请先在实体树窗口选择一个实体。"));
        return;
    }
    glWidget->update();
}

void Window::createEntity(const int id, ENTITY*& ptrEntity, std::vector<std::vector<SPAposition>>& handles) {
    BODY* ptrBody = nullptr;
    FACE* ptrFace = nullptr;
    EDGE* ptrEdge = nullptr;
    switch (id) {
        case BASIC_ENTITIES::E_CUBE: {
            api_solid_block(SPAposition(0.0, 0.0, 0.0), SPAposition(1.0, 1.0, 1.0), ptrBody);
            break;
        }
        case BASIC_ENTITIES::E_SPHERE: {
            api_make_sphere(1.0, ptrBody);
            break;
        }
        case BASIC_ENTITIES::E_CONE: {
            api_solid_cylinder_cone(SPAposition(0.0, 0.0, 0.0), SPAposition(0.0, 1.0, 0.0), 0.5, 0.5, 0.0, nullptr, ptrBody);
            break;
        }
        case BASIC_ENTITIES::E_CYLINDER: {
            api_solid_cylinder_cone(SPAposition(0.0, 0.0, 0.0), SPAposition(0.0, 1.0, 0.0), 0.5, 0.5, 0.5, nullptr, ptrBody);
            break;
        }
        case BASIC_ENTITIES::E_TORUS: {
            api_solid_torus(SPAposition(0.0, 0.0, 0.0), 1.0, 0.5, ptrBody);
            break;
        }
        case BASIC_ENTITIES::E_PRISM: {
            api_make_prism(1.0, 1.0, 1.0, 6, ptrBody);
            break;
        }
        case BASIC_ENTITIES::E_PYRAMID: {
            api_make_pyramid(1.0, 1.0, 1.0, 0.0, 4, ptrBody);
            break;
        }
        case BASIC_ENTITIES::E_WIGGLE: {
            api_wiggle(1.0, 1.0, 1.0, -1, 1, -2, 2, 1, ptrBody);
            break;
        }
        case BASIC_ENTITIES::E_PLANE: {
            SPAvector v = SPAvector(0.0, 0.0, 1.0);
            api_face_plane(SPAposition(0.0, 0.0, 0.0), 1.0, 1.0, &v, ptrFace);
            break;
        }
        case BASIC_ENTITIES::E_PPLANE: {
            api_make_plface(SPAposition(0.0, 0.0, 0.0), SPAposition(0.5, -1.0, 0.0), SPAposition(-1.0, 0.5, 0.0), ptrFace);
            break;
        }
        case BASIC_ENTITIES::E_CPLANE: {
            api_make_planar_disk(SPAposition(0.0, 0.0, 0.0), SPAunit_vector(0.0, 0.0, 1.0), 1.0, ptrFace);
            break;
        }
        case BASIC_ENTITIES::E_SPHERES: {
            SPAvector v = SPAvector(0.0, 1.0, 0.0);
            api_face_sphere(SPAposition(0.0, 0.0, 0.0), 1.0, -90, 90, 0, 360, &v, ptrFace);
            break;
        }
        case BASIC_ENTITIES::E_PSPHERES: {
            SPAposition center{ 0, 0, 0 };
            auto norm = SPAunit_vector(0, 0, 1);
            api_face_sphere(center, 1, -33, 45, 45, 266, &norm, ptrFace);
            break;
        }
        case BASIC_ENTITIES::E_CONIC_SIDE: {
            SPAposition p = SPAposition(1, 0, 0.0);
            api_face_cylinder_cone(SPAposition(0.0, 0.0, 0.0), SPAvector(0.0, 0.0, 1.0), 1.0, 0.5, 0, 360, 1.0, &p, ptrFace);
            break;
        }
        case BASIC_ENTITIES::E_CYLINDER_SIDE: {
            SPAposition p = SPAposition(1, 0, 0.0);
            api_face_cylinder_cone(SPAposition(0.0, 0.0, 0.0), SPAvector(0.0, 0.0, 1.0), 1.0, 1.0, 0, 360, 1.0, &p, ptrFace);
            break;
        }
        case BASIC_ENTITIES::E_PCONIC: {
            SPAposition p = SPAposition(0.0, 1.0, 0.0);
            api_face_cylinder_cone(SPAposition(0.0, 0.0, 0.0), SPAvector(0.0, 0.0, 1.0), 1.0, 0.5, 0, 100, 1.0, &p, ptrFace);
            break;
        }
        case BASIC_ENTITIES::E_TORUSS: {
            api_make_trface(SPAposition(0.0, 0.0, 0.0), SPAunit_vector(0.0, 0.0, 1.0), 2.0, 0.5, SPAposition(1.0, 0.0, 0), 0, 2 * M_PI, 0, 2 * M_PI, ptrFace);
            break;
        }
        case BASIC_ENTITIES::E_PTORUSS: {
            api_make_trface(SPAposition(0.0, 0.0, 0.0), SPAunit_vector(0.0, 0.0, 1.0), 2.0, 0.5, SPAposition(1.0, 0.0, 0), 0, 1, 0, 1, ptrFace);
            break;
        }
        case BASIC_ENTITIES::E_LAWS: {
            const char* law_equation1 = "vec(x,y,sin(x)*cos(y))";
            law* ptrLaw = nullptr;
            api_str_to_law(law_equation1, &ptrLaw);
            api_face_law(ptrLaw, -10.0, 10.0, -10.0, 10.0, ptrFace);
            break;
        }
        case BASIC_ENTITIES::E_BSPLINE_CTRLPTS: {
            int rows_v = 0;
            int rows_u = 0;
            std::vector<SPAposition> pos;
            double* weights = nullptr;
            double* knots_u = nullptr;
            double* knots_v = nullptr;
            if (handles.size()) {
                rows_v = handles.size();
                rows_u = handles[0].size();
                for (auto p : handles) pos.insert(std::end(pos), std::begin(p), std::end(p));
                weights = (double*)malloc(rows_v * rows_u * sizeof(double));
                knots_u = (double*)malloc(2 * rows_v * sizeof(double));
                knots_v = (double*)malloc(2 * rows_v * sizeof(double));
                for (int i = 0; i < rows_v; i++) {
                    std::vector<SPAposition> ps;
                    for (int j = 0; j < rows_v; j++) {
                        weights[i * rows_v + j] = 1.0;
                    }
                    knots_u[i] = 0.0;
                    knots_u[i + rows_v] = 1.0;
                    knots_v[i] = 0.0;
                    knots_v[i + rows_v] = 1.0;
                }
            } else {
                rows_u = 4;
                rows_v = 4;
                weights = (double*)malloc(rows_u * rows_v * sizeof(double));
                knots_u = (double*)malloc(2 * rows_u * sizeof(double));
                knots_v = (double*)malloc(2 * rows_v * sizeof(double));
                for (int i = 0; i < rows_u; i++) {
                    std::vector<SPAposition> ps;
                    for (int j = 0; j < rows_u; j++) {
                        pos.push_back(SPAposition(i, j, (i + j) % 2));
                        weights[i * rows_v + j] = 1.0;
                        ps.push_back(pos[i * rows_v + j]);
                    }
                    handles.push_back(ps);
                    knots_u[i] = 0.0;
                    knots_u[i + rows_u] = 1.0;
                    knots_v[i] = 0.0;
                    knots_v[i + rows_u] = 1.0;
                }
            }
            api_mk_fa_spl_ctrlpts(3, false, 0, 0, rows_u, 3, false, 0, 0, rows_v, &pos[0], nullptr, 0.001, 2 * rows_u, knots_u, 2 * rows_u, knots_v, 0.0, ptrFace);
            break;
        }
        case BASIC_ENTITIES::E_BSPLINE_FIT: {
            splgrid* ptrSplgrid = ACIS_NEW splgrid();
            int rows_v = 0;
            int rows_u = 0;
            std::vector<SPAposition> pos;
            if (handles.size()) {
                rows_v = handles.size();
                rows_u = handles[0].size();
                for (auto p : handles) pos.insert(std::end(pos), std::begin(p), std::end(p));
            } else {
                rows_u = 5;
                rows_v = 5;
                for (int i = 0; i < rows_u; i++) {
                    std::vector<SPAposition> ps;
                    for (int j = 0; j < rows_v; j++) {
                        pos.push_back(SPAposition(-i, j, (i + j) % 2));
                        ps.push_back(pos[i * rows_v + j]);
                    }
                    handles.push_back(ps);
                }
            }
            ptrSplgrid->set_gridpts_array(&pos[0], rows_v, rows_u);
            ptrSplgrid->set_u_start_array(nullptr);
            ptrSplgrid->set_u_end_array(nullptr);
            ptrSplgrid->set_v_start_array(nullptr);
            ptrSplgrid->set_v_end_array(nullptr);
            api_face_spl_apprx(ptrSplgrid, ptrFace);
            break;
        }
        case BASIC_ENTITIES::E_LINE: {
            SPAposition s = SPAposition(0.0, 0.0, 0.0);
            SPAposition e = SPAposition(1.0, 0.0, 0.0);
            api_curve_line(s, e, ptrEdge);
            break;
        }
        case BASIC_ENTITIES::E_ARC: {
            SPAposition p = SPAposition(0.0, 0.0, 0.0);
            api_curve_arc(p, 1.0, 0.0, 2.0, ptrEdge);
            break;
        }
        case BASIC_ENTITIES::E_ELLIPSE: {
            SPAposition c = SPAposition(0.0, 0.0, 0.0);
            SPAposition m = SPAposition(1.0, 0.0, 0.0);
            api_curve_ellipse(c, m, 1.0, 0.0, 3.14, ptrEdge);
            break;
        }
        case BASIC_ENTITIES::E_CONIC: {
            SPAposition s = SPAposition(0.0, 0.0, 0.0);
            SPAunit_vector sv = SPAunit_vector(1.0, 1.0, 0.0);
            SPAposition e = SPAposition(1.0, 0.0, 0.0);
            SPAunit_vector ev = SPAunit_vector(1.0, -1.0, 0.0);
            api_mk_ed_conic(s, sv, e, ev, 0.5, ptrEdge);
            break;
        }
        case BASIC_ENTITIES::E_HELIX: {
            SPAposition s = SPAposition(0.0, 0.0, 0.0);
            SPAvector sv = SPAvector(1.0, 1.0, 0.0);
            SPAposition e = SPAposition(1.0, 0.0, 0.0);
            api_edge_helix(s, e, sv, 1.0, 0.5, 1, ptrEdge);
            break;
        }
        case BASIC_ENTITIES::E_SPIRAL: {
            SPAposition c = SPAposition(0.0, 0.0, 0.0);
            SPAvector cv = SPAvector(0.0, 0.0, 10.0);
            SPAposition s = SPAposition(5.0, 0.0, 5.0);
            api_edge_spiral(c, cv, s, 2.0, 50.0, ptrEdge);
            break;
        }
        case BASIC_ENTITIES::E_SPRING: {
            SPAposition d = SPAposition(0.0, 10.0, 0.0);
            SPAvector dv = SPAvector(0.0, 0.0, 10.0);
            SPAposition s = SPAposition(0.0, 0.0, 0.0);
            const char* law_equation1 = "5+(4/(1+((x/4)-5)^2))";
            law* ptrLaw = nullptr;
            api_str_to_law(law_equation1, &ptrLaw);
            double thread_distance_array[10] = { 1.0, 2.0, 3.0, 1.0, 2.0, 3.0, 1.0, 2.0, 3.0, 1.0 };
            double rotation_angle_array[10] = { 1.0, 2.0, 3.0, 1.0, 2.0, 3.0, 1.0, 2.0, 3.0, 1.0 };
            double transition_height_array[10] = { 1.0, 2.0, 3.0, 1.0, 2.0, 3.0, 1.0, 2.0, 3.0, 1.0 };
            double transition_angle_array[10] = { 1.0, 2.0, 3.0, 1.0, 2.0, 3.0, 1.0, 2.0, 3.0, 1.0 };
            api_edge_spring_law(d, dv, s, ptrLaw, 1, 4, thread_distance_array, rotation_angle_array, transition_height_array, transition_angle_array, ptrEdge);
            break;
        }
        case BASIC_ENTITIES::E_LAWC: {
            const char* law_equation1 = "vec(cos(x),sin(x),x/5)";
            law* ptrLaw = nullptr;
            api_str_to_law(law_equation1, &ptrLaw);
            api_edge_law(ptrLaw, 0.0, 50.0, ptrEdge);
            break;
        }
        case BASIC_ENTITIES::E_BSPLINE_INTRP: {
            splgrid* ptrSplgrid = ACIS_NEW splgrid();
            int rows_v = 0;
            int rows_u = 0;
            std::vector<SPAposition> pos;
            if (handles.size()) {
                rows_v = handles.size();
                rows_u = handles[0].size();
                for (auto p : handles) pos.insert(std::end(pos), std::begin(p), std::end(p));
            } else {
                rows_v = 5;
                rows_u = 5;
                for (int i = 0; i < rows_u; i++) {
                    std::vector<SPAposition> ps;
                    for (int j = 0; j < rows_v; j++) {
                        pos.push_back(SPAposition(-i, j, (i + j) % 2));
                        ps.push_back(pos[i * rows_v + j]);
                    }
                    handles.push_back(ps);
                }
            }
            ptrSplgrid->set_gridpts_array(&pos[0], rows_v, rows_u);
            ptrSplgrid->set_u_start_array(nullptr);
            ptrSplgrid->set_u_end_array(nullptr);
            ptrSplgrid->set_v_start_array(nullptr);
            ptrSplgrid->set_v_end_array(nullptr);
            api_face_spl_intrp(ptrSplgrid, ptrFace);
            break;
        }
        case BASIC_ENTITIES::E_BEZIER: {
            if (!handles.size()) {
                std::vector<SPAposition> ps;
                ps.push_back(SPAposition(-5.0, -4.0, 0.0));
                ps.push_back(SPAposition(-1.0, 3.0, 0.0));
                ps.push_back(SPAposition(1.0, 2.5, 0.0));
                ps.push_back(SPAposition(4.0, -6.0, 0.0));
                handles.push_back(ps);
            }
            api_curve_bezier(handles[0][0], handles[0][1], handles[0][2], handles[0][3], ptrEdge);
            break;
        }
        case BASIC_ENTITIES::E_BSPLINE: {
            if (handles.size()) {
                api_curve_spline(handles[0].size(), &handles[0][0], nullptr, nullptr, ptrEdge);
            } else {
                if (imode == INPUT_MODES::INPUT_SPLINE)
                    imode = INPUT_MODES::SELECTION;
                else {
                    imode = INPUT_MODES::INPUT_SPLINE;
                    API_BEGIN;
                    ptrEntity = ACIS_NEW ENTITY();
                    API_END;
                    glWidget->setPickedEntity(ptrEntity);
                }
            }
            break;
        }
        default: {
            break;
        }
    }
    if (ptrBody)
        ptrEntity = ptrBody;
    else if (ptrFace) {
        ptrFace->set_cont(BOTH_OUTSIDE);
        ptrFace->set_sides(DOUBLE_SIDED);
        ptrEntity = ptrFace;
    } else if (ptrEdge)
        ptrEntity = ptrEdge;
}

bool Window::operation(const OPERATOR_TYPES ot, const int subOT, std::vector<int>& ids, ENTITY_LIST*& el, bool isHide) {
    BODY* ptrBody = nullptr;
    if (ot == OPERATOR_TYPES::OPERATOR_BOOLEAN) {
        if (ids.size() < 2) {
            QMessageBox::information(this, tr("未选择足够实体"), tr("当前选择%1个实体，请选择2个实体。").arg(ids.size()));
            return false;
        }
        ENTITY_TREE_ITEM* eti0 = getEntityItemByIndex(ids[0]);
        ENTITY_TREE_ITEM* eti1 = getEntityItemByIndex(ids[1]);
        BODY* ptrBody0 = nullptr;
        BODY* ptrBody1 = nullptr;
        if (is_BODY(eti0->ptrEntity))
            ptrBody0 = (BODY*)eti0->ptrEntity;
        else if (is_FACE(eti0->ptrEntity)) {
            FACE* ptrFaces[1];
            ptrFaces[0] = (FACE*)eti0->ptrEntity;
            api_sheet_from_ff(1, ptrFaces, ptrBody0);
        } else if (is_EDGE(eti0->ptrEntity)) {
            EDGE* ptrEdges[1];
            ptrEdges[0] = (EDGE*)eti0->ptrEntity;
            api_make_ewire(1, ptrEdges, ptrBody0);
        }
        if (is_BODY(eti1->ptrEntity))
            ptrBody1 = (BODY*)eti1->ptrEntity;
        else if (is_FACE(eti1->ptrEntity)) {
            FACE* ptrFaces[1];
            ptrFaces[0] = (FACE*)eti1->ptrEntity;
            api_sheet_from_ff(1, ptrFaces, ptrBody1);
        } else if (is_EDGE(eti1->ptrEntity)) {
            EDGE* ptrEdges[1];
            ptrEdges[0] = (EDGE*)eti1->ptrEntity;
            api_make_ewire(1, ptrEdges, ptrBody1);
        }
        api_boolean(ptrBody1, ptrBody0, (BOOL_TYPE)subOT, NDBOOL_KEEP_NEITHER, ptrBody);
        if (ptrBody) {
            el->add(ptrBody);
            if (isHide) {
                int eti0_index = eti0->index;
                int eti1_index = eti1->index;
                for (auto it = entity_tree.begin(); it != entity_tree.end();) {
                    if (it->index == eti0_index || it->index == eti1_index) {
                        it = entity_tree.erase(it);
                    } else {
                        it++;
                    }
                }
                clearSelectedEntities();
                ids.resize(2);
            }
        }
    } else if (ot == OPERATOR_INTERSECTOR) {
        if (ids.size() < 2) {
            QMessageBox::information(this, tr("未选择足够实体"), tr("当前选择%1个实体，请选择2个实体。").arg(ids.size()));
            return false;
        }
        ENTITY_TREE_ITEM* eti0 = getEntityItemByIndex(ids[0]);
        ENTITY_TREE_ITEM* eti1 = getEntityItemByIndex(ids[1]);
        if (is_FACE(eti0->ptrEntity) && is_FACE(eti1->ptrEntity)) {
            api_fafa_int((FACE*)eti0->ptrEntity, (FACE*)eti1->ptrEntity, ptrBody);
        } else if (is_EDGE(eti0->ptrEntity) && is_FACE(eti1->ptrEntity)) {
            api_edfa_int((EDGE*)eti0->ptrEntity, (FACE*)eti1->ptrEntity, el);
        } else if (is_FACE(eti0->ptrEntity) && is_EDGE(eti1->ptrEntity)) {
            api_edfa_int((EDGE*)eti1->ptrEntity, (FACE*)eti0->ptrEntity, el);
        } else if (is_EDGE(eti0->ptrEntity) && is_EDGE(eti1->ptrEntity)) {
            curve_curve_int* inters = nullptr;
            api_inter_ed_ed((EDGE*)eti0->ptrEntity, (EDGE*)eti1->ptrEntity, inters);
            if (inters) api_ed_inters_to_ents((EDGE*)eti0->ptrEntity, inters, *el);
        }
        if (ptrBody) el->add(ptrBody);
        if (el->iteration_count() && isHide) {
            eti0->visible = false;
            eti1->visible = false;
            clearSelectedEntities();
            ids.resize(2);
        }
    } else if (ot == OPERATOR_TYPES::OPERATOR_DEFEATURE) {
        if (ids.size() < 1) {
            QMessageBox::information(this, tr("未选择足够实体"), tr("请至少选择一个实体"));
            return false;
        }
        if (subOT == DEFEATURE_BREP2CSG) {
            //std::vector<BODY*> ptrBodys;
            //ENTITY_TREE_ITEM* eti0 = getEntityItemByIndex(ids[0]);
            //BODY* ptrBody0 = nullptr;
            //if (is_BODY(eti0->ptrEntity))
            //    ptrBody0 = (BODY*)eti0->ptrEntity;
            //else if (is_FACE(eti0->ptrEntity)) {
            //    FACE* ptrFaces[1];
            //    ptrFaces[0] = (FACE*)eti0->ptrEntity;
            //    api_sheet_from_ff(1, ptrFaces, ptrBody0);
            //}
            //else if (is_EDGE(eti0->ptrEntity)) {
            //    EDGE* ptrEdges[1];
            //    ptrEdges[0] = (EDGE*)eti0->ptrEntity;
            //    api_make_ewire(1, ptrEdges, ptrBody0);
            //}
            //SpdSharedPtr<SpdShape> shape;
            //SpdSharedPtr<SpdShape> shape0;
            //gme2spd(ptrBody0, shape0, 0.1);
            //    // 获取形状数据到shapes
            //    std::vector<SpdSharedPtr<SpdShape>> shapes;
            //    shapes.push_back(shape0);
            //    // 设置参数
            //    Brep2CsgOptions options;
            //    options.SetTolerance(0.01);
            //    options.SetPrintLog(true);
            //    options.SetSkipEvaluation(true);
            //    options.SetMultiThread(false);
            //    options.SetPointCloudFit(false);
            //    // 保存转换结果
            //    std::vector<HD_PEL_ENT*> entities;
            //    std::vector<std::vector<int>> unused_face_groups;
            //    // 调用转换接口
            //    ConvertBrepToCsg(shapes, options, entities, unused_face_groups);
            //    if (entities.size() == 0) {
            //        QMessageBox::information(this, tr("转换失败"), tr("生成了0个实体"));
            //        return false;
            //    }
            //    // 转回Brep
            //    for (auto entity : entities) {
            //        spd2gme(*entity, ptrBody);
            //        ptrBodys.push_back(ptrBody);
            //    }
            //if (ptrBody) {
            //    for (auto ptr : ptrBodys) {
            //        el->add(ptr);
            //    }
            //    if (isHide) {
            //        eti0->visible = false;
            //        clearSelectedEntities();
            //    }
            //}
        } else if (subOT == DEFEATURE_SHELL) {
            //std::vector<BODY*> body;
            //for (size_t i = 0; i < ids.size(); i++) {
            //    ENTITY_TREE_ITEM* eti = getEntityItemByIndex(ids[i]);
            //    BODY* ptrBody1 = nullptr;
            //    if (is_BODY(eti->ptrEntity))
            //        ptrBody1 = (BODY*)eti->ptrEntity;
            //    else if (is_FACE(eti->ptrEntity)) {
            //        FACE* ptrFaces[1];
            //        ptrFaces[0] = (FACE*)eti->ptrEntity;
            //        api_sheet_from_ff(1, ptrFaces, ptrBody1);
            //    }
            //    else if (is_EDGE(eti->ptrEntity)) {
            //        EDGE* ptrEdges[1];
            //        ptrEdges[0] = (EDGE*)eti->ptrEntity;
            //        api_make_ewire(1, ptrEdges, ptrBody1);
            //    }
            //    body.push_back(ptrBody1);
            //}

            //BODY* body1 = nullptr;
            //for (size_t i = 0; i < body.size(); i++) {
            //    std::vector<LUMP*> lumpvect;

            //    LUMP* lump = body[i]->lump();
            //    lumpvect.push_back(lump);
            //    while (lump->next()) {
            //        lump = lump->next();
            //        lumpvect.push_back(lump);
            //    }
            //    for (size_t j = 0; j < lumpvect.size(); j++) {
            //        std::vector<SHELL*> shellvect;
            //        std::set<int> remove;
            //        SHELL* shell = lumpvect[j]->shell();
            //        shellvect.push_back(shell);
            //        while (shell->next()) {
            //            shell = shell->next();
            //            shellvect.push_back(shell);
            //        }
            //        for (size_t k = 0; k < shellvect.size(); k++) {
            //            for (size_t ii = k; ii < shellvect.size(); ii++) {
            //                if (ii == k || remove.contains(ii)) continue;
            //                point_containment result;
            //                point_containment result1;
            //                double closest_param = DBL_MAX;
            //                gme_api_point_in_shell(shellvect[k]->face()->loop()->start()->start()->geometry()->coords(), shellvect[ii], result, closest_param);
            //                gme_api_point_in_shell(shellvect[ii]->face()->loop()->start()->start()->geometry()->coords(), shellvect[k], result1, closest_param);
            //                if (result == point_inside) {
            //                    remove.insert(k);
            //                    break;
            //                }
            //                if (result1 == point_inside) {
            //                    remove.insert(ii);
            //                }
            //            }
            //        }

            //        for (size_t k = 0; k < shellvect.size() - 1; k++) {
            //            if (remove.contains(shellvect.size() - 1 - k)) {
            //                shellvect[shellvect.size() - 2 - k]->set_next(shellvect[shellvect.size() - 1 - k]->next());
            //            }
            //        }
            //    }
            //}

            //if (&body[0] != nullptr) {
            //    body1 = new BODY(*body[0]);
            //    for (int i = 1; i < body.size(); i++) {
            //        BODY* gme_body = new BODY(*body[i]);
            //        api_boolean(gme_body, body1, UNION, NDBOOL_KEEP_BOTH, body1);
            //    }
            //}
            //body.clear();
            //ptrBody = new BODY(*body1);
            //if (ptrBody) {
            //    el->add(ptrBody);
            //    if (isHide) {
            //        for (size_t i = 0; i < ids.size(); i++) {
            //            ENTITY_TREE_ITEM* eti = getEntityItemByIndex(ids[i]);
            //            eti->visible = false;
            //        }

            //        clearSelectedEntities();
            //        ids.resize(ids.size());
            //    }
            //}
        } else if (subOT == DEFEATURE_UNREPEAT) {
            std::vector<BODY*> body;
            for (size_t i = 0; i < ids.size(); i++) {
                ENTITY_TREE_ITEM* eti = getEntityItemByIndex(ids[i]);
                BODY* ptrBody1 = nullptr;
                if (is_BODY(eti->ptrEntity))
                    ptrBody1 = (BODY*)eti->ptrEntity;
                else if (is_FACE(eti->ptrEntity)) {
                    FACE* ptrFaces[1];
                    ptrFaces[0] = (FACE*)eti->ptrEntity;
                    api_sheet_from_ff(1, ptrFaces, ptrBody1);
                } else if (is_EDGE(eti->ptrEntity)) {
                    EDGE* ptrEdges[1];
                    ptrEdges[0] = (EDGE*)eti->ptrEntity;
                    api_make_ewire(1, ptrEdges, ptrBody1);
                }
                body.push_back(ptrBody1);
            }
            std::set<int> remove;
            for (int i = 0; i < body.size(); i++) {
                for (size_t j = 0; j < body.size(); j++) {
                    if (i == j || remove.contains(j)) continue;
                    BODY* result = nullptr;
                    BODY* result1 = nullptr;
                    api_boolean(body[j], body[i], SUBTRACTION, NDBOOL_KEEP_BOTH, result);
                    api_boolean(body[i], body[j], SUBTRACTION, NDBOOL_KEEP_BOTH, result1);
                    if (!result->lump()) {
                        remove.insert(i);
                        break;
                    }
                    if (!result1->lump()) {
                        remove.insert(j);
                    }
                }
            }
            for (size_t i = 0; i < body.size(); i++) {
                if (remove.contains(i)) {
                    continue;
                }
                el->add(body[i]);
            }

            if (isHide) {
                for (size_t i = 0; i < ids.size(); i++) {
                    ENTITY_TREE_ITEM* eti = getEntityItemByIndex(ids[i]);
                    eti->visible = false;
                }
                clearSelectedEntities();
                ids.resize(ids.size());
            }
        }
    }
    return true;
}

ENTITY_TREE_ITEM* Window::addEntity(ENTITY* ptrEntity, const std::string name, int subOperatorType) {
    std::vector<std::vector<SPAposition>> handles;
    std::vector<int> index;
    return addEntity(ptrEntity, name, subOperatorType, handles, index);
}

ENTITY_TREE_ITEM* Window::addEntity(ENTITY* ptrEntity, const std::string name, int subOperatorType, std::vector<int> index_base, OPERATOR_TYPES ot) {
    std::vector<std::vector<SPAposition>> handles;
    return addEntity(ptrEntity, name, subOperatorType, handles, index_base, ot);
}

ENTITY_TREE_ITEM* Window::addEntity(ENTITY* ptrEntity, const std::string name, int subOperatorType, std::vector<std::vector<SPAposition>>& handles) {
    std::vector<int> index_base;
    return addEntity(ptrEntity, name, subOperatorType, handles, index_base);
}

ENTITY_TREE_ITEM* Window::addEntity(ENTITY* ptrEntity, const std::string name, int sot, std::vector<std::vector<SPAposition>>& handles, std::vector<int> index_base, OPERATOR_TYPES ot) {
    if (nullptr == ptrEntity) return new ENTITY_TREE_ITEM();
    ENTITY_TREE_ITEM eti;
    eti.name = name;
    eti.index = entity_tree.size();
    eti.ptrEntity = ptrEntity;
    eti.handles = handles;
    eti.visible = true;
    eti.operatorType = ot;
    eti.subOperatorType = sot;
    eti.index_base = index_base;
    entity_tree.push_back(eti);
    for (auto index : index_base) {
        for (auto& e : entity_tree) {
            if (e.index == index) e.index_support.push_back(eti.index);
        }
    }
    this->updateTreeWidget();
    this->updateMeshData();
    mainWindow->notifyModelChangedForCollaboration();
    return &entity_tree[eti.index];
}

void Window::updateMeshData() {
    api_logging(FALSE);
    std::vector<GmeMesh::DisplayData*>& md = glWidget->getMeshData();
    md.clear();
    for (auto& e : entity_tree) {
        if (e.visible) {
            if (e.ptrDisplayData == nullptr) {
                GmeMesh::DisplayData* dd = new GmeMesh::DisplayData;
                // 生成面片数据
                if (CreateMeshFromEntity(e.ptrEntity, *dd)) {
                    dd->displayType = e.displayType;
                    e.ptrDisplayData = dd;
                    md.push_back(dd);
                } else {
                    delete dd;
                }
                isModified = true;
            } else
                md.push_back(e.ptrDisplayData);
        }
    }
    api_logging(TRUE);

    glWidget->updateMeshData();
}

void Window::addHandle(ENTITY* ptrEntity, double* p) {
    for (auto& eti : entity_tree) {
        if (eti.ptrEntity == ptrEntity) {
            input_handles.push_back(SPAposition(p));
            int size = input_handles.size();
            EDGE* ptrEdge = nullptr;
            bool update = true;
            api_curve_spline(size, &input_handles[0], nullptr, nullptr, ptrEdge);
            if (ptrEdge == nullptr) {
                ptrEdge = ACIS_NEW EDGE();
                update = false;
            }
            eti.ptrEntity = ptrEdge;
            std::vector<std::vector<SPAposition>> points;
            points.push_back(input_handles);
            eti.handles = points;
            glWidget->setPickedEntity(ptrEdge);
            if (eti.ptrDisplayData) delete eti.ptrDisplayData;
            eti.ptrDisplayData = nullptr;
            if (update) this->updateMeshData();
            mainWindow->notifyModelChangedForCollaboration();
            return;
        }
    }
}

bool Window::getHandles(ENTITY* ptrEntity, std::vector<std::vector<SPAposition>>& handles) {
    for (auto eti : entity_tree) {
        if (eti.ptrEntity == ptrEntity) {
            handles = eti.handles;
            return true;
        }
    }
    return false;
}

bool Window::getVisibility(ENTITY* ptrEntity) {
    for (auto eti : entity_tree) {
        if (eti.ptrEntity == ptrEntity) {
            return eti.visible;
        }
    }
    return false;
}

void Window::changeHandle(ENTITY* ptrEntity, SPAposition p, QVector3D v) {
    for (auto& eti : entity_tree) {
        if (eti.ptrEntity == ptrEntity) {
            for (auto& hps : eti.handles) {
                for (auto& hp : hps) {
                    if (hp == p) {
                        hp += SPAvector(v.x(), v.y(), v.z());
                        glWidget->setPickedHandle(hp);
                    }
                }
            }
            ENTITY* ptrEntity = nullptr;
            this->createEntity(eti.subOperatorType, ptrEntity, eti.handles);
            if (ptrEntity) {
                rgb_color ce;
                logical found = FALSE;
                outcome out = api_rh_get_entity_rgb(eti.ptrEntity, ce, TRUE, found);
                if (found) api_rh_set_entity_rgb(ptrEntity, ce);
            }
            eti.ptrEntity = ptrEntity;
            glWidget->setPickedEntity(eti.ptrEntity);

            if (eti.ptrDisplayData) delete eti.ptrDisplayData;
            eti.ptrDisplayData = nullptr;
            updateEntites(eti.index_support);
            this->updateMeshData();
            mainWindow->notifyModelChangedForCollaboration();
            return;
        }
    }
}

void Window::changeEntity(ENTITY_LIST e_list, QVector3D v) {
    for (auto e : e_list) {
        this->changeEntity(e, v);
    }
}

void Window::changeEntity(ENTITY* ptrEntity, QVector3D v) {
    SPAtransf t = translate_transf(SPAvector(v.x(), v.y(), v.z()));
    changeEntity(ptrEntity, t);
}

void Window::changeEntity(ENTITY* ptrEntity, SPAtransf t) {
    if (ptrEntity == nullptr) return;
    api_transform_entity(ptrEntity, t);
    for (auto& eti : entity_tree) {
        if (eti.ptrEntity == ptrEntity) {
            if (eti.operatorType == OPERATOR_TYPES::OPERATOR_CONSTRUCTOR && eti.subOperatorType < BASIC_ENTITIES::E_BSPLINE_CTRLPTS) eti.trans *= t;
            for (auto& h_u : eti.handles)
                for (auto& h : h_u) {
                    h += t.translation();
                }
        }
    }
    //if (is_BODY(ptrEntity)) api_change_body_trans((BODY*)ptrEntity, nullptr);
    //In general, when transformations are applied to a body, the underlying geometries of all the subordinate entities are not changed. ACIS simply attaches a transformation object to the body entity, and calculations involving the body's geometry are automatically piped through the transform. This reduces the risk of introducing round-off errors when the body is subjected to repeated transformations.  If you want to apply a transform to the underlying geometry, you must explicitly do so. For example, you can apply the transformation to the body first, using api_transform_entity, then propagate it to the geometry by calling api_change_body_trans with a NULL transform argument.
    for (auto& eti : entity_tree)
        if (eti.ptrEntity == ptrEntity) {
            if (eti.ptrDisplayData) delete eti.ptrDisplayData;
            eti.ptrDisplayData = nullptr;
            this->updateEntites(eti.index_support);
        }
    this->updateMeshData();
    mainWindow->notifyModelChangedForCollaboration();
}

void Window::updateEntites(std::vector<int>& index) {
    for (auto id : index) {
        ENTITY_TREE_ITEM& eti = *getEntityItemByIndex(id);
        if (eti.ptrEntity && eti.isAutoUpdate) {
            std::vector<ENTITY*> e_base;
            for (auto i_b : eti.index_base) {
                ENTITY_TREE_ITEM& eti_b = *getEntityItemByIndex(i_b);
                if (eti_b.ptrEntity) {
                    e_base.push_back(eti_b.ptrEntity);
                }
            }
            ENTITY_LIST* el = ACIS_NEW ENTITY_LIST();
            if (eti.operatorType == OPERATOR_TYPES::OPERATOR_CONSTRUCTOR) {
                ENTITY* ptrEntity = nullptr;
                this->createEntity(eti.subOperatorType, ptrEntity, eti.handles);
                if (ptrEntity) el->add(ptrEntity);
            }
            else
                this->operation(eti.operatorType, eti.subOperatorType, eti.index_base, el, false);
            if (eti.operatorType == OPERATOR_TYPES::OPERATOR_INTERSECTOR && (is_EDGE(e_base[0]) || is_EDGE(e_base[1]))) {
                // 边面相交结果为实体列表，新的结果列表与原结果列表如何对应
                // 此处删除所有原求交结果，将新的结果重新加入
                // 缺陷：此时无法支持多层运算的更新，例如A与B求交结果为C和D，E和C进行运算得到F，则F无法自动更新
                // @todo：将列表合并为一个body
                std::vector<int> i_base = eti.index_base;
                std::vector<int> i_remove;
                for (auto& et : entity_tree) {
                    if (eti.index_base == et.index_base && et.operatorType == OPERATOR_TYPES::OPERATOR_INTERSECTOR) {
                        for (auto& e : entity_tree) {
                            e.index_base.erase(std::remove(e.index_base.begin(), e.index_base.end(), et.index), e.index_base.end());
                        }
                        i_remove.push_back(et.index);
                    }
                }
                for (int i = 0; i < entity_tree.size(); i++) {
                    if (std::find(i_remove.begin(), i_remove.end(), entity_tree[i].index) != i_remove.end()) {
                        entity_tree.erase(entity_tree.begin() + i);
                        i = 0;
                    }
                }
                if (el->count())
                    for (ENTITY* ent = el->first(); ent; ent = el->next()) {
                        addEntity(ent, "求交结果", eti.subOperatorType, i_base, OPERATOR_INTERSECTOR);
                    }
                else {
                    ENTITY* e = ACIS_NEW ENTITY();
                    addEntity(e, "求交结果", eti.subOperatorType, i_base, OPERATOR_INTERSECTOR);
                }
                updateTreeWidget();
            }
            else {
                if (el->count()) {
                    ENTITY* ptrEntity = el->first();
                    rgb_color ce;
                    logical found = FALSE;
                    outcome out = api_rh_get_entity_rgb(eti.ptrEntity, ce, TRUE, found);
                    if (found) api_rh_set_entity_rgb(ptrEntity, ce);
                    api_transform_entity(ptrEntity, eti.trans);
                    if (is_BODY(ptrEntity)) api_change_body_trans((BODY*)ptrEntity, nullptr);
                    eti.ptrEntity = ptrEntity;
                }
                else
                    eti.ptrEntity = ACIS_NEW ENTITY();
                if (eti.ptrDisplayData) delete eti.ptrDisplayData;
                eti.ptrDisplayData = nullptr;
            }
            updateEntites(eti.index_support);
        }
    }
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
