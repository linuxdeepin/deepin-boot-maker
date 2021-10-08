include($$PWD/vendor.pri)
QT += core dbus
QT -= gui

CONFIG += c++11

TARGET = deepin-boot-maker-service
CONFIG += console link_pkgconfig
CONFIG -= app_bundle

#添加安全编译参数
CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += -fstack-protector-strong -D_FORTITY_SOURCE=1 \
    -z noexecstack -pie -fPIC -z lazy
}

TEMPLATE = app

RESOURCES +=  \
    ../translate.qrc \

SOURCES += main.cpp \
    bootmakerservice.cpp \
    LogManager.cpp

HEADERS += \
    bootmakerservice.h \
    LogManager.h

unix{
binary.path = $${PREFIX}/lib/deepin-daemon/
binary.files = $${OUT_PWD}/deepin-boot-maker-service

service.path = $${PREFIX}/share/dbus-1/system-services/
service.files = $$PWD/data/com.deepin.bootmaker.service

dbus.path = $${PREFIX}/share/dbus-1/system.d/
dbus.files = $$PWD/data/com.deepin.bootmaker.conf

INSTALLS += service dbus binary
}
