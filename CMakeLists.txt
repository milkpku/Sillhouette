cmake_minimum_required (VERSION 2.6)
project (base)

# set version numer
set (base_VERSION_MAJOR 1)
set (base_VERSION_MINOR 0)

# find OpenGL lib
find_package(OpenGL REQUIRED)

set(ALL_LIBS
    ${OPENGL_LIBRARY}
    glfw
    glut
    GLEW
)

# include MeshLib
include_directories ("${PROJECT_SOURCE_DIR}/MeshLib/core")

# set Debug mode
set (CMAKE_BUILD_TYPE "Debug")
set (CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -o0 -Wall -g2 -ggdb")

add_executable (mytest
    ./src/main.cpp
    ./src/controls.cpp
)

target_link_libraries(mytest ${ALL_LIBS})

add_custom_command(
    TARGET mytest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "mytest" "${CMAKE_CURRENT_SOURCE_DIR}/mytest"
)
