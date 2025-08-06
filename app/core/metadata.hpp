#include <cstdint>
template <typename Derived>
struct Metadata {
    [[nodiscard]] static constexpr uint8_t number() { return Derived::NB; }
    [[nodiscard]] static constexpr uint8_t mask() { return Derived::MASK; }
    [[nodiscard]] static constexpr uint8_t size() { return Derived::SIZE; }
    friend Derived;
};
