#include "Camera.hpp"

math::Vec2 cameraPos;

namespace camera
{
void _bind(py::module_& module)
{
    py::class_<Camera>(module, "Camera")
        .def(py::init(), "Default constructor for Camera")
        .def(py::init([](const double x, const double y) { return Camera({x, y}); }),
             "Constructor for Camera with position specified by x and y coordinates", py::arg("x"),
             py::arg("y"))
        .def(py::init(
                 [](const py::object& pos)
                 {
                     if (py::isinstance<math::Vec2>(pos))
                         return Camera(py::cast<math::Vec2>(pos));

                     if (py::isinstance<py::sequence>(pos))
                     {
                         const auto posSeq = py::cast<py::sequence>(pos);
                         if (posSeq.size() == 2)
                             return Camera({posSeq[0].cast<double>(), posSeq[1].cast<double>()});
                         throw std::invalid_argument(
                             "Position sequence must have exactly two elements.");
                     }

                     throw std::invalid_argument(
                         "Position must be a Vec2 or a sequence of two numbers.");
                 }),
             "Constructor for Camera with position as a sequence of two numbers")
        .def_property("pos", &Camera::getPos, &Camera::setPos, "The camera position as a Vec2")
        .def("set", &Camera::set, "Set this camera as the currently active one for rendering");
}

Camera* Camera::active = nullptr;

Camera::Camera(const math::Vec2& pos) : pos(pos) {}

void Camera::setPos(const math::Vec2& pos)
{
    this->pos = pos;
    if (Camera::active == this)
        cameraPos = pos;
}

math::Vec2 Camera::getPos() const { return pos; }

void Camera::set()
{
    Camera::active = this;
    cameraPos = pos;
}
} // namespace camera
