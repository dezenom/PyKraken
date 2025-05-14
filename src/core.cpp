#include "Window.hpp"
#include "Event.hpp"
#include <SDL3/SDL.h>

PYBIND11_MODULE(_core, m)
{
    m.def("init", &init, "Initialize Kraken Engine");
    m.def("quit", &quit, "Quit Kraken Engine");

    window::_bind(m);
    event::_bind(m);
}
