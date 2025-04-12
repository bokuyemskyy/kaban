#ifndef GAME_HPP
#define GAME_HPP

#include <imgui.h>

#include <vector>

#include "position.hpp"
#include "types.hpp"

class Game {
   public:
    [[nodiscard]] bool isHoldingPiece() const { return m_holdingPiece; }
    [[nodiscard]] Square getSelectedSquare() const { return m_selectedSquare; }

    const std::array<std::array<Piece, BOARD_SIZE>, BOARD_SIZE> &getMatrixBoard();

    const std::vector<Move> &getPossibleMoves();

    void holdPiece(int index);

    void releasePiece();

   private:
    Position m_position;

    bool m_holdingPiece = false;
    Square m_selectedSquare = Square::NONE;
};

#endif