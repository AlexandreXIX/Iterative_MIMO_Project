// @author Alexandre P.J. Dixneuf

#include "Channel.h"
#include <iostream>
#include <string>

// Helper function to print out test results
std::string OutputTestResults(const bool testResult) {
  if (testResult) {
    return "Passed";
  }
  return "Failed";
}

// Test that two identical signals will propagate in identical ways
bool Test1() {
  // TODO - add tests
  return false;
}

// Test that the propagation will refuse to work with different problem
// parameters
bool Test2() {
  // TODO - add tests
  return false;
}

bool Test3() {
  // TODO - add tests
  return false;
}

int main() {
  std::cout << "Channel Tests: " << std::endl;
  std::cout << "Test 1: " << OutputTestResults(Test1()) << std::endl;
  std::cout << "Test 2: " << OutputTestResults(Test2()) << std::endl;
  std::cout << "Test 3: " << OutputTestResults(Test3()) << std::endl;
}