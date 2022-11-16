cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(game)

if(NOT MSVC)
  add_definitions("-w")
else()
  add_definitions("-WX -W2")
endif()

add_executable(game src/main.cpp src/Scene.cpp)

target_link_libraries(game "-framework OpenGL" "-framework GLUT")
