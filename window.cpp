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

#include "glwidget.h"
#include "window.h"

Window::Window()
{
    glWidget = new GLWidget;

    // Initialize UI sliders to control program
    QLabel *massLabel = new QLabel("Mass");
    QSlider *massSlider = new QSlider(Qt::Horizontal);
    massSlider->setRange(0, 100);
    massSlider->setSingleStep(5);
    massSlider->setPageStep(5);
    massSlider->setTickInterval(5);
    massSlider->setTickPosition(QSlider::TicksRight);

    QLabel *springConstantLabel = new QLabel("Spring Constant");
    QSlider *springConstantSlider = new QSlider(Qt::Horizontal);
    springConstantSlider->setRange(0, 100);
    springConstantSlider->setSingleStep(5);
    springConstantSlider->setPageStep(5);
    springConstantSlider->setTickInterval(5);
    springConstantSlider->setTickPosition(QSlider::TicksRight);

    QLabel *dampingConstantLabel = new QLabel("Damping Constant");
    QSlider *dampingConstantSlider = new QSlider(Qt::Horizontal);
    dampingConstantSlider->setRange(0, 100);
    dampingConstantSlider->setSingleStep(5);
    dampingConstantSlider->setPageStep(5);
    dampingConstantSlider->setTickInterval(5);
    dampingConstantSlider->setTickPosition(QSlider::TicksRight);

    QLabel *restLengthLabel = new QLabel("Rest Length");
    QSlider *restLengthSlider = new QSlider(Qt::Horizontal);
    restLengthSlider->setRange(0, 100);
    restLengthSlider->setSingleStep(5);
    restLengthSlider->setPageStep(5);
    restLengthSlider->setTickInterval(5);
    restLengthSlider->setTickPosition(QSlider::TicksRight);

    QLabel *timeStepLabel = new QLabel("Time Step");
    QSlider *timeStepSlider = new QSlider(Qt::Horizontal);
    timeStepSlider->setRange(0, 100);
    timeStepSlider->setSingleStep(5);
    timeStepSlider->setPageStep(5);
    timeStepSlider->setTickInterval(5);
    timeStepSlider->setTickPosition(QSlider::TicksRight);

    QPushButton *restartButton = new QPushButton("Add Point to Rocket Spline");

    // add sliders to a layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(glWidget);
    mainLayout->addWidget(restartButton, 0, Qt::AlignTop);
    mainLayout->addWidget(massLabel, 0, Qt::AlignTop);
    mainLayout->addWidget(massSlider, 0, Qt::AlignTop);
    mainLayout->addWidget(springConstantLabel, 0, Qt::AlignTop);
    mainLayout->addWidget(springConstantSlider, 0, Qt::AlignTop);
    mainLayout->addWidget(dampingConstantLabel, 0, Qt::AlignTop);
    mainLayout->addWidget(dampingConstantSlider, 0, Qt::AlignTop);
    mainLayout->addWidget(restLengthLabel, 0, Qt::AlignTop);
    mainLayout->addWidget(restLengthSlider, 0, Qt::AlignTop);
    mainLayout->addWidget(timeStepLabel, 0, Qt::AlignTop);
    mainLayout->addWidget(timeStepSlider, 0, Qt::AlignTop);
    setLayout(mainLayout);
    setWindowTitle(tr("Mass-Spring System"));

    // Connect signals to slots
    connect(restartButton, SIGNAL(pressed()), glWidget, SLOT(restartSimulation()));
    connect(massSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setMass(int)));
    connect(glWidget, SIGNAL(massChanged(int)), massSlider, SLOT(setValue(int)));
    connect(springConstantSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setSpringConstant(int)));
    connect(glWidget, SIGNAL(springConstantChanged(int)), springConstantSlider, SLOT(setValue(int)));
    connect(dampingConstantSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setDampingConstant(int)));
    connect(glWidget, SIGNAL(dampingConstantChanged(int)), dampingConstantSlider, SLOT(setValue(int)));
    connect(restLengthSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setRestLength(int)));
    connect(glWidget, SIGNAL(restLengthChanged(int)), restLengthSlider, SLOT(setValue(int)));
    connect(timeStepSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setTimeStep(int)));
    connect(glWidget, SIGNAL(timeStepChanged(int)), timeStepSlider, SLOT(setValue(int)));

    // initialide slider values
    massSlider->setValue(20);
    springConstantSlider->setValue(50);
    dampingConstantSlider->setValue(5);
    restLengthSlider->setValue(10);
    timeStepSlider->setValue(1);
}

void Window::drawOpenGL()
{
    glWidget->repaint();
}

void Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);

    glWidget->keyPressEvent(e);

}

void Window::keyReleaseEvent(QKeyEvent *e)
{
    glWidget->keyReleaseEvent(e);
}
















