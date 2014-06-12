TEMPLATE = app
TARGET = deepin-boot-maker
CONFIG += static
DEPENDPATH += .
INCLUDEPATH += .

HEADERS += unetbootin.h \
    qml/Deepin/Widgets/plugins/dicon.h \
    qml/Deepin/Widgets/plugins/dwindow.h \
    bootmaker.h \
    qml/Deepin/Widgets/plugins/ddroparea.h

SOURCES += main.cpp \
    unetbootin.cpp \
    qml/Deepin/Widgets/plugins/dicon.cpp \
    qml/Deepin/Widgets/plugins/dwindow.cpp \
    bootmaker.cpp \
    qml/Deepin/Widgets/plugins/ddroparea.cpp

CONFIG(lupdate) {
    SOURCES += qml/MainUI/UsbCreatorFrame.qml
}

QT += core gui quick widgets

CONFIG(debug, debug|release) {
    DESTDIR = ../build/debug
    OBJECTS_DIR = ../build/debug/.obj
    MOC_DIR = ../build/debug/.moc
    RCC_DIR = ../build/debug/.rcc
    UI_DIR = ../build/debug/.ui
} else {
    DESTDIR = ../build/release
    OBJECTS_DIR = ../build/release/.obj
    MOC_DIR = ../build/release/.moc
    RCC_DIR = ../build/release/.rcc
    UI_DIR = ../build/release/.ui
}

TRANSLATIONS += deepin-boot-maker_en_US.ts \
    deepin-boot-maker_zh_CN.ts

win32{
RESOURCES += \
    deepin-boot-maker.qrc \
    unetbootin-windows.qrc \
    unetbootin-sevnz.qrc
    RC_FILE += deepin-boot-maker.rc
}else{
RESOURCES += \
    deepin-boot-maker.qrc
}

OTHER_FILES +=

RESOURCES += \
    deepin-boot-maker-qml.qrc

ICON = deepin-boot-maker.icns

