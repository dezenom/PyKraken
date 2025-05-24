#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;

struct Color;
struct Rect;
namespace math
{
class Vec2;
} // namespace math

namespace draw
{
void _bind(pybind11::module_& module);

void rect(const Rect& rect, const Color& color, int thickness = 0);

void line(const math::Vec2& start, const math::Vec2& end, const Color& color, int thickness = 1);

void point(const math::Vec2& point, const Color& color);
} // namespace draw
