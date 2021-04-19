cmake_minimum_required(VERSION 3.16)
project(proj3 C)

set(CMAKE_C_STANDARD 99)

add_executable(client UDPClient.c die.c UDPCookie.h)

add_executable(server UDPServer.c die.c UDPCookie.h)