TEMPLATE  = subdirs
CONFIG   += ordered

system(python $$PWD/translate_generation.py $$PWD)

SUBDIRS  += vendor/src/libxsys

unix {
    SUBDIRS  += service
}

SUBDIRS  += app
