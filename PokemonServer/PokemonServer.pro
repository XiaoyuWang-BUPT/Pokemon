QT += core
QT -= gui

CONFIG += c++11

TARGET = PokemonServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += src/main.cpp \
    src/pokemon.cpp \
    src/pokemonfactory.cpp \
    src/player.cpp \
    lib/sqlite3.c \
    src/playerfactory.cpp

HEADERS += src/catchunittest.h \
    src/pokemon.h \
    src/pokemonfactory.h \
    lib/catch.hpp \
    src/player.h \
    lib/sqlite3.h \
    src/playerfactory.h
