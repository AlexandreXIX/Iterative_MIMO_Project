// @author Alexandre P.J. Dixneuf

#include "../include/User.h"
#include <iostream>
#include <random>

User::User(const Num &time, const Num &dim) : time(time), Data(time) {
  // Initialize a random number generator
  std::random_device rd;  // a seed source for the random number engine
  std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> distrib(1, dim);
  // Now for each value in the data stream, assign some random value between 1
  // and dim (dim = M in M-QAM)
  for (Num &sample : Data) {
    sample = distrib(gen);
  }
}

std::vector<Num> User::getData() const { return Data; }

double User::verify(const std::vector<Num> &testVector) const {
  uint32_t testSize = testVector.size();
  if (testSize != time) {
    std::cerr << "test vector size mismatch" << std::endl;
    return 0;
  }
  double accuracy = 0;
  for (uint32_t i = 0; i < testSize; i++) {
    if (testVector[i] == Data[i]) {
      accuracy++;
    }
  }
  return accuracy / testSize;
}

#include "../include/User.h"
