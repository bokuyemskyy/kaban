# Kaban

[![CI/CD](https://github.com/bokuyemskyy/kaban/actions/workflows/ci-cd.yml/badge.svg)](https://github.com/bokuyemskyy/kaban/actions/workflows/ci-cd.yml)  

![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)

[Documentation (WIP)](https://bokuyemskyy.github.io/kaban/docs/html/)
## Overview
A complete (rule compliant) chess move generation library and GUI. 
- Provides a standalone library `kaban_lib`.
- Has basic UCI and GUI interfaces built on top.
- Implements efficient generation via compiler optimizations, bitboards, magics, raw bitfields, constexpressions and inlines.
- Mitigates a fundamental C++ `enum class` limitation by introducing a reusable `StrongValue` type, enabling object-like enums (e.g., `Squares::A4.file().toString()`) for an elegant, type-safe, and zero-cost access.

## Screenshots
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
- OpenGL, GLFW3, ImGui, Stb, Boost

## Build

### Requirements
Install system libraries for GUI:
```
libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev pkg-config
```
Install Boost headers if you want to build the `compare_perft` script.

### Clone the repository
Clone including the VCPKG submodule:
```
git clone --recurse-submodules https://github.com/bokuyemskyy/kaban
```

Or clone and update submodules manually:
```
git clone https://github.com/bokuyemskyy/kaban
cd kaban
git submodule update --init --recursive
```

### Configuration
Configure the project using CMake:
```
cmake --preset default      # automatic generator and compiler
```
or
```
cmake --preset ninja        # uses clang++ with Ninja, faster and multi-config
```

The chess library `kaban_lib` is always included in the build.
Optional flags:
```
-DBUILD_GUI=ON/OFF       # default=ON
-DBUILD_UCI=ON/OFF       # default=ON
-DBUILD_TESTS=ON/OFF     # default=OFF
-DBUILD_SCRIPTS=ON/OFF   # default=OFF
```

### Compilation
With default generator:
```
cmake --build build
```
With Ninja:
```
cmake --build --preset Release   # or Debug
```

### Usage
Run the binaries under:
```
build/app/
```

## Credits
- Source of truth for perft is [Stockfish](https://stockfishchess.org/). Also uses the Stockfish PRNGs to generate magics.
- Inspired by Sebastian Lague [Chess Engine in C#](https://www.youtube.com/watch?v=U4ogK0MIzqk)
