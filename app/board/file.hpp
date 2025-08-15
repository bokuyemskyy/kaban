#pragma once

#include <array>
#include <cctype>
#include <cstdint>

#include "strong_enum.hpp"

class File : public StrongEnum<File, uint8_t> {
   public:
    using StrongEnum<File, uint8_t>::StrongEnum;

    static constexpr uint8_t distance(File from, File to) {
        return from.value() > to.value() ? from.value() - to.value() : to.value() - from.value();
    }

    static constexpr File fromChar(char c) {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
        for (uint8_t i = 0; i < number(); ++i)
            if (chars[i] == c) return File(i);
        return File();
    }
    [[nodiscard]] constexpr char toChar() const { return isValid() ? chars[m_value] : '?'; }

   private:
    static constexpr std::array<char, 8> chars = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

    // clang-format off
    enum Values : uint8_t {
        FA = 0, FB, FC, FD, FE, FF, FG, FH,
        NONE = 0xFF
    };
    // clang-format on

    enum Metadata : uint8_t {
        MASK   = 0b111,
        SIZE   = 3,
        NUMBER = 8
    };

    friend struct Files;
    friend class StrongEnum<File, uint8_t>;
};

struct Files {
    static constexpr File FA{File::Values::FA};
    static constexpr File FB{File::Values::FB};
    static constexpr File FC{File::Values::FC};
    static constexpr File FD{File::Values::FD};
    static constexpr File FE{File::Values::FE};
    static constexpr File FF{File::Values::FF};
    static constexpr File FG{File::Values::FG};
    static constexpr File FH{File::Values::FH};

    static constexpr std::array<File, File::number()> all() { return {FA, FB, FC, FD, FE, FF, FG, FH}; }
};