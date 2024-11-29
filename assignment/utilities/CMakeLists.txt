cmake_minimum_required(VERSION 3.10)
project(utilities)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(mesh Mesh.cpp ../../triangle_renderer/Cartesian3.cpp)