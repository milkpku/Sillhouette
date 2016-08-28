cmake_minimum_required (VERSION 2.6)
project (base)

# set version numer
set (base_VERSION_MAJOR 1)
set (base_VERSION_MINOR 0)

# include MeshLib
include_directories ("${PROJECT_SOURCE_DIR}/MeshLib/core")

# find OpenGL lib

file (GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file (GLOB_RECURSE HEAD_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

set (CMAKE_CXX_COMPILER g++)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lGL -lGLU -lglut")
# set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -lGL -lglut")

add_executable (mytest ${SRC_FILES})
