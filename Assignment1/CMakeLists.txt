cmake_minimum_required(VERSION 2.8)

project(assignment1)

set(SOURCE_EXE main.cpp lightning.cpp building.cpp keyboard.cpp loadOFF.cpp skybox.cpp tesla_boat.cpp tesla_coil.cpp tesla_oscillator.cpp)


find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} )

add_executable(assignment1 ${SOURCE_EXE})

target_link_libraries( assignment1 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

