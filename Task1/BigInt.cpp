#include "stdafx.h"
#include "BigInt.h"
#include "FormatException.h"

#include <cassert>

#include <algorithm>
#include <functional>
#include <iterator>
#include <locale>
#include <stdexcept>
#include <utility>

BigUint::BigUint()
    : data_{ 0 }
{
}

BigUint::BigUint(NumericT number)
    : data_{ number }
{
    carry(data_, 0);
}

BigUint::operator NumericT() const
{
    return (data_.size()) ? data_[0] : NumericT(0);
}

BigUint& BigUint::operator+=(const BigUint& rhs)
{
    BigUint lhs(*this);

    auto& ldata = lhs.data_;
    auto& rdata = rhs.data_;

    while (ldata.size() < rdata.size())
        ldata.push_back(NumericT());

    for (size_t i = 0; i < rdata.size(); ++i)
    {
        ldata[i] += rdata[i];
        carry(ldata, i);
    }

    data_.swap(ldata);

    return *this;
}

BigUint& BigUint::operator-=(const BigUint& rhs)
{
    assert(rhs < *this);

    BigUint lhs(*this);

    auto& ldata = lhs.data_;
    auto& rdata = rhs.data_;

    for (size_t i = 0; i < rdata.size(); ++i)
    {
        auto left = ldata[i];
        auto right = rdata[i];

        if (left >= right)
        {
            ldata[i] -= rdata[i];
        }
        else
        {
            ldata[i] = rdata[i] - ldata[i];
            carrySub(ldata, i);
        }
    }

    data_.swap(ldata);

    return *this;
}

BigUint& BigUint::operator*=(const BigUint& rhs)
{
    BigUint lhs(*this);
    auto result = BigUint();

    auto& ldata = lhs.data_;
    auto& rdata = rhs.data_;

    for (size_t i = 0; i < rdata.size(); ++i)
    {
        auto digit = rdata[i];
        auto temp = BigUint(lhs);

        multiply(temp.data_, digit);

        temp.shift(i);

        result += temp;
    }

    data_.swap(result.data_);

    return *this;
}

BigUint& BigUint::operator/=(const BigUint & rhs)
{
    throw std::runtime_error("Деление еще не реализовано.");
}

BigUint& BigUint::operator/=(NumericT rhs)
{
    auto lhs = ContainerT(data_);

    auto result = divide(lhs, rhs).first;

    data_.swap(result);

    return *this;
}

BigUint& BigUint::operator%=(NumericT rhs)
{
    auto lhs = *this;

    auto result = divide(lhs.data_, rhs).second;

    this->swap(BigUint(result));

    return *this;
}

// Парсинг строки, представляющей число без знаков и пробелов
BigUint BigUint::parse(const std::string& number)
{
    using namespace std::placeholders;

    const auto Base = NumericT(10ULL);

    auto result = BigUint();

    if (!number.size())
        return result;

    auto isdigit = std::bind(std::isdigit<char>, _1, std::locale());

    for (size_t i = 0U; i < number.size() - 1; ++i)
    {
        char c = number[i];

        if (isdigit(c))
        {
            // ToDo: добавить оператор+ для встроенных чисел
            result += c - '0';
            result *= Base;
        }
        else
        {
            throw FormatException("Неверное число");
        }
    }

    char c = number.back();

    if (isdigit(c))
        result += c - '0';
    else
        throw FormatException("Неверное число");

    return result;
}

inline void BigUint::swap(BigUint other) noexcept
{
    data_.swap(other.data_);
}

const std::string BigUint::toString() const
{
    auto number = BigUint(*this);
    auto zero = BigUint(0);
    auto output = std::string();

    while (number != zero)
    {
        auto c = ((char)(NumericT)(number % Base)) + '0';
        output.push_back(c);
        number /= Base;
    }

    return std::string(output.rbegin(), output.rend());
}

inline constexpr BigUint::NumericT BigUint::clearHiBits(NumericT number) noexcept
{
    return number & ~HighMask;
}

inline constexpr BigUint::NumericT BigUint::getHiBits(NumericT number) noexcept
{
    return (number & HighMask) >> (BitSize / 2);
}

inline constexpr BigUint::NumericT BigUint::getLoBits(NumericT number) noexcept
{
    return number & LowMask;
}

inline void BigUint::setHiBits(NumericT& number, NumericT bits) noexcept
{
    number = getLoBits(number) | (bits << (BitSize / 2));
}

// Функция переноса для использования после сложения и умножения
inline void BigUint::carry(ContainerT& container, size_t index)
{
    assert(0 <= index && index < container.size());

    auto previousCarry = NumericT();

    do
    {
        // Берем верхние разряды для переноса в следующий
        auto higher = getHigherHalf(container, index);
        clearHigherHalf(container, index);

        // Увеличиваем разряд на количество для переноса
        container[index] += previousCarry;

        // Повторяем начальную процедуру из-за возможного наполнения переноса
        higher += getHigherHalf(container, index);
        clearHigherHalf(container, index);

        // Устанавливаем перенос для следующего разряда
        previousCarry = higher;

    } while ((++index < container.size()) && previousCarry);

    if (previousCarry)
        container.push_back(previousCarry);
}

inline void BigUint::carrySub(ContainerT& container, size_t index)
{
    assert(0 <= index && index < container.size());

    bool sub = true;

    for (++index; index < container.size() && sub; ++index)
    {
        if (container[index])
        {
            sub = false;
            container[index];
        }
        else
        {
            container[index] = (container[index] - 1) & LowMask;
            sub = true;
        }
    }

    auto end = container.rbegin() + (container.size() - 1);

    // Привести цифры в порядок удалением нулей в начале
    for (auto it = container.rbegin() - 1; it >= end && !(*it); --it)
        container.pop_back();
}

inline void BigUint::clearHigherHalf(ContainerT& container, size_t index)
{
    assert(0 <= index && index < container.size());

    container[index] = clearHiBits(container[index]);
}

inline BigUint::NumericT BigUint::getHigherHalf(const ContainerT& container, size_t index) noexcept
{
    assert(0 <= index && index < container.size());

    return getHiBits(container[index]);
}

inline void BigUint::multiply(ContainerT& container, NumericT number)
{
    std::for_each(
        container.begin(),
        container.end(), 
        [number](auto& x) { x *= number; }
    );

    for (size_t i = 0; i < container.size(); ++i)
        carry(container, i);
}

inline std::pair<BigUint::ContainerT&, BigUint::NumericT> BigUint::divide(ContainerT& container, NumericT number)
{
    if (!number)
        throw std::runtime_error("Деление на ноль.");

    auto remainder = NumericT();
    auto end = container.rend();
    
    for (auto it = container.rbegin(); it != end; ++it)
    {
        auto temp = *it;
        setHiBits(temp, remainder);
        remainder = temp % number;
        *it /= number;

    }

    auto size = container.size() - 1;

    for (size_t i = 0; i < size; ++i)
    {
        auto index = size - i - 1;

        if (!container[index])
            container.pop_back();
        else 
            break;
    }

    return { container, remainder }; // std::pair<ContainerT&, NumericT>(container, remainder);
}

inline void BigUint::shift(size_t amount)
{
    auto extended = ContainerT(amount);

    extended.reserve(data_.size() + amount);

    std::copy(
        data_.cbegin(),
        data_.cend(),
        std::back_inserter(extended)
    );

    data_.swap(extended);
}

bool operator<(const BigUint& lhs, const BigUint& rhs) noexcept
{
    if (lhs.data_.size() < rhs.data_.size())
    {
        return true;
    }
    else if (lhs.data_.size() > 0U && lhs.data_.size() == rhs.data_.size())
    {
        return *(lhs.data_.cend() - 1) < *(rhs.data_.cend() - 1);
    }
    else
    {
        return false;
    }
}

bool operator==(const BigUint & lhs, const BigUint & rhs) noexcept
{
    if (!lhs.data_.size() && lhs.data_.size() == rhs.data_.size())
    {
        return true;
    }
    else if (lhs.data_.size() == rhs.data_.size())
    {
        return *(lhs.data_.cend() - 1) == *(rhs.data_.cend() - 1);
    }
    else
    {
        return false;
    }
}

bool operator!=(const BigUint& lhs, const BigUint& rhs) noexcept
{
    return !(lhs == rhs);
}

bool operator>(const BigUint& lhs, const BigUint& rhs) noexcept
{
    return rhs < lhs;
}

bool operator<=(const BigUint& lhs, const BigUint& rhs) noexcept
{
    return !(rhs < lhs);
}

bool operator>=(const BigUint& lhs, const BigUint& rhs) noexcept
{
    return !(lhs < rhs);
}

const BigUint operator+(const BigUint& lhs, const BigUint& rhs)
{
    return BigUint(lhs) += rhs;
}

const BigUint operator-(const BigUint& lhs, const BigUint& rhs)
{
    return BigUint(lhs) -= rhs;
}

const BigUint operator*(const BigUint& lhs, const BigUint& rhs)
{
    return BigUint(lhs) *= rhs;
}

const BigUint operator/(const BigUint& lhs, const BigUint& rhs)
{
    return BigUint(lhs) /= rhs;
}

const BigUint operator/(const BigUint& lhs, BigUint::NumericT rhs)
{
    return BigUint(lhs) /= rhs;
}

const BigUint operator%(const BigUint& lhs, BigUint::NumericT rhs)
{
    return BigUint(lhs) %= rhs;
}

const BigUint operator""_BU(const char* number, size_t size)
{
    return BigUint::parse(number);
}