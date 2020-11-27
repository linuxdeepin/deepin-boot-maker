include($$PWD/vendor.pri)

QT       += network
QT       -= gui

TARGET = libdbm
TEMPLATE = lib
CONFIG += staticlib c++11 link_pkgconfig

SOURCES += \
    backend/bootmaker.cpp \
    backend/diskutil.cpp \
    backend/syslinux.cpp \
    util/devicemonitor.cpp \
    util/sevenzip.cpp \
    util/utils.cpp \
    bminterface.cpp \
    backend/qtbaseinstaller.cpp \
    backend/qtlinuxinstaller.cpp \
    backend/bmhandler.cpp

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
    backend/qtbaseinstaller.h \
    backend/qtlinuxinstaller.h

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
binary.files = $$PWD/qrc/xfbinst_x64 \
               $$PWD/qrc/xfbinst_x32
INSTALLS += binary desktop hicolor
}
}
}

linux {
    RESOURCES += blob_linux.qrc
} else {
    RESOURCES += blob.qrc
}
