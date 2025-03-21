// @author Alexandre P.J. Dixneuf

#ifndef PROBLEMPARAMETERS_H
#define PROBLEMPARAMETERS_H

#include <cmath>
#include <stdexcept>

/**
 * @class ProblemParameters
 * @brief This class serves as a simple container to store all the problem
 * parameters. This ensures that the exception handling and the problem
 * verification only has to happen once. In addition, it simplifies ensuring
 * that every part of the simulation agrees on the problem.
 * @param Four values, N_t, N_r, T, and M
 *        N_t: The number of transceiver antennas
 *        N_r: The number of receiving antennas
 *        T: The number of transmissions / the timespan of signal transmissions
 *        M: The modulation level. Wished it wasn't necessary for this
 * container, but if 256-QAM, message can't be >256 Note: The class also stores
 * N = sqrt(M), but calculates it itself
 */

class ProblemParameters {
public:
  // Initializer + Constructor
  ProblemParameters(const int &N_t, const int &N_r, const int &T, const int &M);

  // Copy Constructor (deleted)
  ProblemParameters(const ProblemParameters &rhs) = delete;

  // Assignment Operator (deleted)
  ProblemParameters &operator=(const ProblemParameters &rhs) = delete;

  // Destructor (default)
  ~ProblemParameters() = default;

  [[nodiscard]] int GetNt() const;

  [[nodiscard]] int GetNr() const;

  [[nodiscard]] int GetT() const;

  [[nodiscard]] int GetM() const;

  [[nodiscard]] int GetN() const;

private:
  int N_t;
  int N_r;
  int T;
  int M;
  int N;
};

#endif // PROBLEMPARAMETERS_H
