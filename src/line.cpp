#include "Line.hpp"
#include "Math.hpp"

#include <cmath>

namespace line
{
void _bind(py::module_& module)
{
    py::classh<Line>(module, "Line", R"doc(
A 2D line segment defined by two points: A and B.
You can access or modify points using `.a`, `.b`, or directly via `.ax`, `.ay`, `.bx`, `.by`.
    )doc")
        .def(py::init(), R"doc(
Create a default line with all values set to 0.
    )doc")
        .def(py::init<double, double, double, double>(), py::arg("ax"), py::arg("ay"),
             py::arg("bx"), py::arg("by"), R"doc(
Create a line from two coordinate points.

Args:
    ax (float): X-coordinate of point A.
    ay (float): Y-coordinate of point A.
    bx (float): X-coordinate of point B.
    by (float): Y-coordinate of point B.
         )doc")
        .def(py::init<double, double, const Vec2&>(), py::arg("ax"), py::arg("ay"), py::arg("b"),
             R"doc(
Create a line from A coordinates and a Vec2 B point.

Args:
    ax (float): X-coordinate of point A.
    ay (float): Y-coordinate of point A.
    b (Vec2): Point B.
         )doc")
        .def(py::init<const Vec2&, double, double>(), py::arg("a"), py::arg("bx"), py::arg("by"),
             R"doc(
Create a line from a Vec2 A point and B coordinates.

Args:
    a (Vec2): Point A.
    bx (float): X-coordinate of point B.
    by (float): Y-coordinate of point B.
         )doc")
        .def(py::init<const Vec2&, const Vec2&>(), py::arg("a"), py::arg("b"), R"doc(
Create a line from two Vec2 points.

Args:
    a (Vec2): Point A.
    b (Vec2): Point B.
         )doc")
        .def(py::init(
                 [](const py::sequence& abSeq) -> Line
                 {
                     if (abSeq.size() != 2)
                         throw std::invalid_argument("Line expects two 2D points");

                     if (!py::isinstance<py::sequence>(abSeq[0]))
                         throw std::invalid_argument("A point must be a sequence");
                     if (!py::isinstance<py::sequence>(abSeq[1]))
                         throw std::invalid_argument("B point must be a sequence");

                     py::sequence aSeq = abSeq[0].cast<py::sequence>();
                     py::sequence bSeq = abSeq[1].cast<py::sequence>();

                     if (aSeq.size() != 2)
                         throw std::invalid_argument("A point must be a 2-element sequence");
                     if (bSeq.size() != 2)
                         throw std::invalid_argument("B point must be a 2-element sequence");

                     return {aSeq[0].cast<double>(), aSeq[1].cast<double>(), bSeq[0].cast<double>(),
                             bSeq[1].cast<double>()};
                 }),
             R"doc(
Create a line from two 2-element sequences: [[ax, ay], [bx, by]].

Raises:
    ValueError: If either point is not a 2-element sequence.
         )doc")

        .def(
            "__iter__", [](const Line& self) -> py::iterator
            { return py::make_iterator(&self.ax, &self.ax + 4); }, py::keep_alive<0, 1>())
        .def(
            "__getitem__",
            [](const Line& self, size_t i) -> double
            {
                switch (i)
                {
                case 0:
                    return self.ax;
                case 1:
                    return self.ay;
                case 2:
                    return self.bx;
                case 3:
                    return self.by;
                default:
                    throw py::index_error("Index out of range");
                }
            },
            R"doc(
Get coordinate by index:
    0 = ax, 1 = ay, 2 = bx, 3 = by

Raises:
    IndexError: If index is not 0-3.
         )doc")
        .def(
            "__len__", [](const Line&) -> int { return 4; }, R"doc(
Return the number of components (always 4).

Returns:
    int: Always returns 4 (ax, ay, bx, by).
        )doc")
        .def("__eq__", &Line::operator==, py::arg("other"), R"doc(
Check if two lines are equal.

Args:
    other (Line): The other line to compare.

Returns:
    bool: True if all components are equal.
        )doc")
        .def("__ne__", &Line::operator!=, py::arg("other"), R"doc(
Check if two lines are not equal.

Args:
    other (Line): The other line to compare.

Returns:
    bool: True if any component differs.
        )doc")

        .def_property("a", &Line::getA, &Line::setA, R"doc(
Get or set point A as a tuple or Vec2.
        )doc")
        .def_property("b", &Line::getB, &Line::setB, R"doc(
Get or set point B as a tuple or Vec2.
        )doc")

        .def_readwrite("ax", &Line::ax, "X-coordinate of point A.")
        .def_readwrite("ay", &Line::ay, "Y-coordinate of point A.")
        .def_readwrite("bx", &Line::bx, "X-coordinate of point B.")
        .def_readwrite("by", &Line::by, "Y-coordinate of point B.")

        .def_property_readonly("length", &Line::getLength, R"doc(
The Euclidean length of the line segment.
        )doc")

        .def("copy", &Line::copy, R"doc(
Return a copy of this line.
        )doc")
        .def("move", &Line::move, py::arg("offset"), R"doc(
Move this line by a Vec2 or 2-element sequence.

Args:
    offset (Vec2 | list[float]): The amount to move.
        )doc");
    py::implicitly_convertible<py::sequence, Line>();

    auto subLine = module.def_submodule("line");

    subLine.def("move", &move, py::arg("line"), py::arg("offset"), R"doc(
Move the given line by a Vec2 or 2-element sequence.

Args:
    offset (Vec2 | list[float]): The amount to move.
        )doc");
}

Line move(const Line& line, const Vec2& offset)
{
    return {line.ax + offset.x, line.ay + offset.y, line.bx + offset.x, line.by + offset.y};
}
} // namespace line

Line::Line() : ax(0.0), ay(0.0), bx(0.0), by(0.0) {}

Line::Line(double ax, double ay, double bx, double by) : ax(ax), ay(ay), bx(bx), by(by) {}

Line::Line(double ax, double ay, const Vec2& b) : ax(ax), ay(ay), bx(b.x), by(b.y) {}

Line::Line(const Vec2& a, double bx, double by) : ax(a.x), ay(a.y), bx(bx), by(by) {}

Line::Line(const Vec2& a, const Vec2& b) : ax(a.x), ay(a.y), bx(b.x), by(b.y) {}

double Line::getLength() const
{
    const double dx = bx - ax;
    const double dy = by - ay;
    return sqrt(dx * dx + dy * dy);
}

Vec2 Line::getA() const { return {ax, ay}; }

void Line::setA(const Vec2& pos)
{
    ax = pos.x;
    ay = pos.y;
}

Vec2 Line::getB() const { return {bx, by}; }

void Line::setB(const Vec2& pos)
{
    bx = pos.x;
    by = pos.y;
}

void Line::move(const Vec2& offset)
{
    ax += offset.x;
    ay += offset.y;
    bx += offset.x;
    by += offset.y;
}

Line Line::copy() const { return {ax, ay, bx, by}; }

bool Line::operator==(const Line& other) const
{
    return ax == other.ax && ay == other.ay && bx == other.bx && by == other.by;
}

bool Line::operator!=(const Line& other) const { return !(*this == other); }
