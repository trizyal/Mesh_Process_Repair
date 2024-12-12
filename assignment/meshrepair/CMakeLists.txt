cmake_minimum_required(VERSION 3.10)
project(meshrepair)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -mmmx -msse -msse -msse2 -msse3 -mssse3 -fomit-frame-pointer")

add_executable(meshrepair meshrepair.cpp ../../triangle_renderer/Cartesian3.cpp ../utilities/Mesh.cpp)