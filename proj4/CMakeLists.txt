cmake_minimum_required(VERSION 3.17)
project(proj4)

set(CMAKE_CXX_STANDARD 14)

add_executable(proj4 simpsh.cpp Variables.cpp Commands.cpp)