CONFIG += link_pkgconfig

DEFINES += QT_MESSAGELOGCONTEXT

unix {
isEmpty(PREFIX) {
    PREFIX = /usr
}
}

win32{
DEFINES += STATIC_LIB

INCLUDEPATH += $$PWD

# Deepin tool kit
DTK_INCLUDE = D:\Develop\Library\include\libdtk-1.0
DTK_DEPEND  = D:\Develop\Library\include\libdtk-1.0
DTK_LIBRARY = D:\Develop\Library\lib
}

NO_BOOTLOADER=FALSE
ARCH = $$QMAKE_HOST.arch
isEqual(ARCH, sw_64) | isEqual(ARCH, mips64) | isEqual(ARCH, mips32) {
    NO_BOOTLOADER=TRUE
}

isEqual(NO_BOOTLOADER, TRUE) {
    DEFINES += DBM_NO_BOOTLOADER
}
