**The backend is fork from unetbootin just now. however, it will rewrite sooner or later, maybe.**

Deepin Boot Maker is help for user to create a boot usb stick quick and easy, it designed to support only deepin install iso, but it can can work for all ubuntu live install iso too.

深度启动器制作工具是用来帮助用户简单便捷的制作启动U盘的工具. 设计上只支持深度系统安装镜像,但实际上Ubuntu Live系列的镜像都是可以支持的.

![duc](https://cloud.githubusercontent.com/assets/1117694/3170269/8cfbd364-ebb4-11e3-811e-39da9026f4c7.png)

系统支持:
====

Windows平台:

Windows 7/ Windows 8

**需要安装显卡驱动**

Windows XP

**需要安装显卡驱动**

**需要更新最新版本的DirectX**

Direct X环境安装包：

DirectX Redist Package 地址:

![DirectX Redist Package](http://pan.baidu.com/s/1ntJTktb)

或者使用DirectX Runtime在线安装工具 ![dxwbsetup](http://pan.baidu.com/s/1dD289WH)

Linux平台

Ubuntu12.04/Ubuntu14.04/Deepin 14.04

**安装以下软件包**

sudo apt-get install syslinux  p7zip mtools libgl1-mesa-dri libx11-xcb1 libsm6 libxcb1 libfontconfig1 libxrender1 libpng12-0 libgl1-mesa-dev

Mac:

直接安装DMG文件

不支持Mac OS 10.10（可以使用命令行直接运行MacOS目录下的二进制文件）

开发指南
================
开发环境：Qt5.3
请注意以下事项：
1. 现有阶段代码主要为静态编译准备，qt5.3中的qtquick qml文件是直接打包在qrc文件中发布的，使用其他版本的qt可能导致兼容性问题。
2. 如需要动态编译版本，

Windows平台:

开发环境：
Windows 7 64bit + msvc2010 + WinSDK 8.1

1.如果准备支持Windows XP，请自行编译静态或者动态版本的Qt 5.3 with angle版本。编译参数：

set DXSDK_DIR="C:\Program Files (x86)\Windows Kits\8.0\Include\um"
configure -prefix "C:\Qt\QtStatic\5.3\vs2010" -release -platform win32-msvc2010 -no-qml-debug -confirm-license -opensource -static -qt-pcre -no-icu -no-sql-sqlite -no-nis -no-cups -no-iconv -no-dbus -nomake examples  -no-sql-odbc -no-compile-examples -skip qtwebkit -skip qtwebkit-examples -skip qtactiveqt -no-openssl -qt-zlib -qt-libpng -qt-freetype -qt-libjpeg -opengl es2 -angle


注意：1.不要编译icu，不然发布时会附带30M左右的icu支持的dll

      2.静态编译版本，如果程序运行需要libegl相关dll，请自行编译angle。需要win7系统，msvc 2010。详细编译参数参考：
          https://code.google.com/p/angleproject/wiki/DevSetup
          
2.如果不准备支持Windows XP，则可以使用官方版本的Qt with mingw 进行静态编译：

configure -prefix "D:\Qt\QtStatic\mingw5.3" -debug-and-release -platform win32-g++ -confirm-license -opensource -static -qt-pcre -no-icu -no-sql-sqlite -no-nis -no-cups -no-iconv -no-dbus -nomake examples  -no-sql-odbc -no-compile-examples -skip qtwebkit -skip qtwebkit-examples -skip qtactiveqt -no-openssl -qt-zlib -no-angle -opengl desktop


