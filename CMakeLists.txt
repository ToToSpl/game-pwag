cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(game)

find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)

if(NOT MSVC)
  add_definitions("-w")
else()
  add_definitions("-WX -W2")
endif()

add_executable(
  game
  src/main.cpp
  src/GameObject.cpp
  src/Scene.cpp
  src/Renderer.cpp
  src/Shader.cpp
  src/Texture.cpp
  src/Camera.cpp
  src/Player.cpp
  src/Duck.cpp
  lib/lodepng.cpp)

target_link_libraries(game -lglfw -lglew "-framework Cocoa" "-framework OpenGL"
                      "-framework IOKit")
