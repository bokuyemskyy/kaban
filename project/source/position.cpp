#include <array>
#include <cstdint>
#include <error_handler.hpp>
#include <position.hpp>
#include <sstream>
#include <string>
#include <vector>

#include "constants.hpp"
#include "types.hpp"

bool Bitboard::isSet(uint8_t square) const { return (_value & (1ULL << square)) != 0; };

void Bitboard::set(uint8_t square) { _value |= (1ULL << square); }

void Bitboard::unset(uint8_t square) { _value &= (1ULL << square); }

void Position::setFEN(std::string fen) {
    std::stringstream ss(fen);
    std::string argument;
    size_t argument_index = 0;
    while (std::getline(ss, argument, ' ')) {
        switch (argument_index) {
            case 0: {
                std::pair<uint8_t, uint8_t> boardSquarePosition = {7, 0};
                for (char c : argument) {
                    if (c == '/') {
                        boardSquarePosition.first--;
                        boardSquarePosition.second = 0;
                    } else if (isdigit(c)) {
                        boardSquarePosition.second =
                            static_cast<unsigned char>(boardSquarePosition.second + (c - '0'));
                    } else {
                        setPiece(Square(boardSquarePosition.first * 8 + boardSquarePosition.second),
                                 FENtoPiece.at(c));
                        boardSquarePosition.second++;
                    }
                }
                break;
            }

            default:
                break;
        }
        argument_index++;
    }
}

void Position::setPiece(uint8_t destination, Piece piece) {
    if (piece == EMPTY) return;
    m_piece_bitboards[piece].set(destination);
    if (isWhite(piece))
        m_white_occupied.set(destination);
    else
        m_black_occupied.set(destination);
}

void Position::unsetPiece(uint8_t square) {
    if (square == NO_SQ) return;
    const uint64_t mask = ~(1ULL << square);
    for (uint8_t i = 0; i < 12; i++) {
        m_piece_bitboards[i]._value &= mask;
    }
    m_white_occupied._value &= mask;
    m_black_occupied._value &= mask;
    m_occupied._value &= mask;
}

Piece Position::getPiece(uint8_t source) const {
    if (source == NO_SQ) return EMPTY;
    const uint64_t mask = (1ULL << source);
    for (uint8_t i = 0; i < 12; i++) {
        if ((m_piece_bitboards[i]._value & mask) != 0) return Piece(i);
    }
    return EMPTY;
}

void Position::makeMove(const Move &move) {
    setPiece(move.to, getPiece(move.from));
    unsetPiece(move.from);
    // m_Turn = !m_Turn;
}

void Position::unmakeMove() {}

void Position::updateExternalData() {
    if (m_isExternalDataDirty) {
        // update matrix board
        for (uint8_t row = 0; row < 8; row++) {
            for (uint8_t col = 0; col < 8; col++) {
                m_matrixBoard[row][col] = getPiece(Square(row * 8 + col));
            }
        }
        // update moves
        // ADD
        m_isExternalDataDirty = false;
    }
}

const std::array<std::array<Piece, 8>, 8> &Position::getMatrixBoard() {
    updateExternalData();
    return m_matrixBoard;
}

const std::vector<Move> &Position::getPossibleMoves() {
    updateExternalData();
    return m_possibleMoves;
}

/*const std::vector<Move>& generateMoveMap() {
  std::vector<Move> moveMap;
  moveMap.reserve(MAX_MOVES);
  //generatePawnMoves();
};*/

/*int Position::countMoves(uint8_t depth) {
  int total = 0;
  const bool leaf = (depth == 2);
  for (const auto &move : generateMoveMap()) {
    if (depth == 1)
      m_position.makeMove(m_moveMap[i]);
    totalMoves += countMoves(depth - 1);
    m_position.unmakeMove();
  }
  m_moveMap.PopArray();
  return totalMoves;
};*/