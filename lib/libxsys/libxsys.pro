#-------------------------------------------------
#
# Project created by QtCreator 2015-01-28T13:59:02
#
#-------------------------------------------------

QT       -= gui

TARGET = xsys
TEMPLATE = lib
CONFIG += staticlib

win32* {
    DESTDIR = ./
}

CONFIG(debug, debug|release) {
    TARGET = xsys
    OBJECTS_DIR = .build/debug/.obj
    MOC_DIR = .build/debug/.moc
    RCC_DIR = .build/debug/.rcc
    UI_DIR = .build/debug/.ui
} else {
    OBJECTS_DIR = .build/release/.obj
    MOC_DIR = .build/release/.moc
    RCC_DIR = .build/release/.rcc
    UI_DIR = .build/release/.ui
}

SOURCES += DiskUtil/DiskUtil.cpp \
    Common/Result.cpp \
    Cmd/Cmd.cpp \
    FileSystem/FileSystem.cpp

HEADERS +=     XSys \
    DiskUtil/DiskUtil.h \
    Common/Result.h \
    Cmd/Cmd.h \
    FileSystem/FileSystem.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


