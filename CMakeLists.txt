cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(sirius)

set(CMAKE_CXX_STANDARD 17)

include(cmake/init.cmake)

add_subdirectory(src)
