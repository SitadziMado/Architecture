#include "stdafx.h"

#include "Vector.h"

namespace geometry
{
    Vector& geometry::Vector::operator+=(const Vector& rhs)
    {
        x_ += rhs.x_;
        y_ += rhs.y_;
        z_ += rhs.z_;

        return *this;
    }

    Vector& Vector::operator-=(const Vector & rhs)
    {
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        z_ -= rhs.z_;

        return *this;
    }

    inline constexpr double Vector::getX() const noexcept
    {
        return x_;
    }

    inline constexpr double Vector::getY() const noexcept
    {
        return y_;
    }

    inline constexpr double Vector::getZ() const noexcept
    {
        return z_;
    }

    inline void Vector::setX(double value) noexcept
    {
        x_ = value;
    }

    inline void Vector::setY(double value) noexcept
    {
        y_ = value;
    }

    inline void Vector::setZ(double value) noexcept
    {
        z_ = value;
    }

    double operator*(const Vector & lhs, const Vector & rhs)
    {
        return
            lhs.getX() * rhs.getX() +
            lhs.getY() * rhs.getY() +
            lhs.getZ() * rhs.getZ();
    }

    Vector operator*(const Vector& lhs, double rhs)
    {
        return Vector(lhs) *= rhs;
    }

    Vector operator*(double lhs, const Vector& rhs)
    {
        return Vector(rhs) *= lhs;
    }

    std::istream& operator>>(std::istream& stream, Vector& vector)
    {
        double x, y, z;

        stream >> x >> y >> z;
        
        vector.setX(x);
        vector.setY(y);
        vector.setZ(z);

        return stream;
    }

    std::ostream& operator<<(std::ostream& stream, const Vector& vector)
    {
        stream << "(" << vector.getX() << "; " << vector.getY() << "; " << vector.getZ() << ")";
        return stream;
    }
}