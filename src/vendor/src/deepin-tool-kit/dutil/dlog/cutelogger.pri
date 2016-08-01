INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/RollingFileAppender.h \
    $$PWD/Logger.h \
    $$PWD/FileAppender.h \
    $$PWD/CuteLogger_global.h \
    $$PWD/ConsoleAppender.h \
    $$PWD/AbstractStringAppender.h \
    $$PWD/AbstractAppender.h \
    $$PWD/LogManager.h

SOURCES += \
    $$PWD/RollingFileAppender.cpp \
    $$PWD/Logger.cpp \
    $$PWD/FileAppender.cpp \
    $$PWD/ConsoleAppender.cpp \
    $$PWD/AbstractStringAppender.cpp \
    $$PWD/AbstractAppender.cpp \
    $$PWD/LogManager.cpp

win32 {
    SOURCES += $$PWD/OutputDebugAppender.cpp
    HEADERS += $$PWD/OutputDebugAppender.h
}
