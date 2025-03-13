// @author Alexandre P.J. Dixneuf

#ifndef ITERATIVE_MIMO_PROJECT_SYSTEM_H
#define ITERATIVE_MIMO_PROJECT_SYSTEM_H

#include "User.h"
typedef int Num; // can switch to uint32_t if necessary

class System {
public:
  // Initializer + Constructor
  explicit System(const Num &userCount, const Num &receiverCount,
                  const Num &time, const Num &dim);

  // Copy Constructor (banned)
  System(const System &rhs) = delete;

  // Assignment Operator (banned)
  System &operator=(const System &rhs) = delete;

  // Deconstructor (delete each user)
  ~System() = default;

private:
  Num userCount;
  Num receiverCount;
  Num time;
  Num dim;
  std::vector<User> Users;
};

#endif // ITERATIVE_MIMO_PROJECT_SYSTEM_H
