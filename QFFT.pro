#-------------------------------------------------
#
# Project created by QtCreator 2013-10-29T00:40:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia
lessThan(QT_MAJOR_VERSION, 5): {
    CONFIG += mobility
    MOBILITY += multimedia
}

TARGET = QFFT
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    audiobuffer.cpp \
    waterfallwidget.cpp \
    fftthread.cpp

HEADERS  += mainwindow.h \
    audiobuffer.h \
    waterfallwidget.h \
    fftthread.h

FORMS    += mainwindow.ui
