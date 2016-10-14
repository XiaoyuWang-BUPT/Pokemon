#-------------------------------------------------
#
# Project created by QtCreator 2016-10-13T14:48:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PokemonClient
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    dialog.cpp

HEADERS  += widget.h \
    dialog.h

FORMS    += widget.ui \
    dialog.ui
