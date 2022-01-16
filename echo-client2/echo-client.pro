TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        client.cpp
QMAKE_CXXFLAGS += -std=c++0x -pthread

LIBS += -pthread

HEADERS += \
    header.h
