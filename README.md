# kaban - chess movegen

A project, aimed to be a high-performance chess engine that uses bitboard techniques. 
Inspired by [Stockfish](https://stockfishchess.org/), this project aims to utilize efficient board representation, move generation and modern C++ practices to achieve high perft (performance test) results.

## Documentation
The draft of documentation you can find on here: [DOCUMENTATION](https://bokuyemskyy.github.io/kaban/docs/html/) (work in progress)

## Features roadmap
### Implemented
- **Build system**: convenient, agile and fast CMake-Ninja build system;
- **Additional configurations**: clang-family tools to ensure clean code, Doxygen documentation generator and VCPKG dependency manager;
- **Testing framework**: unit tests for the code;
- **Project structure**: modular code, split into Main, Renderer, Board and Movegen. The code combines different practices, e.g: object-oriented classes, functional-style code in namespaces;
- **Bitboards**: board representation and operations using 64-bit integers.
- **Bit-packing**: using raw bit fields instead of structures to increase efficiency and have more control over the memory; 
- **Magic bitboards**: a powerful approach of mapping the board occupancy to an index to access precomputed rook and bishop moves;
- **Simple move generation**: accurate, but simple (no en-passant, castling, 50-moves rule) move generation with legality checks;
### Pending
- **Complex move generation**: accurate and absolutely rule-compliant move generation;
- **UCI protocol compatibility**: integration with Universal Chess Interface for compatibility with chess GUIs;
- **Multithreading support**: parallel computations for improved performance;
- **Code cleanup**: the code is quite messy after all these experiments, so it is definitely worth it to refactor it.
  
## Future features
- **Transformation into an engine**: the project initially was aimed to be an engine, not a movegen. But the development takes too much time.
- **Evaluation function**: a scalable evaluation function with tuning capabilities.
- **Search algorithms**: Implementation of minimax, alpha-beta pruning, and potentially more advanced techniques like Principal Variation Search (PVS) and Monte Carlo Tree Search (MCTS).

## Motivation
This project is a learning endeavor to deepen my understanding of chess engine development and improve my skills in C++ programming, problem-solving, algorithms, and optimization.

## Installation
```bash
git clone https://github.com/bokuyemskyy/kaban.git
cd kaban

# IMPORTANT: initialize and update all submodules recursively
git submodule update --init --recursive

# Run cmake build as usual
cmake --preset release -B build
cmake --build build --preset release
```

## Usage
```bash
./out/bin/release/kaban
```

## Contributing
Feel free to open issues or submit pull requests if you have ideas, improvements, or bugs to report.

## License
MIT License. See [LICENSE](LICENSE) for details.
