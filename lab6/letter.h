#ifndef LETTER_H
#define LETTER_H

#include <QMainWindow >

#include <QtOpenGL>
#include <QOpenGLFunctions>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QGLWidget>


class letter : public QGLWidget
{
public:
    GLfloat xRot;
    GLfloat yRot;
    GLfloat zRot;
    GLfloat zTra;
    GLfloat nSca;
    int check=0;
    float angle =30;
    float x_transfer=1,y_transfer=1,z_transfer=1,x_scale=1,y_scale=2,z_scale=3;
    QColor colour;

    QPoint ptrMousePosition;
    void scale_plus();
    void scale_minus();
    void rotate_up();
    void rotate_down();
    void rotate_left();
    void rotate_right();
    void translate_down();
    void translate_up();
    void defaultScene();

    void drawAxis();

    void drawFigure();
    void getVertexArray(float k);
    void getIndexArray();
    void draw_xy_projection();
    void draw_xz_projection();
    void draw_zy_projection();
    void draw_scaling();
    void draw_transfer();
    void draw_rotation_x();
    void draw_rotation_y();
    void draw_rotation_z();
    void initializeGL();
    void resizeGL(int nWidth, int nHeight);
    void paintGL();

    void mousePressEvent(QMouseEvent* pe);
    void mouseMoveEvent(QMouseEvent* pe);
    void wheelEvent(QWheelEvent* pe);
    void keyPressEvent(QKeyEvent* pe);

public:
    letter(QWidget *parent = nullptr);
    virtual ~letter() = default;
};


#endif // LETTER_H
