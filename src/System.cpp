// @author Alexandre P.J. Dixneuf

#include "System.h"
#include <complex>
#include <bitset>

typedef int Num;

System::System(const Num &N_t, const Num &N_r, const Num &T, const Num &M)
    : N_t(N_t), N_r(N_r), T(T), M(M), N(std::sqrt(M)), H(N_r, N_t), X_int(N_t, T), X_QAM(N_t, T), z(N_r, T), GrayCodeGrid(N, std::vector<Num>(N)) {
  // First, obvious error checks
  if (N * N != M) {
    throw std::invalid_argument("M must be a perfect square.");
  }
  // Currently this code only works with square QAM, so M must be a perfect power of 2
  int intTest = std::log2(M); // If log2(M) converted to int is not perfect, then not a power of 2
  if (pow(2,intTest) != M) {
    throw std::invalid_argument("M is not a power of 2.");
  }
  // Now, construct
  Users.reserve(N_t);
  for (Num i = 0; i < N_t; i++) {
    Users.emplace_back(T, M);
  }
  GenerateChannelConditions();
  GenerateXInt();
  GenerateGrayCode();
  GenerateXQAM();

  // TODO - remove temporary line
  std::cout << "Here is the matrix H:\n" << H << std::endl;
  std::cout << "Here is the matrix X in int:\n" << X_int << std::endl;
  std::cout << "Here is the Gray-code:\n";
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      std::cout << GrayCodeGrid[i][j] << " ";
    }
    std::cout << std::endl;
  }
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

// Generate the Gray-Code, store to keep consistent
// Note: exact encoding not too important, consistency is
void System::GenerateGrayCode() {
  // Generate initial
  std::vector<Num> tmpVector(M);
  for (int i = 0; i < M; i++) {
    tmpVector[i] = (i ^ (i >> 1)); // Generating Gray code
  }
  // Problem is this fails when on a grid
  // Easier to make grid and then back into vector
  int index = 0;
  for (int i = 0; i < N; i++) {
    // Left to right
    if (i % 2 == 0) {for (int j = 0; j < N; j++) {GrayCodeGrid[i][j] = tmpVector[index++];} }
    // Right to left (snake pattern)
    else {for (int j = N - 1; j >= 0; j--) {GrayCodeGrid[i][j] = tmpVector[index++];} }
  }
}

// Helper function
// Converts individual ints to QAM
std::complex<double> System::IntToQAM(Num int_value) {
  // TODO - fix this, completely wrong
  // Find max magnitude (same for real or img)
  int max_mag = (N-1);
  // Find the integer in the matrix
  // TODO - find a more optimized way to do this
  int row = -1, col = -1;
  for (int i = 0; i < N; i++) {
    // This is one of the reasons only perfect square QAM possible
    for (int j = 0; j < N; j++) {
      if (GrayCodeGrid[i][j] == int_value) {
        row = i;
        col = j;
        break;
      }
    }
  }
  if (row == -1 || col == -1) {throw std::invalid_argument("QAM encoding error.");}
  // Convert to complex
  // Use double because eventually need to modulate power
  const double real = -max_mag + (2*col);
  const double img = -max_mag + (2*row);
  // Real goes - to + left to right, but img + to -, so flip
  return {real, -img};
}


// Takes X in integers and converts to QAM (gray-code)
void System::GenerateXQAM() {
  int sqrtM = sqrt(M); // Guaranteeed to be int thanks to error catching in constructor
  // Normalization for unit power
  double normFactor = sqrt((2.0 * (M - 1)) / 3.0);
  // Calculate every value
  for (int R = 0; R < N_t; R++) {
    for (int C = 0; C < T; C++) {
      // TODO - use normFactor
      X_QAM(R, C) = IntToQAM(X_int(R,C));
    }
  }
}

