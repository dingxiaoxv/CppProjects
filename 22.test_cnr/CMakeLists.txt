cmake_minimum_required(VERSION 3.0.2)

project(test_cnr VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

set(LIBRARY_PATH ${CMAKE_SOURCE_DIR}/libs)

target_link_libraries(${PROJECT_NAME} ${LIBRARY_PATH}/libcnr.a)