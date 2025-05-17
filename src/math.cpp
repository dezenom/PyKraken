#include "Math.hpp"

#include <algorithm>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace math
{
void _bind(py::module_& module)
{
    auto subMath = module.def_submodule("math", "Math related functions");

    py::class_<PolarCoordinate>(subMath, "PolarCoordinate")
        .def(py::init<>())
        .def_readwrite("angle", &PolarCoordinate::angle)
        .def_readwrite("radius", &PolarCoordinate::radius);
    module.attr("PolarCoordinate") = subMath.attr("PolarCoordinate");

    py::class_<Vec2>(subMath, "Vec2")
        .def(py::init<>(), "Create a zero vector")
        .def(py::init<double, double>(), py::arg("x"), py::arg("y"),
             "Create a Vec2 with given x and y values")
        .def(py::init(
                 [](py::tuple t)
                 {
                     if (t.size() != 2)
                         throw std::runtime_error("Vec2 constructor requires a tuple of size 2");
                     return Vec2(py::float_(t[0]), py::float_(t[1]));
                 }),
             py::arg("tuple"), "Construct from a 2-tuple")

        // Fields
        .def_readwrite("x", &Vec2::x)
        .def_readwrite("y", &Vec2::y)

        // String and repr
        .def("__str__", [](const Vec2& v)
             { return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")"; })
        .def("__repr__", [](const Vec2& v)
             { return "Vec2(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")"; })

        // Arithmetic operators
        .def("__add__", &Vec2::operator+)
        .def("__sub__", [](const Vec2& a, const Vec2& b) { return a - b; })
        .def("__neg__", [](const Vec2& v) { return -v; })

        .def("__iadd__", &Vec2::operator+=)
        .def("__isub__", &Vec2::operator-=)

        .def("__truediv__", [](const Vec2& v, double scalar) { return v / scalar; })
        .def("__itruediv__", [](Vec2& v, double scalar) -> Vec2& { return v /= scalar; })

        .def("__mul__", [](const Vec2& v, double scalar) { return v * scalar; })
        .def("__rmul__",
             [](double scalar, const Vec2& v) { return scalar * v; }) // Uses your global overload
        .def("__imul__", [](Vec2& v, double scalar) -> Vec2& { return v *= scalar; })

        .def("__hash__",
             [](const Vec2& v)
             {
                 std::size_t hx = std::hash<double>{}(v.x);
                 std::size_t hy = std::hash<double>{}(v.y);
                 return hx ^ (hy << 1);
             })

        // Comparison operators
        .def("__eq__", &Vec2::operator==)
        .def("__ne__", &Vec2::operator!=)
        .def("__lt__", &Vec2::operator<)
        .def("__le__", &Vec2::operator<=)
        .def("__gt__", &Vec2::operator>)
        .def("__ge__", &Vec2::operator>=)

        // Utility methods
        .def_property_readonly("length", &Vec2::getLength, "Get the length of the vector")
        .def_property_readonly("angle", &Vec2::getAngle, "Get the angle of the vector in radians")
        .def("rotate", &Vec2::rotate, py::arg("radians"))
        .def("normalize", &Vec2::normalize)
        .def("scale_to_length", &Vec2::scaleToLength, py::arg("length"))
        .def("distance_to", &Vec2::distanceTo, py::arg("other"))
        .def("as_polar", &Vec2::asPolar, "Return a polar coordinate pair (angle, length)");
    module.attr("Vec2") = subMath.attr("Vec2");

    subMath.def("scale_to_length", &scaleToLength, "Scale a vector to a given length");
    subMath.def("from_polar", static_cast<Vec2 (*)(double, double)>(&fromPolar), py::arg("angle"),
                py::arg("radius"), "Convert radians and a radius to a Cartesian vector");
    subMath.def("from_polar", static_cast<Vec2 (*)(const PolarCoordinate&)>(&fromPolar),
                py::arg("polar"), "Convert a PolarCoordinate object to a Cartesian vector");
    subMath.def("normalize", &normalize, "Normalize a vector");
    subMath.def("clamp", &clampVec, "Clamp a vector between two vectors");
    subMath.def(
        "clamp",
        [](py::float_ value, py::float_ min_val, py::float_ max_val) -> py::float_
        {
            return std::clamp(static_cast<double>(value), static_cast<double>(min_val),
                              static_cast<double>(max_val));
        },
        "Clamp a value between two values");
    subMath.def("lerp", static_cast<Vec2 (*)(const Vec2&, const Vec2&, double)>(&lerp),
                "Linearly interpolate between two Vec2s");
    subMath.def("lerp", static_cast<double (*)(double, double, double)>(&lerp),
                "Linearly interpolate between two doubles");
    subMath.def("remap", &remap, "Remap a value from one range to another");
    subMath.def("to_degrees", &toDegrees, "Convert radians to degrees");
    subMath.def("to_radians", &toRadians, "Convert degrees to radians");
    subMath.def("dot", &dot, "Calculate the dot product of two vectors");
    subMath.def("cross", &cross, "Calculate the cross product of two vectors");
    subMath.def("angle_between", &angleBetween, "Calculate the angle between two vectors");
}

Vec2 scaleToLength(const Vec2& vec, double scalar)
{
    double length = vec.getLength();
    if (length == 0.0)
        return vec;

    double scale = scalar / length;
    return Vec2(vec.x * scale, vec.y * scale);
}

Vec2 fromPolar(double rad, double radius)
{
    return Vec2(radius * std::cos(rad), radius * std::sin(rad));
}

Vec2 fromPolar(const PolarCoordinate& polar) { return fromPolar(polar.angle, polar.radius); }

Vec2 normalize(const Vec2& vec)
{
    auto newVec = vec;
    newVec.normalize();
    return newVec;
}

Vec2 clampVec(const Vec2& vec, const Vec2& min, const Vec2& max)
{
    return Vec2(std::clamp(vec.x, min.x, max.x), std::clamp(vec.y, min.y, max.y));
}

Vec2 lerp(const Vec2& a, const Vec2& b, double t)
{
    return Vec2(lerp(a.x, b.x, t), lerp(a.y, b.y, t));
}

double lerp(double a, double b, double t) { return a + (b - a) * t; }

double remap(double in_min, double in_max, double out_min, double out_max, double value)
{
    if (in_min == in_max)
        throw std::invalid_argument("in_min and in_max must not be equal");

    double scale = (value - in_min) / (in_max - in_min);
    return out_min + scale * (out_max - out_min);
}

double toDegrees(double angle) { return angle * (180.0 / M_PI); }

double toRadians(double angle) { return angle * (M_PI / 180.0); }

double dot(const Vec2& a, const Vec2& b) { return a.x * b.x + a.y * b.y; }

double cross(const Vec2& a, const Vec2& b) { return a.x * b.y - a.y * b.x; }

double angleBetween(const Vec2& a, const Vec2& b)
{
    double dotProduct = dot(a, b);
    double lengths = a.getLength() * b.getLength();
    if (lengths == 0.0)
        return 0.0;

    double cosTheta = dotProduct / lengths;
    return std::acos(std::clamp(cosTheta, -1.0, 1.0));
}

bool Vec2::isZero(double tolerance) const
{
    return std::abs(x) < tolerance && std::abs(y) < tolerance;
}

double Vec2::getLength() const { return std::hypot(x, y); }

double Vec2::getAngle() const { return std::atan2(y, x); }

void Vec2::rotate(const double rad)
{
    if (isZero())
        return;

    double cosTheta = std::cos(rad);
    double sinTheta = std::sin(rad);
    double newX = x * cosTheta - y * sinTheta;
    double newY = x * sinTheta + y * cosTheta;
    x = newX;
    y = newY;
}

PolarCoordinate Vec2::asPolar() const { return {getAngle(), getLength()}; }

void Vec2::scaleToLength(const double scalar)
{
    double length = getLength();
    if (length == 0.0)
        return;

    double scale = scalar / length;
    x *= scale;
    y *= scale;
}

Vec2 Vec2::project(const Vec2& other) const
{
    double lenSq = other.x * other.x + other.y * other.y;
    if (lenSq == 0.0)
        return {};
    return other * (dot(*this, other) / lenSq);
}

Vec2 Vec2::reject(const Vec2& other) const
{
    Vec2 projection = project(other);
    return *this - projection;
}

Vec2 Vec2::reflect(const Vec2& other) const
{
    Vec2 projection = project(other) * 2;
    return *this - projection;
}

void Vec2::normalize()
{
    double length = getLength();
    if (length == 0.0)
        return;

    x /= length;
    y /= length;
}

double Vec2::distanceTo(const Vec2& other) const { return (other - *this).getLength(); }

Vec2 Vec2::operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
Vec2 Vec2::operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
Vec2 Vec2::operator-() const { return Vec2(-x, -y); }
Vec2& Vec2::operator+=(const Vec2& other)
{
    x += other.x;
    y += other.y;
    return *this;
}
Vec2& Vec2::operator-=(const Vec2& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}
bool Vec2::operator==(const Vec2& other) const
{
    return std::abs(x - other.x) < 1e-8 && std::abs(y - other.y) < 1e-8;
}
bool Vec2::operator!=(const Vec2& other) const { return !(*this == other); }
bool Vec2::operator<(const Vec2& other) const { return (x < other.x && y < other.y); }
bool Vec2::operator>(const Vec2& other) const { return (x > other.x && y > other.y); }
bool Vec2::operator<=(const Vec2& other) const { return !(*this > other); }
bool Vec2::operator>=(const Vec2& other) const { return !(*this < other); }
} // namespace math