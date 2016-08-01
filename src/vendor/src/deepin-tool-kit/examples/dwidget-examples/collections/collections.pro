QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = collections
TEMPLATE = app
CONFIG += c++11

DEFINES += QT_MESSAGELOGCONTEXT

unix{
    QT += x11extras dbus
    CONFIG += link_pkgconfig
}
DEFINES += STATIC_LIB

SOURCES += main.cpp\
    mainwindow.cpp \
    buttontab.cpp \
    linetab.cpp \
    bartab.cpp \
    inputtab.cpp \
    slidertab.cpp \
    buttonlisttab.cpp \
    segmentedcontrol.cpp\
    indicatortab.cpp \
    widgetstab.cpp \
    comboboxtab.cpp \
    buttongridtab.cpp \
    imagebuttongridtab.cpp \
    cameraform.cpp \
    graphicseffecttab.cpp

HEADERS  += mainwindow.h \
    buttontab.h \
    linetab.h \
    bartab.h \
    inputtab.h \
    slidertab.h \
    buttonlisttab.h \
    segmentedcontrol.h \
    indicatortab.h \
    widgetstab.h \
    comboboxtab.h \
    buttongridtab.h \
    imagebuttongridtab.h \
    cameraform.h \
    graphicseffecttab.h

RESOURCES += \
    images.qrc

FORMS += \
    cameraform.ui

unix{
    INCLUDEPATH += $$PWD/../../../dwidget/platforms/linux
}
win32* {
    INCLUDEPATH += $$PWD/../../../dwidget/platforms/windows
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../dbase/release/ -ldtkbase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../dbase/debug/ -ldtkbase
else:unix: LIBS += -L$$OUT_PWD/../../../dbase/ -ldtkbase

INCLUDEPATH += $$PWD/../../../dbase
DEPENDPATH += $$PWD/../../../dbase

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../dwidget/release/ -ldtkwidget
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../dwidget/debug/ -ldtkwidget
else:unix: LIBS += -L$$OUT_PWD/../../../dwidget/ -ldtkwidget

INCLUDEPATH += $$PWD/../../../dwidget
INCLUDEPATH += $$PWD/../../../dwidget/widgets
DEPENDPATH += $$PWD/../../../dwidget

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../dutil/release/ -ldtkutil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../dutil/debug/ -ldtkutil
else:unix: LIBS += -L$$OUT_PWD/../../../dutil/ -ldtkutil

INCLUDEPATH += $$PWD/../../../dutil
INCLUDEPATH += $$PWD/../../../dutil/dlog
DEPENDPATH += $$PWD/../../../dutil
