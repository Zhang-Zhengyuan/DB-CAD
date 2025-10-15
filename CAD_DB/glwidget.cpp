#include "glwidget.h"

#include <math.h>

#include <QCoreApplication>
#include <QMouseEvent>

#include "acis/include/box.hxx"
#include "acis/include/edge.hxx"
#include "acis/include/face.hxx"
#include "acis/include/kernapi.hxx"
#include "acis/include/point.hxx"
#include "acis/include/vertex.hxx"
#include "window.h"

static const char* vshader_face =
"attribute vec4 vertex;\n"
"attribute vec3 normal;\n"
"varying vec3 vert;\n"
"varying vec3 vertNormal;\n"
"uniform mat4 projMatrix;\n"
"uniform mat4 mvMatrix;\n"
"uniform mat3 normalMatrix;\n"
"void main() {\n"
"   vert = vertex.xyz;\n"
"   vertNormal = normalMatrix * normal;\n"
"   gl_Position = projMatrix * mvMatrix * vertex;\n"
"}\n";

static const char* vshader_edge =
"attribute vec4 vertex;\n"
"varying vec3 vert;\n"
"uniform mat4 projMatrix;\n"
"uniform mat4 mvMatrix;\n"
"void main() {\n"
"   vert = vertex.xyz;\n"
"   gl_Position = projMatrix * mvMatrix * vertex;\n"
"}\n";

static const char* fshader_face =
"varying highp vec3 vert;\n"
"varying highp vec3 vertNormal;\n"
"uniform highp vec3 lightPos;\n"
"uniform highp vec4 vertColor;\n"
"void main() {\n"
"   highp vec3 L = normalize(lightPos - vert);\n"
"   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
"   highp vec4 color = vertColor;\n"
"   highp vec4 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
"   col.w = color.w;\n"
"   gl_FragColor = col;\n"
"}\n";

static const char* fshader_edge =
"uniform highp vec4 vertColor;\n"
"void main() {\n"
"   gl_FragColor = vertColor;\n"
"}\n";

bool GLWidget::transparent = false;

GLWidget::GLWidget(Window* p, int w, int h) : QOpenGLWidget(p), vbo_face(QOpenGLBuffer::VertexBuffer), vbo_edge(QOpenGLBuffer::VertexBuffer) {
    parent = p;
    width = w;
    height = h;
    if (transparent) {
        QSurfaceFormat fmt = format();
        fmt.setAlphaBufferSize(8);
        setFormat(fmt);
    }
}

GLWidget::~GLWidget() {
    cleanup();
}

void GLWidget::clear() {
    updateLP_local = false;
    xRot = 0;
    yRot = 0;
    zRot = 0;
    xTrans = 0;
    yTrans = 0;
    zTrans = 0;
    posZ = 0.0;
    updateMove_local = false;
    scale = 1.0;
    ptrPickedEntity = nullptr;
    isPickedHandle = false;
    addPickedHandle = false;
    updateMove_handle = false;
    display_data.clear();
    radius = 0.0;
    vao_face.release();
    vbo_face.release();
    data_face.clear();
    count_face = 0;
    vao_edge.release();
    vbo_edge.release();
    data_edge.clear();
    count_edge = 0;
    update();
}

QSize GLWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const {
    return QSize(width, height);
}

static void qNormalizeAngle(int& angle) {
    while (angle < 0) angle += 360 * 16;
    while (angle > 360 * 16) angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle) {
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        update();
    }
}

void GLWidget::setYRotation(int angle) {
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        update();
    }
}

void GLWidget::setZRotation(int angle) {
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        update();
    }
}

void GLWidget::cleanup() {
    if (program_face == nullptr && program_edge == nullptr) return;
    makeCurrent();
    if (program_face) {
        delete program_face;
        program_face = nullptr;
    }
    if (program_edge) {
        delete program_edge;
        program_edge = nullptr;
    }

    vbo_face.destroy();
    vao_face.destroy();
    vbo_edge.destroy();
    vao_edge.destroy();

    doneCurrent();
    QObject::disconnect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(1, 1, 1, transparent ? 0 : 1);

    initializeFace();
    initializeEdge();

    initializeShadersFace();
    initializeShadersEdge();

    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);
}

void GLWidget::initializeFace() {
    vao_face.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vao_face);

    vbo_face.create();
    vbo_face.bind();
    vbo_face.allocate(data_face.constData(), count_face * sizeof(GLfloat));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));
}

void GLWidget::initializeEdge() {
    vao_edge.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vao_edge);

    vbo_edge.create();
    vbo_edge.bind();
    vbo_edge.allocate(data_edge.constData(), count_edge * sizeof(GLfloat));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
}

void GLWidget::initializeShadersFace() {
    program_face = new QOpenGLShaderProgram;
    program_face->addShaderFromSourceCode(QOpenGLShader::Vertex, vshader_face);
    program_face->addShaderFromSourceCode(QOpenGLShader::Fragment, fshader_face);
    program_face->bindAttributeLocation("vertex", 0);
    program_face->bindAttributeLocation("normal", 1);
    program_face->link();
}

void GLWidget::initializeShadersEdge() {
    program_edge = new QOpenGLShaderProgram;
    program_edge->addShaderFromSourceCode(QOpenGLShader::Vertex, vshader_edge);
    program_edge->addShaderFromSourceCode(QOpenGLShader::Fragment, fshader_edge);
    program_edge->bindAttributeLocation("vertex", 0);
    program_edge->link();
}

void GLWidget::updateMeshData() {
    // 将面网格数据写入VBO
    data_face.clear();
    count_face = 0;
    for (auto dd : display_data) {
        for (unsigned int i = 0; i < dd->faceMesh.size(); i++) {
            unsigned int nT = (unsigned int)(dd->faceMesh[i].numIndices / 3);
            for (unsigned int j = 0; j < nT; j++) {
                int jj = 3 * j + dd->faceMesh[i].baseIndex;
                for (int k = 0; k < 3; k++) {
                    int kk = 3 * dd->triangles[jj + k];
                    for (int l = 0; l < 3; l++) {
                        data_face.push_back(dd->faceCoords[kk + l]);
                    }
                    for (int l = 0; l < 3; l++) {
                        data_face.push_back(dd->normalCoords[kk + l]);
                    }
                    count_face += 6;
                }
            }
        }
    }
    vbo_face.bind();
    vbo_face.allocate(data_face.constData(), count_face * sizeof(GLfloat));

    // 将边数据写入VBO
    data_edge.clear();
    count_edge = 0;
    for (auto dd : display_data) {
        for (unsigned int i = 0; i < dd->edgeMesh.size(); i++) {
            unsigned int nI = (unsigned int)(dd->edgeMesh[i].numIndices / 3);
            for (unsigned int j = 0; j < nI; j++) {
                int jj = 3 * j + dd->edgeMesh[i].baseVertex;
                for (int k = 0; k < 3; k++) {
                    data_edge.push_back(dd->edgeCoords[jj + k]);
                }
                count_edge += 3;
            }
        }
    }
    vbo_edge.bind();
    vbo_edge.allocate(data_edge.constData(), count_edge * sizeof(GLfloat));

    // 计算包围球
    if (parent->getInputMode() == INPUT_MODES::SELECTION) {
        radius = 0.0;
        for (auto dd : display_data) {
            for (size_t ii = 0; ii < dd->faceCoords.size(); ii++) {
                if (fabs(dd->faceCoords[ii]) > radius) {
                    radius = fabs(dd->faceCoords[ii]);
                }
            }
            for (size_t ii = 0; ii < dd->edgeCoords.size(); ii++) {
                if (fabs(dd->edgeCoords[ii]) > radius) {
                    radius = fabs(dd->edgeCoords[ii]);
                }
            }
            for (size_t ii = 0; ii < dd->vertexCoords.size(); ii++) {
                if (fabs(dd->vertexCoords[ii]) > radius) {
                    radius = fabs(dd->vertexCoords[ii]);
                }
            }
        }
        radius *= 1.5;
    }

    update();
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LEQUAL);

    world.setToIdentity();
    // Qt Opengl世界坐标系Y轴向下，这里修改为向上
    world.rotate(180.0f, 1, 0, 0);
    world.rotate(180.0f + (xRot / 16.0f), 1, 0, 0);
    world.rotate(yRot / 16.0f, 0, 1, 0);
    world.rotate(zRot / 16.0f, 0, 0, 1);
    world.scale(scale);

    projection.setToIdentity();
    projection.viewport(0, 0, width, height);
    projection.setToIdentity();
    double r = radius;
    if (r == 0.0) r = 1.0;
    GLdouble ratio = width / (float)height;
    projection.ortho(-ratio * r, ratio * r, -r, r, -2 * r, 2 * r);

    // 绘制面
    {
        program_face->bind();
        projMatrixLoc = program_face->uniformLocation("projMatrix");
        mvMatrixLoc = program_face->uniformLocation("mvMatrix");
        normalMatrixLoc = program_face->uniformLocation("normalMatrix");
        lightPosLoc = program_face->uniformLocation("lightPos");
        colorLoc = program_face->uniformLocation("vertColor");
        program_face->setUniformValue(projMatrixLoc, projection);
        program_face->setUniformValue(mvMatrixLoc, camera * world);
        QMatrix3x3 normalMatrix = world.normalMatrix();
        program_face->setUniformValue(normalMatrixLoc, normalMatrix);
        program_face->setUniformValue(lightPosLoc, QVector3D(0, 0, 70));
        QOpenGLVertexArrayObject::Binder vaoBinder(&vao_face);
        GLint dd_offset = 0;
        for (auto dd : display_data) {
            if (dd->displayType == DISPLAY_TYPES::DISPLAY_FACE || dd->displayType == DISPLAY_TYPES::DISPLAY_ALL) {
                for (unsigned int i = 0; i < dd->faceMesh.size(); i++) {
                    QColor color;
                    ENTITY* ptrEntity = nullptr;
                    api_get_owner(dd->faceMesh[i].ptrFace, ptrEntity);
                    color.setRedF(ptrPickedEntity == ptrEntity ? 1.0 : dd->faceMesh[i].color.red());
                    color.setGreenF(ptrPickedEntity == ptrEntity ? 0.0 : dd->faceMesh[i].color.green());
                    color.setBlueF(ptrPickedEntity == ptrEntity ? 0.0 : dd->faceMesh[i].color.blue());
                    color.setAlphaF(surface_alpha);
                    program_face->setUniformValue(colorLoc, color);
                    glDrawArrays(GL_TRIANGLES, dd_offset + dd->faceMesh[i].baseIndex, dd->faceMesh[i].numIndices);
                }
            }
            dd_offset += dd->triangles.size();
        }
        program_face->release();
    }

    // 绘制边
    {
        program_edge->bind();
        projMatrixLoc = program_edge->uniformLocation("projMatrix");
        mvMatrixLoc = program_edge->uniformLocation("mvMatrix");
        colorLoc = program_edge->uniformLocation("vertColor");
        program_edge->setUniformValue(projMatrixLoc, projection);
        program_edge->setUniformValue(mvMatrixLoc, camera * world);
        QMatrix3x3 normalMatrix = world.normalMatrix();
        program_edge->setUniformValue(lightPosLoc, QVector3D(0, 0, 70));
        QOpenGLVertexArrayObject::Binder vaoBinder(&vao_edge);
        GLint dd_offset = 0;
        for (auto dd : display_data) {
            if (dd->displayType == DISPLAY_TYPES::DISPLAY_EDGE || dd->displayType == DISPLAY_TYPES::DISPLAY_ALL) {
                for (unsigned int i = 0; i < dd->edgeMesh.size(); i++) {
                    QColor color;
                    ENTITY* ptrEntity = nullptr;
                    api_get_owner(dd->edgeMesh[i].ptrEdge, ptrEntity);
                    color.setRedF(ptrPickedEntity == ptrEntity ? 1.0 : dd->edgeMesh[i].color.red());
                    color.setGreenF(ptrPickedEntity == ptrEntity ? 0.0 : dd->edgeMesh[i].color.green());
                    color.setBlueF(ptrPickedEntity == ptrEntity ? 0.0 : dd->edgeMesh[i].color.blue());
                    color.setAlphaF(1.0);
                    program_edge->setUniformValue(colorLoc, color);
                    glLineWidth(ptrPickedEntity == ptrEntity ? line_width * 1.5 : line_width);
                    glDrawArrays(GL_LINE_STRIP, dd_offset + dd->edgeMesh[i].baseVertex / 3, dd->edgeMesh[i].numIndices / 3);
                }
            }
            dd_offset += dd->edgeCoords.size() / 3;
        }
        program_edge->release();
    }

    // 绘制选中体的控制节点与边
    {
        program_edge->bind();
        projMatrixLoc = program_edge->uniformLocation("projMatrix");
        mvMatrixLoc = program_edge->uniformLocation("mvMatrix");
        colorLoc = program_edge->uniformLocation("vertColor");
        program_edge->setUniformValue(projMatrixLoc, projection);
        program_edge->setUniformValue(mvMatrixLoc, camera * world);
        QMatrix3x3 normalMatrix = world.normalMatrix();
        program_edge->setUniformValue(lightPosLoc, QVector3D(0, 0, 70));
        QOpenGLVertexArrayObject::Binder vaoBinder(&vao_edge);
        GLint dd_offset = 0;
        if (ptrPickedEntity && parent->getVisibility(ptrPickedEntity)) {
            std::vector<std::vector<SPAposition>> handles;
            parent->getHandles(ptrPickedEntity, handles);
            glPointSize(point_size);
            glColor4f(1, 0, 0, 1);
            glBegin(GL_POINTS);
            for (auto r : handles) {
                for (auto h : r) {
                    GLfloat p[3] = { h.x(), h.y(), h.z() };
                    glVertex3fv(p);
                }
            }
            glEnd();
            glLineWidth(line_width);
            glColor4f(1, 0, 0, 1);
            glBegin(GL_LINES);
            for (int i = 0; i < handles.size(); i++) {
                GLfloat p1[3] = { handles[i][0].x(), handles[i][0].y(), handles[i][0].z() };
                glVertex3fv(p1);
                for (int j = 1; j < handles[i].size() - 1; j++) {
                    GLfloat p2[3] = { handles[i][j].x(), handles[i][j].y(), handles[i][j].z() };
                    glVertex3fv(p2);
                    glVertex3fv(p2);
                }
                GLfloat p3[3] = { handles[i][handles[i].size() - 1].x(), handles[i][handles[i].size() - 1].y(), handles[i][handles[i].size() - 1].z() };
                glVertex3fv(p3);
            }
            if (handles.size() > 1) {
                // 假设：控制点组成M*N的标准网格
                for (int i = 0; i < handles[0].size(); i++) {
                    GLfloat p1[3] = { handles[0][i].x(), handles[0][i].y(), handles[0][i].z() };
                    glVertex3fv(p1);
                    for (int j = 1; j < handles.size() - 1; j++) {
                        GLfloat p2[3] = { handles[j][i].x(), handles[j][i].y(), handles[j][i].z() };
                        glVertex3fv(p2);
                        glVertex3fv(p2);
                    }
                    GLfloat p3[3] = { handles[handles.size() - 1][i].x(), handles[handles.size() - 1][i].y(), handles[handles.size() - 1][i].z() };
                    glVertex3fv(p3);
                }
            }
            glEnd();
        }
        program_edge->release();
    }

    // 绘制顶点
    {
        program_edge->bind();
        projMatrixLoc = program_edge->uniformLocation("projMatrix");
        mvMatrixLoc = program_edge->uniformLocation("mvMatrix");
        colorLoc = program_edge->uniformLocation("vertColor");
        program_edge->setUniformValue(projMatrixLoc, projection);
        program_edge->setUniformValue(mvMatrixLoc, camera * world);
        QMatrix3x3 normalMatrix = world.normalMatrix();
        program_edge->setUniformValue(lightPosLoc, QVector3D(0, 0, 70));
        QOpenGLVertexArrayObject::Binder vaoBinder(&vao_edge);
        for (auto dd : display_data) {
            for (unsigned int i = 0; i < dd->vertexMesh.size(); i++) {
                QColor color;
                color.setRedF(ptrPickedEntity == (ENTITY*)dd->vertexMesh[i].ptrVertex ? 1.0 : dd->vertexMesh[i].color.red());
                color.setGreenF(ptrPickedEntity == (ENTITY*)dd->vertexMesh[i].ptrVertex ? 0.0 : dd->vertexMesh[i].color.green());
                color.setBlueF(ptrPickedEntity == (ENTITY*)dd->vertexMesh[i].ptrVertex ? 0.0 : dd->vertexMesh[i].color.blue());
                color.setAlphaF(1.0);
                program_edge->setUniformValue(colorLoc, color);
                glPointSize(point_size);
                glColor4f(1, 0, 0, 1);
                glBegin(GL_POINTS);
                GLfloat p[3] = { dd->vertexCoords[i * 3], dd->vertexCoords[i * 3 + 1], dd->vertexCoords[i * 3 + 2] };
                glVertex3fv(p);
                glEnd();
            }
        }
        program_edge->release();
    }

    // 更新拾取坐标
    if (updateLP_local || parent->getInputMode() != INPUT_MODES::SELECTION) {
        glReadPixels((int)lastPosition_screen.x(), height - (int)lastPosition_screen.y(), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &posZ);
        if (addPickedHandle && posZ == 1.0) posZ = 0.5;
        QVector3D worldPosition = QVector3D((int)lastPosition_screen.x(), height - (int)lastPosition_screen.y(), posZ).unproject(camera, projection, QRect(0, 0, width, height));
        lastPostion_local = worldPosition * world;
        lastPostion_local /= scale * scale;
        parent->showMessage(QString(tr("拾取位置：[%1, %2, %3]")).arg(lastPostion_local.x()).arg(lastPostion_local.y()).arg(lastPostion_local.z()));
        if (updateLP_local) {
            getSelectedEntites();
            updateLP_local = false;
        } else if (addPickedHandle) {
            double p[3] = { lastPostion_local.x(), lastPostion_local.y(), lastPostion_local.z() };
            parent->addHandle(ptrPickedEntity, p);
            addPickedHandle = false;
            update();
        }
    }
    // 修改ENTITY
    if (updateMove_local || updateMove_handle) {
        QVector3D wp1 = QVector3D((int)prePosition_screen.x(), height - (int)prePosition_screen.y(), posZ).unproject(camera, projection, QRect(0, 0, width, height));
        QVector3D lp1 = wp1 * world;
        lp1 /= scale * scale;
        QVector3D wp2 = QVector3D((int)lastPosition_screen.x(), height - (int)lastPosition_screen.y(), posZ).unproject(camera, projection, QRect(0, 0, width, height));
        QVector3D lp2 = wp2 * world;
        lp2 /= scale * scale;
        lp2 -= lp1;
        if (updateMove_local) {
            parent->changeEntity(ptrPickedEntity, lp2);
            updateMove_local = false;
        }
        if (updateMove_handle) {
            parent->changeHandle(ptrPickedEntity, pickedHandle, lp2);
            updateMove_handle = false;
        }
    }
}

void GLWidget::resizeGL(int w, int h) {
    width = w;
    height = h;
}

void GLWidget::mousePressEvent(QMouseEvent* event) {
    lastPosition_screen = event->position().toPoint();
    if (parent->getInputMode() == INPUT_MODES::SELECTION) {
        updateLP_local = true;
    } else {
        addPickedHandle = true;
    }
    update();
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    lastPosition_screen = event->position().toPoint();
    if (parent->getInputMode() == INPUT_MODES::SELECTION) {
        ptrPickedEntity = nullptr;
    } else {
        addPickedHandle = true;
    }
    update();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event) {
    int dx = event->position().toPoint().x() - lastPosition_screen.x();
    int dy = event->position().toPoint().y() - lastPosition_screen.y();
    prePosition_screen = lastPosition_screen;
    lastPosition_screen = event->position().toPoint();

    if (event->buttons() & Qt::LeftButton) {
        if (isPickedHandle) {
            updateMove_handle = true;
            update();
        } else {
            setXRotation(xRot + 8 * dy);
            setYRotation(yRot + 8 * dx);
        }
    } else if (event->buttons() & Qt::RightButton) {
        if (ptrPickedEntity) {
            updateMove_local = true;
            update();
        } else {
            setXRotation(xRot + 8 * dy);
            setZRotation(zRot + 8 * dx);
        }
    }
}

void GLWidget::wheelEvent(QWheelEvent* event) {
    const bool vertical = qAbs(event->angleDelta().y()) >= qAbs(event->angleDelta().x());
    const int delta = vertical ? event->angleDelta().y() : event->angleDelta().x();
    if (delta > 0) {
        if (vertical) {
            scale += 0.02;
        } else {
        }
    } else if (delta < 0) {
        if (vertical) {
            scale -= 0.02;
        } else {
        }
    }
    update();
}

void GLWidget::getSelectedEntites() {
    // 选择控制点
    SPAposition lp = SPAposition(lastPostion_local.x(), lastPostion_local.y(), lastPostion_local.z());
    if (ptrPickedEntity) {
        std::vector<std::vector<SPAposition>> pos_vector;
        parent->getHandles(ptrPickedEntity, pos_vector);
        for (auto& hp : pos_vector) {
            for (auto& p : hp) {
                if (same_point(p, lp, 0.2)) {
                    pickedHandle = p;
                    isPickedHandle = true;
                    update();
                    return;
                }
            }
        }
    }
    isPickedHandle = false;

    // 选择顶点
    for (auto dd : display_data) {
        for (unsigned int i = 0; i < dd->vertexMesh.size(); i++) {
            if (same_point(dd->vertexMesh[i].ptrVertex->geometry()->coords(), lp, 0.2)) {
                if (ptrPickedEntity == dd->vertexMesh[i].ptrVertex) return;
                ptrPickedEntity = dd->vertexMesh[i].ptrVertex;
                update();
                return;
            }
        }
    }

    // 选择边
    for (auto dd : display_data) {
        for (unsigned int i = 0; i < dd->edgeMesh.size(); i++) {
            if (dd->edgeMesh[i].ptrEdge == nullptr) continue;
            SPAbox* ptrBox = dd->edgeMesh[i].ptrEdge->bound();
            bool inside = true;
            if (ptrBox) {
                if (ptrBox->x_range().finite_above() && ptrBox->x_range().end_pt() < lastPostion_local.x()) inside = false;
                if (ptrBox->x_range().finite_below() && ptrBox->x_range().start_pt() > lastPostion_local.x()) inside = false;
                if (ptrBox->y_range().finite_above() && ptrBox->y_range().end_pt() < lastPostion_local.y()) inside = false;
                if (ptrBox->y_range().finite_below() && ptrBox->y_range().start_pt() > lastPostion_local.y()) inside = false;
                if (ptrBox->z_range().finite_above() && ptrBox->z_range().end_pt() < lastPostion_local.z()) inside = false;
                if (ptrBox->z_range().finite_below() && ptrBox->z_range().start_pt() > lastPostion_local.z()) inside = false;
            }
            if (!inside) continue;

            unsigned int nI = (unsigned int)(dd->edgeMesh[i].numIndices / 3);
            if (nI > 0)
                for (unsigned int j = 0; j < nI - 1; j++) {
                    int jj = 3 * j + dd->edgeMesh[i].baseVertex;
                    if (dd->edgeCoords.size() <= jj) break;
                    QVector3D t[2];
                    for (int k = 0; k < 2; k++) {
                        t[k].setX(dd->edgeCoords[jj + k * 3]);
                        t[k].setY(dd->edgeCoords[jj + k * 3 + 1]);
                        t[k].setZ(dd->edgeCoords[jj + k * 3 + 2]);
                    }
                    if (pointInEdge(lastPostion_local, t[0], t[1])) {
                        ENTITY* ptrEntity = nullptr;
                        api_get_owner(dd->edgeMesh[i].ptrEdge, ptrEntity);
                        if (ptrPickedEntity == ptrEntity) return;
                        ptrPickedEntity = ptrEntity;
                        update();
                        return;
                    }
                }
        }
    }
    // 选择面
    for (auto dd : display_data) {
        for (unsigned int i = 0; i < dd->faceMesh.size(); i++) {
            if (dd->faceMesh[i].ptrFace == nullptr) continue;
            SPAbox* ptrBox = dd->faceMesh[i].ptrFace->bound();
            bool inside = true;
            if (ptrBox) {
                if (ptrBox->x_range().finite_above() && ptrBox->x_range().end_pt() < lastPostion_local.x()) inside = false;
                if (ptrBox->x_range().finite_below() && ptrBox->x_range().start_pt() > lastPostion_local.x()) inside = false;
                if (ptrBox->y_range().finite_above() && ptrBox->y_range().end_pt() < lastPostion_local.y()) inside = false;
                if (ptrBox->y_range().finite_below() && ptrBox->y_range().start_pt() > lastPostion_local.y()) inside = false;
                if (ptrBox->z_range().finite_above() && ptrBox->z_range().end_pt() < lastPostion_local.z()) inside = false;
                if (ptrBox->z_range().finite_below() && ptrBox->z_range().start_pt() > lastPostion_local.z()) inside = false;
            }
            if (!inside) continue;

            unsigned int nT = (unsigned int)(dd->faceMesh[i].numIndices / 3);
            if (nT > 0)
                for (unsigned int j = 0; j < nT; j++) {
                    int jj = 3 * j + dd->faceMesh[i].baseIndex;
                    QVector3D t[3];
                    for (int k = 0; k < 3; k++) {
                        int kk = 3 * dd->triangles[jj + k];
                        t[k].setX(dd->faceCoords[kk]);
                        t[k].setY(dd->faceCoords[kk + 1]);
                        t[k].setZ(dd->faceCoords[kk + 2]);
                    }
                    if (pointInTriangle(lastPostion_local, t[0], t[1], t[2])) {
                        ENTITY* ptrEntity = nullptr;
                        api_get_owner(dd->faceMesh[i].ptrFace, ptrEntity);
                        if (ptrPickedEntity == ptrEntity) return;
                        ptrPickedEntity = ptrEntity;
                        update();
                        return;
                    }
                }
        }
    }
    update();
}

bool GLWidget::pointInTriangle(QVector3D p, QVector3D tv1, QVector3D tv2, QVector3D tv3) {
    QVector3D v1 = tv1 - p;
    QVector3D v2 = tv2 - p;
    QVector3D v3 = tv3 - p;

    QVector3D u = QVector3D::crossProduct(v2, v3);
    QVector3D v = QVector3D::crossProduct(v3, v1);
    QVector3D w = QVector3D::crossProduct(v1, v2);

    if (QVector3D::dotProduct(u, v) < 0.0f) {
        return false;
    }
    if (QVector3D::dotProduct(u, w) < 0.0f) {
        return false;
    }
    return true;
}

bool GLWidget::pointInEdge(QVector3D p, QVector3D ev1, QVector3D ev2) {
    double len0 = QVector3D(ev2 - ev1).length();
    double len1 = QVector3D(p - ev1).length();
    double len2 = QVector3D(ev2 - p).length();
    if (len1 < 1e-6 || len2 < 1e-6) return true;
    if (len0 < 1e-6) return false;
    double tolerance = 1e-2 / len0;
    if (tolerance > 1e-1)
        tolerance = 1e-1;
    else if (tolerance < 1e-3)
        tolerance = 1e-3;
    if (std::abs((len1 + len2) / len0 - 1.0) < tolerance) return true;
    return false;
}