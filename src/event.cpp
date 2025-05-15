#include "Event.hpp"
#include "Window.hpp"

#include <SDL3/SDL.h>
#include <pybind11/stl.h>

#define UINT32(x) static_cast<uint32_t>(x)
#define UINT16(x) static_cast<uint16_t>(x)
#define UINT8(x) static_cast<uint8_t>(x)

namespace event
{
std::vector<knEvent> get()
{
    std::vector<knEvent> events;
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        knEvent e(event.type);
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            window::close();
            break;
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
            e.data["key"] = event.key.key;
            e.data["scancode"] = UINT32(event.key.scancode);
            break;
        default:
            break;
        }
        events.push_back(std::move(e));
    }

    return events;
}

py::object knEvent::getAttr(const std::string& name) const
{
    if (name == "type")
        return py::int_(type);
    if (data.contains(name))
        return data[name.c_str()];
    throw py::attribute_error("Attribute '" + name + "' not found");
}

void _bind(py::module_& module)
{
    py::class_<knEvent>(module, "Event")
        .def_readonly("type", &knEvent::type)
        .def("__getattr__", &knEvent::getAttr);

    auto subEvent = module.def_submodule("event", "Event related functions");
    subEvent.def("get", &get, "Get all window user events");

    // Define event types
    module.attr("QUIT") = UINT32(SDL_EVENT_QUIT);

    module.attr("WINDOWSHOWN") = UINT32(SDL_EVENT_WINDOW_SHOWN);
    module.attr("WINDOWHIDDEN") = UINT32(SDL_EVENT_WINDOW_HIDDEN);
    module.attr("WINDOWEXPOSED") = UINT32(SDL_EVENT_WINDOW_EXPOSED);
    module.attr("WINDOWMOVED") = UINT32(SDL_EVENT_WINDOW_MOVED);
    module.attr("WINDOWRESIZED") = UINT32(SDL_EVENT_WINDOW_RESIZED);
    module.attr("WINDOWMINIMIZED") = UINT32(SDL_EVENT_WINDOW_MINIMIZED);
    module.attr("WINDOWMAXIMIZED") = UINT32(SDL_EVENT_WINDOW_MAXIMIZED);
    module.attr("WINDOWRESTORED") = UINT32(SDL_EVENT_WINDOW_RESTORED);
    module.attr("WINDOWMOUSEENTER") = UINT32(SDL_EVENT_WINDOW_MOUSE_ENTER);
    module.attr("WINDOWMOUSELEAVE") = UINT32(SDL_EVENT_WINDOW_MOUSE_LEAVE);
    module.attr("WINDOWFOCUSGAINED") = UINT32(SDL_EVENT_WINDOW_FOCUS_GAINED);
    module.attr("WINDOWFOCUSLOST") = UINT32(SDL_EVENT_WINDOW_FOCUS_LOST);
    module.attr("WINDOWOCCLUDED") = UINT32(SDL_EVENT_WINDOW_OCCLUDED);
    module.attr("WINDOWENTERFULLSCREEN") = UINT32(SDL_EVENT_WINDOW_ENTER_FULLSCREEN);
    module.attr("WINDOWLEAVEFULLSCREEN") = UINT32(SDL_EVENT_WINDOW_LEAVE_FULLSCREEN);

    module.attr("KEYDOWN") = UINT32(SDL_EVENT_KEY_DOWN);
    module.attr("KEYUP") = UINT32(SDL_EVENT_KEY_UP);
    module.attr("TEXTEDITING") = UINT32(SDL_EVENT_TEXT_EDITING);
    module.attr("TEXTINPUT") = UINT32(SDL_EVENT_TEXT_INPUT);
    module.attr("KEYBOARD_ADDED") = UINT32(SDL_EVENT_KEYBOARD_ADDED);
    module.attr("KEYBOARD_REMOVED") = UINT32(SDL_EVENT_KEYBOARD_REMOVED);

    module.attr("MOUSEMOTION") = UINT32(SDL_EVENT_MOUSE_MOTION);
    module.attr("MOUSEBUTTONDOWN") = UINT32(SDL_EVENT_MOUSE_BUTTON_DOWN);
    module.attr("MOUSEBUTTONUP") = UINT32(SDL_EVENT_MOUSE_BUTTON_UP);
    module.attr("MOUSEWHEEL") = UINT32(SDL_EVENT_MOUSE_WHEEL);
    module.attr("MOUSEADDED") = UINT32(SDL_EVENT_MOUSE_ADDED);
    module.attr("MOUSEREMOVED") = UINT32(SDL_EVENT_MOUSE_REMOVED);

    module.attr("DROPFILE") = UINT32(SDL_EVENT_DROP_FILE);
    module.attr("DROPTEXT") = UINT32(SDL_EVENT_DROP_TEXT);
    module.attr("DROPBEGIN") = UINT32(SDL_EVENT_DROP_BEGIN);
    module.attr("DROPCOMPLETE") = UINT32(SDL_EVENT_DROP_COMPLETE);
    module.attr("DROPPOSITION") = UINT32(SDL_EVENT_DROP_POSITION);

    module.attr("CAMERAADDED") = UINT32(SDL_EVENT_CAMERA_DEVICE_ADDED);
    module.attr("CAMERAREMOVED") = UINT32(SDL_EVENT_CAMERA_DEVICE_REMOVED);
    module.attr("CAMERAAPPROVED") = UINT32(SDL_EVENT_CAMERA_DEVICE_APPROVED);
    module.attr("CAMERADENIED") = UINT32(SDL_EVENT_CAMERA_DEVICE_DENIED);

    // Mouse buttons
    module.attr("M_LEFT") = UINT8(SDL_BUTTON_LEFT);
    module.attr("M_MIDDLE") = UINT8(SDL_BUTTON_MIDDLE);
    module.attr("M_RIGHT") = UINT8(SDL_BUTTON_RIGHT);
    module.attr("M_SIDE1") = UINT8(SDL_BUTTON_X1);
    module.attr("M_SIDE2") = UINT8(SDL_BUTTON_X2);

    // Scancodes
    module.attr("S_a") = UINT16(SDL_SCANCODE_A);
    module.attr("S_b") = UINT16(SDL_SCANCODE_B);
    module.attr("S_c") = UINT16(SDL_SCANCODE_C);
    module.attr("S_d") = UINT16(SDL_SCANCODE_D);
    module.attr("S_e") = UINT16(SDL_SCANCODE_E);
    module.attr("S_f") = UINT16(SDL_SCANCODE_F);
    module.attr("S_g") = UINT16(SDL_SCANCODE_G);
    module.attr("S_h") = UINT16(SDL_SCANCODE_H);
    module.attr("S_i") = UINT16(SDL_SCANCODE_I);
    module.attr("S_j") = UINT16(SDL_SCANCODE_J);
    module.attr("S_k") = UINT16(SDL_SCANCODE_K);
    module.attr("S_l") = UINT16(SDL_SCANCODE_L);
    module.attr("S_m") = UINT16(SDL_SCANCODE_M);
    module.attr("S_n") = UINT16(SDL_SCANCODE_N);
    module.attr("S_o") = UINT16(SDL_SCANCODE_O);
    module.attr("S_p") = UINT16(SDL_SCANCODE_P);
    module.attr("S_q") = UINT16(SDL_SCANCODE_Q);
    module.attr("S_r") = UINT16(SDL_SCANCODE_R);
    module.attr("S_s") = UINT16(SDL_SCANCODE_S);
    module.attr("S_t") = UINT16(SDL_SCANCODE_T);
    module.attr("S_u") = UINT16(SDL_SCANCODE_U);
    module.attr("S_v") = UINT16(SDL_SCANCODE_V);
    module.attr("S_w") = UINT16(SDL_SCANCODE_W);
    module.attr("S_x") = UINT16(SDL_SCANCODE_X);
    module.attr("S_y") = UINT16(SDL_SCANCODE_Y);
    module.attr("S_z") = UINT16(SDL_SCANCODE_Z);

    module.attr("S_1") = UINT16(SDL_SCANCODE_1);
    module.attr("S_2") = UINT16(SDL_SCANCODE_2);
    module.attr("S_3") = UINT16(SDL_SCANCODE_3);
    module.attr("S_4") = UINT16(SDL_SCANCODE_4);
    module.attr("S_5") = UINT16(SDL_SCANCODE_5);
    module.attr("S_6") = UINT16(SDL_SCANCODE_6);
    module.attr("S_7") = UINT16(SDL_SCANCODE_7);
    module.attr("S_8") = UINT16(SDL_SCANCODE_8);
    module.attr("S_9") = UINT16(SDL_SCANCODE_9);
    module.attr("S_0") = UINT16(SDL_SCANCODE_0);

    module.attr("S_RETURN") = UINT16(SDL_SCANCODE_RETURN);
    module.attr("S_ESC") = UINT16(SDL_SCANCODE_ESCAPE);
    module.attr("S_BACKSPACE") = UINT16(SDL_SCANCODE_BACKSPACE);
    module.attr("S_TAB") = UINT16(SDL_SCANCODE_TAB);
    module.attr("S_SPACE") = UINT16(SDL_SCANCODE_SPACE);

    module.attr("S_MINUS") = UINT16(SDL_SCANCODE_MINUS);
    module.attr("S_EQ") = UINT16(SDL_SCANCODE_EQUALS);
    module.attr("S_LBRACKET") = UINT16(SDL_SCANCODE_LEFTBRACKET);
    module.attr("S_RBRACKET") = UINT16(SDL_SCANCODE_RIGHTBRACKET);
    module.attr("S_BACKSLASH") = UINT16(SDL_SCANCODE_BACKSLASH);
    module.attr("S_SEMICOLON") = UINT16(SDL_SCANCODE_SEMICOLON);
    module.attr("S_APOSTROPHE") = UINT16(SDL_SCANCODE_APOSTROPHE);
    module.attr("S_GRAVE") = UINT16(SDL_SCANCODE_GRAVE);
    module.attr("S_COMMA") = UINT16(SDL_SCANCODE_COMMA);
    module.attr("S_PERIOD") = UINT16(SDL_SCANCODE_PERIOD);
    module.attr("S_SLASH") = UINT16(SDL_SCANCODE_SLASH);
    module.attr("S_CAPS") = UINT16(SDL_SCANCODE_CAPSLOCK);

    module.attr("S_F1") = UINT16(SDL_SCANCODE_F1);
    module.attr("S_F2") = UINT16(SDL_SCANCODE_F2);
    module.attr("S_F3") = UINT16(SDL_SCANCODE_F3);
    module.attr("S_F4") = UINT16(SDL_SCANCODE_F4);
    module.attr("S_F5") = UINT16(SDL_SCANCODE_F5);
    module.attr("S_F6") = UINT16(SDL_SCANCODE_F6);
    module.attr("S_F7") = UINT16(SDL_SCANCODE_F7);
    module.attr("S_F8") = UINT16(SDL_SCANCODE_F8);
    module.attr("S_F9") = UINT16(SDL_SCANCODE_F9);
    module.attr("S_F10") = UINT16(SDL_SCANCODE_F10);
    module.attr("S_F11") = UINT16(SDL_SCANCODE_F11);
    module.attr("S_F12") = UINT16(SDL_SCANCODE_F12);

    module.attr("S_PRTSCR") = UINT16(SDL_SCANCODE_PRINTSCREEN);
    module.attr("S_SCRLK") = UINT16(SDL_SCANCODE_SCROLLLOCK);
    module.attr("S_PAUSE") = UINT16(SDL_SCANCODE_PAUSE);
    module.attr("S_INS") = UINT16(SDL_SCANCODE_INSERT);

    module.attr("S_HOME") = UINT16(SDL_SCANCODE_HOME);
    module.attr("S_PGUP") = UINT16(SDL_SCANCODE_PAGEUP);
    module.attr("S_DEL") = UINT16(SDL_SCANCODE_DELETE);
    module.attr("S_END") = UINT16(SDL_SCANCODE_END);
    module.attr("S_PGDOWN") = UINT16(SDL_SCANCODE_PAGEDOWN);
    module.attr("S_RIGHT") = UINT16(SDL_SCANCODE_RIGHT);
    module.attr("S_LEFT") = UINT16(SDL_SCANCODE_LEFT);
    module.attr("S_DOWN") = UINT16(SDL_SCANCODE_DOWN);
    module.attr("S_UP") = UINT16(SDL_SCANCODE_UP);
    module.attr("S_NUMLOCK") = UINT16(SDL_SCANCODE_NUMLOCKCLEAR);

    module.attr("S_KP_DIV") = UINT16(SDL_SCANCODE_KP_DIVIDE);
    module.attr("S_KP_MULT") = UINT16(SDL_SCANCODE_KP_MULTIPLY);
    module.attr("S_KP_MINUS") = UINT16(SDL_SCANCODE_KP_MINUS);
    module.attr("S_KP_PLUS") = UINT16(SDL_SCANCODE_KP_PLUS);
    module.attr("S_KP_ENTER") = UINT16(SDL_SCANCODE_KP_ENTER);
    module.attr("S_KP_1") = UINT16(SDL_SCANCODE_KP_1);
    module.attr("S_KP_2") = UINT16(SDL_SCANCODE_KP_2);
    module.attr("S_KP_3") = UINT16(SDL_SCANCODE_KP_3);
    module.attr("S_KP_4") = UINT16(SDL_SCANCODE_KP_4);
    module.attr("S_KP_5") = UINT16(SDL_SCANCODE_KP_5);
    module.attr("S_KP_6") = UINT16(SDL_SCANCODE_KP_6);
    module.attr("S_KP_7") = UINT16(SDL_SCANCODE_KP_7);
    module.attr("S_KP_8") = UINT16(SDL_SCANCODE_KP_8);
    module.attr("S_KP_9") = UINT16(SDL_SCANCODE_KP_9);
    module.attr("S_KP_0") = UINT16(SDL_SCANCODE_KP_0);
    module.attr("S_KP_PERIOD") = UINT16(SDL_SCANCODE_KP_PERIOD);

    module.attr("S_AGAIN") = UINT16(SDL_SCANCODE_AGAIN);
    module.attr("S_UNDO") = UINT16(SDL_SCANCODE_UNDO);
    module.attr("S_CUT") = UINT16(SDL_SCANCODE_CUT);
    module.attr("S_COPY") = UINT16(SDL_SCANCODE_COPY);
    module.attr("S_PASTE") = UINT16(SDL_SCANCODE_PASTE);
    module.attr("S_FIND") = UINT16(SDL_SCANCODE_FIND);
    module.attr("S_MUTE") = UINT16(SDL_SCANCODE_MUTE);
    module.attr("S_VOLUP") = UINT16(SDL_SCANCODE_VOLUMEUP);
    module.attr("S_VOLDOWN") = UINT16(SDL_SCANCODE_VOLUMEDOWN);

    module.attr("S_LCTRL") = UINT16(SDL_SCANCODE_LCTRL);
    module.attr("S_LSHIFT") = UINT16(SDL_SCANCODE_LSHIFT);
    module.attr("S_LALT") = UINT16(SDL_SCANCODE_LALT);
    module.attr("S_LGUI") = UINT16(SDL_SCANCODE_LGUI);
    module.attr("S_RCTRL") = UINT16(SDL_SCANCODE_RCTRL);
    module.attr("S_RSHIFT") = UINT16(SDL_SCANCODE_RSHIFT);
    module.attr("S_RALT") = UINT16(SDL_SCANCODE_RALT);
    module.attr("S_RGUI") = UINT16(SDL_SCANCODE_RGUI);

    // Keycodes
    module.attr("K_BACKSPACE") = UINT32(SDLK_BACKSPACE);
    module.attr("K_TAB") = UINT32(SDLK_TAB);
    module.attr("K_RETURN") = UINT32(SDLK_RETURN);
    module.attr("K_ESC") = UINT32(SDLK_ESCAPE);
    module.attr("K_SPACE") = UINT32(SDLK_SPACE);
    module.attr("K_EXCLAIM") = UINT32(SDLK_EXCLAIM);
    module.attr("K_DBLQUOTE") = UINT32(SDLK_DBLAPOSTROPHE);
    module.attr("K_HASH") = UINT32(SDLK_HASH);
    module.attr("K_DOLLAR") = UINT32(SDLK_DOLLAR);
    module.attr("K_PERCENT") = UINT32(SDLK_PERCENT);
    module.attr("K_AMPERSAND") = UINT32(SDLK_AMPERSAND);
    module.attr("K_SGLQUOTE") = UINT32(SDLK_APOSTROPHE);
    module.attr("K_LPAREN") = UINT32(SDLK_LEFTBRACKET);
    module.attr("K_RPAREN") = UINT32(SDLK_RIGHTBRACKET);
    module.attr("K_ASTERISK") = UINT32(SDLK_ASTERISK);
    module.attr("K_PLUS") = UINT32(SDLK_PLUS);
    module.attr("K_COMMA") = UINT32(SDLK_COMMA);
    module.attr("K_MINUS") = UINT32(SDLK_MINUS);
    module.attr("K_PERIOD") = UINT32(SDLK_PERIOD);
    module.attr("K_SLASH") = UINT32(SDLK_SLASH);

    module.attr("K_0") = UINT32(SDLK_0);
    module.attr("K_1") = UINT32(SDLK_1);
    module.attr("K_2") = UINT32(SDLK_2);
    module.attr("K_3") = UINT32(SDLK_3);
    module.attr("K_4") = UINT32(SDLK_4);
    module.attr("K_5") = UINT32(SDLK_5);
    module.attr("K_6") = UINT32(SDLK_6);
    module.attr("K_7") = UINT32(SDLK_7);
    module.attr("K_8") = UINT32(SDLK_8);
    module.attr("K_9") = UINT32(SDLK_9);

    module.attr("K_COLON") = UINT32(SDLK_COLON);
    module.attr("K_SEMICOLON") = UINT32(SDLK_SEMICOLON);
    module.attr("K_LT") = UINT32(SDLK_LESS);
    module.attr("K_EQ") = UINT32(SDLK_EQUALS);
    module.attr("K_GT") = UINT32(SDLK_GREATER);
    module.attr("K_QUESTION") = UINT32(SDLK_QUESTION);
    module.attr("K_AT") = UINT32(SDLK_AT);
    module.attr("K_LBRACKET") = UINT32(SDLK_LEFTBRACKET);
    module.attr("K_BACKSLASH") = UINT32(SDLK_BACKSLASH);
    module.attr("K_RBRACKET") = UINT32(SDLK_RIGHTBRACKET);
    module.attr("K_CARET") = UINT32(SDLK_CARET);
    module.attr("K_UNDERSCORE") = UINT32(SDLK_UNDERSCORE);
    module.attr("K_GRAVE") = UINT32(SDLK_GRAVE);

    module.attr("K_a") = UINT32(SDLK_A);
    module.attr("K_b") = UINT32(SDLK_B);
    module.attr("K_c") = UINT32(SDLK_C);
    module.attr("K_d") = UINT32(SDLK_D);
    module.attr("K_e") = UINT32(SDLK_E);
    module.attr("K_f") = UINT32(SDLK_F);
    module.attr("K_g") = UINT32(SDLK_G);
    module.attr("K_h") = UINT32(SDLK_H);
    module.attr("K_i") = UINT32(SDLK_I);
    module.attr("K_j") = UINT32(SDLK_J);
    module.attr("K_k") = UINT32(SDLK_K);
    module.attr("K_l") = UINT32(SDLK_L);
    module.attr("K_m") = UINT32(SDLK_M);
    module.attr("K_n") = UINT32(SDLK_N);
    module.attr("K_o") = UINT32(SDLK_O);
    module.attr("K_p") = UINT32(SDLK_P);
    module.attr("K_q") = UINT32(SDLK_Q);
    module.attr("K_r") = UINT32(SDLK_R);
    module.attr("K_s") = UINT32(SDLK_S);
    module.attr("K_t") = UINT32(SDLK_T);
    module.attr("K_u") = UINT32(SDLK_U);
    module.attr("K_v") = UINT32(SDLK_V);
    module.attr("K_w") = UINT32(SDLK_W);
    module.attr("K_x") = UINT32(SDLK_X);
    module.attr("K_y") = UINT32(SDLK_Y);
    module.attr("K_z") = UINT32(SDLK_Z);

    module.attr("K_LBRACE") = UINT32(SDLK_LEFTBRACE);
    module.attr("K_PIPE") = UINT32(SDLK_PIPE);
    module.attr("K_RBRACE") = UINT32(SDLK_RIGHTBRACE);
    module.attr("K_TILDE") = UINT32(SDLK_TILDE);
    module.attr("K_DEL") = UINT32(SDLK_DELETE);
    module.attr("K_CAPS") = UINT32(SDLK_CAPSLOCK);
    module.attr("K_F1") = UINT32(SDLK_F1);
    module.attr("K_F2") = UINT32(SDLK_F2);
    module.attr("K_F3") = UINT32(SDLK_F3);
    module.attr("K_F4") = UINT32(SDLK_F4);
    module.attr("K_F5") = UINT32(SDLK_F5);
    module.attr("K_F6") = UINT32(SDLK_F6);
    module.attr("K_F7") = UINT32(SDLK_F7);
    module.attr("K_F8") = UINT32(SDLK_F8);
    module.attr("K_F9") = UINT32(SDLK_F9);
    module.attr("K_F10") = UINT32(SDLK_F10);
    module.attr("K_F11") = UINT32(SDLK_F11);
    module.attr("K_F12") = UINT32(SDLK_F12);

    module.attr("K_PRTSCR") = UINT32(SDLK_PRINTSCREEN);
    module.attr("K_SCRLK") = UINT32(SDLK_SCROLLLOCK);
    module.attr("K_PAUSE") = UINT32(SDLK_PAUSE);
    module.attr("K_INS") = UINT32(SDLK_INSERT);
    module.attr("K_HOME") = UINT32(SDLK_HOME);
    module.attr("K_PGUP") = UINT32(SDLK_PAGEUP);
    module.attr("K_END") = UINT32(SDLK_END);
    module.attr("K_PGDOWN") = UINT32(SDLK_PAGEDOWN);
    module.attr("K_RIGHT") = UINT32(SDLK_RIGHT);
    module.attr("K_LEFT") = UINT32(SDLK_LEFT);
    module.attr("K_DOWN") = UINT32(SDLK_DOWN);
    module.attr("K_UP") = UINT32(SDLK_UP);
    module.attr("K_NUMLOCK") = UINT32(SDLK_NUMLOCKCLEAR);

    module.attr("K_KP_DIV") = UINT32(SDLK_KP_DIVIDE);
    module.attr("K_KP_MULT") = UINT32(SDLK_KP_MULTIPLY);
    module.attr("K_KP_MINUS") = UINT32(SDLK_KP_MINUS);
    module.attr("K_KP_PLUS") = UINT32(SDLK_KP_PLUS);
    module.attr("K_KP_ENTER") = UINT32(SDLK_KP_ENTER);
    module.attr("K_KP_1") = UINT32(SDLK_KP_1);
    module.attr("K_KP_2") = UINT32(SDLK_KP_2);
    module.attr("K_KP_3") = UINT32(SDLK_KP_3);
    module.attr("K_KP_4") = UINT32(SDLK_KP_4);
    module.attr("K_KP_5") = UINT32(SDLK_KP_5);
    module.attr("K_KP_6") = UINT32(SDLK_KP_6);
    module.attr("K_KP_7") = UINT32(SDLK_KP_7);
    module.attr("K_KP_8") = UINT32(SDLK_KP_8);
    module.attr("K_KP_9") = UINT32(SDLK_KP_9);
    module.attr("K_KP_0") = UINT32(SDLK_KP_0);
    module.attr("K_KP_PERIOD") = UINT32(SDLK_KP_PERIOD);

    module.attr("K_AGAIN") = UINT32(SDLK_AGAIN);
    module.attr("K_UNDO") = UINT32(SDLK_UNDO);
    module.attr("K_CUT") = UINT32(SDLK_CUT);
    module.attr("K_COPY") = UINT32(SDLK_COPY);
    module.attr("K_PASTE") = UINT32(SDLK_PASTE);
    module.attr("K_FIND") = UINT32(SDLK_FIND);
    module.attr("K_MUTE") = UINT32(SDLK_MUTE);
    module.attr("K_VOLUP") = UINT32(SDLK_VOLUMEUP);
    module.attr("K_VOLDOWN") = UINT32(SDLK_VOLUMEDOWN);
    module.attr("K_LPAREN") = UINT32(SDLK_LEFTPAREN);
    module.attr("K_RPAREN") = UINT32(SDLK_RIGHTPAREN);
    module.attr("K_LBRACE") = UINT32(SDLK_LEFTBRACE);
    module.attr("K_RBRACE") = UINT32(SDLK_RIGHTBRACE);
    module.attr("K_TAB") = UINT32(SDLK_TAB);
    module.attr("K_BACKSPACE") = UINT32(SDLK_BACKSPACE);
    module.attr("K_LCTRL") = UINT32(SDLK_LCTRL);
    module.attr("K_LSHIFT") = UINT32(SDLK_LSHIFT);
    module.attr("K_LALT") = UINT32(SDLK_LALT);
    module.attr("K_LGUI") = UINT32(SDLK_LGUI);
    module.attr("K_RCTRL") = UINT32(SDLK_RCTRL);
    module.attr("K_RSHIFT") = UINT32(SDLK_RSHIFT);
    module.attr("K_RALT") = UINT32(SDLK_RALT);
    module.attr("K_RGUI") = UINT32(SDLK_RGUI);

    // Gamepad buttons
    module.attr("C_SOUTH") = UINT8(SDL_GAMEPAD_BUTTON_SOUTH);
    module.attr("C_EAST") = UINT8(SDL_GAMEPAD_BUTTON_EAST);
    module.attr("C_WEST") = UINT8(SDL_GAMEPAD_BUTTON_WEST);
    module.attr("C_NORTH") = UINT8(SDL_GAMEPAD_BUTTON_NORTH);
    module.attr("C_BACK") = UINT8(SDL_GAMEPAD_BUTTON_BACK);
    module.attr("C_GUIDE") = UINT8(SDL_GAMEPAD_BUTTON_GUIDE);
    module.attr("C_START") = UINT8(SDL_GAMEPAD_BUTTON_START);
    module.attr("C_LEFTSTICK") = UINT8(SDL_GAMEPAD_BUTTON_LEFT_STICK);
    module.attr("C_RIGHTSTICK") = UINT8(SDL_GAMEPAD_BUTTON_RIGHT_STICK);
    module.attr("C_LEFTSHOULDER") = UINT8(SDL_GAMEPAD_BUTTON_LEFT_SHOULDER);
    module.attr("C_RIGHTSHOULDER") = UINT8(SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER);
    module.attr("C_DPADUP") = UINT8(SDL_GAMEPAD_BUTTON_DPAD_UP);
    module.attr("C_DPADDOWN") = UINT8(SDL_GAMEPAD_BUTTON_DPAD_DOWN);
    module.attr("C_DPADLEFT") = UINT8(SDL_GAMEPAD_BUTTON_DPAD_LEFT);
    module.attr("C_DPADRIGHT") = UINT8(SDL_GAMEPAD_BUTTON_DPAD_RIGHT);

    // Gamepad axes
    module.attr("C_LX") = UINT8(SDL_GAMEPAD_AXIS_LEFTX);
    module.attr("C_LY") = UINT8(SDL_GAMEPAD_AXIS_LEFTY);
    module.attr("C_RX") = UINT8(SDL_GAMEPAD_AXIS_RIGHTX);
    module.attr("C_RY") = UINT8(SDL_GAMEPAD_AXIS_RIGHTY);
    module.attr("C_LTRIGGER") = UINT8(SDL_GAMEPAD_AXIS_LEFT_TRIGGER);
    module.attr("C_RTRIGGER") = UINT8(SDL_GAMEPAD_AXIS_RIGHT_TRIGGER);

    // Gamepad types
    module.attr("STANDARD") = UINT8(SDL_GAMEPAD_TYPE_STANDARD);
    module.attr("C_XBOX360") = UINT8(SDL_GAMEPAD_TYPE_XBOX360);
    module.attr("C_XBOXONE") = UINT8(SDL_GAMEPAD_TYPE_XBOXONE);
    module.attr("C_PS3") = UINT8(SDL_GAMEPAD_TYPE_PS3);
    module.attr("C_PS4") = UINT8(SDL_GAMEPAD_TYPE_PS4);
    module.attr("C_PS5") = UINT8(SDL_GAMEPAD_TYPE_PS5);
    module.attr("C_SWITCHPRO") = UINT8(SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO);
    module.attr("C_SWITCHJOYCONLEFT") = UINT8(SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT);
    module.attr("C_SWITCHJOYCONRIGHT") = UINT8(SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT);
    module.attr("C_SWITCHJOYCONPAIR") = UINT8(SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR);
}
} // namespace event