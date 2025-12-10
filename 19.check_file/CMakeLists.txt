cmake_minimum_required(VERSION 3.16)

set(CMAKE_BUILD_TYPE Debug)

project(check_file)

include_directories(include)

add_executable(check_file src/main.cpp)
