CONFIG += link_pkgconfig

DEFINES += QT_MESSAGELOGCONTEXT

unix {
isEmpty(PREFIX) {
    PREFIX = /usr
}
}

win32{
    DEFINES += DTK_STATIC_LIB
}
NO_BOOTLOADER=FALSE
ARCH = $$QMAKE_HOST.arch
isEqual(ARCH, sw_64) | isEqual(ARCH, mips64) | isEqual(ARCH, mips32) {
    NO_BOOTLOADER=TRUE
}

isEqual(NO_BOOTLOADER, TRUE) {
    DEFINES += DBM_NO_BOOTLOADER
}
