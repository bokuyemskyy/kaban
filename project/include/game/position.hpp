#ifndef POSITION_HPP
#define POSITION_HPP

#include <array>
#include <cstddef>
#include <iostream>
#include <list>
#include <shared_mutex>
#include <string>
#include <vector>

#include "bitboard.hpp"
#include "move.hpp"
#include "piece.hpp"
#include "square.hpp"
#include "utils.hpp"

constexpr auto DEFAULT_FEN       = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
const int      MAX_DEPTH         = 64;  // Hard cap for recursion
const int      MAX_MOVES_PER_POS = 256;

class Position {
   public:
    Position(const std::string &fen = DEFAULT_FEN) {
        setFromFEN(fen);
        updateCachedState();
        m_moveBuffer.reserve(MAX_MOVES_PER_POS * MAX_DEPTH);
    }

    void                      setFromFEN(const std::string &fen);
    [[nodiscard]] std::string toFEN() const;

    void setPiece(Square square, Piece p);
    void unsetPiece(Square square);
    void resetBoard();

    [[nodiscard]] Piece pieceAt(Square square) const { return m_board[square]; };

    void updateCachedState();
    void toggleTurn();

    [[nodiscard]] bool isLegal();

    size_t generatePseudoLegalMoves(std::vector<Move> &moves);
    template <bool Verbose = false>
    int perft(uint8_t depth) {
        if (depth == 0) return 1;

        const size_t start = m_moveBuffer.size();
        const size_t count = generatePseudoLegalMoves(m_moveBuffer);
        int          nodes = 0;

        for (size_t i = 0; i < count; ++i) {
            const Move &move = m_moveBuffer[start + i];
            doMove(move);
            if (isLegal()) [[likely]] {
                int child = perft(depth - 1);
                if constexpr (Verbose) {
                    std::cout << squareToString(getFrom(move)) << squareToString(getTo(move)) << ": " << child << '\n';
                }
                nodes += child;
            }
            undoMove();
        }
        m_moveBuffer.resize(start);
        return nodes;
    }
    void makeMoveUci(std::string moveUci) {
        Move move = createMove(stringToSquare(moveUci.substr(0, 2)), stringToSquare(moveUci.substr(2, 2)), 0);
        doMove(move);
    }

    void                doMove(Move move);
    void                undoMove();
    [[nodiscard]] Color turn() const { return m_turn; }

   private:
    std::vector<Move> m_moveBuffer;

    std::array<Piece, Squares::NB>       m_board{};
    std::array<Bitboard, Colors::NB>     m_colorBB{BITBOARD_ZERO};
    std::array<Bitboard, PieceTypes::NB> m_pieceTypeBB{BITBOARD_ZERO};

    std::list<Delta> m_deltas;
    std::list<Move>  m_moves;

    Color    m_turn      = Colors::WHITE;
    Castling m_castling  = Castlings::ANY;
    uint8_t  m_halfmoves = 0;

    Bitboard m_occupancy{};
    Square   m_wking{};
    Square   m_bking{};
    Color    m_us{};
    Color    m_them{};
};

#endif