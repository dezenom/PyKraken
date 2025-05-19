#pragma once
#include <pybind11/pybind11.h>

namespace py = pybind11;

struct SDL_Window;

void init();

void quit();

namespace window
{
void _bind(py::module_& module);

SDL_Window* get();

void create(const py::tuple& resolution, const std::string& title, bool scaled);

bool isOpen();

void close();

void clear(py::tuple color);

void flip();

float getScale();

void setFullscreen(bool fullscreen);

bool isFullscreen();
} // namespace window
