TEMPLATE = app
TARGET = deepin-boot-maker

QT += core gui widgets

CONFIG(debug, debug|release) {
    DESTDIR = ./
    OBJECTS_DIR = .build/debug/.obj
    MOC_DIR = .build/debug/.moc
    RCC_DIR = .build/debug/.rcc
    UI_DIR = .build/debug/.ui
} else {
    DESTDIR = ./
    OBJECTS_DIR = .build/release/.obj
    MOC_DIR = .build/release/.moc
    RCC_DIR = .build/release/.rcc
    UI_DIR = .build/release/.ui
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
    src/deepin-boot-maker.qrc \
    src/deepin-boot-maker-blobs.qrc

win32* {
    RESOURCES += src/deepin-boot-maker-windows.qrc
    DEFINES += _USING_V110_SDK71_
}

win32-msvc* {
    system(importPo.bat deepin-boot-maker.pro)
    QMAKE_LFLAGS += /MANIFESTUAC:"level='requireAdministrator'uiAccess='false'"
    QMAKE_LFLAGS += /SUBSYSTEM:WINDOWS",5.01"
}

win32-g++* {
    system(./importPo)
    RC_FILE += deepin-boot-maker-manifest.rc
}

macx {
    CONFIG += static
    ICON = deepin-boot-maker.icns
}

unix {
    system(./importPo)
}

SOURCES += \
    src/bootmaker.cpp \
    src/diskunity.cpp \
    src/main.cpp \
    src/usbinstaller.cpp \
    src/xsys.cpp \
    src/ui/dcheckbox.cpp \
    src/ui/dclosebutton.cpp \
    src/ui/dcombobox.cpp \
    src/ui/ddevicon.cpp \
    src/ui/ddigitprogressmovie.cpp \
    src/ui/dfilechooseinput.cpp \
    src/ui/dimageicon.cpp \
    src/ui/dlistview.cpp \
    src/ui/dprogress.cpp \
    src/ui/dprogressframe.cpp \
    src/ui/dpushbutton.cpp \
    src/ui/drotemovie.cpp \
    src/ui/dserialframemovie.cpp \
    src/ui/dtips.cpp \
    src/ui/dusblist.cpp \
    src/ui/dusbprogressmovie.cpp \
    src/ui/dwindowui.cpp

HEADERS += \
    src/bootmaker.h \
    src/diskunity.h \
    src/progressrate.h \
    src/usbinstaller.h \
    src/xsys.h \
    src/ui/dcheckbox.h \
    src/ui/dclosebutton.h \
    src/ui/dcombobox.h \
    src/ui/ddevicon.h \
    src/ui/ddigitprogressmovie.h \
    src/ui/dfilechooseinput.h \
    src/ui/dimageicon.h \
    src/ui/dlistview.h \
    src/ui/dprogress.h \
    src/ui/dprogressframe.h \
    src/ui/dpushbutton.h \
    src/ui/drotemovie.h \
    src/ui/dserialframemovie.h \
    src/ui/dtips.h \
    src/ui/dui.h \
    src/ui/dusblist.h \
    src/ui/dusbprogressmovie.h \
    src/ui/dwindowui.h

