TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -pthread

SOURCES += main.cpp \
    pen.cpp \
    motor.cpp \
    plotter.cpp \
    simulator.cpp \
    utils.cpp

HEADERS += \
    pen.h \
    motor.h \
    plotter.h \
    simulator.h \
    typesdefs.h \
    utils.h
