#-------------------------------------------------
#
# Project created by QtCreator 2012-08-08T22:34:43
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = ThreadedServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    qsimpleserver.cpp \
    qsocketthread.cpp \
    requesthandler.cpp

HEADERS += \
    qsimpleserver.h \
    qsocketthread.h \
    requesthandler.h




