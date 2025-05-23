#include "Mouse.hpp"
#include "Math.hpp"
#include "Window.hpp"
#include "_globals.hpp"

#include <iostream>

namespace mouse
{
void _bind(pybind11::module_& module)
{
    auto subMouse = module.def_submodule("mouse", "Mouse related functions");
    subMouse.def("get_pos", &mouse::getPos, "Get the current mouse position");
    subMouse.def("get_rel", &mouse::getRel, "Get the relative mouse movement");
    subMouse.def("is_pressed", &mouse::isPressed, py::arg("button"),
                 "Check if a mouse button is pressed");
    subMouse.def("is_just_pressed", &mouse::isJustPressed, py::arg("button"),
                 "Check if a mouse button was just pressed");
    subMouse.def("is_just_released", &mouse::isJustReleased, py::arg("button"),
                 "Check if a mouse button was just released");
    subMouse.def("lock", &mouse::lock, "Lock the mouse to the window");
    subMouse.def("unlock", &mouse::unlock, "Unlock the mouse from the window");
    subMouse.def("is_locked", &mouse::isLocked, "Check if the mouse is locked to the window");
    subMouse.def("hide", &mouse::hide, "Hide the mouse cursor");
    subMouse.def("show", &mouse::show, "Show the mouse cursor");
    subMouse.def("is_hidden", &mouse::isHidden, "Check if the mouse cursor is hidden");
}

py::tuple getPos()
{
    float x, y;
    SDL_GetMouseState(&x, &y);
    const auto s = window::getScale();
    return py::make_tuple(x / s, y / s);
}

py::tuple getRel()
{
    float dx, dy;
    SDL_GetRelativeMouseState(&dx, &dy);
    const auto s = window::getScale();
    return py::make_tuple(dx / s, dy / s);
}

bool isPressed(knMouseButton button)
{
    return SDL_GetMouseState(nullptr, nullptr) & static_cast<uint32_t>(button);
}

bool isJustPressed(knMouseButton button) { return g_mousePressed[static_cast<size_t>(button) - 1]; }

bool isJustReleased(knMouseButton button)
{
    return g_mouseReleased[static_cast<size_t>(button) - 1];
}

void lock() { SDL_SetWindowRelativeMouseMode(window::get(), true); }

void unlock() { SDL_SetWindowRelativeMouseMode(window::get(), false); }

bool isLocked() { return SDL_GetWindowRelativeMouseMode(window::get()); }

void hide() { SDL_HideCursor(); }

void show() { SDL_ShowCursor(); }

bool isHidden() { return !SDL_CursorVisible(); }

} // namespace mouse
