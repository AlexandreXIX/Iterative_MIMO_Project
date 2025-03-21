// @author Alexandre P.J. Dixneuf

#ifndef QAMCONSTELLATION_H
#define QAMCONSTELLATION_H

#include "MySignal.h"

/**
 * @class QAMConstellation
 * @brief This class serves to handle the tasks involving the QAM Constellation.
 * @details There are three main tasks this class should handle:
 *          1) Generate a grid representing a gray-code M-QAM constellation.
 *          2) Takes a Signal object with integer values and encodes them into
 * QAM using the generated constellation. 3) Takes an encoded Signal object and
 * decodes them using the constellation back into integer values. It is
 * important to note that this class only stores the constellation. Re-encoding
 * and decoding using the same constellation is possible by keeping the same
 * instance of this class. Note that the container also stores N = sqrt(M) to
 * avoid recalculation.
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

  // Outputs the problem parameters pointer to verify
  const ProblemParameters *GetParameters() const;

  // Checks if a given pointer is the same pointer as this object's parameters
  // (output True means same)
  bool SameParameters(const ProblemParameters *otherPointer) const;

private:
  // Helper function
  // Finds the corresponding QAM symbol for a given message (only takes real)
  [[nodiscard]] std::pair<int, int> FindQAM(const double &message) const;

  std::vector<std::vector<int>> Constellation;
  const ProblemParameters *params;
};

#endif // QAMCONSTELLATION_H
