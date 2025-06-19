#-------------------------------------------------
#
# Project created by QtCreator 2015-01-28T13:59:02
#
#-------------------------------------------------

QT       -= gui

TARGET = xsys
TEMPLATE = lib
CONFIG += staticlib

#添加安全编译参数
QMAKE_LFLAGS += -z noexecstack -pie -z relro -z now
QMAKE_CFLAGS += -fstack-protector-all -fPIC
QMAKE_CXXFLAGS += -fstack-protector-all -fPIC

DEFINES += QT_MESSAGELOGCONTEXT

SOURCES += \
    DiskUtil/DiskUtil.cpp \
    Common/Result.cpp \
    Cmd/Cmd.cpp \
    FileSystem/FileSystem.cpp \
    DiskUtil/Syslinux.cpp

HEADERS += \
    XSys \
    DiskUtil/DiskUtil.h \
    Common/Result.h \
    Cmd/Cmd.h \
    FileSystem/FileSystem.h \
    DiskUtil/Syslinux.h

unix {
isEmpty(PREFIX){
    PREFIX = /usr
}
target.path = $${PREFIX}/lib/
}


