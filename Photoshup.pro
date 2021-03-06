QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT       += charts

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controlswrapper.cpp \
    histogram.cpp \
    image.cpp \
    imagewindow.cpp \
    main.cpp \
    mainwindow.cpp \
    pixel.cpp

HEADERS += \
    controlswrapper.h \
    histogram.h \
    image.h \
    imagewindow.h \
    mainwindow.h \
    pixel.h \
    stb_image.h \
    stb_image_write.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    Photoshup_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
