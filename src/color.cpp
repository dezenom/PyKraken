#include "Color.hpp"

#include <iomanip>
#include <sstream>

namespace color
{
void _bind(py::module_& module)
{
    py::classh<Color>(module, "Color", R"doc(
Represents an RGBA color.

Each channel (r, g, b, a) is an 8-bit unsigned integer.
    )doc")

        .def(py::init(), R"doc(
Create a Color with default values (0, 0, 0, 255).
        )doc")

        .def(py::init<uint8_t, uint8_t, uint8_t, uint8_t>(), py::arg("r"), py::arg("g"),
             py::arg("b"), py::arg("a") = 255, R"doc(
Create a Color from RGBA components.

Args:
    r (int): Red value [0-255].
    g (int): Green value [0-255].
    b (int): Blue value [0-255].
    a (int, optional): Alpha value [0-255]. Defaults to 255.
        )doc")

        .def(py::init(
                 [](const py::object& objparam) -> Color
                 {
                     if (py::isinstance<py::str>(objparam))
                         return fromHex(objparam.cast<std::string>());

                     if (py::isinstance<py::sequence>(objparam))
                     {
                         auto seq = objparam.cast<py::sequence>();
                         if (seq.size() == 3 || seq.size() == 4)
                         {
                             Color c;
                             c.r = seq[0].cast<uint8_t>();
                             c.g = seq[1].cast<uint8_t>();
                             c.b = seq[2].cast<uint8_t>();
                             c.a = seq.size() == 4 ? seq[3].cast<uint8_t>() : 255;
                             return c;
                         }
                     }

                     throw std::invalid_argument(
                         "Argument must be a hex string or a sequence of 3-4 integers.");
                 }),
             R"doc(
Create a Color from a hex string or a sequence of RGB(A) integers.

Examples:
    Color("#ff00ff")
    Color([255, 0, 255])
    Color((255, 0, 255, 128))
        )doc")

        .def(
            "__str__",
            [](const Color& c) -> std::string
            {
                return "(" + std::to_string(c.r) + ", " + std::to_string(c.g) + ", " +
                       std::to_string(c.b) + ", " + std::to_string(c.a) + ")";
            },
            R"doc(
Return a human-readable string representation.

Returns:
    str: String in format "(r, g, b, a)" with integer values.
        )doc")

        .def(
            "__repr__",
            [](const Color& c) -> std::string
            {
                return "Color(" + std::to_string(c.r) + ", " + std::to_string(c.g) + ", " +
                       std::to_string(c.b) + ", " + std::to_string(c.a) + ")";
            },
            R"doc(
Return a string suitable for debugging and recreation.

Returns:
    str: String in format "Color(r, g, b, a)" that can recreate the object.
        )doc")

        .def(
            "__iter__", [](const Color& c) -> py::iterator
            { return py::make_iterator(&c.r, &c.r + 4); }, py::keep_alive<0, 1>(), R"doc(
Return an iterator over color channels.

Yields:
    int: The r, g, b, a values in that order (0-255 each).

Example:
    for channel in color:
        print(channel)  # Prints r, g, b, a values
        )doc")

        .def(
            "__getitem__",
            [](const Color& c, size_t i) -> int
            {
                if (i >= 4)
                    throw py::index_error();
                return *(&c.r + i);
            },
            py::arg("index"), R"doc(
Access color channels by index.

Args:
    index (int): Channel index (0=r, 1=g, 2=b, 3=a).

Returns:
    int: Channel value (0-255).

Raises:
    IndexError: If index is not in range [0, 3].
        )doc")

        .def(
            "__setitem__",
            [](Color& c, size_t i, uint8_t value) -> void
            {
                if (i >= 4)
                    throw py::index_error();
                *(&c.r + i) = value;
            },
            py::arg("index"), py::arg("value"), R"doc(
Set a color channel by index.

Args:
    index (int): Channel index (0=r, 1=g, 2=b, 3=a).
    value (int): New channel value (0-255).

Raises:
    IndexError: If index is not in range [0, 3].
        )doc")

        .def(
            "__len__", [](const Color&) -> int { return 4; }, R"doc(
Return the number of color channels.

Returns:
    int: Always returns 4 (for r, g, b, a channels).
        )doc")

        .def_readwrite("r", &Color::r, R"doc(
Red channel value.

Type: int
Range: 0-255 (8-bit unsigned integer)
        )doc")
        .def_readwrite("g", &Color::g, R"doc(
Green channel value.

Type: int
Range: 0-255 (8-bit unsigned integer)
        )doc")
        .def_readwrite("b", &Color::b, R"doc(
Blue channel value.

Type: int
Range: 0-255 (8-bit unsigned integer)
        )doc")
        .def_readwrite("a", &Color::a, R"doc(
Alpha (transparency) channel value.

Type: int
Range: 0-255 (8-bit unsigned integer)
Note: 0 = fully transparent, 255 = fully opaque
        )doc")

        .def_property("hex", &Color::toHex, &Color::fromHex, R"doc(
Get or set the color as a hex string.

When getting, returns an 8-digit hex string in the format "#RRGGBBAA".
When setting, accepts various hex formats (see from_hex for details).

Example:
    color.hex = "#FF00FF"     # Set to magenta
    print(color.hex)          # Returns "#FF00FFFF"
        )doc")
        .def_property("hsv", &Color::toHSV, &Color::fromHSV, R"doc(
Get or set the color as an HSV tuple.

When getting, returns a tuple of (hue, saturation, value, alpha).
When setting, accepts a tuple of 3 or 4 values.

Values:
    hue (float): Hue angle in degrees (0-360)
    saturation (float): Saturation level (0-1)
    value (float): Brightness/value level (0-1)
    alpha (float): Alpha transparency (0-1), optional

Example:
    color.hsv = (120, 1.0, 1.0)        # Pure green
    color.hsv = (240, 0.5, 0.8, 0.9)   # Light blue with transparency
    h, s, v, a = color.hsv              # Get HSV values
        )doc");

    py::implicitly_convertible<py::sequence, Color>();
    py::implicitly_convertible<py::str, Color>();

    auto subColor = module.def_submodule("color", R"doc(
Color utility functions and predefined color constants.

This module provides functions for color manipulation and conversion,
as well as commonly used color constants for convenience.
    )doc");

    subColor.def("from_hex", &fromHex, py::arg("hex"), R"doc(
Create a Color from a hex string.

Supports multiple hex formats:
- "#RRGGBB" - 6-digit hex with full opacity
- "#RRGGBBAA" - 8-digit hex with alpha
- "#RGB" - 3-digit hex (each digit duplicated)
- "#RGBA" - 4-digit hex with alpha (each digit duplicated)

Args:
    hex (str): Hex color string (with or without '#' prefix).

Returns:
    Color: New Color object from the hex string.

Examples:
    from_hex("#FF00FF")      # Magenta, full opacity
    from_hex("#FF00FF80")    # Magenta, 50% opacity
    from_hex("#F0F")         # Same as "#FF00FF"
    from_hex("RGB")          # Without '#' prefix
        )doc");

    subColor.def("from_hsv", &fromHSV, py::arg("h"), py::arg("s"), py::arg("v"), py::arg("a") = 1.0,
                 R"doc(
Create a Color from HSV(A) values.

Args:
    h (float): Hue angle (0-360).
    s (float): Saturation (0-1).
    v (float): Value/brightness (0-1).
    a (float, optional): Alpha (0-1). Defaults to 1.0.
        )doc");

    subColor.def("lerp", &lerp, py::arg("a"), py::arg("b"), py::arg("t"), R"doc(
Linearly interpolate between two colors.

Performs component-wise linear interpolation between start and end colors.
All RGBA channels are interpolated independently.

Args:
    a (Color): Start color (when t=0.0).
    b (Color): End color (when t=1.0).
    t (float): Blend factor. Values outside [0,1] will extrapolate.

Returns:
    Color: New interpolated color.

Examples:
    lerp(Color.RED, Color.BLUE, 0.5)    # Purple (halfway between red and blue)
    lerp(Color.BLACK, Color.WHITE, 0.25) # Dark gray
        )doc");

    subColor.def("invert", &invert, py::arg("color"), R"doc(
Return the inverse of a color by flipping RGB channels.

The alpha channel is preserved unchanged.

Args:
    color (Color): The color to invert.

Returns:
    Color: New Color with inverted RGB values (255 - original value).

Example:
    invert(Color(255, 0, 128, 200))  # Returns Color(0, 255, 127, 200)
        )doc");

    subColor.def("grayscale", &grayscale, py::arg("color"), R"doc(
Convert a color to grayscale.

Args:
    color (Color): The color to convert.

Returns:
    Color: New Color object representing the grayscale version.

Example:
    grayscale(Color(255, 0, 0))  # Returns Color(76, 76, 76, 255)
        )doc");

    subColor.attr("BLACK") = BLACK;
    subColor.attr("WHITE") = WHITE;
    subColor.attr("RED") = RED;
    subColor.attr("GREEN") = GREEN;
    subColor.attr("BLUE") = BLUE;
    subColor.attr("YELLOW") = YELLOW;
    subColor.attr("MAGENTA") = MAGENTA;
    subColor.attr("CYAN") = CYAN;
    subColor.attr("GRAY") = GRAY;
    subColor.attr("GREY") = GRAY;
    subColor.attr("DARK_GRAY") = DARK_GRAY;
    subColor.attr("DARK_GREY") = DARK_GRAY;
    subColor.attr("LIGHT_GRAY") = LIGHT_GRAY;
    subColor.attr("LIGHT_GREY") = LIGHT_GRAY;
    subColor.attr("ORANGE") = ORANGE;
    subColor.attr("BROWN") = BROWN;
    subColor.attr("PINK") = PINK;
    subColor.attr("PURPLE") = PURPLE;
    subColor.attr("NAVY") = NAVY;
    subColor.attr("TEAL") = TEAL;
    subColor.attr("OLIVE") = OLIVE;
    subColor.attr("MAROON") = MAROON;
}

Color fromHex(std::string_view hex)
{
    if (hex.empty())
        throw std::invalid_argument("Hex string cannot be empty");

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

    throw std::invalid_argument("Invalid hex string format");
}

Color fromHSV(const double h, const double s, const double v, const double a)
{
    if (s < 0 || s > 1 || v < 0 || v > 1 || a < 0 || a > 1)
        throw std::invalid_argument("Saturation, value, and alpha must be in the range [0, 1]");
    if (h < 0 || h >= 360)
        throw std::invalid_argument("Hue must be in the range [0, 360)");

    const double c = v * s;
    const double x = c * (1 - std::fabs(fmod(h / 60.0, 2.0) - 1));
    const double m = v - c;

    double r, g, b;

    if (h < 60.0)
    {
        r = c;
        g = x;
        b = 0.0;
    }
    else if (h < 120.0)
    {
        r = x;
        g = c;
        b = 0.0;
    }
    else if (h < 180.0)
    {
        r = 0.0;
        g = c;
        b = x;
    }
    else if (h < 240.0)
    {
        r = 0.0;
        g = x;
        b = c;
    }
    else if (h < 300.0)
    {
        r = x;
        g = 0.0;
        b = c;
    }
    else
    {
        r = c;
        g = 0.0;
        b = x;
    }

    return {static_cast<uint8_t>((r + m) * 255.0), static_cast<uint8_t>((g + m) * 255.0),
            static_cast<uint8_t>((b + m) * 255.0), static_cast<uint8_t>(a * 255.0)};
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

Color grayscale(const Color& color)
{
    uint8_t gray = static_cast<uint8_t>(0.299 * color.r + 0.587 * color.g + 0.114 * color.b);
    return {gray, gray, gray, color.a};
}
} // namespace color

void Color::fromHex(std::string_view hex) { *this = color::fromHex(hex); }

std::string Color::toHex() const
{
    std::stringstream ss;

    ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << r << std::setw(2) << g
       << std::setw(2) << b << std::setw(2) << a;

    return "#" + ss.str();
}

void Color::fromHSV(const py::sequence& hsv)
{
    if (hsv.size() < 3 || hsv.size() > 4)
        throw std::invalid_argument("HSV tuple must have 3 or 4 elements.");

    const auto h = hsv[0].cast<float>();
    const auto s = hsv[1].cast<float>();
    const auto v = hsv[2].cast<float>();
    const auto a = hsv.size() == 4 ? hsv[3].cast<float>() : 1.0f;

    *this = color::fromHSV(h, s, v, a);
}

py::tuple Color::toHSV() const
{
    float rNorm = r / 255.f;
    float gNorm = g / 255.f;
    float bNorm = b / 255.f;

    float maxVal = std::max({rNorm, gNorm, bNorm});
    float minVal = std::min({rNorm, gNorm, bNorm});
    float delta = maxVal - minVal;

    float h, s, v = maxVal;

    if (delta < 0.00001f)
    {
        h = 0; // Undefined hue
        s = 0;
    }
    else
    {
        s = delta / maxVal;

        if (maxVal == rNorm)
            h = (gNorm - bNorm) / delta + (gNorm < bNorm ? 6 : 0);
        else if (maxVal == gNorm)
            h = (bNorm - rNorm) / delta + 2;
        else
            h = (rNorm - gNorm) / delta + 4;

        h *= 60; // Convert to degrees
    }

    return py::make_tuple(h, s, v, a / 255.f);
}

bool Color::_isValid() const
{
    return 0 <= r && r <= 255 && 0 <= g && g <= 255 && 0 <= b && b <= 255 && 0 <= a && a <= 255;
}
