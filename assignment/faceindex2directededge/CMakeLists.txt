cmake_minimum_required(VERSION 3.10)
project(faceindex2directededge)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(faceindex2directededge faceindex2directededge.cpp ../../triangle_renderer/Cartesian3.cpp ../utilities/Mesh.cpp)