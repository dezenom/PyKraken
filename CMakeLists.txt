cmake_minimum_required(VERSION 3.15...3.30)

project(
  ${SKBUILD_PROJECT_NAME}
  VERSION ${SKBUILD_PROJECT_VERSION}
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED) # PyBind11
find_package(SDL3 REQUIRED) # SDL3 core

pybind11_add_module(_core MODULE
  src/_globals.cpp
  src/color.cpp
  src/constants.cpp
  src/core.cpp
  src/event.cpp
  src/key.cpp
  src/math.cpp
  src/mouse.cpp
  src/rect.cpp
  src/time.cpp
  src/window.cpp
  WITH_SOABI
)
target_link_libraries(_core PRIVATE pybind11::headers SDL3::SDL3)

target_include_directories(_core PRIVATE
  ${SDL3_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

install(TARGETS _core DESTINATION ${SKBUILD_PROJECT_NAME})
