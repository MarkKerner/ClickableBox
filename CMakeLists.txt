cmake_minimum_required(VERSION 3.3)
project(ClickableBox)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c main.h util.h util.c matrix.h matrix.c)
add_executable(ClickableBox ${SOURCE_FILES})