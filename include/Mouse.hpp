#pragma once
#include <pybind11/pybind11.h>

namespace py = pybind11;

enum class knMouseButton : uint8_t;

namespace math
{
class Vec2;
}

namespace mouse
{
void _bind(py::module_& module);

math::Vec2 getPos();

math::Vec2 getRel();

bool isPressed(knMouseButton button);

bool isJustPressed(knMouseButton button);

bool isJustReleased(knMouseButton button);

void lock();

void unlock();

bool isLocked();

void hide();

void show();

bool isHidden();
}