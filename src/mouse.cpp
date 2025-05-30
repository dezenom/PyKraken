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

    subMouse.def("get_pos", &mouse::getPos, R"doc(
        Get the current position of the mouse cursor.

        :return: The current mouse position.
        :rtype: ``tuple[float, float]``
    )doc");
    subMouse.def("get_rel", &mouse::getRel, R"doc(
        Get the relative mouse movement since the last frame.

        :return: The relative movement of the mouse.
        :rtype: ``tuple[float, float]``
    )doc");
    subMouse.def("is_pressed", &mouse::isPressed, py::arg("button"), R"doc(
        Check if a mouse button is currently pressed.

        :param button: The mouse button to check (e.g., kn.MOUSE_LEFT).
        :type button: MouseButton
        :return: True if the button is currently pressed.
        :rtype: ``bool``
    )doc");
    subMouse.def("is_just_pressed", &mouse::isJustPressed, py::arg("button"), R"doc(
        Check if a mouse button was pressed this frame.

        :param button: The mouse button to check.
        :type button: MouseButton
        :return: True if the button was just pressed.
        :rtype: ``bool``
    )doc");
    subMouse.def("is_just_released", &mouse::isJustReleased, py::arg("button"), R"doc(
        Check if a mouse button was released this frame.

        :param button: The mouse button to check.
        :type button: MouseButton
        :return: True if the button was just released.
        :rtype: ``bool``
    )doc");
    subMouse.def("lock", &mouse::lock, R"doc(
        Lock the mouse to the center of the window. Useful for first-person controls.
    )doc");
    subMouse.def("unlock", &mouse::unlock, R"doc(
        Unlock the mouse from the window, allowing it to move freely.
    )doc");
    subMouse.def("is_locked", &mouse::isLocked, R"doc(
        Check if the mouse is currently locked to the window.

        :return: True if the mouse is locked.
        :rtype: ``bool``
    )doc");
    subMouse.def("hide", &mouse::hide, R"doc(
        Hide the mouse cursor from view.
    )doc");
    subMouse.def("show", &mouse::show, R"doc(
        Show the mouse cursor if it was hidden.
    )doc");
    subMouse.def("is_hidden", &mouse::isHidden, R"doc(
        Check if the mouse cursor is currently hidden.

        :return: True if the cursor is hidden.
        :rtype: ``bool``
    )doc");
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

void lock() { SDL_SetWindowRelativeMouseMode(window::getWindow(), true); }

void unlock() { SDL_SetWindowRelativeMouseMode(window::getWindow(), false); }

bool isLocked() { return SDL_GetWindowRelativeMouseMode(window::getWindow()); }

void hide() { SDL_HideCursor(); }

void show() { SDL_ShowCursor(); }

bool isHidden() { return !SDL_CursorVisible(); }

} // namespace mouse
