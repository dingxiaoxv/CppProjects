cmake_minimum_required(VERSION 3.0.0)
project(InfoManage VERSION 0.1.0)

# 添加头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)
# 添加源文件
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)
# 设置编译选项
add_compile_options(-Wall -std=c++11)
# 设置版本
set(CMAKE_BUILD_TYPE Debug)
# 设置生成文件路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
# 生成库文件
add_library(${PROJECT_NAME} STATIC ${SRC_LIST})

# GTest
include(CTest)
enable_testing()
add_executable(${PROJECT_NAME}_test ./test/InfoManageTest.cpp)
# 寻找GTest和Threads库，两个库必需
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
# 包含GTest的头文件目录
include_directories(${GTEST_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)
# 需要链接要生成的库、gtest、pthread库
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
# Enable CMake's testing runner to discover the test executable file.
# If CMake's version is greater than 3.10, use the "gtest_discover_tests" statement, otherwise use the "add_test" statement.
gtest_discover_tests(${PROJECT_NAME}_test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
