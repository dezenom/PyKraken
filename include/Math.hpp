#pragma once

#include <ostream>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace math
{
struct PolarCoordinate;
class Vec2;

void _bind(py::module_& module);

Vec2 scaleToLength(const Vec2& vec, double scalar);

Vec2 fromPolar(double rad, double radius);

Vec2 fromPolar(const PolarCoordinate& polar);

Vec2 normalize(const Vec2& vec);

Vec2 clampVec(const Vec2& vec, const Vec2& min, const Vec2& max);

Vec2 lerp(const Vec2& a, const Vec2& b, double t);

double lerp(double a, double b, double t);

double remap(double in_min, double in_max, double out_min, double out_max, double value);

double toDegrees(double angle);

double toRadians(double angle);

double dot(const Vec2& a, const Vec2& b);

double cross(const Vec2& a, const Vec2& b);

double angleBetween(const Vec2& a, const Vec2& b);

struct PolarCoordinate
{
    double angle = 0;
    double radius = 0;
};

class Vec2
{
  public:
    double x = 0.0;
    double y = 0.0;

    Vec2() = default;

    template <typename _first, typename _second>
    Vec2(_first x, _second y) : x(static_cast<double>(x)), y(static_cast<double>(y))
    {
    }

    bool isZero(double tolerance = 1e-8) const;

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
};

template <typename T> Vec2 operator*(const T& lhs, const Vec2& rhs)
{
    const double x = lhs * rhs.x;
    const double y = lhs * rhs.y;

    return {x, y};
}

template <typename T> Vec2 operator*(const Vec2& lhs, const T& rhs) { return rhs * lhs; }
} // namespace math
