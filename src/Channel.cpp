// @author Alexandre P.J. Dixneuf

#include "Channel.h"

// Initializer + Constructor
Channel::Channel(const ProblemParameters *params)
    : H(params->GetNr(), params->GetNt()), Z(params->GetNr(), params->GetT()),
      params(params) {
  const int N_t = params->GetNt();
  const int N_r = params->GetNr();
  const int T = params->GetT();
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
    // H and z have the same number of rows, so this optimizes for loops
    for (int t = 0; t < T; ++t) {
      double const real = dist(gen);
      double const imag = dist(gen);
      auto comp = std::complex(real, imag);
      // sqrt(0.5) due to complex Gaussian random stuff
      Z(i, t) = std::sqrt(0.5) * comp;
    }
  }
}

// Take in a signal, and propagate it according to the channel conditions
void Channel::ChannelPropagation(MySignal &transmitted) const {
  if (CompatibilityTest(transmitted.GetParameters()) == false) {
    throw std::invalid_argument("Channel and Signal parameters don't match.");
  }
  transmitted.AlterData((H * transmitted.CopyData()) + Z);
}

// Output the fading condition matrix H
MatrixType Channel::GetH() const { return H; }

// Output the noise matrix Z
MatrixType Channel::GetZ() const { return Z; }

// Outputs the problem parameters pointer to verify
const ProblemParameters *Channel::GetParameters() const { return params; };

// Checks if a given pointer is the same pointer as this object's parameters
// (output True means same)
bool Channel::SameParameters(const ProblemParameters *otherPointer) const {
  return params == otherPointer;
};

// Verify that the parameter is compatible
// Different than SameParameters() because a different object with same elements
// still results in True This allows same channel for different M for testing
bool Channel::CompatibilityTest(const ProblemParameters *otherPointer) const {
  // If the params are same, no need to check
  if (params == otherPointer) {
    return true;
  }
  if (params->GetNr() == otherPointer->GetNr() &&
      params->GetNt() == otherPointer->GetNt() &&
      params->GetT() == otherPointer->GetT()) {
    return true;
  }
  return false;
};
