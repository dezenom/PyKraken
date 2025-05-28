#include "Draw.hpp"
#include "Color.hpp"
#include "Math.hpp"
#include "Rect.hpp"
#include "Window.hpp"

#include <SDL3/SDL.h>
#include <gfx/SDL3_gfxPrimitives.h>
#include <set>

static Uint64 packPoint(int x, int y)
{
    return (Uint64(uint32_t(x + 32768)) << 32) | uint32_t(y + 32768);
}

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
            if (radius < 1)
                return;

            if (py::isinstance<math::Vec2>(centerparam) && py::isinstance<Color>(color))
            {
                thickness == 1
                    ? circleThin(centerparam.cast<math::Vec2>(), radius, color.cast<Color>())
                    : circle(centerparam.cast<math::Vec2>(), radius, color.cast<Color>(),
                             thickness);
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

            thickness == 1 ? circleThin(c, radius, col) : circle(c, radius, col, thickness);
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
    if (thickness < 1)
        return;

    SDL_Renderer* r = window::getRenderer();
    if (r == nullptr)
        throw std::runtime_error("Renderer not initialized");

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

void circleThin(const math::Vec2& center, const int radius, const Color& color)
{
    SDL_Renderer* renderer = window::getRenderer();
    if (!renderer)
        throw std::runtime_error("Renderer not initialized");

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    int f = 1 - radius;
    int ddF_x = 0;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    std::set<Uint64> pointSet;

    auto emit = [&](int dx, int dy)
    { pointSet.insert(packPoint(int(center.x + dx), int(center.y + dy))); };

    while (x <= y)
    {
        emit(x, y);
        emit(-x, y);
        emit(x, -y);
        emit(-x, -y);
        emit(y, x);
        emit(-y, x);
        emit(y, -x);
        emit(-y, -x);

        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x + 1;
    }

    // Convert to SDL_FPoint vector
    std::vector<SDL_FPoint> points;
    points.reserve(pointSet.size());
    for (auto packed : pointSet)
    {
        int x = int(int32_t(packed >> 32)) - 32768;
        int y = int(int32_t(packed & 0xFFFFFFFF)) - 32768;
        points.push_back(SDL_FPoint{float(x), float(y)});
    }

    SDL_RenderPoints(renderer, points.data(), static_cast<int>(points.size()));
}

void circle(const math::Vec2& center, int radius, const Color& color, int thickness)
{
    SDL_Renderer* renderer = window::getRenderer();
    if (!renderer)
        throw std::runtime_error("Renderer not initialized");

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    const int innerRadius = (thickness <= 0 || thickness >= radius) ? -1 : radius - thickness;

    auto hline = [&](int x1, int y, int x2)
    {
        SDL_RenderLine(renderer, static_cast<float>(x1), static_cast<float>(y),
                       static_cast<float>(x2), static_cast<float>(y));
    };

    auto drawCircleSpan = [&](int r, std::unordered_map<int, std::pair<int, int>>& bounds)
    {
        int x = 0;
        int y = r;
        int d = 3 - 2 * r;

        while (x <= y)
        {
            auto update = [&](int y_offset, int xval)
            {
                int yPos = static_cast<int>(center.y) + y_offset;
                int minX = static_cast<int>(center.x) - xval;
                int maxX = static_cast<int>(center.x) + xval;
                bounds[yPos].first = std::min(bounds[yPos].first, minX);
                bounds[yPos].second = std::max(bounds[yPos].second, maxX);
            };

            for (int sign : {-1, 1})
            {
                update(sign * y, x);
                update(sign * x, y);
            }

            if (d < 0)
                d += 4 * x + 6;
            else
            {
                d += 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    };

    std::unordered_map<int, std::pair<int, int>> outerBounds;
    std::unordered_map<int, std::pair<int, int>> innerBounds;

    // Initialize bounds with max/mins
    for (int i = -radius; i <= radius; ++i)
    {
        outerBounds[static_cast<int>(center.y) + i] = {INT_MAX, INT_MIN};
        innerBounds[static_cast<int>(center.y) + i] = {INT_MAX, INT_MIN};
    }

    drawCircleSpan(radius, outerBounds);
    drawCircleSpan(innerRadius, innerBounds);

    for (const auto& [y, outer] : outerBounds)
    {
        const auto& inner = innerBounds[y];

        if (inner.first == INT_MAX || inner.second == INT_MIN)
        {
            // No inner circle on this line → full span
            hline(outer.first, y, outer.second);
        }
        else
        {
            // Left ring
            if (outer.first < inner.first)
                hline(outer.first, y, inner.first - 1);
            // Right ring
            if (outer.second > inner.second)
                hline(inner.second + 1, y, outer.second);
        }
    }
}

} // namespace draw
