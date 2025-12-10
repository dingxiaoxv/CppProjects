cmake_minimum_required(VERSION 3.16)
project(sdbus-bluenet CXX)

add_executable(ble-server
    src/main.cpp
    src/application.cpp
    src/service.cpp
    src/characteristic.cpp
    src/advertisement.cpp
)

target_include_directories(ble-server PRIVATE ${VENDOR_INCLUDES})

target_link_libraries(ble-server ${SDBUS_LIBRARY})

target_compile_options(ble-server PRIVATE -Wall -Wextra)
