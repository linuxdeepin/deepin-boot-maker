#-------------------------------------------------
#
# Project created by QtCreator 2015-01-28T13:59:02
#
#-------------------------------------------------

QT       -= gui

TARGET = xsys
TEMPLATE = lib
CONFIG += staticlib

SOURCES += DiskUtil/DiskUtil.cpp \
    Common/Result.cpp \
    Cmd/Cmd.cpp \
    FileSystem/FileSystem.cpp \
    DiskUtil/Syslinux.cpp

HEADERS +=     XSys \
    DiskUtil/DiskUtil.h \
    Common/Result.h \
    Cmd/Cmd.h \
    FileSystem/FileSystem.h \
    DiskUtil/Syslinux.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


