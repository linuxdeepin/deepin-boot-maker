TEMPLATE  = subdirs
CONFIG   += ordered
SUBDIRS  += vendor/src/libxsys \

unix {
    SUBDIRS  += service
}

SUBDIRS  += app
