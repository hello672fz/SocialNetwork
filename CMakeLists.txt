cmake_minimum_required(VERSION 3.16)
project(exp02)

set(CMAKE_CXX_STANDARD 17)
set(EXP02_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_compile_options(-fexceptions)
add_compile_options(-frtti)

include_directories(utils/)

# src
add_subdirectory(src exp02_media_service)

# test
add_subdirectory(test test_exp02_media_service)
