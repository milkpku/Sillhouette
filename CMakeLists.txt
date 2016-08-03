cmake_minimum_required (VERSION 2.6)
project (hm1)

# set version numer
set (hm1_VERSION_MAJOR 1)
set (hm1_VERSION_MINOR 0)

# include MeshLib
include_directories ("${PROJECT_SOURCE_DIR}/MeshLib/core")

# find OpenGL lib

set (SOURCE_FILES src/main.cpp)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lGL -lglut")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -lGL -lglut")

add_executable (mytest ${SOURCE_FILES})
