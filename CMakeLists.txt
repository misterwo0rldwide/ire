cmake_minimum_required(VERSION 3.20)

project(ire C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(ire
    src/main.c
)

target_compile_options(ire PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)