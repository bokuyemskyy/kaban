#pragma once

#include <string>
#include <vector>

#include "bit_operations.hpp"
#include "bitboard.hpp"
#include "castling.hpp"
#include "direction.hpp"
#include "en_passant.hpp"
#include "halfmove.hpp"
#include "move.hpp"
#include "piece.hpp"
#include "piece_type.hpp"
#include "square.hpp"
#include "undo_info.hpp"

enum class Side : uint8_t {
    US,
    THEM,
    BOTH
};

enum class GenerationType : uint8_t {
    ALL,
    LEGAL
};

class Position {
   public:
    Position(const std::string& fen = DEFAULT_FEN) { set_from_fen(fen); }

    void set_from_fen(const std::string& fen = DEFAULT_FEN);
    void clear() { set_from_fen(""); };

    [[nodiscard]] std::string to_fen() const;

    void do_move(Move move);
    void undo_move(Move move, UndoInfo undoInfo);

    [[nodiscard]] auto turn() const { return m_sideToMove; }
    [[nodiscard]] auto castling() const { return m_castling; }

    [[nodiscard]] auto board() const { return m_board; }
    [[nodiscard]] auto at(Square square) const { return m_board[square.value()]; };

    [[nodiscard]] bool is_legal() const;

    template <PieceType::ValueType PT>
    [[nodiscard]] constexpr Bitboard pseudo_attacks(Square square) const {
        if constexpr (PT == PieceTypes::KNIGHT.value()) {
            constexpr std::array<Direction, 8> dirs  = {Directions::NNE, Directions::NNW, Directions::ENE,
                                                        Directions::WNW, Directions::SSE, Directions::SSW,
                                                        Directions::ESE, Directions::WSW};
            static constexpr auto              table = Bitboard::pseudo_attacks(dirs);
            return table[square.value()];
        } else if constexpr (PT == PieceTypes::KING.value()) {
            constexpr std::array<Direction, 8> dirs = {Directions::E,  Directions::N,  Directions::W,  Directions::S,
                                                       Directions::NE, Directions::NW, Directions::SE, Directions::SW};
            static constexpr std::array<Bitboard, Square::count()> table = Bitboard::pseudo_attacks(dirs);
            return table[square.value()];
        }
    };

    template <GenerationType GT>
    [[nodiscard]] std::vector<Move> generate_moves() const {
        std::vector<Move> moves;

        Bitboard pieces = occupancy<Side::US>(PieceTypes::PAWN);
        Bitboard piece_moves;

        // while (pieces.has_value()) {
        //  for each pawn we do poplsb and generate moves
        //}

        Square s = Squares::B1.moved(Directions::NNW);

        pieces = occupancy<Side::US>(PieceTypes::KNIGHT);
        while (pieces.has_value()) {
            Square from = poplsb(pieces);

            piece_moves = pseudo_attacks<PieceTypes::KNIGHT.value()>(from) & ~occupancy<Side::US>();
            while (piece_moves.has_value()) {
                Square to = poplsb(piece_moves);
                moves.emplace_back(Move(from, to));
            }
        }

        pieces = occupancy<Side::US>(PieceTypes::KING);
        while (pieces.has_value()) {
            Square from = poplsb(pieces);

            piece_moves = pseudo_attacks<PieceTypes::KING.value()>(from) & ~occupancy<Side::US>();
            while (piece_moves.has_value()) {
                Square to = poplsb(piece_moves);
                moves.emplace_back(Move(from, to));
            }
        }

        //...
        return moves;
    }

    template <Side S>
    [[nodiscard]] Bitboard occupancy() const {
        if constexpr (S == Side::US)
            return m_color[m_sideToMove.value()];
        else if constexpr (S == Side::THEM)
            return m_color[(!m_sideToMove).value()];
        else
            return m_color[Colors::WHITE.value()] | m_color[Colors::BLACK.value()];
    }

    template <Side S>
    [[nodiscard]] Bitboard occupancy(PieceType pt) const {
        return occupancy<S>() & m_pieceType[pt.value()];
    }

   private:
    void set(Square square, Piece p);
    void unset(Square square);

    std::array<Piece, Square::count()>       m_board{};
    std::array<Bitboard, Color::count()>     m_color{Bitboards::ZERO};
    std::array<Bitboard, PieceType::count()> m_pieceType{Bitboards::ZERO};

    Color     m_sideToMove = Colors::WHITE;
    Castling  m_castling   = Castlings::ANY;
    EnPassant m_en_passant;
    Halfmove  m_halfmove;

    static constexpr auto DEFAULT_FEN       = "rnbqkbnr/pppppppp/8/8/8/5p2/PP3PPP/RNBQKBNR w";
    static constexpr int  MAX_DEPTH         = 64;
    static constexpr int  MAX_MOVES_PER_POS = 256;
};