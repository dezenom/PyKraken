#include "Rect.hpp"

namespace rect
{
void _bind(py::module_& module)
{
    auto subRect = module.def_submodule("rect", "Rectangle related functions");

    py::class_<Rect>(subRect, "Rect")
        .def(py::init<>())
        .def(py::init<const py::sequence&, const py::sequence&>())
        .def(py::init<double, double, double, double>())
        .def(py::init<const py::sequence&, double, double>())
        .def(py::init<double, double, const py::sequence&>())
        .def_readwrite("x", &Rect::x)
        .def_readwrite("y", &Rect::y)
        .def_readwrite("w", &Rect::w)
        .def_readwrite("h", &Rect::h)
        .def_property("size", &Rect::getSize, &Rect::setSize)
        .def_property("left", &Rect::getLeft, &Rect::setLeft)
        .def_property("right", &Rect::getRight, &Rect::setRight)
        .def_property("top", &Rect::getTop, &Rect::setTop)
        .def_property("bottom", &Rect::getBottom, &Rect::setBottom)
        .def_property("top_left", &Rect::getTopLeft, &Rect::setTopLeft)
        .def_property("top_mid", &Rect::getTopMid, &Rect::setTopMid)
        .def_property("top_right", &Rect::getTopRight, &Rect::setTopRight)
        .def_property("mid_left", &Rect::getMidLeft, &Rect::setMidLeft)
        .def_property("center", &Rect::getCenter, &Rect::setCenter)
        .def_property("mid_right", &Rect::getMidRight, &Rect::setMidRight)
        .def_property("bottom_left", &Rect::getBottomLeft, &Rect::setBottomLeft)
        .def_property("bottom_mid", &Rect::getBottomMid, &Rect::setBottomMid)
        .def_property("bottom_right", &Rect::getBottomRight, &Rect::setBottomRight)
        .def("collide_point", &Rect::collidePoint)
        .def("collide_rect", &Rect::collideRect)
        .def("clamp", py::overload_cast<const py::sequence&, const py::sequence&>(&Rect::clamp))
        .def("clamp", py::overload_cast<const Rect&>(&Rect::clamp))
        .def("scale_by", py::overload_cast<double>(&Rect::scaleBy))
        .def("scale_by", py::overload_cast<const py::sequence&>(&Rect::scaleBy))
        .def("scale_to", &Rect::scaleTo)
        .def("__eq__", &Rect::operator==)
        .def("__ne__", &Rect::operator!=)
        .def("__bool__", [](const Rect& rect) { return rect.w > 0 && rect.h > 0; })
        .def("__str__",
             [](const Rect& rect)
             {
                 return "[" + std::to_string(rect.x) + ", " + std::to_string(rect.y) + ", " +
                        std::to_string(rect.w) + ", " + std::to_string(rect.h) + "]";
             })
        .def("__repr__",
             [](const Rect& rect)
             {
                 return "Rect(x=" + std::to_string(rect.x) + ", y=" + std::to_string(rect.y) +
                        ", w=" + std::to_string(rect.w) + ", h=" + std::to_string(rect.h) + ")";
             })
        .def(
            "__iter__", [](const Rect& rect) { return py::make_iterator(&rect.x, &rect.x + 4); },
            py::keep_alive<0, 1>())
        .def("__len__", [](const Rect& rect) { return 4; })
        .def("__getitem__",
             [](const Rect& rect, const size_t i)
             {
                 switch (i)
                 {
                 case 0:
                     return rect.x;
                 case 1:
                     return rect.y;
                 case 2:
                     return rect.w;
                 case 3:
                     return rect.h;
                 default:
                     throw py::index_error("Index out of range");
                 }
             });
    module.attr("Rect") = subRect.attr("Rect");

    subRect.def("clamp", py::overload_cast<const Rect&, const py::sequence&, const py::sequence&>(
                             &rect::clamp));
    subRect.def("clamp", py::overload_cast<const Rect&, const Rect&>(&rect::clamp));
    subRect.def("scale_by", py::overload_cast<const Rect&, double>(&rect::scaleBy));
    subRect.def("scale_by", py::overload_cast<const Rect&, const py::sequence&>(&rect::scaleBy));
    subRect.def("scale_to", &rect::scaleTo);
}

Rect clamp(const Rect& rect, const py::sequence& min, const py::sequence& max)
{
    Rect result = rect;
    result.clamp(min, max);
    return result;
}

Rect clamp(const Rect& rect, const Rect& other)
{
    Rect result = rect;
    result.clamp(other);
    return result;
}

Rect scaleBy(const Rect& rect, const double scale)
{
    Rect result = rect;
    result.scaleBy(scale);
    return result;
}

Rect scaleBy(const Rect& rect, const py::sequence& scale)
{
    Rect result = rect;
    result.scaleBy(scale);
    return result;
}

Rect scaleTo(const Rect& rect, const py::sequence& size)
{
    Rect result = rect;
    result.scaleTo(size);
    return result;
}
} // namespace rect

Rect::Rect() : x(0), y(0), w(0), h(0) {}

Rect::Rect(const py::sequence& pos, const py::sequence& size)
{
    if (pos.size() != 2)
        throw std::invalid_argument("Position must be a sequence of length 2");
    if (size.size() != 2)
        throw std::invalid_argument("Size must be a sequence of length 2");

    x = pos[0].cast<double>();
    y = pos[1].cast<double>();
    w = size[0].cast<double>();
    h = size[1].cast<double>();
}

bool Rect::collidePoint(const py::sequence& point) const
{
    if (point.size() != 2)
        throw std::invalid_argument("Point must be a sequence of length 2");

    const auto px = point[0].cast<double>();
    const auto py = point[1].cast<double>();

    return px >= x && px <= x + w && py >= y && py <= y + h;
}

bool Rect::collideRect(const Rect& other) const
{
    return x < other.x + other.w && x + w > other.x && y < other.y + other.h && y + h > other.y;
}

void Rect::clamp(const py::sequence& min, const py::sequence& max)
{
    if (min.size() != 2)
        throw std::invalid_argument("Min must be a sequence of length 2");
    if (max.size() != 2)
        throw std::invalid_argument("Max must be a sequence of length 2");

    const auto minX = min[0].cast<double>();
    const auto minY = min[1].cast<double>();
    const auto maxX = max[0].cast<double>();
    const auto maxY = max[1].cast<double>();

    if (minX > maxX || minY > maxY)
        throw std::invalid_argument("Invalid min/max values: min must be less than max");

    if (w > maxX - minX || h > maxY - minY)
        throw std::invalid_argument("Rect size exceeds the given area");

    x = std::max(minX, std::min(x, maxX));
    y = std::max(minY, std::min(y, maxY));
    w = std::max(0.0, std::min(w, maxX - x));
    h = std::max(0.0, std::min(h, maxY - y));
}

void Rect::clamp(const Rect& other) { clamp(other.getTopLeft(), other.getBottomRight()); }

void Rect::scaleBy(const double scale)
{
    if (scale <= 0)
        throw std::invalid_argument("Scale must be greater than 0");

    w *= scale;
    h *= scale;
}

void Rect::scaleBy(const py::sequence& scale)
{
    if (scale.size() != 2)
        throw std::invalid_argument("Scale must be a sequence of length 2");

    const auto scaleX = scale[0].cast<double>();
    const auto scaleY = scale[1].cast<double>();

    if (scaleX <= 0 || scaleY <= 0)
        throw std::invalid_argument("Scale must be greater than 0");

    w *= scaleX;
    h *= scaleY;
}

void Rect::scaleTo(const py::sequence& size)
{
    if (size.size() != 2)
        throw std::invalid_argument("Size must be a sequence of length 2");

    const auto width = size[0].cast<double>();
    const auto height = size[1].cast<double>();

    if (width <= 0.0)
        throw std::invalid_argument("Width must be greater than 0");
    if (height <= 0.0)
        throw std::invalid_argument("Height must be greater than 0");

    w = width;
    h = height;
}

bool Rect::operator==(const Rect& other) const
{
    return x == other.x && y == other.y && w == other.w && h == other.h;
}

bool Rect::operator!=(const Rect& other) const { return !(*this == other); }

Rect::operator SDL_Rect() const
{
    return {static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h)};
}

Rect::operator SDL_FRect() const
{
    return {static_cast<float>(x), static_cast<float>(y), static_cast<float>(w),
            static_cast<float>(h)};
}

void Rect::setSize(const py::sequence& size)
{
    if (size.size() != 2)
        throw std::invalid_argument("Size must be a sequence of length 2");

    w = size[0].cast<double>();
    h = size[1].cast<double>();
}
void Rect::setLeft(const double x) { this->x = x; }
void Rect::setRight(const double x) { this->x = x - w; }
void Rect::setTop(const double y) { this->y = y; }
void Rect::setBottom(const double y) { this->y = y - h; }
void Rect::setTopLeft(const py::sequence& pos)
{
    if (pos.size() != 2)
        throw std::invalid_argument("Position must be a sequence of length 2");

    x = pos[0].cast<double>();
    y = pos[1].cast<double>();
}
void Rect::setTopMid(const py::sequence& pos)
{
    if (pos.size() != 2)
        throw std::invalid_argument("Position must be a sequence of length 2");

    x = pos[0].cast<double>() - w / 2.0;
    y = pos[1].cast<double>();
}
void Rect::setTopRight(const py::sequence& pos)
{
    if (pos.size() != 2)
        throw std::invalid_argument("Position must be a sequence of length 2");

    x = pos[0].cast<double>() - w;
    y = pos[1].cast<double>();
}
void Rect::setMidLeft(const py::sequence& pos)
{
    if (pos.size() != 2)
        throw std::invalid_argument("Position must be a sequence of length 2");

    x = pos[0].cast<double>();
    y = pos[1].cast<double>() - h / 2.0;
}
void Rect::setCenter(const py::sequence& pos)
{
    if (pos.size() != 2)
        throw std::invalid_argument("Position must be a sequence of length 2");

    x = pos[0].cast<double>() - w / 2.0;
    y = pos[1].cast<double>() - h / 2.0;
}
void Rect::setMidRight(const py::sequence& pos)
{
    if (pos.size() != 2)
        throw std::invalid_argument("Position must be a sequence of length 2");

    x = pos[0].cast<double>() - w;
    y = pos[1].cast<double>() - h / 2.0;
}
void Rect::setBottomLeft(const py::sequence& pos)
{
    if (pos.size() != 2)
        throw std::invalid_argument("Position must be a sequence of length 2");

    x = pos[0].cast<double>();
    y = pos[1].cast<double>() - h;
}
void Rect::setBottomMid(const py::sequence& pos)
{
    if (pos.size() != 2)
        throw std::invalid_argument("Position must be a sequence of length 2");

    x = pos[0].cast<double>() - w / 2.0;
    y = pos[1].cast<double>() - h;
}
void Rect::setBottomRight(const py::sequence& pos)
{
    if (pos.size() != 2)
        throw std::invalid_argument("Position must be a sequence of length 2");

    x = pos[0].cast<double>() - w;
    y = pos[1].cast<double>() - h;
}

py::tuple Rect::getSize() const { return py::make_tuple(w, h); }
double Rect::getLeft() const { return x; }
double Rect::getRight() const { return x + w; }
double Rect::getTop() const { return y; }
double Rect::getBottom() const { return y + h; }
py::tuple Rect::getTopLeft() const { return py::make_tuple(x, y); }
py::tuple Rect::getTopMid() const { return py::make_tuple(x + w / 2.0, y); }
py::tuple Rect::getTopRight() const { return py::make_tuple(x + w, y); }
py::tuple Rect::getMidLeft() const { return py::make_tuple(x, y + h / 2.0); }
py::tuple Rect::getCenter() const { return py::make_tuple(x + w / 2.0, y + h / 2.0); }
py::tuple Rect::getMidRight() const { return py::make_tuple(x + w, y + h / 2.0); }
py::tuple Rect::getBottomLeft() const { return py::make_tuple(x, y + h); }
py::tuple Rect::getBottomMid() const { return py::make_tuple(x + w / 2.0, y + h); }
py::tuple Rect::getBottomRight() const { return py::make_tuple(x + w, y + h); }
