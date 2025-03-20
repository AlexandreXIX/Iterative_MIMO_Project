cmake_minimum_required(VERSION 3.24)
project(Iterative_MIMO_Project)

set(CMAKE_CXX_STANDARD 17)
include_directories(./include ./eigen-3.4.0 ./decoders)

# Automatically detect all decoders
# This means future users only need to add the .cpp and never touch CmakeLists.txt
#aux_source_directory(decoders DECODER_FILES)

# Add main executable
add_executable(Iterative_MIMO_Project main.cpp src/User.cpp src/System.cpp
        src/Signal.cpp
        tests/signal_tests.cpp
        src/QAMConstellation.cpp
        include/QAMConstellation.h)

# Add test executable (only built when needed)
add_executable(my_tests tests/test.cpp src/User.cpp src/System.cpp)

# Add test executable (only built when needed)
add_executable(placeholder_tests tests/placeholder_test.cpp)

# New tests for v2.0
add_executable(Signal_tests tests/Signal_tests.cpp src/Signal.cpp)
add_executable(QAMConstellation_tests tests/QAMConstellation_tests.cpp src/QAMConstellation.cpp)
add_executable(FinalTests tests/all_tests.cpp
        src/Channel.cpp
        include/Channel.h)