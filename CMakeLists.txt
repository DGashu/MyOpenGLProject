cmake_minimum_required(VERSION 3.16)
project(OpenGLApp)

set(CMAKE_CXX_STANDARD 17)

add_executable(OpenGLApp src/main.cpp)

find_package(OpenGL REQUIRED)

add_subdirectory(external/glfw)
add_subdirectory(external/glad)

target_link_libraries(OpenGLApp PRIVATE OpenGL::GL glfw glad)