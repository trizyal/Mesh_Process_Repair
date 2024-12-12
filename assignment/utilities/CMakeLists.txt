cmake_minimum_required(VERSION 3.10)
project(utilities)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

add_executable(mesh Mesh.cpp ../../triangle_renderer/Cartesian3.cpp)