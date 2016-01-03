cmake_minimum_required(VERSION 3.3)
project(Eat_snake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

link_libraries(curses m)
set(SOURCE_FILES main.c main.h)
add_executable(Eat_snake ${SOURCE_FILES} cJSON.h cJSON.c)