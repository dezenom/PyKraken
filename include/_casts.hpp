#pragma once

#include "Math.hpp"
#include "Rect.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

inline math::Vec2 toVec2(py::handle obj)
{
    if (py::isinstance<math::Vec2>(obj))
    {
        return obj.cast<math::Vec2>();
    }

    if (py::isinstance<py::sequence>(obj))
    {
        auto seq = obj.cast<py::sequence>();
        if (seq.size() != 2)
            throw std::invalid_argument("Expected a sequence of length 2");
        return math::Vec2(seq[0].cast<double>(), seq[1].cast<double>());
    }

    throw std::invalid_argument("Expected a Vec2 or a sequence of length 2");
}
