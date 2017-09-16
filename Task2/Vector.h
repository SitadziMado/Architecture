#pragma once

#include <iosfwd>
#include <utility>

namespace geometry
{
    using std::forward;

    class Vector
    {
    public:
        constexpr Vector::Vector()
            : x_(0.0), y_(0.0), z_(0.0)
        {
        }

        constexpr Vector::Vector(double x, double y, double z)
            : x_(x), y_(y), z_(z)
        {
        }

        template <typename ScalarT> Vector& operator*=(ScalarT&& rhs);
        template <typename ScalarT> Vector& operator/=(ScalarT&& rhs);

        Vector& operator+=(const Vector& rhs);
        Vector& operator-=(const Vector& rhs);

        inline constexpr double getX() const noexcept;
        inline constexpr double getY() const noexcept;
        inline constexpr double getZ() const noexcept;
        inline void setX(double value) noexcept;
        inline void setY(double value) noexcept;
        inline void setZ(double value) noexcept;

    private:
        double x_;
        double y_;
        double z_;
    };

    template<typename ScalarT>
    inline Vector& Vector::operator*=(ScalarT&& rhs)
    {
        x_ *= rhs;
        y_ *= rhs;
        z_ *= rhs;

        return *this;
    }

    template<typename ScalarT>
    inline Vector& Vector::operator/=(ScalarT&& rhs)
    {
        x_ /= rhs;
        y_ /= rhs;
        z_ /= rhs;       
        
        return *this;
    }

    double operator*(const Vector& lhs, const Vector& rhs);
    Vector operator*(const Vector& lhs, double rhs);
    Vector operator*(double lhs, const Vector& rhs);

    template <typename ScalarT> 
    Vector operator/(const Vector& lhs, ScalarT&& rhs)
    {
        return Vector(lhs) /= forward<ScalarT>(rhs);
    }

    std::istream& operator>>(std::istream& stream, Vector& vector);
    std::ostream& operator<<(std::ostream& stream, const Vector& vector);
}