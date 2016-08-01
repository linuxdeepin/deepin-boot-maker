CONFIG += c++11 link_pkgconfig
PKGCONFIG += xcb-util libstartup-notification-1.0

INCLUDEPATH += $$PWD
DEFINES += SN_API_NOT_YET_FROZEN

HEADERS += \
    $$PWD/startupnotificationmonitor.h

SOURCES += \
    $$PWD/startupnotificationmonitor.cpp
