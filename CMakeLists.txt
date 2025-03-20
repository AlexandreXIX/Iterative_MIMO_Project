cmake_minimum_required(VERSION 3.24)
project(Iterative_MIMO_Project)

set(CMAKE_CXX_STANDARD 17)
include_directories(./include ./eigen-3.4.0 ./decoders)

# Automatically detect all decoders
# This means future users only need to add the .cpp and never touch CmakeLists.txt
#aux_source_directory(decoders DECODER_FILES)

# Add main executable
# TODO - change this once finally done
add_executable(Iterative_MIMO_Project main.cpp src/MySignal.cpp)

# New tests for v2.0
add_executable(Parameter_tests tests/parameter_tests.cpp src/ProblemParameters.cpp)
add_executable(Signal_tests tests/Signal_tests.cpp src/MySignal.cpp src/ProblemParameters.cpp)
add_executable(QAMConstellation_tests tests/QAMConstellation_tests.cpp src/QAMConstellation.cpp src/ProblemParameters.cpp)
add_executable(Channel_tests tests/channel_tests.cpp src/Channel.cpp src/MySignal.cpp src/ProblemParameters.cpp)
add_executable(Final_tests tests/all_tests.cpp)
add_dependencies(Final_tests Parameter_tests Signal_tests QAMConstellation_tests Channel_tests)