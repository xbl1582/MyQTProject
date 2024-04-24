QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = KSTSCAN_EN
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
    capturescreen.cpp \
    folderwidget.cpp \
    imageroiwidget.cpp \
    imagesonwidget.cpp \
    imagewidget.cpp \
    main.cpp \
    mylabel.cpp \
    mywidget.cpp \
    scanner.cpp \
    scannerwidget.cpp \
    translationwidget.cpp

HEADERS += \
    capturescreen.h \
    folderwidget.h \
    imageroiwidget.h \
    imagesonwidget.h \
    imagewidget.h \
    mylabel.h \
    mywidget.h \
    scanner.h \
    scannerwidget.h \
    translationwidget.h

FORMS += \
    folderwidget.ui \
    imagewidget.ui \
    quheibian.ui \
    scanner.ui \
    scannerwidget.ui
#INCLUDEPATH +=/usr/local/include/openv4/


LIBS +=/cvlib/lib/libopencv_*.so
INCLUDEPATH +=/cvlib/include/opencv4/

TRANSLATIONS += SacnnerPage_en_en_AS.ts\
                ScannerPage_zh_cn_zh_CN.ts


#LIBS += /usr/local/lib/libopencv_*.so

#INCLUDEPATH +=/usr/local/include/opencv4/


#INCLUDEPATH += D:\App\opencv3.4\build\install\include
#LIBS += D:\App\opencv3.4\build\lib\libopencv_*.a



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=



