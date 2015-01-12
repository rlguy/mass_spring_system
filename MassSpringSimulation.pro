#    CSC 473 Fall 2014
#    Ryan Guy

HEADERS       = glwidget.h \
                window.h \
    camera3d.h \
    massspringsystem.h \
    pointmass.h \
    spring.h \
    quaternion.h
SOURCES       = glwidget.cpp \
                main.cpp \
                window.cpp \
    camera3d.cpp \
    massspringsystem.cpp \
    pointmass.cpp \
    spring.cpp \
    quaternion.cpp
QT           += opengl widgets


contains(QT_CONFIG, opengles.) {
    contains(QT_CONFIG, angle): \
        warning("Qt was built with ANGLE, which provides only OpenGL ES 2.0 on top of DirectX 9.0c")
    error("This example requires Qt to be configured with -opengl desktop")
}
