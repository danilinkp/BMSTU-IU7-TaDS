cmake_minimum_required(VERSION 3.31)
project(lab_08_07 C)

include_directories("inc/")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wpedantic -Wextra -Wfloat-equal -O0 -g3 -Wfloat-conversion -lm")

set(SOURCES src/main.c
        src/graph.c
        src/input_tools.c)

add_executable(app ${SOURCES})
