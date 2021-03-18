include($$PWD/../vendor/env.pri)

unix{
    QT += dbus
    CONFIG += link_pkgconfig
    PKGCONFIG += dtkcore
}

win32{
    INCLUDEPATH += $$DTK_INCLUDE\DCore
    DEPENDPATH  += $$DTK_DEPEND\DCore
    LIBS += -L$$DTK_LIBRARY -ldtkcore
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
