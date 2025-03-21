// @author Alexandre P.J. Dixneuf

#include "ProblemParameters.h"
#include <iostream>
#include <string>

// Helper function to print out test results
std::string OutputTestResults(const bool testResult) {
  if (testResult) {
    return "Passed";
  }
  return "Failed";
}

bool Test1() {
  // for a range of acceptable parameters, ProblemParameters properly stores and returns the values
  try {
    for (int N_t = 1; N_t <= 1001; N_t = N_t + 100) {
      for (int N_r = 1; N_r <= 1001; N_r = N_r + 100) {
        for (int T = 1; T <= 1001; T = T + 100) {
          for (int M = 4; M <= 4096; M = M * 4) {
            if (ProblemParameters p(N_t, N_r, T, M); (p.GetNt() != N_t) || (p.GetNr() != N_r) || (p.GetT() != T) || (p.GetM() != M) || (p.GetN() != std::sqrt(M))) {return false;}
          }
        }
      }
    }
  }
  catch (std::invalid_argument&) {return false;}
  return true;
}

bool Test2() {
  // If all but one variable is acceptable, ProblemParameters still throws an error
  for (int N_t = 1; N_t <= 100; N_t = N_t + 10) {
    for (int N_r = 1; N_r <= 100; N_r = N_r + 10) {
      for (int T = 1; T <= 100; T = T + 10) {
        for (int M = 4; M <= 2048; M = M * 4) {
          try {
            ProblemParameters a(0, N_r, T, M);
            return false;
          }
          catch (std::invalid_argument&) {
            try {
              int N_rTest = 0;
              ProblemParameters b(N_t, 0, T, M);
              return false;
            }
            catch (std::invalid_argument&) {
              try {
                ProblemParameters c(N_t, N_r, 0, M);
                return false;
              }
              catch (std::invalid_argument&) {
                try {
                  ProblemParameters d(N_t, N_r, T, 0);
                  return false;
                }
                catch (std::invalid_argument&) {;}
              }
            }
          }
        }
      }
    }
  }
  return true;
}

int main() {
  std::cout << "Parameter Tests: " << std::endl;
  std::cout << "Test 1: " << OutputTestResults(Test1()) << std::endl;
  std::cout << "Test 2: " << OutputTestResults(Test2()) << std::endl;
}