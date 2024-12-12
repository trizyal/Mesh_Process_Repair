cmake_minimum_required(VERSION 3.10)
project(assignment)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

add_subdirectory(face2faceindex)
add_subdirectory(faceindex2directededge)
add_subdirectory(manifoldtesting)
add_subdirectory(eulerformulacalculation)
add_subdirectory(meshrepair)
# add_subdirectory(utilities)
add_library(utilities utilities/Mesh.cpp)
