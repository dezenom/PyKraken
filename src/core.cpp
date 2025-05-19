#include "Constants.hpp"
#include "Event.hpp"
#include "Key.hpp"
#include "Math.hpp"
#include "Mouse.hpp"
#include "Time.hpp"
#include "Window.hpp"

PYBIND11_MODULE(_core, m)
{
    m.def("init", &init, "Initialize Kraken Engine");
    m.def("quit", &quit, "Quit Kraken Engine");

    constants::_bind(m);
    event::_bind(m);
    key::_bind(m);
    math::_bind(m);
    mouse::_bind(m);
    kn::time::_bind(m);
    window::_bind(m);
}
