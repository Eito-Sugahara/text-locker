cmake_minimum_required(VERSION 3.16)
project(text_locker CXX)

add_executable(text_locker
    src/main.cpp
)

target_include_directories(text_locker PRIVATE
    include
)
