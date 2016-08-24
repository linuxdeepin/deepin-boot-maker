TEMPLATE = lib
VERSION = 1.0
CONFIG += c++11 create_pc create_prl no_install_prl
DEFINES += LIBDTK_LIBRARY

DEFINES += QT_MESSAGELOGCONTEXT

win32* {
    DEFINES += STATIC_LIB
    CONFIG += staticlib
    LIB_DIR =
}

isEmpty(LIB_INSTALL_DIR) {
    target.path = /usr/lib
} else {
    target.path = $$LIB_INSTALL_DIR
}

QMAKE_PKGCONFIG_LIBDIR = $$target.path
QMAKE_PKGCONFIG_VERSION = $$VERSION
QMAKE_PKGCONFIG_DESTDIR = pkgconfig

isEmpty(INCLUDE_INSTALL_DIR) {
    DTK_INCLUDEPATH = /usr/include/libdtk-$$VERSION
} else {
    DTK_INCLUDEPATH = $$INCLUDE_INSTALL_DIR/libdtk-$$VERSION
}

#unix{
    INSTALLS += includes target
#}
