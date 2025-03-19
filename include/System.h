// @author Alexandre P.J. Dixneuf

#ifndef SYSTEM_H
#define SYSTEM_H

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

  // Takes all the Users' messages, compiles it into matrix X (ints)
  void GenerateXInt();

  // Generate the Gray-Code, store to keep consistent
  // Note: exact encoding not too important, consistency is
  void GenerateGrayCode();

  // Helper function
  // Converts individual ints to QAM
  std::complex<double> IntToQAM(Num int_value);

  // Creates a gray-code QAM constellation
  void QAMGenerator();

  // Takes X in integers and converts to QAM (gray-code)
  void GenerateXQAM();

  // Converts QAM to integer
  // TODO

  // Generates X from users
  // TODO

  void TestRandom();

  Num N_t;
  Num N_r;
  Num T;
  Num M;
  Num N;
  std::vector<User> Users;
  Eigen::MatrixXcd H;
  Eigen::MatrixXi X_int;
  Eigen::MatrixXcd X_QAM;
  Eigen::MatrixXcd z;
  std::vector<std::vector<Num>> GrayCodeGrid;
};

#endif // SYSTEM_H
