#-------------------------------------------------
#
# Project created by QtCreator 2012-02-11T21:04:26
#
#-------------------------------------------------

QT       += core gui qwt

TARGET = ampMeter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ampplot.cpp \
    randomdata.cpp \
    constantdata.cpp

HEADERS  += mainwindow.h \
    ampplot.h \
    datasource.h \
    randomdata.h \
    constantdata.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/include/qwt
LIBS    +=  -lqwt

