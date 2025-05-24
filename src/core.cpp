#include "Color.hpp"
#include "Constants.hpp"
#include "Event.hpp"
#include "Key.hpp"
#include "Math.hpp"
#include "Mouse.hpp"
#include "Rect.hpp"
#include "Time.hpp"
#include "Window.hpp"

PYBIND11_MODULE(_core, m)
{
    m.def("init", &init, "Initialize Kraken Engine");
    m.def("quit", &quit, "Quit Kraken Engine");

    color::_bind(m);
    constants::_bind(m);
    event::_bind(m);
    key::_bind(m);
    math::_bind(m);
    mouse::_bind(m);
    rect::_bind(m);
    kn::time::_bind(m);
    window::_bind(m);
}
