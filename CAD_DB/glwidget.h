#pragma once

#include <QMatrix4x4>
#include <QtOpenGL/QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QtOpenGL/QOpenGLShaderProgram>
#include <QtOpenGL/QOpenGLVertexArrayObject>
#include <QtOpenGLWidgets/QOpenGLWidget>

#include "acis/include/lists.hxx"
#include "acis/include/position.hxx"
#include "gme_mesh.hxx"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class Window;

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    struct ViewState {
        int xRot = 0;
        int yRot = 0;
        int zRot = 0;
        double scale = 1.0;
    };

    GLWidget(Window* p = nullptr, int w = 800, int h = 600);
    ~GLWidget();

    void clear();
    ViewState viewState() const;
    void setViewState(const ViewState& state);

    std::vector<GmeMesh::DisplayData*>& getMeshData() { return display_data; }
    void updateMeshData();

    static bool getTransparent() { return transparent; }
    static void setTransparent(bool t) { transparent = t; }

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void setPickedEntity(ENTITY* ptrEntity) { ptrPickedEntity = ptrEntity; }
    void setPickedHandle(SPAposition p) { pickedHandle = p; }
    void setSurfaceAlpha(double sa) {
        surface_alpha = sa;
        update();
    }
    void setLineWidth(int lw) {
        line_width = lw;
        update();
    }
    void setPointSize(int ps) {
        point_size = ps;
        update();
    }

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void cleanup();

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void initializeFace();
    void initializeEdge();
    void initializeShadersFace();
    void initializeShadersEdge();
    void uploadMeshDataToGpu();
    void getSelectedEntites();
    bool pointInTriangle(QVector3D p, QVector3D tv1, QVector3D tv2, QVector3D tv3);
    bool pointInEdge(QVector3D p, QVector3D ev1, QVector3D ev2);

    static bool transparent;
    double surface_alpha = 1.0;
    int line_width = 2;
    int point_size = 10;

    int width;
    int height;

    QPoint prePosition_screen;
    QPoint lastPosition_screen;
    QVector3D lastPostion_local;
    bool updateLP_local = false;
    int xRot = 0;
    int yRot = 0;
    int zRot = 0;
    int xTrans = 0;
    int yTrans = 0;
    int zTrans = 0;
    GLfloat posZ = 0.0;
    bool updateMove_local = false;
    double scale = 1.0;
    ENTITY* ptrPickedEntity = nullptr;
    SPAposition pickedHandle;
    bool isPickedHandle = false;
    bool addPickedHandle = false;
    bool updateMove_handle = false;

    std::vector<GmeMesh::DisplayData*> display_data;
    double radius = 0.0;
    bool pendingGpuUpload = false;

    QOpenGLVertexArrayObject vao_face;
    QOpenGLBuffer vbo_face;
    QList<GLfloat> data_face;
    int count_face = 0;

    QOpenGLVertexArrayObject vao_edge;
    QOpenGLBuffer vbo_edge;
    QList<GLfloat> data_edge;
    int count_edge = 0;

    QOpenGLShaderProgram* program_face = nullptr;
    QOpenGLShaderProgram* program_edge = nullptr;

    int projMatrixLoc = 0;
    int mvMatrixLoc = 0;
    int normalMatrixLoc = 0;
    int lightPosLoc = 0;
    int colorLoc = 0;
    QMatrix4x4 projection;
    QMatrix4x4 camera;
    QMatrix4x4 world;

    Window* parent;
};
