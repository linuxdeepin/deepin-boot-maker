include($$PWD/../vendor/env.pri)


versionAtLeast(QT_VERSION, 6.0.0) {
unix{
    QT += dbus
    CONFIG += link_pkgconfig
    PKGCONFIG += dtk6core
}

win32{
    INCLUDEPATH += $$DTK_INCLUDE\DCore
    DEPENDPATH  += $$DTK_DEPEND\DCore
    LIBS += -L$$DTK_LIBRARY -ldtkcore
}

} else {
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
}

