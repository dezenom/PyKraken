#include "Color.hpp"
#include "Math.hpp"
#include "PixelArray.hpp"
#include "Rect.hpp"

#include <SDL3_image/SDL_image.h>

namespace pixel_array
{
void _bind(py::module_& module)
{
    // py::native_enum<ScrollType>(module, "ScrollType", "enum.IntEnum")
    //     .value("SCROLL_SMEAR", ScrollType::SCROLL_SMEAR)
    //     .value("SCROLL_ERASE", ScrollType::SCROLL_ERASE)
    //     .value("SCROLL_REPEAT", ScrollType::SCROLL_REPEAT)
    //     .export_values()
    //     .finalize();

    py::classh<PixelArray>(module, "PixelArray", R"doc(
Represents a 2D pixel buffer for image manipulation and blitting operations.

A PixelArray is a 2D array of pixels that can be manipulated, drawn on, and used as a source
for texture creation or blitting to other PixelArrays. Supports pixel-level operations,
color key transparency, and alpha blending.
    )doc")
        .def(py::init<const Vec2&>(), py::arg("size"), R"doc(
Create a new PixelArray with the specified dimensions.

Args:
    size (Vec2): The size of the pixel array as (width, height).

Raises:
    RuntimeError: If pixel array creation fails.
        )doc")
        .def(py::init<const std::string&>(), py::arg("file_path"), R"doc(
Create a PixelArray by loading an image from a file.

Args:
    file_path (str): Path to the image file to load.

Raises:
    RuntimeError: If the file cannot be loaded or doesn't exist.
        )doc")

        .def("fill", &PixelArray::fill, py::arg("color"), R"doc(
Fill the entire pixel array with a solid color.

Args:
    color (Color): The color to fill the pixel array with.
        )doc")
        .def("blit",
             py::overload_cast<const PixelArray&, const Vec2&, Anchor, py::object>(
                 &PixelArray::blit, py::const_),
             py::arg("pixel_array"), py::arg("pos"), py::arg("anchor") = Anchor::CENTER,
             py::arg("src") = py::none(), R"doc(
Blit (copy) another pixel array onto this pixel array at the specified position with anchor alignment.

Args:
    pixel_array (PixelArray): The source pixel array to blit from.
    pos (Vec2): The position to blit to.
    anchor (Anchor, optional): The anchor point for positioning. Defaults to CENTER.
    src (Rect, optional): The source rectangle to blit from. Defaults to entire source pixel array.

Raises:
    RuntimeError: If the blit operation fails.
        )doc")
        .def("blit",
             py::overload_cast<const PixelArray&, const Rect&, py::object>(&PixelArray::blit,
                                                                           py::const_),
             py::arg("pixel_array"), py::arg("dst"), py::arg("src") = py::none(), R"doc(
Blit (copy) another pixel array onto this pixel array with specified destination and source rectangles.

Args:
    pixel_array (PixelArray): The source pixel array to blit from.
    dst (Rect): The destination rectangle on this pixel array.
    src (Rect, optional): The source rectangle to blit from. Defaults to entire source pixel array.

Raises:
    RuntimeError: If the blit operation fails.
        )doc")
        .def("get_at", &PixelArray::getAt, py::arg("coord"), R"doc(
Get the color of a pixel at the specified coordinates.

Args:
    coord (Vec2): The coordinates of the pixel as (x, y).

Returns:
    Color: The color of the pixel at the specified coordinates.

Raises:
    IndexError: If coordinates are outside the pixel array bounds.
        )doc")
        .def("set_at", &PixelArray::setAt, py::arg("coord"), py::arg("color"), R"doc(
Set the color of a pixel at the specified coordinates.

Args:
    coord (Vec2): The coordinates of the pixel as (x, y).
    color (Color): The color to set the pixel to.

Raises:
    IndexError: If coordinates are outside the pixel array bounds.
        )doc")
        .def("copy", &PixelArray::copy, R"doc(
Create a copy of this pixel array.

Returns:
    PixelArray: A new PixelArray that is an exact copy of this one.

Raises:
    RuntimeError: If pixel array copying fails.
        )doc")

        .def_property("color_key", &PixelArray::getColorKey, &PixelArray::setColorKey, R"doc(
The color key for transparency.

When set, pixels of this color will be treated as transparent during blitting operations.
Used for simple transparency effects.

Returns:
    Color: The current color key.

Raises:
    RuntimeError: If getting the color key fails.
        )doc")
        .def_property("alpha_mod", &PixelArray::getAlpha, &PixelArray::setAlpha, R"doc(
The alpha modulation value for the pixel array.

Controls the overall transparency of the pixel array. Values range from 0 (fully transparent)
to 255 (fully opaque).

Returns:
    int: The current alpha modulation value [0-255].

Raises:
    RuntimeError: If getting the alpha value fails.
        )doc")

        .def_property_readonly("width", &PixelArray::getWidth, R"doc(
The width of the pixel array.

Returns:
    int: The pixel array width.
        )doc")
        .def_property_readonly("height", &PixelArray::getHeight, R"doc(
The height of the pixel array.

Returns:
    int: The pixel array height.
        )doc")
        .def_property_readonly("size", &PixelArray::getSize, R"doc(
The size of the pixel array as a Vec2.

Returns:
    Vec2: The pixel array size as (width, height).
        )doc")
        .def_property_readonly("rect", &PixelArray::getRect, R"doc(
A rectangle representing the pixel array bounds.

Returns:
    Rect: A rectangle with position (0, 0) and the pixel array's dimensions.
        )doc");
}
} // namespace pixel_array

PixelArray::PixelArray(SDL_Surface* sdlSurface) : m_surface(sdlSurface) {}

PixelArray::PixelArray(const Vec2& size)
{
    if (m_surface)
    {
        SDL_DestroySurface(m_surface);
        m_surface = nullptr;
    }

    m_surface = SDL_CreateSurface(static_cast<int>(size.x), static_cast<int>(size.y),
                                  SDL_PIXELFORMAT_RGBA32);

    if (!m_surface)
        throw std::runtime_error("PixelArray failed to create: " + std::string(SDL_GetError()));
}

PixelArray::PixelArray(const std::string& filePath)
{
    if (m_surface)
    {
        SDL_DestroySurface(m_surface);
        m_surface = nullptr;
    }

    m_surface = IMG_Load(filePath.c_str());
    if (!m_surface)
        throw std::runtime_error("Failed to load pixel array from file '" + filePath +
                                 "': " + std::string(SDL_GetError()));
}

PixelArray::~PixelArray()
{
    if (m_surface)
    {
        SDL_DestroySurface(m_surface);
        m_surface = nullptr;
    }
}

void PixelArray::fill(const Color& color) const
{
    auto colorMap = SDL_MapSurfaceRGBA(m_surface, color.r, color.g, color.b, color.a);
    SDL_FillSurfaceRect(m_surface, nullptr, colorMap);
}

void PixelArray::blit(const PixelArray& other, const Vec2& pos, const Anchor anchor,
                      py::object srcRect) const
{
    SDL_Rect srcSDL;
    if (!srcRect.is_none())
    {
        try
        {
            srcSDL = srcRect.cast<Rect>();
        }
        catch (const py::cast_error&)
        {
            throw std::invalid_argument("'src' must be a Rect");
        }
    }
    else
    {
        srcSDL = other.getRect();
    }

    Rect dstRect = other.getRect();
    switch (anchor)
    {
    case Anchor::TOP_LEFT:
        dstRect.setTopLeft(pos);
        break;
    case Anchor::TOP_MID:
        dstRect.setTopMid(pos);
        break;
    case Anchor::TOP_RIGHT:
        dstRect.setTopRight(pos);
        break;
    case Anchor::MID_LEFT:
        dstRect.setMidLeft(pos);
        break;
    case Anchor::CENTER:
        dstRect.setCenter(pos);
        break;
    case Anchor::MID_RIGHT:
        dstRect.setMidRight(pos);
        break;
    case Anchor::BOTTOM_LEFT:
        dstRect.setBottomLeft(pos);
        break;
    case Anchor::BOTTOM_MID:
        dstRect.setBottomMid(pos);
        break;
    case Anchor::BOTTOM_RIGHT:
        dstRect.setBottomRight(pos);
        break;
    }

    SDL_Rect dstSDL = dstRect;

    if (!SDL_BlitSurface(other.getSDL(), &srcSDL, m_surface, &dstSDL))
        throw std::runtime_error("Failed to blit pixel array: " + std::string(SDL_GetError()));
}

void PixelArray::blit(const PixelArray& other, const Rect& dstRect, py::object srcRect) const
{
    SDL_Rect dstSDL = dstRect;
    SDL_Rect srcSDL;

    if (!srcRect.is_none())
    {
        try
        {
            srcSDL = srcRect.cast<Rect>();
        }
        catch (const py::cast_error&)
        {
            throw std::invalid_argument("'src' must be a Rect");
        }
    }
    else
    {
        srcSDL = other.getRect();
    }

    if (!SDL_BlitSurface(other.getSDL(), &srcSDL, m_surface, &dstSDL))
        throw std::runtime_error("Failed to blit pixel array: " + std::string(SDL_GetError()));
}

void PixelArray::setColorKey(const Color& color) const
{
    SDL_SetSurfaceColorKey(m_surface, true,
                           SDL_MapSurfaceRGBA(m_surface, color.r, color.g, color.b, color.a));
}

Color PixelArray::getColorKey() const
{
    uint32_t key;
    if (!SDL_GetSurfaceColorKey(m_surface, &key))
        throw std::runtime_error("Failed to get pixel array color key: " +
                                 std::string(SDL_GetError()));

    Color color;
    color.r = static_cast<uint8_t>((key >> 24) & 0xFF);
    color.g = static_cast<uint8_t>((key >> 16) & 0xFF);
    color.b = static_cast<uint8_t>((key >> 8) & 0xFF);
    color.a = static_cast<uint8_t>(key & 0xFF);

    return color;
}

void PixelArray::setAlpha(const uint8_t alpha) const { SDL_SetSurfaceAlphaMod(m_surface, alpha); }

int PixelArray::getAlpha() const
{
    uint8_t alpha;
    if (!SDL_GetSurfaceAlphaMod(m_surface, &alpha))
        throw std::runtime_error("Failed to get pixel array alpha: " + std::string(SDL_GetError()));
    return alpha;
}

Color PixelArray::getAt(const Vec2& coord) const
{
    if (coord.x < 0 || coord.x >= m_surface->w || coord.y < 0 || coord.y >= m_surface->h)
        throw std::out_of_range("Coordinates out of bounds for pixel array");

    auto* pixels = static_cast<uint8_t*>(m_surface->pixels);
    int pitch = m_surface->pitch;
    auto x = static_cast<int>(coord.x);
    auto y = static_cast<int>(coord.y);

    uint32_t pixel = *reinterpret_cast<uint32_t*>(pixels + y * pitch + x * sizeof(uint32_t));

    Color color;
    auto formatDetails = SDL_GetPixelFormatDetails(m_surface->format);
    SDL_GetRGBA(pixel, formatDetails, nullptr, &color.r, &color.g, &color.b, &color.a);

    return color;
}

void PixelArray::setAt(const Vec2& coord, const Color& color) const
{
    if (coord.x < 0 || coord.x >= m_surface->w || coord.y < 0 || coord.y >= m_surface->h)
        throw std::out_of_range("Coordinates out of bounds for pixel array");

    auto* pixels = static_cast<uint8_t*>(m_surface->pixels);
    int pitch = m_surface->pitch;
    auto x = static_cast<int>(coord.x);
    auto y = static_cast<int>(coord.y);

    auto formatDetails = SDL_GetPixelFormatDetails(m_surface->format);
    uint32_t pixel = SDL_MapRGBA(formatDetails, nullptr, color.r, color.g, color.b, color.a);
    *reinterpret_cast<uint32_t*>(pixels + y * pitch + x * sizeof(uint32_t)) = pixel;
}

int PixelArray::getWidth() const { return m_surface->w; }

int PixelArray::getHeight() const { return m_surface->h; }

Vec2 PixelArray::getSize() const { return {m_surface->w, m_surface->h}; }

Rect PixelArray::getRect() const { return Rect(0, 0, m_surface->w, m_surface->h); }

std::unique_ptr<PixelArray> PixelArray::copy() const
{
    SDL_Surface* surfaceCopy = SDL_CreateSurface(m_surface->w, m_surface->h, m_surface->format);
    if (!surfaceCopy)
        throw std::runtime_error("Failed to create copy pixel array: " +
                                 std::string(SDL_GetError()));

    if (!SDL_BlitSurface(m_surface, nullptr, surfaceCopy, nullptr))
        throw std::runtime_error("Failed to blit pixel array copy: " + std::string(SDL_GetError()));

    auto copy = std::make_unique<PixelArray>();
    copy->m_surface = surfaceCopy;
    return copy;
}

SDL_Surface* PixelArray::getSDL() const { return m_surface; }
