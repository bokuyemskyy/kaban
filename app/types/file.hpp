
#pragma once

#include <array>
#include <cstdint>

#include "strong_value.hpp"

class File : public StrongValue<File, uint8_t> {
   public:
    using StrongValue::StrongValue;

    static constexpr ValueType   count() noexcept { return static_cast<ValueType>(8); }
    [[nodiscard]] constexpr bool hasValue() const { return value() < count(); }
    static constexpr uint8_t     bitlength() { return 3; }
    static constexpr ValueType   bitmask() { return static_cast<ValueType>((ValueType(1) << bitlength()) - 1); }

    static constexpr uint8_t distance(File from, File to) noexcept {
        return from.value() > to.value() ? from.value() - to.value() : to.value() - from.value();
    }

    static constexpr File fromChar(char c) noexcept {
        if (c >= 'a' && c <= 'h') return File(static_cast<uint8_t>(c - 'a'));
        return File();
    }

    [[nodiscard]] constexpr char toChar() const noexcept { return hasValue() ? static_cast<char>('a' + value()) : '?'; }

    constexpr File operator+(int offset) const noexcept {
        return File(static_cast<uint8_t>(static_cast<int>(value()) + offset));
    }
    constexpr File  operator-(int offset) const noexcept { return *this + (-offset); }
    constexpr File& operator+=(int offset) noexcept { return *this = *this + offset; }
    constexpr File& operator-=(int offset) noexcept { return *this = *this - offset; }

   private:
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

    static constexpr std::array<File, File::count()> all() { return {FA, FB, FC, FD, FE, FF, FG, FH}; }
    // clang-format on
};
