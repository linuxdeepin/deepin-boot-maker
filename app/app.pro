TEMPLATE  = app

TARGET = deepin-boot-maker

QT += core gui widgets

RESOURCES +=  \
    blobs.qrc \
    tranlate.qrc \
    src/ui.qrc

win32* {
    RC_FILE += src\deepin-boot-maker.rc
}

win32-msvc* {
    system(importPo.bat app.pro)
    QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
}

unix {
    system(./importPo app.pro)
}

macx {
    ICON = src/deepin-boot-maker.icns
}

SOURCES += \
    src/bootmaker.cpp \
    src/main.cpp \
    src/usbinstaller.cpp \
    src/ui/dcheckbox.cpp \
    src/ui/dclosebutton.cpp \
    src/ui/dcombobox.cpp \
    src/ui/ddevicon.cpp \
    src/ui/ddigitprogressmovie.cpp \
    src/ui/dfilechooseinput.cpp \
    src/ui/dimageicon.cpp \
    src/ui/dlistview.cpp \
    src/ui/dprogress.cpp \
    src/ui/dprogressframe.cpp \
    src/ui/dpushbutton.cpp \
    src/ui/drotemovie.cpp \
    src/ui/dserialframemovie.cpp \
    src/ui/dtips.cpp \
    src/ui/dusblist.cpp \
    src/ui/dusbprogressmovie.cpp \
    src/ui/dwindowui.cpp \
    src/utils.cpp

HEADERS += \
    src/bootmaker.h \
    src/progressrate.h \
    src/usbinstaller.h \
    src/ui/dcheckbox.h \
    src/ui/dclosebutton.h \
    src/ui/dcombobox.h \
    src/ui/ddevicon.h \
    src/ui/ddigitprogressmovie.h \
    src/ui/dfilechooseinput.h \
    src/ui/dimageicon.h \
    src/ui/dlistview.h \
    src/ui/dprogress.h \
    src/ui/dprogressframe.h \
    src/ui/dpushbutton.h \
    src/ui/drotemovie.h \
    src/ui/dserialframemovie.h \
    src/ui/dtips.h \
    src/ui/dui.h \
    src/ui/dusblist.h \
    src/ui/dusbprogressmovie.h \
    src/ui/dwindowui.h \
    src/utils.h

TRANSLATIONS += po/en_US.ts \
    po/zh_CN.ts \
    po/am.ts \
    po/cs.ts \
    po/de_DE.ts \
    po/es.ts \
    po/es_CL.ts \
    po/es_MX.ts \
    po/fr.ts \
    po/it.ts \
    po/nb.ts \
    po/pt_BR.ts \
    po/pt_PT.ts \
    po/ru.ts \
    po/tr.ts \
    po/zh_TW.ts \

#################################################
#    Auto Lib Ref
#################################################


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/libxsys/release/ -lxsys
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/libxsys/debug/ -lxsys
else:unix: LIBS += -L$$OUT_PWD/../lib/libxsys/ -lxsys

INCLUDEPATH += $$PWD/../lib/libxsys
DEPENDPATH += $$PWD/../lib/libxsys

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/libxsys/release/libxsys.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/libxsys/debug/libxsys.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/libxsys/release/xsys.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/libxsys/debug/xsys.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../lib/libxsys/libxsys.a
