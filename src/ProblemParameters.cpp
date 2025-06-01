// @author Alexandre P.J. Dixneuf

#include "ProblemParameters.h"

// Initializer + Constructor
ProblemParameters::ProblemParameters(const int &N_t, const int &N_r,
                                     const int &T, const int &M, const int &SNR)
    : N_t(N_t), N_r(N_r), T(T), M(M), N(static_cast<int>(sqrt(M))), SNR(SNR) {
  // First verify that every value is within acceptable domain
  // For M, ignore the technically valid 2 since no reason to spend time fixing
  // everything to allow
  if ((N_t < 1) || (N_r < 1) || (T < 1) || (M <= 1) || (SNR < 1)) {
    throw std::invalid_argument("Invalid problem parameters.");
  }
  // Now verify that M is acceptable
  int N = static_cast<int>(sqrt(M));
  if (N * N != M) {
    throw std::invalid_argument("M must be a perfect square.");
  }
  int power2Test = static_cast<int>(std::log2(M));
  if (pow(2, power2Test) != M) {
    throw std::invalid_argument("M is not a power of 2.");
  }
}

int ProblemParameters::GetNt() const { return N_t; }

int ProblemParameters::GetNr() const { return N_r; }

int ProblemParameters::GetT() const { return T; }

int ProblemParameters::GetM() const { return M; }

int ProblemParameters::GetN() const { return N; }

int ProblemParameters::GetSNR() const { return SNR; }
