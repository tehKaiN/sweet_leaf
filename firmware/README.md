# Sweet Leaf firmware

## Directory structure

It's a mess, because I wanted the project to be regenerate'able from CubeMX.

- `Core` and `Drivers` directories contain Cube-specific files. Who the hell
  uses uppercase in folders anyway? Most of those files are git-ignored.
- True project files reside in `sweet_leaf` directory.

## Build instructions

CubeMX generates Makefile but it can't be reliably edited when regenerating
project. This project uses tupfile instead.

Prerequisites:

- ARM GCC
- STM32 CubeMX (needs Java RE)
- [tup](http://gittup.org/tup/), type `tup init` inside this directory

Launch `firmware.ioc` and generate project files in current directory. Then
type `tup` to (re)build the project.

## License

Unless otherwise stated, the sources are licensed under MPL 2.0.
See LICENSE for details.

TLDR: if you base your own project on MPL sources, you need to publish your
changes to them.

## 3rd party stuff used

I'm not keeping those as submodules to keep project tree simple. They'll get
updated manually if there's need or something.

- [printf / sprintf for embedded systems](https://github.com/mpaland/printf)
