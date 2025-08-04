# kaban - chess movegen with GUI

A project, aimed to be a high-performance chess engine that uses bitboard techniques. 
Inspired by [Stockfish](https://stockfishchess.org/), this project utilizes efficient board representation, move generation and modern C++ practices to achieve high perft (performance test) results.

<img width="802" height="629" alt="Screenshot_20250804_103950-1" src="https://github.com/user-attachments/assets/34ada001-ab00-466a-b595-88b31466d711" />

## Documentation
The project's future documentation you will be able to find on here: [DOCUMENTATION](https://bokuyemskyy.github.io/kaban/docs/html/) (empty by now)

## Features roadmap
### Implemented
- **Build system**: convenient and fast CMake-Ninja build system;
- **Additional configurations**: clang tools to ensure clean code, Doxygen documentation generator and VCPKG dependency manager;
- **Testing framework**: unit tests for the code;
- **Modular structure**: modular code, split into Main, Renderer, UCI, Board and Movegen.
- **Bitboards**: board representation and operations using 64-bit integers.
- **Bit-packing**: using raw bit fields inside of structures to increase efficiency and have more control over the memory; 
- **Magic bitboards**: mapping the board occupancy to an index to access precomputed rook and bishop moves;
- **Simple move generation**: accurate, but simplified (no en-passant, castling, 50-moves rule) move generation with legality checks;
### Pending
- **Complex move generation**: accurate and absolutely rule-compliant move generation;
- **UCI protocol compatibility**: integration with Universal Chess Interface for compatibility with chess GUIs;
- **Multithreading support**: parallel computations for improved performance;
- **Code cleanup**: the code is quite messy after all these experiments, so it is definitely worth it to refactor it.
  
## Future features
- **Transformation into an engine**: the project initially was aimed to be an engine, not a movegen. But the development takes too much time.
- **Evaluation function**: a scalable evaluation function with tuning capabilities.
- **Search algorithms**: Implementation of minimax, alpha-beta pruning, and potentially more advanced techniques like Principal Variation Search (PVS) and Monte Carlo Tree Search (MCTS).
- **Inline ASM**: Hot paths may work faster if specific processor instructions are used (such ones that a compiler does not utilize)

## Motivation
This project is a learning endeavor to deepen my understanding of chess engine development and improve my skills in C++ programming, problem-solving, algorithms, and optimization.

## Installation

- Have VCPKG installed on your machine
- Set VCPKG_ROOT env variable (e.g. export VCPKG_ROOT="$HOME/Code/vcpkg")

```bash
git clone https://github.com/bokuyemskyy/kaban.git
cd kaban

# Configure build
cmake -B build --preset default

# Build the app
cmake --build build --preset debug
```


## Usage
```bash
./build/Debug/kaban
```

## Contributing
Feel free to open issues or submit pull requests if you have ideas, improvements, or bugs to report.

## License
MIT License. See [LICENSE](LICENSE) for details.
