#pragma once

#include <array>
#include <cassert>
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
#include "magics.hpp"
#include "move.hpp"
#include "move_flag.hpp"
#include "piece.hpp"
#include "piece_type.hpp"
#include "square.hpp"
#include "undo_info.hpp"

enum class Sides : uint8_t {
    US,
    THEM,
    BOTH
};

enum class GenerationTypes : uint8_t {
    ALL,
    LEGAL
};

class Position {
   public:
    explicit Position(const std::string& fen = DEFAULT_FEN) { fromFen(fen); }

    void fromFen(const std::string& fen = DEFAULT_FEN);
    void reset();

    [[nodiscard]] std::string toFen() const;

    UndoInfo makeMove(Move move);
    void     unmakeMove(Move move, const UndoInfo& undo_info);

    [[nodiscard]] auto us() const { return m_stm; }
    [[nodiscard]] auto castling() const { return m_castling; }

    [[nodiscard]] const auto& board() const { return m_board; }

    Piece&                        at(Square square) { return m_board[square.value()]; }
    [[nodiscard]] const Piece&    at(Square square) const { return m_board[square.value()]; }
    Bitboard&                     at(Color color) { return m_color[color.value()]; }
    [[nodiscard]] const Bitboard& at(Color color) const { return m_color[color.value()]; }
    Bitboard&                     at(PieceType piece_type) { return m_piece_type[piece_type.value()]; }
    [[nodiscard]] const Bitboard& at(PieceType piece_type) const { return m_piece_type[piece_type.value()]; }

    template <bool F>
    [[nodiscard]] bool isLegal() const {
        if constexpr (F) {  // first check, when new position is set. the moves are guaranteed to be legal
            if (popcount(occupancy<Sides::US>(PieceTypes::KING)) != 1) return false;
            if (popcount(occupancy<Sides::THEM>(PieceTypes::KING)) != 1) return false;
            if (popcount(occupancy<Sides::US>(PieceTypes::PAWN)) > 8) return false;
            if (popcount(occupancy<Sides::THEM>(PieceTypes::PAWN)) > 8) return false;
            if (popcount(occupancy<Sides::US>()) > 16) return false;
            if (popcount(occupancy<Sides::THEM>()) > 16) return false;
            if ((occupancy<Sides::BOTH>(PieceTypes::PAWN) & (Bitboard::rank(Ranks::R1) | Bitboard::rank(Ranks::R8)))
                    .any())
                return false;
        }

        Square king = lsb(occupancy<Sides::THEM>(PieceTypes::KING));

        return isSafe<Sides::US>(king);
    }

    template <Sides Attacker>
    [[nodiscard]] bool isSafe(Square square) const {
        assert(Attacker == Sides::US || Attacker == Sides::THEM);
        if constexpr (Attacker == Sides::US) {
            if (m_stm == Colors::WHITE) {
                if ((pawnAttacks<Colors::BLACK>(square) & occupancy<Attacker>(PieceTypes::PAWN)).any()) return false;
            } else {
                if ((pawnAttacks<Colors::WHITE>(square) & occupancy<Attacker>(PieceTypes::PAWN)).any()) return false;
            }
        } else {
            if (m_stm == Colors::WHITE) {
                if ((pawnAttacks<Colors::WHITE>(square) & occupancy<Attacker>(PieceTypes::PAWN)).any()) return false;
            } else {
                if ((pawnAttacks<Colors::BLACK>(square) & occupancy<Attacker>(PieceTypes::PAWN)).any()) return false;
            }
        }
        if ((pseudoAttacks<PieceTypes::KING>(square) & occupancy<Attacker>(PieceTypes::KING)).any()) return false;
        if ((pseudoAttacks<PieceTypes::KNIGHT>(square) & occupancy<Attacker>(PieceTypes::KNIGHT)).any()) return false;
        if ((pseudoAttacks<PieceTypes::BISHOP>(square) &
             (occupancy<Attacker>(PieceTypes::BISHOP) | occupancy<Attacker>(PieceTypes::QUEEN)))
                .any())
            return false;

        if ((pseudoAttacks<PieceTypes::ROOK>(square) &
             (occupancy<Attacker>(PieceTypes::ROOK) | occupancy<Attacker>(PieceTypes::QUEEN)))
                .any())
            return false;

        return true;
    }

    template <GenerationTypes GT>
    Move* generateMoves(Move* move_list) {
        if constexpr (GT == GenerationTypes::ALL) {
            if (m_stm == Colors::WHITE)
                move_list = generatePawnMoves<Colors::WHITE>(occupancy<Sides::US>(PieceTypes::PAWN), move_list);
            else
                move_list = generatePawnMoves<Colors::BLACK>(occupancy<Sides::US>(PieceTypes::PAWN), move_list);

            move_list = generatePieceMoves<PieceTypes::KNIGHT>(occupancy<Sides::US>(PieceTypes::KNIGHT), move_list);
            move_list = generatePieceMoves<PieceTypes::BISHOP>(occupancy<Sides::US>(PieceTypes::BISHOP), move_list);
            move_list = generatePieceMoves<PieceTypes::ROOK>(occupancy<Sides::US>(PieceTypes::ROOK), move_list);
            move_list = generatePieceMoves<PieceTypes::QUEEN>(occupancy<Sides::US>(PieceTypes::QUEEN), move_list);
            move_list = generatePieceMoves<PieceTypes::KING>(occupancy<Sides::US>(PieceTypes::KING), move_list);

            move_list = generateCastling(move_list);
            return move_list;
        } else if constexpr (GT == GenerationTypes::LEGAL) {
            Move* legal_move_list = move_list;

            move_list = generateMoves<GenerationTypes::ALL>(move_list);

            for (Move* i = legal_move_list; i < move_list; i++) {
                UndoInfo undo_info = makeMove(*i);
                if (isLegal<false>()) *legal_move_list++ = *i;
                unmakeMove(*i, undo_info);
            }

            return legal_move_list;
        } else {
            return move_list;
        }
    }
    uint64_t perftRoot(int depth) {
        static constexpr size_t MAX_MOVES = 256;
        static constexpr size_t MAX_DEPTH = 10;

        static std::array<Move, MAX_MOVES * MAX_DEPTH> move_stack;
        uint64_t                                       total = 0;

        using Clock = std::chrono::high_resolution_clock;
        auto start  = Clock::now();

        Move* move_list_end = generateMoves<GenerationTypes::ALL>(move_stack.data());

        for (Move* i = move_stack.data(); i < move_list_end; ++i) {
            UndoInfo undo = makeMove(*i);
            if (isLegal<false>()) {
                uint64_t nodes = (depth == 1) ? 1 : perft(depth - 1, move_list_end);
                std::cout << (*i).toString() << ": " << nodes << std::endl;
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

        Move*    move_list_end = generateMoves<GenerationTypes::ALL>(move_list);
        uint64_t nodes         = 0;

        for (Move* i = move_list; i < move_list_end; ++i) {
            UndoInfo undo = makeMove(*i);
            if (isLegal<false>()) [[likely]] {
                nodes += perft(depth - 1, move_list_end);
            }
            unmakeMove(*i, undo);
        }

        return nodes;
    }

    template <Sides S>
    [[nodiscard]] Bitboard occupancy() const {
        if constexpr (S == Sides::US)
            return m_color[m_stm.value()];
        else if constexpr (S == Sides::THEM)
            return m_color[(!m_stm).value()];
        else
            return m_color[Colors::WHITE.value()] | m_color[Colors::BLACK.value()];
    }

    template <Sides S>
    [[nodiscard]] Bitboard occupancy(PieceType pt) const {
        return occupancy<S>() & m_piece_type[pt.value()];
    }

    [[nodiscard]] Bitboard occupancy(PieceType pt) const { return m_piece_type[pt.value()]; }

   private:
    void setPiece(Square square, Piece p);
    void unsetPiece(Square square);
    void movePiece(Square from, Square to);

    std::array<Piece, Squares::count()>       m_board{};
    std::array<Bitboard, Colors::count()>     m_color{Bitboards::ZERO};
    std::array<Bitboard, PieceTypes::count()> m_piece_type{Bitboards::ZERO};

    Color     m_stm      = Colors::WHITE;
    Castling  m_castling = Castlings::ANY;
    EnPassant m_en_passant{};
    Halfmove  m_halfmove{};

    Magics m_magics{};

    template <PieceType PT>
    [[nodiscard]] constexpr Bitboard pseudoAttacks(Square square) const {
        if constexpr (PT == PieceTypes::KNIGHT) {
            static constexpr auto table = Bitboard::pseudoAttacks(Directions::of(PieceTypes::KNIGHT));
            return table[square.value()];
        } else if constexpr (PT == PieceTypes::KING) {
            static constexpr auto table = Bitboard::pseudoAttacks(Directions::of(PieceTypes::KING));
            return table[square.value()];
        } else if constexpr (PT == PieceTypes::BISHOP) {
            return m_magics.lookup<PieceTypes::BISHOP>(square, occupancy<Sides::BOTH>());
        } else if constexpr (PT == PieceTypes::ROOK) {
            return m_magics.lookup<PieceTypes::ROOK>(square, occupancy<Sides::BOTH>());
        } else if constexpr (PT == PieceTypes::QUEEN) {
            return m_magics.lookup<PieceTypes::ROOK>(square, occupancy<Sides::BOTH>()) |
                   m_magics.lookup<PieceTypes::BISHOP>(square, occupancy<Sides::BOTH>());
        }
    }

    template <Color C>
    [[nodiscard]] constexpr Bitboard pawnAttacks(Square square) const {
        if constexpr (C == Colors::WHITE) {
            static constexpr auto table = Bitboard::pseudoAttacks(Directions::of(Pieces::W_PAWN));
            return table[square.value()];
        } else {
            static constexpr auto table = Bitboard::pseudoAttacks(Directions::of(Pieces::B_PAWN));
            return table[square.value()];
        }
    }

    template <Color C>
    Move* generatePawnMoves(Bitboard pieces, Move* move_list) const {
        while (pieces.any()) {
            Square from = poplsb(pieces);

            Bitboard attacks = pawnAttacks<C>(from) & occupancy<Sides::THEM>();
            while (attacks.any()) {
                Square to = poplsb(attacks);
                if (to.rank() == (C == Colors::WHITE ? Ranks::R8 : Ranks::R1)) {
                    *move_list++ = Move(from, to, MoveFlags::PROMOTION_QUEEN);
                    *move_list++ = Move(from, to, MoveFlags::PROMOTION_ROOK);
                    *move_list++ = Move(from, to, MoveFlags::PROMOTION_BISHOP);
                    *move_list++ = Move(from, to, MoveFlags::PROMOTION_KNIGHT);
                } else {
                    *move_list++ = Move(from, to);
                }
            }

            if (m_en_passant.hasValue()) {
                if constexpr (C == Colors::WHITE) {
                    if ((pawnAttacks<C>(from) & Bitboard::square(Square(m_en_passant.file(), Ranks::R6))).any())
                        *move_list++ = Move(from, Square(m_en_passant.file(), Ranks::R6), MoveFlags::EN_PASSANT);
                } else {
                    if ((pawnAttacks<C>(from) & Bitboard::square(Square(m_en_passant.file(), Ranks::R3))).any()) {
                        *move_list++ = Move(from, Square(m_en_passant.file(), Ranks::R3), MoveFlags::EN_PASSANT);
                    }
                }
            }

            Square single_push = from.move(C == Colors::WHITE ? Directions::N : Directions::S);
            if ((Bitboard::square(single_push) & ~occupancy<Sides::BOTH>()).any()) {
                if (single_push.rank() == (C == Colors::WHITE ? Ranks::R8 : Ranks::R1)) {
                    *move_list++ = Move(from, single_push, MoveFlags::PROMOTION_QUEEN);
                    *move_list++ = Move(from, single_push, MoveFlags::PROMOTION_ROOK);
                    *move_list++ = Move(from, single_push, MoveFlags::PROMOTION_BISHOP);
                    *move_list++ = Move(from, single_push, MoveFlags::PROMOTION_KNIGHT);
                } else {
                    *move_list++ = Move(from, single_push);

                    if constexpr (C == Colors::WHITE) {
                        if (from.rank() == Ranks::R2) {
                            Square double_push = single_push.move(Directions::N);
                            if ((Bitboard::square(double_push) & ~occupancy<Sides::BOTH>()).any())
                                *move_list++ = Move(from, double_push, MoveFlags::PAWN_DOUBLE_PUSH);
                        }
                    } else {
                        if (from.rank() == Ranks::R7) {
                            Square double_push = single_push.move(Directions::S);
                            if ((Bitboard::square(double_push) & ~occupancy<Sides::BOTH>()).any())
                                *move_list++ = Move(from, double_push, MoveFlags::PAWN_DOUBLE_PUSH);
                        }
                    }
                }
            }
        }
        return move_list;
    }

    template <PieceType PT>
    Move* generatePieceMoves(Bitboard pieces, Move* move_list) const {
        while (pieces.any()) {
            Square   from    = poplsb(pieces);
            Bitboard targets = pseudoAttacks<PT>(from) & ~occupancy<Sides::US>();
            while (targets.any()) {
                *move_list++ = Move(from, poplsb(targets));
            }
        }
        return move_list;
    }

    Move* generateCastling(Move* move_list) const {
        if (m_stm == Colors::WHITE) {
            if (m_castling.has(Castlings::W_KING_SIDE) &&
                !(Bitboard::between(Squares::E1, Squares::H1) & occupancy<Sides::BOTH>()).any()) {
                if (isSafe<Sides::THEM>(Squares::E1) && isSafe<Sides::THEM>(Squares::F1))
                    *move_list++ = Move(Squares::E1, Squares::G1, MoveFlags::CASTLING_KING);
            }
            if (m_castling.has(Castlings::W_QUEEN_SIDE) &&
                !(Bitboard::between(Squares::E1, Squares::A1) & occupancy<Sides::BOTH>()).any()) {
                if (isSafe<Sides::THEM>(Squares::E1) && isSafe<Sides::THEM>(Squares::D1))
                    *move_list++ = Move(Squares::E1, Squares::C1, MoveFlags::CASTLING_QUEEN);
            }
        } else {
            if (m_castling.has(Castlings::B_KING_SIDE) &&
                !(Bitboard::between(Squares::E8, Squares::H8) & occupancy<Sides::BOTH>()).any()) {
                if (isSafe<Sides::THEM>(Squares::E8) && isSafe<Sides::THEM>(Squares::F8))
                    *move_list++ = Move(Squares::E8, Squares::G8, MoveFlags::CASTLING_KING);
            }
            if (m_castling.has(Castlings::B_QUEEN_SIDE) &&
                !(Bitboard::between(Squares::E8, Squares::A8) & occupancy<Sides::BOTH>()).any()) {
                if (isSafe<Sides::THEM>(Squares::E8) && isSafe<Sides::THEM>(Squares::D8))
                    *move_list++ = Move(Squares::E8, Squares::C8, MoveFlags::CASTLING_QUEEN);
            }
        }
        return move_list;
    }

    static constexpr auto DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w";
};