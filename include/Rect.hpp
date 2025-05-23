#pragma once

#include "Math.hpp"
#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

class Rect;

namespace rect
{
void _bind(py::module_& module);

Rect clamp(const Rect& rect, const py::sequence& min, const py::sequence& max);
Rect clamp(const Rect& rect, const Rect& other);
Rect scaleBy(const Rect& rect, double scale);
Rect scaleBy(const Rect& rect, const py::sequence& scale);
Rect scaleTo(const Rect& rect, const py::sequence& size);
} // namespace rect

class Rect
{
  public:
    double x, y, w, h;

    Rect();

    Rect(const py::sequence& pos, const py::sequence& size);

    template <typename T>
    Rect(T x, T y, T w, T h)
        : x(static_cast<double>(x)), y(static_cast<double>(y)), w(static_cast<double>(w)),
          h(static_cast<double>(h))
    {
    }

    template <typename T>
    Rect(const py::sequence& pos, T w, T h) : w(static_cast<double>(w)), h(static_cast<double>(h))
    {
        if (pos.size() != 2)
            throw std::invalid_argument("Position must be a sequence of length 2");

        x = pos[0].cast<double>();
        y = pos[1].cast<double>();
    }

    template <typename T>
    Rect(T x, T y, const py::sequence& size) : x(static_cast<double>(x)), y(static_cast<double>(y))
    {
        if (size.size() != 2)
            throw std::invalid_argument("Size must be a sequence of length 2");

        w = size[0].cast<double>();
        h = size[1].cast<double>();
    }

    bool collidePoint(const py::sequence& point) const;

    bool collideRect(const Rect& other) const;

    void clamp(const py::sequence& min, const py::sequence& max);

    void clamp(const Rect& other);

    void scaleBy(double scale);

    void scaleBy(const py::sequence& scale);

    void scaleTo(const py::sequence& size);

    bool operator==(const Rect& other) const;
    bool operator!=(const Rect& other) const;

    operator SDL_Rect() const;
    operator SDL_FRect() const;

    void setSize(const py::sequence& size);
    void setLeft(double left);
    void setRight(double right);
    void setTop(double top);
    void setBottom(double bottom);
    void setTopLeft(const py::sequence& topLeft);
    void setTopMid(const py::sequence& topMid);
    void setTopRight(const py::sequence& topRight);
    void setMidLeft(const py::sequence& midLeft);
    void setCenter(const py::sequence& mid);
    void setMidRight(const py::sequence& midRight);
    void setBottomLeft(const py::sequence& bottomLeft);
    void setBottomMid(const py::sequence& bottomMid);
    void setBottomRight(const py::sequence& bottomRight);

    py::tuple getSize() const;
    double getLeft() const;
    double getRight() const;
    double getTop() const;
    double getBottom() const;
    py::tuple getTopLeft() const;
    py::tuple getTopMid() const;
    py::tuple getTopRight() const;
    py::tuple getMidLeft() const;
    py::tuple getCenter() const;
    py::tuple getMidRight() const;
    py::tuple getBottomLeft() const;
    py::tuple getBottomMid() const;
    py::tuple getBottomRight() const;
};
