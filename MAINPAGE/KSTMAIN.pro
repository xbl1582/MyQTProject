QT       += core gui network concurrent sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    app.cpp \
    applist.cpp \
    apppathspojo.cpp \
    apps.cpp \
    appsitem.cpp \
    appsiteminfo.cpp \
    appsversion.cpp \
    checkipuse.cpp \
    driverpojo.cpp \
    empowerpojo.cpp \
    httpclient.cpp \
    ipservicepojo.cpp \
    loadingwidget.cpp \
    logitem.cpp \
    main.cpp \
    mainwindow.cpp \
    more.cpp \
    mythread.cpp \
    shop.cpp \
    shopitem.cpp \
    systemsetting.cpp \
    dialog.cpp \
    zipobj.cpp

HEADERS += \
    app.h \
    applist.h \
    apppathspojo.h \
    apps.h \
    appsitem.h \
    appsiteminfo.h \
    appsversion.h \
    checkipuse.h \
    driverpojo.h \
    empowerpojo.h \
    httpclient.h \
    ipservicepojo.h \
    loadingwidget.h \
    logitem.h \
    mainwindow.h \
    more.h \
    mythread.h \
    shop.h \
    shopitem.h \
    systemsetting.h \
    dialog.h \
    zipobj.h

FORMS += \
    app.ui \
    apps.ui \
    appsitem.ui \
    appsiteminfo.ui \
    loadingwidget.ui \
    logitem.ui \
    mainwindow.ui \
    more.ui \
    shop.ui \
    shopitem.ui \
    systemsetting.ui \
    dialog.ui

TRANSLATIONS += \
    KSTMAIN_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_FILE += KSTMAIN.rc
OTHER_FILES+= \KSTMAIN.rc
ICON = KSTMAIN.rc
#unix:!macx: LIBS += -L$$PWD/lib/ -lquazip

#INCLUDEPATH += $$PWD/.
#DEPENDPATH += $$PWD/.

#unix:!macx: LIBS += -L$$PWD/lib/quazip/ -lz

#INCLUDEPATH += $$PWD/lib/quazip
#DEPENDPATH += $$PWD/lib/quazip

#unix:!macx: LIBS += -L$$PWD/lib/quazip/ -lquazip

#INCLUDEPATH += $$PWD/lib/quazip
#DEPENDPATH += $$PWD/lib/quazip

#unix:!macx: LIBS += -L$$PWD/lib/quazip/ -lz

#INCLUDEPATH += $$PWD/lib/quazip
#DEPENDPATH += $$PWD/lib/quazip

#unix:!macx: PRE_TARGETDEPS += $$PWD/lib/quazip/libz.a

DISTFILES += \
    KSTMAIN.ico

RESOURCES += \
    res.qrc
TRANSLATIONS+=KSTMAIN_zh_CN.ts\
              KSTMAIN_en_AS.ts
