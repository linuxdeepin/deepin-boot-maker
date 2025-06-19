include($$PWD/vendor.pri)

TEMPLATE  = app

TARGET = deepin-boot-maker
#添加安全编译参数
QMAKE_LFLAGS += -z noexecstack -pie -z relro -z now
QMAKE_CFLAGS += -fstack-protector-all -fPIE
QMAKE_CXXFLAGS += -fstack-protector-all -fPIE

## 添加内存泄露检测
#CONFIG(debug, debug|release) {
#    QMAKE_CXX += -g -fsanitize=address -O2
#    QMAKE_CXXFLAGS += -g -fsanitize=address -O2
#    QMAKE_LFLAGS += -g -fsanitize=address -O2
#}

QT += core gui widgets concurrent network svg

CONFIG += c++11 link_pkgconfig
CONFIG   += resources_big

versionAtLeast(QT_VERSION, 6.0.0) {
    PKGCONFIG += dtk6widget
    PKGCONFIG += polkit-qt6-1
} else {
    PKGCONFIG += dtkwidget
    PKGCONFIG += polkit-qt5-1
}

RESOURCES +=  \
    ../translate.qrc \
    view/resource/resource.qrc

HEADERS += \
    view/isoselectview.h \
    view/widgetutil.h \
    view/usbselectview.h \
    view/deviceinfoitem.h \
    view/devicelistwidget.h \
    view/progressview.h \
    view/resultview.h \
    view/dropframe.h \
    view/unmountusbview.h \
    ../vendor/src/libxsys/DiskUtil/DiskUtil.h \
    view/formatdialog.h

SOURCES += \
    view/isoselectview.cpp \
    view/widgetutil.cpp \
    view/usbselectview.cpp \
    view/deviceinfoitem.cpp \
    view/devicelistwidget.cpp \
    view/progressview.cpp \
    view/resultview.cpp \
    view/dropframe.cpp \
    bmwindow.cpp \
    main.cpp \
    view/unmountusbview.cpp \
    ../vendor/src/libxsys/DiskUtil/DiskUtil.cpp \
    view/formatdialog.cpp

unix {

INCLUDEPATH += $$PWD/linux
HEADERS += $$PWD/linux/bmwindow.h

}

linux {

binary.path = $${PREFIX}/bin
binary.files = $${OUT_PWD}/deepin-boot-maker

desktop.path = $${PREFIX}/share/applications
desktop.files = $$PWD/platform/linux/deepin-boot-maker.desktop

hicolor.path =  $${PREFIX}/share/icons/hicolor/scalable/apps
hicolor.files = $$PWD/view/resource/theme/light/image/deepin-boot-maker.svg

INSTALLS += binary desktop hicolor

}

win* {
    INCLUDEPATH += $$PWD/other
    HEADERS += $$PWD/other/bmwindow.h
#    QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
    RC_FILE += $$PWD/platform/windows/deepin-boot-maker.rc
}

mac* {
    CONFIG += app_bundle

    ICON = $$PWD/platform/mac/deepin-boot-maker.icns

    blob.files = $$files($$PWD/platform/mac/Contents/Resources/*)
    blob.path = Contents/Resources

    QMAKE_BUNDLE_DATA += blob

    QMAKE_POST_LINK = macdeployqt deepin-boot-maker.app/ -libpath=/Users/deepin/Development/ci/usr/lib
}


