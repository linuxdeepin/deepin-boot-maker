TEMPLATE = app
TARGET = deepin-boot-maker

DEPENDPATH += .
INCLUDEPATH += .

DEFINES += STATICLINUX
DEFINES += NOEXTRACTKERNEL
DEFINES += NOEXTRACTINITRD

macx {
CONFIG += static
ICON = deepin-boot-maker.icns
}

HEADERS += unetbootin.h \
    bootmaker.h \
    diskunity.h \
    xsys.h \
    ui/dwindowui.h \
    ui/dcombobox.h \
    ui/dfilechooseinput.h \
    ui/dcheckbox.h \
    ui/dpushbutton.h \
    ui/droteanimation.h

SOURCES += main.cpp \
    unetbootin.cpp \
    bootmaker.cpp \
    diskunity.cpp \
    xsys.cpp \
    ui/dwindowui.cpp \
    ui/dcombobox.cpp \
    ui/dfilechooseinput.cpp \
    ui/dcheckbox.cpp \
    ui/dpushbutton.cpp \
    ui/droteanimation.cpp

QT += core gui widgets

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

TRANSLATIONS += po/en_US.ts \
    po/zh_CN.ts \
    po/am.ts \
    po/cs.ts \
    po/de_DE.ts \
    po/es.ts \
    po/es_CL.ts \
    po/es_MX.ts \
    po/fr.ts \
    po/it.ts \
    po/nb.ts \
    po/pt_BR.ts \
    po/pt_PT.ts \
    po/ru.ts \
    po/tr.ts \
    po/zh_TW.ts \


RESOURCES += \
    deepin-boot-maker.qrc \
    deepin-boot-maker-bootloader.qrc

win32{
RESOURCES += \
    deepin-boot-maker-windows.qrc \

RC_FILE += deepin-boot-maker.rc

}

win32-msvc* {
    DEFINES += _USING_V110_SDK71_
    QMAKE_LFLAGS += /MANIFESTUAC:"level='requireAdministrator'uiAccess='false'"
    QMAKE_LFLAGS += /SUBSYSTEM:WINDOWS",5.01"
}

OTHER_FILES +=


