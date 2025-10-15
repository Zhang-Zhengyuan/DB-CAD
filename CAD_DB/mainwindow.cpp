#include "mainwindow.h"


#include <QActionGroup>
#include <QApplication>
#include <QFileDialog>
#include <QInputDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QSettings>
#include <QStatusBar>
#include <QToolBar>
#include <QFormLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <fstream>
#include <sstream>

#include "gme_dump_object.hxx"
#include "acis/include/bulletin.hxx"
#include "acis/include/attrib.hxx"
#include "acis/include/kernapi.hxx"
#include "acis/include/boolapi.hxx"
#include "acis/include/cstrapi.hxx"
#include "acis/include/insanity_list.hxx"
#include "acis/include/rnd_api.hxx"
#include "gme_user_data.hxx"
#include "window.h"
#include "acis/include/debug.hxx"

#include "acis/include/part_api.hxx"

#include "neo4j.hxx"
#include "access.hxx"
#include "common.hxx"
#include <mgclient-1.4.2/mgclient.h>
#include <acis/include/ckoutcom.hxx>


std::string process(outcome& result);
int initialize_acis();
void terminate_acis(int level);

struct menu_struct {
    std::string name_cn = "菜单";                                        // 菜单项中文名/状态栏提示
    std::string name_en = "menu";                                        // 菜单项英文名/icon文件名.png
    bool checkable = false;                                              // 该菜单是否可以check
    OPERATOR_TYPES operatorType = OPERATOR_TYPES::OPERATOR_CONSTRUCTOR;  // 该项对应的操作类型
    // 说明：-1表示读入实体；OPERATOR_TYPES::OPERATOR_SPERATOR，表示插入分隔符
    int subOperatorType = 0;                                             // 该项对应的子操作类型
};

std::vector<menu_struct> menus_entities = {
  {"体",                      "Body",             false, OPERATOR_TYPES::OPERATOR_SPERATOR,    0                                                },
  {"立方体",                "cube",             false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_CUBE                           },
  {"球体",                   "sphere",           false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_SPHERE                         },
  {"圆柱",                   "cylinder",         false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_CYLINDER                       },
  {"圆锥",                   "cone",             false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_CONE                           },
  {"圆环",                   "torus",            false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_TORUS                          },
  {"棱柱",                   "prism",            false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_PRISM                          },
  {"金字塔",                "pyramid",          false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_PYRAMID                        },
  {"摆动面",                "wiggle",           false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_WIGGLE                         },
  {"面",                      "Face",             false, OPERATOR_TYPES::OPERATOR_SPERATOR,    0                                                },
  {"矩形面",                "plane",            false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_PLANE                          },
  {"平行四边形面",       "pplane",           false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_PPLANE                         },
  {"圆形面",                "cplane",           false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_CPLANE                         },
  {"球面",                   "spheres",          false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_SPHERES                        },
  {"部分球面",             "pspheres",         false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_PSPHERES                       },
  {"圆锥面",                "conic",            false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_CONIC_SIDE                     },
  {"圆柱面",                "cylinders",        false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_CYLINDER_SIDE                  },
  {"部分圆锥面",          "pconic",           false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_PCONIC                         },
  {"圆环面",                "toruss",           false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_TORUSS                         },
  {"部分圆环面",          "ptoruss",          false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_PTORUSS                        },
  {"公式面",                "laws",             false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_LAWS                           },
  {"B样条面-控制点",     "bsplines_ctrlpts", false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_BSPLINE_CTRLPTS                },
  {"B样条面-拟合",        "bsplines_fit",     false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_BSPLINE_FIT                    },
  {"B样条面-插值",        "bsplines_intrp",   false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_BSPLINE_INTRP                  },
  {"线",                      "Edge",             false, OPERATOR_TYPES::OPERATOR_SPERATOR,    0                                                },
  {"换行符",                "line_break",       false, OPERATOR_TYPES::OPERATOR_SPERATOR,    1                                                },
  {"直线",                   "line",             false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_LINE                           },
  {"圆弧",                   "arc",              false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_ARC                            },
  {"椭圆",                   "ellipse",          false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_ELLIPSE                        },
  {"二次线",                "conicc",           false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_CONIC                          },
  {"螺旋线",                "helix",            false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_HELIX                          },
  {"阿基米德螺旋线",    "spiral",           false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_SPIRAL                         },
  {"弹簧线",                "spring",           false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_SPRING                         },
  {"方程线",                "lawc",             false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_LAWC                           },
  {"贝塞尔曲线",          "bezier",           false, OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_BEZIER                         },
  {"B样条线",               "bsplinec",         true,  OPERATOR_TYPES::OPERATOR_CONSTRUCTOR, BASIC_ENTITIES::E_BSPLINE                        },
};

std::vector<menu_struct> menus_operators = {
  {"求交",          "intersector",  false, OPERATOR_TYPES::OPERATOR_INTERSECTOR, 0                     },
  {"分割线",       "separate",     false, OPERATOR_TYPES::OPERATOR_SPERATOR,    0                     },
  {"布尔并",       "union",        false, OPERATOR_TYPES::OPERATOR_BOOLEAN,     UNION                 },
  {"布尔交",       "intersection", false, OPERATOR_TYPES::OPERATOR_BOOLEAN,     INTERSECTION          },
  {"布尔差",       "subtraction",  false, OPERATOR_TYPES::OPERATOR_BOOLEAN,     SUBTRACTION           },
  {"分割线",       "separate",     false, OPERATOR_TYPES::OPERATOR_SPERATOR,    0                     },
  {"去重",          "unrepeat",     false, OPERATOR_TYPES::OPERATOR_DEFEATURE,   DEFEATURE_UNREPEAT    },
  //{"去除小特征", "remove_small", false, OPERATOR_TYPES::OPERATOR_DEFEATURE,   DEFEATURE_REMOVE_SMALL},
  //{"抽壳",          "shell",        false, OPERATOR_TYPES::OPERATOR_DEFEATURE,   DEFEATURE_SHELL       },
  //{"拆分",          "split",        false, OPERATOR_TYPES::OPERATOR_DEFEATURE,   DEFEATURE_SPLIT       },
  //{"包络",          "envelope",     false, OPERATOR_TYPES::OPERATOR_DEFEATURE,   DEFEATURE_ENVELOPE    },
  //{"Brep转CSG",      "brep2csg",     false, OPERATOR_TYPES::OPERATOR_DEFEATURE,   DEFEATURE_BREP2CSG    },
};

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags flags) : QMainWindow(parent, flags) {
    setObjectName("MainWindow");
    setWindowTitle("DBCAD");
    curWindow = nullptr;

    if (0 == initialize_acis()) {
        QMessageBox::warning(this, tr("异常"), tr("ACIS初始化失败"));
    }

    createActions();
    createStatusBar();

#ifndef QT_NO_SESSIONMANAGER
    connect(qApp, &QGuiApplication::commitDataRequest, this, &MainWindow::commitData);
#endif

    addWindow();

    // option_header* delete_forward_states_option = find_option("delete_forward_states");
    // delete_forward_states_option->push(FALSE);

    HISTORY_STREAM* new_hs = new HISTORY_STREAM();
    set_default_stream(new_hs);

    setNEO4JConnectInfo();
    setMEMGRAPHConnectInfo();
    setPOSTGRESQLConnectInfo();
    mg_init();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (maybeSave()) {
        event->accept();
        if (curWindow) {
            curWindow->close();
        }
        mg_finalize();
        terminate_acis(2);
    } else {
        event->ignore();
    }
}

void MainWindow::addWindow() {
    if (!centralWidget()) {
        curWindow = new Window(this);
        setCentralWidget(curWindow);
    } else
        QMessageBox::information(this, tr("无法创建新窗口"), tr("您最多只能创建一个窗口。"));
}

void MainWindow::open() {
    if (maybeSave()) {
        QAction* checkedAct = setModeActGroup->checkedAction();
        if (checkedAct == setACISModeAct) {
            QString fileName = QFileDialog::getOpenFileName(this, QObject::tr("打开文件"), QString::fromStdString(std::string(".")));
            if (!fileName.isEmpty()) loadFile(fileName);
        } else if (checkedAct == setNEO4JModeAct) {
            bool ok;
            QDateTime curDateTime = QDateTime::currentDateTime();
            QString curDateTime_qstr = QString("part") + curDateTime.toString("yyyyMMddhhmmss");  // 保存零件对话框的默认零件名是part+当前年月日时分秒
            QString partnametext = QInputDialog::getText(this, tr("打开零件(neo4j)"), tr("请输入打开的零件名:"), QLineEdit::Normal, curDateTime_qstr, &ok, this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
            if (ok && !partnametext.isEmpty()) {
                loadFile(partnametext);
            }
        } else if (checkedAct == setMEMGRAPHModeAct) {
            bool ok;
            QDateTime curDateTime = QDateTime::currentDateTime();
            QString curDateTime_qstr = QString("part") + curDateTime.toString("yyyyMMddhhmmss");  // 保存零件对话框的默认零件名是part+当前年月日时分秒
            QString partnametext = QInputDialog::getText(this, tr("打开零件(memgraph)"), tr("请输入打开的零件名:"), QLineEdit::Normal, curDateTime_qstr, &ok, this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
            if (ok && !partnametext.isEmpty()) {
                loadFile(partnametext);
            }
        } else {
            assert(checkedAct == setNEO4JIncrementalModeAct);
            QDateTime curDateTime = QDateTime::currentDateTime();
            QString curDateTime_qstr = QString("part") + curDateTime.toString("yyyyMMddhhmmss");  // 保存零件对话框的默认零件名是part+当前年月日时分秒

            QDialog dialog(this);
            dialog.setMinimumSize(200, 200);
            dialog.setWindowTitle(tr("打开零件(neo4j)"));
            QFormLayout form(&dialog);

            QLabel lineEdit_partname_label(tr("请输入打开的零件名:"), &dialog);
            form.addRow(&lineEdit_partname_label);
            QLineEdit lineEdit_partname(&dialog);
            lineEdit_partname.setText(curDateTime_qstr);
            form.addRow(&lineEdit_partname);

            QLabel spinBox_generation_label(tr("请输入打开的零件版次（正整数）:"), &dialog);
            form.addRow(&spinBox_generation_label);
            QSpinBox spinBox_generation(&dialog);
            spinBox_generation.setMinimum(1);
            spinBox_generation.setMaximum(2000000000);
            form.addRow(&spinBox_generation);

            QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
            form.addRow(&buttonBox);
            QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
            QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

            if (dialog.exec() == QDialog::Accepted) {
                loadFile(lineEdit_partname.text(), spinBox_generation.value());
            }

        }
    }
}

void MainWindow::buildTreeFromHistroy(HISTORY_STREAM* hs) {
    if (hs == nullptr) {
        hs = get_default_stream();
    }
    set_logging(false);
    curWindow->clear();
    DELTA_STATE* this_ds = hs->get_root_ds();
    while (this_ds) {
        GME_DELTA_STATE_user_data* delta_state_user_data = (GME_DELTA_STATE_user_data*)(this_ds->get_user_data());
        if (delta_state_user_data) {
            auto tree_items = delta_state_user_data->get_tree_items();
            for (auto item : *tree_items) {
                item.ptrDisplayData = nullptr;
                curWindow->addTreeItem(item);
                curWindow->entity_tree[item.index].index_support.clear();
                for (auto base : item.index_base) {
                    curWindow->entity_tree[base].visible = false;
                    curWindow->entity_tree[base].index_support.push_back(item.index);
                }
            }
        }
        this_ds = this_ds->prev();
    }
    curWindow->updateTreeWidget();
    curWindow->updateMeshData();

    if (hs->current_ds) {
        delete hs->current_ds;
        hs->current_ds = nullptr;
    }
    set_logging(true);
}

void MainWindow::undo() {
    HISTORY_STREAM* hs = get_default_stream();
    int request_n = -1;
    int actual_n = 0;

    if (hs) {
        api_roll_n_states(hs, request_n, actual_n);
    }
    if (actual_n != request_n) {
        return;
    }

    buildTreeFromHistroy(hs);
    return;
}

void MainWindow::redo() {
    HISTORY_STREAM* hs = get_default_stream();
    int request_n = 1;
    int actual_n = 0;

    if (hs) {
        api_roll_n_states(hs, request_n, actual_n);
    }
    if (actual_n != request_n) {
        return;
    }

    buildTreeFromHistroy(hs);
    return;
}

bool MainWindow::save() {
    if (curWindow == nullptr) {
        const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("DBCAD"),
            tr("当前窗口未初始化\n"
                "是否初始化？"),
            QMessageBox::Yes | QMessageBox::No);
        switch (ret) {
            case QMessageBox::Yes:
                addWindow();
        }
        return false;
    }

    ENTITY_LIST el;
    acis_get_noattrib_toplevel_active_entities(el);
    if (el.count() == 0) {
        QMessageBox::warning(this, tr("DBCAD"), tr("当前无顶级活跃实体。"));
        return false;
    }

    QAction* checkedAct = setModeActGroup->checkedAction();

    if (checkedAct == setACISModeAct) {
        if (curWindow->getCurFile().isEmpty()) {
            return saveAs();
        } else {
            return saveFile(curWindow->getCurFile());
        }
    } else {
        assert(checkedAct == setNEO4JModeAct || checkedAct == setNEO4JIncrementalModeAct || checkedAct == setMEMGRAPHModeAct);
        if (curWindow->getCurPartName().isEmpty()) {
            return saveAs();
        } else {
            return saveFile(curWindow->getCurPartName());
        }
    }
}

void MainWindow::saveEntity() {
    if (curWindow == nullptr) {
        const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("DBCAD"),
            tr("当前窗口未初始化\n"
                "是否初始化？"),
            QMessageBox::Yes | QMessageBox::No);
        switch (ret) {
            case QMessageBox::Yes:
                addWindow();
        }
        return;
    }

    if (curWindow->getSelectedEntities().size() == 0) {
        QMessageBox::warning(this, tr("DBCAD"), tr("请在实体树中选择导出对象"), QMessageBox::Ok);
        return;
    }

    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix(QString("txt"));
    if (dialog.exec() != QDialog::Accepted) return;
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    bool ss = false;
    FILE* file = fopen(dialog.selectedFiles().first().toStdString().c_str(), "a+");
    if (file) {
        for (auto ei : curWindow->getSelectedEntities()) {
            ENTITY_TREE_ITEM* ptrEti = curWindow->getEntityItemByIndex(ei);
            dump_object(file, ptrEti->name.c_str(), 0, ptrEti->ptrEntity);
        }
        fclose(file);
        ss = true;
    }
    QGuiApplication::restoreOverrideCursor();
    if (ss)
        statusBar()->showMessage(tr("已保存"), 2000);
    else
        QMessageBox::warning(this, tr("DBCAD"), tr("保存文件%1失败。").arg(QDir::toNativeSeparators(dialog.selectedFiles().first())));
}

void MainWindow::saveDebugFile() {
    if (curWindow == nullptr) {
        const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("DBCAD"),
            tr("当前窗口未初始化\n"
                "是否初始化？"),
            QMessageBox::Yes | QMessageBox::No);
        switch (ret) {
            case QMessageBox::Yes:
                addWindow();
        }
        return;
    }

    if (curWindow->getSelectedEntities().size() == 0) {
        QMessageBox::warning(this, tr("DBCAD"), tr("请在实体树中选择导出对象"), QMessageBox::Ok);
        return;
    }

    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix(QString("dbg"));
    if (dialog.exec() != QDialog::Accepted) return;
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    bool ss = false;
    FILE* file = fopen(dialog.selectedFiles().first().toStdString().c_str(), "a+");
    if (file) {
        for (auto ei : curWindow->getSelectedEntities()) {
            ENTITY_TREE_ITEM* ptrEti = curWindow->getEntityItemByIndex(ei);
            api_set_int_option("brief_surface_debug", false);
            api_set_int_option("brief_curve_debug", false);
            api_set_int_option("brief_pcurve_debug", false);
            debug_entity(ptrEti->ptrEntity, file);
        }
        fclose(file);
        ss = true;
    }
    QGuiApplication::restoreOverrideCursor();
    if (ss)
        statusBar()->showMessage(tr("已保存"), 2000);
    else
        QMessageBox::warning(this, tr("DBCAD"), tr("保存文件%1失败。").arg(QDir::toNativeSeparators(dialog.selectedFiles().first())));
}

bool MainWindow::saveAs() {
    QAction* checkedAct = setModeActGroup->checkedAction();
    if (checkedAct == setACISModeAct) {
        QFileDialog dialog(this);
        dialog.setWindowModality(Qt::WindowModal);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setDefaultSuffix(QString("sat"));
        if (dialog.exec() != QDialog::Accepted) return false;
        return saveFile(dialog.selectedFiles().first());
    } else {
        assert(checkedAct == setNEO4JModeAct || checkedAct == setNEO4JIncrementalModeAct || checkedAct == setMEMGRAPHModeAct);
        bool ok;
        QDateTime curDateTime = QDateTime::currentDateTime();
        QString curDateTime_qstr = QString("part") + curDateTime.toString("yyyyMMddhhmmss");  // 保存零件对话框的默认零件名是part+当前年月日时分秒
        QString text = QInputDialog::getText(this, tr("保存零件(neo4j)"), tr("请输入保存的零件名:"), QLineEdit::Normal, curDateTime_qstr, &ok, this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
        if (ok && !text.isEmpty()) {
            return saveFile(text);
        } else {
            return false;
        }
    }
}

void MainWindow::visiablility() {
    if (curWindow) curWindow->visiablility();
}

void MainWindow::displayInfo() {
    if (curWindow) curWindow->displayInfo();
}

void MainWindow::about() {
    {
        ENTITY_LIST el;
        BODY* ptrBody = nullptr;
        api_solid_sphere(SPAposition(1.3, 2.5, -3.4), 5.2, ptrBody);
        el.add(ptrBody);
        std::unordered_map<void*, int64_t> ptr2elemid;
        api_save_entity_list_postgresql(*postgresqldb_conn, el, ptr2elemid);
        api_del_entity_list(el);
    }

    QMessageBox::about(this, tr("关于DBCAD"), tr("用于测试和演示DBCAD功能与用法。"));
}

void MainWindow::runTest() {
    Neo4jPart conn(neo4jdb_host.c_str(), neo4jdb_port_bolt, neo4jdb_username.c_str(), neo4jdb_password.c_str(), "");

    for (const auto& entry : std::filesystem::directory_iterator(".\\testcases")) {
        if (entry.is_regular_file()) {
            std::string filepath = entry.path().string();
            std::string extname = std::filesystem::path(filepath).extension().string();
            std::transform(extname.begin(), extname.end(), extname.begin(), tolower); //忽略大小写
            if (extname == ".sat") {
                std::string TestCaseName = std::filesystem::path(filepath).stem().string();
                ENTITY_LIST el;
                acis_restore_entity_list(el, filepath.c_str(), 2, 0, true);

                auto [testresult, neo4j_save_duration, acis_save_duration, neo4j_restore_duration, acis_restore_duration] = AccessTest::CheckTestCase(conn, TestCaseName, el);
                api_del_entity_list(el);

                std::ofstream logfs("testlog.txt", std::ios::app);
                logfs << std::format("=======Test: {}=======", filepath) << std::endl;
                logfs << (testresult ? "PASS" : "FAIL") << "\t数据库存:" << neo4j_save_duration << "ms\t文件存:" << acis_save_duration << "ms\t数据库存/文件存:" << neo4j_save_duration / acis_save_duration << "\t数据库取:" << neo4j_restore_duration << "ms\t文件取:" << acis_restore_duration << "ms\t数据库取/文件取:" << neo4j_restore_duration / acis_restore_duration << std::endl;
                logfs.close();
            }
        }
    }

    QMessageBox::information(this, tr("DBCAD"), tr("neo4j测试运行结束，请打开./testlog.txt文件查看测试报告。"));
}

void MainWindow::runTest_memgraph() {
    Neo4jPart conn(memgraphdb_host.c_str(), memgraphdb_port_bolt, memgraphdb_username.c_str(), memgraphdb_password.c_str(), "");

    for (const auto& entry : std::filesystem::directory_iterator(".\\testcases")) {
        if (entry.is_regular_file()) {
            std::string filepath = entry.path().string();
            std::string extname = std::filesystem::path(filepath).extension().string();
            std::transform(extname.begin(), extname.end(), extname.begin(), tolower); //忽略大小写
            if (extname == ".sat") {
                std::string TestCaseName = std::filesystem::path(filepath).stem().string();
                ENTITY_LIST el;
                acis_restore_entity_list(el, filepath.c_str(), 2, 0, true);

                auto [testresult, neo4j_save_duration, acis_save_duration, neo4j_restore_duration, acis_restore_duration] = AccessTest::CheckTestCase_memgraph(conn, TestCaseName, el);
                api_del_entity_list(el);

                std::ofstream logfs("testlog_memgraph.txt", std::ios::app);
                logfs << std::format("=======Test: {}=======", filepath) << std::endl;
                logfs << (testresult ? "PASS" : "FAIL") << "\t数据库存:" << neo4j_save_duration << "ms\t文件存:" << acis_save_duration << "ms\t数据库存/文件存:" << neo4j_save_duration / acis_save_duration << "\t数据库取:" << neo4j_restore_duration << "ms\t文件取:" << acis_restore_duration << "ms\t数据库取/文件取:" << neo4j_restore_duration / acis_restore_duration << std::endl;
                logfs.close();
            }
        }
    }

    QMessageBox::information(this, tr("DBCAD"), tr("memgraph测试运行结束，请打开./testlog_memgraph.txt文件查看测试报告。"));
}

void MainWindow::runTest_memgraph_neo4j() {
    Neo4jPart conn_memgraph(memgraphdb_host.c_str(), memgraphdb_port_bolt, memgraphdb_username.c_str(), memgraphdb_password.c_str(), "");
    Neo4jPart conn_neo4j(neo4jdb_host.c_str(), neo4jdb_port_bolt, neo4jdb_username.c_str(), neo4jdb_password.c_str(), "");

    for (const auto& entry : std::filesystem::directory_iterator(".\\testcases")) {
        if (entry.is_regular_file()) {
            std::string filepath = entry.path().string();
            std::string extname = std::filesystem::path(filepath).extension().string();
            std::transform(extname.begin(), extname.end(), extname.begin(), tolower); //忽略大小写
            if (extname == ".sat") {
                qDebug() << std::format("=======Test: {}=======", filepath);
                std::string TestCaseName = std::filesystem::path(filepath).stem().string();
                ENTITY_LIST el;
                acis_restore_entity_list(el, filepath.c_str(), 2, 0, true);
                auto [testresult_memgraph, testresult_neo4j, memgraph_save_duration, neo4j_save_duration, acis_save_duration, memgraph_restore_duration, neo4j_restore_duration, acis_restore_duration] = AccessTest::CheckTestCase_memgraph_neo4j(conn_memgraph, conn_neo4j, TestCaseName, el);
                api_del_entity_list(el);

                std::ofstream logfs("testlog_memgraph_neo4j.txt", std::ios::app);
                logfs << std::format("=======Test: {}=======", filepath) << std::endl;
                logfs << "\tmemgraph存:" << memgraph_save_duration << "ms\tneo4j存:" << neo4j_save_duration << "ms\t文件存:" << acis_save_duration
                    << "ms\tmemgraph存/文件存:" << memgraph_save_duration / acis_save_duration << "\tneo4j存/文件存:" << neo4j_save_duration / acis_save_duration
                    << "\tmemgraph取:" << memgraph_restore_duration << "ms\tneo4j取:" << neo4j_restore_duration << "ms\t文件取:" << acis_restore_duration
                    << "ms\tmemgraph取/文件取:" << memgraph_restore_duration / acis_restore_duration << "\tneo4j取/文件取:" << neo4j_restore_duration / acis_restore_duration
                    << std::endl;

                int measurecnt = 1;
                for (; measurecnt < 10; measurecnt++) {
                    ENTITY_LIST el;
                    acis_restore_entity_list(el, filepath.c_str(), 2, 0, true);
                    auto [i_testresult_memgraph, i_testresult_neo4j, i_memgraph_save_duration, i_neo4j_save_duration, i_acis_save_duration, i_memgraph_restore_duration, i_neo4j_restore_duration, i_acis_restore_duration] = AccessTest::CheckTestCase_memgraph_neo4j(conn_memgraph, conn_neo4j, TestCaseName, el);
                    api_del_entity_list(el);

                    logfs << "\tmemgraph存:" << i_memgraph_save_duration << "ms\tneo4j存:" << i_neo4j_save_duration << "ms\t文件存:" << i_acis_save_duration
                        << "ms\tmemgraph存/文件存:" << i_memgraph_save_duration / i_acis_save_duration << "\tneo4j存/文件存:" << i_neo4j_save_duration / i_acis_save_duration
                        << "\tmemgraph取:" << i_memgraph_restore_duration << "ms\tneo4j取:" << i_neo4j_restore_duration << "ms\t文件取:" << i_acis_restore_duration
                        << "ms\tmemgraph取/文件取:" << i_memgraph_restore_duration / i_acis_restore_duration << "\tneo4j取/文件取:" << i_neo4j_restore_duration / i_acis_restore_duration
                        << std::endl;

                    assert(i_testresult_memgraph == testresult_memgraph);
                    assert(i_testresult_neo4j == testresult_neo4j);
                    memgraph_save_duration = (memgraph_save_duration * measurecnt + i_memgraph_save_duration) / (measurecnt + 1.0);
                    neo4j_save_duration = (neo4j_save_duration * measurecnt + i_neo4j_save_duration) / (measurecnt + 1.0);
                    acis_save_duration = (acis_save_duration * measurecnt + i_acis_save_duration) / (measurecnt + 1.0);
                    memgraph_restore_duration = (memgraph_restore_duration * measurecnt + i_memgraph_restore_duration) / (measurecnt + 1.0);
                    neo4j_restore_duration = (neo4j_restore_duration * measurecnt + i_neo4j_restore_duration) / (measurecnt + 1.0);
                    acis_restore_duration = (acis_restore_duration * measurecnt + i_acis_restore_duration) / (measurecnt + 1.0);
                }


                logfs << (testresult_memgraph ? "memgraphPASS" : "memgraphFAIL") << "\t" << (testresult_neo4j ? "neo4jPASS" : "neo4jFAIL")
                    << "\tmemgraph存:" << memgraph_save_duration << "ms\tneo4j存:" << neo4j_save_duration << "ms\t文件存:" << acis_save_duration
                    << "ms\tmemgraph存/文件存:" << memgraph_save_duration / acis_save_duration << "\tneo4j存/文件存:" << neo4j_save_duration / acis_save_duration
                    << "\tmemgraph取:" << memgraph_restore_duration << "ms\tneo4j取:" << neo4j_restore_duration << "ms\t文件取:" << acis_restore_duration
                    << "ms\tmemgraph取/文件取:" << memgraph_restore_duration / acis_restore_duration << "\tneo4j取/文件取:" << neo4j_restore_duration / acis_restore_duration
                    << std::endl;
                logfs.close();
            }
        }
    }

    QMessageBox::information(this, tr("DBCAD"), tr("memgraph_neo4j测试运行结束，请打开./testlog_memgraph_neo4j.txt文件查看测试报告。"));
}

void MainWindow::runTest_postgresql_neo4j() {
    Neo4jPart conn_neo4j(neo4jdb_host.c_str(), neo4jdb_port_bolt, neo4jdb_username.c_str(), neo4jdb_password.c_str(), "");

    for (const auto& entry : std::filesystem::directory_iterator(".\\testcases_postgresql")) {
        if (entry.is_regular_file()) {
            std::string filepath = entry.path().string();
            std::string extname = std::filesystem::path(filepath).extension().string();
            std::transform(extname.begin(), extname.end(), extname.begin(), tolower); //忽略大小写
            if (extname == ".sat") {
                qDebug() << std::format("=======Test: {}=======", filepath);
                std::string TestCaseName = std::filesystem::path(filepath).stem().string();
                ENTITY_LIST el;
                acis_restore_entity_list(el, filepath.c_str(), 2, 0, true);
                auto [testresult_postgresql, testresult_neo4j, postgresql_save_duration, neo4j_save_duration, acis_save_duration, postgresql_restore_duration, neo4j_restore_duration, acis_restore_duration] = AccessTest::CheckTestCase_postgresql_neo4j(*postgresqldb_conn, conn_neo4j, TestCaseName, el);
                api_del_entity_list(el);

                std::ofstream logfs("testlog_postgresql_neo4j.txt", std::ios::app);
                logfs << std::format("=======Test: {}=======", filepath) << std::endl;
                logfs << "\tpostgresql存:" << postgresql_save_duration << "ms\tneo4j存:" << neo4j_save_duration << "ms\t文件存:" << acis_save_duration
                    << "ms\tpostgresql存/文件存:" << postgresql_save_duration / acis_save_duration << "\tneo4j存/文件存:" << neo4j_save_duration / acis_save_duration
                    << "\tpostgresql取:" << postgresql_restore_duration << "ms\tneo4j取:" << neo4j_restore_duration << "ms\t文件取:" << acis_restore_duration
                    << "ms\tpostgresql取/文件取:" << postgresql_restore_duration / acis_restore_duration << "\tneo4j取/文件取:" << neo4j_restore_duration / acis_restore_duration
                    << std::endl;

                int measurecnt = 1;
                for (; measurecnt < 10; measurecnt++) {
                    ENTITY_LIST el;
                    acis_restore_entity_list(el, filepath.c_str(), 2, 0, true);
                    auto [i_testresult_postgresql, i_testresult_neo4j, i_postgresql_save_duration, i_neo4j_save_duration, i_acis_save_duration, i_postgresql_restore_duration, i_neo4j_restore_duration, i_acis_restore_duration] = AccessTest::CheckTestCase_postgresql_neo4j(*postgresqldb_conn, conn_neo4j, TestCaseName, el);
                    api_del_entity_list(el);

                    logfs << "\tpostgresql存:" << i_postgresql_save_duration << "ms\tneo4j存:" << i_neo4j_save_duration << "ms\t文件存:" << i_acis_save_duration
                        << "ms\tpostgresql存/文件存:" << i_postgresql_save_duration / i_acis_save_duration << "\tneo4j存/文件存:" << i_neo4j_save_duration / i_acis_save_duration
                        << "\tpostgresql取:" << i_postgresql_restore_duration << "ms\tneo4j取:" << i_neo4j_restore_duration << "ms\t文件取:" << i_acis_restore_duration
                        << "ms\tpostgresql取/文件取:" << i_postgresql_restore_duration / i_acis_restore_duration << "\tneo4j取/文件取:" << i_neo4j_restore_duration / i_acis_restore_duration
                        << std::endl;

                    assert(i_testresult_postgresql == testresult_postgresql);
                    assert(i_testresult_neo4j == testresult_neo4j);
                    postgresql_save_duration = (postgresql_save_duration * measurecnt + i_postgresql_save_duration) / (measurecnt + 1.0);
                    neo4j_save_duration = (neo4j_save_duration * measurecnt + i_neo4j_save_duration) / (measurecnt + 1.0);
                    acis_save_duration = (acis_save_duration * measurecnt + i_acis_save_duration) / (measurecnt + 1.0);
                    postgresql_restore_duration = (postgresql_restore_duration * measurecnt + i_postgresql_restore_duration) / (measurecnt + 1.0);
                    neo4j_restore_duration = (neo4j_restore_duration * measurecnt + i_neo4j_restore_duration) / (measurecnt + 1.0);
                    acis_restore_duration = (acis_restore_duration * measurecnt + i_acis_restore_duration) / (measurecnt + 1.0);
                }


                logfs << (testresult_postgresql ? "postgresqlPASS" : "postgresqlFAIL") << "\t" << (testresult_neo4j ? "neo4jPASS" : "neo4jFAIL")
                    << "\tpostgresql存:" << postgresql_save_duration << "ms\tneo4j存:" << neo4j_save_duration << "ms\t文件存:" << acis_save_duration
                    << "ms\tpostgresql存/文件存:" << postgresql_save_duration / acis_save_duration << "\tneo4j存/文件存:" << neo4j_save_duration / acis_save_duration
                    << "\tpostgresql取:" << postgresql_restore_duration << "ms\tneo4j取:" << neo4j_restore_duration << "ms\t文件取:" << acis_restore_duration
                    << "ms\tpostgresql取/文件取:" << postgresql_restore_duration / acis_restore_duration << "\tneo4j取/文件取:" << neo4j_restore_duration / acis_restore_duration
                    << std::endl;
                logfs.close();
            }
        }
    }

    QMessageBox::information(this, tr("DBCAD"), tr("postgresql_neo4j测试运行结束，请打开./testlog_postgresql_neo4j.txt文件查看测试报告。"));
}

void MainWindow::incrementalTest() {
    TMDF;

    {

        std::ofstream logfs("incremental_dighole_testlog.txt", std::ios::app);
        BODY* block;
        check_outcome(api_solid_block(SPAposition(0.0, 0.0, 0.0), SPAposition(1000.0, 2000.0, 300.0), block));
        bool is_through = false;
        int holecnt = 0;
        for (int i = 10; i <= 990; i += 20) { //50个
            for (int j = 10; j <= 1990; j += 20) { //100个
                BODY* hole;
                check_outcome(api_solid_cylinder_cone(SPAposition(i, j, is_through ? -10.0 : 100.0), SPAposition(i, j, 310.0), 5.0, 5.0, 5.0, nullptr, hole));
                check_outcome(api_subtract(hole, block));
                holecnt++;

                std::string acis_save_filename_str = std::format("acis_partTest_save_{}.sat", holecnt);
                const char* acis_save_filename = acis_save_filename_str.c_str();
                std::string acis_save_check_filename_str = std::format("check_acis_partTest_save_{}.sat", holecnt);
                const char* acis_save_check_filename = acis_save_check_filename_str.c_str();
                std::string conn_check_filename_str = std::format("check_incrementalTest_{}.sat", holecnt);
                const char* conn_check_filename = conn_check_filename_str.c_str();

                Neo4jPart conn_incremental(neo4jdb_host.c_str(), neo4jdb_port_bolt, neo4jdb_username.c_str(), neo4jdb_password.c_str(), "incrementalTest");

                //增量存
                TMST;
                api_save_neo4j(conn_incremental);
                TMED;
                double neo4j_incremental_save_duration = TMDR;

                //ACIS存
                TMST;
                acis_save_noattrib_toplevel_active_entities(acis_save_filename, 2, 0, true);
                TMED;
                double acis_save_duration = TMDR;

                is_through = !is_through;
            }
        }
    }

    QMessageBox::information(this, tr("DBCAD"), tr("增量存取测试结束。"));
}

void MainWindow::createActions() {
    QMenu* fileMenu = menuBar()->addMenu(tr("文件(&F)"));
    QToolBar* fileToolBar = addToolBar(tr("文件"));

    // 清空当前Window
    const QIcon clearIcon = QIcon::fromTheme("document-clear", QIcon(":/images/clear.png"));
    QAction* clearAct = new QAction(clearIcon, tr("清空(&L)"), this);
    clearAct->setStatusTip(tr("清空"));
    connect(clearAct, &QAction::triggered, this, &MainWindow::clear);
    fileMenu->addAction(clearAct);
    fileToolBar->addAction(clearAct);

    // 创建Window
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction* newAct = new QAction(newIcon, tr("新建(&N)"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("新建文件"));
    connect(newAct, &QAction::triggered, this, &MainWindow::addWindow);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction* openAct = new QAction(openIcon, tr("打开(&O)"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("打开已有文件"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

    const QIcon undoIcon = QIcon::fromTheme("document-undo", QIcon(":/images/undo.png"));
    QAction* undoAct = new QAction(undoIcon, tr("撤销(&U)"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("撤销"));
    connect(undoAct, &QAction::triggered, this, &MainWindow::undo);
    fileMenu->addAction(undoAct);
    fileToolBar->addAction(undoAct);

    const QIcon redoIcon = QIcon::fromTheme("document-redo", QIcon(":/images/redo.png"));
    QAction* redoAct = new QAction(redoIcon, tr("重做(&Y)"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("重做"));
    connect(redoAct, &QAction::triggered, this, &MainWindow::redo);
    fileMenu->addAction(redoAct);
    fileToolBar->addAction(redoAct);

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction* saveAct = new QAction(saveIcon, tr("保存模型(&S)"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("保存模型"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);

    const QIcon saveEntityIcon = QIcon::fromTheme("document-save", QIcon(":/images/save_entity.png"));
    QAction* saveEntityAct = new QAction(saveEntityIcon, tr("保存模型树(&D)"), this);
    saveEntityAct->setShortcut(tr("ctrl+D"));
    saveEntityAct->setStatusTip(tr("导出模型结构"));
    connect(saveEntityAct, &QAction::triggered, this, &MainWindow::saveEntity);
    fileMenu->addAction(saveEntityAct);
    fileToolBar->addAction(saveEntityAct);

    const QIcon saveDebugFileIcon = QIcon::fromTheme("document-save", QIcon(":/images/save_entity.png"));
    QAction* saveDebugFileAct = new QAction(saveDebugFileIcon, tr("保存调试文件(&D)"), this);
    saveDebugFileAct->setStatusTip(tr("保存调试文件"));
    connect(saveDebugFileAct, &QAction::triggered, this, &MainWindow::saveDebugFile);
    fileMenu->addAction(saveDebugFileAct);
    fileToolBar->addAction(saveDebugFileAct);

    const QIcon saveImageIcon = QIcon::fromTheme("document-save", QIcon(":/images/save_image.png"));
    QAction* saveImageAct = new QAction(saveImageIcon, tr("保存截图(&T)"), this);
    saveImageAct->setStatusTip(tr("保存截图"));
    connect(saveImageAct, &QAction::triggered, this, &MainWindow::saveImage);
    fileMenu->addAction(saveImageAct);
    fileToolBar->addAction(saveImageAct);

    const QIcon saveAsIcon = QIcon::fromTheme("document-save-as", QIcon(":/images/saveas.png"));
    QAction* saveAsAct = new QAction(saveAsIcon, tr("另存为(&A)"), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("另存为新模型"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);
    fileMenu->addAction(saveAsAct);
    fileToolBar->addAction(saveAsAct);

    const QIcon visiablilityIcon = QIcon::fromTheme("document-visiablility", QIcon(":/images/visiablility.png"));
    QAction* visiablilityAct = new QAction(visiablilityIcon, tr("显示(&V)"), this);
    visiablilityAct->setStatusTip(tr("是否显示"));
    connect(visiablilityAct, &QAction::triggered, this, &MainWindow::visiablility);
    fileMenu->addAction(visiablilityAct);
    fileToolBar->addAction(visiablilityAct);

    const QIcon infoIcon = QIcon::fromTheme("document-info", QIcon(":/images/info.png"));
    QAction* infoAct = new QAction(infoIcon, tr("模型信息(&I)"), this);
    infoAct->setStatusTip(tr("模型信息"));
    connect(infoAct, &QAction::triggered, this, &MainWindow::displayInfo);
    fileMenu->addAction(infoAct);
    fileToolBar->addAction(infoAct);

    fileMenu->addSeparator();

    // 退出系统
    const QIcon exitIcon = QIcon::fromTheme("application-exit", QIcon(":/images/exit.png"));
    QAction* exitAct = fileMenu->addAction(exitIcon, tr("退出(&X)"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("退出系统"));

    // 插入基本体的菜单项和工具栏
    QMenu* insertMenu = menuBar()->addMenu(tr("插入(&I)"));
    QMenu* insertSubMenu = nullptr;
    QToolBar* insertSubToolBar = nullptr;
    for (auto it = menus_entities.begin(); it != menus_entities.end(); ++it) {
        if (it->operatorType == OPERATOR_TYPES::OPERATOR_SPERATOR) {
            if (it->subOperatorType == 0) {
                insertSubMenu = insertMenu->addMenu(it->name_cn.c_str());
                insertSubToolBar = addToolBar(tr("插入%1").arg(it->name_cn.c_str()));
            } else if (it->subOperatorType == 1)
                insertToolBarBreak(insertSubToolBar);

            continue;
        }
        const QIcon icon = QIcon::fromTheme("application", QIcon(QString::fromStdString(":/images/" + it->name_en + ".png")));
        QAction* act = insertSubMenu->addAction(icon, tr(it->name_cn.c_str()), this, [=]() { this->insertElements(it->operatorType, it->subOperatorType); });
        act->setCheckable(it->checkable);
        act->setStatusTip(QString::fromStdString("插入" + it->name_cn));
        insertSubToolBar->addAction(act);
    }

    // 插入布尔运算菜单项和工具栏
    QMenu* operatorMenu = menuBar()->addMenu(tr("操作(&O)"));
    QToolBar* operatorToolBar = addToolBar(tr("操作"));
    for (auto it = menus_operators.begin(); it != menus_operators.end(); ++it) {
        if (it->operatorType == OPERATOR_TYPES::OPERATOR_SPERATOR) {
            operatorMenu->addSeparator();
            operatorToolBar = addToolBar(tr("操作"));
            continue;
        }
        const QIcon icon = QIcon::fromTheme("application", QIcon(QString::fromStdString(":/images/" + it->name_en + ".png")));
        QAction* act = operatorMenu->addAction(icon, tr(it->name_cn.c_str()), this, [=]() { this->operation(it->operatorType, it->subOperatorType); });
        act->setCheckable(it->checkable);
        act->setStatusTip(QString::fromStdString(it->name_cn));
        operatorToolBar->addAction(act);
    }

    // 使用帮助
    QMenu* helpMenu = menuBar()->addMenu(tr("帮助(&H)"));
    QAction* aboutAct = helpMenu->addAction(tr("关于(&A)"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("显示系统说明"));
    QAction* runTestAct = helpMenu->addAction(tr("运行测试neo4j(&T)"), this, &MainWindow::runTest);
    runTestAct->setStatusTip(tr("使用./testcases目录下的SAT文件（2.0版本，不含attrib类实体）作为测试用例，对基于neo4j数据库的几何模型存取接口进行功能和性能测试，测试报告以追加方式输出至./testlog.txt文件"));
    QAction* runTestAct_memgraph = helpMenu->addAction(tr("运行测试memgraph(&T)"), this, &MainWindow::runTest_memgraph);
    runTestAct_memgraph->setStatusTip(tr("使用./testcases目录下的SAT文件（2.0版本，不含attrib类实体）作为测试用例，对基于memgraph数据库的几何模型存取接口进行功能和性能测试，测试报告以追加方式输出至./testlog_memgraph.txt文件"));
    QAction* runTestAct_memgraph_neo4j = helpMenu->addAction(tr("运行测试memgraph_neo4j(&T)"), this, &MainWindow::runTest_memgraph_neo4j);
    runTestAct_memgraph_neo4j->setStatusTip(tr("使用./testcases目录下的SAT文件（2.0版本，不含attrib类实体）作为测试用例，对基于memgraph和neo4j数据库的几何模型存取接口进行功能和性能测试，测试报告以追加方式输出至./testlog_memgraph_neo4j.txt文件"));
    QAction* runTestAct_postgresql_neo4j = helpMenu->addAction(tr("运行测试postgresql_neo4j(&T)"), this, &MainWindow::runTest_postgresql_neo4j);
    runTestAct_postgresql_neo4j->setStatusTip(tr("使用./testcases目录下的SAT文件（2.0版本，不含attrib类实体）作为测试用例，对基于postgresql和neo4j数据库的几何模型存取接口进行功能和性能测试，测试报告以追加方式输出至./testlog_postgresql_neo4j.txt文件"));
    QAction* incrementalTestAct = helpMenu->addAction(tr("增量存取测试"), this, &MainWindow::incrementalTest);
    incrementalTestAct->setStatusTip(tr("增量存取测试"));


    // 设置存取模式
    QMenu* settingsMenu = menuBar()->addMenu(tr("设置存取模式(&S)"));

    setACISModeAct = new QAction(tr("SAT文件(ACIS)(&A)"), this);
    setACISModeAct->setStatusTip(tr("存取为SAT文件，存取函数使用ACIS提供的API"));
    setACISModeAct->setCheckable(true);
    settingsMenu->addAction(setACISModeAct);

    setNEO4JModeAct = new QAction(tr("neo4j图数据全量存取(&N)"), this);
    setNEO4JModeAct->setStatusTip(tr("全量存取为neo4j图数据，通过Bolt协议连接到neo4j图数据库"));
    setNEO4JModeAct->setCheckable(true);
    settingsMenu->addAction(setNEO4JModeAct);

    setNEO4JIncrementalModeAct = new QAction(tr("neo4j图数据增量存取(&I)"), this);
    setNEO4JIncrementalModeAct->setStatusTip(tr("增量存取为neo4j图数据，通过Bolt协议连接到neo4j图数据库"));
    setNEO4JIncrementalModeAct->setCheckable(true);
    settingsMenu->addAction(setNEO4JIncrementalModeAct);

    setMEMGRAPHModeAct = new QAction(tr("memgraph内存图数据全量存取(&M)"), this);
    setMEMGRAPHModeAct->setStatusTip(tr("全量存取为memgraph内存图数据，通过Bolt协议连接到memgraph内存图数据库"));
    setMEMGRAPHModeAct->setCheckable(true);
    settingsMenu->addAction(setMEMGRAPHModeAct);

    setModeActGroup = new QActionGroup(this);
    setModeActGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::Exclusive);
    setModeActGroup->addAction(setACISModeAct);
    setModeActGroup->addAction(setNEO4JModeAct);
    setModeActGroup->addAction(setNEO4JIncrementalModeAct);
    setModeActGroup->addAction(setMEMGRAPHModeAct);
    setACISModeAct->setChecked(true);

    settingsMenu->addSeparator();
    QAction* setNEO4JConnectInfoAct = settingsMenu->addAction(tr("设置neo4j连接信息"), this, &MainWindow::setNEO4JConnectInfo);
    setNEO4JConnectInfoAct->setStatusTip(tr("读取与程序相同目录下的neo4j_connect_info.conf文件，设置neo4j图数据库连接信息。该文件第1行是IP地址，第2行是Bolt协议端口号，第3行是用户名，第4行是密码，不能包含空格和多余的换行符，编码须为UTF-8（无BOM）。"));
    settingsMenu->addAction(setNEO4JConnectInfoAct);
    QAction* setMEMGRAPHConnectInfoAct = settingsMenu->addAction(tr("设置memgraph连接信息"), this, &MainWindow::setMEMGRAPHConnectInfo);
    setMEMGRAPHConnectInfoAct->setStatusTip(tr("读取与程序相同目录下的memgraph_connect_info.conf文件，设置memgraph内存图数据库连接信息。该文件第1行是IP地址，第2行是Bolt协议端口号，第3行是用户名，第4行是密码，不能包含空格和多余的换行符，编码须为UTF-8（无BOM）。"));
    settingsMenu->addAction(setMEMGRAPHConnectInfoAct);
    QAction* setPOSTGRESQLConnectInfoAct = settingsMenu->addAction(tr("设置postgresql连接信息"), this, &MainWindow::setPOSTGRESQLConnectInfo);
    setPOSTGRESQLConnectInfoAct->setStatusTip(tr("读取与程序相同目录下的postgresql_connect_info.conf文件，设置postgresql关系数据库连接信息。该文件第1行是PostgreSQL Keyword/Value Connection String，不能包含空格和多余的换行符，编码须为UTF-8（无BOM）。"));
    settingsMenu->addAction(setPOSTGRESQLConnectInfoAct);
}

void MainWindow::setNEO4JConnectInfo() {
    std::ifstream fs;
    fs.open("neo4j_connect_info.conf", std::ios_base::in);
    if (fs.is_open()) {
        // 判断序号是否在begin和end之间
        std::string str, str_trim;
        int i = 0;
        while (std::getline(fs, str)) {
            size_t first = str.find_first_not_of(' ');
            if (first == std::string::npos) {
                str_trim = str;
            } else {
                size_t last = str.find_last_not_of(' ');
                str_trim = str.substr(first, (last - first + 1));
            }
            // if(str_trim.size() == 0) continue;
            if (i == 0) {  // IP地址
                neo4jdb_host = str_trim;
            } else if (i == 1) {  // Bolt端口号
                neo4jdb_port_bolt = std::stoi(str_trim);
            } else if (i == 2) {  // 用户名(第4行是空行说明用户名为"")
                neo4jdb_username = str_trim;
            } else if (i == 3) {  // 密码(第5行是空行说明密码为"")
                neo4jdb_password = str_trim;
                break;
            }
            i++;
        }
        statusBar()->showMessage(tr("neo4j连接信息已设置"), 2000);
    } else {
        statusBar()->showMessage(tr("neo4j_connect_info.conf文件打开失败！"), 2000);
    }
}

void MainWindow::setMEMGRAPHConnectInfo() {
    std::ifstream fs;
    fs.open("memgraph_connect_info.conf", std::ios_base::in);
    if (fs.is_open()) {
        // 判断序号是否在begin和end之间
        std::string str, str_trim;
        int i = 0;
        while (std::getline(fs, str)) {
            size_t first = str.find_first_not_of(' ');
            if (first == std::string::npos) {
                str_trim = str;
            } else {
                size_t last = str.find_last_not_of(' ');
                str_trim = str.substr(first, (last - first + 1));
            }
            // if(str_trim.size() == 0) continue;
            if (i == 0) {  // IP地址
                memgraphdb_host = str_trim;
            } else if (i == 1) {  // Bolt端口号
                memgraphdb_port_bolt = std::stoi(str_trim);
            } else if (i == 2) {  // 用户名(第4行是空行说明用户名为"")
                memgraphdb_username = str_trim;
            } else if (i == 3) {  // 密码(第5行是空行说明密码为"")
                memgraphdb_password = str_trim;
                break;
            }
            i++;
        }
        statusBar()->showMessage(tr("memgraph连接信息已设置"), 2000);
    } else {
        statusBar()->showMessage(tr("memgraph_connect_info.conf文件打开失败！"), 2000);
    }
}

void MainWindow::setPOSTGRESQLConnectInfo() {
    std::ifstream fs;
    fs.open("postgresql_connect_info.conf", std::ios_base::in);
    if (fs.is_open()) {
        std::string str;
        std::getline(fs, str);
        size_t first = str.find_first_not_of(' ');
        if (first == std::string::npos) {
            postgresqldb_connection_string = str;
        } else {
            size_t last = str.find_last_not_of(' ');
            postgresqldb_connection_string = str.substr(first, (last - first + 1));
        }
        if (postgresqldb_conn != nullptr) {
            delete postgresqldb_conn;
        }
        postgresqldb_conn = new pqxx::connection(postgresqldb_connection_string);
        statusBar()->showMessage(tr("postgresql连接信息已设置"), 2000);
    } else {
        statusBar()->showMessage(tr("postgresql_connect_info.conf文件打开失败！"), 2000);
    }
}

void MainWindow::clear() {
    curWindow->clear();
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage(tr("已就绪"));
}


void MainWindow::insertElements(const OPERATOR_TYPES ot, const int subOperatorType) {
    if (!curWindow) return;
    ENTITY* ptrEntity = nullptr;
    std::vector<std::vector<SPAposition>> handles;
    curWindow->createEntity(subOperatorType, ptrEntity, handles);
    if (ptrEntity == nullptr) return;

    std::string name_cn = "实体";
    for (auto mi : menus_entities) {
        if (mi.operatorType == ot && mi.subOperatorType == subOperatorType) name_cn = mi.name_cn;
    }

    // curWindow->addEntity(ptrEntity, name_cn, subOperatorType, handles);
    //  @todo：以下版本管理导致demo_qt创建任意实体后再交互式插入B样条时程序崩溃。
    GME_DELTA_STATE_user_data* delta_state_user_data = ACIS_NEW GME_DELTA_STATE_user_data();
    delta_state_user_data->add_tree_item(curWindow->addEntity(ptrEntity, name_cn, subOperatorType, handles));

    DELTA_STATE* ds = nullptr;
    api_note_state(ds);
    ds->set_user_data(delta_state_user_data);

    // debug_history_stream();
}

void MainWindow::operation(const OPERATOR_TYPES ot, const int subOperatorType) {
    if (!curWindow) return;

    std::vector<int> ids = curWindow->getSelectedEntities();
    ENTITY_TREE_ITEM* eti0 = curWindow->getEntityItemByIndex(ids[0]);
    std::string eti0_name = eti0->name;
    ENTITY_TREE_ITEM* eti1 = curWindow->getEntityItemByIndex(ids[1]);
    std::string eti1_name = eti1->name;

    ENTITY_LIST* el = ACIS_NEW ENTITY_LIST();
    if (!curWindow->operation(ot, subOperatorType, ids, el, true)) return;
    std::string name_cn = "运算结果";
    for (auto mi : menus_operators) {
        if (mi.operatorType == ot && mi.subOperatorType == subOperatorType) name_cn = mi.name_cn;
    }
    std::string name_cn2 = std::format("[{}]和[{}]的{}", eti0_name, eti1_name, name_cn);

    ids.clear(); //两实体做布尔运算后，只保留运算结果，删除参与运算的两实体

    GME_DELTA_STATE_user_data* delta_state_user_data = ACIS_NEW GME_DELTA_STATE_user_data();
    for (ENTITY* ent = el->first(); ent; ent = el->next()) {
        delta_state_user_data->add_tree_item(curWindow->addEntity(ent, name_cn2, subOperatorType, ids, ot));
    }

    DELTA_STATE* ds = nullptr;
    api_note_state(ds);
    ds->set_user_data(delta_state_user_data);
    // debug_history_stream();
}

void MainWindow::saveImage() {
    if (curWindow == nullptr) {
        const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("DBCAD"),
            tr("当前窗口未初始化\n"
                "是否初始化？"),
            QMessageBox::Yes | QMessageBox::No);
        switch (ret) {
            case QMessageBox::Yes:
                addWindow();
        }
        return;
    }
    if (curWindow->getEntityList().count() == 0) {
        QMessageBox::warning(this, tr("DBCAD"), tr("当前窗口为空。"));
        return;
    }
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix(QString("png"));
    if (dialog.exec() != QDialog::Accepted) return;
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QImage im = curWindow->getScreenshot();
    bool ss = im.save(dialog.selectedFiles().first(), "PNG");
    QGuiApplication::restoreOverrideCursor();
    if (ss)
        statusBar()->showMessage(tr("截图已保存"), 2000);
    else
        QMessageBox::warning(this, tr("DBCAD"), tr("保存文件%1失败。").arg(QDir::toNativeSeparators(dialog.selectedFiles().first())));
}

bool MainWindow::maybeSave() {
    if (curWindow->getIsModified()) {
        const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("DBCAD"),
            tr("模型已修改。\n"
                "是否保存？"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        switch (ret) {
            case QMessageBox::Save:
                return save();
            case QMessageBox::Cancel:
                return false;
            default:
                break;
        }
    }
    return true;
}

void MainWindow::loadFile(const QString& fileName) {
    if (curWindow == nullptr) addWindow();

    curWindow->clear();

    bool isRead = false;

#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    QAction* checkedAct = setModeActGroup->checkedAction();

    if (checkedAct == setACISModeAct) {
        FILE* f = nullptr;
        f = fopen(fileName.toStdString().c_str(), "r");
        if (f) {
            ENTITY_LIST el;

            API_BEGIN;
            api_save_version(2, 0);
            result = api_restore_entity_list(f, true, el);
            API_END;

            for (int i = 0; i < el.count(); i++) curWindow->addEntity(el[i], tr("导入(ACIS)实体%1").arg(i).toStdString(), -1);
            fclose(f);
            isRead = true;
        } else {
            QMessageBox::warning(this, tr("DBCAD"), tr("无法读取文件%1。").arg(QDir::toNativeSeparators(fileName)));
        }
    } else if (checkedAct == setNEO4JModeAct) {
        Neo4jPart f(neo4jdb_host.c_str(), neo4jdb_port_bolt, neo4jdb_username.c_str(), neo4jdb_password.c_str(), fileName.toStdString());
        int64_t countn = count_partnode(f);
        if (countn == 1) {
            ENTITY_LIST el;
            API_BEGIN;
            api_restore_entity_list_neo4j_part(f, el);
            API_END;
            for (int i = 0; i < el.count(); i++) curWindow->addEntity(el[i], tr("导入(neo4j)实体%1").arg(i).toStdString(), -1);
            isRead = true;
        } else if (countn == 0) {
            QMessageBox::warning(this, tr("DBCAD"), tr("名为%1的零件(neo4j)不存在。").arg(QString(fileName)));
        } else {
            assert(countn > 1);
            QMessageBox::warning(this, tr("DBCAD"), tr("名为%1的零件(neo4j)不唯一。").arg(QString(fileName)));
        }
    } else {
        assert(checkedAct == setMEMGRAPHModeAct);
        Neo4jPart f(memgraphdb_host.c_str(), memgraphdb_port_bolt, memgraphdb_username.c_str(), memgraphdb_password.c_str(), fileName.toStdString());
        int64_t countn = count_partnode(f);
        if (countn == 1) {
            ENTITY_LIST el;
            API_BEGIN;
            api_restore_entity_list_memgraph_part(f, el);
            API_END;
            for (int i = 0; i < el.count(); i++) curWindow->addEntity(el[i], tr("导入(memgraph)实体%1").arg(i).toStdString(), -1);
            isRead = true;
        } else if (countn == 0) {
            QMessageBox::warning(this, tr("DBCAD"), tr("名为%1的零件(memgraph)不存在。").arg(QString(fileName)));
        } else {
            assert(countn > 1);
            QMessageBox::warning(this, tr("DBCAD"), tr("名为%1的零件(memgraph)不唯一。").arg(QString(fileName)));
        }
    }

#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
    if (isRead) {
        if (checkedAct == setACISModeAct) {
            setCurrentFile(fileName);
            statusBar()->showMessage(tr("文件已导入"), 2000);
        } else if (checkedAct == setNEO4JModeAct) {
            setCurrentPartName(fileName);
            statusBar()->showMessage(tr("零件已导入(neo4j)"), 2000);
        } else {
            assert(checkedAct == setMEMGRAPHModeAct);
            setCurrentPartName(fileName);
            statusBar()->showMessage(tr("零件已导入(memgraph)"), 2000);
        }
    }
}

void MainWindow::loadFile(const QString& partName, const int generation) {
    if (curWindow == nullptr) addWindow();

    curWindow->clear();

    bool isRead = false;

#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    QAction* checkedAct = setModeActGroup->checkedAction();

    assert(checkedAct == setNEO4JIncrementalModeAct);
    Neo4jPart f(neo4jdb_host.c_str(), neo4jdb_port_bolt, neo4jdb_username.c_str(), neo4jdb_password.c_str(), partName.toStdString());
    int64_t countn = count_partnode(f);
    if (countn == 1) {
        api_restore_neo4j(f, generation);
        ENTITY_LIST el;
        acis_get_noattrib_toplevel_active_entities(el);
        for (int i = 0; i < el.count(); i++) curWindow->addEntity(el[i], tr("导入(neo4j)实体%1").arg(i).toStdString(), -1);
        isRead = true;
    } else if (countn == 0) {
        QMessageBox::warning(this, tr("DBCAD"), tr("名为%1的零件(neo4j)不存在。").arg(QString(partName)));
    } else {
        assert(countn > 1);
        QMessageBox::warning(this, tr("DBCAD"), tr("名为%1的零件(neo4j)不唯一。").arg(QString(partName)));
    }

#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
    if (isRead) {
        setCurrentPartName(partName);
        statusBar()->showMessage(tr("零件已导入(neo4j)"), 2000);
    }

}

bool MainWindow::saveFile(const QString& fileName) {
    QString errorMessage;

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);

    QAction* checkedAct = setModeActGroup->checkedAction();

    if (checkedAct == setACISModeAct) {
        FILE* f = nullptr;
        f = fopen(fileName.toStdString().c_str(), "wb");
        if (f) {
            API_NOP_BEGIN;
            api_save_version(2, 0);
            FileInfo fileinfo;
            fileinfo.set_units(1.0);
            fileinfo.set_product_id("demo_qt");
            result = api_set_file_info((FileIdent | FileUnits), fileinfo);
            result = api_set_int_option("sequence_save_files", 1);
            ENTITY_LIST el;
            acis_get_noattrib_toplevel_active_entities(el);
            result = api_save_entity_list(f, true, el);
            API_NOP_END;
            fclose(f);
        } else {
            errorMessage = tr("无法以写入方式打开文件%1。").arg(QDir::toNativeSeparators(fileName));
        }

    } else if (checkedAct == setNEO4JModeAct) {
        Neo4jPart f(neo4jdb_host.c_str(), neo4jdb_port_bolt, neo4jdb_username.c_str(), neo4jdb_password.c_str(), fileName.toStdString());
        ENTITY_LIST el;
        acis_get_noattrib_toplevel_active_entities(el);
        api_save_entity_list_neo4j_part(f, el);
    } else if (checkedAct == setNEO4JIncrementalModeAct) {
        Neo4jPart f(neo4jdb_host.c_str(), neo4jdb_port_bolt, neo4jdb_username.c_str(), neo4jdb_password.c_str(), fileName.toStdString());
        api_save_neo4j(f);
    } else {
        assert(checkedAct == setMEMGRAPHModeAct);
        Neo4jPart f(memgraphdb_host.c_str(), memgraphdb_port_bolt, memgraphdb_username.c_str(), memgraphdb_password.c_str(), fileName.toStdString());
        ENTITY_LIST el;
        acis_get_noattrib_toplevel_active_entities(el);
        api_save_entity_list_memgraph_part(f, el);
    }

    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, tr("DBCAD"), errorMessage);
        return false;
    } else
        curWindow->setIsModified(false);

    if (checkedAct == setACISModeAct) {
        setCurrentFile(fileName);
        statusBar()->showMessage(tr("文件已保存"), 2000);
    } else if (checkedAct == setNEO4JModeAct || checkedAct == setNEO4JIncrementalModeAct) {
        setCurrentPartName(fileName);
        statusBar()->showMessage(tr("零件已保存(neo4j)"), 2000);
    } else {
        assert(checkedAct == setMEMGRAPHModeAct);
        setCurrentPartName(fileName);
        statusBar()->showMessage(tr("零件已保存(memgraph)"), 2000);
    }
    return true;
}

void MainWindow::setCurrentFile(const QString& fileName) {
    curWindow->setCurFile(fileName);
    curWindow->setWindowModified(false);
}

void MainWindow::setCurrentPartName(const QString& fileName) {
    curWindow->setCurPartName(fileName);
    curWindow->setWindowModified(false);
}

void MainWindow::showMessage(const QString s, int duration) {
    if (duration == -1)
        statusBar()->showMessage(s);
    else
        statusBar()->showMessage(s, duration);
}

#ifndef QT_NO_SESSIONMANAGER
void MainWindow::commitData(QSessionManager& manager) {
    if (manager.allowsInteraction()) {
        if (!maybeSave()) manager.cancel();
    } else {
        // 自动保存
        if (curWindow->getIsModified()) save();
    }
}
#endif