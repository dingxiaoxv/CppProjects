# CMake最小版本要求
cmake_minimum_required(VERSION 3.0)

# 指定工程名
project(InfoManageSystem)

# 添加头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)

# 添加库文件搜索路径
link_directories(${PROJECT_SOURCE_DIR}/lib)

# 添加源文件
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)

# 设置编译选项
add_compile_options(-Wall -std=c++11)

# 设置版本
set(CMAKE_BUILD_TYPE Debug)

# 设置生成文件路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# 生成可执行文件
add_executable(InfoManageSystem ${SRC_LIST})

# 链接库文件
target_link_libraries(InfoManageSystem InfoManage)
