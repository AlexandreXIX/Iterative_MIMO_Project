cmake_minimum_required(VERSION 3.24)
project(Iterative_MIMO_Project)

set(CMAKE_CXX_STANDARD 17)
include_directories(./include ./eigen-3.4.0 ./decoders)

# Automatically detect all decoders
# This means future users only need to add the .cpp and never touch CmakeLists.txt
#aux_source_directory(decoders DECODER_FILES)

# Add main executable
add_executable(Iterative_MIMO_Project main.cpp src/User.cpp src/System.cpp src/MySignal.cpp)

# Add test executable (only built when needed)
add_executable(my_tests tests/test.cpp src/User.cpp src/System.cpp)

# Add test executable (only built when needed)
add_executable(placeholder_tests tests/placeholder_test.cpp)

# New tests for v2.0
add_executable(Parameter_tests tests/parameter_tests.cpp src/ProblemParameters.cpp)
add_executable(Signal_tests tests/Signal_tests.cpp src/MySignal.cpp)
add_executable(QAMConstellation_tests tests/QAMConstellation_tests.cpp src/QAMConstellation.cpp)
add_executable(Final_tests tests/all_tests.cpp)
add_dependencies(Final_tests Parameter_tests Signal_tests QAMConstellation_tests)