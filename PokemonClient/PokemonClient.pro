#-------------------------------------------------
#
# Project created by QtCreator 2016-11-28T10:29:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PokemonClient
TEMPLATE = app


SOURCES += src/main.cpp \
    src/signin.cpp \
    src/mainpage.cpp \
    src/signon.cpp \
    src/socketclient.cpp \
    src/hunt.cpp \
    src/battle.cpp \
    src/acquaintance.cpp

HEADERS  += \
    lib/json.hpp \
    src/signin.h \
    src/mainpage.h \
    src/signon.h \
    src/socketClient.h \
    src/hunt.h \
    src/battle.h \
    src/acquaintance.h

FORMS    += \
    signin.ui \
    mainpage.ui \
    signon.ui \
    hunt.ui \
    battle.ui \
    acquaintance.ui

RESOURCES   += resource.qrc \
    resource2.qrc
