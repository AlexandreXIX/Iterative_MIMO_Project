// @author Alexandre P.J. Dixneuf
#include "src/User.cpp"

using namespace std;

// This function tests the User class
// True = Pass, False = Fail
bool testUsers() {
  bool testResult = true;
  // Run the test on a large sample of time slots and QAM constellation values
  for (uint32_t T = 1; T <= 100; T++) {
    for (uint32_t M = 1; M <= 2048; M = M * 2) {
      User const User1(T, M);

      // Test 1:
      // Each user will contain T data points, all between 1 and M
      std::vector<uint32_t> myData = User1.getData();
      for (Num const &sample : myData) {
        if (sample < 1 || sample > M) {
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
      if (User1.getData() != myData || User1.verify(myData) != 1) {
        testResult = false;
      }

      // Test 3:
      // If I change a value in the array, the User accuracy determines the
      // accuracy
      constexpr double errorCount = 1;
      double const expectedAccuracy = (T - errorCount) / T;
      myData[0] = 0;
      if (User1.verify(myData) != expectedAccuracy) {
        testResult = false;
      }

      // Test 4:
      // Changes to the copy of the User's data does not change the User
      // (properly handles const) (technically redundant) myData[0] = 0
      // determined above
      if (User1.getData()[0] == 0) {
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
  return 0;
}
