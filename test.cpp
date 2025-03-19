// @author Alexandre P.J. Dixneuf
#include "System.h"
#include "User.h"

typedef int Num;
using namespace std;

// This function tests the User class
// True = Pass, False = Fail
bool testUsers() {
  bool testResult = true;
  // Run the test on a large sample of time slots and QAM constellation values
  for (Num t = 2; t <= 100; ++t) {
    for (Num m = 4; m <= 2048; m = m * 4) {
      User const User1(t, m);

      // Test 1:
      // Each user will contain T data points, all between 1 and M
      std::vector<Num> myData = User1.copyData();
      for (Num const &sample : myData) {
        if (sample < 0 || sample > m - 1) {
          testResult = false; // for debugging, can add a breakpoint here
        }
      }

      // Test 2:
      // If I create my own copy of the data, do some operations to it that lead
      // to the final result identical to initial (simulation encoding then
      // decoding), the User will verify perfectly identical)
      for (Num &sample : myData) {
        sample = sample * 2;
        sample = sample + 2;
        sample = sample * 10;
        sample = sample / 10;
        sample = sample - 2;
        sample = sample / 2;
      }
      if (User1.copyData() != myData || User1.verify(myData) != 1) {
        testResult = false;
      }

      // Test 3:
      // If I change a value in the array, the User accuracy determines the
      // accuracy
      constexpr double errorCount = 1;
      double const expectedAccuracy = (t - errorCount) / t;
      myData[0] = m;
      if (User1.verify(myData) != expectedAccuracy) {
        double getDataTmp = User1.verify(myData);
        testResult = false;
      }

      // Test 4:
      // Changes to the copy of the User's data does not change the User
      // (properly handles const) (technically redundant) myData[0] = m
      // determined above
      if (User1.copyData()[0] == m) {
        testResult = false;
      }

      // Test 5:
      // Proper error is thrown when wrong array size is used
      myData.pop_back();
      try {
        User1.verify(myData);
        testResult = false; // code should not reach this point due to error
      } catch (std::length_error &) {
        myData.push_back(0); // just to add a line to make compiler happy
      } catch (...) {
        testResult = false; // Error thrown, but not expected error
      }

      // Test 6:
      // Test that each User is independently randomly generated
      // Only run tests when enough data points to avoid false positives (need
      // enough constellation size and time)
      if (m > 1 && t > 5) {
        if (User const User2(t, m); User1.copyData() == User2.copyData()) {
          testResult = false;
        }
      }

      // Test 7:
      // Test default copy constructor to ensure it is a deep copy
      // Required some memory viewing to verify addresses are different, so hard
      // to rerun, but passed
    }
  }
  return testResult;
}

// Simple function that cleans up main function
// Takes in a bool, and either prints Passed or Failed instead of 1 or 0
string testResult(const bool &testResult) {
  if (testResult) {
    return "Passed";
  }
  return "Failed";
}

int main() {
  cout << "User Test: " << testResult(testUsers()) << endl;
  // TODO - add more comprehensive tests
  System testSystem(3, 2, 10, 256);
  return 0;
}
