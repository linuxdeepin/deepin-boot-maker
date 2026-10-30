TEMPLATE  = subdirs
CONFIG   += ordered


SUBDIRS += vendor/src/libxsys \
    libdbm

linux {
    SUBDIRS += service

    # DConfig 调试模式日志规则 meta（应用内动态调整日志级别使用），
    # 安装到 /usr/share/dsg/configs/org.deepin.boot-maker/ 供主程序与服务进程共用
    dconfig_meta.path = $${PREFIX}/share/dsg/configs/org.deepin.boot-maker
    dconfig_meta.files = $$PWD/configs/org.deepin.boot-maker.json
    INSTALLS += dconfig_meta
}
SUBDIRS += app


#SUBDIRS += tests


mac* {
    TRANSLATIONS_NAME = deepin-boot-maker
    TRANSLATIONS += $$PWD/translations/$${TRANSLATIONS_NAME}.ts \
                $$PWD/translations/$${TRANSLATIONS_NAME}_zh_CN.ts
    system(python3 $$PWD/tools/translate_generation.py $$PWD)
} else {
    TRANSLATIONS = $$files($$PWD/translations/*.ts)
    for(tsfile, TRANSLATIONS) {
        qmfile = $$replace(tsfile, .ts$, .qm)
        versionAtLeast(QT_VERSION, 6.0.0) {
            system(/usr/lib/qt6/bin/lrelease $$tsfile -qm $$qmfile) | error("Failed to lrelease")
        } else {
            system(lrelease $$tsfile -qm $$qmfile) | error("Failed to lrelease")
        }
    }
    linux {
        dtk_translations.path = /usr/share/deepin-boot-maker/translations
        dtk_translations.files = $$PWD/translations/*.qm
        INSTALLS += dtk_translations
    }
}

