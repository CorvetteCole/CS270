cmake_minimum_required(VERSION 3.17)
project(proj2 C)

set(CMAKE_C_STANDARD 99)

add_executable(forker forker.c)

add_executable(ipc ipc.c)

add_executable(sigs sigs.c)
