// @author Alexandre P.J. Dixneuf

#ifndef USER_H
#define USER_H

#include <iostream>
#include <random>
#include <vector>

typedef int Num; // can switch to uint32_t if necessary

class User {
public:
  // Initializer + Constructor
  explicit User(const Num &T, const Num &M);

  // Copy Constructor (banned)
  User(const User &rhs) = default;

  // Assignment Operator (banned)
  User &operator=(const User &rhs) = delete;

  // Deconstructor (default)
  ~User() = default;

  // Returns the data within User
  std::vector<Num> copyData() const;

  // Returns a pointer to the data within User
  std::vector<Num> &getData();

  // Given a data stream, compare to stored data and return accuracy rate
  double verify(const std::vector<Num> &testVector) const;

private:
  // Data members
  Num T;
  std::vector<Num> Data;
};

#endif // USER_H
