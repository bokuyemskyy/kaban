# Kaban

[![CI/CD](https://github.com/bokuyemskyy/kaban/actions/workflows/ci-cd.yml/badge.svg)](https://github.com/bokuyemskyy/kaban/actions/workflows/ci-cd.yml)   

![Linux](https://img.shields.io/badge/Linux-FCC624?style=flat&logo=linux&logoColor=black) ![Windows](https://custom-icon-badges.demolab.com/badge/Windows-0078D6?logo=windows11&logoColor=white) [![macOS](https://img.shields.io/badge/macOS-000000?logo=apple&logoColor=F0F0F0)]

![Documentation](https://img.shields.io/badge/Documentation-WIP-cc2a34?labelColor=31393f)

## Overview
This project is a high-performance, rule-compliant chess move generation library. It includes optional UCI and GUI components that can be built as part of the project.

## Features
- Provides a standalone core chess library `kaban_lib`.
- Fully implements basic UCI and GUI that can be included in build if needed.
- Efficiently generates moves via compiler optimizations, bitboards, magics, raw bitfields, constexpressions and inlines.
- Bypasses a fundamental C++ `enum class` limitation by introducing a reusable `StrongValue` type, enabling object-like enums (e.g., `Squares::A4.file().toString()`) for an elegant, type-safe, and zero-cost access.

## Screenshots
<img width="802" height="629" alt="Screenshot_20250804_103950-1" src="https://github.com/user-attachments/assets/34ada001-ab00-466a-b595-88b31466d711" />

## Technical stack

### Requirements
- C++23
- CMake 3.30+
- g++15, clang++20 or MSVC 2022
- Make 4.4 or Ninja 1.12
- VCPKG (included as a submodule)
- OpenGL, GLFW3, ImGui, stb (managed by VCPKG)
- Linux packages: libxinerama-dev, libxcursor-dev, xorg-dev, libglu1-mesa-dev, pkg-config

### Optional requirements
- Shell
- Boost library (header-only, required by scripts)
- clangd, clang-format, clang-tidy

### Other technologies
- GitHub Actions (CI/CD)
- GTest (unit and integration tests)
- Doxygen (documentation draft)
  
## Usage
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
Configure the project with CMake:
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
cmake --build --preset release   # or debug
```

### Run
Binaries are located under:
```
build/app/
```

## Credits
- Source of truth for perft is [Stockfish](https://stockfishchess.org/). Also uses the Stockfish PRNGs to generate magics.
- Inspired by Sebastian Lague [Chess Engine in C#](https://www.youtube.com/watch?v=U4ogK0MIzqk)
