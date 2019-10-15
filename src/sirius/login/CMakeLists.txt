cmake_minimum_required(VERSION 3.14)
project(login-server)

set(source_file_list main.cpp)

add_executable(login-cli ${source_file_list})
target_link_libraries(login-cli PRIVATE EnTT)

set(CMAKE_CXX_STANDARD 17)