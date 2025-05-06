// @author Alexandre P.J. Dixneuf

#include "Decoder.h"
#include <iostream>

// Helper function to print out test results
std::string OutputTestResults(const bool testResult) {
  if (testResult) {
    return "Passed";
  }
  return "Failed";
}

bool Test1() {
  return false;
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
