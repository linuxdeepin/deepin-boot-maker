TEMPLATE = app
TARGET = deepin-boot-maker
CONFIG += static
DEPENDPATH += .
INCLUDEPATH += .

win32-msvc* {
    DEFINES += _USING_V110_SDK71_
    QMAKE_LFLAGS += /MANIFESTUAC:"level='requireAdministrator'uiAccess='false'"
    QMAKE_LFLAGS += /SUBSYSTEM:WINDOWS",5.01"
    LIBS += -llibGLESv2 -llibEGL -lpreprocessor
}

DEFINES += STATICLINUX
HEADERS += unetbootin.h \
    qml/Deepin/Widgets/plugins/dicon.h \
    qml/Deepin/Widgets/plugins/dwindow.h \
    bootmaker.h \
    qml/Deepin/Widgets/plugins/ddroparea.h \
    diskunity.h \
    xsys.h

SOURCES += main.cpp \
    unetbootin.cpp \
    qml/Deepin/Widgets/plugins/dicon.cpp \
    qml/Deepin/Widgets/plugins/dwindow.cpp \
    bootmaker.cpp \
    qml/Deepin/Widgets/plugins/ddroparea.cpp \
    diskunity.cpp \
    xsys.cpp

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

TRANSLATIONS += po/en_US.ts \
    po/zh_CN.ts \
    po/zh_TW.ts \
	po/am.ts \
	po/de_DE.ts \
	po/es_MX.ts \
	po/fr.ts \
	po/nb.ts \
	po/pt_PT.ts \
	po/tr.ts \
	po/cs.ts \
	po/es_CL.ts \
	po/es.ts \
	po/it.ts \
	po/pt_BR.ts \
	po/ru.ts \


win32{
RESOURCES += \
    deepin-boot-maker.qrc
    RC_FILE += deepin-boot-maker.rc
}else{
RESOURCES += \
    deepin-boot-maker.qrc
}

OTHER_FILES +=

RESOURCES += \
    deepin-boot-maker-qml.qrc \
    deepin-boot-maker-windows.qrc \
    deepin-boot-maker-sevnz.qrc

ICON = deepin-boot-maker.icns

