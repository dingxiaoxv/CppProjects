cmake_minimum_required(VERSION 3.10)
project(sdbus-bluenet CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDBUS REQUIRED sdbus-c++)

add_executable(ble-server
    src/main.cpp
    src/application.cpp
    src/service.cpp
    src/characteristic.cpp
)

target_include_directories(ble-server PRIVATE
    include
    ${SDBUS_INCLUDE_DIRS}
)

target_link_libraries(ble-server
    ${SDBUS_LIBRARIES}
)

# 编译选项
target_compile_options(ble-server PRIVATE -Wall -Wextra)
