DCOMMON_DIR = $$PWD/../common
include($$DCOMMON_DIR/lib.pri)
include($$PWD/dbase.pri)

QT -= gui

TARGET = dtkbase

DEFINES += LIBDTKBASE_LIBRARY

includes.path = $${DTK_INCLUDEPATH}/DBase
includes.files += *.h
includes.files += DObject

QMAKE_PKGCONFIG_NAME = DTK_BASE
QMAKE_PKGCONFIG_DESCRIPTION = Deepin Tool Kit Base Header Files
QMAKE_PKGCONFIG_INCDIR = $$includes.path
