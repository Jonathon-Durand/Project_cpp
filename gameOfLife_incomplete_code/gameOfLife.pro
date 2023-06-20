QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gameOfLife
TEMPLATE = app
CONFIG   += console

SOURCES +=\
    moregraphicswindow.cpp \
    gameOfLife.cpp \
    moregraphics.cpp \
    pset9_prob1_Durand_Jonathon.cpp

HEADERS  += moregraphicswindow.h \
    moregraphics.h \
    life.h

FORMS    += moregraphicswindow.ui
