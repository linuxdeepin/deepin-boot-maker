# Installation

If you want binary install, please see [README](README.md).

## Linux Platform

### Development Environment

- Release base debian

### Install Build Dependencies

```bash
sudo apt-get install debhelper, qt5-default, qt5-qmake, qttools5-dev-tools
```

### Build Deepin Boot Maker

```
mkdir build
cd build
qmake -r ../deepin-boot-maker.pro
make
```

## Windows Platform

### Development Environment

- Windows 7 64bit
- Microsoft Visual C++ 2010
- DirectX SDK
- Qt5.4

### Build Qt static

```
set DXSDK_DIR="C:\Program Files (x86)\Windows Kits\8.0\Include\um"
configure -prefix "C:\Qt\QtStatic\5.3\vs2010" -release -platform win32-msvc2010 \
-no-qml-debug -confirm-license -opensource -static -qt-pcre -no-icu -no-sql-sqlite \
-no-nis -no-cups -no-iconv -no-dbus -nomake examples -no-sql-odbc -no-compile-examples \
-skip qtwebkit -skip qtwebkit-examples -skip qtactiveqt -no-openssl -qt-zlib \
-qt-libpng -qt-freetype -qt-libjpeg -opengl es2 -angle
```

### Build Deepin Boot Maker

```
mkdir build
cd build
qmake.exe -r ../deepin-boot-maker.pro
nmake
```


## Mac OSX Platform

### Development Environment

- Mac OS 10.11
- Qt5.4

### Set Qt PATH

```
export QtInstallPath=/User/yourhome/Qt5.3/5.3/clang_64
export PATH=$QtInstallPath/bin:$PATH
```

### Build Deepin Boot Maker

```
cd osx
qmake -r ../deepin-boot-maker.pro
make
macdeployqt app/deepin-boot-maker.app
```
