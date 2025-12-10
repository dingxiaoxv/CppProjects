cmake_minimum_required(VERSION 3.14)
project(CppProjects)

# ============================================
# 全局配置
# ============================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")

# ============================================
# Spdlog 配置（Header-Only）
# ============================================
set(SPDLOG_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third_party/include)
include_directories(${SPDLOG_INCLUDE_DIR})

# ============================================
# 统一输出目录
# ============================================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ============================================
# 项目选择编译
# ============================================
set(PROJECT "all" CACHE STRING "Project to build (all or specific project name)")

# 定义所有可用项目
set(ALL_PROJECTS
    "1.InfoManageSystem"
    "3.udp_thread"
    "5.read_json"
    "12.md5"
    "16.thread_pool"
    "18.compress_file"
    "19.check_file"
    "20.blue_ble"
    "21.myFsm"
    "22.test_cnr"
    "23.simple_enum"
    "24.methodDispatcher"
    "25.nested_cmake"
    "26.sdbus"
    "26.sdbus-bluenet"
)

# 显示编译项目信息
if(PROJECT STREQUAL "all")
    message(STATUS "Building all projects")
    foreach(proj ${ALL_PROJECTS})
        if(EXISTS ${CMAKE_SOURCE_DIR}/${proj}/CMakeLists.txt)
            add_subdirectory(${proj})
        endif()
    endforeach()
else()
    # 处理多个项目（逗号分隔）
    string(REPLACE "," ";" PROJECT_LIST ${PROJECT})
    message(STATUS "Building selected project(s): ${PROJECT_LIST}")
    foreach(proj ${PROJECT_LIST})
        string(STRIP ${proj} proj)  # 移除空格
        if(EXISTS ${CMAKE_SOURCE_DIR}/${proj}/CMakeLists.txt)
            add_subdirectory(${proj})
            message(STATUS "  - Added: ${proj}")
        else()
            message(WARNING "  - Project '${proj}' not found or has no CMakeLists.txt")
        endif()
    endforeach()
endif()

# # ============================================
# # 编译信息总结
# # ============================================
# message(STATUS "==========================================")
# message(STATUS "Configuration Summary:")
# message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
# message(STATUS "  Output Directory: ${CMAKE_BINARY_DIR}")
# message(STATUS "  - Binaries: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
# message(STATUS "  - Libraries: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
# message(STATUS "==========================================")
