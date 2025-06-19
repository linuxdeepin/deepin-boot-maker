include($$PWD/vendor.pri)

QT       += network
QT       -= gui

TARGET = libdbm
TEMPLATE = lib
CONFIG += staticlib c++11 link_pkgconfig

#添加安全编译参数
QMAKE_LFLAGS += -z noexecstack -pie -z relro -z now
QMAKE_CFLAGS += -fstack-protector-all -fPIC
QMAKE_CXXFLAGS += -fstack-protector-all -fPIC



SOURCES += \
    backend/bootmaker.cpp \
    backend/diskutil.cpp \
    backend/syslinux.cpp \
    util/devicemonitor.cpp \
    util/sevenzip.cpp \
    util/utils.cpp \
    bminterface.cpp \
    backend/bmhandler.cpp \
    installer/qtbaseinstaller.cpp \
    installer/qtinstallerfactory.cpp \
    installer/qtX86Installer.cpp \
    installer/qtmipsinstaller.cpp

HEADERS += \
    backend/bmhandler.h \
    backend/bootmaker.h \
    backend/diskutil.h \
    backend/syslinux.h \
    util/deviceinfo.h \
    util/devicemonitor.h \
    util/sevenzip.h \
    util/utils.h \
    bminterface.h \
    installer/qtbaseinstaller.h \
    installer/qtinstallerfactory.h \
    installer/qtX86Installer.h \
    installer/qtmipsinstaller.h \
    installer/qtarminstaller.h \
    installer/qtswinstaller.h

linux {
HEADERS += \
    backend/bmdbusinterface.h \
    backend/bmdbushandler.h

SOURCES += \
    backend/bmdbusinterface.cpp
}

unix {
    target.path = /usr/lib
#    INSTALLS += target
}

linux {
isEqual(ARCH, i386) | isEqual(ARCH, x86_64){
linux {
binary.path = $${PREFIX}/bin
INSTALLS += binary desktop hicolor
}
}
}

linux {
    RESOURCES += blob_linux.qrc
} else {
    RESOURCES += blob.qrc
}
