#include "Event.hpp"
#include "Window.hpp"
#include "_globals.hpp"

#include <SDL3/SDL.h>
#include <pybind11/stl.h>

namespace event
{
std::vector<knEvent> get()
{
    std::vector<knEvent> events;
    SDL_Event event;

    // Reset pressed/released maps
    std::fill(std::begin(g_scancodePressed), std::end(g_scancodePressed), false);
    std::fill(std::begin(g_scancodeReleased), std::end(g_scancodeReleased), false);
    g_keycodePressed.clear();
    g_keycodeReleased.clear();

    while (SDL_PollEvent(&event))
    {
        knEvent e(event.type);
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            window::close();
            break;
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
            // update pressed/released maps
            if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat)
            {
                g_scancodePressed[event.key.scancode] = true;
                g_keycodePressed[event.key.key] = true;
            }
            else if (event.type == SDL_EVENT_KEY_UP)
            {
                g_scancodeReleased[event.key.scancode] = true;
                g_keycodeReleased[event.key.key] = true;
            }
            // attach key & scan to the event
            e.data["key"] = py::cast(static_cast<KnKeycode>(event.key.key));
            e.data["scan"] = py::cast(event.key.scancode);
            break;
        default:
            break;
        }
        events.push_back(std::move(e));
    }

    return events;
}

py::object knEvent::getAttr(const std::string& name) const
{
    if (name == "type")
        return py::int_(type);
    if (data.contains(name))
        return data[name.c_str()];
    throw py::attribute_error("Attribute '" + name + "' not found");
}

void _bind(py::module_& module)
{
    auto subEvent = module.def_submodule("event", "Event related functions");

    subEvent.def("get", &get, "Get all window user events");

    py::class_<knEvent>(subEvent, "Event")
        .def_readonly("type", &knEvent::type)
        .def("__getattr__", &knEvent::getAttr);
    module.attr("Event") = subEvent.attr("Event");
}
} // namespace event