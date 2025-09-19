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
    void     unmakeMove(Move move, const UndoInfo& undo_info);

    [[nodiscard]] auto us() const { return m_stm; }
    [[nodiscard]] auto castling() const { return m_castling; }

    [[nodiscard]] auto board() const { return m_board; }

    Piece&                        at(Square square) { return m_board[square.value()]; }
    [[nodiscard]] const Piece&    at(Square square) const { return m_board[square.value()]; }
    Bitboard&                     at(Color color) { return m_color[color.value()]; }
    [[nodiscard]] const Bitboard& at(Color color) const { return m_color[color.value()]; }
    Bitboard&                     at(PieceType piece_type) { return m_piece_type[piece_type.value()]; }
    [[nodiscard]] const Bitboard& at(PieceType piece_type) const { return m_piece_type[piece_type.value()]; }

    template <bool F>
    [[nodiscard]] bool isLegal() const {
        if constexpr (F) {  // first check, when new position is set. the moves are guaranteed to be legal
            if (popcount(occupancy<Side::US>(PieceTypes::KING)) != 1) return false;
            if (popcount(occupancy<Side::THEM>(PieceTypes::KING)) != 1) return false;
            if (popcount(occupancy<Side::US>(PieceTypes::PAWN)) > 8) return false;
            if (popcount(occupancy<Side::THEM>(PieceTypes::PAWN)) > 8) return false;
            if (popcount(occupancy<Side::US>()) > 16) return false;
            if (popcount(occupancy<Side::THEM>()) > 16) return false;
            if ((occupancy<Side::BOTH>(PieceTypes::PAWN) & (Bitboard::rank(Ranks::R1) | Bitboard::rank(Ranks::R8)))
                    .any())
                return false;
        }

        Square king = lsb(occupancy<Side::THEM>(PieceTypes::KING));

        return isUnderAttack<Side::US>(king);
    }

    template <Side S>
    [[nodiscard]] bool isUnderAttack(Square square) const {
        if (m_stm == Colors::WHITE) {
            if ((pawnAttacks<Colors::BLACK>(square) & occupancy<S>(PieceTypes::PAWN)).any()) return false;
        } else {
            if ((pawnAttacks<Colors::WHITE>(square) & occupancy<S>(PieceTypes::PAWN)).any()) return false;
        }
        if ((pseudoAttacks<PieceTypes::KING>(square) & occupancy<S>(PieceTypes::KING)).any()) return false;
        if ((pseudoAttacks<PieceTypes::KNIGHT>(square) & occupancy<S>(PieceTypes::KNIGHT)).any()) return false;
        if ((pseudoAttacks<PieceTypes::BISHOP>(square) &
             (occupancy<S>(PieceTypes::BISHOP) | occupancy<S>(PieceTypes::QUEEN)))
                .any())
            return false;

        if ((pseudoAttacks<PieceTypes::ROOK>(square) &
             (occupancy<S>(PieceTypes::ROOK) | occupancy<S>(PieceTypes::QUEEN)))
                .any())
            return false;

        return true;
    }

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

            // move_list = generateCastling(move_list);
            return move_list;
        } else if constexpr (GT == GenerationType::LEGAL) {
            Move* legal_move_list = move_list;

            move_list = generateMoves<GenerationType::ALL>(move_list);

            for (Move* i = legal_move_list; i < move_list; i++) {
                UndoInfo undo_info = makeMove(*i);
                if (isLegal<false>()) *legal_move_list++ = *i;
                unmakeMove(*i, undo_info);
            }

            return legal_move_list;
        }
    };
    uint64_t perftRoot(int depth) {
        static constexpr size_t MAX_MOVES = 256;
        static constexpr size_t MAX_DEPTH = 10;

        static std::array<Move, MAX_MOVES * MAX_DEPTH> move_stack;
        uint64_t                                       total = 0;

        using Clock = std::chrono::high_resolution_clock;
        auto start  = Clock::now();

        Move* move_list_end = generateMoves<GenerationType::ALL>(move_stack.data());

        for (Move* i = move_stack.data(); i < move_list_end; ++i) {
            UndoInfo undo = makeMove(*i);
            if (isLegal<false>()) {
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
            if (isLegal<false>()) [[likely]] {
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
    void setPiece(Square square, Piece p);
    void unsetPiece(Square square);
    void movePiece(Square from, Square to);

    std::array<Piece, Squares::count()>      m_board{};
    std::array<Bitboard, Colors::count()>    m_color{Bitboards::ZERO};
    std::array<Bitboard, PieceType::count()> m_piece_type{Bitboards::ZERO};

    Color     m_stm      = Colors::WHITE;
    Castling  m_castling = Castlings::ANY;
    EnPassant m_en_passant;
    Halfmove  m_halfmove;

    Magics m_magics;

    template <PieceType PT>
    [[nodiscard]] constexpr Bitboard pseudoAttacks(Square square) const {
        if constexpr (PT == PieceTypes::KNIGHT) {
            static constexpr auto table = Bitboard::pseudoAttacks(Directions::of(PieceTypes::KNIGHT));
            return table[square.value()];
        } else if constexpr (PT == PieceTypes::KING) {
            static constexpr auto table = Bitboard::pseudoAttacks(Directions::of(PieceTypes::KING));
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

            Bitboard attacks = pawnAttacks<C>(from) & occupancy<Side::THEM>();
            while (attacks.any()) *move_list++ = Move(from, poplsb(attacks));

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
            if ((Bitboard::square(single_push) & ~occupancy<Side::BOTH>()).any()) {
                *move_list++ = Move(from, single_push);

                if constexpr (C == Colors::WHITE) {
                    if (from.rank() == Ranks::R2) {
                        Square double_push = single_push.move(Directions::N);
                        if ((Bitboard::square(double_push) & ~occupancy<Side::BOTH>()).any())
                            *move_list++ = Move(from, double_push, MoveFlags::PAWN_DOUBLE_PUSH);
                    }
                } else {
                    if (from.rank() == Ranks::R7) {
                        Square double_push = single_push.move(Directions::S);
                        if ((Bitboard::square(double_push) & ~occupancy<Side::BOTH>()).any())
                            *move_list++ = Move(from, double_push, MoveFlags::PAWN_DOUBLE_PUSH);
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
            Bitboard targets = pseudoAttacks<PT>(from) & ~occupancy<Side::US>();
            while (targets.any()) {
                *move_list++ = Move(from, poplsb(targets));
            }
        }
        return move_list;
    }

    Move* generateCastling(Move* move_list) const {
        if (m_stm == Colors::WHITE) {
            if (m_castling.has(Castlings::W_KING_SIDE) &&
                !(Bitboard::between(Squares::E1, Squares::H1) & occupancy<Side::BOTH>()).any()) {
                if (!isUnderAttack<Side::THEM>(Squares::E1) && !isUnderAttack<Side::THEM>(Squares::F1))
                    *move_list++ = Move(Squares::E1, Squares::G1, MoveFlags::CASTLING_KING);
            }
            if (m_castling.has(Castlings::W_QUEEN_SIDE) &&
                !(Bitboard::between(Squares::E1, Squares::A1) & occupancy<Side::BOTH>()).any()) {
                if (!isUnderAttack<Side::THEM>(Squares::E1) && !isUnderAttack<Side::THEM>(Squares::D1))
                    *move_list++ = Move(Squares::E1, Squares::C1, MoveFlags::CASTLING_QUEEN);
            }
        } else {
            if (m_castling.has(Castlings::B_KING_SIDE) &&
                !(Bitboard::between(Squares::E8, Squares::H8) & occupancy<Side::BOTH>()).any()) {
                if (!isUnderAttack<Side::THEM>(Squares::E8) && !isUnderAttack<Side::THEM>(Squares::F8))
                    *move_list++ = Move(Squares::E8, Squares::G8, MoveFlags::CASTLING_KING);
            }
            if (m_castling.has(Castlings::B_QUEEN_SIDE) &&
                !(Bitboard::between(Squares::E8, Squares::A8) & occupancy<Side::BOTH>()).any()) {
                if (!isUnderAttack<Side::THEM>(Squares::E8) && !isUnderAttack<Side::THEM>(Squares::D8))
                    *move_list++ = Move(Squares::E8, Squares::C8, MoveFlags::CASTLING_QUEEN);
            }
        }
        return move_list;
    }

    static constexpr auto DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w";
};