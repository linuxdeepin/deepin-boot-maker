#extralib.target = extra
#extralib.commands = $$PWD/prebuild;
#extralib.depends =
#QMAKE_EXTRA_TARGETS += extralib
#PRE_TARGETDEPS = extra

DEFINES += STATIC_LIB

unix{
    QT += x11extras dbus  network x11extras
    CONFIG += c++11 link_pkgconfig
#    PKGCONFIG += dtkbase dtkutil dtkwidget
    PKGCONFIG += xcb-util libstartup-notification-1.0
    LIBS += -lxcb -lX11 -lXext

    QMAKE_CXXFLAGS += $$system(env PKG_CONFIG_PATH=$$PWD/usr/lib/pkgconfig pkg-config --cflags dtkbase)
    LIBS +=  $$system(env PKG_CONFIG_PATH=$$PWD/usr/lib/pkgconfig pkg-config --libs dtkbase)
    QMAKE_CXXFLAGS += $$system(env PKG_CONFIG_PATH=$$PWD/usr/lib/pkgconfig pkg-config --cflags dtkutil)
    LIBS +=  $$system(env PKG_CONFIG_PATH=$$PWD/usr/lib/pkgconfig pkg-config --libs dtkutil)
    QMAKE_CXXFLAGS += $$system(env PKG_CONFIG_PATH=$$PWD/usr/lib/pkgconfig pkg-config --cflags dtkwidget)
    LIBS +=  $$system(env PKG_CONFIG_PATH=$$PWD/usr/lib/pkgconfig pkg-config --libs dtkwidget)
}

win32{

    INCLUDEPATH += $$PWD/usr/include/libdtk-1.0/DWidget
    DEPENDPATH += $$PWD/usr/include/libdtk-1.0/DWidget
    LIBS += -L$$PWD/usr/lib/ -ldtkwidget

    INCLUDEPATH += $$PWD/usr/include/libdtk-1.0/DUtil
    DEPENDPATH += $$PWD/usr/include/libdtk-1.0/DUtil
    LIBS += -L$$PWD/usr/lib/ -ldtkutil

    INCLUDEPATH += $$PWD/usr/include/libdtk-1.0/DBase
    DEPENDPATH += $$PWD/usr/include/libdtk-1.0/DBase
    LIBS += -L$$PWD/usr/lib/ -ldtkbase
}
