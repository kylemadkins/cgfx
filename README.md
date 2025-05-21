# cgfx

A software renderer written in C

## Building the project

### SDL2 2.32.6

This repository includes the necessary SDL2 development files and binaries
precompiled for the [MinGW-w64](https://www.mingw-w64.org/) toolchain

To build the project with MinGW-w64 on Windows, use the following command

```sh
mingw32-make.exe
```

For macOS and Linux, install SDL2 with your package manager (e.g. `sudo apt install libsdl2-dev`). Then install [`pkg-config`](https://en.wikipedia.org/wiki/Pkg-config) (e.g. `brew install pkg-config`). Finally, build with Make

```sh
make
```
