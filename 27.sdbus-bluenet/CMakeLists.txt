cmake_minimum_required(VERSION 3.10)
project(sdbus-bluenet CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SDBUS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SDBUS_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/lib/libsdbus-c++.so)

add_executable(ble-server
    src/main.cpp
    src/application.cpp
    src/service.cpp
    src/characteristic.cpp
    src/advertisement.cpp
)

target_include_directories(ble-server PRIVATE
    ${SDBUS_INCLUDE_DIR}
)

target_link_libraries(ble-server
    ${SDBUS_LIBRARY}
)

target_compile_options(ble-server PRIVATE -Wall -Wextra)
