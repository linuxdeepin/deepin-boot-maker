include($$PWD/vendor.pri)

DEFINES += QT_MESSAGELOGCONTEXT

TEMPLATE  = app

TARGET = deepin-boot-maker

QT += core gui widgets concurrent network

CONFIG += c++11

RESOURCES +=  \
    ../translate.qrc \
    view/resource/resource.qrc

HEADERS += \
    view/isoselectview.h \
    view/setepindicatorbar.h \
    view/suggestbutton.h \
    view/widgetutil.h \
    view/usbselectview.h \
    view/deviceinfoitem.h \
    view/devicelistwidget.h \
    view/dwaterprogress.h \
    view/progressview.h \
    view/resultview.h \
    view/dropframe.h \
    bmwindow.h \

SOURCES += \
    view/isoselectview.cpp \
    view/setepindicatorbar.cpp \
    view/suggestbutton.cpp \
    view/widgetutil.cpp \
    view/usbselectview.cpp \
    view/deviceinfoitem.cpp \
    view/devicelistwidget.cpp \
    view/dwaterprogress.cpp \
    view/progressview.cpp \
    view/resultview.cpp \
    view/dropframe.cpp \
    bmwindow.cpp \
    main.cpp

linux {

binary.path = $${PREFIX}/bin
binary.files = deepin-boot-maker

desktop.path = $${PREFIX}/share/applications
desktop.files = $$PWD/platform/linux/deepin-boot-maker.desktop

INSTALLS += binary desktop
}

win {
    QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
    RC_FILE += $$PWD/platform/windows/deepin-boot-maker.rc
}


