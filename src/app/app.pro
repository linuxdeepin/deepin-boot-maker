include($$PWD/../vendor/vendor.pri)

TEMPLATE  = app

TARGET = deepin-boot-maker

QT += core gui widgets concurrent

CONFIG += c++11 link_pkgconfig

PKGCONFIG += dtkbase dtkutil dtkwidget

DEFINES += QT_MESSAGELOGCONTEXT

RESOURCES +=  \
    blob.qrc \
#    tranlate.qrc \
    view/resource/resource.qrc

HEADERS += \
    bmwindow.h \
    backend/bootmaker.h \
    view/isoselectview.h \
    util/sevenzip.h \
    backend/syslinux.h \
    backend/diskutil.h \
    util/utils.h \
    util/bmreporter.h \
    view/setepindicatorbar.h \
    view/suggestbutton.h \
    view/widgetutil.h \
    view/usbselectview.h \
    view/deviceinfoitem.h \
    view/devicelistwidget.h \
    view/dwaterprogress.h \
    view/progressview.h \
    view/resultview.h

SOURCES += \
    main.cpp \
    bmwindow.cpp \
    backend/bootmaker.cpp \
    view/isoselectview.cpp \
    util/sevenzip.cpp \
    backend/syslinux.cpp \
    backend/diskutil.cpp \
    util/utils.cpp \
    util/bmreporter.cpp \
    view/setepindicatorbar.cpp \
    view/suggestbutton.cpp \
    view/widgetutil.cpp \
    view/usbselectview.cpp \
    view/deviceinfoitem.cpp \
    view/devicelistwidget.cpp \
    view/dwaterprogress.cpp \
    view/progressview.cpp \
    view/resultview.cpp

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





