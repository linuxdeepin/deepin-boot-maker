include($$PWD/vendor.pri)

TEMPLATE  = app

TARGET = deepin-boot-maker

QT += core gui widgets concurrent network svg

CONFIG += c++11

RESOURCES +=  \
    ../translate.qrc \
    view/resource/resource.qrc

HEADERS += \
    view/isoselectview.h \
    view/suggestbutton.h \
    view/widgetutil.h \
    view/usbselectview.h \
    view/deviceinfoitem.h \
    view/devicelistwidget.h \
    view/progressview.h \
    view/resultview.h \
    view/dropframe.h

SOURCES += \
    view/isoselectview.cpp \
    view/suggestbutton.cpp \
    view/widgetutil.cpp \
    view/usbselectview.cpp \
    view/deviceinfoitem.cpp \
    view/devicelistwidget.cpp \
    view/progressview.cpp \
    view/resultview.cpp \
    view/dropframe.cpp \
    bmwindow.cpp \
    main.cpp

linux {

INCLUDEPATH += $$PWD/linux
HEADERS += $$PWD/linux/bmwindow.h

binary.path = $${PREFIX}/bin
binary.files = $${OUT_PWD}/deepin-boot-maker

desktop.path = $${PREFIX}/share/applications
desktop.files = $$PWD/platform/linux/deepin-boot-maker.desktop

hicolor.path =  $${PREFIX}/share/icons/hicolor/scalable/apps
hicolor.files = $$PWD/view/resource/theme/light/image/deepin-boot-maker.svg

dman.path = /usr/share/dman/
dman.files = $$PWD/dman/*

INSTALLS += binary desktop hicolor dman

}

!linux {
    INCLUDEPATH += $$PWD/other
    HEADERS += $$PWD/other/bmwindow.h
}

mac{
    ICON = $$PWD/platform/mac/deepin-boot-maker.icns

    blob.files = $$PWD/platform/mac/Contents/Resources
    blob.path = Contents
    QMAKE_BUNDLE_DATA += blob
}

win32{
    message("add admin pri")
    QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
    RC_FILE += $$PWD/platform/windows/deepin-boot-maker.rc
}


