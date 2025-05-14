#include "Window.hpp"
#include <stdexcept>
#include <SDL3/SDL.h>

static SDL_Window *_window = nullptr;
static SDL_Renderer *_renderer = nullptr;
static bool _isOpen = false;

namespace window
{
    void _bind(pybind11::module_ &module)
    {
        auto subWindow = module.def_submodule("window", "Window related functions");
        subWindow.def("create", &window::create, "Create a window", py::arg("title"), py::arg("width"),
                      py::arg("height"));
        subWindow.def("is_open", &window::isOpen, "Check if the window is open");
        subWindow.def("close", &window::close, "Close the window");
        subWindow.def("clear", &window::clear, "Clear the screen");
        subWindow.def("flip", &window::flip, "Flip the render buffer");
    }

    void create(const std::string &title, int width, int height)
    {
        if (_window || _renderer)
            return;

        SDL_CreateWindowAndRenderer(title.c_str(), width, height, 0, &_window, &_renderer);
        if (!_window || !_renderer)
            throw std::runtime_error(SDL_GetError());

        _isOpen = true;
    }

    bool isOpen()
    {
        return _isOpen;
    }

    void close()
    {
        _isOpen = false;
    }

    void clear()
    {
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
        SDL_RenderClear(_renderer);
    }

    void flip()
    {
        SDL_RenderPresent(_renderer);
    }
}

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