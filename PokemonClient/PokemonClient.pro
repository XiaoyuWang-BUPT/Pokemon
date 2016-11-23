#-------------------------------------------------
#
# Project created by QtCreator 2016-10-13T14:48:18
#
#-------------------------------------------------

QT       += core gui\

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PokemonClient
TEMPLATE = app


SOURCES += src/main.cpp \
    src/signin.cpp \
    src/mainpage.cpp \
    src/signon.cpp

HEADERS  += \
    src/signin.h \
    src/mainpage.h \
    src/signon.h

FORMS    += \
    signin.ui \
    mainpage.ui \
    signon.ui
