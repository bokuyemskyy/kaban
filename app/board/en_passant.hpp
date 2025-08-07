#pragma once

#include <sys/types.h>

#include <cstdint>
#include <optional>

#include "navigation.hpp"
#include "piece.hpp"

struct EnPassant : Metadata<EnPassant> {
    EnPassant(uint8_t value = 0)
        : m_file((value & 0b1000) ? std::optional<File>(File(value & 0b0111)) : std::nullopt) {}

    [[nodiscard]] bool hasValue() const { return m_file.has_value(); }
    void               clear() { m_file.reset(); }

    [[nodiscard]] Square square(Color toMove) const {
        assert(m_file.has_value());
        return Square(m_file.value(), toMove == Color::WHITE ? Ranks::R6 : Ranks::R3);
    }

    void set(Square toSquare) {
        assert(toSquare.rank() == Ranks::R4 || toSquare.rank() == Ranks::R5);
        m_file = toSquare.file();
    }

    [[nodiscard]] uint8_t value() const { return m_file.has_value() ? (0b1000 | m_file->value()) : 0b0000; }

    enum : uint8_t {
        NB   = 8,
        SIZE = 4,
        MASK = 0b1111
    };

   private:
    std::optional<File> m_file;
};
