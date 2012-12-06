#-------------------------------------------------
#
# Project created by QtCreator 2012-11-02T19:14:59
#
#-------------------------------------------------

QT       += core gui
QT       += xml

TARGET = projetSDD1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    game.cpp \
    mygraphicsscene.cpp \
    xmlfilechoice.cpp \
    graph.cpp \
    list.cpp \
    endwindow.cpp \
    IA.cpp \
    historicalwindow.cpp

HEADERS  += mainwindow.h \
    game.h \
    graph.h \
    list.h \
    collection.h \
    matrix.h \
    tools.h \
    vector.h \
    mygraphicsscene.h \
    xmlfilechoice.h \
    state.h \
    endwindow.h \
    IA.h \
    historicalwindow.h

FORMS    += mainwindow.ui \
    xmlfilechoice.ui \
    endwindow.ui \
    historicalwindow.ui
