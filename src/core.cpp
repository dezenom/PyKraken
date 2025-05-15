#include "Event.hpp"
#include "Time.hpp"
#include "Window.hpp"

PYBIND11_MODULE(_core, m)
{
    m.def("init", &init, "Initialize Kraken Engine");
    m.def("quit", &quit, "Quit Kraken Engine");

    window::_bind(m);
    event::_bind(m);
    kn::time::_bind(m);
}
