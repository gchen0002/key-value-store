cmake_minimum_required(VERSION 3.16)
project(kv_store VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(kv_store
    src/main.cpp
    src/store.cpp
    src/server.cpp
    src/request.cpp
    src/response.cpp
    src/thread_pool.cpp
    src/socket.cpp
    src/config.cpp
)

target_include_directories(kv_store PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(WIN32)
    target_link_libraries(kv_store PRIVATE ws2_32)
endif()
