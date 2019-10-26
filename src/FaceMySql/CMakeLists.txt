cmake_minimum_required(VERSION 3.14)
project(FaceMySql)

set(CMAKE_CXX_STANDARD 17)

if (NOT MySqlClient_FOUND)
    message(FATAL_ERROR "MySqlClient is not found.")
endif ()

