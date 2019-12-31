#-------------------------------------------------
#
# Project created by QtCreator 2017-09-16T21:40:42
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = boost_configurator
TEMPLATE = app
#CONFIG += serialport
QT += serialport

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += D:\dev\perso\BoostControler\snxcomm
LIBS += -LD:\dev\perso\BoostControler\snxcomm\debug -lsnxcomm
#LIBS += -LD:\dev\perso\BoostControler\build-snxcomm-Desktop_Qt_5_9_1_MSVC2015_64bit-Debug -lsnxcomm
LIBS += -lws2_32

SOURCES += \
        main.cpp \
        mainwindow.cpp \
	mutreader.cpp \
    qcustomplot.cpp \
    mutservice.cpp \
    logger.cpp \
    plotter.cpp

HEADERS += \
        mainwindow.h \
    config_data.h \
	mutreader.h \
    qcustomplot.h \
    mutservice.h \
    logger.h \
    plotter.h

FORMS += \
        mainwindow.ui
