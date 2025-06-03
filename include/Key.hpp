#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

enum class KnKeycode : SDL_Keycode;
namespace event
{
class knEvent;
}

namespace key
{
void _bind(py::module_& module);

void _handleEvents(const SDL_Event& sdle, event::knEvent& e);

void _clearStates();

bool isPressed(SDL_Scancode scancode);

bool isJustPressed(SDL_Scancode scancode);

bool isJustReleased(SDL_Scancode scancode);

bool isPressed(KnKeycode keycode);

bool isJustPressed(KnKeycode keycode);

bool isJustReleased(KnKeycode keycode);
} // namespace key