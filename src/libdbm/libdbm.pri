SOURCES += \
    $$PWD/backend/bootmaker.cpp \
    $$PWD/backend/diskutil.cpp \
    $$PWD/backend/syslinux.cpp \
    $$PWD/util/devicemonitor.cpp \
    $$PWD/util/sevenzip.cpp \
    $$PWD/util/utils.cpp \
    $$PWD/bminterface.cpp \
    $$PWD/backend/bmhandler.cpp \
    $$PWD/installer/qtbaseinstaller.cpp \
    $$PWD/installer/qtinstallerfactory.cpp \
    $$PWD/installer/qtX86Installer.cpp \
    $$PWD/installer/qtmipsinstaller.cpp

HEADERS += \
    $$PWD/backend/bmhandler.h \
    $$PWD/backend/bootmaker.h \
    $$PWD/backend/diskutil.h \
    $$PWD/backend/syslinux.h \
    $$PWD/util/deviceinfo.h \
    $$PWD/util/devicemonitor.h \
    $$PWD/util/sevenzip.h \
    $$PWD/util/utils.h \
    $$PWD/bminterface.h \
    $$PWD/installer/qtbaseinstaller.h \
    $$PWD/installer/qtinstallerfactory.h \
    $$PWD/installer/qtX86Installer.h \
    $$PWD/installer/qtmipsinstaller.h \
    $$PWD/installer/qtarminstaller.h \
    $$PWD/installer/qtswinstaller.h

linux {
HEADERS += \
    $$PWD/backend/bmdbusinterface.h \
    $$PWD/backend/bmdbushandler.h

SOURCES += \
    $$PWD/backend/bmdbusinterface.cpp
}

INCLUDEPATH += $$PWD/

linux {
    RESOURCES += $$PWD/blob_linux.qrc
} else {
    RESOURCES += $$PWD/blob.qrc
}
