# cgfx

A software renderer written in C

## Building the project

### SDL2 2.32.6

This repository includes the necessary SDL2 development files and binaries
precompiled for the [MinGW-w64](https://www.mingw-w64.org/) toolchain.

Use this command with MinGW-w64 on Windows:
`mingw32-make.exe`

For macOS and Linux, install SDL2 with your package manager (e.g. `sudo apt install libsdl2-dev`) and build with Make:
`make`
