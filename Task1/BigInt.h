#pragma once

#include <type_traits>

class BigUint
{
public:
    using NumericT = unsigned long long;
    using ContainerT = std::vector<NumericT>;

    constexpr BigUint();

    friend bool operator<(const BigUint& lhs, const BigUint& rhs) noexcept;

    BigUint& operator+=(const BigUint& rhs);
    BigUint& operator-=(const BigUint& rhs);
    BigUint& operator*=(const BigUint& rhs);
    BigUint& operator/=(const BigUint& rhs);

    inline void swap(BigUint other);

private:
    static_assert(std::is_arithmetic_v<NumericT>, "Неверный значимый тип.");

    static constexpr size_t BitSize = sizeof(NumericT) * 8;
    static constexpr NumericT LowMask = ~NumericT() >> (BitSize / 2);
    static constexpr NumericT HighMask = ~NumericT() << (BitSize / 2);

    inline static void carry(ContainerT& container, size_t index);
    inline static void carrySub(ContainerT& container, size_t index);
    inline static void clearHigherHalf(ContainerT& container, size_t index);
    inline static NumericT getHigherHalf(const ContainerT& container, size_t number) noexcept;
    inline static void multiply(ContainerT& container, NumericT number);

    ContainerT data_;
};

const BigUint operator+(const BigUint& lhs, const BigUint& rhs);
const BigUint operator-(const BigUint& lhs, const BigUint& rhs);
const BigUint operator*(const BigUint& lhs, const BigUint& rhs);
const BigUint operator/(const BigUint& lhs, const BigUint& rhs);