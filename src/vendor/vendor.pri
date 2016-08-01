#extralib.target = extra
#extralib.commands = $$PWD/prebuild;
#extralib.depends =
#QMAKE_EXTRA_TARGETS += extralib
#PRE_TARGETDEPS = extra

unix{
    QT += x11extras dbus  network x11extras
    CONFIG += c++11 link_pkgconfig
    PKGCONFIG += xcb-util libstartup-notification-1.0
    LIBS += -lxcb -lX11 -lXext
}

QMAKE_CXXFLAGS += $$system(env PKG_CONFIG_PATH=$$PWD/lib/pkgconfig pkg-config --cflags dtkbase)
LIBS +=  $$system(env PKG_CONFIG_PATH=$$PWD/lib/pkgconfig pkg-config --libs dtkbase)
QMAKE_CXXFLAGS += $$system(env PKG_CONFIG_PATH=$$PWD/lib/pkgconfig pkg-config --cflags dtkutil)
LIBS +=  $$system(env PKG_CONFIG_PATH=$$PWD/lib/pkgconfig pkg-config --libs dtkutil)
QMAKE_CXXFLAGS += $$system(env PKG_CONFIG_PATH=$$PWD/lib/pkgconfig pkg-config --cflags dtkwidget)
LIBS +=  $$system(env PKG_CONFIG_PATH=$$PWD/lib/pkgconfig pkg-config --libs dtkwidget)
