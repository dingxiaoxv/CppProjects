cmake_minimum_required(VERSION 3.20)
project(nested_cmake)

set(LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(EXEC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_subdirectory(calc)
add_subdirectory(test1)