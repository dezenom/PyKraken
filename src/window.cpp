#include "Window.hpp"
#include <SDL3/SDL.h>
#include <stdexcept>

#include <iostream>

static SDL_Window* _window = nullptr;
static SDL_Renderer* _renderer = nullptr;
static bool _isOpen = false;

namespace window
{
void _bind(pybind11::module_& module)
{
    auto subWindow = module.def_submodule("window", "Window related functions");
    subWindow.def("create", &window::create, py::arg("resolution"),
                  py::arg("title") = "Kraken Engine", py::arg("scaled") = false,
                  "Create a window with (width, height), optional title, and auto-scaling mode");
    subWindow.def("is_open", &window::isOpen, "Check if the window is open");
    subWindow.def("close", &window::close, "Close the window");
    subWindow.def("clear", &window::clear, "Clear the screen");
    subWindow.def("flip", &window::flip, "Flip the render buffer");
}

void create(const py::tuple& resolution, const std::string& title, const bool scaled)
{
    if (_window || _renderer)
        return;

    if (resolution.size() != 2)
        throw std::invalid_argument("Resolution tuple must be (width, height)");

    int resW = resolution[0].cast<int>();
    int resH = resolution[1].cast<int>();

    if (resW <= 0 || resH <= 0)
        throw std::invalid_argument("Resolution values must be greater than 0");

    int winW = resW;
    int winH = resH;

    if (scaled)
    {
        SDL_Rect usableBounds;
        if (!SDL_GetDisplayUsableBounds(SDL_GetPrimaryDisplay(), &usableBounds))
            throw std::runtime_error(SDL_GetError());

        int scaleX = usableBounds.w / resW;
        int scaleY = usableBounds.h / resH;
        int scale = std::max(1, std::min(scaleX, scaleY));

        winW = resW * scale;
        winH = resH * scale;
    }

    SDL_CreateWindowAndRenderer(title.c_str(), winW, winH, 0, &_window, &_renderer);
    if (!_window || !_renderer)
        throw std::runtime_error(SDL_GetError());

    if (scaled)
    {
        if (!SDL_SetRenderLogicalPresentation(_renderer, resW, resH,
                                              SDL_LOGICAL_PRESENTATION_LETTERBOX))
            throw std::runtime_error(SDL_GetError());
    }

    _isOpen = true;
}

bool isOpen() { return _isOpen; }

void close() { _isOpen = false; }

void clear()
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
}

void flip() { SDL_RenderPresent(_renderer); }
} // namespace window

void init()
{
    if (_window || _renderer)
        return;

    if (!SDL_Init(SDL_INIT_VIDEO))
        throw std::runtime_error(SDL_GetError());
}

void quit()
{
    if (_window)
        SDL_DestroyWindow(_window);
    if (_renderer)
        SDL_DestroyRenderer(_renderer);
    _window = nullptr;
    _renderer = nullptr;
    SDL_Quit();
}