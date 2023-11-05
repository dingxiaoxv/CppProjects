cmake_minimum_required(VERSION 3.0.0)
project(ringbuffer_1 VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(include ${OPENSSL_INCLUDE_DIR})
aux_source_directory(src SRC_LIST)
add_compile_options(-Wall -std=c++11)
add_executable(md5 ${SRC_LIST})
target_link_libraries(md5 ssl crypto)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
