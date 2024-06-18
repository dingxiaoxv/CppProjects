cmake_minimum_required(VERSION 3.8)
project(blue_ble)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
add_compile_options(-Wall;-Wextra)

include_directories(include)

set(BLUEZ_SRC src/bluez/att.c
              src/bluez/hci.c
              src/bluez/bluetooth.c
              src/bluez/crypto.c
              src/bluez/gatt-client.c
              src/bluez/gatt-db.c
              src/bluez/gatt-helpers.c
              src/bluez/gatt-server.c
              src/bluez/io-mainloop.c
              src/bluez/mainloop.c
              src/bluez/queue.c
              src/bluez/timeout-mainloop.c
              src/bluez/util.c
              src/bluez/uuid.c)

set(cnr_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/lib/libcnr.a)

add_executable(blue_ble src/main.cpp 
                        src/gattclient.cpp
                        ${BLUEZ_SRC})
                    
target_link_libraries(blue_ble ${cnr_LIBRARIES} pthread)