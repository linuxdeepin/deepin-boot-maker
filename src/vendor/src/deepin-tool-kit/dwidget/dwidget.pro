include($$PWD/../common/lib.pri)
include(widgets/widgets.pri)

TARGET = dtkwidget

DEFINES += LIBDTKWIDGET_LIBRARY

QT += multimedia multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

unix{
    QT += x11extras dbus
}



HEADERS += dwidget_global.h
includes.path = $${DTK_INCLUDEPATH}/DWidget
includes.files += dwidget_global.h widgets/*.h
unix{
    includes.files += $$PWD/platforms/linux/*.h
}
win32* {
    includes.files += $$PWD/platforms/windows/*.h
}
includes.files += widgets/DTitlebar \
    widgets/DAboutDialog \
    widgets/DWindow \
    widgets/DMenu \
    widgets/DApplication \
    widgets/DGraphicsDropShadowEffect \
    widgets/DAction

QMAKE_PKGCONFIG_NAME = DTK_WIDGET
QMAKE_PKGCONFIG_DESCRIPTION = Deepin Tool Kit UI Module
QMAKE_PKGCONFIG_INCDIR = $$includes.path

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../dbase/release/ -ldtkbase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../dbase/debug/ -ldtkbase
else:unix: LIBS += -L$$OUT_PWD/../dbase/ -ldtkbase

INCLUDEPATH += $$PWD/../dbase
DEPENDPATH += $$PWD/../dbase
INCLUDEPATH += $$PWD/../dutil
DEPENDPATH += $$PWD/../dutil
