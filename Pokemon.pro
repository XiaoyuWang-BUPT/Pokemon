#-------------------------------------------------
#
# Project created by QtCreator 2016-10-07T16:09:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pokemon
TEMPLATE = app


SOURCES += src/main.cpp\
        src/widget.cpp \
    src/pokemon.cpp \
    src/pokemonfactory.cpp

HEADERS  += src/widget.h \
    src/pokemon.h \
    src/pokemonfactory.h \
    src/catchunittest.h \
    lib/catch.hpp

FORMS    += widget.ui
