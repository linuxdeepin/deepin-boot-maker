**The backend is fork from unetbootin just now. however, it will rewrite sooner or later, maybe.**

Deepin Usb Creator is help for user to create a boot usb stick quick and easy, it designed to support only deepin install iso, but it can can work for all ubuntu live install iso too.

深度启动器制作工具是用来帮助用户简单便捷的制作启动U盘的工具. 设计上只支持深度系统安装镜像,但实际上Ubuntu Live系列的镜像都是可以支持的.

![duc](https://cloud.githubusercontent.com/assets/1117694/3170269/8cfbd364-ebb4-11e3-811e-39da9026f4c7.png)

系统支持:
====

Windows平台:
Windows 7/ Windows 8
**需要安装显卡驱动**
Windows XP 平台待测试

Linux平台
Ubuntu14.04/Deepin 14.04
**请安装以下软件包**
sudo apt-get install p7zip mtools

Mac:
预计支持


开发指南
=====
深度启动器制作工具,前端使用Qt/QML强烈依赖opengl支持.请保证qt版本在5.3或以上.

Linux平台需要先静态编译qt5.3, 但是在ubuntu14.04之后系统中不再包含libGL.a库,请先静态编译libgl1-mesa-dev.
编译libgl1-mesa-dev.
./configure --disable-dri --enable-glx --enable-osmesa --enable-xlib-glx --enable-32-bit
生成make文件部分*.so未改为*.a, 请根据错误提示自行修改
qt编译32bit版本,最好禁用icu支持.

Windows平台使用Qt5.3发行版本的mingw进行编译,编译参数: 
configure -prefix "D:\Qt\QtStatic\mingw5.3" -debug-and-release -platform win32-g++ -confirm-license -opensource -static -qt-pcre -no-icu -no-sql-sqlite -no-nis -no-cups -no-iconv -no-dbus -nomake examples  -no-sql-odbc -no-compile-examples -skip qtwebkit -skip qtwebkit-examples -skip qtactiveqt -no-openssl -qt-zlib -no-angle -opengl desktop
修改qmake.conf文件, 静态编译libstdc++运行库, 添加 -static-libgcc  -static-libstdc++编译参数.
