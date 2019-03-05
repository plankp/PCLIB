cmake_minimum_required(VERSION 2.8.12)
cmake_policy(SET CMP0054 NEW)
project(PCLIB)

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_C_FLAGS "-std=c99 -Wall -O3")
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_C_FLAGS, "-std=c99 -Wall -O3")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_C_FLAGS "/O2 /W4")
endif()

include_directories(${PCLIB_SOURCE_DIR}/header)
add_subdirectory(${PCLIB_SOURCE_DIR}/src)

add_library(pclib
    $<TARGET_OBJECTS:src>)
