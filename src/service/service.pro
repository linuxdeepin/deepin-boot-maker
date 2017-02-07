QT += core dbus
QT -= gui

CONFIG += c++11

TARGET = deepin-boot-maker-service
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

RESOURCES +=  \
    ../app/blob.qrc \

SOURCES += main.cpp \
    bootmakerservice.cpp \
    ../app/util/devicemonitor.cpp \
    ../app/backend/bootmaker.cpp \
    ../app/util/sevenzip.cpp \
    ../app/util/utils.cpp

HEADERS += \
    bootmakerservice.h \
    ../app/util/deviceinfo.h \
    ../app/util/devicemonitor.h \
    ../app/backend/bootmaker.h \
    ../app/util/sevenzip.h \
    ../app/util/utils.h \
    ../app/backend/bmhandler.h

unix{
    CONFIG += c++11 link_pkgconfig
    PKGCONFIG += dtkbase dtkutil polkit-qt5-1
}

isEmpty(PREFIX) {
    PREFIX = /usr
}

binary.path = $${PREFIX}/lib/deepin-daemon/
binary.files = deepin-boot-maker-service

service.path = $${PREFIX}/share/dbus-1/system-services/
service.files = $$PWD/data/com.deepin.bootmaker.service

dbus.path = $${PREFIX}/share/dbus-1/system.d/
dbus.files = $$PWD/data/com.deepin.bootmaker.conf

INSTALLS += service dbus binary

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
