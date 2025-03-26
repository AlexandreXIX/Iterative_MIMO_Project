// @author Alexandre P.J. Dixneuf

#include "QAMConstellation.h"
#include <iostream>

/* Note: most of the tests for this channel rely on manual testing, as the code
 * to verify the accuracy would itself have to be tested for accuracy, only
 * pushing the problem further back. */

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
  constexpr int M = 16;
  const ProblemParameters p(2, 2, 2, M);
  QAMConstellation q(&p);
  std::string input;
  // First verify proper placing of ints
  q.DrawConstellation();
  q.DrawGrayCode();
  std::cout << "Does the constellation follow proper gray-code? (Y/N): ";
  std::cin >> input;
  if (input == "N" || input == "n") {
    return false;
  }
  if (input != "Y" && input != "y") {
    throw std::invalid_argument("Invalid input, input 'Y' or 'N'");
  }
  // Now verify proper graphing of complex values
  q.DrawEncoder();
  std::cout << "Are the complex values properly graphed? (Y/N): ";
  std::cin >> input;
  if (input == "N" || input == "n") {
    return false;
  }
  if (input != "Y" && input != "y") {
    throw std::invalid_argument("Invalid input, input 'Y' or 'N'");
  }
  // Now verify proper encoding
  MySignal s(&p);
  std::cout << "For a given signal X: " << std::endl;
  std::cout << s.CopyData().real() << std::endl;
  std::cout << "The signal gets encoded as:" << std::endl;
  q.QAMEncoding(s);
  const double normFactor = sqrt((2.0 * (p.GetM() - 1)) / 3.0);
  std::cout << s.CopyData() * normFactor << std::endl;
  std::cout << "Is the data properly encoded? (Y/N): ";
  std::cin >> input;
  if (input == "N" || input == "n") {
    return false;
  }
  if (input != "Y" && input != "y") {
    throw std::invalid_argument("Invalid input, input 'Y' or 'N'");
  }
  return true;
}

int main() {
  std::cout << "QAM Constellation Tests: " << std::endl;
  std::string input;
  std::cout << "Would you like to perform manual testing? (Y/[other]): ";
  std::cin >> input;
  if (input == "Y" || input == "y") {
    std::cout << OutputTestResults(Test0()) << " Manual Testing" << std::endl;
  }
}
