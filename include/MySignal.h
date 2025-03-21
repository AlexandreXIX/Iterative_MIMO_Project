// @author Alexandre P.J. Dixneuf

#ifndef MYSIGNAL_H
#define MYSIGNAL_H

#include "ProblemParameters.h"
#include <Eigen/Dense>
#include <random>

/**
 * @class MySignal
 * @brief This class serves as a container for a signal.
 * @details To avoid asking a user to understand the Eigen class and how to
 * handle matrices, it is better to create this class. This class should
 * generate random signals and handle the matrix automatically (again,
 * simplifies for user.) All other classes for this project will simply take in
 * the Object rather than each handle random matrices and run exception tests at
 * each step. NOTE: THE CLASS CANNOT BE CALLED "Signal", because that is in the
 * standard library and will destroy everything.
 * @param ProblemParameters object
 */

// Defines a matrix as being dynamically sized with complex double values
using MatrixType =
    Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic>;

class MySignal {
public:
  // Initializer + Constructor
  explicit MySignal(const ProblemParameters *params);

  // Copy Constructor (Deep-Copy) can be default, Eigen does deep copy by
  // default
  MySignal(const MySignal &rhs) = default;

  // Assignment Operator (Deep-Copy) can be default, Eigen does deep copy by
  // default
  MySignal &operator=(const MySignal &rhs) = default;

  // Destructor (default)
  ~MySignal() = default;

  // This function allows a new matrix of data to replace currently stored data.
  // This is how we update the signal after data modulation / encoding /
  // decoding / etc.
  void AlterData(const MatrixType &newMatrix);

  // This functions compare they current signal to another signal to return how
  // accurately they match
  double VerifyAccuracy(const MatrixType &variant);

  // This function runs the verification check, but on Signal object, so must
  // first extract data
  double VerifyAccuracy(const MySignal &variant);

  // Outputs a new matrix that is a deep copy of the stored data
  MatrixType CopyData();

  // Outputs a pointer to the stored matrix
  // This works by using: (*myMatrix) = ...; or (*myMatrix)(i,j) = ...;
  MatrixType *GetData();

  // Outputs the problem parameters pointer to verify
  const ProblemParameters *GetParameters() const;

  // Checks if a given pointer is the same pointer as this object's parameters
  // (output True means same)
  bool SameParameters(const ProblemParameters *otherPointer) const;

private:
  MatrixType data;
  const ProblemParameters *params;
};

#endif // MYSIGNAL_H
