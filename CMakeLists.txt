cmake_minimum_required(VERSION 3.17)
project(parallel_final_c C)

set(CMAKE_C_STANDARD 99)
# added -fopenmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++99 -fopenmp ")
find_package(OpenMP REQUIRED)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
add_executable(parallel_final_c main.c main.h utils.c utils.h sequence.c sequence.h char_cmp.c char_cmp.h mpi.h)