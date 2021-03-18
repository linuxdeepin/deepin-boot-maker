include($$PWD/vendor.pri)
QT += core gui widgets testlib

#CONFIG += c++11 console
CONFIG += c++11 console link_pkgconfig
PKGCONFIG += dtkwidget

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    unittestobj.cpp \
    diskapitestobj.cpp \
    imagecheckobj.cpp

HEADERS += \
    unittestobj.h \
    diskapitestobj.h \
    imagecheckobj.h

INCLUDEPATH += $${PWD}/../app

LIBS+= -lgtest

QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0 -fno-access-control
QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0 -fno-access-control
