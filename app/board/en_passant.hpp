#include <optional>

#include "navigation.hpp"
struct EnPassant {
    [[nodiscard]] bool has_value() const { return m_file.has_value(); }
    void               clear() { m_file.reset(); }

    [[nodiscard]] Square square(Color toMove) const {
        assert(m_file.has_value());
        return Square(m_file.value(), toMove == Color::WHITE ? Rank::R6 : Rank::R3);
    }

    void set(Square toSquare) {
        assert(toSquare.rank() == Rank::R4 || toSquare.rank() == Rank::R5);
        m_file = toSquare.file();
    }

   private:
    std::optional<File> m_file;
};
