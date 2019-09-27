# Deepin Boot Maker

Deepin Boot Maker is a tool that helps user to create a bootable usb stick quick and easy, it is designed to support deepin install iso, but it works for all ubuntu live install iso too.

![screenshot](https://raw.githubusercontent.com/linuxdeepin/deepin-boot-maker/master/docs/deepin-boot-maker.png)

## Dependencies

Deepin Boot Maker is designed to support Windows/Linux/Mac platforms. It has no external dependencies on Windows and Mac OS. On Linux, you need packages below:

```bash
#For release based on debian
sudo apt-get install p7zip-full mtools syslinux syslinux-common
```

Official support platform:

- Deepin 2015

- Mac OSX 10.12

- Windows 7 32/64bit, Windows 8 32/64 bit.

## Installation

- Official binary release

The only binary release is from Deepin OS iso for now. You can download iso from: https://www.deepin.org/original/deepin-boot-maker/. Or you can find binary file on the root of iso.

- Repository Install

If you are deepin user, you can install Deepin Boot Maker from repository:

```bash
sudo apt-get install deepin-boot-maker
```

- Build and install

See [INSTALL](INSTALL.md) for detail.

## Getting help

Any usage issues can ask for help via

* [Gitter](https://gitter.im/orgs/linuxdeepin/rooms)
* [IRC channel](https://webchat.freenode.net/?channels=deepin)
* [Forum](https://bbs.deepin.org)
* [WiKi](https://wiki.deepin.org/)

## Getting involved

We encourage you to report issues and contribute changes

* [Contribution guide for developers](https://github.com/linuxdeepin/developer-center/wiki/Contribution-Guidelines-for-Developers-en). (English)
* [开发者代码贡献指南](https://github.com/linuxdeepin/developer-center/wiki/Contribution-Guidelines-for-Developers) (中文)

## License

Deepin Boot Maker is licensed under [GPLv3](LICENSE).

## Credits and references

1. [UNetbootin](https://github.com/unetbootin/unetbootin): this project is derived form unetbootin, but now there are big differences between them.
