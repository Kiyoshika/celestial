cmake_minimum_required(VERSION 3.10)

project(lite C)

set(CMAKE_C_STANDARD 99)

option(DEBUG_MODE OFF)

if (DEBUG_MODE)
    set(CMAKE_C_FLAGS "-O0 -Wall -Wextra -g")
    message("Compiling in DEBUG mode...")
else()
    set(CMAKE_C_FLAGS "-O2 -Wall -Wextra")
    message("Compiling in RELEASE mode...")
endif()

add_subdirectory(src)

include(CTest)
enable_testing()

add_subdirectory(test)
