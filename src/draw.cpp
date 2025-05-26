#include "Draw.hpp"
#include "Color.hpp"
#include "Math.hpp"
#include "Rect.hpp"
#include "Window.hpp"

#include <SDL3/SDL.h>
#include <gfx/SDL3_gfxPrimitives.h>

#define P(dx, dy) {float(center.x + dx), float(center.y + dy)}

namespace draw
{
void _bind(pybind11::module_& module)
{
    auto subDraw = module.def_submodule("draw", "Drawing related functions");

    subDraw.def(
        "rect",
        [](const py::sequence& rectparam, const py::sequence& color, const int thickness)
        {
            if (py::isinstance<Rect>(rectparam) && py::isinstance<Color>(color))
            {
                rect(rectparam.cast<Rect>(), color.cast<Color>(), thickness);
                return;
            }

            if (rectparam.size() != 4)
                throw std::invalid_argument("Rect must be a sequence of (x, y, width, height)");

            if (color.size() != 3 && color.size() != 4)
                throw std::invalid_argument(
                    "Color must be a sequence of (r, g, b) or (r, g, b, a)");

            const Rect r = {rectparam[0].cast<double>(), rectparam[1].cast<double>(),
                            rectparam[2].cast<double>(), rectparam[3].cast<double>()};
            const Color c = {color[0].cast<uint8_t>(), color[1].cast<uint8_t>(),
                             color[2].cast<uint8_t>(),
                             color.size() == 4 ? color[3].cast<uint8_t>() : 255};

            rect(r, c, thickness);
        },
        py::arg("rect"), py::arg("color"), py::arg("thickness") = 0);
    subDraw.def(
        "line",
        [](const py::sequence& start, const py::sequence& end, const py::sequence& color,
           const int thickness)
        {
            if (py::isinstance<math::Vec2>(start) && py::isinstance<math::Vec2>(end) &&
                py::isinstance<Color>(color))
            {
                line(start.cast<math::Vec2>(), end.cast<math::Vec2>(), color.cast<Color>(),
                     thickness);
                return;
            }

            if (start.size() != 2)
                throw std::invalid_argument("Start point must be a sequence of (x, y)");

            if (end.size() != 2)
                throw std::invalid_argument("End point must be a sequence of (x, y)");

            if (color.size() != 3 && color.size() != 4)
                throw std::invalid_argument(
                    "Color must be a sequence of (r, g, b) or (r, g, b, a)");

            const math::Vec2 s = {start[0].cast<double>(), start[1].cast<double>()};
            const math::Vec2 e = {end[0].cast<double>(), end[1].cast<double>()};
            const Color c = {color[0].cast<uint8_t>(), color[1].cast<uint8_t>(),
                             color[2].cast<uint8_t>(),
                             color.size() == 4 ? color[3].cast<uint8_t>() : 255};

            line(s, e, c, thickness);
        },
        py::arg("start"), py::arg("end"), py::arg("color"), py::arg("thickness") = 1);
    subDraw.def("point",
                [](const py::sequence& pointparam, const py::sequence& color)
                {
                    if (py::isinstance<math::Vec2>(pointparam) && py::isinstance<Color>(color))
                    {
                        point(pointparam.cast<math::Vec2>(), color.cast<Color>());
                        return;
                    }

                    if (pointparam.size() != 2)
                        throw std::invalid_argument("Point must be a sequence of (x, y)");

                    if (color.size() != 3 && color.size() != 4)
                        throw std::invalid_argument(
                            "Color must be a sequence of (r, g, b) or (r, g, b, a)");

                    const math::Vec2 p = {pointparam[0].cast<double>(),
                                          pointparam[1].cast<double>()};
                    const Color c = {color[0].cast<uint8_t>(), color[1].cast<uint8_t>(),
                                     color[2].cast<uint8_t>(),
                                     color.size() == 4 ? color[3].cast<uint8_t>() : 255};

                    point(p, c);
                });
    subDraw.def(
        "circle",
        [](const py::sequence& centerparam, const int radius, const py::sequence& color,
           const int thickness)
        {
            if (py::isinstance<math::Vec2>(centerparam) && py::isinstance<Color>(color))
            {
                circle(centerparam.cast<math::Vec2>(), radius, color.cast<Color>(), thickness);
                return;
            }

            if (centerparam.size() != 2)
                throw std::invalid_argument("Center must be a sequence of (x, y)");

            if (color.size() != 3 && color.size() != 4)
                throw std::invalid_argument(
                    "Color must be a sequence of (r, g, b) or (r, g, b, a)");

            const math::Vec2 c = {centerparam[0].cast<double>(), centerparam[1].cast<double>()};
            const Color col = {color[0].cast<uint8_t>(), color[1].cast<uint8_t>(),
                               color[2].cast<uint8_t>(),
                               color.size() == 4 ? color[3].cast<uint8_t>() : 255};

            circle(c, radius, col, thickness);
        },
        py::arg("center"), py::arg("radius"), py::arg("color"), py::arg("thickness") = 1);
}

void rect(const Rect& rect, const Color& color, const int thickness)
{
    SDL_Renderer* r = window::getRenderer();
    if (r == nullptr)
    {
        throw std::runtime_error("Renderer not initialized");
    }

    SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);

    const auto halfWidth = static_cast<float>(rect.w / 2.f);
    const auto halfHeight = static_cast<float>(rect.h / 2.f);

    if (thickness <= 0 || thickness > halfWidth || thickness > halfHeight)
    {
        SDL_FRect frect = rect;
        SDL_RenderFillRect(window::getRenderer(), &frect);
        return;
    }

    for (int i = 0; i < thickness; i++)
    {
        const auto offset = static_cast<float>(i);
        Rect layer = {rect.x + offset, rect.y + offset, rect.w - offset * 2, rect.h - offset * 2};
        SDL_FRect sdllayer = layer;
        SDL_RenderRect(window::getRenderer(), &sdllayer);
    }
}

void line(const math::Vec2& start, const math::Vec2& end, const Color& color, const int thickness)
{
    SDL_Renderer* r = window::getRenderer();
    if (r == nullptr)
    {
        throw std::runtime_error("Renderer not initialized");
    }

    const auto x1 = static_cast<Sint16>(start.x);
    const auto y1 = static_cast<Sint16>(start.y);
    const auto x2 = static_cast<Sint16>(end.x);
    const auto y2 = static_cast<Sint16>(end.y);

    thickLineRGBA(r, x1, y1, x2, y2, thickness, color.r, color.g, color.b, color.a);
}

void point(const math::Vec2& point, const Color& color)
{
    SDL_Renderer* r = window::getRenderer();
    if (r == nullptr)
    {
        throw std::runtime_error("Renderer not initialized");
    }

    SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);

    const auto px = static_cast<float>(point.x);
    const auto py = static_cast<float>(point.y);
    SDL_RenderPoint(r, px, py);
}

void circle(const math::Vec2& center, int radius, const Color& color, const int thickness)
{
    SDL_Renderer* r = window::getRenderer();
    if (r == nullptr)
        throw std::runtime_error("Renderer not initialized");

    if (radius < 1)
        radius = 1;

    SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);

    int x = 0;
    int y = radius;
    int d = 3 - (radius << 1);
    const bool skipCorner = (radius == 4);
    int lastY = -1;

    const SDL_FPoint points[] = {
        P(x, y), P(x, -y), P(-x, y), P(-x, -y), P(y, x), P(y, -x), P(-y, x), P(-y, -x),
    };
    SDL_RenderPoints(r, points, 8);

    while (x <= y)
    {
        if (x == y && y == lastY)
        {
            x++;
            continue;
        }
        lastY = y;

        if (d & 0x80000000)
            d += (x << 2) + 6;
        else
        {
            d += ((x - y) << 2) + 10;
            y--;
        }
        x++;

        if (skipCorner && x == y)
            continue;

        const SDL_FPoint points[] = {
            P(x, y), P(x, -y), P(-x, y), P(-x, -y), P(y, x), P(y, -x), P(-y, x), P(-y, -x),
        };
        SDL_RenderPoints(r, points, 8);
    }
}

} // namespace draw
