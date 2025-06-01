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
  // for a range of acceptable parameters, ProblemParameters properly stores and
  // returns the values
  try {
    for (int N_t = 1; N_t <= 1001; N_t = N_t + 100) {
      for (int N_r = 1; N_r <= 1001; N_r = N_r + 100) {
        for (int T = 1; T <= 1001; T = T + 100) {
          for (int M = 4; M <= 4096; M = M * 4) {
            for (int SNR = 1; SNR <= 1001; SNR = SNR + 100) {
              if (ProblemParameters p(N_t, N_r, T, M, SNR);
                  (p.GetNt() != N_t) || (p.GetNr() != N_r) || (p.GetT() != T) ||
                  (p.GetM() != M) || (p.GetN() != std::sqrt(M)) ||
                  (p.GetSNR() != SNR)) {
                return false;
              }
            }
          }
        }
      }
    }
  } catch (std::invalid_argument &) {
    return false;
  }
  return true;
}

bool Test2() {
  // If all but one variable is acceptable, ProblemParameters still throws an
  // error
  for (int N_t = 1; N_t <= 100; N_t = N_t + 10) {
    for (int N_r = 1; N_r <= 100; N_r = N_r + 10) {
      for (int T = 1; T <= 100; T = T + 10) {
        for (int M = 4; M <= 2048; M = M * 4) {
          for (int SNR = 1; SNR <= 100; SNR = SNR + 10) {
            try {
              ProblemParameters a(0, N_r, T, M, SNR);
              return false;
            } catch (std::invalid_argument &) {
              try {
                ProblemParameters b(N_t, 0, T, M, SNR);
                return false;
              } catch (std::invalid_argument &) {
                try {
                  ProblemParameters c(N_t, N_r, 0, M, SNR);
                  return false;
                } catch (std::invalid_argument &) {
                  try {
                    ProblemParameters d(N_t, N_r, T, 0, SNR);
                    return false;
                  } catch (std::invalid_argument &) {
                    try {
                      ProblemParameters d(N_t, N_r, T, M, 0);
                      return false;
                    } catch (std::invalid_argument &) {
                      ;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return true;
}

bool Test3() {
  // Verify that ProblemParameter copies the value of integers, not the
  // reference Also serves as a stress test for larger values
  int N_t = 100000, N_r = 100000, T = 100000, M = 16384, SNR = 100000;
  // Not const just in case in alters test
  ProblemParameters p(N_t, N_r, T, M, SNR);
  ++N_t;
  ++N_r;
  ++T;
  ++M;
  ++SNR;
  if (p.GetNt() == N_t) {
    return false;
  }
  if (p.GetNr() == N_r) {
    return false;
  }
  if (p.GetT() == T) {
    return false;
  }
  if (p.GetM() == M) {
    return false;
  }
  if (p.GetSNR() == SNR) {
    return false;
  }
  return true;
}

int main() {
  std::cout << "Parameter Tests: " << std::endl;
  std::cout << "Test 1: " << OutputTestResults(Test1()) << std::endl;
  std::cout << "Test 2: " << OutputTestResults(Test2()) << std::endl;
  std::cout << "Test 3: " << OutputTestResults(Test2()) << std::endl;
}