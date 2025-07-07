#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <array>
#include <cstdint>

#include "coordinate.hpp"
#include "direction.hpp"
#include "piece.hpp"

class Bitboard {
   private:
    uint64_t m_val;

   public:
    constexpr Bitboard(uint64_t val = zero()) : m_val(val) {};

    constexpr operator uint64_t() const { return m_val; }

    static constexpr Bitboard zero() { return 0ULL; }

    static constexpr Bitboard squareBB(Square sq) { return (1ULL << uint8_t(sq)); }

    static constexpr Bitboard destinationBB(Square square, Direction direction) {
        Square destination = square + direction;
        return (destination.isValid() && Square::distance(square, destination) <= 2) ? squareBB(destination) : zero();
    };

    static Bitboard rankBB(Rank rank) {
        static auto table = [] {
            std::array<Bitboard, Rank::NB> t{};

            Bitboard RANK_A = 0xFFULL;

            for (auto rank : Rank::all()) {
                t[rank.value()] = (RANK_A << (rank.value() * File::number()));
            }
            return t;
        }();

        return table[rank];
    }

    static Bitboard fileBB(File file) {
        static auto table = [] {
            std::array<Bitboard, File::NB> t{};

            Bitboard FILE_A = 0x0101010101010101ULL;

            for (File i = File::first(); i <= File::last(); ++i) {
                t[i] = (FILE_A << i);
            }
            return t;
        }();

        return table[file];
    }
    static Bitboard diagBB(Square square) {
        static auto table = [] {
            std::array<Bitboard, Square::NB> t{};
            for (Square i = Square::first(); i <= Square::last(); ++i) {
                Bitboard diag       = zero();
                File     squareFile = i.file();
                Rank     squareRank = i.rank();

                for (Rank r = Rank::FIRST; r <= Rank::last(); ++r) {
                    for (File f = File::FIRST; f <= File::last(); ++f) {
                        Square possibleSquare = Square::create(f, r);
                        if (f - r == squareFile - squareRank) {
                            diag = diag | squareBB(possibleSquare);
                        }
                    }
                }
                t[i] = diag;
            }
            return t;
        }();
        return table[square];
    }
    static Bitboard antiDiagBB(Square square) {
        static auto table = [] {
            std::array<Bitboard, Square::NB> t{};
            for (Square i = Square::first(); i <= Square::last(); ++i) {
                Bitboard antiDiag   = zero();
                File     squareFile = i.file();
                Rank     squareRank = i.rank();

                for (Rank r = Rank::first(); r <= Rank::last(); ++r) {
                    for (File f = File::first(); f <= File::last(); ++f) {
                        Square possibleSquare = Square::create(f, r);
                        if (f + r == squareFile + squareRank) {
                            antiDiag = antiDiag | squareBB(possibleSquare);
                        }
                    }
                }
                t[i] = antiDiag;
            }
            return t;
        }();
        return table[square];
    }
    template <PieceType PT>
    static constexpr Bitboard attacksBB(Square square, Bitboard occupied) {
        switch (PT) { case  }
    }
};

#endif
