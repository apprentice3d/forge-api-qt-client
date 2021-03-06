#-------------------------------------------------
#
# Project created by QtCreator 2017-04-03T14:47:13
#
#-------------------------------------------------

QT       += core gui network widgets

TARGET = BucketExplorer
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
    ../../src/oauth/bearer.cpp \
    ../../src/oauth/twoleggedapi.cpp \
    ../../src/oss/bucket.cpp \
    ../../src/oss/bucketapi.cpp \
    ../../src/oss/bucketobject.cpp \
    ../../src/oss/bucketobjectapi.cpp \
    ../../src/shared/baseapi.cpp \
    ../../src/shared/httprequest.cpp

HEADERS  += mainwindow.h \
    ../../src/oauth/bearer.h \
    ../../src/oauth/twoleggedapi.h \
    ../../src/oss/bucket.h \
    ../../src/oss/bucketapi.h \
    ../../src/oss/bucketobject.h \
    ../../src/oss/bucketobjectapi.h \
    ../../src/shared/baseapi.h \
    ../../src/shared/httprequest.h

FORMS    += mainwindow.ui
