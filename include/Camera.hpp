#pragma once

#include <Math.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace camera
{
void _bind(py::module_& module);

math::Vec2 getActivePos();

class Camera
{
  public:
    Camera() = default;
    explicit Camera(const math::Vec2& pos);
    ~Camera() = default;

    math::Vec2 getPos() const;
    void setPos(const math::Vec2& pos);
    void set();

    static Camera* active;

  private:
    math::Vec2 pos;
};
} // namespace camera
