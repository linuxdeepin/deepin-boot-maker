include($$PWD/../vendor/vendor.pri)

TEMPLATE  = app

TARGET = deepin-boot-maker

QT += core gui widgets concurrent network

CONFIG += c++11

DEFINES += QT_MESSAGELOGCONTEXT

RESOURCES +=  \
    blob.qrc \
    ../translate.qrc \
    view/resource/resource.qrc

HEADERS += \
    bmwindow.h \
    backend/bootmaker.h \
    view/isoselectview.h \
    util/sevenzip.h \
    backend/syslinux.h \
    backend/diskutil.h \
    util/utils.h \
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
    util/deviceinfo.h \
    bminterface.h \
    util/devicemonitor.h \
    backend/bmhandler.h

SOURCES += \
    main.cpp \
    bmwindow.cpp \
    backend/bootmaker.cpp \
    view/isoselectview.cpp \
    util/sevenzip.cpp \
    backend/syslinux.cpp \
    backend/diskutil.cpp \
    util/utils.cpp \
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
    bminterface.cpp \
    util/devicemonitor.cpp \

unix {
    HEADERS += \
        backend/bmdbusinterface.h \
        backend/bmdbushandler.h

    SOURCES += \
        backend/bmdbusinterface.cpp
}



TRANSLATIONS += $$PWD/../translations/$${TARGET}.ts \
                $$PWD/../translations/$${TARGET}_zh_CN.ts

isEmpty(PREFIX) {
    PREFIX = /usr
}

binary.path = $${PREFIX}/bin
binary.files = deepin-boot-maker

desktop.path = $${PREFIX}/share/applications
desktop.files = deepin-boot-maker.desktop

INSTALLS += binary desktop

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../vendor/src/libxsys/release/ -lxsys
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../vendor/src/libxsys/debug/ -lxsys
else:unix: LIBS += -L$$OUT_PWD/../vendor/src/libxsys/ -lxsys

INCLUDEPATH += $$PWD/../vendor/src/libxsys
DEPENDPATH += $$PWD/../vendor/src/libxsys

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../vendor/src/libxsys/release/libxsys.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../vendor/src/libxsys/debug/libxsys.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../vendor/src/libxsys/release/xsys.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../vendor/src/libxsys/debug/xsys.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../vendor/src/libxsys/libxsys.a
