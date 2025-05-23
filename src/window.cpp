#include "Window.hpp"

#include <SDL3/SDL.h>
#include <iostream>
#include <stdexcept>

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
    subWindow.def("clear", &window::clear, py::arg("color") = py::make_tuple(0, 0, 0),
                  "Clear the window with the specified color (r, g, b)");
    subWindow.def("flip", &window::flip, "Flip the render buffer");
    subWindow.def("get_scale", &window::getScale, "Get the current scale of the renderer");
    subWindow.def("set_fullscreen", &window::setFullscreen, py::arg("fullscreen"),
                  "Set the fullscreen mode of the window");
    subWindow.def("is_fullscreen", &window::isFullscreen,
                  "Check if the window is in fullscreen mode");
}

SDL_Window* get() { return _window; }

void create(const py::tuple& resolution, const std::string& title, const bool scaled)
{
    if (_window || _renderer)
        return;

    if (resolution.size() != 2)
        throw std::invalid_argument("Resolution tuple must be (width, height)");

    const auto resW = resolution[0].cast<int>();
    const auto resH = resolution[1].cast<int>();

    if (resW <= 0 || resH <= 0)
        throw std::invalid_argument("Resolution values must be greater than 0");

    int winW = resW;
    int winH = resH;
    if (scaled)
    {
        SDL_Rect usableBounds;
        if (!SDL_GetDisplayUsableBounds(SDL_GetPrimaryDisplay(), &usableBounds))
            throw std::runtime_error(SDL_GetError());

        const int scaleX = usableBounds.w / resW;
        const int scaleY = usableBounds.h / resH;
        const int scale = std::max(1, std::min(scaleX, scaleY));

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

void clear(py::tuple color)
{
    if (!_renderer)
        throw std::runtime_error("Renderer not initialized");

    if (color.size() != 3)
        throw std::invalid_argument("Color tuple must be (r, g, b)");

    int r = color[0].cast<int>();
    int g = color[1].cast<int>();
    int b = color[2].cast<int>();

    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
        throw std::invalid_argument("Color values must be between 0 and 255");

    SDL_SetRenderDrawColor(_renderer, r, g, b, 255);
    SDL_RenderClear(_renderer);
}

void flip() { SDL_RenderPresent(_renderer); }

float getScale()
{
    if (!_renderer)
        throw std::runtime_error("Renderer not initialized");

    int winScale, renScale;
    SDL_GetWindowSize(_window, &winScale, nullptr);
    SDL_GetRenderLogicalPresentation(_renderer, &renScale, nullptr, nullptr);

    return static_cast<float>(winScale) / static_cast<float>(renScale);
}

void setFullscreen(bool fullscreen)
{
    if (!_window)
        throw std::runtime_error("Window not initialized");

    SDL_SetWindowFullscreen(_window, fullscreen);
}

bool isFullscreen()
{
    if (!_window)
        throw std::runtime_error("Window not initialized");

    return (SDL_GetWindowFlags(_window) & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN;
}

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
    if (_renderer)
        SDL_DestroyRenderer(_renderer);
    if (_window)
        SDL_DestroyWindow(_window);

    _renderer = nullptr;
    _window = nullptr;

    SDL_Quit();
}