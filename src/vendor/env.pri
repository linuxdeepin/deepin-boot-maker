CONFIG += link_pkgconfig

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
