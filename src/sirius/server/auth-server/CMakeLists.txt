cmake_minimum_required(VERSION 3.14)
project(auth_server)

set(CMAKE_CXX_STANDARD 17)

add_executable(auth-server main.cpp)
target_link_libraries(auth-server pthread stdc++fs uuid)