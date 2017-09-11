#pragma once

#include <cassert>

#include <type_traits>
#include <utility>

class BigUint
{
public:
    constexpr BigUint();

    friend bool operator<(const BigUint& lhs, const BigUint& rhs) noexcept;

    BigUint& operator+=(const BigUint& rhs);
    BigUint& operator-=(const BigUint& rhs);
    BigUint& operator*=(const BigUint& rhs);
    BigUint& operator/=(const BigUint& rhs);

    inline void swap(BigUint other);

private:
    using NumericT = unsigned long long;
    using ContainerT = std::vector<NumericT>;
    static_assert(std::is_arithmetic_v<NumericT>, "Неверный значимый тип.");

    static constexpr size_t BitSize = sizeof(NumericT) * 8;
    static constexpr NumericT LowMask = ~NumericT() >> (BitSize / 2);
    static constexpr NumericT HighMask = ~NumericT() << (BitSize / 2);

    inline static void carry(ContainerT& container, size_t index);
    inline static void carrySub(ContainerT& container, size_t index);
    inline static void clearHigherHalf(ContainerT& container, size_t index);
    inline static NumericT getHigherHalf(const ContainerT& container, size_t number) noexcept;

    ContainerT data_;
};

