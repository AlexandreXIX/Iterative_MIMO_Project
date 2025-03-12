cmake_minimum_required(VERSION 3.24)
project(Iterative_MIMO_Project)

set(CMAKE_CXX_STANDARD 17)
include_directories(./include ./Eigen)
add_executable(Iterative_MIMO_Project main.cpp)
