QMAKE_CXXFLAGS += $$system(env PKG_CONFIG_PATH=$$PWD/lib/pkgconfig pkg-config --cflags dtkbase)
LIBS +=  $$system(env PKG_CONFIG_PATH=$$PWD/lib/pkgconfig pkg-config --libs dtkbase)
QMAKE_CXXFLAGS += $$system(env PKG_CONFIG_PATH=$$PWD/lib/pkgconfig pkg-config --cflags dtkutil)
LIBS +=  $$system(env PKG_CONFIG_PATH=$$PWD/lib/pkgconfig pkg-config --libs dtkutil)
