cmake_minimum_required(VERSION 3.12)
project(net_safe)

set(CMAKE_CXX_STANDARD 14)

add_executable(net_safe
        src/dh.cpp
        src/dh.h
        tests/test_dh.cpp)