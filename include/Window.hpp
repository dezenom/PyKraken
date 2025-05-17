#pragma once
#include <pybind11/pybind11.h>

namespace py = pybind11;

void init();

void quit();

namespace window
{
void _bind(py::module_& module);

void create(const py::tuple& resolution, const std::string& title = "Kraken Engine",
            bool scaled = false);

bool isOpen();

void close();

void clear();

void flip();
} // namespace window
