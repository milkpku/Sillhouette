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

add_executable (mytest
    ./src/main.cpp
    ./src/controls.cpp
)

target_link_libraries(mytest ${ALL_LIBS})
