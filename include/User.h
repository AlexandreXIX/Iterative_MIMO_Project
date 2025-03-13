// @author Alexandre P.J. Dixneuf

#ifndef ITERATIVE_MIMO_PROJECT_USER_H
#define ITERATIVE_MIMO_PROJECT_USER_H

#include <vector>

typedef int Num; // can switch to uint32_t if necessary

class User {
public:
  // Initializer + Constructor
  explicit User(const Num &time, const Num &dim);

  // Copy Constructor (banned)
  User(const User &rhs) = default;

  // Assignment Operator (banned)
  User &operator=(const User &rhs) = delete;

  // Deconstructor (default)
  ~User() = default;

  // Returns the data within User
  std::vector<Num> getData() const;

  // Given a data stream, compare to stored data and return accuracy rate
  double verify(const std::vector<Num> &testVector) const;

private:
  // Data members
  Num time;
  std::vector<Num> Data;
};

#endif // ITERATIVE_MIMO_PROJECT_USER_H
