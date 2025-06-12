#include "Draw.hpp"
#include "Camera.hpp"
#include "Circle.hpp"
#include "Color.hpp"
#include "Line.hpp"
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
        [](const py::object& rectparam, const py::object& colorparam, const int thickness)
        {
            Rect r;
            Color c;

            if (py::isinstance<Rect>(rectparam))
            {
                r = rectparam.cast<Rect>();
            }
            else if (py::isinstance<py::sequence>(rectparam))
            {
                const py::sequence rectSeq = rectparam.cast<py::sequence>();
                if (rectSeq.size() != 4)
                    throw std::invalid_argument("Rect must be (x, y, width, height)");
                r = {rectSeq[0].cast<double>(), rectSeq[1].cast<double>(),
                     rectSeq[2].cast<double>(), rectSeq[3].cast<double>()};
            }
            else
            {
                throw std::invalid_argument(
                    "Invalid rect type. Must be Rect or sequence of 4 numbers.");
            }

            if (py::isinstance<Color>(colorparam))
            {
                c = colorparam.cast<Color>();
            }
            else if (py::isinstance<py::sequence>(colorparam))
            {
                const py::sequence colorSeq = colorparam.cast<py::sequence>();
                if (colorSeq.size() != 3 && colorSeq.size() != 4)
                    throw std::invalid_argument("Color must be (r, g, b) or (r, g, b, a)");
                c = {colorSeq[0].cast<uint8_t>(), colorSeq[1].cast<uint8_t>(),
                     colorSeq[2].cast<uint8_t>(),
                     colorSeq.size() == 4 ? colorSeq[3].cast<uint8_t>()
                                          : static_cast<uint8_t>(255)};
            }
            else
            {
                throw std::invalid_argument(
                    "Invalid color type. Must be Color or sequence of 3-4 numbers.");
            }

            rect(r, c, thickness);
        },
        py::arg("rect"), py::arg("color"), py::arg("thickness") = 0);

    subDraw.def(
        "line",
        [](const py::object& start, const py::object& end, const py::object& color, int thickness)
        {
            if (thickness <= 0)
                return;

            math::Vec2 s, e;
            Color c;

            // Parse start
            if (py::isinstance<math::Vec2>(start))
                s = start.cast<math::Vec2>();
            else if (py::isinstance<py::sequence>(start) && start.cast<py::sequence>().size() == 2)
                s = {start.cast<py::sequence>()[0].cast<double>(),
                     start.cast<py::sequence>()[1].cast<double>()};
            else
                throw std::invalid_argument("Start must be a Vec2 or a sequence of two floats");

            // Parse end
            if (py::isinstance<math::Vec2>(end))
                e = end.cast<math::Vec2>();
            else if (py::isinstance<py::sequence>(end) && end.cast<py::sequence>().size() == 2)
                e = {end.cast<py::sequence>()[0].cast<double>(),
                     end.cast<py::sequence>()[1].cast<double>()};
            else
                throw std::invalid_argument("End must be a Vec2 or a sequence of two floats");

            // Parse color
            if (py::isinstance<Color>(color))
                c = color.cast<Color>();
            else if (py::isinstance<py::sequence>(color))
            {
                auto seq = color.cast<py::sequence>();
                if (seq.size() != 3 && seq.size() != 4)
                    throw std::invalid_argument(
                        "Color must be a sequence of (r, g, b) or (r, g, b, a)");
                c = {seq[0].cast<uint8_t>(), seq[1].cast<uint8_t>(), seq[2].cast<uint8_t>(),
                     seq.size() == 4 ? seq[3].cast<uint8_t>() : static_cast<uint8_t>(255)};
            }
            else
                throw std::invalid_argument(
                    "Color must be a Color or a sequence of 3 or 4 integers");

            if (thickness == 1)
                lineThin(s, e, c);
            else
                line(s, e, c, thickness);
        },
        py::arg("start"), py::arg("end"), py::arg("color"), py::arg("thickness") = 1);

    subDraw.def(
        "line",
        [](const Line& lineObj, const py::object& color, const int thickness)
        {
            if (thickness <= 0)
                return;

            Color c;

            // Parse color
            if (py::isinstance<Color>(color))
                c = color.cast<Color>();
            else if (py::isinstance<py::sequence>(color))
            {
                auto seq = color.cast<py::sequence>();
                if (seq.size() != 3 && seq.size() != 4)
                    throw std::invalid_argument(
                        "Color must be a sequence of (r, g, b) or (r, g, b, a)");
                c = {seq[0].cast<uint8_t>(), seq[1].cast<uint8_t>(), seq[2].cast<uint8_t>(),
                     seq.size() == 4 ? seq[3].cast<uint8_t>() : static_cast<uint8_t>(255)};
            }
            else
                throw std::invalid_argument(
                    "Color must be a Color or a sequence of 3 or 4 integers");

            if (thickness == 1)
                lineThin(lineObj.getA(), lineObj.getB(), c);
            else
                line(lineObj.getA(), lineObj.getB(), c, thickness);
        },
        py::arg("line"), py::arg("color"), py::arg("thickness") = 1);

    subDraw.def(
        "point",
        [](const py::object& pointparam, const py::object& color)
        {
            math::Vec2 p;
            Color c;

            // Parse point
            if (py::isinstance<math::Vec2>(pointparam))
                p = pointparam.cast<math::Vec2>();
            else if (py::isinstance<py::sequence>(pointparam) &&
                     pointparam.cast<py::sequence>().size() == 2)
            {
                auto seq = pointparam.cast<py::sequence>();
                p = {seq[0].cast<double>(), seq[1].cast<double>()};
            }
            else
                throw std::invalid_argument("Point must be a Vec2 or a sequence of two floats");

            // Parse color
            if (py::isinstance<Color>(color))
                c = color.cast<Color>();
            else if (py::isinstance<py::sequence>(color))
            {
                auto seq = color.cast<py::sequence>();
                if (seq.size() != 3 && seq.size() != 4)
                    throw std::invalid_argument(
                        "Color must be a sequence of (r, g, b) or (r, g, b, a)");
                c = {seq[0].cast<uint8_t>(), seq[1].cast<uint8_t>(), seq[2].cast<uint8_t>(),
                     seq.size() == 4 ? seq[3].cast<uint8_t>() : static_cast<uint8_t>(255)};
            }
            else
                throw std::invalid_argument(
                    "Color must be a Color or a sequence of 3 or 4 integers");

            point(p, c);
        },
        py::arg("point"), py::arg("color"));

    subDraw.def(
        "circle",
        [](const py::object& centerparam, int radius, const py::object& color, int thickness)
        {
            if (radius < 1)
                return;

            math::Vec2 center;
            Color col;

            // Parse center
            if (py::isinstance<math::Vec2>(centerparam))
                center = centerparam.cast<math::Vec2>();
            else if (py::isinstance<py::sequence>(centerparam) &&
                     centerparam.cast<py::sequence>().size() == 2)
            {
                auto seq = centerparam.cast<py::sequence>();
                center = {seq[0].cast<double>(), seq[1].cast<double>()};
            }
            else
                throw std::invalid_argument("Center must be a Vec2 or a sequence of two floats");

            // Parse color
            if (py::isinstance<Color>(color))
                col = color.cast<Color>();
            else if (py::isinstance<py::sequence>(color))
            {
                auto seq = color.cast<py::sequence>();
                if (seq.size() != 3 && seq.size() != 4)
                    throw std::invalid_argument(
                        "Color must be a sequence of (r, g, b) or (r, g, b, a)");
                col = {seq[0].cast<uint8_t>(), seq[1].cast<uint8_t>(), seq[2].cast<uint8_t>(),
                       seq.size() == 4 ? seq[3].cast<uint8_t>() : static_cast<uint8_t>(255)};
            }
            else
                throw std::invalid_argument(
                    "Color must be a Color or a sequence of 3 or 4 integers");

            if (thickness == 1)
                circleThin(center, radius, col);
            else
                circle(center, radius, col, thickness);
        },
        py::arg("center"), py::arg("radius"), py::arg("color"), py::arg("thickness") = 0);

    subDraw.def(
        "circle",
        [](const Circle& circleObj, const py::object& color, int thickness)
        {
            if (circleObj.radius < 1)
                return;

            Color col;

            // Parse color
            if (py::isinstance<Color>(color))
                col = color.cast<Color>();
            else if (py::isinstance<py::sequence>(color))
            {
                auto seq = color.cast<py::sequence>();
                if (seq.size() != 3 && seq.size() != 4)
                    throw std::invalid_argument(
                        "Color must be a sequence of (r, g, b) or (r, g, b, a)");
                col = {seq[0].cast<uint8_t>(), seq[1].cast<uint8_t>(), seq[2].cast<uint8_t>(),
                       seq.size() == 4 ? seq[3].cast<uint8_t>() : static_cast<uint8_t>(255)};
            }
            else
                throw std::invalid_argument(
                    "Color must be a Color or a sequence of 3 or 4 integers");

            if (thickness == 1)
                circleThin(circleObj.pos, circleObj.radius, col);
            else
                circle(circleObj.pos, circleObj.radius, col, thickness);
        },
        py::arg("circle"), py::arg("color"), py::arg("thickness") = 0);
}

void rect(const Rect& rect, const Color& color, const int thickness)
{
    SDL_Renderer* renderer = window::getRenderer();
    if (renderer == nullptr)
        throw std::runtime_error("Renderer not initialized");

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    Rect screenRect = rect;
    screenRect.setTopLeft(screenRect.getTopLeft() - camera::getActivePos());
    SDL_FRect sdlRect = screenRect;

    const auto halfWidth = static_cast<int>(rect.w / 2.0);
    const auto halfHeight = static_cast<int>(rect.h / 2.0);
    if (thickness <= 0 || thickness > halfWidth || thickness > halfHeight)
    {
        SDL_RenderFillRect(renderer, &sdlRect);
        return;
    }

    SDL_RenderRect(renderer, &sdlRect);
    for (int i = 1; i < thickness; i++)
    {
        screenRect.inflate({-2, -2});
        sdlRect = screenRect;
        SDL_RenderRect(renderer, &sdlRect);
    }
}

void lineThin(math::Vec2 start, math::Vec2 end, const Color& color)
{
    SDL_Renderer* renderer = window::getRenderer();
    if (renderer == nullptr)
        throw std::runtime_error("Renderer not initialized");

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    start -= camera::getActivePos();
    end -= camera::getActivePos();
    auto x1 = static_cast<int>(start.x);
    auto y1 = static_cast<int>(start.y);
    const auto x2 = static_cast<int>(end.x);
    const auto y2 = static_cast<int>(end.y);

    if (y1 == y2 || x1 == x2)
    {
        SDL_RenderLine(renderer, x1, y1, x2, y2);
        return;
    }

    const int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    const int dy = abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;

    while (true)
    {
        SDL_RenderPoint(renderer, x1, y1);

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = err;
        if (e2 > -dx)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dy)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void line(math::Vec2 start, math::Vec2 end, const Color& color, const int thickness)
{
    SDL_Renderer* renderer = window::getRenderer();
    if (renderer == nullptr)
        throw std::runtime_error("Renderer not initialized");

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    start -= camera::getActivePos();
    end -= camera::getActivePos();
    auto x1 = static_cast<int>(start.x);
    auto y1 = static_cast<int>(start.y);
    const auto x2 = static_cast<int>(end.x);
    const auto y2 = static_cast<int>(end.y);

    const int extraWidth = 1 - (thickness % 2);
    const int halfWidth = thickness / 2;

    if (y1 == y2)
    {
        for (int i = -halfWidth + extraWidth; i <= halfWidth; i++)
            SDL_RenderLine(renderer, x1, y1 + i, x2, y2 + i);
        return;
    }

    if (x1 == x2)
    {
        for (int i = -halfWidth + extraWidth; i <= halfWidth; i++)
            SDL_RenderLine(renderer, x1 + i, y1, x2 + i, y2);
        return;
    }

    const int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    const int dy = abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;
    const bool xinc = (dx <= dy); // Direction of thickness

    while (true)
    {
        if (xinc)
            // Thickness is horizontal, draw horizontal segment
            for (int i = -halfWidth + extraWidth; i <= halfWidth; i++)
                SDL_RenderPoint(renderer, x1 + i, y1);
        else
            // Thickness is vertical, draw vertical segment
            for (int i = -halfWidth + extraWidth; i <= halfWidth; i++)
                SDL_RenderPoint(renderer, x1, y1 + i);

        if (x1 == x2 && y1 == y2)
            break;

        const int e2 = err;
        if (e2 > -dx)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dy)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void point(math::Vec2 point, const Color& color)
{
    SDL_Renderer* r = window::getRenderer();
    if (r == nullptr)
        throw std::runtime_error("Renderer not initialized");

    SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);

    point -= camera::getActivePos();
    const auto px = static_cast<float>(point.x);
    const auto py = static_cast<float>(point.y);
    SDL_RenderPoint(r, px, py);
}

void circleThin(math::Vec2 center, const int radius, const Color& color)
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

    center -= camera::getActivePos();
    auto emit = [&](int dx, int dy) { pointSet.insert(packPoint(center.x + dx, center.y + dy)); };

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

void circle(math::Vec2 center, int radius, const Color& color, int thickness)
{
    SDL_Renderer* renderer = window::getRenderer();
    if (!renderer)
        throw std::runtime_error("Renderer not initialized");

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    const int innerRadius = (thickness <= 0 || thickness >= radius) ? -1 : radius - thickness;
    center -= camera::getActivePos();

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
