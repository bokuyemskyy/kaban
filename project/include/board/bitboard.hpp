#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <array>
#include <cstdint>

#include "navigation.hpp"

class Bitboard {
   public:
    constexpr Bitboard(uint64_t val = zero()) : m_val(val) {};

    constexpr void                   set(uint64_t val) { m_val = val; }
    [[nodiscard]] constexpr uint64_t value() const { return m_val; }

    constexpr operator uint64_t() const { return m_val; }

    static constexpr Bitboard zero() { return 0ULL; }

    static constexpr Bitboard squareBB(Square square) { return (1ULL << square); }

    static constexpr Bitboard destinationBB(Square square, Direction direction) {
        Square destination = square + direction;
        return (destination.ok() && Square::distance(square, destination) <= 2) ? squareBB(destination) : zero();
    };

    static Bitboard rankBB(Rank rank) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, Rank::number()> t{};

            constexpr Bitboard RANK_A = 0xFFULL;

            for (auto i : Rank::all()) {
                t[i] = (RANK_A << (i * File::number()));
            }
            return t;
        }();

        return table[rank];
    }

    static Bitboard fileBB(File file) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, File::NB> t{};

            constexpr Bitboard FILE_A = 0x0101010101010101ULL;

            for (auto i : File::all()) {
                t[i] = (FILE_A << i);
            }
            return t;
        }();

        return table[file];
    }
    static Bitboard diagBB(Square square) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, Square::NB> t{};
            for (auto square : Square::all()) {
                Bitboard diag       = zero();
                File     squareFile = square.file();
                Rank     squareRank = square.rank();

                for (auto rank : Rank::all()) {
                    for (auto file : File::all()) {
                        Square possibleSquare = Square(file, rank);
                        if (file - rank == squareFile - squareRank) {
                            diag = diag | squareBB(possibleSquare);
                        }
                    }
                }
                t[square] = diag;
            }
            return t;
        }();
        return table[square];
    }
    static Bitboard antiDiagBB(Square square) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, Square::NB> t{};
            for (auto square : Square::all()) {
                Bitboard antiDiag   = zero();
                File     squareFile = square.file();
                Rank     squareRank = square.rank();

                for (auto rank : Rank::all()) {
                    for (auto file : File::all()) {
                        Square possibleSquare = Square(file, rank);
                        if (file + rank == squareFile + squareRank) {
                            antiDiag = antiDiag | squareBB(possibleSquare);
                        }
                    }
                }
                t[square] = antiDiag;
            }
            return t;
        }();
        return table[square];
    }
    // template <PieceType PT>
    // static constexpr Bitboard attacksBB(Square square, Bitboard occupied) {
    //     switch (PT) { case  }
    // };

   private:
    uint64_t m_val;
};

#endif
