#include "Key.hpp"
#include "Event.hpp"
#include "_globals.hpp"

#include <unordered_map>

static bool _scancodePressed[SDL_SCANCODE_COUNT] = {};
static bool _scancodeReleased[SDL_SCANCODE_COUNT] = {};
static std::unordered_map<SDL_Keycode, bool> _keycodePressed;
static std::unordered_map<SDL_Keycode, bool> _keycodeReleased;

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

bool isJustPressed(SDL_Scancode scancode) { return _scancodePressed[scancode]; }

bool isJustReleased(SDL_Scancode scancode) { return _scancodeReleased[scancode]; }

bool isPressed(KnKeycode keycode)
{
    SDL_Scancode scancode = SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(keycode), nullptr);
    return SDL_GetKeyboardState(nullptr)[scancode];
}

bool isJustPressed(KnKeycode keycode)
{
    auto it = _keycodePressed.find(static_cast<SDL_Keycode>(keycode));
    return it != _keycodePressed.end() && it->second;
}

bool isJustReleased(KnKeycode keycode)
{
    auto it = _keycodeReleased.find(static_cast<SDL_Keycode>(keycode));
    return it != _keycodeReleased.end() && it->second;
}

void _clearStates()
{
    std::fill(std::begin(_scancodePressed), std::end(_scancodePressed), false);
    std::fill(std::begin(_scancodeReleased), std::end(_scancodeReleased), false);
    _keycodePressed.clear();
    _keycodeReleased.clear();
}

void _handleEvents(const SDL_Event& sdle, event::knEvent& e)
{
    switch (sdle.type)
    {
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        if (sdle.type == SDL_EVENT_KEY_DOWN && !sdle.key.repeat)
        {
            _scancodePressed[sdle.key.scancode] = true;
            _keycodePressed[sdle.key.key] = true;
        }
        else if (sdle.type == SDL_EVENT_KEY_UP)
        {
            _scancodeReleased[sdle.key.scancode] = true;
            _keycodeReleased[sdle.key.key] = true;
        }
        e.data["key"] = py::cast(static_cast<KnKeycode>(sdle.key.key));
        e.data["scan"] = py::cast(sdle.key.scancode);
        break;
    default:
        break;
    }
}
} // namespace key