include($$PWD/../vendor/env.pri)

QT += dtkwidget

unix{
    QT += network
    CONFIG += link_pkgconfig
}

linux{
    QT += x11extras dbus x11extras
    CONFIG += link_pkgconfig
    PKGCONFIG += dtkwidget
    PKGCONFIG += xcb-util libstartup-notification-1.0
    LIBS += -lxcb -lX11 -lXext
}

win32{
    INCLUDEPATH += $$QT.dtkcore.includes
    LIBS += -L$$QT.dtkcore.libs -ldtkcore
    INCLUDEPATH += $$QT.dtkwidget.includes
    LIBS += -L$$QT.dtkwidget.libs -ldtkwidget
}

##################################################################
# Library
##################################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libdbm/release/ -llibdbm
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libdbm/debug/ -llibdbm
else:unix: LIBS += -L$$OUT_PWD/../libdbm/ -llibdbm

INCLUDEPATH += $$PWD/../libdbm
DEPENDPATH += $$PWD/../libdbm

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libdbm/release/liblibdbm.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libdbm/debug/liblibdbm.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libdbm/release/libdbm.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libdbm/debug/libdbm.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../libdbm/liblibdbm.a

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
