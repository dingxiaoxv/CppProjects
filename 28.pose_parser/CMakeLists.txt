cmake_minimum_required(VERSION 3.16)

project(pose_parser)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")

add_executable(pose_parser src/main.cpp)
target_include_directories(pose_parser PRIVATE ${VENDOR_INCLUDES})