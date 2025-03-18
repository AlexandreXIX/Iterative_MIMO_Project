// @author Alexandre P.J. Dixneuf

#include "System.h"
#include <complex>

typedef int Num;

System::System(const Num &N_t, const Num &N_r, const Num &T, const Num &M)
    : N_t(N_t), N_r(N_r), T(T), M(M), H(N_r, N_t), X_int(N_t, T), X_QAM(N_t, T), z(N_r, T) {
  // First, obvious error checks
  if (sqrt(M) * sqrt(M) != M) {
    throw std::invalid_argument("M must be a perfect square (e.g., 4, 16, 64, etc.).");
  }
  // Now, construct
  Users.reserve(N_t);
  for (Num i = 0; i < N_t; i++) {
    Users.emplace_back(T, M);
  }
  GenerateChannelConditions();
  GenerateXInt();
  GenerateXQAM();

  // TODO - remove temporary line
  std::cout << "Here is the matrix H:\n" << H << std::endl;
  std::cout << "Here is the matrix X in int:\n" << X_int << std::endl;
  std::cout << "Here is the matrix X in QAM:\n" << X_QAM << std::endl;
  std::cout << "Here is the matrix z:\n" << z << std::endl;
  std::cout << "Here is the matrix Y:\n" << (H * X_QAM) + z << std::endl;
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
      double const real = dist(gen);
      double const imag = dist(gen);
      auto comp = std::complex(real, imag);
      // sqrt(0.5) due to complex Gaussian random stuff
      z(i, t) = std::sqrt(0.5) * comp;
    }
  }
}

// Takes all the Users' messages, compiles it into matrix X (ints)
void System::GenerateXInt() {
  // For each row, which will correspond to each user
  for (int user_idx = 0; user_idx < N_t; ++user_idx) {
    std::vector<Num> &currentData = Users[user_idx].getData();
    // For each column, which corresponds to each timestep of a user
    for (int t = 0; t < T; ++t) {
      X_int(user_idx, t) = currentData[t];
    }
  }
}

// Takes X in integers and converts to QAM (gray-code)
void System::GenerateXQAM() {
  int sqrtM = sqrt(M); // Guaranteeed to be int thanks to error catching in constructor
  double normFactor =
      sqrt((2.0 * (M - 1)) / 3.0); // Normalization for unit power
  // Generate Gray code
  std::vector<int> grayCode(1 << sqrtM);
  for (int i = 0; i < (1 << sqrtM); i++) {grayCode[i] = i ^ (i >> 1);}





  for (int i = 0; i < N_t; i++) {
    for (int j = 0; j < T; j++) {
      // TODO - verify if this code works
      // TODO - fix error, uses in-order encoding instead of grey-code
      // TODO - just create seperate alphabet
      int symbol = X_int(i, j) - 1; // Convert 1-based index to 0-based

      int row = symbol / sqrtM; // Get row index in constellation
      int col = symbol % sqrtM; // Get column index in constellation

      double I = 2 * col - (sqrtM - 1); // Map to I component
      double Q = (sqrtM - 1) - 2 * row; // Map to Q component (inverted axis)

      X_QAM(i, j) = std::complex<double>(I, Q) / normFactor;
    }
  }
}

