#include "_globals.hpp"

// Scancodes
bool g_scancodePressed[SDL_SCANCODE_COUNT] = {};
bool g_scancodeReleased[SDL_SCANCODE_COUNT] = {};

// Keycodes
std::unordered_map<SDL_Keycode, bool> g_keycodePressed;
std::unordered_map<SDL_Keycode, bool> g_keycodeReleased;

// Mouse buttons
bool g_mousePressed[MOUSE_BUTTON_COUNT] = {};
bool g_mouseReleased[MOUSE_BUTTON_COUNT] = {};
