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
  std::cout << "Test 1: " << OutputTestResults(Test1()) << std::endl;
  std::cout << "Test 2: " << OutputTestResults(Test2()) << std::endl;
  std::cout << "Test 3: " << OutputTestResults(Test3()) << std::endl;
}
