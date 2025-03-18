cmake_minimum_required(VERSION 3.24)
project(Iterative_MIMO_Project)

set(CMAKE_CXX_STANDARD 17)
include_directories(./include ./eigen-3.4.0)

# Add main executable
add_executable(Iterative_MIMO_Project main.cpp src/User.cpp src/System.cpp)

# Add test executable (only built when needed)
add_executable(my_tests test.cpp src/User.cpp src/System.cpp)

# Add test executable (only built when needed)
add_executable(placeholder_tests placeholder_test.cpp)