#pragma once

#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <ostream>
#include <string>

#include "bit_operations.hpp"
#include "bitboard.hpp"
#include "castling.hpp"
#include "color.hpp"
#include "direction.hpp"
#include "en_passant.hpp"
#include "halfmove.hpp"
#include "magic.hpp"
#include "move.hpp"
#include "move_flag.hpp"
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
    Position(const std::string& fen = DEFAULT_FEN) { fromFen(fen); }

    void fromFen(const std::string& fen = DEFAULT_FEN);
    void clear() { fromFen(""); };

    [[nodiscard]] std::string toFen() const;

    UndoInfo makeMove(Move move);
    void     unmakeMove(Move move, UndoInfo undo_info);

    [[nodiscard]] auto us() const { return m_stm; }
    [[nodiscard]] auto castling() const { return m_castling; }

    [[nodiscard]] auto board() const { return m_board; }
    [[nodiscard]] auto at(Square square) const { return m_board[square.value()]; };

    [[nodiscard]] bool isLegal() const;

    template <GenerationType GT>
    Move* generateMoves(Move* move_list) {
        if constexpr (GT == GenerationType::ALL) {
            if (m_stm == Colors::WHITE)
                move_list = generatePawnMoves<Colors::WHITE>(occupancy<Side::US>(PieceTypes::PAWN), move_list);
            else
                move_list = generatePawnMoves<Colors::BLACK>(occupancy<Side::US>(PieceTypes::PAWN), move_list);

            move_list = generatePieceMoves<PieceTypes::KNIGHT>(occupancy<Side::US>(PieceTypes::KNIGHT), move_list);
            move_list = generatePieceMoves<PieceTypes::BISHOP>(occupancy<Side::US>(PieceTypes::BISHOP), move_list);
            move_list = generatePieceMoves<PieceTypes::ROOK>(occupancy<Side::US>(PieceTypes::ROOK), move_list);
            move_list = generatePieceMoves<PieceTypes::QUEEN>(occupancy<Side::US>(PieceTypes::QUEEN), move_list);
            move_list = generatePieceMoves<PieceTypes::KING>(occupancy<Side::US>(PieceTypes::KING), move_list);
            return move_list;
        } else if constexpr (GT == GenerationType::LEGAL) {
            Move* legal_move_list = move_list;

            move_list = generateMoves<GenerationType::ALL>(move_list);

            for (Move* i = legal_move_list; i < move_list; i++) {
                UndoInfo undo_info = makeMove(*i);
                if (isLegal()) *legal_move_list++ = *i;
                unmakeMove(*i, undo_info);
            }

            return legal_move_list;
        }
    };
    uint64_t perftRoot(int depth) {
        static constexpr size_t MAX_MOVES = 256;
        static constexpr size_t MAX_DEPTH = 256;

        static std::array<Move, MAX_MOVES * MAX_DEPTH> move_stack;
        uint64_t                                       total = 0;

        using Clock = std::chrono::high_resolution_clock;
        auto start  = Clock::now();

        Move* move_list_end = generateMoves<GenerationType::ALL>(move_stack.data());

        for (Move* i = move_stack.data(); i < move_list_end; ++i) {
            UndoInfo undo = makeMove(*i);
            if (isLegal()) {
                uint64_t nodes = (depth == 1) ? 1 : perft(depth - 1, move_list_end);
                std::cout << i->from().toString() << i->to().toString() << ": " << nodes << std::endl;
                total += nodes;
            }
            unmakeMove(*i, undo);
        }

        auto                          end     = Clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Total nodes: " << total << "\n";
        std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
        std::cout << "Nodes per second: " << std::floor(total / elapsed.count()) << std::endl;

        return total;
    }

    uint64_t perft(int depth, Move* move_list) {
        if (depth == 0) return 1;

        Move*    move_list_end = generateMoves<GenerationType::ALL>(move_list);
        uint64_t nodes         = 0;

        for (Move* i = move_list; i < move_list_end; ++i) {
            UndoInfo undo = makeMove(*i);
            if (isLegal()) [[likely]] {
                nodes += perft(depth - 1, move_list_end);
            }
            unmakeMove(*i, undo);
        }

        return nodes;
    }

    template <Side S>
    [[nodiscard]] Bitboard occupancy() const {
        if constexpr (S == Side::US)
            return m_color[m_stm.value()];
        else if constexpr (S == Side::THEM)
            return m_color[(!m_stm).value()];
        else
            return m_color[Colors::WHITE.value()] | m_color[Colors::BLACK.value()];
    }

    template <Side S>
    [[nodiscard]] Bitboard occupancy(PieceType pt) const {
        return occupancy<S>() & m_piece_type[pt.value()];
    }

    [[nodiscard]] Bitboard occupancy(PieceType pt) const { return m_piece_type[pt.value()]; }

   private:
    void set(Square square, Piece p);
    void unset(Square square);

    std::array<Piece, Square::count()>       m_board{};
    std::array<Bitboard, Color::count()>     m_color{Bitboards::ZERO};
    std::array<Bitboard, PieceType::count()> m_piece_type{Bitboards::ZERO};

    Color     m_stm      = Colors::WHITE;
    Castling  m_castling = Castlings::ANY;
    EnPassant m_en_passant;
    Halfmove  m_halfmove;

    Magics m_magics;

    template <PieceType PT>
    [[nodiscard]] constexpr Bitboard pseudoAttacks(Square square) const {
        if constexpr (PT == PieceTypes::KNIGHT) {
            static constexpr auto table = Bitboard::pseudoAttacks(PieceTypes::KNIGHT.directions());
            return table[square.value()];
        } else if constexpr (PT == PieceTypes::KING) {
            static constexpr auto table = Bitboard::pseudoAttacks(PieceTypes::KING.directions());
            return table[square.value()];
        } else if constexpr (PT == PieceTypes::BISHOP) {
            return m_magics.lookup<PieceTypes::BISHOP>(square, occupancy<Side::BOTH>());
        } else if constexpr (PT == PieceTypes::ROOK) {
            return m_magics.lookup<PieceTypes::ROOK>(square, occupancy<Side::BOTH>());
        } else if constexpr (PT == PieceTypes::QUEEN) {
            return m_magics.lookup<PieceTypes::ROOK>(square, occupancy<Side::BOTH>()) |
                   m_magics.lookup<PieceTypes::BISHOP>(square, occupancy<Side::BOTH>());
        }
    }

    template <Color C>
    [[nodiscard]] constexpr Bitboard pawnAttacks(Square square) const {
        if constexpr (C == Colors::WHITE) {
            static constexpr auto table = Bitboard::pseudoAttacks(PieceTypes::PAWN.directions(Colors::WHITE));
            return table[square.value()];
        } else {
            static constexpr auto table = Bitboard::pseudoAttacks(PieceTypes::PAWN.directions(Colors::BLACK));
            return table[square.value()];
        }
    }

    template <Color C>
    Move* generatePawnMoves(Bitboard pieces, Move* move_list) const {
        while (pieces.hasValue()) {
            Square from = poplsb(pieces);

            Bitboard attacks = pawnAttacks<C>(from) & occupancy<Side::THEM>();
            while (attacks.hasValue()) *move_list++ = Move(from, poplsb(attacks));

            if (m_en_passant.hasValue()) {
                if constexpr (C == Colors::WHITE) {
                    if ((pawnAttacks<C>(from) & Bitboard::square(Square(m_en_passant.file(), Ranks::R6))).hasValue())
                        *move_list++ = Move(from, Square(m_en_passant.file(), Ranks::R6), MoveFlags::EN_PASSANT);
                } else {
                    if ((pawnAttacks<C>(from) & Bitboard::square(Square(m_en_passant.file(), Ranks::R3))).hasValue()) {
                        *move_list++ = Move(from, Square(m_en_passant.file(), Ranks::R3), MoveFlags::EN_PASSANT);
                    }
                }
            }

            Square single_push = from.shifted(C == Colors::WHITE ? Directions::N : Directions::S);
            if ((Bitboard::square(single_push) & ~occupancy<Side::BOTH>()).hasValue()) {
                *move_list++ = Move(from, single_push);

                if constexpr (C == Colors::WHITE) {
                    if (from.rank() == Ranks::R2) {
                        Square double_push = single_push.shifted(Directions::N);
                        if ((Bitboard::square(double_push) & ~occupancy<Side::BOTH>()).hasValue())
                            *move_list++ = Move(from, double_push, MoveFlags::PAWN_DOUBLE_PUSH);
                    }
                } else {
                    if (from.rank() == Ranks::R7) {
                        Square double_push = single_push.shifted(Directions::S);
                        if ((Bitboard::square(double_push) & ~occupancy<Side::BOTH>()).hasValue())
                            *move_list++ = Move(from, double_push, MoveFlags::PAWN_DOUBLE_PUSH);
                    }
                }
            }
        }
        return move_list;
    }

    template <PieceType PT>
    Move* generatePieceMoves(Bitboard pieces, Move* move_list) const {
        while (pieces.hasValue()) {
            Square   from    = poplsb(pieces);
            Bitboard targets = pseudoAttacks<PT>(from) & ~occupancy<Side::US>();
            while (targets.hasValue()) {
                *move_list++ = Move(from, poplsb(targets));
            }
        }
        return move_list;
    }

    static constexpr auto DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w";
};