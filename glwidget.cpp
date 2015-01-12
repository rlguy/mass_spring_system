/* CSC 473 Fall 2014
 * Ryan Guy
 */

/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <QtOpenGL>
#include "glwidget.h"
#include <cmath>
#include "glm/glm.hpp"

#include "quaternion.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

//! [0]
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    screenWidth = 1280;
    screenHeight = 400;

    // initialize update/draw timers
    float updatesPerSecond = 120;
    float drawsPerSecond = 30;

    drawTimer = new QTimer(this);
    connect(drawTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
    drawTimer->start(1000.0/drawsPerSecond);

    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateSimulation()));
    updateTimer->start(1000.0/updatesPerSecond);

    deltaTimer = new QTime();
    deltaTimer->start();

    // Initialize camera
    float camX = 16.88;
    float camY = 11.82;
    float camZ = 11.41;
    float dirX = -0.828;
    float dirY = -0.472;
    float dirZ = -0.302;
    camera = camera3d(camX, camY, camZ, dirX, dirY, dirZ);

    // for updating camera movement
    isMovingForward = false;
    isMovingBackward = false;
    isMovingRight = false;
    isMovingLeft = false;
    isMovingUp = false;
    isMovingDown = false;
    isRotatingRight = false;
    isRotatingLeft = false;
    isRotatingUp = false;
    isRotatingDown = false;
    isRollingRight = false;
    isRollingLeft = false;

    // slider ranges and values
    minMassValue = 0.05;
    maxMassValue = 0.5;
    minSpringConstantValue = 1.0;
    maxSpringConstantValue = 50.0;
    minDampingConstantValue = 0.05;
    maxDampingConstantValue = 3.0;
    minRestLengthValue = 0.05;
    maxRestLengthValue = 1.0;
    minTimeStepValue = floor(1000.0/320.0);
    maxTimeStepValue = floor(1000.0/24.0);

    massValue = minMassValue;
    springConstantValue = minSpringConstantValue;
    dampingConstantValue = minDampingConstantValue;
    restLengthValue = minRestLengthValue;
    timeStepValue = minTimeStepValue;

    // simulation system
    isFirstFrameFinished = false;
    minDeltaTimeModifier = 0.125;
    maxDeltaTimeModifier = 1.0;
    deltaTimeModifier = maxDeltaTimeModifier;
    initializeSimulation();

}

GLWidget::~GLWidget()
{
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(screenWidth, screenHeight);
}

void GLWidget::initializeGL()
{
    static const GLfloat lightPos[4] = { 20.0f, 20.0f, 20.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

// Create masses and connect them together with springs
void GLWidget::initializeSimulation() {
    simSystem = MassSpringSystem();
    simSystem.setDistanceScale(0.5);

    int width = 75;
    int height = 75;
    float y = 10.0;
    float xstep = 0.1;
    float zstep = 0.1;

    // create grid of masses aligned to x-z plane
    PointMass *massGrid[height][width];
    float z = 3.0;
    for (int j=0; j<height; j++) {
        float x = 3.0;
        for (int i=0; i<width; i++) {
            massGrid[j][i] = simSystem.addMass(massValue, x, y, z);
            masses.push_back(massGrid[j][i]);
            x = x + xstep;
        }
        z = z + zstep;
    }

    // Set fixed masses for surface to hang from
    for (int i=0; i<width; i++) {
        massGrid[0][i]->setFixedPosition(true);
    }
    int pad = 10;
    for (int j=pad+5; j<height-pad; j++) {
        for (int i=pad; i<width-pad; i++) {
            massGrid[j][i]->setFixedPosition(true);
        }
    }

    // Each mass is connected to it's neighbours with a spring
    float k = springConstantValue;
    float kd = dampingConstantValue;
    float rl = restLengthValue;
    float rlLong = 1.4142 * rl;
    for (int j=0; j<height-1; j++) {
        for (int i=0; i<width-1; i++) {
            PointMass *m = massGrid[j][i];
            PointMass *mRight = massGrid[j][i+1];
            PointMass *mDown = massGrid[j+1][i];
            PointMass *mDownRight = massGrid[j+1][i+1];

            Spring *s1 = simSystem.addSpring(k, rl, m, mRight);
            Spring *s2 = simSystem.addSpring(k, rl, m, mDown);
            Spring *s3 = simSystem.addSpring(k, rlLong, m, mDownRight);
            Spring *s4 = simSystem.addSpring(k, rlLong, mRight, mDown);
            s1->setDampingConstant(kd);
            s2->setDampingConstant(kd);
            s3->setDampingConstant(kd);
            s4->setDampingConstant(kd);
            shortSprings.push_back(s1);
            shortSprings.push_back(s2);
            longSprings.push_back(s3);
            longSprings.push_back(s4);
        }
    }

    // right side and bottom side are still not connected with springs
    for (int j=0; j<height-1; j++) {
        PointMass *m1 = massGrid[j][width-1];
        PointMass *m2 = massGrid[j+1][width-1];
        Spring *s1 = simSystem.addSpring(k, rl, m1, m2);
        s1->setDampingConstant(kd);
        shortSprings.push_back(s1);
    }
    for (int i=0; i<width-1; i++) {
        PointMass *m1 = massGrid[height-1][i];
        PointMass *m2 = massGrid[height-1][i+1];
        Spring *s1 = simSystem.addSpring(k, rl, m1, m2);
        s1->setDampingConstant(kd);
        shortSprings.push_back(s1);
    }
}

// move and rotate camera
void GLWidget::updateCameraMovement(float dt) {
    float camSpeed = 10.0;
    float camRotSpeed = 2.0;

    if (isMovingForward) { camera.moveForward(camSpeed * dt); }
    if (isMovingBackward) { camera.moveBackward(camSpeed * dt); }
    if (isMovingRight) { camera.moveRight(camSpeed * dt); }
    if (isMovingLeft) { camera.moveLeft(camSpeed * dt); }
    if (isMovingUp) { camera.moveUp(camSpeed * dt); }
    if (isMovingDown) { camera.moveDown(camSpeed * dt); }
    if (isRotatingRight) { camera.rotateRight(camRotSpeed * dt); }
    if (isRotatingLeft) { camera.rotateLeft(camRotSpeed * dt); }
    if (isRotatingUp) { camera.rotateUp(camRotSpeed * dt); }
    if (isRotatingDown) { camera.rotateDown(camRotSpeed * dt); }
    if (isRollingRight) { camera.rollRight(camRotSpeed * dt); }
    if (isRollingLeft) { camera.rollLeft(camRotSpeed * dt); }
}

void GLWidget::updateSimulation() {
    float dt = (float) deltaTimer->elapsed() / 1000;

    // dt might be large on the first frame
    if (!isFirstFrameFinished) {
        dt = minTimeStepValue / 1000.0;
        isFirstFrameFinished = true;
    }
    // make sure dt does not get too big
    if (dt > maxTimeStepValue / 1000.0) {
        dt = maxTimeStepValue / 1000.0;
    }
    deltaTimer->restart();

    updateCameraMovement(dt);

    dt *= deltaTimeModifier;  // speed of simulation
    simSystem.update(dt);
}

// use camera to set glueLookAt
void GLWidget::updatePerspective() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 60.0, screenWidth/screenHeight, 0.5, 100.0 );

    float px = camera.position.x, py = camera.position.y, pz = camera.position.z;
    gluLookAt(px, py, pz,
              px+camera.direction.x, py+camera.direction.y, pz+camera.direction.z,
              camera.up.x, camera.up.y, camera.up.z);
}

// Draws coordinate axis' and floor grid
void GLWidget::drawGrid() {
    // draw axis'
    float len = 10.0;
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);   // x
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(len, 0.0, 0.0);
    glColor3f(0.0, 1.0, 0.0);   // y
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, len, 0.0);
    glColor3f(0.0, 0.0, 1.0);   // z
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, len);
    glEnd();

    // draw outline around xy, zy planes
    glLineWidth(2.0);
    glColor4f(0.0, 0.0, 0.0, 0.3);
    glBegin(GL_LINES);
    glVertex3f(0.0, len, 0.0);
    glVertex3f(len, len, 0.0);
    glVertex3f(len, len, 0.0);
    glVertex3f(len, 0.0, 0.0);
    glVertex3f(0.0, len, 0.0);
    glVertex3f(0.0, len, len);
    glVertex3f(0.0, len, len);
    glVertex3f(0.0, 0.0, len);
    glEnd();


    // draw xz plane grid
    float spacing = 0.25;
    int yLines = 120;
    int zLines = 120;
    float height = (float)yLines * spacing;
    float width = (float)zLines * spacing;

    float z = spacing;
    glLineWidth(1.0);
    glColor4f(0.0, 0.0, 0.0, 0.2);
    glBegin(GL_LINES);
    for (int i=0; i < yLines; i++) {
        glVertex3f(0.0, 0.0, z);
        glVertex3f(width, 0.0, z);
        z += spacing;
    }

    float x = spacing;
    for (int i=0; i < zLines; i++) {
        glVertex3f(x, 0.0, 0.0);
        glVertex3f(x, 0.0, height);
        x += spacing;
    }
    glEnd();

}

bool GLWidget::saveFrameToFile(QString fileName) {
    GLubyte *data = (GLubyte*)malloc(4*(int)screenWidth*(int)screenHeight);
    if( data ) {
        glReadPixels(0, 0, screenWidth, screenHeight,
                     GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    QImage image((int)screenWidth, (int)screenHeight, QImage::Format_RGB32);
    for (int j=0; j<screenHeight; j++) {
        for (int i=0; i<screenWidth; i++) {
            int idx = 4*(j*screenWidth + i);
            char r = data[idx+0];
            char g = data[idx+1];
            char b = data[idx+2];

            // sets 32 bit pixel at (x,y) to yellow.
            //uchar *p = image.scanLine(j) + i;
            //*p = qRgb(255, 0, 0);
            QRgb value = qRgb(r, g, b);
            image.setPixel(i, screenHeight-j-1, value);
        }
    }
    bool r = image.save(fileName);
    free(data);

    return r;
}

void GLWidget::paintGL()
{
    updatePerspective();
    drawGrid();
    simSystem.draw();

    if (isRendering) {
        std::ostringstream ss;
        ss << currentFrame;

        std::string s = ss.str();
        s.insert(s.begin(), 6 - s.size(), '0');
        s = "test_render/" + s + ".png";
        bool r = saveFrameToFile(QString::fromStdString(s));
        qDebug() << r << QString::fromStdString(s);

        currentFrame += 1;
    }
}



void GLWidget::resizeGL(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 60.0, screenWidth/screenHeight, 0.5, 10.0 );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    // turn on camera movement
    isMovingForward = event->key()  == Qt::Key_W;
    isMovingBackward = event->key() == Qt::Key_S;
    isMovingRight = event->key()    == Qt::Key_D;
    isMovingLeft = event->key()     == Qt::Key_A;
    isMovingUp = event->key()       == Qt::Key_T;
    isMovingDown = event->key()     == Qt::Key_G;

    isRotatingRight = event->key()  == Qt::Key_E;
    isRotatingLeft = event->key()   == Qt::Key_Q;
    isRotatingUp = event->key()     == Qt::Key_F;
    isRotatingDown = event->key()   == Qt::Key_R;
    isRollingRight = event->key()   == Qt::Key_X;
    isRollingLeft = event->key()    == Qt::Key_Z;

    // slow down simulation
    if (event->key() == Qt::Key_C) {
        deltaTimeModifier = minDeltaTimeModifier;
    }

    // restart simulation
    if (event->key() == Qt::Key_1) {
        restartSimulation();
    }

    if (event->key() == Qt::Key_2) {
        isRendering = !isRendering;
        qDebug() << "isRendering: " << isRendering;
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    // turn off camera movement
    isMovingForward = event->key()  == Qt::Key_W ? false : isMovingForward;
    isMovingBackward = event->key() == Qt::Key_S ? false : isMovingBackward;
    isMovingRight = event->key()    == Qt::Key_D ? false : isMovingRight;
    isMovingLeft = event->key()     == Qt::Key_A ? false : isMovingLeft;
    isMovingUp = event->key()       == Qt::Key_T ? false : isMovingUp;
    isMovingDown = event->key()     == Qt::Key_G ? false : isMovingDown;

    isRotatingRight = event->key()  == Qt::Key_E ? false : isRotatingRight;
    isRotatingLeft = event->key()   == Qt::Key_Q ? false : isRotatingLeft;
    isRotatingDown = event->key()   == Qt::Key_R ? false : isRotatingDown;
    isRotatingUp = event->key()     == Qt::Key_F ? false : isRotatingUp;
    isRollingRight = event->key()   == Qt::Key_X ? false : isRollingRight;
    isRollingLeft = event->key()    == Qt::Key_Z ? false : isRollingLeft;

    if (event->key() == Qt::Key_C) {
        deltaTimeModifier = maxDeltaTimeModifier;
    }
}

// linear interpolation
float GLWidget::lerp(float v1, float v2, float t) {
    return v1 + t * (v2 - v1);
}

void GLWidget::setMass(int percent)
{
    massValue = lerp(minMassValue, maxMassValue, (float)percent*0.01);

    for (uint i=0; i<masses.size(); i++) {
        masses[i]->setMass(massValue);
    }

    qDebug() << "Set mass" << massValue;
    emit massChanged(percent);
}

void GLWidget::setSpringConstant(int percent)
{
    springConstantValue = lerp(minSpringConstantValue,
                               maxSpringConstantValue, (float)percent*0.01);

    for (uint i=0; i<shortSprings.size(); i++) {
        shortSprings[i]->setSpringConstant(springConstantValue);
    }
    for (uint i=0; i<longSprings.size(); i++) {
        longSprings[i]->setSpringConstant(springConstantValue);
    }

    qDebug() << "Set spring" << springConstantValue;
    emit springConstantChanged(percent);
}

void GLWidget::setDampingConstant(int percent)
{
    dampingConstantValue = lerp(minDampingConstantValue,
                                maxDampingConstantValue, (float)percent*0.01);

    for (uint i=0; i<shortSprings.size(); i++) {
        shortSprings[i]->setDampingConstant(dampingConstantValue);
    }

    for (uint i=0; i<longSprings.size(); i++) {
        longSprings[i]->setDampingConstant(dampingConstantValue);
    }

    qDebug() << "Set damping" << dampingConstantValue;
    emit dampingConstantChanged(percent);
}

void GLWidget::setRestLength(int percent)
{
    restLengthValue = lerp(minRestLengthValue,
                           maxRestLengthValue, (float)percent*0.01);

    for (uint i=0; i<shortSprings.size(); i++) {
        shortSprings[i]->setRestLength(restLengthValue);
    }

    float rlLong = restLengthValue * 1.4142;
    for (uint i=0; i<longSprings.size(); i++) {
        longSprings[i]->setRestLength(rlLong);
    }

    qDebug() << "Set length" << restLengthValue;
    emit restLengthChanged(percent);
}

void GLWidget::setTimeStep(int percent)
{
    timeStepValue = floor(lerp(minTimeStepValue,
                          maxTimeStepValue, (float)percent*0.01));
    qDebug() << "Set tstep" << timeStepValue;

    updateTimer->setInterval((int)timeStepValue);
    emit timeStepChanged(percent);
}

void GLWidget::restartSimulation() {
    isFirstFrameFinished = false;
    initializeSimulation();
}







