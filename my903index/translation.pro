#-------------------------------------------------
#
# Project created by QtCreator 2017-07-26T08:35:00
#
#-------------------------------------------------

QT       += core gui  svg network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = translation
TEMPLATE = app
TRANSLATIONS += translation_en_AS.ts\
                translation_cn_zh_CN.ts

unix:!macx{
QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\'"
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\lib'"
}
SOURCES += main.cpp\
    myjsonbuttondata.cpp \
    mylabel.cpp \
    mytitlelabel.cpp \
    qloadingwidget.cpp \
        translationwidget.cpp \
    form.cpp \
    dialog.cpp \
    formclearfile.cpp \
    leecleartooll.cpp \
    listfilecontrol.cpp


HEADERS  += translationwidget.h \
    form.h \
    myjsonbuttondata.h \
    mylabel.h \
    mytitlelabel.h \
    qloadingwidget.h \
    dialog.h \
        formclearfile.h \
        leecleartooll.h \
        listfilecontrol.h

FORMS += \
    form.ui \
    dialog.ui \
        formclearfile.ui \
        listfilecontrol.ui

RESOURCES +=
