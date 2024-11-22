cmake_minimum_required(VERSION 3.10)
project(face2faceindex)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(face2faceindex face2faceindex.cpp ../triangle_renderer/Cartesian3.cpp)