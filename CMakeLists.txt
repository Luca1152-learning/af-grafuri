cmake_minimum_required(VERSION 3.20)
project(Grafuri)

set(CMAKE_CXX_STANDARD 14)

add_executable(Grafuri main.cpp DisjointSet.cpp DisjointSet.h Graf.cpp Graf.h)

set_target_properties(Grafuri PROPERTIES LINK_FLAGS -Wl,-stack_size,10000000)
