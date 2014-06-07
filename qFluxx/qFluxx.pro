#-------------------------------------------------
#
# Project created by QtCreator 2014-06-07T10:30:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qFluxx
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    client.cpp \
    game.cpp \
    mode.cpp \
    register.cpp \
    waiting.cpp \
    cardLib_code/cardLib.cpp \
    mailbox/clientMB/clientMB.cpp

HEADERS  += mainwindow.h \
    client.h \
    game.h \
    mode.h \
    register.h \
    waiting.h \
    cardLib_code/cardLib.h \
    config.h \
    mailbox/clientMB/clientMB.h
