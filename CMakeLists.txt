cmake_minimum_required(VERSION 3.10)
project(PRG1DAG)

set(CMAKE_CXX_STANDARD 17)

add_executable(main 
    src/parse.cpp
    src/error_handling.cpp
    src/main.cpp
)

target_include_directories(main PRIVATE include/)