cmake_minimum_required(VERSION 3.14)
project(sirius)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(sirius/common)
add_subdirectory(sirius/core)
add_subdirectory(sirius/auth_server)
#add_subdirectory(sirius/world_server)