TEMPLATE = app
TARGET = deepin-usb-creator

DEPENDPATH += .
INCLUDEPATH += .

HEADERS += unetbootin.h \
    usbcreator.h \
    qml/Deepin/Widgets/plugins/dicon.h \
    qml/Deepin/Widgets/plugins/dwindow.h

SOURCES += main.cpp \
    unetbootin.cpp \
    usbcreator.cpp \
    qml/Deepin/Widgets/plugins/dicon.cpp \
    qml/Deepin/Widgets/plugins/dwindow.cpp

CONFIG(lupdate) {
    SOURCES += qml/mainui.qml
}

QT += core gui quick widgets

ICON =

CONFIG(debug, debug|release) {
    DESTDIR = ../build/debug
    OBJECTS_DIR = ../build/debug/.obj
    MOC_DIR = ../build/debug/.moc
    RCC_DIR = ../build/debug/.rcc
    UI_DIR = ../build/debug/.ui
} else {
    DESTDIR = ../build/release
    OBJECTS_DIR = ../build/release/.obj
    MOC_DIR = ../build/release/.moc
    RCC_DIR = ../build/release/.rcc
    UI_DIR = ../build/release/.ui
}

TRANSLATIONS += deepin-usb-creator_en_US.ts \
    deepin-usb-creator_zh_CN.ts \
    deepin-usb-creator_zh_TW.ts \

win32{
RESOURCES += \
    deepin-usb-creator.qrc \
    unetbootin-windows.qrc \
    unetbootin-sevnz.qrc
    RC_FILE += deepin-usb-creator.rc
}else{
RESOURCES += \
    deepin-usb-creator.qrc
}
OTHER_FILES +=

RESOURCES += \
    deepin-usb-creator-qml.qrc

ICON = deepin-usb-creator.svg

defineTest(matches) {
  value = $$1
  regex = $$2
  test = $$replace($${value}, $${regex}, "")
  isEmpty($${test}) {
    return(true)
  } else {
    return(false)
  }
}

linux-g++* {
   for(lib, LIBS) {
      # If it's something we don't recognize (neither "-lfoo" nor "-Lfoo") just add it directly
      !matches(lib, "^-l.*$") {
         libtemp *= $${lib}
      # Don't statically link POSIX threading or dlOpen libraries
      } else:isEqual(lib, "-lpthread") | isEqual(lib, "-ldl") {
         libtemp *= $${lib}
      # Ask GCC to find a static version of the library
      } else {
         libfile = $$replace(lib, "^-l(.*)$", "lib\1.a")
         libloc = $$system("gcc $${LIBS} -print-file-name=$${libfile}")
         # If it didn't find it, just keep the
         isEqual(libloc, $${libfile}) {
            libtemp *= $${lib}
         } else {
            libtemp *= $${libloc}
         }
      }
   }
   LIBS = $${libtemp}
   unset(libtemp)
   unset(libfile)
   unset(libloc)
}
