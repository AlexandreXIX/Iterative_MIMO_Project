// @author Alexandre P.J. Dixneuf

#include "../include/System.h"
#include <random>

typedef int Num;

System::System(const Num &N_t, const Num &N_r, const Num &T, const Num &M)
    : N_t(N_t), N_r(N_r), T(T), M(M), H(N_r, N_t), z(N_r, T) {
  Users.reserve(N_t);
  for (Num i = 0; i < N_t; i++) {
    Users.emplace_back(T, M);
  }
  GenerateChannelConditions();
}

// Generates the random channel conditions
// Assumes a normal distribution with an average value of 0 and standard
// deviation of 1
void System::GenerateChannelConditions() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::normal_distribution<double> dist(0.0, 1.0);
  // Generate Rayleigh fading matrix H
  for (int i = 0; i < N_r; ++i) {
    for (int j = 0; j < N_t; ++j) {
      double const real = dist(gen);
      double const imag = dist(gen);
      auto comp = std::complex(real, imag);
      // sqrt(0.5) due to complex Gaussian random stuff
      H(i, j) = std::sqrt(0.5) * comp;
    }
    // H and z have same number of rows, so this optimizes for loops
    for (int t = 0; t < T; ++t) {
      z(i, t) = dist(gen);
    }
  }
}
