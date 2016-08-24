TEMPLATE  = subdirs

SUBDIRS  += \
    dbase \
    dutil \
    dwidget \
    test \
    examples \

dutil.depends = dbase
dwidget.depends = dutil
test.depends = dwidget
examples.depends = test
