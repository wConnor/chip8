cmake_minimum_required(VERSION 2.8.12)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-Wall -ltinfo")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

project(chip8)
add_executable(chip8 main.cpp chip8.cpp)

find_package(Threads REQUIRED)
target_link_libraries(chip8 Threads::Threads)
