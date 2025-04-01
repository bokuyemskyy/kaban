#ifndef GAME_HPP
#define GAME_HPP

#include <imgui.h>

#include <cstdint>
#include <position.hpp>
#include <string>
#include <vector>

// Extracted constant for the default FEN string
constexpr auto DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

class Game {
   public:
    // Pass FEN string by const reference to avoid unnecessary copies
    void setFEN(const std::string &fen);

    // Improved method names for better clarity
    [[nodiscard]] Turn getTurn() const { return m_position.getTurn(); }
    [[nodiscard]] bool isHoldingPiece() const { return m_holdingPiece; }
    [[nodiscard]] Square getSelectedSquare() const { return m_selectedSquare; }

    const std::array<std::array<Piece, BOARD_SIZE>, BOARD_SIZE> &getMatrixBoard();

    const std::vector<Move> &getPossibleMoves();

    int countMoves(uint8_t depth);

    void holdPiece(int index);

    void releasePiece();

   private:
    Position m_position;

    // Renamed variables to make them more descriptive
    bool m_holdingPiece = false;
    Square m_selectedSquare = NO_SQ;
};

#endif