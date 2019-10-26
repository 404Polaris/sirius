cmake_minimum_required(VERSION 3.14)
project(LoginServer)

set(source_file_list
        Main.cpp
        Game/Network/MessageReader.cpp
        System/NetworkSystem.cpp
        System/LoginSystem.cpp)

add_executable(LoginServer ${source_file_list})
target_link_libraries(LoginServer PUBLIC EnTT fmt::fmt fmt::fmt-header-only pthread)

set(CMAKE_CXX_STANDARD 17)