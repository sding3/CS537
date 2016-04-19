cmake_minimum_required(VERSION 3.3)
project(linux)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    mem.c
    mem.h
    tester.c)

add_executable(linux ${SOURCE_FILES})