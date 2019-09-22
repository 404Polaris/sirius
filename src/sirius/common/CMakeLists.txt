cmake_minimum_required(VERSION 3.14)
project(common)

set(CMAKE_CXX_STANDARD 17)

set(commone_files
        network/tcp_server.cpp ../auth_server/main.cpp)


add_library(common SHARED ${commone_files})

target_link_libraries(common PUBLIC asio asio::asio pthread)
target_link_libraries(common PUBLIC fmt::fmt fmt::fmt-header-only)