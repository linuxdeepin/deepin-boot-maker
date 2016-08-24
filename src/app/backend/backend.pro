include($$PWD/../vendor/vendor.pri)

TEMPLATE  = app

TARGET = boot-maker-backend

QT += core

HEADERS += \
    bootmaker.h

SOURCES += \
    bootmaker.cpp \
    main.cpp
