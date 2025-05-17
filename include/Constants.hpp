#pragma once
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace constants
{
void _bind(py::module_& module);
} // namespace constants
