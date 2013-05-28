#-------------------------------------------------
#
# Project created by QtCreator 2013-05-02T08:35:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets core

QMAKE_CXXFLAGS += -std=c++0x

TARGET = VoronoiVisualiser
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Fortune/VParabola.cpp \
    Fortune/voronoi.cpp \
    Controller/Centroids.cpp \
    Controller/kmeans.cpp \
    Model/cpoint.cpp \
    Model/kpoint.cpp \
    Controller/kohonen.cpp \
    Controller/neuralgas.cpp

HEADERS  += mainwindow.h \
    Fortune/VPoint.h \
    Fortune/VParabola.h \
    Fortune/voronoi.h \
    Fortune/VEvent.h \
    Fortune/VEdge.h \
    Defs.h \
    Controller/Centroids.h \
    Controller/kmeans.h \
    Model/cpoint.h \
    Model/kpoint.h \
    Controller/kohonen.h \
    Controller/neuralgas.h


FORMS    += mainwindow.ui

INCLUDEPATH += E:\boost_1_53_0\boost_1_53_0
