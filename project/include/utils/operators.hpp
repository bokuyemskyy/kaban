#include <ostream>
template <typename Derived>
struct ArithmeticOperators {
    constexpr Derived operator+(const Derived& other) const {
        return Derived(static_cast<const Derived*>(this)->value() + other.value());
    }

    constexpr Derived operator-(const Derived& other) const {
        return Derived(static_cast<const Derived*>(this)->value() - other.value());
    }

    constexpr Derived& operator+=(const Derived& other) {
        static_cast<Derived*>(this)->value() += other.value();
        return static_cast<Derived&>(*this);
    }

    constexpr Derived& operator-=(const Derived& other) {
        static_cast<Derived*>(this)->value() -= other.value();
        return static_cast<Derived&>(*this);
    }

    template <typename T>
    constexpr Derived operator+(T offset) const {
        return Derived(static_cast<const Derived*>(this)->value() + offset);
    }

    template <typename T>
    constexpr Derived operator-(T offset) const {
        return Derived(static_cast<const Derived*>(this)->value() - offset);
    }
};

template <typename Derived>
struct BitwiseOperators {
    constexpr Derived operator|(const Derived& other) const {
        return Derived(static_cast<const Derived*>(this)->value() | other.value());
    }

    constexpr Derived operator&(const Derived& other) const {
        return Derived(static_cast<const Derived*>(this)->value() & other.value());
    }

    constexpr Derived operator^(const Derived& other) const {
        return Derived(static_cast<const Derived*>(this)->value() ^ other.value());
    }

    constexpr Derived& operator|=(const Derived& other) {
        static_cast<Derived*>(this)->value() |= other.value();
        return static_cast<Derived&>(*this);
    }

    constexpr Derived& operator&=(const Derived& other) {
        static_cast<Derived*>(this)->value() &= other.value();
        return static_cast<Derived&>(*this);
    }

    constexpr Derived& operator^=(const Derived& other) {
        static_cast<Derived*>(this)->value() ^= other.value();
        return static_cast<Derived&>(*this);
    }

    constexpr Derived operator<<(int shift) const {
        return Derived(static_cast<const Derived*>(this)->value() << shift);
    }

    constexpr Derived operator>>(int shift) const {
        return Derived(static_cast<const Derived*>(this)->value() >> shift);
    }

    constexpr Derived& operator<<=(int shift) {
        static_cast<Derived*>(this)->value() <<= shift;
        return static_cast<Derived&>(*this);
    }

    constexpr Derived& operator>>=(int shift) {
        static_cast<Derived*>(this)->value() >>= shift;
        return static_cast<Derived&>(*this);
    }
};

template <typename Derived>
struct ComparisonOperators {
    constexpr bool operator<(const Derived& other) const {
        return static_cast<const Derived*>(this)->value() < other.value();
    }

    constexpr bool operator>(const Derived& other) const {
        return static_cast<const Derived*>(this)->value() > other.value();
    }

    constexpr bool operator<=(const Derived& other) const {
        return static_cast<const Derived*>(this)->value() <= other.value();
    }

    constexpr bool operator>=(const Derived& other) const {
        return static_cast<const Derived*>(this)->value() >= other.value();
    }

    constexpr bool operator==(const Derived& other) const {
        return static_cast<const Derived*>(this)->value() == other.value();
    }

    constexpr bool operator!=(const Derived& other) const {
        return static_cast<const Derived*>(this)->value() != other.value();
    }
};

template <typename Derived>
struct IncrementOperators {
    constexpr Derived& operator++() {
        ++static_cast<Derived*>(this)->value();
        return static_cast<Derived&>(*this);
    }

    constexpr Derived operator++(int) {
        Derived old = static_cast<Derived&>(*this);
        ++(*this);
        return old;
    }

    constexpr Derived& operator--() {
        --static_cast<Derived*>(this)->value();
        return static_cast<Derived&>(*this);
    }

    constexpr Derived operator--(int) {
        Derived old = static_cast<Derived&>(*this);
        --(*this);
        return old;
    }
};

template <typename Derived>
struct StreamOperators {
    friend std::ostream& operator<<(std::ostream& os, const Derived& obj) {
        obj.print(os);
        return os;
    }
};

template <typename Derived>
struct AllOperators : ArithmeticOperators<Derived>,
                      BitwiseOperators<Derived>,
                      ComparisonOperators<Derived>,
                      IncrementOperators<Derived>,
                      StreamOperators<Derived> {};