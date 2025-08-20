cmake_minimum_required(VERSION 3.0)
project(test1)

aux_source_directory(./ SRC)
include_directories(${HEAD_PATH})
link_directories(${LIB_PATH})

set(EXECUTABLE_OUTPUT_PATH ${EXEC_PATH})
add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC calc)