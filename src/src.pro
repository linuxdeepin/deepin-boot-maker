TEMPLATE  = subdirs
CONFIG   += ordered


SUBDIRS += vendor/src/libxsys \
    libdbm

linux {
    SUBDIRS += service
}

SUBDIRS += app
mac* {
    TRANSLATIONS_NAME = deepin-boot-maker
    TRANSLATIONS += $$PWD/translations/$${TRANSLATIONS_NAME}.ts \
                $$PWD/translations/$${TRANSLATIONS_NAME}_zh_CN.ts
system(python $$PWD/tools/translate_generation.py $$PWD)
}else   {
CONFIG(release, debug|release) {
    TRANSLATIONS = $$files($$PWD/translations/*.ts)
    for(tsfile, TRANSLATIONS) {
        qmfile = $$replace(tsfile, .ts$, .qm)
        system(lrelease $$tsfile -qm $$qmfile) | error("Failed to lrelease")
    }
linux {
    dtk_translations.path = /usr/share/$$TARGET/translations
    dtk_translations.files = $$PWD/translations/*.qm
    INSTALLS += dtk_translations

}
}
}
