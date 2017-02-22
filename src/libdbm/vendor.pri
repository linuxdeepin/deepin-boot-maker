include($$PWD/../vendor/env.pri)

unix{
    QT += dbus
    CONFIG += link_pkgconfig
    PKGCONFIG += dtkbase dtkutil
}

win32{
    INCLUDEPATH += $$DTK_INCLUDE\DUtil
    INCLUDEPATH += $$DTK_INCLUDE\DUtil
    DEPENDPATH  += $$DTK_DEPEND\DUtil

    INCLUDEPATH += $$DTK_INCLUDE\DBase
    DEPENDPATH  += $$DTK_DEPEND\DBase

    LIBS += -L$$DTK_LIBRARY -ldtkutil
    LIBS += -L$$DTK_LIBRARY -ldtkbase
}

##################################################################
# Library
##################################################################
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
