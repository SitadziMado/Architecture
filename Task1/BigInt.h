#pragma once

#include <string>
#include <type_traits>
#include <utility>

class BigUint
{
public:
    using SignedNumericT = long long;
    using NumericT = unsigned long long;
    using ContainerT = std::vector<NumericT>;

    BigUint();
    BigUint(NumericT number);
    // BigUint(const char* number);
    // BigUint(const std::string& number);

    explicit operator NumericT() const;

    friend bool operator<(const BigUint& lhs, const BigUint& rhs) noexcept;
    friend bool operator>(const BigUint& lhs, const BigUint& rhs) noexcept;
    friend bool operator<=(const BigUint& lhs, const BigUint& rhs) noexcept;
    friend bool operator>=(const BigUint& lhs, const BigUint& rhs) noexcept;
    friend bool operator==(const BigUint& lhs, const BigUint& rhs) noexcept;
    friend bool operator!=(const BigUint& lhs, const BigUint& rhs) noexcept;

    BigUint& operator+=(const BigUint& rhs);
    BigUint& operator-=(const BigUint& rhs);
    BigUint& operator*=(const BigUint& rhs);
    BigUint& operator/=(const BigUint& rhs);
    BigUint& operator/=(NumericT rhs);
    BigUint& operator%=(NumericT rhs);

    // static BigUint parse(const char* number);
    static BigUint parse(const std::string& number);

    inline void swap(BigUint other) noexcept;
    const std::string toString() const;

private:
    static_assert(std::is_arithmetic_v<NumericT>, "Неверный значимый тип.");

    static constexpr std::size_t Base = 10;
    static constexpr std::size_t BitSize = sizeof(NumericT) * 8;
    static constexpr NumericT LowMask = ~NumericT() >> (BitSize / 2);
    static constexpr NumericT HighMask = ~NumericT() << (BitSize / 2);

    inline static constexpr NumericT clearHiBits(NumericT number) noexcept;
    inline static constexpr NumericT getHiBits(NumericT number) noexcept;
    inline static constexpr NumericT getLoBits(NumericT number) noexcept;
    inline static void setHiBits(NumericT& number, NumericT bits) noexcept;

    inline static void carry(ContainerT& container, size_t index);
    inline static void carrySub(ContainerT& container, size_t index);
    inline static void clearHigherHalf(ContainerT& container, size_t index);
    inline static NumericT getHigherHalf(const ContainerT& container, size_t number) noexcept;
    inline static void multiply(ContainerT& container, NumericT number);
    inline static std::pair<ContainerT&, NumericT> divide(ContainerT& container, NumericT number);

    inline void shift(size_t amount);

    ContainerT data_;
};

const BigUint operator+(const BigUint& lhs, const BigUint& rhs);
const BigUint operator-(const BigUint& lhs, const BigUint& rhs);
const BigUint operator*(const BigUint& lhs, const BigUint& rhs);
const BigUint operator/(const BigUint& lhs, const BigUint& rhs);
const BigUint operator/(const BigUint& lhs, BigUint::NumericT rhs);
const BigUint operator%(const BigUint& lhs, BigUint::NumericT rhs);

const BigUint operator""_BU(const char* number, size_t size);