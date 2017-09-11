#include "stdafx.h"
#include "BigInt.h"

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

    for (int i = 0; i < rdata.size(); ++i)
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

    for (int i = 0; i < rdata.size(); ++i)
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

    auto& ldata = lhs.data_;
    auto& rdata = rhs.data_;

    for (int i = 0; i < ldata.size(); ++i)
        for (int j = 0; j < rdata.size(); ++j)
        {
            ldata[i] *= rdata[j];
            carry(ldata, i);
        }

    data_.swap(ldata);

    return *this;
}

inline void BigUint::swap(BigUint other) noexcept
{
    data_.swap(other.data_);
}

inline void BigUint::carry(ContainerT& container, size_t index)
{
    assert(0 <= index && index < container.size());

    while (true)
    {
        auto higher = getHigherHalf(container, index);

        if (!higher)
            break;

        clearHigherHalf(container, index);

        if (index != container.size())
            container[index] += higher;
        else
            container.push_back(higher);
    }
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