cmake_minimum_required(VERSION 3.10)
project(lab1 C)

set(CMAKE_C_STANDARD 99)

add_executable(lab1 main.c)

set(CMAKE_C_EXTENSIONS OFF)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")
endif()