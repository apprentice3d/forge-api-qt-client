#-------------------------------------------------
#
# Project created by QtCreator 2017-03-21T10:06:45
#
#-------------------------------------------------

QT       += network testlib

QT       -= gui

TARGET = tst_modelderivativetestsuitetest
CONFIG   += console
CONFIG   -= app_bundle

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


SOURCES += tst_modelderivativetestsuitetest.cpp \
    ../../../src/model_derivative/modelderivativeapi.cpp \
    ../../../src/shared/baseapi.cpp \
    ../../../src/shared/httprequest.cpp \
    ../../../src/oauth/bearer.cpp \
    ../../../src/oauth/twoleggedapi.cpp \
    ../../../src/model_derivative/jobpayload.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../../../src/model_derivative/modelderivativeapi.h \
    ../../../src/shared/baseapi.h \
    ../../../src/shared/httprequest.h \
    ../../../src/oauth/bearer.h \
    ../../../src/oauth/twoleggedapi.h \
    ../../../src/model_derivative/jobpayload.h
