cmake_minimum_required(VERSION 3.10)

set(CMAKE_BUILD_TYPE Debug)

project(thread_pool)

include_directories(include)

add_executable(thread_pool src/main.cpp)

find_package(Threads REQUIRED)

target_link_libraries(thread_pool Threads::Threads)