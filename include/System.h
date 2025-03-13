// @author Alexandre P.J. Dixneuf

#ifndef ITERATIVE_MIMO_PROJECT_SYSTEM_H
#define ITERATIVE_MIMO_PROJECT_SYSTEM_H

#include "User.h"
#include <Eigen/Dense>

typedef int Num; // can switch to uint32_t if necessary

class System {
public:
  // Initializer + Constructor
  explicit System(const Num &N_t, const Num &N_r, const Num &T, const Num &M);

  // Copy Constructor (banned)
  System(const System &rhs) = delete;

  // Assignment Operator (banned)
  System &operator=(const System &rhs) = delete;

  // Deconstructor (delete each user)
  ~System() = default;

private:
  // Generate the Rayleigh fading matrix and noise matrix
  // Separate function to de-clutter constructor function
  void GenerateChannelConditions();

  // Converts an integer to QAM
  // TODO

  // Converts QAM to integere
  // TODO

  // Generates X from users
  // TODO

  void TestRandom();

  Num N_t;
  Num N_r;
  Num T;
  Num M;
  std::vector<User> Users;
  Eigen::MatrixXcd H;
  Eigen::MatrixXcd z;
};

#endif // ITERATIVE_MIMO_PROJECT_SYSTEM_H
