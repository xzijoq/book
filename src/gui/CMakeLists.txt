cmake_minimum_required(VERSION 3.10)
project(gui
    VERSION 1.0)

set(name1 gui)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-Wall -g")

find_package(GLM REQUIRED)
find_package(SDL2 REQUIRED)


include_directories(./include)
#add_executable(${name1} gui.cpp ./src/glad.c)
add_library(${name1} gui.cpp ./src/glad.c)
target_include_directories(${name1} PUBLIC include ${GLM_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})
target_link_libraries(${name1} ${SDL2_LIBRARIES})
