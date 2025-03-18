// @author Alexandre P.J. Dixneuf

#include "User.h"

User::User(const Num &T, const Num &M) : T(T), Data(T) {
  // Initialize a random number generator
  std::random_device rd;  // a seed source for the random number engine
  std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> distrib(1, M);
  // Now for each value in the data stream, assign some random value between 1
  // and dim (dim = M in M-QAM)
  for (Num &sample : Data) {
    sample = distrib(gen);
  }
}

std::vector<Num> User::copyData() const { return Data; }

std::vector<Num> &User::getData() { return Data; }

double User::verify(const std::vector<Num> &testVector) const {
  Num const testSize = testVector.size();
  if (testSize != T) {
    throw std::length_error(
        "Array sizes must match for comparison: " + std::to_string(testSize) +
        " vs " + std::to_string(T));
  }
  double accuracy = 0;
  for (Num i = 0; i < testSize; ++i) {
    if (testVector[i] == Data[i]) {
      ++accuracy;
    }
  }
  return accuracy / testSize;
}
