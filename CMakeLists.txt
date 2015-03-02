cmake_minimum_required(VERSION 2.8.4)
project(memory)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp MemoryManager.h)
add_executable(memory ${SOURCE_FILES})