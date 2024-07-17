cmake_minimum_required(VERSION 3.0.2)

project(test_zip VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

find_package(Boost REQUIRED COMPONENTS filesystem system)

include_directories(${CMAKE_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/src/*.c)

add_executable(${PROJECT_NAME} ${SOURCES})

set(LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(${PROJECT_NAME} ${LIBRARY_PATH}/libzip.a ${Boost_LIBRARIES})