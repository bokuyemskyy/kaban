#include "game.hpp"

#include <array>

#include "types.hpp"

void Game::holdPiece(int index) {
    if (m_position.pieceAt(Square(index)) == Piece::NONE) {
        m_selectedSquare = Square::NONE;
    } else {
        m_selectedSquare = Square(index);
        m_holdingPiece = true;
    }
}

void Game::releasePiece() { m_holdingPiece = false; }
