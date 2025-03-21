// @author Alexandre P.J. Dixneuf

#include "QAMConstellation.h"
#include "../src/MySignal.cpp"
#include <iostream>

// Initializer + Constructor
QAMConstellation::QAMConstellation(const ProblemParameters *params)
    : Constellation(params->GetN(), std::vector<int>(params->GetN())),
      params(params) {
  // Now, construct the constellation
  const int M = params->GetM();
  const int N = params->GetN();
  // Generate initial
  std::vector<int> tmpVector(M);
  for (int i = 0; i < M; i++) {
    tmpVector[i] = (i ^ (i >> 1)); // Generating Gray code
  }
  // Now we convert to a properly ordered grid to simulate graphing
  int index = 0;
  for (int i = 0; i < N; i++) {
    // Left to right
    if (i % 2 == 0) {
      for (int j = 0; j < N; j++) {
        Constellation[i][j] = tmpVector[index++];
      }
    }
    // Right to left (snake pattern)
    else {
      for (int j = N - 1; j >= 0; j--) {
        Constellation[i][j] = tmpVector[index++];
      }
    }
  }
}

// Converts Signal int to complex
void QAMConstellation::QAMEncoding(MySignal &mySignal) {
  // Get necessary data points
  const int M = params->GetM();
  const int N = params->GetN();
  const int N_t = params->GetNt();
  const int T = params->GetT();
  // Normalize the power to 1
  const double normFactor = sqrt((2.0 * (M - 1)) / 3.0);
  // Find the maximum magnitude for a symbol
  const int max_mag = (N - 1);
  // Iterate through all signal points
  MatrixType X_QAM(N_t, T);
  MatrixType X = mySignal.CopyData();
  // For every data point
  for (int R = 0; R < N_t; R++) {
    for (int C = 0; C < T; C++) {
      auto [fst, snd] = FindQAM(Constellation[R][C]);
      const double real = -max_mag + (2 * snd);
      const double img = -max_mag + (2 * fst);
      X_QAM(R, C) = std::complex<double>(real, -img);
    }
  }
  mySignal.AlterData(X_QAM);
}

// Converts Signal complex to int
void QAMConstellation::QAMDecoding(MySignal &mySignal) {
  // TODO - complete
}

void QAMConstellation::DrawConstellation() const {
  for (int i = 0; i < params->GetN(); i++) {
    for (int j = 0; j < params->GetN(); j++) {
      std::cout << Constellation[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

// Outputs the problem parameters pointer to verify
const ProblemParameters *QAMConstellation::GetParameters() const {
  return params;
};

// Checks if a given pointer is the same pointer as this object's parameters
// (output True means same)
bool QAMConstellation::SameParameters(
    const ProblemParameters *otherPointer) const {
  return params == otherPointer;
};

// Helper function
// Finds the corresponding coordinates of the QAM symbol for a given message
// (only takes real)
std::pair<int, int> QAMConstellation::FindQAM(const double &message) const {
  const int N = params->GetN();
  for (int i = 0; i < N; i++) {
    // This is one of the reasons only perfect square QAM possible
    for (int j = 0; j < N; j++) {
      if (Constellation[i][j] == message) {
        return {i, j};
      }
    }
  }
  throw std::invalid_argument("QAM encoding error.");
}
