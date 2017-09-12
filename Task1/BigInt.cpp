#include "stdafx.h"
#include "BigInt.h"

#include <cassert>

#include <algorithm>
#include <stdexcept>
#include <utility>

BigUint::BigUint()
{
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

    for (size_t i = 0; i < ldata.size(); ++i)
    {
        auto digit = ldata[i];
        auto temp = BigUint(lhs);

        multiply(temp.data_, digit);

        result += temp;
    }

    data_.swap(result.data_);

    return *this;
}

BigUint & BigUint::operator/=(const BigUint & rhs)
{
    throw std::runtime_error("Деление еще не реализовано.");
}

inline void BigUint::swap(BigUint other) noexcept
{
    data_.swap(other.data_);
}

// Функция переноса для использования после сложения и умножения. 
inline void BigUint::carry(ContainerT& container, size_t index)
{
    assert(0 <= index && index < container.size());

    auto previousCarry = NumericT();

    do
    {
        // Берем верхние разряды для переноса в следующий.
        auto higher = getHigherHalf(container, index);
        clearHigherHalf(container, index);

        // Увеличиваем разряд на количество для переноса.
        container[index] += previousCarry;

        // Повторяем начальную процедуру из-за возможного наполнения переноса.
        higher += getHigherHalf(container, index);
        clearHigherHalf(container, index);

        // Устанавливаем перенос для следующего разряда.
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

    // Привести цифры в порядок удалением нулей в начале.
    for (auto it = container.end() - 1; it >= container.begin() && !(*it); --it)
        container.pop_back();
}

inline void BigUint::clearHigherHalf(ContainerT& container, size_t index)
{
    assert(0 <= index && index < container.size());

    container[index] &= ~HighMask;
}

inline BigUint::NumericT BigUint::getHigherHalf(const ContainerT& container, size_t index) noexcept
{
    assert(0 <= index && index < container.size());

    return (container[index] & HighMask) >> (BitSize / 2);
}

inline void BigUint::multiply(ContainerT& container, NumericT number)
{
    std::for_each(
        container.begin(),
        container.end(), 
        [](auto& x) { x *= number; }
    );

    for (size_t i = 0; i < container.size(); ++i)
        carry(container, i);
}

bool operator<(const BigUint& lhs, const BigUint& rhs) noexcept
{
    if (lhs.data_.size() < rhs.data_.size())
    {
        return true;
    }
    else if (lhs.data_.size() > 0U && lhs.data_.size() == rhs.data_.size())
    {
        return *(lhs.data_.cend() - 1) == *(rhs.data_.cend() - 1);
    }
    else
    {
        return false;
    }
}

const BigUint operator+(const BigUint& lhs, const BigUint& rhs)
{
    return BigUint(lhs) += rhs;
}

const BigUint operator-(const BigUint & lhs, const BigUint & rhs)
{
    return BigUint(lhs) -= rhs;
}

const BigUint operator*(const BigUint & lhs, const BigUint & rhs)
{
    return BigUint(lhs) *= rhs;
}

const BigUint operator/(const BigUint & lhs, const BigUint & rhs)
{
    return BigUint(lhs) /= rhs;
}
