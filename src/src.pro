TEMPLATE  = subdirs
CONFIG   += ordered

unix {
    system($$PWD/translate_generation.sh)
}

SUBDIRS  += vendor/src/libxsys

unix {
    SUBDIRS  += service
}

SUBDIRS  += app
