TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

TARGET = projectGraph
QMAKE_CC += -std=c99 -Wall -Wextra
CONFIG += link_pkgconfig
PKGCONFIG += gtk+-3.0

SOURCES += main.cpp \
    forcedirectedgraphdrawing.cpp \
    getdistance.cpp \
    graphclass.cpp \
    edgeclass.cpp \
    vertexclass.cpp \
    color.cpp

HEADERS += \
    forcedirectedgraphdrawing.h \
    getdistance.h \
    graphclass.h \
    edgeclass.h \
    vertexclass.h \
    color.h
