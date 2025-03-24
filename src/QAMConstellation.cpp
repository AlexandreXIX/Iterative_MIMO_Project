// @author Alexandre P.J. Dixneuf

#include "QAMConstellation.h"
#include "../src/MySignal.cpp"

// Initializer + Constructor
QAMConstellation::QAMConstellation(const ProblemParameters *params)
    : Constellation(params->GetN(), std::vector<int>(params->GetN())),
      encodingMap(params->GetM()), params(params) {
  // Get constants
  const int M = params->GetM();
  const int N = params->GetN();
  const int N_t = params->GetNt();
  const int T = params->GetT();
  // Generate initial
  std::vector<int> tmpVector(M);
  for (int i = 0; i < M; i++) {
    tmpVector[i] = (i ^ (i >> 1)); // Generating Gray code
  }
  // First generate the Constrellation as a grid
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
  // We now convert this grid into an unordered map for encoding in 0(1)
  // Calculate constants
  // Normalize the power to 1
  const double normFactor = sqrt((2.0 * (M - 1)) / 3.0);
  // Find the maximum magnitude for a symbol
  const int max_mag = (N - 1);
  // Now iterate
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      const double real = -max_mag + (2 * j);
      const double img = -max_mag + (2 * i);
      encodingMap[Constellation[i][j]] =
          std::complex<double>(real, -img) / normFactor;
    }
  }
}

// Converts Signal int to complex
void QAMConstellation::QAMEncoding(MySignal &mySignal) {
  // TODO - complete
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

void QAMConstellation::DrawGrayCode() const {
  const int N = params->GetN();
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      for (int bit = N - 1; bit >= 0; --bit) {
        std::cout << ((Constellation[i][j] >> bit) & 1);
      }
      std::cout << " ";
    }
    std::cout << std::endl;
  }
}

void QAMConstellation::DrawEncoder() const {
  // Remove the normalization to draw cleaner
  const double normFactor = sqrt((2.0 * (params->GetM() - 1)) / 3.0);
  for (int i = 0; i < params->GetN(); i++) {
    for (int j = 0; j < params->GetN(); j++) {
      std::complex<double> toPrint =
          encodingMap.at(Constellation[i][j]) * normFactor;
      std::cout << "(";
      if (toPrint.real() > 0) {
        std::cout << "+";
      }
      std::cout << toPrint.real();
      if (toPrint.imag() > 0) {
        std::cout << " + " << toPrint.imag();
      } else {
        std::cout << " - " << -toPrint.imag();
      }
      std::cout << "i) ";
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
