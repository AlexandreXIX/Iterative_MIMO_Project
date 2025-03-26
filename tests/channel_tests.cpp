// @author Alexandre P.J. Dixneuf

#include "Channel.h"
#include "QAMConstellation.h"
#include <iostream>
#include <string>

// Helper function to print out test results
std::string OutputTestResults(const bool testResult) {
  if (testResult) {
    return "Passed";
  }
  return "Failed";
}

// Test that the propagation will refuse to work with different problem
// parameters, but will allow different object with compatible values (M can be
// different)
bool Test1() {
  constexpr int N_t = 100;
  constexpr int N_r = 100;
  constexpr int T = 100;
  constexpr int M = 64;
  const ProblemParameters p1(N_t, N_r, T, M);
  const ProblemParameters p2(N_t, N_r, T, M);
  const ProblemParameters bad1(N_t + 1, N_r, T, M);
  const ProblemParameters bad2(N_t, N_r + 1, T, M);
  const ProblemParameters bad3(N_t, N_r, T + 1, M);
  const ProblemParameters acceptable(N_t, N_r, T, M * 4);
  QAMConstellation Q(&p1);
  // Define original
  MySignal original(&p1);
  Q.QAMEncoding(original);
  const Channel myChannel(&p1);
  // Part 1: Everything works with same parameters
  try {
    myChannel.ChannelPropagation(original);
  } catch (std::invalid_argument &) {
    return false;
  }
  // Part 2: Signal with p2 (same parameters) works
  try {
    MySignal DiffButSame(&p1);
    Q.QAMEncoding(DiffButSame);
    myChannel.ChannelPropagation(DiffButSame);
  } catch (std::invalid_argument &) {
    return false;
  }
  // Part 3: Signals with bad1, bad2, bad3 throw errors
  try {
    MySignal badSig(&bad1);
    Q.QAMEncoding(badSig);
    myChannel.ChannelPropagation(badSig);
    return false;
  } catch (std::invalid_argument &) {
    ;
  }
  try {
    MySignal badSig(&bad2);
    Q.QAMEncoding(badSig);
    myChannel.ChannelPropagation(badSig);
    return false;
  } catch (std::invalid_argument &) {
    ;
  }
  try {
    MySignal badSig(&bad3);
    Q.QAMEncoding(badSig);
    myChannel.ChannelPropagation(badSig);
    return false;
  } catch (std::invalid_argument &) {
    ;
  }
  // Part 4: Different M acceptable
  try {
    MySignal NoM(&acceptable);
    Q.QAMEncoding(NoM);
    myChannel.ChannelPropagation(NoM);
  } catch (std::invalid_argument &) {
    return false;
  }
  return true;
}

// Test that two identical signals will propagate in identical ways
bool Test2() {
  constexpr int N_t = 100;
  constexpr int N_r = 100;
  constexpr int T = 100;
  constexpr int M = 64;
  const ProblemParameters p1(N_t, N_r, T, M);
  MySignal original(&p1);
  QAMConstellation Q(&p1);
  Channel channel(&p1);
  // Get copies of signal
  MySignal copy = original;
  MySignal untouched = original;
  // Encode with QAM
  Q.QAMEncoding(original);
  Q.QAMEncoding(copy);
  Q.QAMEncoding(untouched);
  // Only go through channel propagation for 2
  channel.ChannelPropagation(original);
  channel.ChannelPropagation(copy);
  // Both propagated are the same and not the original
  if ((original.CopyData() != copy.CopyData()) ||
      (original.CopyData() == untouched.CopyData())) {
    return false;
  }
  return true;
}

// Verify accuracy of channel propagation
bool Test3() {
  constexpr int N_t = 100;
  constexpr int N_r = 100;
  constexpr int T = 100;
  constexpr int M = 64;
  const ProblemParameters p1(N_t, N_r, T, M);
  MySignal s(&p1);
  QAMConstellation Q(&p1);
  Channel channel(&p1);
  // Do it manually
  MatrixType X = s.CopyData();
  MatrixType H = channel.GetH();
  MatrixType Z = channel.GetZ();
  MatrixType YManual = (H * X) + Z;
  // Now function
  channel.ChannelPropagation(s);
  MatrixType YAuto = s.CopyData();
  // Check equality
  if (YManual != YAuto) {
    return false;
  }
  // Check that matrix multiplication is not point-wise
  Eigen::Matrix2d a;
  a << 1, 2, 3, 4;
  Eigen::Matrix2d b;
  b << 1, 2, 3, 4;
  Eigen::Matrix2d correct;
  correct << 7, 10, 15, 22;
  if (a * b != correct) {
    return false;
  }
  return true;
}

int main() {
  std::cout << "Channel Tests: " << std::endl;
  std::cout << "Test 1: " << OutputTestResults(Test1()) << std::endl;
  std::cout << "Test 2: " << OutputTestResults(Test2()) << std::endl;
  std::cout << "Test 3: " << OutputTestResults(Test3()) << std::endl;
}