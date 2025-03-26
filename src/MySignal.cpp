// @author Alexandre P.J. Dixneuf

#include "MySignal.h"

// Initializer + Constructor
MySignal::MySignal(const ProblemParameters *params)
    : data(params->GetNt(), params->GetT()), params(params) {
  // Initialize a random number generator
  std::random_device rd;  // a seed source for the random number engine
  std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> distrib(1, params->GetM());
  // Now for each value in the data stream, assign some random value between 0
  // and dim - 1 (dim = M in M-QAM)
  for (int i = 0; i < params->GetNt(); i++) {
    for (int j = 0; j < params->GetT(); j++) {
      data(i, j) = std::complex<double>(distrib(gen) - 1, 0.0);
    }
  }
}

// This function allows a new matrix of data to replace currently stored data.
// This is how we update the signal after channel propagation / encoding /
// decoding / etc.
void MySignal::AlterData(const MatrixType &newMatrix) { data = newMatrix; }

// This functions compare they current signal to another signal to return how
// accurately they match
double MySignal::VerifyAccuracy(const MatrixType &variant) {
  // If matrix sizes incompatible, throw exception
  if ((data.rows() != variant.rows()) || (data.cols() != variant.cols())) {
    throw std::invalid_argument("Comparing matrices of different sizes.");
  }
  // If we assume more often accurate than not, minimally better to enter if
  // statement only when errors occur
  double accuracy = data.size();
  for (int i = 0; i < data.rows(); i++) {
    for (int j = 0; j < data.cols(); j++) {
      if (data(i, j) != variant(i, j)) {
        --accuracy;
      }
    }
  }
  return accuracy / data.size();
}

// This function runs the verification check, but on Signal object, so must
// first extract data
double MySignal::VerifyAccuracy(const MySignal &variant) {
  return VerifyAccuracy(variant.data);
}

// Outputs a new matrix that is a deep copy of the stored data
MatrixType MySignal::CopyData() { return data; }

// Outputs a pointer to the stored matrix
MatrixType *MySignal::GetData() { return &data; }

// Outputs the problem parameters pointer to verify
const ProblemParameters *MySignal::GetParameters() const { return params; }

// Checks if a given pointer is the same pointer as this object's parameters
// (output True means same)
bool MySignal::SameParameters(const ProblemParameters *otherPointer) const {
  return params == otherPointer;
}
