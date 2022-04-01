cmake_minimum_required(VERSION 3.21)
project(projet C)

set(CMAKE_C_STANDARD 23)

file(GLOB_RECURSE SOURCES src/*.c)
file(GLOB_RECURSE HEADERS src/*.h)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
