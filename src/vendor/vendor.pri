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
    DEFINES += STATIC_LIB

    INCLUDEPATH += $$PWD

    # Deepin tool kit
    DTK_INCLUDE = D:\Develop\Library\include\libdtk-1.0
    DTK_DEPEND  = D:\Develop\Library\include\libdtk-1.0
    DTK_LIBRARY = D:\Develop\Library\lib

    INCLUDEPATH += $$DTK_INCLUDE\DWidget
    DEPENDPATH  += $$DTK_INCLUDE\DWidget

    INCLUDEPATH += $$DTK_INCLUDE\DUtil
    INCLUDEPATH += $$DTK_INCLUDE\DUtil
    DEPENDPATH  += $$DTK_DEPEND\DUtil

    INCLUDEPATH += $$DTK_INCLUDE\DBase
    DEPENDPATH  += $$DTK_DEPEND\DBase

    LIBS += -L$$DTK_LIBRARY -ldtkwidget
    LIBS += -L$$DTK_LIBRARY -ldtkutil
    LIBS += -L$$DTK_LIBRARY -ldtkbase

}
