cmake_minimum_required(VERSION 3.14)

project(sdbus CXX)

set(SDBUS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SDBUS_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/lib/libsdbus-c++.so)

add_executable(sdbus-server src/user_manager_server.cpp)
target_include_directories(sdbus-server PRIVATE ${SDBUS_INCLUDE_DIR})
target_link_libraries(sdbus-server ${SDBUS_LIBRARY})

add_executable(sdbus-client src/user_manager_client.cpp)
target_include_directories(sdbus-client PRIVATE ${SDBUS_INCLUDE_DIR})
target_link_libraries(sdbus-client ${SDBUS_LIBRARY})