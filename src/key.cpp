#include "Key.hpp"
#include "_globals.hpp"

namespace key
{
void _bind(py::module_& module)
{
    auto subKey = module.def_submodule("key", "Key related functions");
    subKey.def("is_pressed", static_cast<bool (*)(SDL_Scancode)>(&isPressed),
               "Check if a key is pressed by scancode");
    subKey.def("is_just_pressed", static_cast<bool (*)(SDL_Scancode)>(&isJustPressed),
               "Check if a key is just pressed by scancode");
    subKey.def("is_just_released", static_cast<bool (*)(SDL_Scancode)>(&isJustReleased),
               "Check if a key is just released by scancode");
    subKey.def("is_pressed", static_cast<bool (*)(KnKeycode)>(&isPressed),
               "Check if a key is pressed by keycode");
    subKey.def("is_just_pressed", static_cast<bool (*)(KnKeycode)>(&isJustPressed),
               "Check if a key is just pressed by keycode");
    subKey.def("is_just_released", static_cast<bool (*)(KnKeycode)>(&isJustReleased),
               "Check if a key is just released by keycode");
}

bool isPressed(SDL_Scancode scancode) { return SDL_GetKeyboardState(nullptr)[scancode]; }

bool isJustPressed(SDL_Scancode scancode) { return g_scancodePressed[scancode]; }

bool isJustReleased(SDL_Scancode scancode) { return g_scancodeReleased[scancode]; }

bool isPressed(KnKeycode keycode)
{
    SDL_Scancode scancode = SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(keycode), nullptr);
    return SDL_GetKeyboardState(nullptr)[scancode];
}

bool isJustPressed(KnKeycode keycode)
{
    auto it = g_keycodePressed.find(static_cast<SDL_Keycode>(keycode));
    return it != g_keycodePressed.end() && it->second;
}

bool isJustReleased(KnKeycode keycode)
{
    auto it = g_keycodeReleased.find(static_cast<SDL_Keycode>(keycode));
    return it != g_keycodeReleased.end() && it->second;
}
} // namespace key