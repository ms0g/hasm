cmake_minimum_required(VERSION 3.5)
project(hvm)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=gnu99")


set(SOURCE_FILES
        hvm.c
        hvm.h
        ../lib/hasmlib.c ../lib/hasmlib.h
        ../lib/hopcodes.h
        )
add_executable(hvm ${SOURCE_FILES})