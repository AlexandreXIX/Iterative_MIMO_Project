cmake_minimum_required(VERSION 3.24)
project(Iterative_MIMO_Project)

set(CMAKE_CXX_STANDARD 17)

# Collect all .h files
include_directories(./include ./eigen-3.4.0 ./decoders)

# TODO - figure out how to use this without breaking everything
# Collect all .cpp files to run things
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
# Collect all test files .cpp
file(GLOB TESTS ${PROJECT_SOURCE_DIR}/tests/*.cpp)
# Collect all the decoders .cpp
file(GLOB DECODERS ${PROJECT_SOURCE_DIR}/decoders/*.cpp)

# Add main executable
# TODO - change this once finally done
add_executable(Iterative_MIMO_Project main.cpp src/MySignal.cpp)

# New tests for v2.0
add_executable(Parameter_tests tests/parameter_tests.cpp src/ProblemParameters.cpp)
add_executable(Signal_tests tests/Signal_tests.cpp src/MySignal.cpp src/ProblemParameters.cpp)
add_executable(QAMConstellation_tests tests/QAMConstellation_tests.cpp src/QAMConstellation.cpp src/ProblemParameters.cpp)
add_executable(Channel_tests tests/channel_tests.cpp src/Channel.cpp src/QAMConstellation.cpp src/ProblemParameters.cpp)
add_executable(Decoder_tests tests/decoder_tests.cpp ${DECODERS})
add_executable(Final_tests tests/all_tests.cpp)
add_dependencies(Final_tests Parameter_tests Signal_tests QAMConstellation_tests Channel_tests)