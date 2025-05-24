#include "Color.hpp"

#include <sstream>

namespace color
{
void _bind(py::module_& module)
{
    py::class_<Color>(module, "Color")
        .def(py::init())
        .def(py::init<uint8_t, uint8_t, uint8_t, uint8_t>(), py::arg("r"), py::arg("g"),
             py::arg("b"), py::arg("a") = 255)
        .def(py::init([](const std::string& hex) { return fromHex(hex); }), py::arg("hex"))
        .def("__str__",
             [](const Color& c)
             {
                 return "(" + std::to_string(c.r) + ", " + std::to_string(c.g) + ", " +
                        std::to_string(c.b) + ", " + std::to_string(c.a) + ")";
             })
        .def("__repr__",
             [](const Color& c)
             {
                 return "Color(" + std::to_string(c.r) + ", " + std::to_string(c.g) + ", " +
                        std::to_string(c.b) + ", " + std::to_string(c.a) + ")";
             })
        .def(
            "__iter__", [](const Color& c) { return py::make_iterator(&c.r, &c.r + 4); },
            py::keep_alive<0, 1>())
        .def("__getitem__",
             [](const Color& c, size_t i)
             {
                 if (i >= 4)
                     throw py::index_error();
                 return *(&c.r + i);
             })
        .def("__len__", [](const Color&) { return 4; })
        .def("__setitem__",
             [](Color& c, size_t i, uint8_t value)
             {
                 if (i >= 4)
                     throw py::index_error();
                 *(&c.r + i) = value;
             })
        .def_readwrite("r", &Color::r)
        .def_readwrite("g", &Color::g)
        .def_readwrite("b", &Color::b)
        .def_readwrite("a", &Color::a);

    auto subColor = module.def_submodule("color", "Color related functions");

    subColor
        .def("from_hex", &fromHex, py::arg("hex"),
             "Create a Color from a hex string (e.g. '#FF5733' or 'FF5733').")
        .def("from_hsv", &fromHSV, py::arg("h"), py::arg("s"), py::arg("v"), py::arg("a") = 1.0f,
             "Create a Color from HSV values.")
        .def("lerp", &lerp, py::arg("a"), py::arg("b"), py::arg("t") = 0.5,
             "Linearly interpolate between two Colors.")
        .def("invert", &invert, py::arg("color"), "Invert the given Color.");
}

Color fromHex(std::string_view hex)
{
    if (hex.empty())
        return Color{0, 0, 0, 255};

    if (hex[0] == '#')
        hex.remove_prefix(1);

    auto hexToByte = [](std::string_view str) -> uint8_t
    {
        uint32_t byte;
        std::stringstream ss;
        ss << std::hex << str;
        ss >> byte;
        return static_cast<uint8_t>(byte);
    };

    if (hex.length() == 6)
    {
        // RRGGBB
        return {hexToByte(hex.substr(0, 2)), hexToByte(hex.substr(2, 2)),
                hexToByte(hex.substr(4, 2)), 255};
    }
    else if (hex.length() == 8)
    {
        // RRGGBBAA
        return {hexToByte(hex.substr(0, 2)), hexToByte(hex.substr(2, 2)),
                hexToByte(hex.substr(4, 2)), hexToByte(hex.substr(6, 2))};
    }
    else if (hex.length() == 3)
    {
        // RGB → duplicate each
        return {hexToByte(std::string(2, hex[0])), hexToByte(std::string(2, hex[1])),
                hexToByte(std::string(2, hex[2])), 255};
    }
    else if (hex.length() == 4)
    {
        // RGBA → duplicate each
        return {hexToByte(std::string(2, hex[0])), hexToByte(std::string(2, hex[1])),
                hexToByte(std::string(2, hex[2])), hexToByte(std::string(2, hex[3]))};
    }

    // Invalid format fallback
    return {0, 0, 0, 255};
}

Color fromHSV(const float h, const float s, const float v, const float a)
{
    const float c = v * s;
    const float x = c * (1 - std::fabs(fmod(h / 60.0f, 2) - 1));
    const float m = v - c;

    float r, g, b;

    if (h < 60)
    {
        r = c;
        g = x;
        b = 0;
    }
    else if (h < 120)
    {
        r = x;
        g = c;
        b = 0;
    }
    else if (h < 180)
    {
        r = 0;
        g = c;
        b = x;
    }
    else if (h < 240)
    {
        r = 0;
        g = x;
        b = c;
    }
    else if (h < 300)
    {
        r = x;
        g = 0;
        b = c;
    }
    else
    {
        r = c;
        g = 0;
        b = x;
    }

    return {static_cast<uint8_t>((r + m) * 255), static_cast<uint8_t>((g + m) * 255),
            static_cast<uint8_t>((b + m) * 255), static_cast<uint8_t>(a * 255)};
}

Color lerp(const Color& a, const Color& b, const double t)
{
    return {
        static_cast<uint8_t>(a.r + (b.r - a.r) * t), static_cast<uint8_t>(a.g + (b.g - a.g) * t),
        static_cast<uint8_t>(a.b + (b.b - a.b) * t), static_cast<uint8_t>(a.a + (b.a - a.a) * t)};
}

Color invert(const Color& color)
{
    return {static_cast<uint8_t>(255 - color.r), static_cast<uint8_t>(255 - color.g),
            static_cast<uint8_t>(255 - color.b), color.a};
}
} // namespace color
