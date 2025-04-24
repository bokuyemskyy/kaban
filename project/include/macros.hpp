#include "types.hpp"

#define ENABLE_ENUM_CLASS_OPERATIONS(Enum)                                                     \
    inline Enum& operator++(Enum& e) {                                                         \
        e = static_cast<Enum>(static_cast<u8>(e) + 1);                                         \
        return e;                                                                              \
    }                                                                                          \
    inline Enum operator++(Enum& e, int) {                                                     \
        Enum old = e;                                                                          \
        ++e;                                                                                   \
        return old;                                                                            \
    }                                                                                          \
    inline bool operator>=(Enum lhs, Enum rhs) {                                               \
        return static_cast<u8>(lhs) >= static_cast<u8>(rhs);                                   \
    }                                                                                          \
    inline bool operator>(Enum lhs, Enum rhs) {                                                \
        return static_cast<u8>(lhs) > static_cast<u8>(rhs);                                    \
    }                                                                                          \
    inline bool operator==(Enum lhs, Enum rhs) {                                               \
        return static_cast<u8>(lhs) == static_cast<u8>(rhs);                                   \
    }                                                                                          \
    inline bool operator<=(Enum lhs, Enum rhs) {                                               \
        return static_cast<u8>(lhs) <= static_cast<u8>(rhs);                                   \
    }                                                                                          \
    inline bool operator<(Enum lhs, Enum rhs) {                                                \
        return static_cast<u8>(lhs) < static_cast<u8>(rhs);                                    \
    }                                                                                          \
    inline Enum operator+(Enum e, u8 i) {                                                      \
        u8 res = static_cast<u8>(e) + i;                                                       \
        return (static_cast<Enum>(res) >= Enum::FIRST && static_cast<Enum>(res) <= Enum::LAST) \
                   ? static_cast<Enum>(res)                                                    \
                   : Enum::NONE;                                                               \
    }\
