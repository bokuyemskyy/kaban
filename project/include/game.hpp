#ifndef GAME_HPP
#define GAME_HPP

#include <imgui.h>

#include <vector>

#include "position.hpp"

constexpr auto DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

class Game {
   public:
    // Improved method names for better clarity
    [[nodiscard]] bool isHoldingPiece() const { return m_holdingPiece; }
    [[nodiscard]] Square getSelectedSquare() const { return m_selectedSquare; }

    const std::array<std::array<Piece, BOARD_SIZE>, BOARD_SIZE> &getMatrixBoard();

    const std::vector<Move> &getPossibleMoves();

    void holdPiece(int index);

    void releasePiece();

   private:
    Position m_position;

    // Renamed variables to make them more descriptive
    bool m_holdingPiece = false;
    Square m_selectedSquare = NO_SQ;
};

#endif