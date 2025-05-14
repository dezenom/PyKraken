#pragma once
#include <pybind11/pybind11.h>
#include <SDL3/SDL.h>

namespace py = pybind11;

namespace event
{
    class knEvent
    {
    public:
        uint32_t type;
        py::dict data;

        knEvent(uint32_t type) : type(type) {}

        py::object getAttr(const std::string &name) const;
    };

    void _bind(py::module_ &module);

    std::vector<knEvent> get();
}
