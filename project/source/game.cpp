#include <array>
#include <game.hpp>
#include <string>

#include "constants.hpp"
#include "types.hpp"

void Game::setFEN(const std::string &fen) { m_position.setFEN(fen); }

void Game::holdPiece(int index) {
    if (m_position.getPiece(Square(index)) == EMPTY) {
        m_selectedSquare = NO_SQ;
    } else {
        m_selectedSquare = Square(index);
        m_holdingPiece = true;
    }
}

void Game::releasePiece() { m_holdingPiece = false; }

const std::array<std::array<Piece, 8>, 8> &Game::getMatrixBoard() {
    return m_position.getMatrixBoard();
}

const std::vector<Move> &Game::getPossibleMoves() { return m_position.getPossibleMoves(); }
