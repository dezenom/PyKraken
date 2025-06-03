#include "Camera.hpp"
#include "Color.hpp"
#include "Constants.hpp"
#include "Draw.hpp"
#include "Ease.hpp"
#include "Event.hpp"
#include "Gamepad.hpp"
#include "Input.hpp"
#include "Key.hpp"
#include "Math.hpp"
#include "Mouse.hpp"
#include "Rect.hpp"
#include "Time.hpp"
#include "Window.hpp"

PYBIND11_MODULE(_core, m)
{
    py::options options;
    options.disable_function_signatures();

    m.def("init", &init, "Initialize Kraken Engine");
    m.def("quit", &quit, "Quit Kraken Engine");

    camera::_bind(m);
    color::_bind(m);
    constants::_bind(m);
    draw::_bind(m);
    ease::_bind(m);
    event::_bind(m);
    gamepad::_bind(m);
    input::_bind(m);
    key::_bind(m);
    math::_bind(m);
    mouse::_bind(m);
    rect::_bind(m);
    kn::time::_bind(m);
    window::_bind(m);
}
