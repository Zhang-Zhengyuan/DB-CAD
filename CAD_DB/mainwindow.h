#pragma once

#include <QMainWindow>
#include <QSessionManager>

enum OPERATOR_TYPES { OPERATOR_IMPORT = 0, OPERATOR_CONSTRUCTOR, OPERATOR_INTERSECTOR, OPERATOR_BOOLEAN, OPERATOR_DEFEATURE, OPERATOR_UNKNOWN, OPERATOR_SPERATOR = 999 };
enum DEFEATURE_TYPES { DEFEATURE_UNREPEAT = 0, DEFEATURE_REMOVE_SMALL, DEFEATURE_SHELL, DEFEATURE_SPLIT, DEFEATURE_ENVELOPE, DEFEATURE_BREP2CSG };
enum INPUT_MODES { SELECTION, INPUT_SPLINE };
// 注意：BASIC_ENTITIES中非样条基本体需放在BSPLINE_CTRLPTS之前
// 否则将导致进行变换时未修改ENTITY_TREE_ITEM的trans。
enum BASIC_ENTITIES {
    E_CUBE = 0,         // 立方体
    E_SPHERE,           // 球体
    E_CYLINDER,         // 圆柱体
    E_TORUS,            // 圆环体
    E_CONE,             // 圆锥体
    E_PRISM,            // 多棱锥
    E_PYRAMID,          // 金字塔
    E_WIGGLE,           // 摆动体
    E_PLANE,            // 矩形面
    E_PPLANE,           // 平行四边形面
    E_CPLANE,           // 圆形面
    E_SPHERES,          // 球面
    E_PSPHERES,         // 部分球面
    E_CONIC_SIDE,       // 圆锥面
    E_CYLINDER_SIDE,    // 圆柱面
    E_PCONIC,           // 部分圆锥面
    E_TORUSS,           // 完整圆环面
    E_PTORUSS,          // 部分圆环面
    E_LAWS,             // 公式面
    E_LINE,             // 直线
    E_ARC,              // 圆弧
    E_ELLIPSE,          // 椭圆
    E_CONIC,            // 二次线
    E_HELIX,            // 螺旋线
    E_SPIRAL,           // 阿基米德螺旋线
    E_SPRING,           // 弹簧线
    E_LAWC,             // 方程线
    E_BSPLINE_CTRLPTS,  // 控制点b样条
    E_BSPLINE_FIT,      // 拟合b样条
    E_BSPLINE_INTRP,    // 插值b样条
    E_BEZIER,           // 贝塞尔
    E_BSPLINE           // b样条
};

class ENTITY_LIST;
class AcisOptions;
class outcome;
class Window;
class HISTORY_STREAM;
class DELTA_STATE;
class BULLETIN_BOARD;

typedef outcome(*GME_fp)(ENTITY_LIST&, AcisOptions*);

struct menu_map {
    std::string name;
    int value;
};

extern std::unordered_map<std::string, std::vector<menu_map>> menus_example;

const QStringList colorNames = QColor::colorNames();

// 备用：默认提供12种颜色用于显示区分不同的输入体
// double colors[12][3] = {
//  {166 / 255.0, 206 / 255.0, 227 / 255.0},
//  {31 / 255.0,  120 / 255.0, 180 / 255.0},
//  {178 / 255.0, 223 / 255.0, 138 / 255.0},
//  {51 / 255.0,  160 / 255.0, 44 / 255.0 },
//  {251 / 255.0, 154 / 255.0, 153 / 255.0},
//  {227 / 255.0, 26 / 255.0,  28 / 255.0 },
//  {253 / 255.0, 191 / 255.0, 111 / 255.0},
//  {255 / 255.0, 127 / 255.0, 0 / 255.0  },
//  {202 / 255.0, 178 / 255.0, 214 / 255.0},
//  {106 / 255.0, 61 / 255.0,  154 / 255.0},
//  {255 / 255.0, 255 / 255.0, 153 / 255.0},
//  {177 / 255.0, 89 / 255.0,  40 / 255.0 }
//};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr, Qt::WindowFlags flags = {});
    void setCurWindow(Window* w) { curWindow = w; }
    void showMessage(const QString s, int duration = -1);

    void buildTreeFromHistroy(HISTORY_STREAM* hs = nullptr);

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
    void runTest();
    void incrementalTest();
    void setNEO4JConnectInfo();
    void setFastAPIConnectInfo();
    void visiablility();
    void displayInfo();
#ifndef QT_NO_SESSIONMANAGER
    void commitData(QSessionManager&);
#endif
    void insertElements(const OPERATOR_TYPES ot, const int subOperatorType);
    void operation(const OPERATOR_TYPES ot, const int subOperatorType);
    void clear();

private:
    void createActions();
    void createStatusBar();
    bool maybeSave();
    bool saveFile(const QString& fileName);
    void setCurrentFile(const QString& fileName);
    void setCurrentPartName(const QString& partName);

    Window* curWindow;
    QAction* setACISModeAct;
    QAction* setNEO4JModeAct;
    QAction* setNEO4JIncrementalModeAct;
    QAction* setMEMGRAPHModeAct;
    QAction* setFASTAPIModeAct;
    QActionGroup* setModeActGroup;
    std::string neo4jdb_host;
    int neo4jdb_port_bolt;
    std::string neo4jdb_username;
    std::string neo4jdb_password;
    std::string fastapi_base_url;
    std::string fastapi_author;
    QString fastapi_project_id;
    int fastapi_model_version = 0;
};
