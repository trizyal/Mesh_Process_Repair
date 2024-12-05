cmake_minimum_required(VERSION 3.10)
project(eulerformulacalculation)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(eulerformulacalculation eulerformulacalculation.cpp ../../triangle_renderer/Cartesian3.cpp ../utilities/Mesh.cpp)