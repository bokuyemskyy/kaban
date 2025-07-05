#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <array>
#include <cstdint>

#include "direction.hpp"
#include "navigation.hpp"

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

            for (Rank i = Rank::FIRST; i <= Rank::LAST; ++i) {
                t[i] = (RANK_A << (i * File::NB));
            }
            return t;
        }();

        return table[rank];
    }

    static Bitboard fileBB(File file) {
        static auto table = [] {
            std::array<Bitboard, File::NB> t{};

            Bitboard FILE_A = 0x0101010101010101ULL;

            for (File i = File::FIRST; i <= File::LAST; ++i) {
                t[i] = (FILE_A << i);
            }
            return t;
        }();

        return table[file];
    }
    static Bitboard diagBB(Square square) {
        static auto table = [] {
            std::array<Bitboard, Square::NB> t{};
            for (Square sq = Square::FIRST; sq <= Square::LAST; ++sq) {
                Bitboard diag   = zero();
                File     sqFile = sq.file();
                Rank     sqRank = sq.rank();

                for (Rank r = Rank::FIRST; r <= Rank::LAST; ++r) {
                    for (File f = File::FIRST; f <= File::LAST; ++f) {
                        Square possibleSq = Square::create(f, r);
                        if (f - r == sqFile - sqRank) {
                            diag = diag | squareBB(possibleSq);
                        }
                    }
                }
                t[sq] = diag;
            }
            return t;
        }();
        return table[square];
    }
    static Bitboard antiDiagBB(Square square) {
        static auto table = [] {
            std::array<Bitboard, Square::NB> t{};
            for (Square sq = Square::FIRST; sq <= Square::LAST; ++sq) {
                Bitboard antiDiag = zero();
                File     sqFile   = sq.file();
                Rank     sqRank   = sq.rank();

                for (Rank r = Rank::FIRST; r <= Rank::LAST; ++r) {
                    for (File f = File::FIRST; f <= File::LAST; ++f) {
                        Square possibleSq = Square::create(f, r);
                        if (f + r == sqFile + sqRank) {
                            antiDiag = antiDiag | squareBB(possibleSq);
                        }
                    }
                }
                t[sq] = antiDiag;
            }
            return t;
        }();
        return table[square];
    }
};

#endif
