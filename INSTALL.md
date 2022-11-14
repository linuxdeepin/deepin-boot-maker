# Installation

If you want binary install, please see [README](README.md).

## Linux Platform

### Development Environment

- Release base debian

### Install Build Dependencies

```bash
sudo apt-get install debhelper python qt5-default qt5-qmake qttools5-dev-tools libstartup-notification0-dev libxcb-util0-dev libdtkwidget-dev
```

### Build Deepin Boot Maker

```
mkdir build
cd build
qmake -r ..
make
```

## Windows Platform

### Development Environment

- Windows 10 64bit
- Microsoft Visual C++ 2017
- Windows SDK
- Qt5.8
- Dtk

### Build Qt static

```
configure -prefix "C:\Qt\QtStatic\5.8\vs2017" -release -platform win32-msvc2010 \
-no-qml-debug -confirm-license -opensource -static -qt-pcre -no-icu -no-sql-sqlite \
-no-nis -no-cups -no-iconv -no-dbus -nomake examples -no-sql-odbc -no-compile-examples \
-skip qtwebkit -skip qtwebkit-examples -skip qtactiveqt -no-openssl -qt-zlib \
-qt-libpng -qt-freetype -qt-libjpeg -opengl es2 -angle
```

### Build Dtk

TODO

### Build Deepin Boot Maker

```
mkdir build
cd build
qmake -r ..
nmake
```


## Mac OSX Platform

### Development Environment

- Mac OS 10.12
- Qt5.8

### Set Qt PATH

```
export QtInstallPath=/User/yourhome/Qt5.8/5.8/clang_64
export PATH=$QtInstallPath/bin:$PATH
```
### Build Dtk

TODO

### Build Deepin Boot Maker

```
mkdir build
cd build
qmake -r ..
make
macdeployqt src/app/deepin-boot-maker.app
```
