// @author Alexandre P.J. Dixneuf

#include "QAMConstellation.h"
#include <iostream>

// Helper function to print out test results
std::string OutputTestResults(const bool testResult) {
  if (testResult) {
    return "Passed";
  }
  return "Failed";
}

// Manually verify the M-QAM Constellation mapping
// Automatic check would be time-consuming to code
bool Test0() {
  // Easy test = 16, better test = 64
  constexpr int M = 64;
  const ProblemParameters p(2, 2, 2, M);
  const QAMConstellation q(&p);
  std::string input;
  // First verify proper placing of ints
  q.DrawConstellation();
  q.DrawGrayCode();
  std::cout << "Does the constellation follow proper gray-code? (Y/N)";
  std::cin >> input;
  if (input == "N" || input == "n") {
    return false;
  }
  if (input != "Y" && input != "y") {
    throw std::invalid_argument("Invalid input, input 'Y' or 'N'");
  }
  // Now verify proper graphing of complex values
  q.DrawEncoder();
  std::cout << "Are the complex values properly graphed? (Y/N)";
  std::cin >> input;
  if (input == "N" || input == "n") {
    return false;
  }
  if (input != "Y" && input != "y") {
    throw std::invalid_argument("Invalid input, input 'Y' or 'N'");
  }
  return true;
}

bool Test1() {
  // TODO - add tests
  return false;
}

bool Test2() {
  // TODO - add tests
  return false;
}

bool Test3() {
  // TODO - add tests
  return false;
}

int main() {
  std::cout << "QAM Constellation Tests: " << std::endl;
  std::cout << "Begin Manual Testing: " << std::endl;
  std::cout << OutputTestResults(Test0()) << " Manual Testing" << std::endl;
  std::cout << "Test 1: " << OutputTestResults(Test1()) << std::endl;
  std::cout << "Test 2: " << OutputTestResults(Test2()) << std::endl;
  std::cout << "Test 3: " << OutputTestResults(Test3()) << std::endl;
}
