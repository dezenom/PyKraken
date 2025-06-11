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

void lineThin(math::Vec2 start, math::Vec2 end, const Color& color);

void line(math::Vec2 start, math::Vec2 end, const Color& color, int thickness);

void point(math::Vec2 point, const Color& color);

void circleThin(math::Vec2 center, int radius, const Color& color);

void circle(math::Vec2 center, int radius, const Color& color, int thickness);
} // namespace draw
