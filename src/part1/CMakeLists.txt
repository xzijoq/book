cmake_minimum_required(VERSION 3.10)

project(part1
    VERSION 1.0
    LANGUAGES CXX
    )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#message("the top: \${book_SOURCE_DIR}")

add_library(one one.cpp )

#install(a $())
