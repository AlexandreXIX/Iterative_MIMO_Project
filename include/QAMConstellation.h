// @author Alexandre P.J. Dixneuf

#ifndef QAMCONSTELLATION_H
#define QAMCONSTELLATION_H

#include "MySignal.h"
#include <iostream>
#include <unordered_map>

/**
 * @class QAMConstellation
 * @brief This class serves to handle the tasks involving the QAM Constellation.
 * @details There are three main tasks this class should handle: 1) Generate a
 * map representing a gray-code M-QAM constellation
 *          2) Use the map to encode an integer signal into a complex signal
 *          3) TODO - find an optimized way to rever this process
 * @param ProblemParameters object
 */

class QAMConstellation {
public:
  // Initializer + Constructor
  QAMConstellation(const ProblemParameters *params);

  // Copy Constructor (banned)
  QAMConstellation(const QAMConstellation &rhs) = delete;

  // Assignment Operator (banned)
  QAMConstellation &operator=(const QAMConstellation &rhs) = delete;

  // Deconstructor (default)
  ~QAMConstellation() = default;

  // Converts Signal int to complex
  void QAMEncoding(MySignal &mySignal);

  // Converts Signal complex to int
  void QAMDecoding(MySignal &mySignal);

  // Draws the constellation to assist testing and debugging
  void DrawConstellation() const;

  // Outputs the constellation for decoder
  std::vector<std::vector<int>> GetConstellation() const;

  // Draws the constellation in binary to verify gray-code
  void DrawGrayCode() const;

  // Print the complex values for the map for testing
  void DrawEncoder() const;

  // Outputs the problem parameters pointer to verify
  const ProblemParameters *GetParameters() const;

  // Checks if a given pointer is the same pointer as this object's parameters
  // (output True means same)
  bool SameParameters(const ProblemParameters *otherPointer) const;

private:
  std::vector<std::vector<int>> Constellation;
  std::unordered_map<int, std::complex<double>> encodingMap;
  const ProblemParameters *params;
};

#endif // QAMCONSTELLATION_H
