cmake_minimum_required(VERSION 3.16)

project(sdbus CXX)

add_executable(sdbus-server src/user_manager_server.cpp)
target_include_directories(sdbus-server PRIVATE ${VENDOR_INCLUDES})
target_link_libraries(sdbus-server ${SDBUS_LIBRARY})

add_executable(sdbus-client src/user_manager_client.cpp)
target_include_directories(sdbus-client PRIVATE ${VENDOR_INCLUDES})
target_link_libraries(sdbus-client ${SDBUS_LIBRARY})