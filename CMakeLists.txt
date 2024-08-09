cmake_minimum_required(VERSION 3.26.0)  # CMake version check
project(SahilxRicky C)      
set(CMAKE_CXX_STANDARD 11)
# add_library(

# )
find_package(SDL2 REQUIRED COMPONENTS SDL2)
# SET(GCC_COVERAGE_COMPILE_FLAGS "'sdl2-config --libs --cflags' lldb -O0 --std=c99 -Wall -lSDL2_image -lm")
# SET(CMAKE_CXX_FLAGS  ${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS})
add_executable(main src/main.c src/window.c)
target_link_libraries(main PRIVATE SDL2::SDL2)