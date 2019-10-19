cmake_minimum_required(VERSION 3.14)
project(login-server)

set(source_file_list main.cpp game/network/message_reader.cpp system/network_system.cpp)

add_executable(login-cli ${source_file_list})
target_link_libraries(login-cli PUBLIC EnTT fmt::fmt fmt::fmt-header-only pthread)

set(CMAKE_CXX_STANDARD 17)