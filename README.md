# libblueprint
===============

## Description

libblueprint is a library providing a stable API for reading the .blueprint files used by FtD

(Writing is not yet implemented)

**This is alpha grade software. I did not pay any attention to security for this release.
It *WILL* contain errors and bugs.**

## Building

libblueprint has been tested and confirmed working on:
 - Arch Linux 64-bit, using clang 3.8
 - Windows 8.1 64-bit, using MinGW-GCC 4.8.1

Requirements:
 - C11 compatible compiler
 - CMake

Build using cmake

1. `mkdir build && cd build`
2. `cmake ..`
3. `make`

Build examples using the provided Makefile.

The examples need to find libblueprint.

On Linux either run `export LD_LIBRARY_PATH=/path/to/cmake/build`
or
run examples with prefix `LD_LIBRARY_PATH=/path/to/cmake/build`

On Windows put the created .dll in the same directory and the examples .exe


## Usage

Have a look at the `examples` subdirectory

## License

[MIT License](LICENSE)

This library includes:
 - [parson](https://github.com/kgabis/parson/) - MIT License
 - [bstrlib](https://github.com/websnarf/bstrlib) - 3-clause BSD
