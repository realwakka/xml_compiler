cmake_minimum_required(VERSION 2.8.6)
project(xmlcompiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g")

add_library(default_parser SHARED default_parser.cc)

add_executable(xtest main.cc)
target_link_libraries(xtest dl)
