cmake_minimum_required(VERSION 2.8)
project(aha-c)

set( SRC_LIST
        ./src/app/elf/elflib.c
        ./src/app/elf/elfmain.c)

add_executable(elfmain ${SRC_LIST})