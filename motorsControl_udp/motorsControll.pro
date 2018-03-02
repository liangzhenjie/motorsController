#-------------------------------------------------
#
# Project created by QtCreator 2018-01-04T18:34:48
#
#-------------------------------------------------

QT       -= gui
QT += network
QT += core
TARGET = motorsControll
TEMPLATE = lib
#CONFIG -= app_bundle

DEFINES += MOTORSCONTROLL_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += motorscontroller.cpp \
    communication.cpp \
    dataUtil.cpp \
    idata.cpp \
    innfosproxy.cpp \
    mediator.cpp \
    motordata.cpp \
    proxyparser.cpp \
    autorecoginze.cpp \
    communicateunit.cpp

HEADERS += motorscontroller.h\
        motorscontroll_global.h \
    communication.h \
    dataUtil.h \
    idata.h \
    innfosproxy.h \
    mediator.h \
    motordata.h \
    proxyparser.h \
    userdefine.h \
    autorecoginze.h \
    communicateunit.h \
    CSignal.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES +=
