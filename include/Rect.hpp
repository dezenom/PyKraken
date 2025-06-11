#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

class Rect;

namespace math
{
class Vec2;
}

namespace rect
{
void _bind(py::module_& module);

Rect move(const Rect& rect, const math::Vec2& offset);
Rect clamp(const Rect& rect, const math::Vec2& min, const math::Vec2& max);
Rect clamp(const Rect& rect, const Rect& other);
Rect scaleBy(const Rect& rect, double factor);
Rect scaleBy(const Rect& rect, const math::Vec2& factor);
Rect scaleTo(const Rect& rect, const math::Vec2& size);
} // namespace rect

class Rect
{
  public:
    double x = 0;
    double y = 0;
    double w = 0;
    double h = 0;

    Rect() = default;
    Rect(const math::Vec2& pos, const math::Vec2& size);
    Rect(double x, double y, double w, double h);
    Rect(const math::Vec2& pos, double w, double h);
    Rect(double x, double y, const math::Vec2& size);

    Rect copy() const;

    void move(const math::Vec2& offset);

    void inflate(const math::Vec2& offset);

    void fit(const Rect& other);

    bool contains(const Rect& other) const;

    bool collidePoint(const math::Vec2& point) const;

    bool collideRect(const Rect& other) const;

    void clamp(const math::Vec2& min, const math::Vec2& max);

    void clamp(const Rect& other);

    void scaleBy(double factor);

    void scaleBy(const math::Vec2& factor);

    void scaleTo(const math::Vec2& size);

    bool operator==(const Rect& other) const;
    bool operator!=(const Rect& other) const;

    operator SDL_Rect() const;
    operator SDL_FRect() const;

    void setSize(const math::Vec2& size);
    void setLeft(double left);
    void setRight(double right);
    void setTop(double top);
    void setBottom(double bottom);
    void setTopLeft(const math::Vec2& topLeft);
    void setTopMid(const math::Vec2& topMid);
    void setTopRight(const math::Vec2& topRight);
    void setMidLeft(const math::Vec2& midLeft);
    void setCenter(const math::Vec2& mid);
    void setMidRight(const math::Vec2& midRight);
    void setBottomLeft(const math::Vec2& bottomLeft);
    void setBottomMid(const math::Vec2& bottomMid);
    void setBottomRight(const math::Vec2& bottomRight);

    math::Vec2 getSize() const;
    double getLeft() const;
    double getRight() const;
    double getTop() const;
    double getBottom() const;
    math::Vec2 getTopLeft() const;
    math::Vec2 getTopMid() const;
    math::Vec2 getTopRight() const;
    math::Vec2 getMidLeft() const;
    math::Vec2 getCenter() const;
    math::Vec2 getMidRight() const;
    math::Vec2 getBottomLeft() const;
    math::Vec2 getBottomMid() const;
    math::Vec2 getBottomRight() const;
};
