include($$PWD/../vendor/env.pri)

unix{
    QT += dbus
    CONFIG += link_pkgconfig
    PKGCONFIG += dtkcore
}

win32{
    INCLUDEPATH += $$DTK_INCLUDE\DCore
    DEPENDPATH  += $$DTK_DEPEND\DCore
    LIBS += -L$$DTK_LIBRARY -ldtkcore
}