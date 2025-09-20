# kaban - a chess movegen with GUI

A complete (rule compliant) chess move generator. Supports basic UCI commands.
<img width="802" height="629" alt="Screenshot_20250804_103950-1" src="https://github.com/user-attachments/assets/34ada001-ab00-466a-b595-88b31466d711" />

## Documentation (WIP)
[Documentation](https://bokuyemskyy.github.io/kaban/docs/html/)

## Technologies and stack
- C/C++
- Shell scripts
- CMake
- Ninja build system
- Clang compiler
- Clangd, clang-format, clang-tidy
- Test units
- Doxygen

## Libraries used
- Boost
- GLWF
- ImGui
- GTest

## Roadmap
### Implemented features
- Type-safe and object-oriented type base
- Clear class separation
- Bitboard representation
- Magic bitboards
- Raw bit fields
- Constexpr evaluation of hot paths
- Simple move generation (pseudo moves)
- Legal, complete move generation (en-passants, castlings, promotions)
- UCI protocol
- Textures, resources inlined
- GUI
### Pending features
- Position evaluation function
- Transformation into an engine
- Search algorithms (minimax, alpha-beta pruning)
- Inline ASM in hot paths

## How to run
### Requirements
- VCPKG

### Build
```bash
git clone https://github.com/bokuyemskyy/kaban.git
cd kaban

# Configure build as you need
cmake --preset default -G "Unix Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=/usr/bin/vcpkg/scripts/buildsystems/vcpkg.cmake
# Or if you have Ninja, Clang installed
cmake --preset ninja -DCMAKE_TOOLCHAIN_FILE=/usr/bin/vcpkg/scripts/buildsystems/vcpkg.cmake

# Build the app
cmake --build build 
# Or if you use Ninja Multi-Config
cmake --build build --preset release

# Run
./build/kaban 
# Or run UCI
./build/kaban uci
```

## Inspiration
Inspired by [Stockfish](https://stockfishchess.org/). Uses the Stockfish PRNGs to generate magics.

## Contributing
Feel free to open issues or submit pull requests if you have ideas, improvements, or bugs to report.

## License
MIT License. See [LICENSE](LICENSE) for details.
