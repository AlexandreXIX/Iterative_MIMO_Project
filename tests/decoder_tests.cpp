// @author Alexandre P.J. Dixneuf

#include "Decoder.h"
#include "../decoders/BPDecoder.cpp"
#include <iostream>

// Helper function to print out test results
std::string OutputTestResults(const bool testResult) {
  if (testResult) {
    return "Passed";
  }
  return "Failed";
}

// Just test simple operation
bool Test1() {
  constexpr int N_t = 2;
  constexpr int N_r = 3;
  constexpr int T = 2;
  constexpr int M = 16;
  const ProblemParameters p(N_t, N_r, T, M);
  MySignal original(&p);
  QAMConstellation Q(&p);
  Channel channel(&p);
  // Get copies of signal
  MySignal copy = original;
  // Encode with QAM
  Q.QAMEncoding(original);
  Q.QAMEncoding(copy);
  // Get second channel to propagate
  channel.ChannelPropagation(copy);
  BPDecoder decoder;
  Eigen::MatrixXcd decoded = decoder.decode(copy.CopyData(), p, Q, channel);
  std::cout << original.CopyData();
  std::cout << decoded;
  return true;
}

bool Test2() {
  return false;
}

bool Test3() {
  return false;
}

bool Test4() {
  return false;
}

bool Test5() {
  return false;
}

bool Test6() {
  return false;
}

bool Test7() {
  return false;
}

int main() {
  std::cout << "Decoder Tests: " << std::endl;
  std::cout << "Test 1: " << OutputTestResults(Test1()) << std::endl;
  std::cout << "Test 2: " << OutputTestResults(Test2()) << std::endl;
  std::cout << "Test 3: " << OutputTestResults(Test3()) << std::endl;
  std::cout << "Test 4: " << OutputTestResults(Test4()) << std::endl;
  std::cout << "Test 5: " << OutputTestResults(Test5()) << std::endl;
  std::cout << "Test 6: " << OutputTestResults(Test6()) << std::endl;
  std::cout << "Test 7: " << OutputTestResults(Test6()) << std::endl;
}
