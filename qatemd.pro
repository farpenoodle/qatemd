#-------------------------------------------------
#
# Project created by QtCreator 2014-09-24T22:21:46
#
#-------------------------------------------------

QT       += core network

#QT       -= gui

TARGET = qatemd
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    qatemcmd.cpp \
    libqatemcontrol/qatemconnection.cpp \
    qatemhandler.cpp \
    qatemd.cpp

HEADERS += qatemcmd.h \
    libqatemcontrol/libqatemcontrol_global.h \
    libqatemcontrol/qatemconnection.h \
    libqatemcontrol/qdownstreamkeysettings.h \
    libqatemcontrol/qupstreamkeysettings.h \
    qatemhandler.h \
    qatemd.h
