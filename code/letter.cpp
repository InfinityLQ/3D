#include "letter.h"
#include "ui_mainwindow.h"
#include <QDebug>
GLfloat VertexArray[52][3];
GLubyte IndexArray[13][4];
letter::letter(QWidget *parent)
    : QGLWidget(parent)
{
    setGeometry(400,200,800,600);
    xRot=0; yRot=0; zRot=0; zTra=-1; nSca=1;
}

void letter::initializeGL()
{
    qglClearColor(Qt::white);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    glEnableClientState(GL_VERTEX_ARRAY);
}

void letter::resizeGL(int nWidth, int nHeight)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat ratio=(GLfloat)nHeight/(GLfloat)nWidth;

    if (nWidth>=nHeight)
        glOrtho(-1.0/ratio, 1.0/ratio, -1.0, 1.0, -5.0, 5.0);
    else
        glOrtho(-1.0, 1.0, -1.0*ratio, 1.0*ratio, -5.0, 5.0);
    glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
}

void letter::paintGL()
{
    glClearColor(1,1,1,0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(nSca, nSca, nSca);
    glTranslatef(0.0f, zTra, 0.0f);
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot, 0.0f, 0.0f, 1.0f);
    drawAxis();
    double e=0,r=0,t=0;
    colour.getRgbF(&e,&r,&t,nullptr);
    glColor4f(e, r, t, 0.0f);
    getVertexArray(1);
    getIndexArray();
    if(check==1)
        draw_xy_projection();
    else if(check==2)
        draw_xz_projection();
    else if(check==3)
        draw_zy_projection();
    else if(check==4)
        draw_scaling();
    else if(check==5)
        draw_transfer();
    else if(check==6)
        draw_rotation_x();
    else if(check==7)
        draw_rotation_y();
    else if(check==8)
        draw_rotation_z();
    else if(check==9)
        drawFigure();
    QFont tmpfont;
    tmpfont.setFamily("Arial Black");
    tmpfont.setPointSize(10);
    tmpfont.setBold(false);
    glColor3f(0,0,0);
    renderText(10,0.0,0.0,"X",tmpfont);
    glColor3f(0,0,0);
    renderText(0.0,10,0.0,"Y",tmpfont);
    glColor3f(0,0,0);
    renderText(0.0,0.0,10,"Z",tmpfont);

}

void letter::mousePressEvent(QMouseEvent* pe)
{
    ptrMousePosition = pe->pos();
}
void letter::mouseMoveEvent(QMouseEvent* pe)
{
    xRot += 1/M_PI*(GLfloat)(pe->y()-ptrMousePosition.y());
    yRot += 1/M_PI*(GLfloat)(pe->x()-ptrMousePosition.x());
    ptrMousePosition = pe->pos();
    updateGL();
}

/*virtual*/void letter::wheelEvent(QWheelEvent* pe)
{
    if ((pe->delta())>0) scale_plus(); else if ((pe->delta())<0) scale_minus();

    updateGL();
}

/*virtual*/void letter::keyPressEvent(QKeyEvent* pe)
{
    switch (pe->key())
    {
    case Qt::Key_Plus:
        scale_plus();
        break;

    case Qt::Key_Equal:
        scale_plus();
        break;

    case Qt::Key_Minus:
        scale_minus();
        break;

    case Qt::Key_W:
        rotate_up();
        break;

    case Qt::Key_S:
        rotate_down();
        break;

    case Qt::Key_A:
        rotate_left();
        break;

    case Qt::Key_D:
        rotate_right();
        break;

    case Qt::Key_Z:
        translate_down();
        break;

    case Qt::Key_X:
        translate_up();
        break;

    case Qt::Key_Space:
        defaultScene();
        break;

    case Qt::Key_Escape:
        this->close();
        break;
    }

    updateGL();
}


void letter::scale_plus()
{
    nSca = nSca*1.1;
}

void letter::scale_minus()
{
    nSca = nSca/1.1;
}

void letter::rotate_up()
{
    xRot += 1.0;
}

void letter::rotate_down()
{
    xRot -= 1.0;
}

void letter::rotate_left()
{
    zRot += 1.0;
}

void letter::rotate_right()
{
    zRot -= 1.0;
}

void letter::translate_down()
{
    zTra -= 0.05;
}

void letter::translate_up()
{
    zTra += 0.05;
}

void letter::defaultScene()
{
    xRot=-90; yRot=0; zRot=0; zTra=0; nSca=1;
}
void letter::drawAxis()
{
    glLineWidth(3.0f);

    glColor4f(1.00f, 0.00f, 0.00f, 1.0f);

    glBegin(GL_LINES);
    glVertex3f( 10.0f,  0.0f,  0.0f);
    glVertex3f(-10.0f,  0.0f,  0.0f);
    glEnd();

    QColor halfGreen(0, 128, 0, 255);
    qglColor(halfGreen);
    glBegin(GL_LINES);

    glVertex3f( 0.0f,  10.0f,  0.0f);
    glVertex3f( 0.0f, -10.0f,  0.0f);

    glColor4f(0.00f, 0.00f, 1.00f, 1.0f);

    glVertex3f( 0.0f,  0.0f,  10.0f);
    glVertex3f( 0.0f,  0.0f, -10.0f);

    glColor4f(0.00f, 0.00f, 0.00f, 0.0f);
    for (float i = -10; i < 10; i+=0.25){
        float k = 20.0;
        glBegin(GL_LINES);
        glVertex3f(i, -1.0 / k, 0.0f);
        glVertex3f(i, 1.0 / k, 0.0f);
        glVertex3f(-1.0 / k, i, 0.0f);
        glVertex3f(1.0 / k, i, 0.0f);
        glVertex3f( 0.0f, 1.0 / k, i);
        glVertex3f( 0.0f, -1.0 / k, i);
    }

    glEnd();
}

void letter::getVertexArray(float k){
    VertexArray[0][0]=0;
    VertexArray[0][1]=0;
    VertexArray[0][2]=0;

    VertexArray[1][0]=0;
    VertexArray[1][1]=0;
    VertexArray[1][2]=-0.25*k;

    VertexArray[2][0]=0;
    VertexArray[2][1]=2*k;
    VertexArray[2][2]=-0.25*k;

    VertexArray[3][0]=0;
    VertexArray[3][1]=2*k;
    VertexArray[3][2]=0;

    VertexArray[4][0]=0;
    VertexArray[4][1]=2*k;
    VertexArray[4][2]=0;

    VertexArray[5][0]=0;
    VertexArray[5][1]=2*k;
    VertexArray[5][2]=-0.25*k;

    VertexArray[6][0]=0.25*k;
    VertexArray[6][1]=2*k;
    VertexArray[6][2]=-0.25*k;

    VertexArray[7][0]=0.25*k;
    VertexArray[7][1]=2*k;
    VertexArray[7][2]=0;

    VertexArray[8][0]=0;
    VertexArray[8][1]=0;
    VertexArray[8][2]=0;

    VertexArray[9][0]=0;
    VertexArray[9][1]=0;
    VertexArray[9][2]=-0.25*k;

    VertexArray[10][0]=0.25*k;
    VertexArray[10][1]=0;
    VertexArray[10][2]=-0.25*k;

    VertexArray[11][0]=0.25*k;
    VertexArray[11][1]=0;
    VertexArray[11][2]=0;

    VertexArray[12][0]=0;
    VertexArray[12][1]=0;
    VertexArray[12][2]=0;

    VertexArray[13][0]=0.25*k;
    VertexArray[13][1]=0;
    VertexArray[13][2]=0;

    VertexArray[14][0]=0.25*k;
    VertexArray[14][1]=2*k;
    VertexArray[14][2]=0;

    VertexArray[15][0]=0;
    VertexArray[15][1]=2*k;
    VertexArray[15][2]=0;

    VertexArray[16][0]=0;
    VertexArray[16][1]=0;
    VertexArray[16][2]=-0.25*k;

    VertexArray[17][0]=0.25*k;
    VertexArray[17][1]=0;
    VertexArray[17][2]=-0.25*k;

    VertexArray[18][0]=0.25*k;
    VertexArray[18][1]=2*k;
    VertexArray[18][2]=-0.25*k;

    VertexArray[19][0]=0;
    VertexArray[19][1]=2*k;
    VertexArray[19][2]=-0.25*k;

    VertexArray[20][0]=0;
    VertexArray[20][1]=0;
    VertexArray[20][2]=0;

    VertexArray[21][0]=0;
    VertexArray[21][1]=0.25*k;
    VertexArray[21][2]=0;

    VertexArray[22][0]=k;
    VertexArray[22][1]=0.25*k;
    VertexArray[22][2]=0;

    VertexArray[23][0]=k;
    VertexArray[23][1]=0;
    VertexArray[23][2]=0;

    VertexArray[24][0]=0;
    VertexArray[24][1]=0;
    VertexArray[24][2]=-0.25*k;

    VertexArray[25][0]=0;
    VertexArray[25][1]=0.25*k;
    VertexArray[25][2]=-0.25*k;

    VertexArray[26][0]=k;
    VertexArray[26][1]=0.25*k;
    VertexArray[26][2]=-0.25*k;

    VertexArray[27][0]=k;
    VertexArray[27][1]=0;
    VertexArray[27][2]=-0.25*k;

    VertexArray[28][0]=0;
    VertexArray[28][1]=0;
    VertexArray[28][2]=0;

    VertexArray[29][0]=k;
    VertexArray[29][1]=0;
    VertexArray[29][2]=0;

    VertexArray[30][0]=k;
    VertexArray[30][1]=0;
    VertexArray[30][2]=-0.25*k;

    VertexArray[31][0]=0;
    VertexArray[31][1]=0;
    VertexArray[31][2]=-0.25*k;

    VertexArray[32][0]=0.25*k;
    VertexArray[32][1]=0.25*k;
    VertexArray[32][2]=0;

    VertexArray[33][0]=0.25*k;
    VertexArray[33][1]=0.25*k;
    VertexArray[33][2]=-0.25*k;

    VertexArray[34][0]=k;
    VertexArray[34][1]=0.25*k;
    VertexArray[34][2]=-0.25*k;

    VertexArray[35][0]=k;
    VertexArray[35][1]=0.25*k;
    VertexArray[35][2]=0;

    VertexArray[36][0]=0.25*k;
    VertexArray[36][1]=0.25*k;
    VertexArray[36][2]=0;

    VertexArray[37][0]=0.25*k;
    VertexArray[37][1]=0.25*k;
    VertexArray[37][2]=-0.25*k;

    VertexArray[38][0]=0.25*k;
    VertexArray[38][1]=2*k;
    VertexArray[38][2]=-0.25*k;

    VertexArray[39][0]=0.25*k;
    VertexArray[39][1]=2*k;
    VertexArray[39][2]=0;

    VertexArray[40][0]=k;
    VertexArray[40][1]=0;
    VertexArray[40][2]=0;

    VertexArray[41][0]=k;
    VertexArray[41][1]=0;
    VertexArray[41][2]=-0.25*k;

    VertexArray[42][0]=k;
    VertexArray[42][1]=0.25*k;
    VertexArray[42][2]=-0.25*k;

    VertexArray[43][0]=k;
    VertexArray[43][1]=0.25*k;
    VertexArray[43][2]=0;
}
void letter::getIndexArray()
{
    IndexArray[0][0]=0;
    IndexArray[0][1]=1;
    IndexArray[0][2]=2;
    IndexArray[0][3]=3;

    IndexArray[1][0]=4;
    IndexArray[1][1]=5;
    IndexArray[1][2]=6;
    IndexArray[1][3]=7;

    IndexArray[2][0]=8;
    IndexArray[2][1]=9;
    IndexArray[2][2]=10;
    IndexArray[2][3]=11;

    IndexArray[3][0]=12;
    IndexArray[3][1]=13;
    IndexArray[3][2]=14;
    IndexArray[3][3]=15;

    IndexArray[4][0]=16;
    IndexArray[4][1]=17;
    IndexArray[4][2]=18;
    IndexArray[4][3]=19;

    IndexArray[5][0]=20;
    IndexArray[5][1]=21;
    IndexArray[5][2]=22;
    IndexArray[5][3]=23;

    IndexArray[6][0]=24;
    IndexArray[6][1]=25;
    IndexArray[6][2]=26;
    IndexArray[6][3]=27;

    IndexArray[7][0]=28;
    IndexArray[7][1]=29;
    IndexArray[7][2]=30;
    IndexArray[7][3]=31;

    IndexArray[8][0]=32;
    IndexArray[8][1]=33;
    IndexArray[8][2]=34;
    IndexArray[8][3]=35;

    IndexArray[9][0]=36;
    IndexArray[9][1]=37;
    IndexArray[9][2]=38;
    IndexArray[9][3]=39;

    IndexArray[10][0]=40;
    IndexArray[10][1]=41;
    IndexArray[10][2]=42;
    IndexArray[10][3]=43;
}
void letter::draw_rotation_z()
{
    GLfloat c[52][3];
    GLfloat conv[3][3] ;
    conv[0][0]=cos(angle*(M_PI/180.0));;
    conv[0][1]=-sin(angle*(M_PI/180.0));;
    conv[0][2]=0;

    conv[1][0]=sin(angle*(M_PI/180.0));
    conv[1][1]=cos(angle*(M_PI/180.0));
    conv[1][2]=0;

    conv[2][0]=0;
    conv[2][1]=0;
    conv[2][2]=1;
    qDebug()<<"Z rotation matrix"<<endl;
    for(int i=0; i < 3; i++)
    {
        qDebug()<<conv[i][0]<<" "<<conv[i][1]<<" "<<conv[i][2]<<" ";
        qDebug()<<endl;
    }

    for(int i=0; i < 52; i++)
    {
        for(int j=0; j < 3; j++)
        {
            c[i][j]=0;
            for(int k=0; k < 3; k++)
            {
                c[i][j]+=VertexArray[i][k]*conv[k][j];
            }
        }
    }
    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 52, GL_UNSIGNED_BYTE, IndexArray);

}
void letter::draw_rotation_y()
{
    GLfloat c[52][3];
    GLfloat conv[3][3] ;
    conv[0][0]=cos(angle*(M_PI/180.0));
    conv[0][1]=0;
    conv[0][2]=sin(angle*(M_PI/180.0));

    conv[1][0]=0;
    conv[1][1]=1;
    conv[1][2]=0;

    conv[2][0]=-sin(angle*(M_PI/180.0));
    conv[2][1]=0;
    conv[2][2]=cos(angle*(M_PI/180.0));

    qDebug()<<"Y rotation matrix"<<endl;
    for(int i=0; i < 3; i++)
    {
        qDebug()<<conv[i][0]<<" "<<conv[i][1]<<" "<<conv[i][2]<<" ";
        qDebug()<<endl;
    }

    for(int i=0; i < 52; i++)
    {
        for(int j=0; j < 3; j++)
        {
            c[i][j]=0;
            for(int k=0; k < 3; k++)
            {
                c[i][j]+=VertexArray[i][k]*conv[k][j];
            }
        }
    }
    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 52, GL_UNSIGNED_BYTE, IndexArray);

}
void letter::draw_rotation_x()
{
    GLfloat c[52][3];
    GLfloat conv[3][3] ;
    conv[0][0]=1;
    conv[0][1]=0;
    conv[0][2]=0;

    conv[1][0]=0;
    conv[1][1]=cos(angle*(M_PI/180.0));
    conv[1][2]=-sin(angle*(M_PI/180.0));

    conv[2][0]=0;
    conv[2][1]=sin(angle*(M_PI/180.0));
    conv[2][2]=cos(angle*(M_PI/180.0));

    qDebug()<<"X rotation matrix"<<endl;
    for(int i=0; i < 3; i++)
    {
        qDebug()<<conv[i][0]<<" "<<conv[i][1]<<" "<<conv[i][2]<<" ";
        qDebug()<<endl;
    }

    for(int i=0; i < 52; i++)
    {
        for(int j=0; j < 3; j++)
        {
            c[i][j]=0;
            for(int k=0; k < 3; k++)
            {
                c[i][j]+=VertexArray[i][k]*conv[k][j];
            }
        }
    }
    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 52, GL_UNSIGNED_BYTE, IndexArray);

}
void letter::draw_transfer()
{
    GLfloat c[52][3];
    GLfloat conv[4][3] ;
    conv[0][0]=1;
    conv[0][1]=0;
    conv[0][2]=0;

    conv[1][0]=0;
    conv[1][1]=1;
    conv[1][2]=0;

    conv[2][0]=0;
    conv[2][1]=0;
    conv[2][2]=1;

    conv[3][0]=x_transfer;
    conv[3][1]=y_transfer;
    conv[3][2]=z_transfer;

    qDebug()<<"transfer matrix"<<endl;
    for(int i=0; i < 4; i++)
    {
        qDebug()<<conv[i][0]<<" "<<conv[i][1]<<" "<<conv[i][2]<<" ";
        qDebug()<<endl;
    }

    for(int i=0; i < 52; i++)
    {
        for(int j=0; j < 3; j++)
        {
            c[i][j]=0;
            for(int k=0; k < 4; k++)
            {
                if(k==3)
                    c[i][j]+=1*conv[k][j];
                else
                    c[i][j]+=VertexArray[i][k]*conv[k][j];
            }
        }
    }
    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 52, GL_UNSIGNED_BYTE, IndexArray);
}
void letter::draw_scaling()
{
    GLfloat c[52][3];
    GLfloat conv[3][3] ;
    conv[0][0]=x_scale;
    conv[0][1]=0;
    conv[0][2]=0;

    conv[1][0]=0;
    conv[1][1]=y_scale;
    conv[1][2]=0;

    conv[2][0]=0;
    conv[2][1]=0;
    conv[2][2]=z_scale;

    qDebug()<<"scale matrix"<<endl;
    for(int i=0; i < 3; i++)
    {
        qDebug()<<conv[i][0]<<" "<<conv[i][1]<<" "<<conv[i][2]<<" ";
        qDebug()<<endl;
    }

    for(int i=0; i < 52; i++)
    {
        for(int j=0; j < 3; j++)
        {
            c[i][j]=0;
            for(int k=0; k < 3; k++)
                c[i][j]+=VertexArray[i][k]*conv[k][j];
        }
    }
    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 52, GL_UNSIGNED_BYTE, IndexArray);
}
void letter::draw_xy_projection()
{
    GLfloat c[52][3];
    GLfloat conv[3][3] ;
    conv[0][0]=1;
    conv[0][1]=0;
    conv[0][2]=0;

    conv[1][0]=0;
    conv[1][1]=1;
    conv[1][2]=0;

    conv[2][0]=0;
    conv[2][1]=0;
    conv[2][2]=0;
    for(int i=0; i < 52; i++)
    {
        for(int j=0; j < 3; j++)
        {
            c[i][j]=0;
            for(int k=0; k < 3; k++)
                c[i][j]+=VertexArray[i][k]*conv[k][j];
        }
    }
    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 52, GL_UNSIGNED_BYTE, IndexArray);
}
void letter::draw_xz_projection()
{
    GLfloat c[52][3];
    GLfloat conv[3][3] ;
    conv[0][0]=1;
    conv[0][1]=0;
    conv[0][2]=0;

    conv[1][0]=0;
    conv[1][1]=0;
    conv[1][2]=0;

    conv[2][0]=0;
    conv[2][1]=0;
    conv[2][2]=1;
    for(int i=0; i < 52; i++)
    {
        for(int j=0; j < 3; j++)
        {
            c[i][j]=0;
            for(int k=0; k < 3; k++)
                c[i][j]+=VertexArray[i][k]*conv[k][j];
        }
    }
    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 52, GL_UNSIGNED_BYTE, IndexArray);
}
void letter::draw_zy_projection()
{
    GLfloat c[52][3];
    GLfloat conv[3][3] ;
    conv[0][0]=0;
    conv[0][1]=0;
    conv[0][2]=0;

    conv[1][0]=0;
    conv[1][1]=1;
    conv[1][2]=0;

    conv[2][0]=0;
    conv[2][1]=0;
    conv[2][2]=1;
    for(int i=0; i < 52; i++)
    {
        for(int j=0; j < 3; j++)
        {
            c[i][j]=0;
            for(int k=0; k < 3; k++)
                c[i][j]+=VertexArray[i][k]*conv[k][j];
        }
    }
    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 52, GL_UNSIGNED_BYTE, IndexArray);
}
void letter::drawFigure(){
    glVertexPointer(3, GL_FLOAT, 0, VertexArray);
    glDrawElements(GL_QUADS, 52, GL_UNSIGNED_BYTE, IndexArray);
}
