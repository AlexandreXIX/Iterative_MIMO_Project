// @author Alexandre P.J. Dixneuf

#ifndef ITERATIVE_MIMO_PROJECT_USER_H
#define ITERATIVE_MIMO_PROJECT_USER_H

#include <cstdint>

typedef uint32_t Num;

class User {
public:
  // Initializer + Constructor
  explicit User(const Num &time, const Num &dim);

  // Copy Constructor (banned)
  User(const User &rhs) = delete;

  // Assignment Operator (banned)
  User &operator=(const User &rhs) = delete;

  // Deconstructor (default)
  ~User() = default;

private:
  // Data members
  Num time;
  std::vector <Num> Data;
};

#endif // ITERATIVE_MIMO_PROJECT_USER_H
