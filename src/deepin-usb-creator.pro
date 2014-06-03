TEMPLATE = app
TARGET = deepin-usb-creator

DEPENDPATH += .
INCLUDEPATH += .


HEADERS += unetbootin.h \
    usbcreator.h \
    qml/Deepin/Widgets/plugins/dicon.h \
    qml/Deepin/Widgets/plugins/dwindow.h

SOURCES += main.cpp \
    unetbootin.cpp \
    usbcreator.cpp \
    qml/Deepin/Widgets/plugins/dicon.cpp \
    qml/Deepin/Widgets/plugins/dwindow.cpp

CONFIG(lupdate) {
    SOURCES += qml/mainui.qml
}

QT += core gui quick widgets

ICON =

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

TRANSLATIONS += deepin-usb-creator_en_US.ts \
    deepin-usb-creator_zh_CN.ts \
    deepin-usb-creator_zh_TW.ts \

win32{
RESOURCES += \
    deepin-usb-creator.qrc \
    unetbootin-windows.qrc \
    unetbootin-sevnz.qrc
}else{
RESOURCES += \
    deepin-usb-creator.qrc
}
OTHER_FILES +=

RESOURCES += \
    deepin-usb-creator-qml.qrc
