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
    constexpr Bitboard(uint64_t val) : m_val(val) {};

    constexpr operator uint64_t() const { return m_val; }

    static constexpr uint64_t zero() { return 0ULL; }

    extern std::array<Bitboard, Ranks::NB> rankBBs;
    extern std::array<Bitboard, Files::NB> fileBBs;

    extern std::array<Bitboard, Squares::NB> diagBBs;
    extern std::array<Bitboard, Squares::NB> antiDiagBBs;

    static constexpr Bitboard squareBB(Square square) {
        assert(square.isValid());
        return 1ULL << square;
    }
    constexpr Bitboard destinationBB(Square square, Direction direction) {
        Square destination = square + direction;
        return (isValid(destination) && Squares::distance(square, destination) <= 2) ? squareBB(destination) : ZERO;
    };

    constexpr Bitboard rankBB(Rank rank) { return rankBBs[rank]; }
    constexpr Bitboard fileBB(File file) { return fileBBs[file]; }
    constexpr Bitboard diagBB(Square square) { return diagBBs[square]; }
    constexpr Bitboard antiDiagBB(Square square) { return antiDiagBBs[square]; }

}  // namespace Bitboards

#endif
