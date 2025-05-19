cmake_minimum_required(VERSION 3.15...3.27)

# Scikit-build-core sets these values for you, or you can just hard-code the
# name and version.
project(
  ${SKBUILD_PROJECT_NAME}
  VERSION ${SKBUILD_PROJECT_VERSION}
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Python REQUIRED COMPONENTS Interpreter Development.Module) # Python
find_package(pybind11 CONFIG REQUIRED) # PyBind11
find_package(SDL3 REQUIRED) # SDL3 core

python_add_library(_core MODULE
  src/_globals.cpp
  src/constants.cpp
  src/core.cpp
  src/event.cpp
  src/key.cpp
  src/math.cpp
  src/mouse.cpp
  src/time.cpp
  src/window.cpp
  WITH_SOABI
)
target_link_libraries(_core PRIVATE pybind11::headers SDL3::SDL3)

target_include_directories(_core PRIVATE
  ${SDL3_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# This is passing in the version as a define just as an example
target_compile_definitions(_core PRIVATE VERSION_INFO=${PROJECT_VERSION})

# The install directory is the output (wheel) directory
install(TARGETS _core DESTINATION pykraken)
