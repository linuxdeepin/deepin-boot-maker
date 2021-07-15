TEMPLATE  = subdirs
CONFIG   += ordered
SUBDIRS  += \
    src  \

linux {
manual_dir.files = $$PWD/src/help/deepin-boot-maker
manual_dir.path = /usr/share/deepin-manual/manual-assets/application
INSTALLS += manual_dir
}

contains(QMAKE_HOST.arch, "sw_64") {
    DEFINES = NO_SUPPORT_ASAN
}

contains(QMAKE_HOST.arch, "mips64") {
    DEFINES = NO_SUPPORT_ASAN
}
