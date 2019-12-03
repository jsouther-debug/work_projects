cmake_minimum_required(VERSION 3.5)
set(CMAKE_C_STANDARD 11)

add_library(masterkey SHARED masterkey.c) # makes an so

add_library(logger SHARED logger.c)
target_link_libraries(logger dl) # required for dlsym lookup

add_executable(target target.c)
add_library(auth SHARED auth.c logger.c)
target_link_libraries(target auth)