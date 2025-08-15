#pragma once

#include <array>
#include <cctype>
#include <cstdint>

#include "strong.hpp"

class File : public Strong::Value<File, uint8_t>,
             public Strong::Enumerator<File, uint8_t, 8>,
             public Strong::Field<File, uint8_t, 3> {
   public:
    using Value::Value;

    static constexpr uint8_t distance(File from, File to) noexcept {
        return from.value() > to.value() ? from.value() - to.value() : to.value() - from.value();
    }

    static constexpr File fromChar(char c) noexcept {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
        for (uint8_t i = 0; i < number(); ++i)
            if (chars[i] == c) return File(i);
        return File();
    }

    [[nodiscard]] constexpr char toChar() const noexcept { return hasValue() ? chars[value()] : '?'; }

    constexpr File operator+(int offset) const noexcept {
        return File(static_cast<uint8_t>(static_cast<int>(value()) + offset));
    }
    constexpr File  operator-(int offset) const noexcept { return *this + (-offset); }
    constexpr File& operator+=(int offset) noexcept { return *this = *this + offset; }
    constexpr File& operator-=(int offset) noexcept { return *this = *this - offset; }

   private:
    static constexpr std::array<char, number()> chars = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

    // clang-format off
    enum Values : uint8_t {
        FA = 0, FB, FC, FD, FE, FF, FG, FH,
        NONE = 0xFF
    };
    // clang-format on

    friend struct Files;
};

struct Files {
    // clang-format off
    #define F(name) \
        static constexpr File name { File::Values::name }

        F(FA); F(FB); F(FC); F(FD); F(FE); F(FF); F(FG); F(FH);
    #undef F

    static constexpr std::array<File, File::number()> all() { return {FA, FB, FC, FD, FE, FF, FG, FH}; }
    // clang-format on
};
