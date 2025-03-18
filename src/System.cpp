// @author Alexandre P.J. Dixneuf

#include "System.h"
#include <complex>

typedef int Num;

System::System(const Num &N_t, const Num &N_r, const Num &T, const Num &M)
    : N_t(N_t), N_r(N_r), T(T), M(M), H(N_r, N_t), X(N_t, T), z(N_r, T) {
  // TODO - instead of having error checks at each point, check all here
  // (example: M perfect square)
  Users.reserve(N_t);
  for (Num i = 0; i < N_t; i++) {
    Users.emplace_back(T, M);
  }
  GenerateChannelConditions();
  // TODO - rather than assign, immediately set
  X = GenerateXQAM();

  // TODO - remove temporary line
  std::cout << "Here is the matrix H:\n" << H << std::endl;
  std::cout << "Here is the matrix X:\n" << X << std::endl;
  std::cout << "Here is the matrix z:\n" << z << std::endl;
  // TODO - fix, this is QAM
  std::cout << "Here is the matrix Y:\n" << (H * X) + z << std::endl;
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

// Takes all the Users' messages, compiles it into matrix X, and converts to QAM
Eigen::MatrixXcd System::GenerateXQAM() {
  // Generate the integer version of X
  Eigen::MatrixXi X_ints = Eigen::MatrixXi::Zero(N_t, T);
  // For each row, which will correspond to each user
  for (int user_idx = 0; user_idx < N_t; ++user_idx) {
    std::vector<Num> &currentData = Users[user_idx].getData();
    // For each column, which corresponds to each timestep of a user
    for (int t = 0; t < T; ++t) {
      X_ints(user_idx, t) = currentData[t];
    }
  }
  // Convert to QAM
  int sqrtM = sqrt(M);
  // TODO - add to constructor
  // if (sqrtM * sqrtM != M) {
  //  throw invalid_argument("M must be a perfect square (e.g., 4, 16, 64,
  //  etc.).");
  //}
  double normFactor =
      sqrt((2.0 * (M - 1)) / 3.0); // Normalization for unit power
  Eigen::MatrixXcd X_QAM(N_t, T);
  for (int i = 0; i < N_t; i++) {
    for (int j = 0; j < T; j++) {
      int symbol = X_ints(i, j) - 1; // Convert 1-based index to 0-based

      int row = symbol / sqrtM; // Get row index in constellation
      int col = symbol % sqrtM; // Get column index in constellation

      double I = 2 * col - (sqrtM - 1); // Map to I component
      double Q = (sqrtM - 1) - 2 * row; // Map to Q component (inverted axis)

      X_QAM(i, j) = std::complex<double>(I, Q) / normFactor;
    }
  }
  return X_QAM;
}
