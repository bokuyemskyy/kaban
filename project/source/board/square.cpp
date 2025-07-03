#include "square.hpp"

namespace Squares {
void init() {
    for (Square from = Squares::FIRST; from <= Squares::LAST; ++from) {
        for (Square to = Squares::FIRST; to <= Squares::LAST; ++to) {
            distances[from][to] = std::max(abs(getFile(from) - getFile(to)), abs(getRank(from) - getRank(to)));
        }
    }
}

std::array<std::array<uint8_t, NB>, NB> distances{};

}  // namespace Squares