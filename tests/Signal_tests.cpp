// @author Alexandre P.J. Dixneuf

#include "MySignal.h"
#include <iostream>

// Helper function to print out test results
std::string OutputTestResults(const bool testResult) {
  if (testResult) {
    return "Passed";
  }
  return "Failed";
}

// Verify that the signal only inputs real values (despite being complex double)
// within the range 0 to M-1
bool Test1() {
  // Smaller for loop since we are iterating through every single value in the
  // matrix
  for (int N_t = 1; N_t <= 10; ++N_t) {
    // Technically MySignal uses only N_t, T, and M, but might as well do N_r
    // just in case
    for (int N_r = 1; N_r <= 10; ++N_r) {
      for (int T = 1; T <= 10; T = T + 100) {
        for (int M = 4; M <= 2048; M = M * 4) {
          ProblemParameters p(N_t, N_r, T, M);
          MySignal signal(&p);
          MatrixType myMatrix = signal.CopyData();
          for (int i = 0; i < myMatrix.rows(); i++) {
            for (int j = 0; j < myMatrix.cols(); j++) {
              if ((myMatrix(i, j).real() < 0) || (myMatrix(i, j).real() >= M) ||
                  (myMatrix(i, j).imag() != 0)) {
                return false;
              }
            }
          }
        }
      }
    }
  }
  return true;
}

// Verify that copying a MySignal generates a new object but the same
// ProblemParameters pointer (assignment + copy)
bool Test2() {
  constexpr int N_t = 1000, N_r = 1000, T = 10, M = 256;
  // This test is independent of matrix size, so here are default test sizes
  const ProblemParameters p(N_t, N_r, T, M);
  const MySignal signal1(&p);
  MySignal signal2 = signal1;
  if (const ProblemParameters *firstPointer = signal1.GetParameters();
      (firstPointer != &p) || !signal1.SameParameters(firstPointer)) {
    return false;
  }
  return true;
}

// Verify that CopyData functions as intended as a deep-copy, and changing the
// copy does not change the original On the other hand, verify that GetData
// points to the data, and thus a change would change the object
bool Test3() {
  constexpr int N_t = 1000, N_r = 1000, T = 10, M = 256;
  const ProblemParameters p(N_t, N_r, T, M);
  MySignal signal(&p);
  MatrixType myMatrix = signal.CopyData();
  // This will also be tested later, but additional tests always useful
  if (signal.VerifyAccuracy(myMatrix) != 1.0) {
    return false;
  }
  MatrixType copyMatrix = signal.CopyData();
  myMatrix(0, 0) = std::complex<double>(M, M);
  // Check that the altered one is no longer 100% match, but copy still is, and
  // they do differ
  if ((signal.VerifyAccuracy(myMatrix) == 1.0) ||
      (signal.VerifyAccuracy(copyMatrix) != 1.0) || (myMatrix == copyMatrix)) {
    return false;
  }
  return true;
}

// Check that an object copied, one copy has alter data and the other change the
// GetData, will be identical
bool Test4() {
  // Easy Mode:
  // constexpr int N_t = 2, N_r = 2, T = 2, M = 16;
  // Hard Mode:
  constexpr int N_t = 1000, N_r = 1000, T = 10, M = 256;
  const ProblemParameters p(N_t, N_r, T, M);
  MySignal originalSignal(&p);
  // Assignment Operator + CopyData
  MySignal firstCopy = originalSignal;
  MatrixType firstMatrix = originalSignal.CopyData();
  // Copy Constructor + GetData
  MySignal secondCopy(originalSignal);
  MatrixType *secondMatrix = secondCopy.GetData();
  // Change the fist
  firstMatrix(0, 0) = std::complex<double>(M, M);
  firstCopy.AlterData(firstMatrix);
  // The second is from Get, so no need to AlterData
  (*secondMatrix)(0, 0) = std::complex<double>(M, M);
  // Now the checks (separate to assist debugging and documentation)
  // First check the two matrices are the same
  if (firstMatrix != *secondMatrix) {
    return false;
  }
  // Now check the two signals are the same
  if (firstCopy.VerifyAccuracy(secondCopy) != 1.0) {
    return false;
  }
  // Lastly, check neither is the original
  if ((firstCopy.VerifyAccuracy(originalSignal) == 1.0) ||
      (secondCopy.VerifyAccuracy(originalSignal) == 1.0)) {
    return false;
  }
  return true;
}

// Test VerifyAccuracy for a MySignal and matrix by slowly altering a copied
// matrix and copied signal to check both versions
bool Test5() {
  // Slightly smaller since I'm iterating through every entry multiple times
  constexpr int N_t = 500, N_r = 500, T = 10, M = 256;
  const ProblemParameters p(N_t, N_r, T, M);
  MySignal originalSignal(&p);
  MySignal alteredSignal = originalSignal;
  MatrixType myMatrix = originalSignal.CopyData();
  constexpr double DataCount = N_t * T;
  double accuracy = DataCount;
  // Added error buffer for computer arithmetic
  for (int i = 0; i < N_t; i++) {
    for (int j = 0; j < T; j++) {
      constexpr double acceptableError = 0.0001;
      // First edit matrix
      myMatrix(i, j) = std::complex<double>(M, M);
      // Then edit signal
      MatrixType *tmpMatrix = alteredSignal.GetData();
      (*tmpMatrix)(i, j) = std::complex<double>(M, M);
      // Now verify accuracy (add buffer due to computer arithmetic)
      --accuracy;
      if ((originalSignal.VerifyAccuracy(myMatrix) >=
           (accuracy / DataCount) + acceptableError) ||
          (originalSignal.VerifyAccuracy(myMatrix) <=
           (accuracy / DataCount) - acceptableError)) {
        return false;
      }
      if ((originalSignal.VerifyAccuracy(alteredSignal) >=
           (accuracy / DataCount) + acceptableError) ||
          (originalSignal.VerifyAccuracy(alteredSignal) <=
           (accuracy / DataCount) - acceptableError)) {
        return false;
      }
    }
  }
  return true;
}

// Test SameParameters by generating two object with different parameters, two
// with the same, and one with a copy
bool Test6() {
  constexpr int N_t = 2, N_r = 2, T = 5, M = 16;
  const ProblemParameters p1(N_t, N_r, T, M);
  // should check address of parameters, not values
  const ProblemParameters p2(N_t, N_r, T, M);
  MySignal firstSignal(&p1);
  MySignal secondSignal(&p2);
  MySignal thirdSignal(firstSignal);
  MySignal fourthSignal = secondSignal;
  // first and third match, second and fourth, but not first and fourth or
  // second and third Also try different combinations of SameParams and
  // GetParams
  if (firstSignal.SameParameters(secondSignal.GetParameters())) {
    return false;
  }
  if (secondSignal.SameParameters(firstSignal.GetParameters())) {
    return false;
  }
  if (!firstSignal.SameParameters(thirdSignal.GetParameters())) {
    return false;
  }
  if (!thirdSignal.SameParameters(firstSignal.GetParameters())) {
    return false;
  }
  if (firstSignal.SameParameters(fourthSignal.GetParameters())) {
    return false;
  }
  if (fourthSignal.SameParameters(firstSignal.GetParameters())) {
    return false;
  }
  if (secondSignal.SameParameters(thirdSignal.GetParameters())) {
    return false;
  }
  if (thirdSignal.SameParameters(secondSignal.GetParameters())) {
    return false;
  }
  if (!secondSignal.SameParameters(fourthSignal.GetParameters())) {
    return false;
  }
  if (!fourthSignal.SameParameters(secondSignal.GetParameters())) {
    return false;
  }
  if (thirdSignal.SameParameters(fourthSignal.GetParameters())) {
    return false;
  }
  if (fourthSignal.SameParameters(thirdSignal.GetParameters())) {
    return false;
  }
  return true;
}

// Test the exception throwing of VerifyData for matrix sizes
bool Test7() {
  constexpr int N_t = 1000, N_r = 1000, T = 100, M = 256;
  const ProblemParameters p1(N_t, N_r, T, M);
  const ProblemParameters p2(N_t + 1, N_r + 1, T + 1, M + 1);
  MySignal signal1(&p1);
  MySignal signal2(&p2);
  try {
    signal1.VerifyAccuracy(signal2);
    return false;
  } catch (std::invalid_argument &) {
    return true;
  }
}

int main() {
  std::cout << "Signal Tests: " << std::endl;
  std::cout << "Test 1: " << OutputTestResults(Test1()) << std::endl;
  std::cout << "Test 2: " << OutputTestResults(Test2()) << std::endl;
  std::cout << "Test 3: " << OutputTestResults(Test3()) << std::endl;
  std::cout << "Test 4: " << OutputTestResults(Test4()) << std::endl;
  std::cout << "Test 5: " << OutputTestResults(Test5()) << std::endl;
  std::cout << "Test 6: " << OutputTestResults(Test6()) << std::endl;
  std::cout << "Test 7: " << OutputTestResults(Test6()) << std::endl;
}
