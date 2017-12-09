cmake_minimum_required(VERSION 3.7)
project(fenwick_tree)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp FenwickTree.h)
add_executable(fenwick_tree ${SOURCE_FILES})