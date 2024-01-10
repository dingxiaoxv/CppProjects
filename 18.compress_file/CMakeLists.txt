cmake_minimum_required(VERSION 3.10)

set(CMAKE_BUILD_TYPE Debug)

project(compress_file)

find_package(ZLIB REQUIRED)

include_directories(include)

add_executable(compress_file src/main.cpp)

target_link_libraries(compress_file PRIVATE ZLIB::ZLIB)
