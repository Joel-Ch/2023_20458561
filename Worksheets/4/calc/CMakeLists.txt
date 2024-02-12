# FILEPATH: /c:/Documents/2023_20458561/Worksheets/4/calc/CMakeLists.txt

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(calc)

# This project contains a library
add_library(maths adder.cpp)

# Note: you could force CMake to always build a static/shared library:
# add_library(maths STATIC adder.cpp)
# add_library(maths SHARED adder.cpp)

if(NOT BUILD_SHARED_LIBS)
    # if static libs are compiled we need
    # to somehow communicate that to the
    # source code. The definition will be
    # used to determine how MATHSLIB API
    # is defined in adder.h
    add_definitions(-Dmaths_STATIC)
endif()

# It also contains an executable
add_executable(calc calc.cpp)

# The executable uses the library
target_link_libraries(calc maths)

# /calc/cmake/CMakeLists.txt

# /calc/cmake/CMakeLists.txt
