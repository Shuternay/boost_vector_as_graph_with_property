cmake_minimum_required(VERSION 3.1)
project(vector_as_graph_with_property)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(vector_as_graph_with_property ${SOURCE_FILES} vector_as_graph_with_property.h)