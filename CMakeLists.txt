cmake_minimum_required(VERSION 3.16)
project(text_locker CXX)

option(BUILD_WINDOWS "Build for Windows using MinGW" OFF)

add_executable(text_locker
    src/main.cpp
)

target_include_directories(text_locker PRIVATE
    include
)

if(BUILD_WINDOWS)
    if(NOT WIN32)
        message(STATUS "Cross-compiling for Windows")
    else()
        message(STATUS "Building for Windows")
    endif()

    target_compile_definitions(text_locker PRIVATE
        _WIN32
    )
endif()
