cmake_minimum_required(VERSION 3.10)
project(book
    VERSION 1.0
    LANGUAGES CXX
    )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS 
    "-Weverything -g  -Wall -Wno-c++98-compat -Wno-c++98-compat-pedantic")
add_subdirectory(./src/part1/)
add_subdirectory(./src/gui/)
set(s ./src/)
#include_directories(./include)
add_executable(p1 main.cpp )
#target_sources()
target_include_directories(p1 PUBLIC ./include)
#target_link_directories(p1 PUBLIC ./lib)
target_link_libraries(p1 one gui)




