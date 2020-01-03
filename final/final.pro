QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = final
TEMPLATE = app
CONFIG += c++11 link_pkgconfig
PKGCONFIG += dtkwidget

SOURCES += \
        main.cpp \
    remotewindow.cpp

RESOURCES +=         \
    images.qrc

HEADERS += \
    remotewindow.h
