cmake_minimum_required(VERSION 3.7)
project(BSTree)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        BSTree.h
        main.cpp)

add_executable(BSTree ${SOURCE_FILES})