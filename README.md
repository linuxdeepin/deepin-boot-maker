
**The backend is fork from unetbootin just now. however, it will rewrite sooner or later, maybe.**

Deepin Boot Maker is help for user to create a boot usb stick quick and easy, it designed to support only deepin install iso, but it can can work for all ubuntu live install iso too.

深度启动器制作工具是用来帮助用户简单便捷的制作启动U盘的工具. 设计上只支持深度系统安装镜像,但实际上Ubuntu Live系列的镜像都是可以支持的.

![duc](https://cloud.githubusercontent.com/assets/1117694/5467718/d3fcf2f4-85fe-11e4-9c6a-e64c2198b9c4.png)

系统支持:
====

**Windows平台：**

Windows 7/ Windows 8 /Windows XP

**Linux平台：**

Ubuntu12.04/Ubuntu14.04/Deepin 14.04

非Deepin 2014.2以下版本以及其他Debian衍生发行版请安装以下软件包

```
sudo apt-get install p7zip mtools
```

**Mac：**

Mac OSX 10.9/Mac OSX 10.10

开发指南
================

**Windows平台：**

开发环境：
Windows 7 64bit + msvc2010 + DirectX SDK + Qt5.4

1. 编译参数：
```
set DXSDK_DIR="C:\Program Files (x86)\Windows Kits\8.0\Include\um"
configure -prefix "C:\Qt\QtStatic\5.3\vs2010" -release -platform win32-msvc2010 \
-no-qml-debug -confirm-license -opensource -static -qt-pcre -no-icu -no-sql-sqlite \
-no-nis -no-cups -no-iconv -no-dbus -nomake examples -no-sql-odbc -no-compile-examples \
-skip qtwebkit -skip qtwebkit-examples -skip qtactiveqt -no-openssl -qt-zlib \
-qt-libpng -qt-freetype -qt-libjpeg -opengl es2 -angle
```

**Mac平台:**

Macx 10.9 + Qt5.4

1. 设置Qt路径
```
export QtInstallPath=/User/yourhome/Qt5.3/5.3/clang_64
export PATH=$QtInstallPath/bin:$PATH
```
2.编译
```
cd src
qmake -r deepin-boot-maker.pro
make
macdeployqt ../build/release/deepin-boot-maker.app
```

