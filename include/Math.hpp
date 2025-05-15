#pragma once

#include <ostream>

namespace math
{
struct PolarCoordinate
{
    double angle;
    double radius;
};

class Vec2
{
  public:
    double x;
    double y;

    Vec2();

    template <typename _first, typename _second>
    Vec2(_first x, _first y, const float tolerance = 0.0001)
        : x(static_cast<double>(x)), y(static_cast<double>(y)), tolerance(tolerance)
    {
    }

    double getLength() const;

    double getAngle() const;

    void rotate(double rad);

    PolarCoordinate asPolar() const;

    void scaleToLength(double scalar);

    Vec2 project(const Vec2& other) const;

    Vec2 reject(const Vec2& other) const;

    Vec2 reflect(const Vec2& other) const;

    void normalize();

    double distanceTo(const Vec2& other) const;

    template <typename T> Vec2 operator/(T scalar) const { return {x / scalar, y / scalar}; }

    template <typename T> Vec2& operator/=(T scalar)
    {
        x /= scalar;
        y /= scalar;

        return *this;
    }

    template <typename T> Vec2& operator*=(T scalar)
    {
        if (!isProductValid(x, static_cast<double>(scalar)) ||
            !isProductValid(y, static_cast<double>(scalar)))
        {
            WARN("Multiplication would result in overflow")
            return *this;
        }

        x *= scalar;
        y *= scalar;

        return *this;
    }

    Vec2 operator+(const Vec2& other) const;

    Vec2 operator-(const Vec2& other) const;

    Vec2 operator-() const;

    Vec2& operator+=(const Vec2& other);

    Vec2& operator-=(const Vec2& other);

    bool operator==(const Vec2& other) const;

    bool operator!=(const Vec2& other) const;

    bool operator<(const Vec2& other) const;

    bool operator>(const Vec2& other) const;

    bool operator<=(const Vec2& other) const;

    bool operator>=(const Vec2& other) const;

  protected:
    double tolerance;
};

Vec2 scaleToLength(const Vec2& vec, double scalar);

Vec2 fromPolar(double angle, double radius);

Vec2 normalize(const Vec2& vec);

Vec2 clampVec(const Vec2& vec, const Vec2& min, const Vec2& max);

Vec2 lerpVec(const Vec2& a, const Vec2& b, double t);

double lerp(double a, double b, double t);

double remap(double in_min, double in_max, double out_min, double out_max, double value);

double toDegrees(double angle);

double toRadians(double angle);

double dot(const Vec2& a, const Vec2& b);

double cross(const Vec2& a, const Vec2& b);

double angleBetween(const Vec2& a, const Vec2& b);

template <typename T> Vec2 operator*(const T& lhs, const Vec2& rhs)
{
    if (!isProductValid(static_cast<double>(lhs), rhs.x) ||
        !isProductValid(static_cast<double>(lhs), rhs.y))
    {
        WARN("Multiplication would result in overflow")
        return {};
    }

    const double x = lhs * rhs.x;
    const double y = lhs * rhs.y;

    return {x, y};
}

template <typename T> Vec2 operator*(const Vec2& lhs, const T& rhs) { return rhs * lhs; }

std::ostream& operator<<(std::ostream& os, const Vec2& vec);
} // namespace math
