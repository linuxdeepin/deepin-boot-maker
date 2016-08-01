
unix {
    include($$PWD/x11/x11.pri)
    include($$PWD/linux/linux.pri)
}

win32* {
    include($$PWD/windows/windows.pri)
}
