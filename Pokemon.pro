#-------------------------------------------------
#
# Project created by QtCreator 2016-10-07T16:09:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pokemon
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    pokemon.cpp \
    pokemonfactory.cpp

HEADERS  += widget.h \
    pokemon.h \
    pokemonfactory.h

FORMS    += widget.ui
