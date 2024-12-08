cmake_minimum_required(VERSION 3.16)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
project(tetris C)

add_executable(tetris main.c constants.h render.c colors.c blocks.c mechanics.c)
target_link_libraries(tetris SDL2 SDL2_ttf)