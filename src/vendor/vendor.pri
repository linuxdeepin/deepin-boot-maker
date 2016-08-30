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

QMAKE_CXXFLAGS += $$system(env PKG_CONFIG_PATH=$$PWD/usr/lib/pkgconfig pkg-config --cflags dtkbase)
LIBS +=  $$system(env PKG_CONFIG_PATH=$$PWD/usr/lib/pkgconfig pkg-config --libs dtkbase)
QMAKE_CXXFLAGS += $$system(env PKG_CONFIG_PATH=$$PWD/usr/lib/pkgconfig pkg-config --cflags dtkutil)
LIBS +=  $$system(env PKG_CONFIG_PATH=$$PWD/usr/lib/pkgconfig pkg-config --libs dtkutil)
QMAKE_CXXFLAGS += $$system(env PKG_CONFIG_PATH=$$PWD/usr/lib/pkgconfig pkg-config --cflags dtkwidget)
LIBS +=  $$system(env PKG_CONFIG_PATH=$$PWD/usr/lib/pkgconfig pkg-config --libs dtkwidget)


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/usr/lib/release/ -lxsys
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/usr/lib/debug/ -lxsys
else:unix: LIBS += -L$$PWD/usr/lib/ -lxsys

INCLUDEPATH += $$PWD/src/libxsys
DEPENDPATH += $$PWD/src/libxsys

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/usr/lib/release/libxsys.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/usr/lib/debug/libxsys.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/usr/lib/release/xsys.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/usr/lib/debug/xsys.lib
else:unix: PRE_TARGETDEPS += $$PWD/usr/lib/libxsys.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/usr/lib/release/ -ldsmipc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/usr/lib/debug/ -ldsmipc
else:unix: LIBS += -L$$PWD/usr/lib/ -ldsmipc

INCLUDEPATH += $$PWD/src/smipc
DEPENDPATH += $$PWD/src/smipc

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/usr/lib/release/libdsmipc.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/usr/lib/debug/libdsmipc.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/usr/lib/release/dsmipc.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/usr/lib/debug/dsmipc.lib
else:unix: PRE_TARGETDEPS += $$PWD/usr/lib/libdsmipc.a
