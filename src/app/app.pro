include($$PWD/../vendor/vendor.pri)

TEMPLATE  = app

TARGET = deepin-boot-maker

QT += core gui widgets

RESOURCES +=  \
    blobs.qrc \
#    tranlate.qrc \


SOURCES += \
    main.cpp

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

