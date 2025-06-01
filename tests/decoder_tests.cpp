// @author Alexandre P.J. Dixneuf

#include "../decoders/BPDecoder.cpp"
#include "../decoders/GaBPDecoder.cpp"
#include "Decoder.h"
#include <iostream>

// Helper function to print out test results
std::string OutputTestResults(const bool testResult) {
  if (testResult) {
    return "Passed";
  }
  return "Failed";
}

// Just test simple operation for BP decoder
bool BPTest() {
  constexpr int N_t = 2;
  constexpr int N_r = 3;
  constexpr int T = 10;
  constexpr int M = 16;
  constexpr int SNR = 10;
  const ProblemParameters p(N_t, N_r, T, M, SNR);
  MySignal original(&p);
  QAMConstellation Q(&p);
  Channel channel(&p);
  // Get copies of signal
  MySignal copy = original;
  // Encode with QAM
  Q.QAMEncoding(original);
  Q.QAMEncoding(copy);
  Eigen::MatrixXcd trueSignal = original.CopyData();
  std::cout << "True: " << std::endl;   // TODO - remove after bug-fixing
  std::cout << trueSignal << std::endl; // TODO - remove after bug-fixing
  // Get second channel to propagate
  channel.ChannelPropagation(copy);
  BPDecoder decoder(copy.CopyData(), &p, Q, channel);
  Eigen::MatrixXcd decoded = decoder.Run();
  std::cout << "Decoded: " << std::endl; // TODO - remove after bug-fixing
  std::cout << decoded << std::endl;     // TODO - remove after bug-fixing
  // Accuracy testing
  double sum = 0;
  double count = decoded.rows() * decoded.cols();
  double tol = 1e-6;
  // For each symbol in the message
  for (int i = 0; i < decoded.rows(); ++i) {
    for (int j = 0; j < decoded.cols(); ++j) {
      // Find the closest signal (HARD)
      // TODO - find the closest signal
      // convert to bits (easy)
      // TODO - convert to bits
      // For each bit, find if equal to original
      // TODO - change if statement to test the individual bits
      if (std::abs(decoded(i, j) - trueSignal(i, j)) < tol) {
        ++sum;
      }
    }
  }
  std::cout << "Accuracy: " << std::endl;
  std::cout << (sum / count) * 100 << "%" << std::endl;
  return true;
}

// Just test simple operation for GaBP decoder
bool GaBPTest() {
  constexpr int N_t = 2;
  constexpr int N_r = 3;
  constexpr int T = 10;
  constexpr int M = 16;
  constexpr int SNR = 40;
  const ProblemParameters p(N_t, N_r, T, M, SNR);
  MySignal original(&p);
  QAMConstellation Q(&p);
  Channel channel(&p);
  // Get copies of signal
  MySignal copy = original;
  // Encode with QAM
  Q.QAMEncoding(original);
  Q.QAMEncoding(copy);
  Eigen::MatrixXcd trueSignal = original.CopyData();
  std::cout << "True: " << std::endl;   // TODO - remove after bug-fixing
  std::cout << trueSignal << std::endl; // TODO - remove after bug-fixing
  // Get second channel to propagate
  channel.ChannelPropagation(copy);
  GaBPDecoder decoder(copy.CopyData(), &p, Q, channel);
  Eigen::MatrixXcd decoded = decoder.Run();
  std::cout << "Decoded: " << std::endl; // TODO - remove after bug-fixing
  std::cout << decoded << std::endl;     // TODO - remove after bug-fixing
  // Accuracy testing
  double sum = 0;
  double count = decoded.rows() * decoded.cols();
  double tol = 1e-6;
  // For each symbol in the message
  for (int i = 0; i < decoded.rows(); ++i) {
    for (int j = 0; j < decoded.cols(); ++j) {
      // Find the closest signal (HARD)
      // TODO - find the closest signal
      // convert to bits (easy)
      // TODO - convert to bits
      // For each bit, find if equal to original
      // TODO - change if statement to test the individual bits
      if (std::abs(decoded(i, j) - trueSignal(i, j)) < tol) {
        ++sum;
      }
    }
  }
  std::cout << "Accuracy: " << std::endl;
  std::cout << (sum / count) * 100 << "%" << std::endl;
  return true;
}

bool Test2() { return false; }

bool Test3() { return false; }

bool Test4() { return false; }

bool Test5() { return false; }

bool Test6() { return false; }

bool Test7() { return false; }

int main() {
  std::cout << "Decoder Tests: " << std::endl;
  std::cout << "BP Test: " << OutputTestResults(BPTest()) << std::endl;
  std::cout << "GaBP Test: " << OutputTestResults(GaBPTest()) << std::endl;
}
