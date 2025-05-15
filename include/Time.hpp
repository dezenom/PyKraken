#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace kn::time
{
void _bind(py::module_& module);

class Clock
{
  public:
    Clock();
    ~Clock() = default;

    double tick(uint16_t frameRate = 0);

    uint64_t getFPS() const;

  private:
    uint64_t m_lastTick;
    uint64_t m_fps = 0;
};

double getElapsedTime();

void delay(uint64_t ms);
} // namespace kn::time
