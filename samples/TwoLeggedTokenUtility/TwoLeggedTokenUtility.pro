#-------------------------------------------------
#
# Project created by QtCreator 2017-02-16T14:40:49
#
#-------------------------------------------------

QT       += core gui widgets network

TARGET = TwoLeggedTokenUtility
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    ../../src/shared/baseapi.cpp \
    ../../src/oauth/bearer.cpp \
    ../../src/shared/httprequest.cpp \
    ../../src/oauth/twoleggedapi.cpp

HEADERS  += mainwindow.h \
    ../../src/shared/baseapi.h \
    ../../src/oauth/bearer.h \
    ../../src/shared/httprequest.h \
    ../../src/oauth/twoleggedapi.h

FORMS    += mainwindow.ui
