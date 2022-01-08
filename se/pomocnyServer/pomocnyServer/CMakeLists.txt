cmake_minimum_required(VERSION 3.7)
project(pomocnyServer C)

set(CMAKE_C_STANDARD 99)

add_executable(pomocnyServer main.c server.c server.h)
target_link_libraries(pomocnyServer pthread)