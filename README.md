# Minecraft-Input-Helper

This project can help you easily enter the Chinese language.

## Dependencies

### Build dependencies

* qmake (>= 5.6)
* [deepin-tool-kit](https://github.com/linuxdeepin/deepin-tool-kit) (developer package)
* xcb-util (developer package)
* xtst (developer package)

### Runtime dependencies

* [deepin-tool-kit](https://github.com/linuxdeepin/deepin-tool-kit)
* Qt5 (>= 5.6)
  * Qt5-X11extras
* xcb-util
* xtst
* xdotool

## Installation

### Build from source code

1. Make sure you have installed all dependencies.

2. Build:
```
$ cd Minecraft-Input-Helper
$ mkdir Build
$ cd Build
$ qmake ..
$ make
```

3. Install:
```
$ sudo make install
```

The executable binary file could be found at `/usr/bin/minecraft-input-helper` after the installation is finished.

## Getting help

You can create a issue, I will help you.

You may also find these channels useful if you encounter any other issues:

* [deepin community](https://bbs.deepin.org) Topbar is also the community software that you can post for help.

## License

Minecraft-Input-Helper is licensed under [GPLv3](LICENSE).

## Want to know how to help me?

It's simple.

* fork and pull request. I want more people to help me improve it.