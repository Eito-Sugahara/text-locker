cmake_minimum_required(VERSION 3.16)
project(tl CXX)

option(BUILD_WINDOWS "Build for Windows using MinGW" OFF)

add_executable(tl
    src/main.cpp
)

target_include_directories(tl PRIVATE
    include
)

install(TARGETS tl DESTINATION bin)

if(BUILD_WINDOWS)
    if(NOT WIN32)
        message(STATUS "Cross-compiling for Windows")
    else()
        message(STATUS "Building for Windows")
    endif()

    target_compile_definitions(tl PRIVATE
        _WIN32
    )
    target_link_options(tl PRIVATE
        -static-libgcc
        -static-libstdc++
    )
endif()
