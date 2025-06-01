// @author Alexandre P.J. Dixneuf

#include "../decoders/BPDecoder.cpp"
#include "../decoders/GaBPDecoder.cpp"
#include "Decoder.h"
#include <iostream>
#include <fstream>

// Configure these values for a base test case
constexpr int base_N_t = 2;
constexpr int base_N_r = 2;
constexpr int base_T = 10;
constexpr int base_M = 16;
constexpr int base_SNR = 40;

// Basic tolerance, add this to all equality checks using double to avoid computer arithmetic errors
constexpr double tol = 1e-6;
// How many trials each simulation should run
constexpr int trial_count = 100;

// TODO - If I had more time, would reverse to binary to do a binary accuracy check

// Returns the accuracy (in decimal) of the decoded matrix compared to the true data by checking how often the complex terms are equal
double accuracyCheck_ComplexEquality(const Eigen::MatrixXcd &trueData, const Eigen::MatrixXcd &decodedData) {
  double sum = 0;
  double count = trueData.rows() * trueData.cols();
  // For each symbol in the message
  for (int i = 0; i < trueData.rows(); ++i) {
    for (int j = 0; j < trueData.cols(); ++j) {
      if (std::abs(decodedData(i, j) - trueData(i, j)) < tol) {
        ++sum;
      }
    }
  }
  return sum / count;
}

int main() {
  std::ofstream outFile("../decoder_accuracy.csv");


  // First simulation, increasing N_t
  outFile << "Change in Accuracy as N_t Increases\n";
  outFile << "N_t, GaBP Accuracy\n";
  for (int N_t = 1; N_t < 10; N_t++) {
    double GaBPaccuracy = 0;
    for (int trial = 0; trial < trial_count; ++trial) {
      const ProblemParameters p(N_t, base_N_r, base_T, base_M, base_SNR);
      // TODO - Take this all out into separate function to avoid repetition
      MySignal signal(&p);
      QAMConstellation Q(&p);
      Channel channel(&p);
      Q.QAMEncoding(signal);
      Eigen::MatrixXcd trueSignal = signal.CopyData();
      channel.ChannelPropagation(signal);
      // TODO - Take this all out into separate function to avoid repetition
      // BPDecoder BP_decoder(signal.CopyData(), &p, Q, channel);
      GaBPDecoder GaBP_decoder(signal.CopyData(), &p, Q, channel);
      // Eigen::MatrixXcd BP_decoded = BP_decoder.Run();
      Eigen::MatrixXcd GaBP_decoded = GaBP_decoder.Run();
      // TODO - accuracy check
      GaBPaccuracy += accuracyCheck_ComplexEquality(trueSignal,GaBP_decoded);
    }
    outFile << N_t << "," << GaBPaccuracy/trial_count << "\n";
  }

  // Second simulation, increasing N_r
  outFile << "Change in Accuracy as N_r Increases\n";
  outFile << "N_r, GaBP Accuracy\n";
  for (int N_r = 1; N_r < 10; N_r++) {
    double GaBPaccuracy = 0;
    for (int trial = 0; trial < trial_count; ++trial) {
      const ProblemParameters p(base_N_t, N_r, base_T, base_M, base_SNR);
      // TODO - Take this all out into separate function to avoid repetition
      MySignal signal(&p);
      QAMConstellation Q(&p);
      Channel channel(&p);
      Q.QAMEncoding(signal);
      Eigen::MatrixXcd trueSignal = signal.CopyData();
      channel.ChannelPropagation(signal);
      // TODO - Take this all out into separate function to avoid repetition
      // BPDecoder BP_decoder(signal.CopyData(), &p, Q, channel);
      GaBPDecoder GaBP_decoder(signal.CopyData(), &p, Q, channel);
      // Eigen::MatrixXcd BP_decoded = BP_decoder.Run();
      Eigen::MatrixXcd GaBP_decoded = GaBP_decoder.Run();
      // TODO - accuracy check
      GaBPaccuracy += accuracyCheck_ComplexEquality(trueSignal,GaBP_decoded);
    }
    outFile << N_r << "," << GaBPaccuracy/trial_count << "\n";
  }


  // Third simulation, increasing T
  outFile << "Change in Accuracy as T Increases\n";
  outFile << "T, GaBP Accuracy\n";
  for (int T = 1; T < 10; T++) {
    double GaBPaccuracy = 0;
    for (int trial = 0; trial < trial_count; ++trial) {
      const ProblemParameters p(base_N_t, base_N_r, T, base_M, base_SNR);
      // TODO - Take this all out into separate function to avoid repetition
      MySignal signal(&p);
      QAMConstellation Q(&p);
      Channel channel(&p);
      Q.QAMEncoding(signal);
      Eigen::MatrixXcd trueSignal = signal.CopyData();
      channel.ChannelPropagation(signal);
      // TODO - Take this all out into separate function to avoid repetition
      // BPDecoder BP_decoder(signal.CopyData(), &p, Q, channel);
      GaBPDecoder GaBP_decoder(signal.CopyData(), &p, Q, channel);
      // Eigen::MatrixXcd BP_decoded = BP_decoder.Run();
      Eigen::MatrixXcd GaBP_decoded = GaBP_decoder.Run();
      // TODO - accuracy check
      GaBPaccuracy += accuracyCheck_ComplexEquality(trueSignal,GaBP_decoded);
    }
    outFile << T << "," << GaBPaccuracy/trial_count << "\n";
  }


  // Fourth simulation, increasing M
  outFile << "Change in Accuracy as M Increases\n";
  outFile << "M, GaBP Accuracy\n";
  for (int M = 4; M <= 256; M = M * 4) {
    double GaBPaccuracy = 0;
    for (int trial = 0; trial < trial_count; ++trial) {
      const ProblemParameters p(base_N_t, base_N_r, base_T, M, base_SNR);
      // TODO - Take this all out into separate function to avoid repetition
      MySignal signal(&p);
      QAMConstellation Q(&p);
      Channel channel(&p);
      Q.QAMEncoding(signal);
      Eigen::MatrixXcd trueSignal = signal.CopyData();
      channel.ChannelPropagation(signal);
      // TODO - Take this all out into separate function to avoid repetition
      // BPDecoder BP_decoder(signal.CopyData(), &p, Q, channel);
      GaBPDecoder GaBP_decoder(signal.CopyData(), &p, Q, channel);
      // Eigen::MatrixXcd BP_decoded = BP_decoder.Run();
      Eigen::MatrixXcd GaBP_decoded = GaBP_decoder.Run();
      // TODO - accuracy check
      GaBPaccuracy += accuracyCheck_ComplexEquality(trueSignal,GaBP_decoded);
    }
    outFile << M << "," << GaBPaccuracy/trial_count << "\n";
  }


  // Fifth simulation, increasing SNR
  outFile << "Change in Accuracy as SNR Increases\n";
  outFile << "SNR, GaBP Accuracy\n";
  for (int SNR = 10; SNR <= 100; SNR = SNR + 10) {
    double GaBPaccuracy = 0;
    for (int trial = 0; trial < trial_count; ++trial) {
      const ProblemParameters p(base_N_t, base_N_r, base_T, base_M, SNR);
      // TODO - Take this all out into separate function to avoid repetition
      MySignal signal(&p);
      QAMConstellation Q(&p);
      Channel channel(&p);
      Q.QAMEncoding(signal);
      Eigen::MatrixXcd trueSignal = signal.CopyData();
      channel.ChannelPropagation(signal);
      // TODO - Take this all out into separate function to avoid repetition
      // BPDecoder BP_decoder(signal.CopyData(), &p, Q, channel);
      GaBPDecoder GaBP_decoder(signal.CopyData(), &p, Q, channel);
      // Eigen::MatrixXcd BP_decoded = BP_decoder.Run();
      Eigen::MatrixXcd GaBP_decoded = GaBP_decoder.Run();
      // TODO - accuracy check
      GaBPaccuracy += accuracyCheck_ComplexEquality(trueSignal,GaBP_decoded);
    }
    outFile << SNR << "," << GaBPaccuracy/trial_count << "\n";
  }


  outFile.close();
  std::cout << "Results written to decoder_accuracy.csv\n";
  return 0;
}
