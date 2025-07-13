cmake_minimum_required(VERSION 3.10)
project(simple_enum)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译警告
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")

# 启用调试模式
set(CMAKE_BUILD_TYPE Debug)

# 生成 compile_commands.json 以便 clangd 使用
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 添加可执行文件
add_executable(simple_enum src/main.cpp)
