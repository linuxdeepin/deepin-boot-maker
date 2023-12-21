include($$PWD/vendor.pri)

# 添加内存泄露检测
CONFIG(debug, debug|release) {
    QMAKE_CXX += -g -fsanitize=address -O2
    QMAKE_CXXFLAGS += -g -fsanitize=address -O2
    QMAKE_LFLAGS += -g -fsanitize=address -O2
}

#添加安全编译参数
QMAKE_LFLAGS += -z noexecstack -pie -fPIC -z relro -z now
QMAKE_CFLAGS += -fstack-protector-all
QMAKE_CXXFLAGS += -fstack-protector-all -fno-access-control


QT += core gui widgets testlib


#CONFIG += c++11 console
CONFIG += c++11 console link_pkgconfig
CONFIG += testcase no_testcase_installs
CONFIG   += resources_big
PKGCONFIG += dtkwidget
PKGCONFIG += dtkcore
PKGCONFIG += polkit-qt5-1

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
    imagecheckobj.cpp \
    bootmakerservicetest.cpp \
    filesystemtest.cpp \
    resulTtest.cpp \
    logmanagertest.cpp \
    qtbaseinstallertest.cpp \
    bootmakertest.cpp \
    utilstest.cpp \
    sevenziptest.cpp \
    ../libdbm/backend/bootmaker.cpp \
    ../libdbm/backend/diskutil.cpp \
    ../libdbm/backend/syslinux.cpp \
    ../libdbm/backend/bmdbusinterface.cpp \
    ../libdbm/util/devicemonitor.cpp \
    ../libdbm/util/sevenzip.cpp \
    ../libdbm/util/utils.cpp \
    ../libdbm/bminterface.cpp \
    ../libdbm/backend/bmhandler.cpp \
    ../libdbm/installer/qtbaseinstaller.cpp \
    ../libdbm/installer/qtinstallerfactory.cpp \
    ../libdbm/installer/qtX86Installer.cpp \
    ../libdbm/installer/qtmipsinstaller.cpp \
    ../vendor/src/libxsys/DiskUtil/DiskUtil.cpp \
    ../vendor/src/libxsys/Common/Result.cpp \
    ../vendor/src/libxsys/Cmd/Cmd.cpp \
    ../vendor/src/libxsys/FileSystem/FileSystem.cpp \
    ../vendor/src/libxsys/DiskUtil/Syslinux.cpp \
    ../service/bootmakerservice.cpp \
    ../service/LogManager.cpp





HEADERS += \
    unittestobj.h \
    diskapitestobj.h \
    imagecheckobj.h \
    bootmakerservicetest.h \
    filesystemtest.h \
    resultTest.h \
    logmanagertest.h \
    qtbaseinstallertest.h \
    bootmakertest.h \
    utilstest.h \
    sevenziptest.h \
    ../libdbm/backend/bmhandler.h \
    ../libdbm/backend/bootmaker.h \
    ../libdbm/backend/diskutil.h \
    ../libdbm/backend/syslinux.h \
    ../libdbm/backend/bmdbushandler.h \
    ../libdbm/backend/bmdbusinterface.h \
    ../libdbm/util/deviceinfo.h \
    ../libdbm/util/devicemonitor.h \
    ../libdbm/util/sevenzip.h \
    ../libdbm/util/utils.h \
    ../libdbm/bminterface.h \
    ../libdbm/installer/qtbaseinstaller.h \
    ../libdbm/installer/qtinstallerfactory.h \
    ../libdbm/installer/qtX86Installer.h \
    ../libdbm/installer/qtmipsinstaller.h \
    ../libdbm/installer/qtarminstaller.h \
    ../libdbm/installer/qtswinstaller.h \
    ../vendor/src/libxsys/DiskUtil/DiskUtil.h \
    ../vendor/src/libxsys/Common/Result.h \
    ../vendor/src/libxsys/Cmd/Cmd.h \
    ../vendor/src/libxsys/FileSystem/FileSystem.h \
    ../vendor/src/libxsys/DiskUtil/Syslinux.h \
    ../service/bootmakerservice.h \
    ../service/LogManager.h \
    ../service/bootmakerservice_p.h




INCLUDEPATH += ../app ../libdbm ../vendor/src/libxsys

LIBS+= -lgtest

QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
