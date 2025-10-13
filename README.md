# Kaban
**Chess move generation library with GUI**

[![Build Release](https://github.com/bokuyemskyy/kaban/actions/workflows/build-release.yml/badge.svg?branch=main)](https://github.com/bokuyemskyy/kaban/actions/workflows/build-release.yml)  
[Documentation (WIP)](https://bokuyemskyy.github.io/kaban/docs/html/)

## Overview
A complete (rule compliant) chess move generator. Provides a distinct library `kaban_lib`. Has basic UCI and GUI interfaces built on top.  
Implements efficient generation via compiler optimizations, bitboards, magics, raw bitfields, constexpressions and inlines.

<img width="802" height="629" alt="Screenshot_20250804_103950-1" src="https://github.com/user-attachments/assets/34ada001-ab00-466a-b595-88b31466d711" />

## Technical stack
- C/C++
- Shell
- CMake
- Ninja
- Clang++
- clangd, clang-format, clang-tidy
- GitHub Actions
- GTest (Unit, integration tests)
- Doxygen (Documentation draft)
- GLFW3, ImGui, GLAD, Boost

## Build

First of all, you have to clone the repo with the VCPKG submodule

```
git clone --recurse-submodules https://github.com/bokuyemskyy/kaban
```
or
```
git clone https://github.com/bokuyemskyy/kaban
cd kaban
git submodule update --init --recursive
```

If you want to build the GUI, also ensure these libraries (or their equivalents) on your system:
`libxinerama-dev`, `libxcursor-dev`, `xorg-dev`, `libglu1-mesa-dev`, `pkg-config`

If you need compare_perft script, install `boost` headers

Next, configure the project. You can do it both ways
```
cmake --preset default # uses g++ and Unix Makefiles.
cmake --preset ninja   # uses clang++ and Ninja. Much faster and has multi-config

# Library will be built by default
# Additional arguments
# -DBUILD_GUI=ON/OFF [default=ON]
# -DBUILD_UCI=ON/OFF [default=ON]
# -DBUILD_TESTS=ON/OFF [default=OFF]
# -DBUILD_SCRIPTS=ON/OFF [default=OFF]
```

Build the app
```
cmake --build
# or, if you used Ninja
cmake --build --preset Debug
# or
cmake --build --preset Release
```

## Usage
Run the binaries under `build` dir  
Ready!


## Credits
- Source of truth for perft is [Stockfish](https://stockfishchess.org/). Also uses the Stockfish PRNGs to generate magics.
- Inspired by Sebastian Lague [Chess Engine in C#](https://www.youtube.com/watch?v=U4ogK0MIzqk)
