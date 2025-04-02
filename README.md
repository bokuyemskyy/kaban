# kaban - chess bitboard engine

A project, aimed to be a high-performance chess engine that uses bitboard techniques. Inspired by [Stockfish](https://stockfishchess.org/), this project aims to explore efficient board representation, move generation, evaluation functions, and search algorithms using modern C++ practices.

## Documentation
The draft of documentation you can find on here: [DOCUMENTATION](docs/html/index.html)

## Features (Existing and planned)
- ✅ **Bitboard Representation**: Efficient board representation using 64-bit integers.
- 🔄 **Move Generation**: Fast, accurate move generation with legality checks.
- 🔄 **Evaluation Function**: A scalable evaluation function with tuning capabilities.
- 🔄 **Search Algorithms**: Implementation of minimax, alpha-beta pruning, and potentially more advanced techniques like Principal Variation Search (PVS) and Monte Carlo Tree Search (MCTS).
- 🔄 **Multithreading Support**: Parallel search for improved performance.
- 🔄 **UCI Protocol Compatibility**: Integration with Universal Chess Interface for compatibility with chess GUIs.
- 🔄 **Testing Framework**: Unit tests for move generation, evaluation, and search algorithms.

## Motivation
Inspired by Stockfish, this project is a learning endeavor to deepen my understanding of chess engine development and improve my skills in C++ programming, algorithms, and optimization.

## Installation
```bash
git clone https://github.com/bokuyemskyy/kaban.git
cd kaban
cmake -Bbuild --preset release
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