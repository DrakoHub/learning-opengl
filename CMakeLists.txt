cmake_minimum_required(VERSION 3.10)
project(MeuProjeto)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(OpenGL REQUIRED)

set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
file(COPY ${ASSETS_DIR}/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)

find_package(glfw3 REQUIRED)

add_executable(opengl 
    src/config.h 
    src/main.cpp
    src/glad.c
)

target_include_directories(opengl PRIVATE depedencies)
target_link_libraries(opengl ${GLFW3_LIBRARY} OpenGL::GL)