cmake_minimum_required(VERSION 3.20)
project(clac)

aux_source_directory(./ SRC)
set(LIBRARY_OUTPUT_PATH ${LIB_PATH})
add_library(${PROJECT_NAME} STATIC ${SRC})